/*!
 * \file   include/Madnex/File.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 janv. 2017
 */

#ifndef LIB_MADNEX_FILE_HXX
#define LIB_MADNEX_FILE_HXX

#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"

namespace madnex{

  /*!
   * \brief a simple wrapper around the `H5File` class.
   */
  struct MADNEX_VISIBILITY_EXPORT File
    : H5::H5File
  {
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
  }; // end of struct File
    
} // end of namespace madnex

#endif /* LIB_MADNEX_FILE_HXX */
