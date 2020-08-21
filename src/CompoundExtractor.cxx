/*!
 * \file   CompoundExtractor.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   22 janv. 2017
 */

#include<string>
#include<cstring>
#include<stdexcept>
#include"Madnex/Raise.hxx"
#include"Madnex/CompoundDataViewBase.hxx"

namespace madnex{

  void CompoundExtractorBase::throwInvalidLengthException(
      const std::string& msg,
      const size_t i,
      const size_t es,
      const size_t cs) {
    raise<std::length_error>("Error while extracting member '" +
                             std::to_string(i) + "' " + "(expected '" +
                             std::to_string(es) + "', " + "found '" +
                             std::to_string(cs) + "')");
  }  // end of CompoundExtractorBase::throwInvalidLengthException

  CompoundExtractorBase::CompoundExtractorBase(const CompoundDataViewBase& v)
    : cview(v)
  {} // end of CompoundExtractorBase::CompoundExtractorBase

  void CompoundExtractorBase::checkMemberClass(const PredType& e,
					       const H5T_class_t t)
  {
    if(e.getClass()!=t){
      raise("CompoundExtractorBase:checkMemberClass: invalid data type");
    }
  } // end of CompoundExtractorBase::checkMemberClass
  
  std::string
  CompoundExtractor<std::string>::extract(const char * const d,
					  const size_t i) const{
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
    std::string r;
    if(s==H5T_VARIABLE){
      return {d+o};
    }
    return {d+o,strnlen(d+o,s)};
  } // end of CompoundExtractor::extract
    
} // end of namespace madnex
