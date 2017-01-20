/*!
 * \file   MainResults.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#include<cstdlib>
#include<stdexcept>
#include"Madnex/MainResults.hxx"

//! a C-structure mapping the MainResult structure
template<typename char_type>
struct CMainResult{
  char_type* description = nullptr;
  char_type* label       = nullptr;
  char_type* value       = nullptr;
  char_type* unit        = nullptr;
  char_type* type        = nullptr;
};

template<typename char_type>
static H5::CompType getMainResultCompType(){
  using CEC = CMainResult<char_type>;
  H5::CompType c(sizeof(CEC));
  const auto type = madnex::getNativeType<std::string>();
  c.insertMember("description", HOFFSET(CEC,description), type);
  c.insertMember("label",HOFFSET(CEC,label),type);
  c.insertMember("value",HOFFSET(CEC,value),type);
  c.insertMember("unit", HOFFSET(CEC,unit), type);
  c.insertMember("type", HOFFSET(CEC,type), type);
  return c;
} // end of getMainResultCompType

namespace madnex{
  
  MainResult::MainResult() = default;
  MainResult::MainResult(MainResult&&) = default;
  MainResult::MainResult(const MainResult&) = default;
  MainResult&
  MainResult::operator=(MainResult&&) = default;
  MainResult&
  MainResult::operator=(const MainResult&) = default;

  void write(Group& g,
	     const std::string& n,
	     const MainResults& o)
  {
    auto make_copy = [](const MainResult& ec){
      CMainResult<const char> r;
      r.description  = ec.description.c_str();
      r.label = ec.label.c_str();
      r.value = ec.value.c_str();
      r.unit  = ec.unit.c_str();
      r.type  = ec.type.c_str();
      return std::move(r);
    };
    const auto c = getMainResultCompType<const char>();
    std::vector<CMainResult<const char>> cecs;
    cecs.reserve(o.size());
    for(const auto& ec: o){
      cecs.emplace_back(make_copy(ec));
    }
    hsize_t dim[] = {o.size()};   /* Dataspace dimensions */
    const auto d = g.createDataSet(n,c,DataSpace(1,dim));
    d.write(cecs.data(),c);
  } // end of write

  void read(MainResults& o,
	    const Group& g,
	    const std::string& n)
  {
    try{
      std::vector<CMainResult<char>> cecs;
      const auto c = getMainResultCompType<char>();
      const auto d = openDataSet(g,n);
      const auto s = d.getSpace();
      if(s.getSimpleExtentNdims()!=1){
	throw(std::runtime_error("madnex::read: "
				 "invalid dimension for "
				 "MainResults '"+n+"'"));
      }
      hsize_t dims[1];
      s.getSimpleExtentDims(dims);
      o.reserve(dims[0]);
      cecs.resize(dims[0]);
      d.read(cecs.data(),c);
      for(auto& cec: cecs){
	MainResult ec;
	ec.description  = cec.description;
	ec.label = cec.label;
	ec.value = cec.value;
	ec.unit  = cec.unit;
	ec.type  = cec.type;
	std::free(cec.description);
	std::free(cec.label);
	std::free(cec.value);
	std::free(cec.unit);
	std::free(cec.type);
	o.emplace_back(std::move(ec));
      }
    } catch (Exception& e){
      throw(std::runtime_error(e.getDetailMsg()));
    }
  }
  
} // end of namespace madnex
