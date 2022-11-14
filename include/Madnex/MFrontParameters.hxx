/*!
 * \file   MFrontParameters.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   19 janv. 2017
 */

#ifndef LIB_MADNEX_MFRONTPARAMETERS_HXX
#define LIB_MADNEX_MFRONTPARAMETERS_HXX

#include <vector>
#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"
#include "Madnex/FCString.hxx"

namespace madnex {

  /*!
   * \brief a structure describing the experimental condition
   */
  struct MADNEX_EXPORT MFrontParameter {
    //! default constructor
    MFrontParameter();
    //! move constructor
    MFrontParameter(MFrontParameter&&);
    //! copy constructor
    MFrontParameter(const MFrontParameter&);
    //! move assignement
    MFrontParameter& operator=(MFrontParameter&&);
    //! standard assignement
    MFrontParameter& operator=(const MFrontParameter&);

    fcstring<32> code;
    fcstring<512> name;
    fcstring<16> type;
    fcstring<512> value;
    fcstring<16> unit;
  };  // end of struct MFrontParameter

  //! a simple wrapper around `std::vector<MFrontParameter>`
  struct MFrontParameters : public std::vector<MFrontParameter> {
    // inheriting std::vector constructors
    using std::vector<MFrontParameter>::vector;
  };  // end of struct MFrontParameters

  /*!
   * \brief read a set of main results in a group
   * \param[out] o: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the data
   */
  MADNEX_EXPORT void read(MFrontParameters&, const Group&, const std::string&);
  /*!
   * \brief write a set of main results in a group
   * \param[out] g: group
   * \param[in]  n: name of the data
   * \param[in]  o: object to be written
   */
  MADNEX_EXPORT void write(Group&, const std::string&, const MFrontParameters&);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MFRONTPARAMETERS_HXX */
