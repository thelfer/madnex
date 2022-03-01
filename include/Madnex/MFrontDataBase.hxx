/*!
 * \file   include/Madnex/MFrontDataBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21/08/2020
 */

#ifndef LIB_MADNEX_MFRONTDATABASE_HXX
#define LIB_MADNEX_MFRONTDATABASE_HXX

#include <map>
#include <string>
#include <vector>
#include "Madnex/Config.hxx"
#include "Madnex/File.hxx"

namespace madnex {

  /*!
   * \brief description of the all the `MFront` files contained in a `madnex`
   * file.
   */
  struct MADNEX_EXPORT MFrontDataBase {
    /*!
     * \brief constructor
     * \param[in] f: madnex file
     */
    MFrontDataBase(const std::string&);
    //! \brief move constructor
    MFrontDataBase(MFrontDataBase&&);
    /*!
     * \return the list of all the materials which appears under the `MFront`
     * group.
     */
    std::vector<std::string> getMaterialsList() const;
    /*!
     * \return the list of all the `MFront` material properties contained in a
     * `madnex` file.
     */
    std::map<std::string, std::vector<std::string>>
    getAvailableMaterialProperties() const;
    /*!
     * \return the list of all the `MFront` material properties contained
     * associated with the given material
     * \param[in] m: material
     */
    std::vector<std::string> getAvailableMaterialProperties(
        const std::string&) const;
    /*!
     * \return the list of all the `MFront` behaviours contained in a
     * `madnex` file. This map is index by the material name,
     */
    std::map<std::string, std::vector<std::string>> getAvailableBehaviours()
        const;
    /*!
     * \return the list of all the `MFront` behaviours associate with the given
     * material.
     * \param[in] m: material
     */
    std::vector<std::string> getAvailableBehaviours(const std::string&) const;
    /*!
     * \return the list of all the `MFront` models contained in a
     * `madnex` file.
     */
    std::map<std::string, std::vector<std::string>> getAvailableModels() const;
    /*!
     * \return the list of all the `MFront` models associated with the given
     * material.
     * \param[in] m: material
     */
    std::vector<std::string> getAvailableModels(const std::string&) const;
    /*!
     * \return the list of all `MTest` tests associated with the given material
     * and behaviour.
     * \param[in] m: material
     * \param[in] b: behaviour
     */
    std::vector<std::string> getAvailableMTestTests(const std::string&,
                                                    const std::string&) const;
    /*!
     * \return the list of all `MTest` tests associated with the given
     * behaviour. The behaviour is assumed not to be associated with any
     * material.
     * \param[in] b: behaviour
     */
    std::vector<std::string> getAvailableMTestTests(const std::string&) const;
    //! \brief destructor
    ~MFrontDataBase();

   private:
    //! \brief copy constructor (disabled)
    MFrontDataBase(const MFrontDataBase&) = default;
    //! \brief standard assignement
    MFrontDataBase& operator=(const MFrontDataBase&) = default;
    //! \brief move assignement  (disabled)
    MFrontDataBase& operator=(MFrontDataBase&&) = default;
    //! underlying file
    File file;
  };  // end of struct MFrontDataBase

}  // end of namespace madnex

#endif /* LIB_MADNEX_MFRONTDATABASE_HXX */
