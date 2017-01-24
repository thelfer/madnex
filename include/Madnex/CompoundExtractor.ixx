/*!
 * \file   CompoundExtractor.ixx
 * \brief    
 * \author THOMAS HELFER
 * \date   22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDEXTRACTOR_IXX
#define LIB_MADNEX_COMPOUNDEXTRACTOR_IXX

namespace madnex{

  template<typename T>
  T CompoundExtractor<T>::extract(const char *n) const{
    return this->extract(this->cview.getMemberIndex(n));
  } // end of CompoundExtractor::extract

  template<typename T>
  T CompoundExtractor<T>::extract(const std::string& n) const{
    return this->extract(this->cview.getMemberIndex(n));
  } // end of CompoundExtractor::extract

  template<typename T>
  T CompoundExtractor<T>::extract(const size_t i) const{
    CompoundExtractorBase::checkMemberClass(getNativeType<T>(),
					    this->cview.getMemberClass(i));
    const auto o = this->cview.getMemberOffset(i);
    return *(reinterpret_cast<const T *>(this->cview.data()+o));
  } // end of CompoundExtractor::extract

} // end of namespace madnex
  
#endif /* LIB_MADNEX_COMPOUNDEXTRACTOR_IXX */
