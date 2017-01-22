/*!
 * \file   CompoundExtractor.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   22 janv. 2017
 */

#include<stdexcept>
#include"Madnex/CompoundExtractor.hxx"

namespace madnex{

  CompoundExtractor::CompoundExtractor(const DataSet& d)
  {
    const auto dtype = d.getDataType();
    if(dtype.getClass()!=H5T_COMPOUND){
      throw(std::runtime_error("CompoundExtractor: invalid data set"));
    }
    this->ctype = H5::CompType(d);
    this->data.resize(ctype.getSize());
    d.read(this->data.data(),ctype);
  }

  void CompoundExtractor::checkMemberClass(const PredType& e,
					   const H5T_class_t t)
  {
    if(e.getClass()!=t){
      throw(std::runtime_error("CompoundExtractor:checkMemberClass: "
			       "invalid data type"));
    }
  }

  template<>
  std::string CompoundExtractor::extract(const std::string& n) const{
    const auto i   = this->ctype.getMemberIndex(n);
    const auto o   = this->ctype.getMemberOffset(i);
    const auto t   = this->ctype.getMemberStrType(i);
    const auto s   = t.getSize();
    const auto ptr = this->data.data()+o;
    std::string r;
    if(s==H5T_VARIABLE){
      return {ptr};
    }
    return {ptr,s};
  } // end of CompoundExtractor::extract

  template<>
  std::string CompoundExtractor::extract(const char* n) const{
    const auto i   = this->ctype.getMemberIndex(n);
    const auto o   = this->ctype.getMemberOffset(i);
    const auto t   = this->ctype.getMemberStrType(i);
    const auto s   = t.getSize();
    const auto ptr = this->data.data()+o;
    std::string r;
    if(s==H5T_VARIABLE){
      return {ptr};
    }
    return {ptr,s};
  } // end of CompoundExtractor::extract
  
  CompoundExtractor::~CompoundExtractor(){
    // const unsigned s = static_cast<unsigned>(this->ctype.getNmembers());
    // for(unsigned i=0;i!=s;++i){
    //   const auto t = this->ctype.getMemberClass(i);
    //   if(t==madnex::getNativeType<std::string>().getClass()){
    // 	const auto ptr =
    // 	  this->data.data()+this->ctype.getMemberOffset(i);
    // 	std::free(ptr);
    //   }
    // }
  } // end of CompoundExtractor::~CompoundExtractor
  
} // end of namespace madnex
