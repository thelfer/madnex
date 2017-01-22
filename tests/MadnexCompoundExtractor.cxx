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

std::string fileName;

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
    this->read_file2();
    return this->result;
  }
 private:
  void write_file(){
    using namespace madnex;
    auto f = File("MadnexCompoundExtractor.madnex",H5F_ACC_TRUNC);
    auto r = f.getRoot();
    struct CompoundData{
      int   integer_value;
      float float_value;
    };
    CompoundData cdata = {12,3.14156f};
    H5::CompType c(sizeof(CompoundData));
    c.insertMember("integer_value", HOFFSET(CompoundData,integer_value),
		   getNativeType<int>());
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
    const auto f = File("MadnexCompoundExtractor.madnex",H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    CompoundExtractor e(openDataSet(r,"test"));
    TFEL_TESTS_ASSERT(e.extract<int>("integer_value")==12);
    TFEL_TESTS_ASSERT(std::abs(e.extract<float>("float_value")-3.14156f)<eps);
  }
  void read_file2(){
    using namespace madnex;
    const auto f = File(fileName,H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    CompoundExtractor e(openDataSet(r,"version"));
    const auto s = e.extract<std::string>("MAJ");
    TFEL_TESTS_ASSERT(e.extract<std::string>("MAJ")[0]=='1');
    TFEL_TESTS_ASSERT(e.extract<std::string>("MIN")[0]=='2');
    TFEL_TESTS_ASSERT(e.extract<std::string>("REL")[0]=='1');
  }
};

TFEL_TESTS_GENERATE_PROXY(MadnexCompoundExtractor,
			  "MadnexCompoundExtractor");

int main(const int argc, const char* const* argv)
{
  if(argc!=2){
    std::cerr << "MadnexReferenceFile: invalid number of arguments\n"
	      << "usage: MadnexReferenceFile reference.madnex\n";
    return EXIT_FAILURE;
  }
  fileName = argv[1];
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexCompoundExtractor.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
