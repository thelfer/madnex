/*!
 * \file   Scripts.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 janv. 2017
 */

#ifndef LIB_MADNEX_SCRIPTS_HXX
#define LIB_MADNEX_SCRIPTS_HXX

#include<vector>
#include"Madnex/Config.hxx"
#include"Madnex/FCString.hxx"
#include"Madnex/HDF5.hxx"

namespace madnex {

  /*!
   * \brief a structure describing the experimental condition
   */
  struct MADNEX_EXPORT Script
  {
    //! default constructor
    Script();
    //! move constructor
    Script(Script&&);
    //! copy constructor
    Script(const Script&);
    //! move assignement
    Script& operator=(Script&&);
    //! standard assignement
    Script& operator=(const Script&);
    
    fcstring<100> author;
    fcstring<20> date;
    fcstring<500> command_line;
    fcstring<10> script_version;
  }; // end of struct Script

  //! a simple alias
  using Scripts = std::vector<Script>;
  
  /*!
   * \brief read a list of scripts in a group
   * \param[out] o: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the data
   */
  MADNEX_EXPORT void
  read(Scripts&,
       const Group&,
       const std::string&);
  /*!
   * \brief write a list of scripts in a group
   * \param[out] g: group
   * \param[in]  n: name of the data
   * \param[in]  o: object to be written
   */
  MADNEX_EXPORT void
  write(Group&,
	const std::string&,
	const Scripts&);
  
} // end of namespace madnex

#endif /* LIB_MADNEX_SCRIPTS_HXX */
