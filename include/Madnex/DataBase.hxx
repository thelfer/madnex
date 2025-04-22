/*!
 * \file   include/Madnex/DataBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21/08/2020
 */

#ifndef LIB_MADNEX_DATABASE_HXX
#define LIB_MADNEX_DATABASE_HXX

#include <map>
#include <string>
#include <vector>
#include "Madnex/Config.hxx"
#include "Madnex/File.hxx"

namespace madnex {

  /*!
   * \brief description of the all the information files contained in a `madnex`
   * file.
   */
  struct MADNEX_EXPORT DataBase {
    /*!
     * \brief constructor
     * \param[in] f: madnex file
     */
    DataBase(const std::string&);
    //! \brief move constructor
    DataBase(DataBase&&);
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
    /*!
     * \return the list of all `mfm-test-generator` tests associated with the
     * given material and behaviour.
     * \param[in] m: material
     * \param[in] b: behaviour
     */
    std::vector<std::string> getAvailableMFMTestGeneratorTests(
        const std::string&, const std::string&) const;
    /*!
     * \return the list of all `mfm-test-generator` tests associated with the
     * given behaviour. The behaviour is assumed not to be associated with any
     * material.
     * \param[in] b: behaviour
     */
    std::vector<std::string> getAvailableMFMTestGeneratorTests(
        const std::string&) const;
    //! \brief destructor
    ~DataBase();

   private:
    //! \brief copy constructor (disabled)
    DataBase(const DataBase&) = default;
    //! \brief standard assignement
    DataBase& operator=(const DataBase&) = default;
    //! \brief move assignement  (disabled)
    DataBase& operator=(DataBase&&) = default;
    /*!
     * \brief check if the given material exists, throws otherwise
     * \param[in] m: material name
     */
    void checkIfMaterialExists(const std::string&) const;
    //! underlying file
    File file;
  };  // end of struct DataBase

  /*!
   * \return the list of materials matching the given regular expression
   * \param[in] m: regex filtering the materials
   */
  MADNEX_EXPORT std::vector<std::string> getMatchingMaterials(
      const DataBase&, const std::string&);
  /*!
   * \return the list of material properties matching the given regular
   * expressions.
   * \param[in] m: regex filtering the materials
   * \param[in] mp: regex filtering the material propertiess
   */
  MADNEX_EXPORT std::vector<std::string> getMatchingMaterialProperties(
      const DataBase&, const std::string&);
  /*!
   * \return the list of behaviours matching the given regular expressions.
   * \param[in] m: regex filtering the materials
   * \param[in] mp: regex filtering the behaviours
   */
  MADNEX_EXPORT std::vector<std::string> getMatchingBehaviours(
      const DataBase&, const std::string&);
  /*!
   * \return the list of models matching the given regular expressions.
   * \param[in] m: regex filtering the materials
   * \param[in] mp: regex filtering the models
   */
  MADNEX_EXPORT std::vector<std::string> getMatchingModels(const DataBase&,
                                                           const std::string&);

}  // end of namespace madnex

#endif /* LIB_MADNEX_DATABASE_HXX */
