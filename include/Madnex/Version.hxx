/*!
 * \file   Version.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 janv. 2017
 */

#ifndef LIB_MADNEX_VERSION_HXX
#define LIB_MADNEX_VERSION_HXX

#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"

namespace madnex{

  struct MADNEX_VISIBILITY_EXPORT Version
  {
    //! default constructor
    Version();
    //! move constructor
    Version(Version&&);
    //! copy constructor
    Version(const Version&);
    //! move assignement
    Version& operator=(Version&&);
    //! standard assignement
    Version& operator=(const Version&);
    std::string major;
    std::string minor;
    std::string revision;
  }; // end of struct Version

  /*!
   * \brief read a version description in a group
   * \param[out] o: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the data
   */
  MADNEX_VISIBILITY_EXPORT void
  read(Version&,
       const Group&,
       const std::string&);
  /*!
   * \brief write a version description in a group
   * \param[out] g: group
   * \param[in]  n: name of the data
   * \param[in]  o: object to be written
   */
  MADNEX_VISIBILITY_EXPORT void
  write(Group&,
	const std::string&,
	const Version&);
  
} // end of namespace madnex

#endif /* LIB_MADNEX_VERSION_HXX */
