/*!
 * \file   include/Madnex/File.hxx
 * \brief
 * \author Thomas Helfer
 * \date   16/01/2017
 */

#ifndef LIB_MADNEX_FILE_HXX
#define LIB_MADNEX_FILE_HXX

#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"

namespace madnex {

  /*!
   * \brief a simple wrapper around the `H5File` class.
   */
  struct MADNEX_EXPORT File : H5::H5File {
    // inheriting constructors
    using H5::H5File::H5File;
    //! return the root of the tree
    Group getRoot();
    //! return the root of the tree
    Group getRoot() const;

   protected:
    /*!
     * \return true if the given group is writable
     * \param[in] g: group name
     */
    bool checkIfGroupIsWritable(const std::string&) const;
  };  // end of struct File

  /*!
   * \brief check if the given path exists in the given file
   * \param[in] f: file
   * \param[in] p: path
   */
  MADNEX_EXPORT bool exists(const File&, const std::string&);


}  // end of namespace madnex

#endif /* LIB_MADNEX_FILE_HXX */
