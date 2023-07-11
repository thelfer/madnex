/*!
 * \file   Scripts.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19/01/2017
 */

#ifndef LIB_MADNEX_SCRIPTS_HXX
#define LIB_MADNEX_SCRIPTS_HXX

#include <vector>
#include "Madnex/Config.hxx"
#include "Madnex/FCString.hxx"
#include "Madnex/HDF5.hxx"

namespace madnex {

  /*!
   * \brief a structure describing the experimental condition
   */
  struct MADNEX_EXPORT Script {
    //! \brief default constructor
    Script();
    //! \brief move constructor
    Script(Script&&);
    //! \brief copy constructor
    Script(const Script&);
    //! \brief move assignement
    Script& operator=(Script&&);
    //! \brief standard assignement
    Script& operator=(const Script&);
    //! \brief destructor
    ~Script();

    fcstring<100> author;
    fcstring<20> date;
    fcstring<500> command_line;
    fcstring<10> script_version;
  };  // end of struct Script

  //! a simple alias
  using Scripts = std::vector<Script>;

  /*!
   * \brief read a list of scripts in a group
   * \param[out] o: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the data
   */
  MADNEX_EXPORT void read(Scripts&, const Group&, const std::string&);
  /*!
   * \brief write a list of scripts in a group
   * \param[out] g: group
   * \param[in]  n: name of the data
   * \param[in]  o: object to be written
   * \param[in]  b: allow overwrite
   */
  MADNEX_EXPORT void write(Group&, const std::string&, const Scripts&,
			   const bool = false);

}  // end of namespace madnex

#endif /* LIB_MADNEX_SCRIPTS_HXX */
