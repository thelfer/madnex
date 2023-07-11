/*!
 * \file   src/HDF5.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  17/01/2017
 */

#include <iostream>
#include <cstring>
#include <stdexcept>
#include "Madnex/Raise.hxx"
#include "Madnex/Types.hxx"
#include "Madnex/TypeName.hxx"
#include "Madnex/HDF5.hxx"

/*!
 * \brief macro used to implement the `read`/`write` functions for
 * plain old data types.
 */
#define MADNEX_POD_IMPLEMENTATION(X)                                        \
  void write(Group& g, const std::string& d, const X& o, const bool b) {    \
    try {                                                                   \
      if (b) {                                                              \
        unlinkIfExists(g, d);                                               \
      }                                                                     \
      hsize_t dimsf[1] = {1};                                               \
      DataSpace dataspace(1, dimsf);                                        \
      auto dataset = g.createDataSet(d, getNativeType<X>(), dataspace);     \
      dataset.write(&o, getNativeType<X>());                                \
      writeTypeAttribute<X>(dataset);                                       \
    } catch (H5::Exception & e) {                                           \
      raise(e.getDetailMsg());                                              \
    }                                                                       \
  }                                                                         \
                                                                            \
  void read(X& o, const Group& g, const std::string& d) {                   \
    try {                                                                   \
      auto dataset = openDataSet(g, d);                                     \
      checkTypeAttribute<X>(dataset);                                       \
      const auto s = dataset.getSpace();                                    \
      if (s.getSimpleExtentNdims() != 1) {                                  \
        raise(                                                              \
            "madnex::read: invalid type size\n"                             \
            "error while retreiving a '#X' in '" +                          \
            d + "'");                                                       \
      }                                                                     \
      hsize_t dims[1];                                                      \
      s.getSimpleExtentDims(dims);                                          \
      if (dims[0] != 1) {                                                   \
        raise(                                                              \
            "madnex::read: invalid type size\n"                             \
            "error while retreiving a '#X' in '" +                          \
            d + "'");                                                       \
      }                                                                     \
      dataset.read(&o, getNativeType<X>());                                 \
    } catch (H5::Exception & e) {                                           \
      raise("Error while reading '" + d + "' in group '" + g.getObjName() + \
            "': " + e.getDetailMsg());                                      \
    }                                                                       \
  }                                                                         \
                                                                            \
  template <>                                                               \
  X read(const Group& g, const std::string& d) {                            \
    X o;                                                                    \
    read(o, g, d);                                                          \
    return o;                                                               \
  }                                                                         \
                                                                            \
  void getAttributeValue(X& v, const Group& g, const std::string& n) {      \
    try {                                                                   \
      const auto attr = openAttribute(g, n);                                \
      const auto s = attr.getSpace();                                       \
      if (s.getSimpleExtentNdims() != 1) {                                  \
        raise(                                                              \
            "madnex::getAttributeValue: invalid type size\n"                \
            "error while retreiving a '#X' in group '" +                    \
            n + "'");                                                       \
      }                                                                     \
      hsize_t dims[1]; /* dataset dimensions */                             \
      s.getSimpleExtentDims(dims);                                          \
      if (dims[0] != 1) {                                                   \
        raise(                                                              \
            "madnex::getAttributeValue: invalid type size\n"                \
            "error while retreiving a '#X' in group '" +                    \
            n + "'");                                                       \
      }                                                                     \
      attr.read(getNativeType<X>(), &v);                                    \
    } catch (H5::Exception&) {                                              \
      raise("madnex::getTypeAttributeValue: can't get attribute '" + n +    \
            "' for group");                                                 \
    }                                                                       \
  }                                                                         \
                                                                            \
  void getAttributeValue(X& v, const DataSet& d, const std::string& n) {    \
    try {                                                                   \
      const auto attr = openAttribute(d, n);                                \
      const auto s = attr.getSpace();                                       \
      if (s.getSimpleExtentNdims() != 1) {                                  \
        raise(                                                              \
            "madnex::getAttributeValue: invalid type size\n"                \
            "error while retreiving a '#X' in data set '" +                 \
            n + "'");                                                       \
      }                                                                     \
      hsize_t dims[1]; /* dataset dimensions */                             \
      s.getSimpleExtentDims(dims);                                          \
      if (dims[0] != 1) {                                                   \
        raise(                                                              \
            "madnex::getAttributeValue: invalid type size\n"                \
            "error while retreiving a '#X' in data set '" +                 \
            n + "'");                                                       \
      }                                                                     \
      attr.read(getNativeType<X>(), &v);                                    \
    } catch (H5::Exception&) {                                              \
      raise("madnex::getTypeAttributeValue: can't get attribute '" + n +    \
            "' for group");                                                 \
    }                                                                       \
  }

