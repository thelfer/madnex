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
  T CompoundExtractor::extract(const char *n) const{
    const auto i = this->ctype.getMemberIndex(n);
    checkMemberClass(getNativeType<T>(),this->ctype.getMemberClass(i));
    const auto o = this->ctype.getMemberOffset(i);
    return *(reinterpret_cast<const T *>(this->data.data()+o));
  } // end of CompoundExtractor::extract

  template<typename T>
  T CompoundExtractor::extract(const std::string& n) const{
    const auto i = this->ctype.getMemberIndex(n);
    checkMemberClass(getNativeType<T>(),this->ctype.getMemberClass(i));
    const auto o = this->ctype.getMemberOffset(i);
    return *(reinterpret_cast<const T *>(this->data.data()+o));
  } // end of CompoundExtractor::extract

} // end of namespace madnex
  
#endif /* LIB_MADNEX_COMPOUNDEXTRACTOR_IXX */
