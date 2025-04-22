/*!
 * \file   src/DataBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21/08/2020
 */

#include <regex>
#include <iterator>
#include <algorithm>
#include "Madnex/Raise.hxx"
#include "Madnex/DataBase.hxx"

namespace madnex {

  static bool checkMFrontGroup(const File& f) {
    try {
      const auto r = f.getRoot();
      if (!exists(r, "MFront")) {
        return false;
      }
      if (!subGroupExists(r, "MFront")) {
        raise("`MFront` is not a sub group of the file");
      }
      return true;
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of checkMFrontGroup

  static std::vector<std::string> getSubGroupIdentifiers(const Group& gr,
                                                         const std::string& p) {
    try {
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
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getSubGroupIdentifiers

  DataBase::DataBase(const std::string& f)
      : file(f, H5F_ACC_RDONLY) {}  // end of DataBase::DataBase

  DataBase::DataBase(DataBase&&) = default;

  std::vector<std::string> DataBase::getMaterialsList() const {
    try {
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
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getMaterialsList

  std::map<std::string, std::vector<std::string>>
  DataBase::getAvailableMaterialProperties() const {
    try {
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
        insert_if(m, getSubGroupIdentifiers(
                         r, "MFront/" + m + "/MaterialProperties"));
      }
      return mmps;
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableMaterialProperties

  std::vector<std::string> DataBase::getAvailableMaterialProperties(
      const std::string& m) const {
    try {
      checkIfMaterialExists(m);
      const auto r = this->file.getRoot();
      return getSubGroupIdentifiers(r, "MFront/" + m + "/MaterialProperties");
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableMaterialProperties

  std::map<std::string, std::vector<std::string>>
  DataBase::getAvailableBehaviours() const {
    try {
      auto mbs = std::map<std::string, std::vector<std::string>>{};
      auto insert_if = [&mbs](const std::string& m,
                              std::vector<std::string> bs) {
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
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableBehaviours

  std::vector<std::string> DataBase::getAvailableBehaviours(
      const std::string& m) const {
    try {
      checkIfMaterialExists(m);
      const auto r = this->file.getRoot();
      return getSubGroupIdentifiers(r, "MFront/" + m + "/Behaviours");
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableBehaviours

  std::map<std::string, std::vector<std::string>> DataBase::getAvailableModels()
      const {
    try {
      auto mms = std::map<std::string, std::vector<std::string>>{};
      auto insert_if = [&mms](const std::string& m,
                              std::vector<std::string> ms) {
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
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableModels

  std::vector<std::string> DataBase::getAvailableModels(
      const std::string& m) const {
    try {
      checkIfMaterialExists(m);
      const auto r = this->file.getRoot();
      return getSubGroupIdentifiers(r, "MFront/" + m + "/Models");
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableModels

  std::vector<std::string> DataBase::getAvailableMFMTestGeneratorTests(
      const std::string& m, const std::string& b) const {
    try {
      if (!checkMFrontGroup(this->file)) {
        return {};
      }
      const auto r = this->file.getRoot();
      if (!subGroupExists(r, "MFront/" + m)) {
        raise(
            "DataBase::getAvailableMFMTestGeneratorTests: no material "
            "named '" +
            m + "'");
      }
      if (!subGroupExists(r, "MFront/" + m + "/Behaviours")) {
        raise(
            "DataBase::getAvailableMFMTestGeneratorTests: "
            "no behaviours associated with "
            "material '" +
            m + "'");
      }
      if (!subGroupExists(r, "MFront/" + m + "/Behaviours/" + b)) {
        raise(
            "DataBase::getAvailableMFMTestGeneratorTests: "
            "no behaviour named '" +
            b +
            "' associated with "
            "material '" +
            m + "'");
      }
      const auto path =
          "MFront/" + m + "/Behaviours/" + b + "/MFMTestGeneratorTests";
      return getSubGroupIdentifiers(r, path);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableMFMTestGeneratorTests

  std::vector<std::string> DataBase::getAvailableMFMTestGeneratorTests(
      const std::string& b) const {
    try {
      if (!checkMFrontGroup(this->file)) {
        return {};
      }
      const auto r = this->file.getRoot();
      if (!subGroupExists(r, "MFront/Behaviours")) {
        raise(
            "DataBase::getAvailableMFMTestGeneratorTests: "
            "no generic behaviours declared");
      }
      if (!subGroupExists(r, "MFront/Behaviours/" + b)) {
        raise(
            "DataBase::getAvailableMFMTestGeneratorTests: "
            "no generic behaviour named '" +
            b + "'");
      }
      const auto path = "MFront/Behaviours/" + b + "/MFMTestGeneratorTests";
      return getSubGroupIdentifiers(r, path);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableMFMTestGeneratorTests

  std::vector<std::string> DataBase::getAvailableMTestTests(
      const std::string& m, const std::string& b) const {
    try {
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
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableMTestTests

  std::vector<std::string> DataBase::getAvailableMTestTests(
      const std::string& b) const {
    const auto r = this->file.getRoot();
    try {
      if (!checkMFrontGroup(this->file)) {
        return {};
      }
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
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of DataBase::getAvailableMTestTests

  void DataBase::checkIfMaterialExists(const std::string& m) const {
    if (!checkMFrontGroup(this->file)) {
      raise(
          "DataBase::getAvailableBehaviours: no material "
          "named '" +
          m + "' (no material defined)");
    }
    const auto r = this->file.getRoot();
    if (subGroupExists(r, "MFront/" + m)) {
      return;
    }
    auto msg =
        "DataBase::getAvailableBehaviours: no material "
        "named '" +
        m + "'";
    const auto materials = this->getMaterialsList();
    if (materials.empty()) {
      msg += " (no material defined)";
    } else {
      msg += ". The following materials are defined:";
      for (const auto& mn : materials) {
        msg += "\n- " + mn;
      }
    }
    raise(msg);
  }  // end of checkIfMaterialExists

  DataBase::~DataBase() = default;

  namespace internals {

    static std::vector<std::string> filter(
        const std::vector<std::string>& names, const std::string& p) {
      auto res = std::vector<std::string>{};
      std::regex r(p);
      std::copy_if(
          names.begin(), names.end(), std::back_inserter(res),
          [&r](const std::string& n) { return std::regex_match(n, r); });
      return res;
    };  // end of filter

    template <std::vector<std::string> (madnex::DataBase::*get)(
        const std::string&) const>
    static std::map<std::string, std::vector<std::string>>
    getMatchingMaterialKnoweldge(const madnex::DataBase& d,
                                 const std::string& m,
                                 const std::string& n) {
      try {
        auto r = std::map<std::string, std::vector<std::string>>{};
        const auto materials = madnex::getMatchingMaterials(d, m);
        for (const auto& material : materials) {
          auto mks = filter((d.*get)(material), n);
          if (!mks.empty()) {
            r.insert({material, mks});
          }
        }
        return r;
      } catch (H5::Exception& e) {
        raise(e.getDetailMsg());
      }
    }  // end of getMatchingMdaterialKnoweldge

  }  // end of namespace internals

  std::vector<std::string> getMatchingMaterials(const DataBase& d,
                                                const std::string& m) {
    try {
      auto materials = d.getMaterialsList();
      materials.push_back("");
      return internals::filter(materials, m);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMatchingMaterials

  std::map<std::string, std::vector<std::string>> getMatchingMaterialProperties(
      const DataBase& d, const std::string& m, const std::string& mp) {
    try {
      using ptr =
          std::vector<std::string> (DataBase::*)(const std::string&) const;
      return internals::getMatchingMaterialKnoweldge<static_cast<ptr>(
          &DataBase::getAvailableMaterialProperties)>(d, m, mp);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMatchingMaterialProperties

  std::map<std::string, std::vector<std::string>> getMatchingBehaviours(
      const DataBase& d, const std::string& m, const std::string& b) {
    try {
      using ptr =
          std::vector<std::string> (DataBase::*)(const std::string&) const;
      return internals::getMatchingMaterialKnoweldge<static_cast<ptr>(
          &DataBase::getAvailableBehaviours)>(d, m, b);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMatchingBehaviours

  std::map<std::string, std::vector<std::string>> getMatchingModels(
      const DataBase& d, const std::string& m, const std::string& model) {
    try {
      using ptr =
          std::vector<std::string> (DataBase::*)(const std::string&) const;
      return internals::getMatchingMaterialKnoweldge<static_cast<ptr>(
          &DataBase::getAvailableModels)>(d, m, model);
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of getMatchingModels

}  // namespace madnex
