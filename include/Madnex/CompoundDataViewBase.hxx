/*! 
 * \file   include/Madnex/CompoundDataViewBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDDATAVIEWBASE_H
#define LIB_MADNEX_COMPOUNDDATAVIEWBASE_H 

#include<vector>
#include"Madnex/Config.hxx"
#include"Madnex/FCString.hxx"
#include"Madnex/HDF5.hxx"
#include"Madnex/Types.hxx"

namespace madnex{

  /*!
   * An helper structure used to extract members of a compound data
   * set.
   */
  struct MADNEX_EXPORT CompoundDataViewBase
  {
    /*!
     * \brief constructor
     * \param[in] d: data set
     */
    CompoundDataViewBase(const DataSet&);
    //! \brief move constructor
    CompoundDataViewBase(CompoundDataViewBase&&);
    //! \brief copy constructor
    CompoundDataViewBase(const CompoundDataViewBase&);
    //! \brief move assignement
    CompoundDataViewBase& operator=(CompoundDataViewBase&&);
    //! \brief standard assignement
    CompoundDataViewBase& operator=(const CompoundDataViewBase&);
    //! destructor
    ~CompoundDataViewBase();
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
     * \brief return the index of the member with the given name; if no
     * member has this name, return the given fallback index. This allows
     * reading files written by foreign (or legacy) applications whose
     * members carry different names but kept the same relative order.
     * \param[in] n: member name
     * \param[in] f: fallback index
     */
    size_t getMemberIndexOr(const char*, const size_t) const;
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
  protected:
    /*!
     * \return a member of the compound of the given type
     * \param[in] d: data
     * \param[in] i: member index
     */
    template<typename T>
     T extract(const char* const,
	       const size_t) const;
    //! the description of the compound data type
    H5::CompType ctype;
  }; // end of CompoundDataViewBase

} // end of namespace madnex

#include"Madnex/CompoundDataViewBase.ixx"

#endif /* LIB_MADNEX__COMPOUNDDATAVIEWBASE_H */
