/*!
 * \file   include/Madnex/CompoundDataView.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  22/01/2017
 */

#ifndef LIB_MADNEX_COMPOUNDDATAVIEW_H
#define LIB_MADNEX_COMPOUNDDATAVIEW_H

#include <vector>
#include "Madnex/Config.hxx"
#include "Madnex/FCString.hxx"
#include "Madnex/HDF5.hxx"
#include "Madnex/Types.hxx"
#include "Madnex/CompoundDataViewBase.hxx"

namespace madnex {

  // forward declaration
  struct CompoundDataArrayView;

  /*!
   * An helper structure used to extract members of a compound data
   * set.
   */
  struct MADNEX_EXPORT CompoundDataView
      : public CompoundDataViewBase {
    /*!
     * \brief constructor
     * \param[in] g: group
     * \param[in] p: data set path
     */
    CompoundDataView(const Group&, const std::string&);
    /*!
     * \brief constructor
     * \param[in] d: data set
     */
    CompoundDataView(const DataSet&);
    // move constructor
    CompoundDataView(CompoundDataView&&);
    // copy constructor
    CompoundDataView(const CompoundDataView&);
    // move assignement
    CompoundDataView& operator=(CompoundDataView&&);
    // copy assignement
    CompoundDataView& operator=(const CompoundDataView&);
    //! destructor
    ~CompoundDataView();
    /*!
     * \return a member of the compound of the given type
     * \param[in] n: member name
     */
    template <typename T>
    T extract(const std::string&) const;
    /*!
     * \return a member of the compound of the given type
     * \param[in] n: member name
     */
    template <typename T>
    T extract(const char*) const;
    /*!
     * \return a member of the compound of the given type
     * \param[in] i: member index
     */
    template <typename T>
    T extract(const size_t) const;

   private:
    //  CompoundDataArrayView is made friend to be able to access the
    //  private constructor
    friend struct CompoundDataArrayView;
    /*!
     * \brief constructor
     * \param[in] b: base class describing the compound data
     * \param[in] rd: data
     */
    CompoundDataView(const CompoundDataViewBase&, const char* const);
    //! \brief an intermediate storage for the compound data type
    std::vector<char> data_storage;
    //! \brief pointer to the data
    const char* data_pointer;
  };  // end of CompoundDataView
  /*!
   * \brief extract a data from a CompoundDataView
   * \tparam     T: type to be extracted
   * \param[out] v: value
   * \param[in]  d: data view
   * \param[in]  i: data index
   */
  template <typename T>
  void extract(T&, const CompoundDataView&, const size_t);
  /*!
   * \brief extract a data from a CompoundDataView
   * \tparam     T: type to be extracted
   * \param[out] v: value
   * \param[in]  d: data view
   * \param[in]  i: data name
   */
  template <typename T>
  void extract(T&, const CompoundDataView&, const char* const);
  /*!
   * \brief extract a data from a CompoundDataView
   * \tparam     T: type to be extracted
   * \param[out] v: value
   * \param[in]  d: data view
   * \param[in]  i: data name
   */
  template <typename T>
  void extract(T&, const CompoundDataView&, const std::string&);

}  // end of namespace madnex

#include "Madnex/CompoundDataView.ixx"

#endif /* LIB_MADNEX__COMPOUNDDATAVIEW_H */
