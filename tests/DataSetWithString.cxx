/*!
 * \file   DataSetWithString.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 janv. 2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<cstring>
#include<cstdlib>
#include<iostream>
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"Madnex/HDF5.hxx"
#include"Madnex/File.hxx"

//! a simple example of C-struct with a string
struct CString
{
  //! pointer to a C-string
  const char* value;
}; // end of struct CString

/*!
 * \brief structure in charge of testing read/write on plain old data
 * types in HDF5 files
 */
struct DataSetWithString final
  : public tfel::tests::TestCase
{
  DataSetWithString()
    : tfel::tests::TestCase("Madnex","DataSetWithString")
  {} // end of DataSetWithString
  virtual tfel::tests::TestResult
  execute() override
  {
    this->write_file();
    this->read_file();
    return this->result;
  }
 private:
  /*!
   * \brief this method create a file and stores various
   * DataSetWithString type in
   * it.
   */
  void write_file(){
    using namespace madnex;
    auto f = File("DataSetWithString.madnex",H5F_ACC_TRUNC);
    auto r = f.getRoot();
    const auto stype = getNativeType<std::string>();
    const auto test = "my test value";
    // data set creation
    hsize_t dim[] = {1};   /* Dataspace dimensions */
    const auto d = r.createDataSet("test",stype,DataSpace(1,dim));
    d.write(&test,stype);
    f.close();
  }
  /*!
   * \brief this method reads the file written by the `write_file`
   * method and checks that all the data can be retrieved without loss.
   */
  void read_file(){
    using namespace madnex;
    auto f = File("DataSetWithString.madnex",H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    // compound description
    const auto stype = getNativeType<std::string>();
    char* test;
    // reading
    const auto d = r.openDataSet("test");
    d.read(&test,stype);
    TFEL_TESTS_ASSERT(std::strcmp(test,"my test value")==0);
    std::free(test);
    f.close();
  }
};

TFEL_TESTS_GENERATE_PROXY(DataSetWithString,
			  "DataSetWithString");

int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DataSetWithString.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}