#define MADNEX_HDF5_VECTOR_IMPLEMENTATION(X)                                 \
  template <>                                                                \
  void write(Group& g, const std::string& d, const std::vector<X>& o,        \
             const bool b) {                                                 \
    using Type = X;                                                          \
    if (b) {                                                                 \
      unlinkIfExists(g, d);                                                  \
    }                                                                        \
    try {                                                                    \
      if (o.empty()) {                                                       \
        auto c = Type();                                                     \
        hsize_t dimsf[1];                                                    \
        dimsf[0] = 1u;                                                       \
        DataSpace dataspace(1, dimsf);                                       \
        StrType datatype(getNativeType<X>());                                \
        DataSet dataset = g.createDataSet(d, datatype, dataspace);           \
        dataset.write(&c, getNativeType<X>());                               \
        /* attribute	*/                                                      \
        writeTypeAttribute<std::vector<X>>(dataset);                         \
        writeEmptyObjectAttribute(dataset);                                  \
      } else {                                                               \
        hsize_t dimsf[1];                                                    \
        dimsf[0] = o.size();                                                 \
        DataSpace dataspace(1, dimsf);                                       \
        DataSet dataset = g.createDataSet(d, getNativeType<X>(), dataspace); \
        dataset.write(&o[0], getNativeType<X>());                            \
        writeTypeAttribute<std::vector<X>>(dataset);                         \
      }                                                                      \
    } catch (H5::Exception & e) {                                            \
      raise(e.getDetailMsg());                                               \
    }                                                                        \
  }                                                                          \
                                                                             \
  template <>                                                                \
  void read(std::vector<X>& o, const Group& g, const std::string& d) {       \
    try {                                                                    \
      DataSet dataset = openDataSet(g, d);                                   \
      checkTypeAttribute<std::vector<X>>(dataset);                           \
      if (hasEmptyObjectAttribute(dataset)) {                                \
        o.clear();                                                           \
      } else {                                                               \
        DataSpace filespace = dataset.getSpace();                            \
        hsize_t dims[1];                                                     \
        filespace.getSimpleExtentDims(dims);                                 \
        o.resize(dims[0]);                                                   \
        dataset.read(&o[0], getNativeType<X>());                             \
      }                                                                      \
    } catch (H5::Exception & e) {                                            \
      raise("Error while reading '" + d + "' in group '" + g.getObjName() + "': "  \
             + e.getDetailMsg());                                            \
    }                                                                        \
  }

namespace madnex {

  static std::vector<std::string> tokenize(std::string_view s,
                                           const char c,
                                           const bool keep_empty_strings) {
    std::vector<std::string> res;
    auto b = std::string::size_type{};
    auto e = s.find_first_of(c, b);
    while (std::string::npos != e || std::string::npos != b) {
      // Found a token, add it to the vector.
      res.push_back(std::string{s.substr(b, e - b)});
      if (keep_empty_strings) {
        b = e == std::string::npos ? e : e + 1;
      } else {
        b = s.find_first_not_of(c, e);
      }
      e = s.find_first_of(c, b);
    }
    return res;
  }  // end of tokenize

  bool exists(const Group& g, const std::string& p) {
    // break paths
    const auto leafs = tokenize(p, '/', false);
    if (leafs.empty()) {
      return false;
    }
    auto cpath = std::string{};
    for (const auto& l : leafs) {
      if (!cpath.empty()) {
        cpath += '/';
      }
      cpath += l;
      const auto b = H5Lexists(g.getId(), cpath.c_str(), H5P_DEFAULT) > 0;
      if (!b) {
        return false;
      }
    }
    return true;
  }

