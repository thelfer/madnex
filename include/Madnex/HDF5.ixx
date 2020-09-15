/*!
 * \file   include/Madnex/HDF5.ixx
 * \brief
 * \author Helfer Thomas
 * \brief  09 fév 2011
 */

#ifndef LIB_MADNEX_HDF5_IXX_
#define LIB_MADNEX_HDF5_IXX_

#include <sstream>
#include "Madnex/TypeName.hxx"

namespace madnex {

  template <typename T>
  void read_if(T& v, const Group& g, const std::string& d) {
    if (exists(g, d)) {
      read(v, g, d);
    } else {
      v = T{};
    }
  }  // end of read_if

  template <typename T>
  T read(const Group& g, const std::string& d) {
    T x;
    read(x, g, d);
    return x;
  }  // end of read

  template <typename T>
  T read_if(const Group& g, const std::string& d) {
    T x;
    read_if(x, g, d);
    return x;
  }  // end of read

  template <typename T>
  T getAttributeValue(const Group& g, const std::string& n) {
    T r;
    getAttributeValue(r, g, n);
    return r;
  }

  template <typename T>
  T getAttributeValue(const DataSet& d, const std::string& n) {
    T r;
    getAttribute(r, d, n);
    return r;
  }

  template <typename T>
  void writeTypeAttribute(Group& g) {
    writeTypeAttribute(g, TypeName<T>::name);
  }  // end of writeTypeAttribute

  template <typename T>
  void writeTypeAttribute(DataSet& d) {
    writeTypeAttribute(d, TypeName<T>::name);
  }  // end of writeTypeAttribute

  template <typename T>
  void checkTypeAttribute(const Group& g) {
    checkTypeAttribute(g, TypeName<T>::name);
  }  // end of checkTypeAttribute

  template <typename T>
  void checkTypeAttribute(const DataSet& d) {
    checkTypeAttribute(d, TypeName<T>::name);
  }  // end of checkTypeAttribute

  template <typename V>
  void write(Group& g, const std::vector<V>& o, const std::string& d) {
    Group gm(createGroup(g, d));
    writeTypeAttribute<std::vector<V>>(gm);
    auto i = typename std::vector<V>::size_type{};
    for (auto p = o.begin(); p != o.end(); ++p, ++i) {
      std::ostringstream idx;
      idx << i;
      Group gr(createGroup(gm, idx.str()));
      write(*p, idx.str(), gr);
    }
  }  // end of write

  template <typename V>
  void read(std::vector<V>& o, const std::string& d, const Group& g) {
    const auto& gm = openGroup(g, d);
    checkTypeAttribute<std::vector<V>>(gm);
    hsize_t s = gm.getNumObjs();
    hsize_t i;
    o.clear();
    o.resize(s);
    for (i = 0u; i != s; ++i) {
      std::ostringstream idx;
      idx << i;
      const auto& gr = openGroup(gm, idx.str());
      read(o[i], idx.str(), gr);
    }
  }  // end of read

  // template<typename... Args>
  // struct CompoundDataHolder;

  // template<typename Data,typename StringType,typename... Args>
  // struct CompoundDataHolder<Data,StringType,Args...>
  //   : CompoundDataHolder<Args...>
  // {
  //   using string = typename std::decay<StringType>::type;
  //   //
  //   static_assert(std::is_same<string,char*>::value||
  // 		  std::is_same<string,std::string>::value,
  // 		  "ill form argument pack");
  //   /*!
  //    * \param[in] d: data
  //    * \param[in] n: member name
  //    */
  //   CompoundDataHolder(const Data& d,const string&,Args&&... a)
  //     : CompoundDataHolder<Args...>(a...),
  //     data(d)
  //   {} // end of CompoundDataHolder
  //   //! holded data
  //   const typename std::decay<Data>::type data;
  // };

  // template<>
  // struct CompoundDataHolder<>
  // {
  //   H5::CompType ctype;
  // }; // end of struct CompoundDataHolder

  // template<typename... Args>
  // void writeCompoundData(Group& o,
  // 			 const std::string&,
  // 			 Args&&... a)
  // {
  //   CompoundDataHolder<typename std::decay<Args>::type...> h(a...);

  // }

}  // end of namespace mandex

#endif /* LIB_MADNEX_HDF5_IXX */
