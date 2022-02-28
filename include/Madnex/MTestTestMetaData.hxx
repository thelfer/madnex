/*!
 * \file   include/Madnex/MTestTestMetaData.hxx
 * \brief
 * \author Thomas Helfer
 * \date   18/02/2022
 */

#ifndef LIB_MADNEX_MTESTTESTMETADATA_HXX
#define LIB_MADNEX_MTESTTESTMETADATA_HXX

#include <string>
#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"

namespace madnex {

  /*!
   * \brief meta-data associated with an `MFront` implementation.
   */
  struct MADNEX_EXPORT MTestTestMetaData {
    //! \brief default constructor
    MTestTestMetaData();
    //! \brief move constructor
    MTestTestMetaData(MTestTestMetaData&&);
    //! \brief copy constructor
    MTestTestMetaData(const MTestTestMetaData&);
    //! \brief move constructor
    MTestTestMetaData& operator=(MTestTestMetaData&&);
    //! \brief copy constructor
    MTestTestMetaData& operator=(const MTestTestMetaData&);
    //! \brief destructor
    ~MTestTestMetaData() noexcept;
    //! \brief author of the implementation
    std::string author;
    //! \brief date of the implementation
    std::string date;
    //! \brief description of the implementation
    std::string description;
  };  // end of struct MTestTestMetaData

  /*!
   * \brief read the metadata of a `MFront` implementation in a group
   * \param[out] i: object to be read
   * \param[in]  g: group
   * \param[in]  n: path to the metadata
   */
  MADNEX_EXPORT void read(MTestTestMetaData&, const Group&, const std::string&);
  /*!
   * \brief write the metadata of a `MFront` implementation in a group
   * \param[out] g: group
   * \param[in]  n: path where the metadata will be written
   * \param[in]  d: metadata
   */
  MADNEX_EXPORT void write(Group&, const std::string&, const MTestTestMetaData&);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MTESTTESTMETADATA_HXX */
