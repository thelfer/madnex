/*!
 * \file   include/Madnex/MFMTestGeneratorTest.hxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#ifndef LIB_MADNEX_MFEM_TEST_GENERATOR_TEST_HXX
#define LIB_MADNEX_MFEM_TEST_GENERATOR_TEST_HXX

#include <map>
#include <string>
#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"
#include "Madnex/MFMTestGeneratorTestMetaData.hxx"

namespace madnex {

  /*!
   * \brief This structure describes the an `MTest` test.
   *
   * An test is described as:
   *
   * - a name.
   * - an `MTest` file.
   * - a set of meta data which may override the information given in the source
   *   file (author, date, description).
   */
  struct MADNEX_EXPORT MFMTestGeneratorTest {
    //! \brief default constructor
    MFMTestGeneratorTest();
    //! \brief move constructor
    MFMTestGeneratorTest(MFMTestGeneratorTest&&);
    //! \brief copy constructor
    MFMTestGeneratorTest(const MFMTestGeneratorTest&);
    //! \brief move constructor
    MFMTestGeneratorTest& operator=(MFMTestGeneratorTest&&);
    //! \brief copy constructor
    MFMTestGeneratorTest& operator=(const MFMTestGeneratorTest&);
    //! \brief destructor
    ~MFMTestGeneratorTest() noexcept;
    //! \brief name of the test
    std::string name;
    //! \brief core of the `MTest` test
    std::string test;
    //! \brief meta-data  describing the test
    MFMTestGeneratorTestMetaData metadata;
  };  // end of struct MFMTestGeneratorTest

  /*!
   * \brief read an `MTest` test in a group
   * \param[out] i: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the test
   */
  MADNEX_EXPORT void read(MFMTestGeneratorTest&,
                          const Group&,
                          const std::string&);
  /*!
   * \brief write a `MTest` test in a group
   * \param[out] g: group
   * \param[in]  i: test
   */
  MADNEX_EXPORT void write(Group&, const MFMTestGeneratorTest&);

  /*!
   * \return a complete description of an MTest test
   * \param[in] f: file name
   * \param[in] m: material name
   * \param[in] b: behaviour name
   * \param[in] i: test name
   */
  MADNEX_EXPORT MFMTestGeneratorTest
  getMFMTestGeneratorTest(const std::string&,
                          const std::string&,
                          const std::string&,
                          const std::string&);

  /*!
   * \return a complete description of an MTest test
   * \param[in] f: file name
   * \param[in] b: behaviour name
   * \param[in] i: test name
   */
  MADNEX_EXPORT MFMTestGeneratorTest getMFMTestGeneratorTest(
							     const std::string&, const std::string&, const std::string&);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MFEM_TEST_GENERATOR_TEST_HXX */
