/*! 
 * \file   src/HDF5.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  17/01/2017
 */

#include<cstring>
#include<stdexcept>
#include"Madnex/HDF5.hxx"
#include"Madnex/TypeName.hxx"

/*!
 * \brief macro used to implement the `read`/`write` functions for 
 * plain old data types.
 */
#define MADNEX_POD_IMPLEMENTATION(X)			         \
  void write(Group& g,				                 \
	     const std::string& d,				 \
	     const X & o)					 \
  {								 \
    try{							 \
      hsize_t dimsf[1] = {1};					 \
      DataSpace dataspace(1,dimsf);				 \
      auto dataset = g.createDataSet(d,getNativeType< X >(),     \
				     dataspace);		 \
      dataset.write(&o,getNativeType< X >());     		 \
      writeTypeAttribute< X >(dataset); 			 \
    } catch (Exception& e){                                      \
      throw(std::runtime_error(e.getDetailMsg()));		 \
    }                                                            \
  }                                                              \
								 \
  void read(X& o,	                    			 \
            const Group& g,					 \
            const std::string& d)				 \
  {								 \
    try{                                                         \
      auto dataset = openDataSet(g,d);			         \
      checkTypeAttribute< X >(dataset);			         \
      const auto s = dataset.getSpace();		         \
      if(s.getSimpleExtentNdims()!=1){				 \
	throw(std::runtime_error("madnex::read: "                \
	                         "invalid type size\n"		 \
                                 "error while retreiving a "	 \
			         "'#X' in '"+d+"'"));		 \
								 \
      }								 \
      hsize_t dims[1];						 \
      s.getSimpleExtentDims(dims);			         \
      if(dims[0]!=1){						 \
	throw(std::runtime_error("madnex::read: "                \
	                         "invalid type size\n"		 \
                                 "error while retreiving a "	 \
			         "'#X' in '"+d+"'"));		 \
      }								 \
      dataset.read(&o,getNativeType< X >());			 \
    } catch (Exception& e){                                      \
      throw(std::runtime_error(e.getDetailMsg()));		 \
    }                                                            \
  }								 \
  								 \
  template<>                                                     \
  X read(const Group& g,const std::string& d)			 \
  {						                 \
    X o;						         \
    read(o,g,d);						 \
    return o;						         \
  }  								 \
  								 \
  void getAttributeValue(X& v,					 \
                         const Group& g,			 \
			 const std::string& n)                   \
  {								 \
    using namespace std;					 \
    try{      							 \
      const auto attr = openAttribute(g,n);			 \
      const auto s = attr.getSpace();			         \
      if(s.getSimpleExtentNdims()!=1){				 \
	throw(std::runtime_error("madnex::getAttributeValue: "   \
	                         "invalid type size\n"		 \
                                 "error while retreiving a "	 \
			         "'#X' in group '"+n+"'"));      \
      }								 \
      hsize_t dims[1]; 	/* dataset dimensions */		 \
      s.getSimpleExtentDims(dims);			         \
      if(dims[0]!=1){						 \
	throw(std::runtime_error("madnex::getAttributeValue: "   \
	                         "invalid type size\n"		 \
                                 "error while retreiving a "	 \
			         "'#X' in group '"+n+"'"));      \
      }								 \
      attr.read(getNativeType<X>(),&v);		                 \
    } catch (Exception& e){					 \
      string msg("madnex::getTypeAttributeValue: ");		 \
      msg += "can't get attribute '"+n+"' for group";		 \
      throw(runtime_error(msg));				 \
    }								 \
  }								 \
  								 \
  void getAttributeValue(X& v,					 \
                         const DataSet& d,			 \
			 const std::string& n)                   \
  {								 \
    using namespace std;					 \
    try{      							 \
      const auto attr = openAttribute(d,n);			 \
      const auto s = attr.getSpace();			         \
      if(s.getSimpleExtentNdims()!=1){				 \
	throw(std::runtime_error("madnex::getAttributeValue: "   \
	                         "invalid type size\n"		 \
                                 "error while retreiving a "	 \
			         "'#X' in data set '"+n+"'"));   \
      }								 \
      hsize_t dims[1]; 	/* dataset dimensions */		 \
      s.getSimpleExtentDims(dims);			         \
      if(dims[0]!=1){						 \
	throw(std::runtime_error("madnex::getAttributeValue: "   \
	                         "invalid type size\n"		 \
                                 "error while retreiving a "	 \
			         "'#X' in data set'"+n+"'"));    \
      }								 \
      attr.read(getNativeType<X>(),&v);		                 \
    } catch (Exception& e){					 \
      string msg("madnex::getTypeAttributeValue: ");		 \
      msg += "can't get attribute '"+n+"' for group";		 \
      throw(runtime_error(msg));				 \
    }								 \
  }
  
