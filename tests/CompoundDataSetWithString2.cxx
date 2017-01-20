/*!
 * \file   CompoundTypeWithString2.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   18/01/2017
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
struct CompoundDataSetWithString2 final
  : public tfel::tests::TestCase
{
  CompoundDataSetWithString2()
    : tfel::tests::TestCase("Madnex","CompoundDataSetWithString2")
  {} // end of CompoundDataSetWithString2
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
   * CompoundDataSetWithString2 type in
   * it.
   */
  void write_file(){
    using namespace madnex;
    auto f = File("CompoundDataSetWithString2.madnex",H5F_ACC_TRUNC);
    auto r = f.getRoot();
    // compound description
    H5::CompType c(sizeof(CString));
    const auto stype = getNativeType<std::string>();
    c.insertMember("value", HOFFSET(CString,value),stype);
    // value to be stored
    std::string test = "my test value";
    // place holder
    CString ctest;
    ctest.value = test.c_str();
    // data set creation
    hsize_t dim[] = {1};   /* Dataspace dimensions */
    const auto d = r.createDataSet("test",c,DataSpace(1,dim));
    d.write(&ctest,c);
  }
  /*!
   * \brief this method reads the file written by the `write_file`
   * method and checks that all the data can be retrieved without loss.
   */
  void read_file(){
    using namespace madnex;
    const auto f = File("CompoundDataSetWithString2.madnex",H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    // compound description
    H5::CompType c(sizeof(CString));
    const auto stype = getNativeType<std::string>();
    c.insertMember("value", HOFFSET(CString,value),stype);
    // value to be read
    CString test;
    // reading
    const auto d = r.openDataSet("test");
    d.read(&test,c);
    std::cout << test.value << std::endl;
    TFEL_TESTS_ASSERT(std::strcmp(test.value,"my test value")==0);
  }
};

TFEL_TESTS_GENERATE_PROXY(CompoundDataSetWithString2,
			  "CompoundDataSetWithString2");

int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CompoundDataSetWithString2.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}

