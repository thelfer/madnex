/*!
 * \file   include/Madnex/CompoundDataView.ixx
 * \brief
 * \author Thomas Helfer
 * \date   22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDDATAVIEW_IXX
#define LIB_MADNEX_COMPOUNDDATAVIEW_IXX

#include "Madnex/CompoundExtractor.hxx"

namespace madnex {

  template <typename T>
  T CompoundDataView::extract(const char* n) const {
    return this->template extract<T>(this->getMemberIndex(n));
  }  // end of CompoundDataView::extract

  template <typename T>
  T CompoundDataView::extract(const std::string& n) const {
    return this->template extract<T>(this->getMemberIndex(n));
  }  // end of CompoundDataView::extract

  template <typename T>
  T CompoundDataView::extract(const size_t i) const {
    return CompoundDataViewBase::template extract<T>(this->data_pointer, i);
  }  // end of CompoundDataView::extract

  template <typename T>
  void extract(T& v, const CompoundDataView& d, const size_t i) {
    v = d.template extract<T>(i);
  }  // end of extract

  template <typename T>
  void extract(T& v, const CompoundDataView& d, const char* const n) {
    v = d.template extract<T>(n);
  }  // end of extract

  template <typename T>
  void extract(T& v, const CompoundDataView& d, const std::string& n) {
    v = d.template extract<T>(n);
  }  // end of extract

}  // end of namespace madnex

#endif /* LIB_MADNEX_COMPOUNDDATAVIEW_IXX */
