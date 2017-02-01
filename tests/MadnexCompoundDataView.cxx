/*!
 * \file   tests/MadnexCompoundDataView.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17 janv. 2017
 */

#include<iostream>
#include"Madnex/CompoundDataView.hxx"

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
#include"Madnex/Types.hxx"

std::string fileName;

/*!
 * \brief structure in charge of testing read/write of the compound
 * dataset in HDF5 files.
 */
struct MadnexCompoundDataView final
  : public tfel::tests::TestCase
{
  MadnexCompoundDataView()
    : tfel::tests::TestCase("Madnex","CompoundDataView")
  {} // end of MadnexCompoundDataView
  virtual tfel::tests::TestResult
  execute() override
  {
    this->write_file();
    this->read_file();
    this->read_file2();
    return this->result;
  }
 private:
  void write_file(){
    using namespace madnex;
    auto f = File("MadnexCompoundDataView.madnex",H5F_ACC_TRUNC);
    auto r = f.getRoot();
    struct CompoundData{
      std::int32_t integer_value;
      float        float_value;
    };
    CompoundData cdata = {12,3.14156f};
    H5::CompType c(sizeof(CompoundData));
    c.insertMember("integer_value", HOFFSET(CompoundData,integer_value),
		   getNativeType<std::int32_t>());
    c.insertMember("float_value", HOFFSET(CompoundData,float_value),
		   getNativeType<float>());
    hsize_t   dimsf[1] = {1u};
    DataSpace s(1, dimsf );
    auto d = r.createDataSet("test",c,s);
    d.write(&cdata,c);
  }
  void read_file(){
    using namespace madnex;
    TFEL_CONSTEXPR const auto eps = std::numeric_limits<float>::epsilon();
    const auto f = File("MadnexCompoundDataView.madnex",H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    CompoundDataView e(openDataSet(r,"test"));
    TFEL_TESTS_ASSERT(e.extract<std::int32_t>("integer_value")==12);
    TFEL_TESTS_ASSERT(std::abs(e.extract<float>("float_value")-3.14156f)<eps);
  }
  void read_file2(){
    using namespace madnex;
    const auto f = File(fileName,H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    CompoundDataView e(openDataSet(r,"version"));
    TFEL_TESTS_ASSERT(e.extract<std::string>("MAJ")=="1");
    TFEL_TESTS_ASSERT(e.extract<std::string>("MIN")=="2");
    TFEL_TESTS_ASSERT(e.extract<std::string>("REL")=="1");
  }
};

TFEL_TESTS_GENERATE_PROXY(MadnexCompoundDataView,
			  "MadnexCompoundDataView");

int main(const int argc, const char* const* argv)
{
  if(argc!=2){
    std::cerr << "MadnexCompoundDataView: invalid number of arguments\n"
	      << "usage: MadnexCompoundDataView reference.madnex\n";
    return EXIT_FAILURE;
  }
  fileName = argv[1];
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexCompoundDataView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
