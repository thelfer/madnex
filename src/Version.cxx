/*!
 * \file   Version.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 janv. 2017
 */

#include<cstdlib>
#include<stdexcept>
#include"Madnex/Types.hxx"
#include"Madnex/CompoundDataView.hxx"
#include"Madnex/Version.hxx"

namespace madnex{
  
  Version::Version() = default;
  Version::Version(Version&&) = default;
  Version::Version(const Version&) = default;
  Version&
  Version::operator=(Version&&) = default;
  Version&
  Version::operator=(const Version&) = default;

  // void write(Group& g,
  // 	     const std::string& n,
  // 	     const Version& o)
  // {
  //   CVersion<const char> r;
  //   r.major    = o.major.c_str();
  //   r.minor    = o.minor.c_str();
  //   r.revision = o.revision.c_str();
  //   const auto c = getVersionCompType<const char>();
  //   hsize_t dim[] = {1};   /* Dataspace dimensions */
  //   const auto d = g.createDataSet(n,c,DataSpace(1,dim));
  //   d.write(&r,c);
  // } // end of write

  void read(Version& o,const Group& g,
	     const std::string& n)
  {
    const auto d = openDataSet(g,n);
    const auto v = CompoundDataView(d);
    extract(o.major   ,v,std::size_t(0));
    extract(o.minor   ,v,std::size_t(1));
    extract(o.revision,v,std::size_t(2));
  }
  
} // end of namespace madnex
