/*!
 * \file   bindings/python/src/File.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   31 janv. 2017
 */

#include<memory>
#include<algorithm>
#include<boost/python.hpp>
#include"Madnex/File.hxx"
#include"Madnex/Exceptions.hxx"
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

static void writeMacroResult(madnex::File& f,
			     const std::string& name,
			     const std::string& label,
			     const std::string& value,
			     const std::string& unit,
			     const std::string& type)
{
  using namespace madnex;
  auto g = f.getRoot();
  MainResult nr;
  nr.name  = name;
  nr.label = label;
  nr.value = value;
  nr.unit  = unit;
  nr.type  = type;
  auto rs = read<MainResults>(g,MainResults::path);
  if(std::find_if(rs.begin(),rs.end(),[&name](const MainResult& r){
	return r.name == name;
      })!=rs.end()){
    throw(RuntimeError("integral result '"+name+"' already defined"));
  }
  rs.push_back(nr);
  write(g,MainResults::path,rs);
} // end of writeMacroResult

void declareFile(){
  
  boost::python::class_<madnex::File>("Pivot",boost::python::no_init)
    .def("__init__",boost::python::make_constructor(makeFile))
    .def("listMacroResultParam",listMacroResultParam,
	 "return the list of integral results")
    .def("writeMacroResult",writeMacroResult,
	 (boost::python::arg("name"),
	  boost::python::arg("label"),
	  boost::python::arg("value"),
	  boost::python::arg("unit"),
	  boost::python::arg("type")),
	 "add an integral result")
    ;
} // end of declareFile
