/*!
 * \file   MadnexCompoundDataArrayView.cxx
 * \brief    
 * \author Helfer Thomas 
 * \date   30 janv. 2017
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
#include"Madnex/Types.hxx"
#include"Madnex/FCString.hxx"
#include"Madnex/CompoundDataArrayView.hxx"

std::string fileName;

/*!
 * \brief structure in charge of testing the `File` class.
 */
struct MadnexCompoundDataArrayView final
  : public tfel::tests::TestCase
{
  MadnexCompoundDataArrayView()
    : tfel::tests::TestCase("Madnex","CompoundDataArrayView")
  {} // end of MadnexCompoundDataArrayView
  virtual tfel::tests::TestResult
  execute() override
  {
    using namespace madnex;
    const auto f  = File(fileName,H5F_ACC_RDONLY);
    const auto r  = f.getRoot();
    const auto d  = openDataSet(r,"resultats/resultats_principaux");
    const auto v  = CompoundDataArrayView(d);
    // // main results
    const auto l  = v.extract<fcstring<32>>("intitule");
    TFEL_TESTS_ASSERT(l.size()==3u);
    TFEL_TESTS_ASSERT(l[0]=="P1");
    TFEL_TESTS_ASSERT(l[1]=="P2");
    TFEL_TESTS_ASSERT(l[2]=="P3");
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(MadnexCompoundDataArrayView,
			  "MadnexCompoundDataArrayView");

int main(const int argc, const char* const* argv)
{
  if(argc!=2){
    std::cerr << "MadnexCompoundDataArrayView: invalid number of arguments\n"
	      << "usage: MadnexCompoundDataArrayView reference.madnex\n";
    return EXIT_FAILURE;
  }
  fileName = argv[1];
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("MadnexCompoundDataArrayView.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
