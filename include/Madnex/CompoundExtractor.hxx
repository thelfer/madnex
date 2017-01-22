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
    //! the description of the compound data type
    H5::CompType ctype;
    //! an intermediate storage for the compound data type
    std::vector<char> data;
  }; // end of CompoundExtractor

  template<typename T>
  T CompoundExtractor::extract(const char *n) const{
    const auto i = this->ctype.getMemberIndex(n);
    const auto o = this->ctype.getMemberOffset(i);
    return *(reinterpret_cast<const T *>(this->data.data()+o));
  } // end of CompoundExtractor::extract

  template<typename T>
  T CompoundExtractor::extract(const std::string& n) const{
    const auto i = this->ctype.getMemberIndex(n);
    const auto o = this->ctype.getMemberOffset(i);
    return *(reinterpret_cast<const T *>(this->data.data()+o));
  } // end of CompoundExtractor::extract
  
} // end of namespace madnex

#endif /* LIB_MADNEX__COMPOUNDEXTRACTOR_H */
