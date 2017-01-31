/*!
 * \file   CompoundDataViewBase.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 janv. 2017
 */

#include"Madnex/CompoundDataViewBase.hxx"

namespace madnex{

  CompoundDataViewBase::CompoundDataViewBase(const DataSet& d)
  {
    const auto dtype  = d.getDataType();
    if(dtype.getClass()!=H5T_COMPOUND){
      throw(std::runtime_error("CompoundDataViewBase: invalid data set"));
    }
    this->ctype = H5::CompType(d);
  }

  size_t CompoundDataViewBase::getMemberIndex(const char * n) const{
    return this->ctype.getMemberIndex(n);
  } // end of CompoundDataViewBase::getMemberIndex

  size_t CompoundDataViewBase::getMemberIndex(const std::string& n) const{
    return this->ctype.getMemberIndex(n);
  } // end of CompoundDataViewBase::getMemberIndex
  
  H5T_class_t CompoundDataViewBase::getMemberClass(const size_t i) const
  {
    return this->ctype.getMemberClass(i);
  } // end of CompoundDataViewBase::getMemberClass

  size_t CompoundDataViewBase::getMemberOffset(const size_t i) const{
    return this->ctype.getMemberOffset(i);
  } // end of CompoundDataViewBase::getMemberOffset

  StrType CompoundDataViewBase::getMemberStrType(const size_t i) const{
    return this->ctype.getMemberStrType(i);
  } // end of CompoundDataViewBase::getMemberStrType

  CompoundDataViewBase::~CompoundDataViewBase() = default;

} // end of namespace madnex