  bool subGroupExists(const Group& g, const std::string& p) {
    if (!exists(g, p)) {
      return false;
    }
    H5O_info_t infobuf;
    auto status =
        H5Oget_info_by_name(g.getId(), p.c_str(), &infobuf, H5P_DEFAULT);
    return (status >= 0) && (infobuf.type == H5O_TYPE_GROUP);
  }

  Group createGroup(const Group& g, const std::string& n) {
    if (subGroupExists(g, n)) {
      return openGroup(g, n);
    }
    Group gr;
    try {
      gr = g.createGroup(n);
    } catch (H5::GroupIException& e) {
      raise("madnex::createGroup: creation of group '" + n + "' failed (" +
            e.getDetailMsg() + ")");
    } catch (H5::Exception& e) {
      raise("madnex::createGroup: creation of group '" + n + "' failed (" +
            e.getDetailMsg() + ")");
    }
    return gr;
  }

  Group openGroup(const Group& g, const std::string& n) {
    Group gr;
    try {
      gr = g.openGroup(n);
    } catch (H5::Exception& e) {
      raise("madnex::openGroup: opening of group '" + n + "' failed (" +
            e.getDetailMsg() + ")");
    }
    return gr;
  }

  void removeDataSet(const Group& g, const std::string& n) {
    g.unlink(n);
  }  // end of removeDataSet

  DataSet openDataSet(const Group& g, const std::string& n) {
    DataSet d;
    try {
      d = g.openDataSet(n);
    } catch (H5::Exception& e) {
      raise("madnex::openDataSet: opening of data set '" + n + "' failed (" +
            e.getDetailMsg() + ")");
    }
    return d;
  }

  Attribute openAttribute(const DataSet& d, const std::string& n) {
    Attribute a;
    try {
      a = d.openAttribute(n);
    } catch (H5::Exception& e) {
      raise("madnex::openAttribute: opening of attribute '" + n + "' failed (" +
            e.getDetailMsg() + ")");
    }
    return a;
  }

  Attribute openAttribute(const Group& g, const std::string& n) {
    Attribute a;
    try {
      a = g.openAttribute(n);
    } catch (H5::Exception& e) {
      raise("madnex::openAttribute: opening of attribute '" + n + "' failed (" +
            e.getDetailMsg() + ")");
    }
    return a;
  }

  void writeTypeAttribute(Group& g, const std::string& t) {
    try {
      const hsize_t dimsf[1] = {1};
      const auto type = getNativeType<std::string>();
      DataSpace dataspace(1, dimsf);
      Attribute attr = g.createAttribute("type", type, dataspace);
      const char* v = t.c_str();
      attr.write(type, &v);
    } catch (H5::Exception& e) {
      raise("madnex::writeTypeAttribute: operation failed (" +
            e.getDetailMsg() + ")");
    }
  }

  void writeTypeAttribute(DataSet& d, const std::string& t) {
    try {
      hsize_t dimsf[1] = {1};
      const auto type = getNativeType<std::string>();
      DataSpace dataspace(1, dimsf);
      Attribute attr = d.createAttribute("type", type, dataspace);
      const char* c = t.c_str();
      attr.write(type, &c);
    } catch (H5::Exception& e) {
      raise("madnex::writeTypeAttribute: operation failed (" +
            e.getDetailMsg() + ")");
    }
  }

  std::string getTypeAttribute(const Group& g) {
    std::string type;
    getTypeAttribute(type, g);
    return type;
  }

  void getAttributeValue(std::string& v, const Group& g, const std::string& n) {
    try {
      const auto attr = openAttribute(g, n);
      const auto s = attr.getSpace();
      if (s.getSimpleExtentNdims() != 1) {
        raise("madnex::getAttributeValue: invalid dimension for attribute '" +
              n + "'");
      }
      hsize_t dims[1];  // dataset dimensions
      s.getSimpleExtentDims(dims);
      v.clear();
      if (attr.getDataType().isVariableStr()) {
        if (dims[0] != 1) {
          raise("madnex::getAttributeValue: invalid dimension for attribute '" +
                n + "'");
        }
        char* pv;
        const auto type = getNativeType<std::string>();
        attr.read(type, &pv);
        v = pv;
        std::free(pv);
      } else {
        // using an fixed length string
        v.resize(dims[0]);
        const auto type = H5::PredType::C_S1;
        attr.read(type, &v[0]);
        if (v.back() == '\0') {
          v.pop_back();
        }
      }
    } catch (H5::Exception&) {
      raise("madnex::getTypeAttributeValue: can't get attribute '" + n +
            "' for group");
    }
  }

