/*!
 * \file   include/Madnex/MFMTestGeneratorTestMetaData.hxx
 * \brief
 * \author Thomas Helfer
 * \date   18/02/2022
 */

#ifndef LIB_MADNEX_MFEM_TEST_GENERATOR_TEST_METADATA_HXX
#define LIB_MADNEX_MFEM_TEST_GENERATOR_TEST_METADATA_HXX

#include <string>
#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"

namespace madnex {

  /*!
   * \brief meta-data associated with an `mfm-test-generator` test.
   */
  struct MADNEX_EXPORT MFMTestGeneratorTestMetaData {
    //! \brief default constructor
    MFMTestGeneratorTestMetaData();
    //! \brief move constructor
    MFMTestGeneratorTestMetaData(MFMTestGeneratorTestMetaData&&);
    //! \brief copy constructor
    MFMTestGeneratorTestMetaData(const MFMTestGeneratorTestMetaData&);
    //! \brief move constructor
    MFMTestGeneratorTestMetaData& operator=(MFMTestGeneratorTestMetaData&&);
    //! \brief copy constructor
    MFMTestGeneratorTestMetaData& operator=(
        const MFMTestGeneratorTestMetaData&);
    //! \brief destructor
    ~MFMTestGeneratorTestMetaData() noexcept;
    //! \brief author of the implementation
    std::string author;
    //! \brief date of the implementation
    std::string date;
    //! \brief description of the implementation
    std::string description;
  };  // end of struct MFMTestGeneratorTestMetaData

  /*!
   * \brief read the metadata of a `mfm-test-generator` test in a group
   * \param[out] i: object to be read
   * \param[in]  g: group
   * \param[in]  n: path to the metadata
   */
  MADNEX_EXPORT void read(MFMTestGeneratorTestMetaData&,
                          const Group&,
                          const std::string&);
  /*!
   * \brief write the metadata of a `mfm-test-generator` test in a group
   * \param[out] g: group
   * \param[in]  n: path where the metadata will be written
   * \param[in]  d: metadata
   */
  MADNEX_EXPORT void write(Group&,
                           const std::string&,
                           const MFMTestGeneratorTestMetaData&);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MFEM_TEST_GENERATOR_TEST_METADATA_HXX */
