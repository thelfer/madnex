/*!
 * \file   ExperimentalConditions.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#include<cstdlib>
#include<stdexcept>
#include"Madnex/ExperimentalConditions.hxx"

//! a C-structure mapping the ExperimentalCondition structure
template<typename char_type>
struct CExperimentalCondition{
  char_type* name  = nullptr;
  char_type* label = nullptr;
  char_type* value = nullptr;
  char_type* unit  = nullptr;
  char_type* type  = nullptr;
};

template<typename char_type>
static H5::CompType getExperimentalConditionCompType(){
  using CEC = CExperimentalCondition<char_type>;
  H5::CompType c(sizeof(CEC));
  const auto type = madnex::getNativeType<std::string>();
  c.insertMember("name", HOFFSET(CEC,name), type);
  c.insertMember("label",HOFFSET(CEC,label),type);
  c.insertMember("value",HOFFSET(CEC,value),type);
  c.insertMember("unit", HOFFSET(CEC,unit), type);
  c.insertMember("type", HOFFSET(CEC,type), type);
  return c;
} // end of getExperimentalConditionCompType

namespace madnex{
  
  ExperimentalCondition::ExperimentalCondition() = default;
  ExperimentalCondition::ExperimentalCondition(ExperimentalCondition&&) = default;
  ExperimentalCondition::ExperimentalCondition(const ExperimentalCondition&) = default;
  ExperimentalCondition&
  ExperimentalCondition::operator=(ExperimentalCondition&&) = default;
  ExperimentalCondition&
  ExperimentalCondition::operator=(const ExperimentalCondition&) = default;

  void write(Group& g,
	     const std::string& n,
	     const ExperimentalConditions& o)
  {
    auto make_copy = [](const ExperimentalCondition& ec){
      CExperimentalCondition<const char> r;
      r.name  = ec.name.c_str();
      r.label = ec.label.c_str();
      r.value = ec.value.c_str();
      r.unit  = ec.unit.c_str();
      r.type  = ec.type.c_str();
      return std::move(r);
    };
    const auto c = getExperimentalConditionCompType<const char>();
    std::vector<CExperimentalCondition<const char>> cecs;
    cecs.reserve(o.size());
    for(const auto& ec: o){
      cecs.emplace_back(make_copy(ec));
    }
    hsize_t dim[] = {o.size()};   /* Dataspace dimensions */
    const auto d = g.createDataSet(n,c,DataSpace(1,dim));
    d.write(cecs.data(),c);
  } // end of write

  void read(ExperimentalConditions& o,
	     const Group& g,
	     const std::string& n)
  {
    try{
      std::vector<CExperimentalCondition<char>> cecs;
      const auto c = getExperimentalConditionCompType<char>();
      const auto d = openDataSet(g,n);
      hsize_t dims[1];
      d.getSpace().getSimpleExtentDims(dims);
      o.reserve(dims[0]);
      cecs.resize(dims[0]);
      d.read(cecs.data(),c);
      for(auto& cec: cecs){
	ExperimentalCondition ec;
	ec.name  = cec.name;
	ec.label = cec.label;
	ec.value = cec.value;
	ec.unit  = cec.unit;
	ec.type  = cec.type;
	std::free(cec.name);
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