  template <>
  std::string getAttributeValue(const Group& g, const std::string& n) {
    std::string s;
    getAttributeValue(s, g, n);
    return s;
  }  // end of getAttribute

  void getAttributeValue(std::string& v,
                         const DataSet& d,
                         const std::string& n) {
    try {
      const auto attr = openAttribute(d, n);
      const auto s = attr.getSpace();
      if (s.getSimpleExtentNdims() != 1) {
        raise("madnex::getAttributeValue: invalid dimension for attribute '" +
              n + "'");
      }
      hsize_t dims[1];  // dataset dimensions
      s.getSimpleExtentDims(dims);
      v.clear();
      if (dims[0] != 1) {
        raise("madnex::getAttributeValue: invalid dimension for attribute '" +
              n + "'");
      }
      char* pv;
      const auto type = getNativeType<std::string>();
      attr.read(type, &pv);
      v = pv;
      std::free(pv);
    } catch (H5::Exception&) {
      raise("madnex::getTypeAttributeValue: can't get attribute '" + n +
            "' for data set");
    }
  }

  template <>
  std::string getAttributeValue(const DataSet& d, const std::string& n) {
    std::string s;
    getAttributeValue(s, d, n);
    return s;
  }  // end of getAttribute

  void getTypeAttribute(std::string& t, const Group& g) {
    getAttributeValue(t, g, "type");
  }  // end of getTypeAttribute

  std::string getTypeAttribute(const DataSet& d) {
    std::string type;
    getTypeAttribute(type, d);
    return type;
  }

  bool hasTypeAttribute(const DataSet& d) { return hasAttribute(d, "type"); }

  void getTypeAttribute(std::string& t, const DataSet& d) {
    getAttributeValue(t, d, "type");
  }  // end of getTypeAttribute

  std::string getTypeAttribute(const Group& g, const std::string& n) {
    std::string a;
    getTypeAttribute(a, g, n);
    return a;
  }

  void getTypeAttribute(std::string& t, const Group& g, const std::string& n) {
    try {
      hsize_t s = g.getNumObjs();
      hsize_t i;
      for (i = 0; i != s; ++i) {
        if (g.getObjnameByIdx(i) == n) {
          if (g.getObjTypeByIdx(i) == H5G_GROUP) {
            const auto& gr = openGroup(g, n);
            getTypeAttribute(t, gr);
            return;
          } else if (g.getObjTypeByIdx(i) == H5G_DATASET) {
            const auto& d = openDataSet(g, n);
            getTypeAttribute(t, d);
            return;
          } else {
            raise(
                "madnex::getTypeAttribute: unsupported object type while "
                "treating '" +
                n + "'");
          }
        }
      }
    } catch (H5::Exception&) {
      raise("madnex::getTypeAttribute: can't get type attribute");
    }
    raise("getTypeAttribute: no object named '" + n + "'");
  }  // end of getTypeAttribute

  void checkTypeAttribute(const Group& g, const std::string& t) {
    std::string type;
    getTypeAttribute(type, g);
    if (type != t) {
      raise(
          "checkTypeAttribute: group type attribute is not what was expected "
          "(expected '" +
          t + "', read '" + type + "')");
    }
  }  // end of checkTypeAttribute

  void checkTypeAttribute(const DataSet& d, const std::string& t) {
    if (!hasTypeAttribute(d)) {
      return;
    }
    std::string type;
    getTypeAttribute(type, d);
    if (type != t) {
      raise(
          "madnex::checkTypeAttribute: group type attribute is not what "
          " was expected (expected '" +
          t + "', read '" + type + "')");
    }
  }  // end of checkTypeAttribute

  MADNEX_POD_IMPLEMENTATION(char)
  MADNEX_POD_IMPLEMENTATION(std::int32_t)
  MADNEX_POD_IMPLEMENTATION(std::int64_t)
  MADNEX_POD_IMPLEMENTATION(float)
  MADNEX_POD_IMPLEMENTATION(double)
  MADNEX_POD_IMPLEMENTATION(long double)