namespace madnex
{

  Group createGroup(const Group& g,
		    const std::string& n)
  {
    using namespace std;
    Group gr;
    try{
      gr =  g.createGroup(n);
    } catch (H5::GroupIException& e){
      string msg("madnex::createGroup: ");
      msg += "creation of group '"+n+"' failed (";
      msg += e.getDetailMsg()+")";
      throw(runtime_error(msg));
    } catch (Exception& e){
      string msg("madnex::createGroup: ");
      msg += "creation of group '"+n+"' failed (";
      msg += e.getDetailMsg();
      msg += ")";
      throw(runtime_error(msg));
    }
    return gr;
  }
    
  Group openGroup(const Group& g,
		  const std::string& n)
  {
    using namespace std;
    Group gr;
    try{
      gr =  g.openGroup(n);
    } catch (Exception& e){
      string msg("madnex::openGroup: ");
      msg += "opening of group '"+n+"' failed (";
      msg += e.getDetailMsg();
      msg += ")";
      throw(runtime_error(msg));
    }
    return gr;
  }

  DataSet openDataSet(const Group& g,
		      const std::string& n)
  {
    using namespace std;
    DataSet d;
    try{
      d = g.openDataSet(n);
    } catch (Exception& e){
      string msg("madnex::openDataSet: ");
      msg += "opening of data set '"+n+"' failed (";
      msg += e.getDetailMsg();
      msg += ")";
      throw(runtime_error(msg));
    }
    return d;
  }

  Attribute openAttribute(const DataSet& d,
			  const std::string& n)
  {
    using namespace std;
    Attribute a;
    try{
      a = d.openAttribute(n);
    } catch (Exception& e){
      string msg("madnex::openAttribute: ");
      msg += "opening of attribute '"+n+"' failed (";
      msg += e.getDetailMsg();
      msg += ")";
      throw(runtime_error(msg));
    }
    return a;
  }

  Attribute openAttribute(const Group& g,
			  const std::string& n)
  {
    using namespace std;
    Attribute a;
    try{
      a = g.openAttribute(n);
    } catch (Exception& e){
      string msg("madnex::openAttribute: ");
      msg += "opening of attribute '"+n+"' failed (";
      msg += e.getDetailMsg();
      msg += ")";
      throw(runtime_error(msg));
    }
    return a;
  }

  void writeTypeAttribute(Group& g,const std::string& t)
  {
    using namespace std;
    try{      
      const hsize_t dimsf[1] = {1};
      const auto type = getNativeType<std::string>();
      DataSpace dataspace(1,dimsf);
      Attribute attr = g.createAttribute("type",type,
					 dataspace);
      const char* v = t.c_str();
      attr.write(type,&v);
    } catch (Exception& e){
      string msg("madnex::writeTypeAttribute: ");
      msg += "operation failed ("+e.getDetailMsg()+")";
      throw(runtime_error(msg));
    }
  }

  void writeTypeAttribute(DataSet& d,const std::string& t)
  {
    using namespace std;
    try{      
      hsize_t dimsf[1] = {1};
      const auto type = getNativeType<std::string>();
      DataSpace dataspace(1,dimsf);
      Attribute attr = d.createAttribute("type",type,
					 dataspace);
      const char *c = t.c_str();
      attr.write(type,&c);
    } catch (Exception& e){
      string msg("madnex::writeTypeAttribute: ");
      msg += "operation failed ("+e.getDetailMsg()+")";
      throw(runtime_error(msg));
    }
  }

