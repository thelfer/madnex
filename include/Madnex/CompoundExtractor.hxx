/*! 
 * \file   include/Madnex/CompoundExtractor.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDEXTRACTOR_H
#define LIB_MADNEX_COMPOUNDEXTRACTOR_H 

#ifndef LIB_MADNEX_COMPOUNDDATAVIEW_H
#error "This file shall not be included directly"
#endif
#ifndef LIB_MADNEX_COMPOUNDDATAVIEW_IXX
#error "This file shall not be included directly"
#endif

#include"TFEL/Utilities/FCString.hxx"

namespace madnex{

  /*!
   * \brief base class for all CompoundExtractor
   */
  struct MADNEX_VISIBILITY_EXPORT CompoundExtractorBase
  {
    /*!
     * \brief constructor
     * \param[in] v: compound view
     */
    CompoundExtractorBase(const CompoundDataView&);
    /*!
     * \param[in] e: expected type
     * \param[in] t: type contained in the the compound type
     */
    static void
    checkMemberClass(const PredType&,const H5T_class_t);
  protected:
    //! a compound data view
    const CompoundDataView& cview;
  }; // end of struct CompoundExtractorBase
  
  /*!
   * An helper structure used to extract members of a compound data
   * set.
   * \tparam T: type of the value to be extracted
   */
  template<typename T>
  struct CompoundExtractor
    : public CompoundExtractorBase
  {
    // inheriting constructors
    using CompoundExtractorBase::CompoundExtractorBase;
    /*!
     * \return a member of the compound of the given type
     * \param[in] n: member name
     */
    T extract(const std::string&) const;
    /*!
     * \return a member of the compound of the given type
     * \param[in] n: member name
     */
    T extract(const char *) const;
    /*!
     * \return a member of the compound of the given type
     * \param[in] i: member index
     */
    T extract(const size_t) const;
  }; // end of CompoundExtractor

  /*!
   * An helper structure used to extract `std::string` members of a
   * compound data set.
   */
  template<>
  struct MADNEX_VISIBILITY_EXPORT CompoundExtractor<std::string>
    : public CompoundExtractorBase
  {
    // inheriting constructors
    using CompoundExtractorBase::CompoundExtractorBase;
    /*!
     * \return a member of the compound of the given type
     * \param[in] n: member name
     */
    std::string extract(const std::string&) const;
    /*!
     * \return a member of the compound of the given type
     * \param[in] n: member name
     */
    std::string extract(const char *) const;
    /*!
     * \return a member of the compound of the given type
     * \param[in] i: member index
     */
    std::string extract(const size_t) const;
  }; // end of CompoundExtractor
  
} // end of namespace madnex

#include"Madnex/CompoundExtractor.ixx"

#endif /* LIB_MADNEX__COMPOUNDEXTRACTOR_H */
