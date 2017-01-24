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
    CompoundExtractor<T> e(*this);
    return e.extract(n);
  } // end of CompoundDataView::extract

  template<typename T>
  T CompoundDataView::extract(const std::string& n) const{
    CompoundExtractor<T> e(*this);
    return e.extract(n);
  } // end of CompoundDataView::extract

} // end of namespace madnex
  
#endif /* LIB_MADNEX_COMPOUNDDATAVIEW_IXX */
