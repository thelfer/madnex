/*!
 * \file   src/DataBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21/08/2020
 */

#include "Madnex/Raise.hxx"
#include "Madnex/DataBase.hxx"

namespace madnex {

  static bool checkMFrontGroup(const File& f) {
    const auto r = f.getRoot();
    if (!exists(r, "MFront")) {
      return false;
    }
    if (!subGroupExists(r, "MFront")) {
      raise("`MFront` is not a sub group of the file");
    }
    return true;
  }  // end of checkMFrontGroup

  static std::vector<std::string> getSubGroupIdentifiers(const Group& gr,
                                                         const std::string& p) {
    if (!exists(gr, p)) {
      return {};
    }
    auto groups = std::vector<std::string>{};
    auto g = openGroup(gr, p);
    for (decltype(g.getNumObjs()) i = 0; i != g.getNumObjs(); ++i) {
      if (g.getObjTypeByIdx(i) != H5G_GROUP) {
        continue;
      }
      groups.push_back(g.getObjnameByIdx(i));
    }
    return groups;
  }  // end of getSubGroupIdentifiers

  DataBase::DataBase(const std::string& f)
      : file(f, H5F_ACC_RDONLY) {}  // end of DataBase::DataBase

  DataBase::DataBase(DataBase&&) = default;

  std::vector<std::string> DataBase::getMaterialsList() const {
    if (!checkMFrontGroup(this->file)) {
      return {};
    }
    const auto r = this->file.getRoot();
    auto materials = std::vector<std::string>{};
    auto g = openGroup(this->file.getRoot(), "MFront");
    for (decltype(g.getNumObjs()) i = 0; i != g.getNumObjs(); ++i) {
      if (g.getObjTypeByIdx(i) != H5G_GROUP) {
        continue;
      }
      const auto n = g.getObjnameByIdx(i);
      if (!((n == "MaterialProperties") || (n == "Behaviours") ||
            (n == "Models"))) {
        materials.push_back(n);
      }
    }
    return materials;
  }  // end of DataBase::getMaterialsList

  std::map<std::string, std::vector<std::string>>
  DataBase::getAvailableMaterialProperties() const {
    auto mmps = std::map<std::string, std::vector<std::string>>{};
    auto insert_if = [&mmps](const std::string& m,
                             std::vector<std::string> mps) {
      if (!mps.empty()) {
        mmps.insert({m, std::move(mps)});
      }
    };
    const auto r = this->file.getRoot();
    insert_if("", getSubGroupIdentifiers(r, "MFront/MaterialProperties"));
    for (const auto& m : this->getMaterialsList()) {
      insert_if(
          m, getSubGroupIdentifiers(r, "MFront/" + m + "/MaterialProperties"));
    }
    return mmps;
  }  // end of DataBase::getAvailableMaterialProperties

  std::vector<std::string> DataBase::getAvailableMaterialProperties(
      const std::string& m) const {
    if (!checkMFrontGroup(this->file)) {
      return {};
    }
    const auto r = this->file.getRoot();
    if (!subGroupExists(r, "MFront/" + m)) {
      raise(
          "DataBase::getAvailableMaterialProperties: no material "
          "named '" +
          m + "'");
    }
    return getSubGroupIdentifiers(r, "MFront/" + m + "/MaterialProperties");
  }  // end of DataBase::getAvailableMaterialProperties

  std::map<std::string, std::vector<std::string>>
  DataBase::getAvailableBehaviours() const {
    auto mbs = std::map<std::string, std::vector<std::string>>{};
    auto insert_if = [&mbs](const std::string& m, std::vector<std::string> bs) {
      if (!bs.empty()) {
        mbs.insert({m, std::move(bs)});
      }
    };
    const auto r = this->file.getRoot();
    insert_if("", getSubGroupIdentifiers(r, "MFront/Behaviours"));
    for (const auto& m : this->getMaterialsList()) {
      insert_if(m, getSubGroupIdentifiers(r, "MFront/" + m + "/Behaviours"));
    }
    return mbs;
  }  // end of DataBase::getAvailableBehaviours

  std::vector<std::string> DataBase::getAvailableBehaviours(
      const std::string& m) const {
    if (!checkMFrontGroup(this->file)) {
      return {};
    }
    const auto r = this->file.getRoot();
    if (!subGroupExists(r, "MFront/" + m)) {
      raise(
          "DataBase::getAvailableBehaviours: no material "
          "named '" +
          m + "'");
    }
    return getSubGroupIdentifiers(r, "MFront/" + m + "/Behaviours");
  }  // end of DataBase::getAvailableBehaviours

  std::map<std::string, std::vector<std::string>>
  DataBase::getAvailableModels() const {
    auto mms = std::map<std::string, std::vector<std::string>>{};
    auto insert_if = [&mms](const std::string& m, std::vector<std::string> ms) {
      if (!ms.empty()) {
        mms.insert({m, std::move(ms)});
      }
    };
    const auto r = this->file.getRoot();
    insert_if("", getSubGroupIdentifiers(r, "MFront/Models"));
    for (const auto& m : this->getMaterialsList()) {
      insert_if(m, getSubGroupIdentifiers(r, "MFront/" + m + "/Models"));
    }
    return mms;
  }  // end of DataBase::getAvailableModels

  std::vector<std::string> DataBase::getAvailableModels(
      const std::string& m) const {
    if (!checkMFrontGroup(this->file)) {
      return {};
    }
    const auto r = this->file.getRoot();
    if (!subGroupExists(r, "MFront/" + m)) {
      raise(
          "DataBase::getAvailableModels: no material "
          "named '" +
          m + "'");
    }
    return getSubGroupIdentifiers(r, "MFront/" + m + "/Models");
  }  // end of DataBase::getAvailableModels

  std::vector<std::string> DataBase::getAvailableMTestTests(
      const std::string& m, const std::string& b) const {
    if (!checkMFrontGroup(this->file)) {
      return {};
    }
    const auto r = this->file.getRoot();
    if (!subGroupExists(r, "MFront/" + m)) {
      raise(
          "DataBase::getAvailableMTestTests: no material "
          "named '" +
          m + "'");
    }
    if (!subGroupExists(r, "MFront/" + m + "/Behaviours")) {
      raise(
          "DataBase::getAvailableMTestTests: "
          "no behaviours associated with "
          "material '" +
          m + "'");
    }
    if (!subGroupExists(r, "MFront/" + m + "/Behaviours/" + b)) {
      raise(
          "DataBase::getAvailableMTestTests: "
          "no behaviour named '" +
          b +
          "' associated with "
          "material '" +
          m + "'");
    }
    const auto path = "MFront/" + m + "/Behaviours/" + b + "/MTestTests";
    return getSubGroupIdentifiers(r, path);
  }  // end of DataBase::getAvailableMTestTests

  std::vector<std::string> DataBase::getAvailableMTestTests(
      const std::string& b) const {
    if (!checkMFrontGroup(this->file)) {
      return {};
    }
    const auto r = this->file.getRoot();
    if (!subGroupExists(r, "MFront/Behaviours")) {
      raise(
          "DataBase::getAvailableMTestTests: "
          "no generic behaviours declared");
    }
    if (!subGroupExists(r, "MFront/Behaviours/" + b)) {
      raise(
          "DataBase::getAvailableMTestTests: "
          "no generic behaviour named '" +
          b + "'");
    }
    const auto path = "MFront/Behaviours/" + b + "/MTestTests";
    return getSubGroupIdentifiers(r, path);
  }  // end of DataBase::getAvailableMTestTests

  DataBase::~DataBase() = default;

}  // end of namespace madnex
