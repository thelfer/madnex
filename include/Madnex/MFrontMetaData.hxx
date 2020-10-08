/*!
 * \file   include/Madnex/MFrontMetaData.hxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#ifndef LIB_MADNEX_MFRONTMETADATA_HXX
#define LIB_MADNEX_MFRONTMETADATA_HXX

#include <string>
#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"

namespace madnex {

  /*!
   * \brief meta-data associated with an `MFront` implementation.
   */
  struct MADNEX_EXPORT MFrontMetaData {
    //! \brief default constructor
    MFrontMetaData();
    //! \brief move constructor
    MFrontMetaData(MFrontMetaData&&);
    //! \brief copy constructor
    MFrontMetaData(const MFrontMetaData&);
    //! \brief move constructor
    MFrontMetaData& operator=(MFrontMetaData&&);
    //! \brief copy constructor
    MFrontMetaData& operator=(const MFrontMetaData&);
    //! \brief destructor
    ~MFrontMetaData() noexcept;
    //! \brief author of the implementation
    std::string author;
    //! \brief date of the implementation
    std::string date;
    //! \brief description of the implementation
    std::string description;
  };  // end of struct MFrontMetaData

  /*!
   * \brief read the metadata of a `MFront` implementation in a group
   * \param[out] i: object to be read
   * \param[in]  g: group
   * \param[in]  n: path to the metadata
   */
  MADNEX_EXPORT void read(MFrontMetaData&, const Group&, const std::string&);
  /*!
   * \brief write the metadata of a `MFront` implementation in a group
   * \param[out] g: group
   * \param[in]  n: path where the metadata will be written
   * \param[in]  d: metadata
   */
  MADNEX_EXPORT void write(Group&, const std::string&, const MFrontMetaData&);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MFRONTMETADATA_HXX */
