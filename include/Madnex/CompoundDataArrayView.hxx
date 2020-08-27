/*!
 * \file   CompoundDataArrayView.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   31 janv. 2017
 */

#ifndef LIB_MADNEX_COMPOUNDDATAARRAYVIEW_HXX
#define LIB_MADNEX_COMPOUNDDATAARRAYVIEW_HXX

#include<vector>
#include"Madnex/Config.hxx"
#include"Madnex/FCString.hxx"
#include"Madnex/HDF5.hxx"
#include"Madnex/Types.hxx"
#include"Madnex/CompoundDataView.hxx"

namespace madnex{

  /*!
   * An helper structure used to extract data out of an array of a
   * compound data set.
   */
  struct MADNEX_EXPORT CompoundDataArrayView
    : public CompoundDataViewBase
  {
    //! a simple alias
    using size_type = hsize_t;
    /*!
     * \brief constructor
     * \param[in] g: group
     * \param[in] p: data set path
     */
    CompoundDataArrayView(const Group&,const std::string&);
    /*!
     * \brief constructor
     * \param[in] d: data set
     */
    CompoundDataArrayView(const DataSet&);
    //! destructor
    ~CompoundDataArrayView();
    //! \return a view to the ith elemnent of the compound data array
    CompoundDataView operator[](const size_type i) const;
    //! \return a view to the ith elemnent of the compound data array
    CompoundDataView at(const size_type i) const;
    /*!
     * \return an array containing all the members of the compound
     * data array of the given type
     * \param[in] n: member name
     */
    template<typename T>
    std::vector<T> extract(const std::string&) const;
    /*!
     * \return an array containing all the members of the compound
     * data array of the given type
     * \param[in] n: member name
     */
    template<typename T>
    std::vector<T> extract(const char *) const;
    /*!
     * \return an array containing all the members of the compound
     * data array of the given type
     * \param[in] i: member index
     */
    template<typename T>
    std::vector<T> extract(const size_t) const;
    //! \return the number of elements in the compound data array
    size_type size() const;
  private:
    // number of elements in the compound data array
    size_type s;
    //! an intermediate storage for the compound data type
    std::vector<char> rdata;

  }; // end of struct CompoundDataArrayView

} // end of namespace madnex

#include"Madnex/CompoundDataArrayView.ixx"

#endif /* LIB_MADNEX_COMPOUNDDATAARRAYVIEW_HXX */
