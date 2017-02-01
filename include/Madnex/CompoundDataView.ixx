/*!
 * \file   CompoundDataView.ixx
 * \brief    
 * \author THOMAS HELFER
 * \date   22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDDATAVIEW_IXX
#define LIB_MADNEX_COMPOUNDDATAVIEW_IXX

#include"Madnex/CompoundExtractor.hxx"

namespace madnex{

  template<typename T>
  T CompoundDataView::extract(const char *n) const{
    return this->template extract<T>(this->getMemberIndex(n));
  } // end of CompoundDataView::extract

  template<typename T>
  T CompoundDataView::extract(const std::string& n) const{
    return this->template extract<T>(this->getMemberIndex(n));
  } // end of CompoundDataView::extract
  
  template<typename T>
  T CompoundDataView::extract(const size_t i) const{
    const auto d =  this->rdata.template is<const char*>() ?
      this->rdata.template get<const char*>() :
      this->rdata.template get<std::vector<char>>().data();
    return CompoundDataViewBase::template extract<T>(d,i);
  } // end of CompoundDataView::extract

  template<typename T>
  void extract(T& v,const CompoundDataView& d, const size_t i){
    v = d.template extract<T>(i);
  } // end of extract

  template<typename T>
  void extract(T& v,const CompoundDataView& d,const char* const n){
    v = d.template extract<T>(n);
  } // end of extract

  template<typename T>
  void extract(T& v,const CompoundDataView& d,const std::string& n){
    v = d.template extract<T>(n);
  } // end of extract
  
} // end of namespace madnex
  
#endif /* LIB_MADNEX_COMPOUNDDATAVIEW_IXX */
