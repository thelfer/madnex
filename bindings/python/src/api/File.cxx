/*!
 * \file   bindings/python/src/File.cxx
 * \brief
 * \author Helfer Thomas
 * \date   31 janv. 2017
 */

#include <iostream>

#include <memory>
#include <fstream>
#include <algorithm>
#include <boost/python.hpp>
#include "TFEL/System/System.hxx"
#include "Madnex/File.hxx"
#include "Madnex/Exceptions.hxx"
#include "Madnex/MainResults.hxx"
#include "Madnex/GetInstallPath.hxx"
#include "Madnex/CompoundDataArrayView.hxx"

static std::shared_ptr<madnex::File> makeFile(const std::string& f) {
  std::ifstream file(f);
  if(!file){
    const auto mpath = madnex::getInstallPath();
    const auto s = tfel::system::dirStringSeparator();
    tfel::system::systemCall::copy(
        mpath + s + "share" + s + "madnex" + s + "pivot-reference.edf", f);
  }
  return std::make_shared<madnex::File>(f, H5F_ACC_RDWR);
}

static std::vector<std::string> listMacroResultParam(const madnex::File& f) {
  using namespace madnex;
  const auto r = f.getRoot();
  const auto d = openDataSet(r, "resultats/resultats_principaux");
  const auto v = CompoundDataArrayView(d);
  // // main results
  return v.extract<std::string>("intitule");
}

static void writeMacroResult(madnex::File& f,
                             const std::string& name,
                             const std::string& label,
                             const std::string& value,
                             const std::string& unit,
                             const std::string& type) {
  using namespace madnex;
  auto r = f.getRoot();
  auto g  = openGroup(r,MainResults::GroupPath);
  auto rs = MainResults{};
  if(contains(g,MainResults::DataSetName)){
    rs = read<MainResults>(g, MainResults::DataSetName);
    auto p = [&name](const MainResult& mr) { return mr.name == name; };
    if (std::find_if(rs.begin(), rs.end(),p) !=	rs.end()) {
#pragma message("Add logging support")
      //      log("integral result '" + name + "' already defined"));
    }
  }
  MainResult nr;
  nr.name = name;
  nr.label = label;
  nr.value = value;
  nr.unit = unit;
  nr.type = type;
  rs.push_back(nr);
  write(g, MainResults::DataSetName, rs);
}  // end of writeMacroResult

void declareFile();
void declareFile() {
  boost::python::class_<madnex::File>("Pivot", boost::python::no_init)
      .def("__init__", boost::python::make_constructor(makeFile))
      .def("listMacroResultParam", listMacroResultParam, "return the list of integral results")
      .def("writeMacroResult", writeMacroResult,
           (boost::python::arg("name"), boost::python::arg("libelle"), boost::python::arg("value"),
            boost::python::arg("unit"), boost::python::arg("data_type")),
           "add an integral result");
}  // end of declareFile
