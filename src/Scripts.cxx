/*!
 * \file   Scripts.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#include<cstdlib>
#include<stdexcept>
#include"Madnex/Types.hxx"
#include"Madnex/Scripts.hxx"

//! a C-structure mapping the Script structure
template<typename char_type>
struct CScript{
  char_type* author         = nullptr;
  char_type* date           = nullptr;
  char_type* command_line   = nullptr;
  char_type* script_version = nullptr;
};

template<typename char_type>
static H5::CompType getScriptCompType(){
  using CEC = CScript<char_type>;
  H5::CompType c(sizeof(CEC));
  const auto type = madnex::getNativeType<std::string>();
  c.insertMember("author", HOFFSET(CEC,author), type);
  c.insertMember("date",HOFFSET(CEC,date),type);
  c.insertMember("command_line",HOFFSET(CEC,command_line),type);
  c.insertMember("script_version", HOFFSET(CEC,script_version), type);
  return c;
} // end of getScriptCompType

namespace madnex{
  
  Script::Script() = default;
  Script::Script(Script&&) = default;
  Script::Script(const Script&) = default;
  Script&
  Script::operator=(Script&&) = default;
  Script&
  Script::operator=(const Script&) = default;

  void write(Group& g,
	     const std::string& n,
	     const Scripts& o)
  {
    auto make_copy = [](const Script& ec){
      CScript<const char> r;
      r.author  = ec.author.c_str();
      r.date = ec.date.c_str();
      r.command_line = ec.command_line.c_str();
      r.script_version  = ec.script_version.c_str();
      return std::move(r);
    };
    const auto c = getScriptCompType<const char>();
    std::vector<CScript<const char>> cecs;
    cecs.reserve(o.size());
    for(const auto& ec: o){
      cecs.emplace_back(make_copy(ec));
    }
    hsize_t dim[] = {o.size()};   /* Dataspace dimensions */
    const auto d = g.createDataSet(n,c,DataSpace(1,dim));
    d.write(cecs.data(),c);
  } // end of write

  void read(Scripts& o,
	     const Group& g,
	     const std::string& n)
  {
    try{
      std::vector<CScript<char>> cecs;
      const auto c = getScriptCompType<char>();
      const auto d = openDataSet(g,n);
      const auto s = d.getSpace();
      if(s.getSimpleExtentNdims()!=1){
	throw(std::runtime_error("madnex::read: "
				 "invalid dimension for "
				 "Script '"+n+"'"));
      }
      hsize_t dims[1];
      s.getSimpleExtentDims(dims);
      o.reserve(dims[0]);
      cecs.resize(dims[0]);
      d.read(cecs.data(),c);
      for(auto& cec: cecs){
	Script ec;
	ec.author  = cec.author;
	ec.date = cec.date;
	ec.command_line = cec.command_line;
	ec.script_version  = cec.script_version;
	std::free(cec.author);
	std::free(cec.date);
	std::free(cec.command_line);
	std::free(cec.script_version);
	o.emplace_back(std::move(ec));
      }
    } catch (Exception& e){
      throw(std::runtime_error(e.getDetailMsg()));
    }
  }
  
} // end of namespace madnex