  std::string getTypeAttribute(const Group& g){
    std::string type;
    getTypeAttribute(type,g);
    return type;
  }

  void getAttributeValue(std::string& v,
			 const Group& g,
			 const std::string& n)
  {
    using namespace std;
    try{      
      const auto attr = openAttribute(g,n);
      const auto s = attr.getSpace();
      if(s.getSimpleExtentNdims()!=1){
	throw(std::runtime_error("madnex::getAttributeValue: "
				 "invalid dimension for "
				 "attribute '"+n+"'"));
      }
      hsize_t dims[1]; 	// dataset dimensions
      s.getSimpleExtentDims(dims);
      v.clear();
      if(attr.getDataType().isVariableStr()){
	if(dims[0]!=1){
	  throw(std::runtime_error("madnex::getAttributeValue: "
				   "invalid dimension for "
				   "attribute '"+n+"'"));
	}
	char *pv;
	const auto type = getNativeType<std::string>();
	attr.read(type,&pv);
	v = pv;
	std::free(pv);
      } else {
	// using an fixed length string
	v.resize(dims[0]);
	const auto type = H5::PredType::C_S1;
	attr.read(type,&v[0]);
	if(v.back()=='\0'){
	  v.pop_back();
	}
      }
    } catch (Exception& e){
      string msg("madnex::getTypeAttributeValue: ");
      msg += "can't get attribute '"+n+"' for group";
      throw(runtime_error(msg));
    }
  }

  template<>
  std::string getAttributeValue(const Group& g,
				const std::string& n)
  {
    std::string s;
    getAttributeValue(s,g,n);
    return s;
  } // end of getAttribute

  void getAttributeValue(std::string& v,
			 const DataSet& d,
			 const std::string& n)
  {
    using namespace std;
    try{      
      const auto attr = openAttribute(d,n);
      const auto s = attr.getSpace();
      if(s.getSimpleExtentNdims()!=1){
	throw(std::runtime_error("madnex::getAttributeValue: "
				 "invalid dimension for "
				 "attribute '"+n+"'"));
      }
      hsize_t dims[1]; 	// dataset dimensions
      s.getSimpleExtentDims(dims);
      v.clear();
      if(dims[0]!=1){
	throw(std::runtime_error("madnex::getAttributeValue: "
				 "invalid dimension for "
				 "attribute '"+n+"'"));
      }
      char *pv;
      const auto type = getNativeType<std::string>();
      attr.read(type,&pv);
      v = pv;
      std::free(pv);
    } catch (Exception& e){
      string msg("madnex::getTypeAttributeValue: ");
      msg += "can't get attribute '"+n+"' for data set";
      throw(runtime_error(msg));
    }
  }

  template<>
  std::string getAttributeValue(const DataSet& d,
				const std::string& n)
  {
    std::string s;
    getAttributeValue(s,d,n);
    return s;
  } // end of getAttribute
  
  void getTypeAttribute(std::string& t,
			const Group& g)
  {
    getAttributeValue(t,g,"type");
  } // end of getTypeAttribute

  std::string getTypeAttribute(const DataSet& d){
    std::string type;
    getTypeAttribute(type,d);
    return type;
  }
  
  void getTypeAttribute(std::string& t,
			const DataSet& d)
  {
    getAttributeValue(t,d,"type");
  } // end of getTypeAttribute

  std::string getTypeAttribute(const Group& g,
			       const std::string& n){
    std::string a;
    getTypeAttribute(a,g,n);
    return a;
  }
  
  void getTypeAttribute(std::string& t,
			const Group& g,
			const std::string& n)
  {
    using namespace std;
    try{      
      hsize_t s = g.getNumObjs();
      hsize_t i;
      for(i=0;i!=s;++i){
	if(g.getObjnameByIdx(i)==n){
	  if(g.getObjTypeByIdx(i)==H5G_GROUP){
	    const auto& gr = openGroup(g,n);
	    getTypeAttribute(t,gr);
	    return;
	  } else if(g.getObjTypeByIdx(i)==H5G_DATASET){
	    const auto& gr = openDataSet(g,n);
	    getTypeAttribute(t,gr);
	    return;
	  } else {
	    string msg("madnex::getTypeAttribute: ");
	    msg += "unsupported object type while treating '"+n+"'";
	    throw(runtime_error(msg));
	  }
	}
      }
    } catch (Exception& e){
      string msg("madnex::getTypeAttribute: ");
      msg += "can't get type attribute";
      throw(runtime_error(msg));
    }
    string msg("getTypeAttribute: ");
    msg += "no object named '"+n+"'";
    throw(runtime_error(msg));
  } // end of getTypeAttribute

