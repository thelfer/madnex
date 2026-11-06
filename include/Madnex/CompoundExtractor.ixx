/*!
 * \file   CompoundExtractor.ixx
 * \brief
 * \author Thomas Helfer
 * \date   22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDEXTRACTOR_IXX
#define LIB_MADNEX_COMPOUNDEXTRACTOR_IXX

#include <cstring>

namespace madnex {

  template <typename T>
  T CompoundExtractor<T>::extract(const char* const d, const size_t i) const {
    CompoundExtractorBase::checkMemberClass(getNativeType<T>(),
                                            this->cview.getMemberClass(i));
    const auto o = this->cview.getMemberOffset(i);
    // the offset of a member in a HDF5 compound type is not guaranteed
    // to satisfy the alignment requirement of `T`: copy the bytes
    // instead of reading them through a reinterpretation of the
    // pointer, which would be undefined behaviour on strict alignment
    // targets
    auto r = T{};
    std::memcpy(&r, d + o, sizeof(T));
    return r;
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
    return {d + o, true};
  }  // end of CompoundExtractor

}  // end of namespace madnex

#endif /* LIB_MADNEX_COMPOUNDEXTRACTOR_IXX */
