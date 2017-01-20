/*!
 * \file   tests/MadnexScripts.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17 janv. 2017
 */

#include"Madnex/Scripts.hxx"

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
 * \brief structure in charge of testing read/write of the
 * `Scripts` data structures in HDF5 files
 */
struct MadnexScripts final
  : public tfel::tests::TestCase
{
  MadnexScripts()
    : tfel::tests::TestCase("Madnex","Scripts")
  {} // end of MadnexScripts
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
    auto f = File("MadnexScripts.madnex",H5F_ACC_TRUNC);
    auto r = f.getRoot();
    Script ec;
    ec.author       = "François Curtit";
    ec.date         = "17/01/2017";
    ec.command_line = "python my_script.py";
    write(r,"scripts",std::vector<Script>(1,ec));
  }
  void read_file(){
    using namespace madnex;
    const auto f = File("MadnexScripts.madnex",H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    const auto ecs = read<Scripts>(r,"scripts");
    TFEL_TESTS_ASSERT(ecs.size()==1);
    const auto& ec = ecs[0];
    TFEL_TESTS_ASSERT(ec.author=="François Curtit");
    TFEL_TESTS_ASSERT(ec.date=="17/01/2017");
    TFEL_TESTS_ASSERT(ec.command_line=="python my_script.py");
    TFEL_TESTS_ASSERT(ec.script_version=="");
  }
};

TFEL_TESTS_GENERATE_PROXY(MadnexScripts,
			  "MadnexScripts");

int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexScripts.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