  void checkTypeAttribute(const Group& g,
			  const std::string& t)
  {
    using namespace std;
    string type;
    getTypeAttribute(type,g);
    if(type!=t){
      string msg("checkTypeAttribute: ");
      msg += "group type attribute is not ";
      msg += "what was expected ";
      msg += "(expected '"+t+"', read '"+type+"')";
      throw(runtime_error(msg));
    }
  } // end of checkTypeAttribute

  void checkTypeAttribute(const DataSet& d,
			  const std::string& t)
  {
    using namespace std;
    string type;
    getTypeAttribute(type,d);
    if(type!=t){
      string msg("madnex::checkTypeAttribute: ");
      msg += "group type attribute is not ";
      msg += "what was expected ";
      msg += "(expected '"+t+"', read '"+type+"')";
      throw(runtime_error(msg));
    }
  } // end of checkTypeAttribute
  
  MADNEX_POD_IMPLEMENTATION(char)
  MADNEX_POD_IMPLEMENTATION(short)
  MADNEX_POD_IMPLEMENTATION(int)
  MADNEX_POD_IMPLEMENTATION(long)
  MADNEX_POD_IMPLEMENTATION(unsigned short)
  MADNEX_POD_IMPLEMENTATION(unsigned int)
  MADNEX_POD_IMPLEMENTATION(unsigned long)
  MADNEX_POD_IMPLEMENTATION(float)
  MADNEX_POD_IMPLEMENTATION(double)
  MADNEX_POD_IMPLEMENTATION(long double)

  void write(Group& g,
	     const std::string& d,
	     const bool& o)
  {
    using namespace std;
    try{
      hsize_t  dimsf[1];
      dimsf[0] = 1;
      DataSpace dataspace(1, dimsf );
      DataSet dataset = g.createDataSet(d,getNativeType<char>(),
					dataspace);
      char c(0);
      if(o){
	c = 1;
      }
      dataset.write(&c,getNativeType<char>());
      writeTypeAttribute< bool >(dataset);
    } catch (Exception& e){
      throw(runtime_error(e.getDetailMsg()));
    }
  }

  void read(bool& o,
	    const Group& g,
	    const std::string& d)
  {
    using namespace std;
    try{
      char c;
      const auto dataset = openDataSet(g,d);
      checkTypeAttribute<bool>(dataset);
      const auto s = dataset.getSpace();
      if(s.getSimpleExtentNdims()!=1){
	throw(runtime_error("madnex::read: invalid type size\n"
			    "error while retreiving boolean in '"+d+"'"));
      }
      hsize_t dims[1];
      s.getSimpleExtentDims(dims);
      if(dims[0]!=1){
	throw(std::runtime_error("madnex::read: invalid type size\n"
				 "error while retreiving boolean in '"+d+"'"));
      }
      dataset.read(&c,getNativeType<char>());
      if(c==0){
	o = false;
      } else {
	o = true;
      }
    } catch (Exception& e){
      throw(std::runtime_error(e.getDetailMsg()));
    }
  }

  template<>
  bool read(const Group& g,
	    const std::string& s)
  {
    bool v;
    read(v,g,s);
    return v;
  } // end of bool read
  