  MADNEX_HDF5_VECTOR_IMPLEMENTATION(char)
  MADNEX_HDF5_VECTOR_IMPLEMENTATION(std::int32_t)
  MADNEX_HDF5_VECTOR_IMPLEMENTATION(std::int64_t)
  MADNEX_HDF5_VECTOR_IMPLEMENTATION(float)
  MADNEX_HDF5_VECTOR_IMPLEMENTATION(double)
  MADNEX_HDF5_VECTOR_IMPLEMENTATION(long double)

  // void write(Group& g,
  // 	     const std::string& d,
  // 	     const bool& o)
  // {
  //   try{
  //     hsize_t  dimsf[1];
  //     dimsf[0] = 1;
  //     DataSpace dataspace(1, dimsf );
  //     DataSet dataset = g.createDataSet(d,getNativeType<char>(),
  // 					dataspace);
  //     char c(0);
  //     if(o){
  // 	c = 1;
  //     }
  //     dataset.write(&c,getNativeType<char>());
  //     writeTypeAttribute< bool >(dataset);
  //   } catch(H5::Exception& e){
  //     raise(e.getDetailMsg());
  //   }
  // }

  // void read(bool& o,
  // 	    const Group& g,
  // 	    const std::string& d)
  // {
  //   try{
  //     char c;
  //     const auto dataset = openDataSet(g,d);
  //     checkTypeAttribute<bool>(dataset);
  //     const auto s = dataset.getSpace();
  //     if(s.getSimpleExtentNdims()!=1){
  // 	raise"madnex::read: invalid type size. Error while retreiving boolean in
  // '"+d+"'");
  //     }
  //     hsize_t dims[1];
  //     s.getSimpleExtentDims(dims);
  //     if(dims[0]!=1){
  // 	raise("madnex::read: invalid type size. Error while retreiving boolean
  // in '"+d+"'");
  //     }
  //     dataset.read(&c,getNativeType<char>());
  //     if(c==0){
  // 	o = false;
  //     } else {
  // 	o = true;
  //     }
  //   } catch(H5::Exception& e){
  //     raise(e.getDetailMsg());
  //   }
  // }

  // template<>
  // bool read(const Group& g,
  // 	    const std::string& s)
  // {
  //   bool v;
  //   read(v,g,s);
  //   return v;
  // } // end of bool read

