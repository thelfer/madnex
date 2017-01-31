/*!
 * \file   bindings/python/src/File.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   31 janv. 2017
 */

#include<memory>
#include<boost/python.hpp>
#include"Madnex/File.hxx"
#include"Madnex/MainResults.hxx"
#include"Madnex/CompoundDataArrayView.hxx"

static std::shared_ptr<madnex::File> makeFile(const std::string& f)
{
  return std::make_shared<madnex::File>(f,H5F_ACC_RDWR);		      
}

static std::vector<std::string>
listMacroResultParam(const madnex::File& f)
{
  using namespace madnex;
  const auto r = f.getRoot();
  const auto d = openDataSet(r,"resultats/resultats_principaux");
  const auto v = CompoundDataArrayView(d);
  // // main results
  return v.extract<std::string>("intitule");
}

static void writeMacroResult(const madnex::File& f,
			     const std::string& name,
			     const std::string& label,
			     const std::string& value,
			     const std::string& unit,
			     const std::string& type)
{
  using namespace madnex;
  MainResult r;
  r.name  = name;
  r.label = label;
  r.value = value;
  r.unit  = unit;
  r.type  = type;
  auto rs = read<MainResults>(f.getRoot(),
			      "resultats/resultats_principaux");
  rs.push_back(r);
  write(f.getRoot(),"resultats/resultats_principaux",rs);
} // end of writeMacroResult

void declareFile(){
  
  boost::python::class_<madnex::File>("Pivot",boost::python::no_init)
    .def("__init__",boost::python::make_constructor(makeFile))
    .def("listMacroResultParam",listMacroResultParam)
    .def("writeMacroResult",writeMacroResult)
    ;
} // end of declareFile