  void write(Group& g,
	     const std::string& d,
	     const std::string& o)
  {
    using namespace std;
    try{
      if(o.empty()){
	const auto c = "";
	hsize_t  dimsf[1];
	dimsf[0] = 1u;
	DataSpace dataspace(1, dimsf );
	const auto type = getNativeType<std::string>();
	DataSet dataset = g.createDataSet(d,type, dataspace);
	dataset.write(&c,type);
	// attribute
	writeTypeAttribute<string>(dataset);
	writeEmptyObjectAttribute(dataset);
      } else {
	hsize_t  dimsf[1] = {1};
	const auto type = getNativeType<std::string>();
	auto dataset = g.createDataSet(d,type,DataSpace(1,dimsf));
	const auto c = o.c_str();
	dataset.write(&c,type);
	// attribute
	writeTypeAttribute<string>(dataset);
      }
    } catch (Exception& e){
      throw(runtime_error(e.getDetailMsg()));
    }
  }

  void write(Group& g,
	     const std::string& d,
	     const char * const o)
  {
    using namespace std;
    try{
      const auto type = getNativeType<std::string>();
      hsize_t  dimsf[1] = {1};
      DataSpace dataspace(1,dimsf);
      auto data = g.createDataSet(d,type,dataspace);
      data.write(&o,type);
      // attribute
      writeTypeAttribute<string>(data);
      if(::strlen(o)==0){
	writeEmptyObjectAttribute(data);
      }
    } catch (Exception& e){
      throw(runtime_error(e.getDetailMsg()));
    }
  }

  void getAttributeNames(std::vector<std::string>& n,
			 const Group& g)
  {
    const auto size = static_cast<hsize_t>(g.getNumAttrs());
    n.clear();
    n.resize(size);
    for(hsize_t i=0;i!=size;++i){
      n[i] = g.openAttribute(i).getName();
    }
  } // end of getAttributeNames

  std::vector<std::string> getAttributeNames(const Group& g)
  {
    std::vector<std::string> n;
    getAttributeNames(n,g);
    return n;
  } // end of getAttributeNames
  
  void getAttributeNames(std::vector<std::string>& n,
			 const DataSet& d)
  {
    const auto size = static_cast<hsize_t>(d.getNumAttrs());
    n.clear();
    n.resize(size);
    for(hsize_t i=0;i!=size;++i){
      n[i] = d.openAttribute(i).getName();
    }
  } // end of getAttributeNames

  std::vector<std::string> getAttributeNames(const DataSet& d)
  {
    std::vector<std::string> n;
    getAttributeNames(n,d);
    return n;
  } // end of getAttributeNames
  
  bool hasAttribute(const DataSet& g,
		    const std::string& n)
  {
    hsize_t size = static_cast<hsize_t>(g.getNumAttrs());
    auto found = false;
    for(hsize_t i=0;(i!=size)&&(!found);++i){
      if(g.openAttribute(i).getName()==n)
	found = true;
    }
    return found;
  } // end of hasAttribute

  void writeEmptyObjectAttribute(DataSet& d)
  {
    try{      
      const std::string empty("__HDF5Empty__");
      hsize_t dimsf[1] = {1};
      const char *c = "";
      const auto type = getNativeType<std::string>();
      DataSpace dataspace(1,dimsf);
      auto attr = d.createAttribute(empty,type,dataspace);
      attr.write(type,&c);
    } catch (Exception& e){
      throw(std::runtime_error(e.getDetailMsg()));
    }
  } // end of writeEmptyObjectAttribute

  bool hasEmptyObjectAttribute(const DataSet& d)
  {
    return hasAttribute(d,"__HDF5Empty__");
  } // end of hasEmptyObjectAttribute

  void read(std::string& o,
	    const Group& g,
	    const std::string& d)
  {
    try{
      auto dataset = openDataSet(g,d);
      checkTypeAttribute<std::string>(dataset);
      if(hasEmptyObjectAttribute(dataset)){
	o.clear();
      } else {
	auto s = dataset.getSpace();
	if(s.getSimpleExtentNdims()!=1){
	  throw(std::runtime_error("madnex::read: "
				   "invalid dimension for "
				   "string '"+d+"'"));
	}
	hsize_t dims[1]; 	// dataset dimensions
	s.getSimpleExtentDims(dims);
	o.clear();
	if(dims[0]==1){
	  char* v;
	  const auto type = getNativeType<std::string>();
	  dataset.read(&v,type);
	  o = v;
	  std::free(v);
	} else {
	  // using an fixed length string
	  o.resize(dims[0]);
	  const auto type = H5::PredType::C_S1;
	  dataset.read(&o[0],type);
	  if(o.back()=='\0'){
	    o.pop_back();
	  }
	}
      }
    } catch (Exception& e){
      throw(std::runtime_error(e.getDetailMsg()));
    }
  }