  void write(Group& g,
             const std::string& d,
             const std::string& o,
             const bool b) {
    try {
      if (b) {
        unlinkIfExists(g, d);
      }
      if (o.empty()) {
        const auto c = "";
        hsize_t dimsf[1];
        dimsf[0] = 1u;
        DataSpace dataspace(1, dimsf);
        const auto type = getNativeType<std::string>();
        DataSet dataset = g.createDataSet(d, type, dataspace);
        dataset.write(&c, type);
        // attribute
        writeTypeAttribute<std::string>(dataset);
        writeEmptyObjectAttribute(dataset);
      } else {
        hsize_t dimsf[1] = {1};
        const auto type = getNativeType<std::string>();
        auto dataset = g.createDataSet(d, type, DataSpace(1, dimsf));
        const auto c = o.c_str();
        dataset.write(&c, type);
        // attribute
        writeTypeAttribute<std::string>(dataset);
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

  void write(Group& g,
             const std::string& d,
             const char* const o,
             const bool b) {
    try {
      if (b) {
        unlinkIfExists(g, d);
      }
      const auto type = getNativeType<std::string>();
      hsize_t dimsf[1] = {1};
      DataSpace dataspace(1, dimsf);
      auto data = g.createDataSet(d, type, dataspace);
      data.write(&o, type);
      // attribute
      writeTypeAttribute<std::string>(data);
      if (::strlen(o) == 0) {
        writeEmptyObjectAttribute(data);
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

  void getAttributeNames(std::vector<std::string>& n, const Group& g) {
    const auto size = static_cast<hsize_t>(g.getNumAttrs());
    n.clear();
    n.resize(size);
    for (hsize_t i = 0; i != size; ++i) {
      n[i] = g.openAttribute(i).getName();
    }
  }  // end of getAttributeNames

  std::vector<std::string> getAttributeNames(const Group& g) {
    std::vector<std::string> n;
    getAttributeNames(n, g);
    return n;
  }  // end of getAttributeNames

  void getAttributeNames(std::vector<std::string>& n, const DataSet& d) {
    const auto size = static_cast<hsize_t>(d.getNumAttrs());
    n.clear();
    n.resize(size);
    for (hsize_t i = 0; i != size; ++i) {
      n[i] = d.openAttribute(i).getName();
    }
  }  // end of getAttributeNames

  std::vector<std::string> getAttributeNames(const DataSet& d) {
    std::vector<std::string> n;
    getAttributeNames(n, d);
    return n;
  }  // end of getAttributeNames

  bool hasAttribute(const DataSet& g, const std::string& n) {
    hsize_t size = static_cast<hsize_t>(g.getNumAttrs());
    auto found = false;
    for (hsize_t i = 0; (i != size) && (!found); ++i) {
      if (g.openAttribute(i).getName() == n) found = true;
    }
    return found;
  }  // end of hasAttribute

  void writeEmptyObjectAttribute(DataSet& d) {
    try {
      const std::string empty("__HDF5Empty__");
      hsize_t dimsf[1] = {1};
      const char* c = "";
      const auto type = getNativeType<std::string>();
      DataSpace dataspace(1, dimsf);
      auto attr = d.createAttribute(empty, type, dataspace);
      attr.write(type, &c);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of writeEmptyObjectAttribute

  bool hasEmptyObjectAttribute(const DataSet& d) {
    return hasAttribute(d, "__HDF5Empty__");
  }  // end of hasEmptyObjectAttribute

  void read(std::string& o, const Group& g, const std::string& d) {
    try {
      auto dataset = openDataSet(g, d);
      checkTypeAttribute<std::string>(dataset);
      if (hasEmptyObjectAttribute(dataset)) {
        o.clear();
      } else {
        auto s = dataset.getSpace();
        if (s.getSimpleExtentNdims() != 1) {
          raise("madnex::read: invalid dimension for string '" + d + "'");
        }
        hsize_t dims[1];  // dataset dimensions
        s.getSimpleExtentDims(dims);
        o.clear();
        if (dims[0] == 1) {
          char* v;
          const auto type = getNativeType<std::string>();
          dataset.read(&v, type);
          o = v;
          std::free(v);
        } else {
          // using an fixed length string
          o.resize(dims[0]);
          const auto type = H5::PredType::C_S1;
          dataset.read(&o[0], type);
          if (o.back() == '\0') {
            o.pop_back();
          }
        }
      }
    } catch (H5::Exception& e) {
      raise("Error while reading '" + d + "' in group '" + g.getObjName() +
            "': " + e.getDetailMsg());
    }
  }

  std::vector<std::string> getSubGroupNames(const Group& g, const bool b) {
    std::vector<std::string> names;
    getSubGroupNames(names, g, b);
    return names;
  }

  void getSubGroupNames(std::vector<std::string>& n,
                        const Group& g,
                        const bool b) {
    try {
      hsize_t s = g.getNumObjs();
      n.clear();
      for (hsize_t i = 0; i != s; ++i) {
        if (g.getObjTypeByIdx(i) == H5G_GROUP) {
          n.push_back(g.getObjnameByIdx(i));
        } else if (!b) {
          raise("getSubGroupNames: object '" + g.getObjnameByIdx(i) +
                "' is not a group");
        }
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getSubGroupNames

  std::vector<std::string> getDataSetNames(const Group& g) {
    std::vector<std::string> names;
    getDataSetNames(names, g);
    return names;
  }

  void getDataSetNames(std::vector<std::string>& n, const Group& g) {
    try {
      hsize_t s = g.getNumObjs();
      hsize_t i;
      n.clear();
      for (i = 0; i != s; ++i) {
        if (g.getObjTypeByIdx(i) == H5G_DATASET) {
          n.push_back(g.getObjnameByIdx(i));
        }
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getDataSetNames

  bool contains(const Group& g, const std::string& n) {
    bool found = false;
    try {
      hsize_t s = g.getNumObjs();
      hsize_t i;
      for (i = 0; (i != s) && (!found); ++i) {
        if (g.getObjnameByIdx(i) == n) {
          found = true;
        }
      }
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
    return found;
  }  // end of contains

  void unlinkIfExists(const Group& g, const std::string& n) {
    if (!exists(g, n)) {
      return;
    }
    g.unlink(n);
  }

}  // end of namespace madnex
