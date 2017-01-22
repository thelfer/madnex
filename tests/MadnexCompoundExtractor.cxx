/*!
 * \file   tests/MadnexCompoundExtractor.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17 janv. 2017
 */

#include"Madnex/CompoundExtractor.hxx"

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

/*!
 * \brief structure in charge of testing read/write of the
 * `CompoundExtractor` data structures in HDF5 files
 */
struct MadnexCompoundExtractor final
  : public tfel::tests::TestCase
{
  MadnexCompoundExtractor()
    : tfel::tests::TestCase("Madnex","CompoundExtractor")
  {} // end of MadnexCompoundExtractor
  virtual tfel::tests::TestResult
  execute() override
  {
    this->write_file();
    this->read_file();
    return this->result;
  }
 private:
  void write_file(){
    using namespace madnex;
    auto f = File("MadnexCompoundExtractor.madnex",H5F_ACC_TRUNC);
    auto r = f.getRoot();
    struct CompoundData{
      int value;
    };
    CompoundData cdata = {12};
    H5::CompType c(sizeof(CompoundData));
    const auto type = getNativeType<int>();
    c.insertMember("value", HOFFSET(CompoundData,value), type);
    hsize_t   dimsf[1] = {1u};
    DataSpace s(1, dimsf );
    auto d = r.createDataSet("test",c,s);
    d.write(&cdata,c);
  }
  void read_file(){
    using namespace madnex;
    const auto f = File("MadnexCompoundExtractor.madnex",H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    CompoundExtractor e(openDataSet(r,"test"));
    TFEL_TESTS_ASSERT(e.extract<int>("value")==12);
  }
};

TFEL_TESTS_GENERATE_PROXY(MadnexCompoundExtractor,
			  "MadnexCompoundExtractor");

int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexCompoundExtractor.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
