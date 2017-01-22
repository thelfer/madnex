/*! 
 * \file   include/Madnex/CompoundExtractor.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDEXTRACTOR_H
#define LIB_MADNEX_COMPOUNDEXTRACTOR_H 

#include<vector>
#include"H5Cpp.h"
#include"Madnex/Config.hxx"
#include"Madnex/HDF5.hxx"
#include"Madnex/Types.hxx"

namespace madnex{

  /*!
   * An helper structure used to extract members of a compound data
   * set.
   */
  struct MADNEX_VISIBILITY_EXPORT CompoundExtractor
  {
    /*!
     * \brief constructor
     * \param[in] d: data set
     */
    CompoundExtractor(const DataSet&);
    //! destructor
    ~CompoundExtractor();
    /*!
     * \return a member of the compound of the given type
     * \param[in] n: member name
     */
    template<typename T>
    T extract(const std::string&) const;
    /*!
     * \return a member of the compound of the given type
     * \param[in] n: member name
     */
    template<typename T>
    T extract(const char *) const;

  private:
    /*!
     * \param[in] e: expected type
     * \param[in] t: type contained in the the compound type
     */
    static void
    checkMemberClass(const PredType&,
		     const H5T_class_t);
    //! the description of the compound data type
    H5::CompType ctype;
    //! an intermediate storage for the compound data type
    std::vector<char> data;
  }; // end of CompoundExtractor

  // partial specialisation for string's
  template<>
  std::string CompoundExtractor::extract(const std::string&) const;
  // partial specialisation for string's
  template<>
  std::string CompoundExtractor::extract(const char*) const;
  
} // end of namespace madnex

#include"Madnex/CompoundExtractor.ixx"

#endif /* LIB_MADNEX__COMPOUNDEXTRACTOR_H */
