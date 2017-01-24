/*! 
 * \file   include/Madnex/CompoundDataView.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDDATAVIEW_H
#define LIB_MADNEX_COMPOUNDDATAVIEW_H 

#include<vector>
#include"H5Cpp.h"
#include"TFEL/Utilities/FCString.hxx"
#include"Madnex/Config.hxx"
#include"Madnex/HDF5.hxx"
#include"Madnex/Types.hxx"

namespace madnex{

  /*!
   * An helper structure used to extract members of a compound data
   * set.
   */
  struct MADNEX_VISIBILITY_EXPORT CompoundDataView
  {
    /*!
     * \brief constructor
     * \param[in] d: data set
     */
    CompoundDataView(const DataSet&);
    //! destructor
    ~CompoundDataView();
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
    /*!
     * \return the index of the member
     * \param[in] n: member name
     */
    size_t getMemberIndex(const char *) const;
    /*!
     * \return the index of the member
     * \param[in] n: member name
     */
    size_t getMemberIndex(const std::string&) const;
    /*!
     * \return the class of the member
     * \param[in] i: member index
     */
    H5T_class_t getMemberClass(const size_t) const;
    /*!
     * \return the offset of the member
     * \param[in] i: member index
     */
    size_t getMemberOffset(const size_t) const;
    /*!
     * \return the string type of the member
     * \param[in] i: member index
     */
    StrType getMemberStrType(const size_t) const;
    /*!
     * \return the raw buffer in which the HDF5
     */
    const char* data() const;
  private:
    //! the description of the compound data type
    H5::CompType ctype;
    //! an intermediate storage for the compound data type
    std::vector<char> rdata;
  }; // end of CompoundDataView

} // end of namespace madnex

#include"Madnex/CompoundDataView.ixx"

#endif /* LIB_MADNEX__COMPOUNDDATAVIEW_H */