  std::vector<std::string>
  getSubGroupNames(const Group& g,const bool b){
    std::vector<std::string> names;
    getSubGroupNames(names,g,b);
    return names;
  }
  
  void getSubGroupNames(std::vector<std::string>& n,
			const Group& g,
			const bool b)
  {
    using namespace std;
    try{
      hsize_t s = g.getNumObjs();
      n.clear();
      for(hsize_t i=0;i!=s;++i){
	if(g.getObjTypeByIdx(i)==H5G_GROUP){
	  n.push_back(g.getObjnameByIdx(i));
	} else if(!b){
	  string msg("getSubGroupNames: ");
	  msg += "object '"+g.getObjnameByIdx(i);
	  msg += "' is not a group";
	  throw(runtime_error(msg));
	}
      }
    } catch (Exception& e){
      throw(runtime_error(e.getDetailMsg()));
    }
  } // end of getSubGroupNames

  std::vector<std::string>
  getDataSetNames(const Group& g){
    std::vector<std::string> names;
    getDataSetNames(names,g);
    return names;
  }
  
  void getDataSetNames(std::vector<std::string>& n,
			const Group& g)
  {
    using namespace std;
    try{
      hsize_t s = g.getNumObjs();
      hsize_t i;
      n.clear();
      for(i=0;i!=s;++i){
	if(g.getObjTypeByIdx(i)==H5G_DATASET){
	  n.push_back(g.getObjnameByIdx(i));
	}
      }
    } catch (Exception& e){
      throw(runtime_error(e.getDetailMsg()));
    }
  } // end of getDataSetNames
  
  bool contains(const Group& g,
		const std::string& n)
  {
    using namespace std;
    bool found = false;
    try{
      hsize_t s = g.getNumObjs();
      hsize_t i;
      for(i=0;(i!=s)&&(!found);++i){
	if(g.getObjnameByIdx(i)==n){
	  found = true;
	}
      }
    } catch (Exception& e){
      throw(runtime_error(e.getDetailMsg()));
    }
    return found;
  } // end of contains

  template<>
  HDF5TypeDescription<char> getNativeType<char>()
  {
    return PredType::NATIVE_CHAR;
  }

  template<>
  HDF5TypeDescription<unsigned char> getNativeType<unsigned char>()
  {
    return PredType::NATIVE_UCHAR;
  }

  template<>
  HDF5TypeDescription<short> getNativeType<short>()
  {
    return PredType::NATIVE_SHORT;
  }

  template<>
  HDF5TypeDescription<unsigned short> getNativeType<unsigned short>()
  {
    return PredType::NATIVE_USHORT;
  }

  template<>
  HDF5TypeDescription<int> getNativeType<int>()
  {
    return PredType::NATIVE_INT;
  }

  template<>
  HDF5TypeDescription<unsigned int> getNativeType<unsigned int>()
  {
    return PredType::NATIVE_UINT;
  }

  template<>
  HDF5TypeDescription<long> getNativeType<long>()
  {
    return PredType::NATIVE_LONG;
  }

  template<>
  HDF5TypeDescription<unsigned long> getNativeType<unsigned long>()
  {
    return PredType::NATIVE_ULONG;
  }

  template<>
  HDF5TypeDescription<float> getNativeType<float>()
  {
    return PredType::NATIVE_FLOAT;
  }
  
  template<>
  HDF5TypeDescription<double> getNativeType<double>()
  {
    return PredType::NATIVE_DOUBLE;
  }

  template<>
  HDF5TypeDescription<long double> getNativeType<long double>()
  {
    return PredType::NATIVE_LDOUBLE;
  }

  template<>
  HDF5TypeDescription<std::string> getNativeType<std::string>()
  {
    return {H5::PredType::C_S1,H5T_VARIABLE};
  }
  
} // end of namespace madnex
