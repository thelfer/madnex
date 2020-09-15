/*!
 * \file   include/Madnex/CompoundExtractor.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDEXTRACTOR_H
#define LIB_MADNEX_COMPOUNDEXTRACTOR_H

#ifndef LIB_MADNEX_COMPOUNDDATAVIEWBASE_H
#error "This file shall not be included directly"
#endif
#ifndef LIB_MADNEX_COMPOUNDDATAVIEWBASE_IXX
#error "This file shall not be included directly"
#endif

namespace madnex {

  /*!
   * \brief base class for all CompoundExtractor
   */
  struct MADNEX_EXPORT CompoundExtractorBase {
    /*!
     * \brief constructor
     * \param[in] v: compound view
     */
    CompoundExtractorBase(const CompoundDataViewBase&);
    /*!
     * \param[in] e: expected type
     * \param[in] t: type contained in the the compound type
     */
    static void checkMemberClass(const PredType&, const H5T_class_t);
    /*!
     * \brief throw std::length
     * \param[in] m: error message
     * \param[in] i: member index
     * \param[in] es: expected size
     * \param[in] cs: actual size
     */
    [[noreturn]] static void throwInvalidLengthException(const std::string&,
							 const size_t,
							 const size_t,
							 const size_t);

   protected:
    //! a compound data view
    const CompoundDataViewBase& cview;
  };  // end of struct CompoundExtractorBase

  /*!
   * An helper structure used to extract members of a compound data
   * set.
   * \tparam T: type of the value to be extracted
   */
  template <typename T>
  struct CompoundExtractor : public CompoundExtractorBase {
    // inheriting constructors
    using CompoundExtractorBase::CompoundExtractorBase;
    /*!
     * \return a member of the compound of the given type
     * \param[in] d: raw data
     * \param[in] i: member index
     */
    T extract(const char* const d, const size_t) const;
  };  // end of CompoundExtractor

  /*!
   * An helper structure used to extract `std::string` members of a
   * compound data set.
   */
  template <>
  struct MADNEX_EXPORT CompoundExtractor<std::string>
      : public CompoundExtractorBase {
    // inheriting constructors
    using CompoundExtractorBase::CompoundExtractorBase;
    /*!
     * \return a member of the compound of the given type
     * \param[in] d: raw data
     * \param[in] i: member index
     */
    std::string extract(const char* const d, const size_t) const;
  };  // end of CompoundExtractor

  /*!
   * An helper structure used to extract `madnex::fcstring`
   * members of a compound data set.
   */
  template <std::size_t N>
  struct MADNEX_EXPORT CompoundExtractor<madnex::fcstring<N>>
      : public CompoundExtractorBase {
    // inheriting constructors
    using CompoundExtractorBase::CompoundExtractorBase;
    /*!
     * \return a member of the compound of the given type
     * \param[in] d: raw data
     * \param[in] i: member index
     */
    madnex::fcstring<N> extract(const char* const d, const size_t) const;
  };  // end of CompoundExtractor

}  // end of namespace madnex

#include "Madnex/CompoundExtractor.ixx"

#endif /* LIB_MADNEX__COMPOUNDEXTRACTOR_H */
