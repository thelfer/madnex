/*!
 * \file   MainResults.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 janv. 2017
 */

#ifndef LIB_MADNEX_MAINRESULTS_HXX
#define LIB_MADNEX_MAINRESULTS_HXX

#include<vector>
#include"Madnex/Config.hxx"
#include"Madnex/HDF5.hxx"
#include"Madnex/FCString.hxx"

namespace madnex {

  /*!
   * \brief a structure describing the experimental condition
   */
  struct MADNEX_VISIBILITY_EXPORT MainResult
  {
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
    
    fcstring<32>  name;
    fcstring<256> label;
    fcstring<32> value;
    fcstring<16> unit;
    fcstring<16> type;
  }; // end of struct MainResult

  //! a simple alias
  using MainResults = std::vector<MainResult>;
  
  /*!
   * \brief read a set of main results in a group
   * \param[out] o: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the data
   */
  MADNEX_VISIBILITY_EXPORT void
  read(MainResults&,
       const Group&,
       const std::string&);
  /*!
   * \brief write a set of main results in a group
   * \param[out] g: group
   * \param[in]  n: name of the data
   * \param[in]  o: object to be written
   */
  MADNEX_VISIBILITY_EXPORT void
  write(Group&,
	const std::string&,
	const MainResults&);
  
} // end of namespace madnex

#endif /* LIB_MADNEX_MAINRESULTS_HXX */
