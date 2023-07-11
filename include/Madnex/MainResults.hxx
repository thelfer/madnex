/*!
 * \file   MainResults.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   19 janv. 2017
 */

#ifndef LIB_MADNEX_MAINRESULTS_HXX
#define LIB_MADNEX_MAINRESULTS_HXX

#include <vector>
#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"
#include "Madnex/FCString.hxx"

namespace madnex {

  /*!
   * \brief a structure describing the experimental condition
   */
  struct MADNEX_EXPORT MainResult {
    //! default constructor
    MainResult();
    //! move constructor
    MainResult(MainResult&&);
    //! copy constructor
    MainResult(const MainResult&);
    //! move assignement
    MainResult& operator=(MainResult&&);
    //! standard assignement
    MainResult& operator=(const MainResult&);

    fcstring<32> name;
    fcstring<512> label;
    fcstring<256> value;
    fcstring<16> unit;
    fcstring<16> type;
  };  // end of struct MainResult

  //! a simple wrapper around `std::vector<MainResult>`
  struct MainResults : public std::vector<MainResult> {
    //! standard name of the group containing the main results in madnex 1.4
    static constexpr const char* path = "resultats/resultats_principaux";
    //! standard name of the group containing the main results in madnex 1.4
    static constexpr const char* groupPath = "resultats";
    //! standard name of the dataset containing the main results in madnex 1.4
    static constexpr const char* dataSetName = "resultats_principaux";
    // inheriting std::vector constructors
    using std::vector<MainResult>::vector;
  };  // end of struct MainResults

  /*!
   * \brief read a set of main results in a group
   * \param[out] o: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the data
   */
  MADNEX_EXPORT void read(MainResults&, const Group&, const std::string&);
  /*!
   * \brief write a set of main results in a group
   * \param[out] g: group
   * \param[in]  n: name of the data
   * \param[in]  o: object to be written
   * \param[in]  b: allow overwrite
   */
  MADNEX_EXPORT void write(Group&, const std::string&, const MainResults&,
			   const bool = false);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MAINRESULTS_HXX */
