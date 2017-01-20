/*! 
 * \file   include/Madnex/HDF5.ixx
 * \brief
 * \author Helfer Thomas
 * \brief  09 fév 2011
 */

#ifndef LIB_MADNEX_HDF5_IXX_
#define LIB_MADNEX_HDF5_IXX_ 

#include"Madnex/TypeName.hxx"

namespace madnex
{

  template<typename T>
  T read(const Group& g,
	 const std::string& d)
  {
    T x;
    read(x,g,d);
    return x;
  } // end of read

  template<typename T>
  T getAttributeValue(const Group& g,
		      const std::string& n)
  {
    T r;
    getAttributeValue(r,g,n);
    return r;
  }

  template<typename T>
  T getAttributeValue(const DataSet& d,
		      const std::string& n)
  {
    T r;
    getAttribute(r,d,n);
    return r;
  }
  
  template<typename T>
  void writeTypeAttribute(Group& g)
  {
    writeTypeAttribute(g,TypeName<T>::name);
  } // end of writeTypeAttribute

  template<typename T>
  void writeTypeAttribute(DataSet& d)
  {
    writeTypeAttribute(d,TypeName<T>::name);
  } // end of writeTypeAttribute

  template<typename T>
  void checkTypeAttribute(const Group& g)
  {
    checkTypeAttribute(g,TypeName<T>::name);
  } // end of checkTypeAttribute

  template<typename T>
  void checkTypeAttribute(const DataSet& d)
  {
    checkTypeAttribute(d,TypeName<T>::name);
  } // end of checkTypeAttribute
  
} // end of namespace mandex

#endif /* LIB_MADNEX_HDF5_IXX */

