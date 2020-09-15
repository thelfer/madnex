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
    MFrontMetaData() noexcept;
    //! \brief move constructor
    MFrontMetaData(MFrontMetaData&&) ;
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
    //! \brief material
    std::string material;
  }; // end of struct MFrontMetaData

  /*!
   * \brief read an `MFront` implementation in a group
   * \param[out] i: object to be read
   * \param[in]  g: group
   * \param[in]  n: destination
   */
  MADNEX_EXPORT void read(MFrontMetaData&, const Group&, const std::string&);
  /*!
   * \brief write a `MFront` implementation in a group
   * \param[out] g: group
   * \param[in]  d: data
   */
  MADNEX_EXPORT void write(Group&, const std::string&, const MFrontMetaData&);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MFRONTMETADATA_HXX */
