/*!
 * \file   CompoundExtractor.ixx
 * \brief
 * \author Thomas Helfer
 * \date   22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDEXTRACTOR_IXX
#define LIB_MADNEX_COMPOUNDEXTRACTOR_IXX

namespace madnex {

  template <typename T>
  T CompoundExtractor<T>::extract(const char* const d, const size_t i) const {
    CompoundExtractorBase::checkMemberClass(getNativeType<T>(),
                                            this->cview.getMemberClass(i));
    const auto o = this->cview.getMemberOffset(i);
    return *(reinterpret_cast<const T*>(d + o));
  }  // end of CompoundExtractor::extract

  template <std::size_t N>
  madnex::fcstring<N> CompoundExtractor<madnex::fcstring<N>>::extract(
      const char* const d, const size_t i) const {
    const auto t = this->cview.getMemberStrType(i);
    const auto s = t.getSize();
    if (s > N) {
      CompoundExtractorBase::throwInvalidLengthException(
          "CompoundExtractor<madnex::fcstring<N>>::extract: "
          "unmatched length",
          i, N, s);
    }
    const auto o = this->cview.getMemberOffset(i);
    // a fixed length string stored in a HDF5 file is not necessarily
    // null terminated: the scan is bounded by the member size so that
    // neither the subsequent members nor the end of the buffer are read
    auto r = madnex::fcstring<N>{};
    r.assign(d + o, s);
    return r;
  }  // end of CompoundExtractor

}  // end of namespace madnex

#endif /* LIB_MADNEX_COMPOUNDEXTRACTOR_IXX */
