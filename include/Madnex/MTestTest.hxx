/*!
 * \file   include/Madnex/MTestTest.hxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#ifndef LIB_MADNEX_MTESTTEST_HXX
#define LIB_MADNEX_MTESTTEST_HXX

#include <map>
#include <string>
#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"
#include "Madnex/MTestTestMetaData.hxx"

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
  struct MADNEX_EXPORT MTestTest {
    //! \brief default constructor
    MTestTest();
    //! \brief move constructor
    MTestTest(MTestTest&&);
    //! \brief copy constructor
    MTestTest(const MTestTest&);
    //! \brief move constructor
    MTestTest& operator=(MTestTest&&);
    //! \brief copy constructor
    MTestTest& operator=(const MTestTest&);
    //! \brief destructor
    ~MTestTest() noexcept;
    //! \brief name of the test
    std::string name;
    //! \brief core of the `MTest` test
    std::string test;
    //! \brief meta-data  describing the test
    MTestTestMetaData metadata;
  };  // end of struct MTestTest

  /*!
   * \brief read an `MTest` test in a group
   * \param[out] i: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the test
   */
  MADNEX_EXPORT void read(MTestTest&,
                          const Group&,
                          const std::string&);
  /*!
   * \brief write a `MTest` test in a group
   * \param[out] g: group
   * \param[in]  i: test
   */
  MADNEX_EXPORT void write(Group&, const MTestTest&);

  /*!
   * \brief display the complete description of a glossary entry
   * \param[in] f: file name
   * \param[in] m: material name
   * \param[in] b: behaviour name
   * \param[in] i: test name
   */
  MADNEX_EXPORT MTestTest getMTestTest(const std::string&,
                                       const std::string&,
                                       const std::string&,
                                       const std::string&);

  /*!
   * \brief display the complete description of a glossary entry
   * \param[in] f: file name
   * \param[in] b: behaviour name
   * \param[in] i: test name
   */
  MADNEX_EXPORT MTestTest getMTestTest(const std::string&,
                                       const std::string&,
                                       const std::string&);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MTESTTEST_HXX */
