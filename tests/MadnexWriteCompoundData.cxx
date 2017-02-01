/*!
 * \file   MadnexWriteCompoundData.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   31 janv. 2017
 */

/*!
 * \file   tests/MadnexWriteCompoundData.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17 janv. 2017
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
 * \brief structure in charge of testing the
 * `writeCompoundData` function
 */
struct MadnexWriteCompoundData final
  : public tfel::tests::TestCase
{
  MadnexWriteCompoundData()
    : tfel::tests::TestCase("Madnex","WriteCompoundData")
  {} // end of MadnexWriteCompoundData
  virtual tfel::tests::TestResult
  execute() override
  {
    this->write_file();
    //    this->read_file();
    return this->result;
  }
 private:
  void write_file(){
    using namespace madnex;
    auto f = File("MadnexWriteCompoundData.madnex",H5F_ACC_TRUNC);
    auto r = f.getRoot();
    // writeCompoundData(r,"compound data",
    // 		      12.,std::string("double value"));
    // fcstring<40>("fcstring value"),"fcstring value",
    // std::string("string value"),"string value");
  }
  // void read_file(){
  //   using namespace madnex;
  //   const auto f = File("MadnexWriteCompoundData.madnex",H5F_ACC_RDONLY);
  //   const auto r = f.getRoot();
  //   const auto ecs = read<WriteCompoundData>(r,"scripts");
  //   TFEL_TESTS_ASSERT(ecs.size()==1);
  //   const auto& ec = ecs[0];
  //   TFEL_TESTS_ASSERT(ec.author=="François Curtit");
  //   TFEL_TESTS_ASSERT(ec.date=="17/01/2017");
  //   TFEL_TESTS_ASSERT(ec.command_line=="python my_script.py");
  //   TFEL_TESTS_ASSERT(ec.script_version=="");
  // }
};

TFEL_TESTS_GENERATE_PROXY(MadnexWriteCompoundData,
			  "MadnexWriteCompoundData");

int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexWriteCompoundData.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
