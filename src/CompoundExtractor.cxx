/*!
 * \file   CompoundExtractor.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   22 janv. 2017
 */

#include<cstring>
#include<stdexcept>
#include"Madnex/CompoundDataView.hxx"

namespace madnex{

  CompoundExtractorBase::CompoundExtractorBase(const CompoundDataView& v)
    : cview(v)
  {} // end of CompoundExtractorBase::CompoundExtractorBase

  void CompoundExtractorBase::checkMemberClass(const PredType& e,
					       const H5T_class_t t)
  {
    if(e.getClass()!=t){
      throw(std::runtime_error("CompoundExtractorBase:checkMemberClass: "
  			       "invalid data type"));
    }
  } // end of CompoundExtractorBase::checkMemberClass
  
  std::string
  CompoundExtractor<std::string>::extract(const std::string& n) const{
    return this->extract(this->cview.getMemberIndex(n));
  } // end of CompoundExtractor::extract

  std::string
  CompoundExtractor<std::string>::extract(const char* n) const{
    return this->extract(this->cview.getMemberIndex(n));
  }

  std::string
  CompoundExtractor<std::string>::extract(const size_t i) const{
    auto strnlen = [](const char *s,const size_t m){
      auto r = size_t{};
      while((s[r]!='\0')&&(r<m)){
	++r;
      }
      return r;
    };
    const auto o   = this->cview.getMemberOffset(i);
    const auto t   = this->cview.getMemberStrType(i);
    const auto s   = t.getSize();
    const auto ptr = this->cview.data()+o;
    std::string r;
    if(s==H5T_VARIABLE){
      return {ptr};
    }
    return {ptr,strnlen(ptr,s)};
  } // end of CompoundExtractor::extract
    
} // end of namespace madnex
