/*!
 * \file   CompoundDataViewBase.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 janv. 2017
 */

#include"Madnex/CompoundDataViewBase.hxx"

namespace madnex{

  CompoundDataViewBase::CompoundDataViewBase(CompoundDataViewBase&&) = default;

  CompoundDataViewBase::CompoundDataViewBase(const CompoundDataViewBase&) = default;

  CompoundDataViewBase& CompoundDataViewBase::operator=(CompoundDataViewBase&&) = default;

  CompoundDataViewBase& CompoundDataViewBase::operator=(const CompoundDataViewBase&) = default;
  
  CompoundDataViewBase::CompoundDataViewBase(const DataSet& d)
  {
    try {
      const auto dtype = d.getDataType();
      if (dtype.getClass() != H5T_COMPOUND) {
        raise("CompoundDataViewBase: invalid data set");
      }
      this->ctype = H5::CompType(d);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

  size_t CompoundDataViewBase::getMemberIndex(const char * n) const{
    try {
      return this->ctype.getMemberIndex(n);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of CompoundDataViewBase::getMemberIndex

  size_t CompoundDataViewBase::getMemberIndex(const std::string& n) const{
    try {
      return this->ctype.getMemberIndex(n);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of CompoundDataViewBase::getMemberIndex

  H5T_class_t CompoundDataViewBase::getMemberClass(const size_t i) const
  {
    try {
      return this->ctype.getMemberClass(i);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of CompoundDataViewBase::getMemberClass

  size_t CompoundDataViewBase::getMemberOffset(const size_t i) const{
    try {
      return this->ctype.getMemberOffset(i);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  } // end of CompoundDataViewBase::getMemberOffset

  StrType CompoundDataViewBase::getMemberStrType(const size_t i) const{
    try {
      return this->ctype.getMemberStrType(i);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  } // end of CompoundDataViewBase::getMemberStrType

  CompoundDataViewBase::~CompoundDataViewBase() = default;

} // end of namespace madnex
