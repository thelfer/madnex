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
#include "Madnex/FCString.hxx"

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
    //! \brief date
    std::string date;
    //! \brief status
    std::string status;
    //! \brief description of the implemnetation
    std::string description;
    //! \brief unit system
    std::string unit_system;
    //! \brief universally unique identifier
    std::string uuid;
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
   * \param[in]  b: allow overwrite
   */
  MADNEX_EXPORT void write(Group&, const std::string&, const MFrontMetaData&,
			   const bool = false);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MFRONTMETADATA_HXX */
