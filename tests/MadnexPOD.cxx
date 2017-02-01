/*!
 * \file   tests/MadnexPOD.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   16/01/2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<cmath>
#include<cstdlib>
#include<limits>
#include<iostream>
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"Madnex/HDF5.hxx"
#include"Madnex/File.hxx"

/*!
 * \brief structure in charge of testing read/write on plain old data
 * types in HDF5 files
 */
struct MadnexPOD final
  : public tfel::tests::TestCase
{
  MadnexPOD()
    : tfel::tests::TestCase("Madnex","POD")
  {} // end of MadnexPOD
  virtual tfel::tests::TestResult
  execute() override
  {
    this->write_file();
    this->read_file();
    return this->result;
  }
 private:
  /*!
   * \brief this method create a file and stores various POD type in
   * it.
   */
  void write_file(){
    using namespace madnex;
    auto f = File("MadnexPOD.madnex",H5F_ACC_TRUNC);
    auto r = f.getRoot();
    write(r,"boolean_value",true);
    write(r,"char_value",'c');
    write(r,"integer_value",static_cast<std::int32_t>(12));
    write(r,"float_value",12.f);
    write(r,"double_value",12.);
    write(r,"long_double_value",static_cast<long double>(12));
    write(r,"string_value","lorem ipsum");
  }
  /*!
   * \brief this method reads the file written by the `write_file`
   * method and checks that all the data can be retrieved without loss.
   */
  void read_file(){
    using namespace madnex;
    const auto f = File("MadnexPOD.madnex",H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    TFEL_TESTS_ASSERT(read<char>(r,"char_value")=='c');
    TFEL_TESTS_ASSERT(read<std::int32_t>(r,"integer_value")==12);
    TFEL_TESTS_CHECK_THROW(read<double>(r,"boolean_value"),
			   std::runtime_error);
    this->read_floatting_point_number<float>(r,"float_value",12.f);
    this->read_floatting_point_number<double>(r,"double_value",12.);
    this->read_floatting_point_number<long double>(r,"long_double_value",12.);
    std::cout << "v: " << read<std::string>(r,"string_value") << std::endl;
    TFEL_TESTS_ASSERT(read<std::string>(r,"string_value")=="lorem ipsum");
    TFEL_TESTS_CHECK_THROW(read<double>(r,"string_value"),
    			   std::runtime_error);
  }
  /*!
   * \brief read a floatting_point value.
   * \param[in] g: group
   * \param[in] n: variable name
   * \param[in] v: expected value
   */
  template<typename T>
  void read_floatting_point_number(const madnex::Group& g,
				   const std::string& n,
				   const T v){
    const auto d = madnex::read<T>(g,n);
    TFEL_TESTS_ASSERT(std::abs(d-v)<v*std::numeric_limits<T>::epsilon());
    TFEL_TESTS_CHECK_THROW(madnex::read<std::string>(g,n),
			   std::runtime_error);
  }
  
};

TFEL_TESTS_GENERATE_PROXY(MadnexPOD,"MadnexPOD");

int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexPOD.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
