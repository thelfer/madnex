/*!
 * \file   include/Madnex/MFrontImplementation.hxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2020
 */

#ifndef LIB_MADNEX_MFRONTIMPLEMENTATION_HXX
#define LIB_MADNEX_MFRONTIMPLEMENTATION_HXX

#include <map>
#include <string>
#include "Madnex/Config.hxx"
#include "Madnex/HDF5.hxx"
#include "Madnex/MFrontMetaData.hxx"

namespace madnex {

  /*!
   * \brief This structure describes the an `MFront` implementation.
   *
   *
   * An implementation is described as:
   *
   * - a name.
   * - a source file.
   * - a set of meta data which may override the information given in the source
   *   file (author, date, description).
   * - a set of values for parameters which may override the values given in the
   *   source file. A material property in the original file may be converted to
   *   a parameter.
   * - a set of bounds describing the domain of validity of the considered
   *   material knowledge. This overrides any bounds found in the original file.
   */
  struct MADNEX_EXPORT MFrontImplementation {
    //! \brief default constructor
    MFrontImplementation();
    //! \brief move constructor
    MFrontImplementation(MFrontImplementation&&);
    //! \brief copy constructor
    MFrontImplementation(const MFrontImplementation&);
    //! \brief move constructor
    MFrontImplementation& operator=(MFrontImplementation&&);
    //! \brief copy constructor
    MFrontImplementation& operator=(const MFrontImplementation&);
    //! \brief destructor
    ~MFrontImplementation() noexcept;
    //! \brief name of the implementation
    std::string name;
    //! \brief source of the `MFront` implementation
    std::string source;
    //! \brief meta-data  describing the implementation
    MFrontMetaData metadata;
    //! \brief overriden parameters
    std::map<std::string, double> parameters;
    //! \brief lower bounds
    std::map<std::string, double> lower_bounds;
    //! \brief upper bounds
    std::map<std::string, double> upper_bounds;
  };  // end of struct MFrontImplementation

  /*!
   * \brief read an `MFront` implementation in a group
   * \param[out] i: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the implementation
   */
  MADNEX_EXPORT void read(MFrontImplementation&,
                          const Group&,
                          const std::string&);
  /*!
   * \brief write a `MFront` implementation in a group
   * \param[out] g: group
   * \param[in]  i: implementation
   */
  MADNEX_EXPORT void write(Group&, const MFrontImplementation&);

  /*!
   * \return an MFront implementation
   * \param[in] f: file name
   * \param[in] t: implementation type (must be either `material_property`,
   * `behaviour` or `model`)
   * \param[in] m: material name
   * \param[in] i: implementation name
   */
  MADNEX_EXPORT MFrontImplementation
  getMFrontImplementation(const std::string&,
                          const std::string&,
                          const std::string&,
                          const std::string&);

  /*!
   * \return an MFront implementation
   * \param[in] f: file name
   * \param[in] t: implementation type (must be either `material_property`,
   * `behaviour` or `model`)
   * \param[in] i: implementation name
   */
  MADNEX_EXPORT MFrontImplementation getMFrontImplementation(
      const std::string&, const std::string&, const std::string&);

}  // end of namespace madnex

#endif /* LIB_MADNEX_MFRONTIMPLEMENTATION_HXX */
