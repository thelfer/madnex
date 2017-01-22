/*!
 * \file   Version.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 janv. 2017
 */

#include<cstdlib>
#include<stdexcept>
#include"Madnex/Types.hxx"
#include"Madnex/Version.hxx"

//! a C-structure mapping the Version structure
template<typename char_type>
struct CVersion{
  char_type* major    = nullptr;
  char_type* minor    = nullptr;
  char_type* revision = nullptr;
};

template<typename char_type>
static H5::CompType getVersionCompType(){
  using CEC = CVersion<char_type>;
  H5::CompType c(sizeof(CEC));
  const auto type = madnex::getNativeType<std::string>();
  c.insertMember("major", HOFFSET(CEC,major), type);
  c.insertMember("minor",HOFFSET(CEC,minor),type);
  c.insertMember("revision",HOFFSET(CEC,revision),type);
  return c;
} // end of getVersionCompType

namespace madnex{
  
  Version::Version() = default;
  Version::Version(Version&&) = default;
  Version::Version(const Version&) = default;
  Version&
  Version::operator=(Version&&) = default;
  Version&
  Version::operator=(const Version&) = default;

  void write(Group& g,
	     const std::string& n,
	     const Version& o)
  {
    CVersion<const char> r;
    r.major    = o.major.c_str();
    r.minor    = o.minor.c_str();
    r.revision = o.revision.c_str();
    const auto c = getVersionCompType<const char>();
    hsize_t dim[] = {1};   /* Dataspace dimensions */
    const auto d = g.createDataSet(n,c,DataSpace(1,dim));
    d.write(&r,c);
  } // end of write

  void read(Version& o,
	     const Group& g,
	     const std::string& n)
  {
    auto alloc_if = [](const StrType& st) -> char* {
      const auto s = st.getSize();
      if(s!=H5T_VARIABLE){
	const auto ptr = static_cast<char *>(std::malloc(s*sizeof(char)));
	if(ptr==nullptr){
	  throw(std::bad_alloc());
	}
	return ptr;
      }
      return nullptr;
    };    
    CVersion<char> co;
    const auto d = openDataSet(g,n);
    //    const auto c = H5::CompType(d);
    const auto c = getVersionCompType<char>();
    const auto s = d.getSpace();
    if(s.getSimpleExtentNdims()!=1){
      throw(std::runtime_error("madnex::read: "
			       "invalid dimension for "
			       "Script '"+n+"'"));
    }
    hsize_t dims[1];
    s.getSimpleExtentDims(dims);
    if(dims[0]!=1){
      std::runtime_error("madnex::read: invalid "
			 "structure dimension for "
			 "Version '"+n+"'");
    }
    co.major    = alloc_if(c.getMemberStrType(0));
    co.minor    = alloc_if(c.getMemberStrType(1));
    co.revision = alloc_if(c.getMemberStrType(2));
    d.read(&co,c);
    o.major    = co.major;
    o.minor    = co.minor;
    o.revision = co.revision;
    std::free(co.major);
    std::free(co.minor);
    std::free(co.revision);
  }
  
} // end of namespace madnex
