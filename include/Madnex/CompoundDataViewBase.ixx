/*!
 * \file   CompoundDataViewBase.ixx
 * \brief    
 * \author THOMAS HELFER
 * \date   22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDDATAVIEWBASE_IXX
#define LIB_MADNEX_COMPOUNDDATAVIEWBASE_IXX

#include"Madnex/CompoundExtractor.hxx"

namespace madnex{
  
  template<typename T>
  T CompoundDataViewBase::extract(const char * const d,
				  const size_t i) const{
    CompoundExtractor<T> e(*this);
    return e.extract(d,i);
  } // end of CompoundDataViewBase::extract

} // end of namespace madnex
  
#endif /* LIB_MADNEX_COMPOUNDDATAVIEWBASE_IXX */
