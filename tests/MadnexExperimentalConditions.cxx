/*!
 * \file   tests/MadnexExperimentalConditions.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17 janv. 2017
 */

#include"Madnex/ExperimentalConditions.hxx"

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
 * `ExperimentalConditions` data structures in HDF5 files
 */
struct MadnexExperimentalConditions final
  : public tfel::tests::TestCase
{
  MadnexExperimentalConditions()
    : tfel::tests::TestCase("Madnex","ExperimentalConditions")
  {} // end of MadnexExperimentalConditions
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
    auto f = File("MadnexExperimentalConditions.madnex",H5F_ACC_TRUNC);
    auto r = f.getRoot();
    ExperimentalCondition ec;
    ec.name  = "Temperature";
    ec.value = "273.15";
    ec.unit  = "K";
    write(r,"experimental_conditions",std::vector<ExperimentalCondition>(1,ec));
  }
  void read_file(){
    using namespace madnex;
    const auto f = File("MadnexExperimentalConditions.madnex",H5F_ACC_RDONLY);
    const auto r = f.getRoot();
    const auto ecs = read<ExperimentalConditions>(r,"experimental_conditions");
    TFEL_TESTS_ASSERT(ecs.size()==1);
    const auto& ec = ecs[0];
    TFEL_TESTS_ASSERT(ec.name=="Temperature");
    TFEL_TESTS_ASSERT(ec.value=="273.15");
    TFEL_TESTS_ASSERT(ec.unit=="K");
  }
};

TFEL_TESTS_GENERATE_PROXY(MadnexExperimentalConditions,
			  "MadnexExperimentalConditions");

int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexExperimentalConditions.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
