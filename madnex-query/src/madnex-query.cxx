/*!
 * \file   madnex-query/src/madnex-query.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21/08/2020
 */

#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <functional>
#include "Madnex/Raise.hxx"
#include "Madnex/MFrontDataBase.hxx"

static bool startsWith(const std::string& s1, const std::string& s2) {
  return ((s1.size() >= s2.size()) &&
          (std::equal(s2.begin(), s2.end(), s1.begin())));
}  // end of starts_with

/*!
 * \brief structure describing a call-back used to treat a command line argument
 */
struct CallBack {
  //! \brief a simple alias
  using Function = std::function<void(const std::string&)>;
  //! \brief default constructor
  CallBack() = default;
  /*!
   * \brief constructor
   * \param[in] d: description of the call-back
   * \param[in] f: function called when the call-back is triggered
   */
  CallBack(std::string d, Function f)
      : description(std::move(d)), c(std::move(f)) {}  // end of constructor
  //! \brief copy constructor
  CallBack(const CallBack&) = default;
  //! \brief move constructor
  CallBack(CallBack&&) = default;
  //! \brief standard assignement
  CallBack& operator=(const CallBack&) = default;
  //! \brief move assignement
  CallBack& operator=(CallBack&&) = default;
  /*!
   * \brief return the function triggered by this call-back
   */
  const Function& getFunction() const { return this->c; }
  /*!
   * \brief return the description of the call-back
   */
  const std::string& getDescription() const { return this->description; }
  //! \brief destructor
  ~CallBack() = default;

 private:
  //! \brief description of the call-back
  std::string description;
  //! \brief function to be executed
  Function c;
};  // end of struct CallBack

// selected material
std::string material;

static std::map<std::string, CallBack> getCallBacks() {
  using Function = CallBack::Function;
  std::map<std::string, CallBack> call_backs;
  auto add = [&call_backs](const std::string& n, const char* const d,
                           Function f) {
#if (defined _WIN32) || (defined _WIN64)
    call_backs.insert({"/" + n, CallBack(d, f)});
#endif /* (defined _WIN32)  || (defined _WIN64) */
    call_backs.insert({"--" + n, CallBack(d, f)});
  };
  add("list-materials", "list all materials in the MFront subgroup",
      [](const std::string& f) {
        auto d = madnex::MFrontDataBase{f};
        for (const auto& m : d.getMaterialsList()) {
          std::cout << "- " << m << '\n';
        }
      });
  add("list-material-properties",
      "list all available MFront material properties",
      [](const std::string& f) {
        auto d = madnex::MFrontDataBase{f};
        if (!material.empty()) {
          const auto mps = d.getAvailableMaterialProperties(
              material == "<none>" ? "" : material);
          if (!mps.empty()) {
            if (material == "<none>") {
              std::cout << "- material properties associated with material '" +
                               material + "':\n";
            } else {
              std::cout
                  << "- material properties associated with no material:\n";
            }
            for (const auto& mp : mps) {
              std::cout << "    - " << mp << '\n';
            }
          }
        } else {
          for (const auto& mmps : d.getAvailableMaterialProperties()) {
            if (mmps.first.empty()) {
              std::cout
                  << "- material properties associated with no material:\n";
            } else {
              std::cout << "- material properties associated with material '" +
                               mmps.first + "':\n";
            }
            for (const auto& mp : mmps.second) {
              std::cout << "    - " << mp << '\n';
            }
          }
        }
      });
  add("list-behaviours", "list all available MFront behaviours",
      [](const std::string& f) {
        auto d = madnex::MFrontDataBase{f};
        if (!material.empty()) {
          const auto bs =
              d.getAvailableBehaviours(material == "<none>" ? "" : material);
          if (!bs.empty()) {
            if (material == "<none>") {
              std::cout << "- behaviours associated with no material:\n";
            } else {
              std::cout << "- behaviours associated with material '" +
                               material + "':\n";
            }
            for (const auto& b : bs) {
              std::cout << "    - " << b << '\n';
            }
          }
        } else {
          for (const auto& mbs : d.getAvailableBehaviours()) {
            if (mbs.first.empty()) {
              std::cout << "- behaviours associated with no material:\n";
            } else {
              std::cout << "- behaviours associated with material '" +
                               mbs.first + "':\n";
            }
            for (const auto& b : mbs.second) {
              std::cout << "    - " << b << '\n';
            }
          }
        }
      });
  add("list-models", "list all available MFront models",
      [](const std::string& f) {
        auto d = madnex::MFrontDataBase{f};
        if (!material.empty()) {
          const auto ms =
              d.getAvailableModels(material == "<none>" ? "" : material);
          if (!ms.empty()) {
            if (material == "<none>") {
              std::cout << "- models associated with no material:\n";
            } else {
              std::cout << "- models associated with material '" + material +
                               ":'\n";
            }
            for (const auto& m : ms) {
              std::cout << "    - " << m << '\n';
            }
          }
        } else {
          for (const auto& mms : d.getAvailableModels()) {
            if (mms.first.empty()) {
              std::cout << "- models associated with no material:\n";
            } else {
              std::cout << "- models associated with material '" + mms.first +
                               "':\n";
            }
            for (const auto& m : mms.second) {
              std::cout << "    - " << m << '\n';
            }
          }
        }
      });
  return call_backs;
}  // end of getCallBacks

int main(const int argc, const char* const* const argv) {
  auto input_file = std::string{};
  const auto available_call_backs = getCallBacks();
  auto call_backs = std::vector<CallBack::Function>();
  for (auto pa = argv + 1; pa != argv + argc; ++pa) {
    const auto a = std::string{*pa};
    if (a == "--help") {
      std::cout << "usage: madnex-query [options] file.madnex\n";
      std::cout << "The valid options are :\n";
      for (const auto& c : available_call_backs) {
        std::cout << c.first << " : " << c.second.getDescription() << '\n';
      }
      std::cout << "--material : specify the material name. <none> may be used "
                   "to select material properties, behaviours or models not "
                   "associated with a material\n";
#if (defined _WIN32) || (defined _WIN64)
      std::cout << "/material : specify the material name (see above)\n";
#endif
      std::exit(EXIT_SUCCESS);
#if (defined _WIN32) || (defined _WIN64)
    } else if (startsWith(a, "/material=")) {
      madnex::raise_if(!material.empty(), "multiple material name specified");
      material = a.substr(std::strlen("/material="));
      madnex::raise_if(material.empty(), "empty material name");
#endif
    } else if (startsWith(a, "--material=")) {
      madnex::raise_if(!material.empty(), "multiple material name specified");
      material = a.substr(std::strlen("--material="));
      madnex::raise_if(material.empty(), "empty material name");
    } else {
      const auto pc = available_call_backs.find(a);
      if (pc == available_call_backs.end()) {
        if (a[0] == '-') {
          madnex::raise("unsupported argument '" + a + "'");
        }
        if (!input_file.empty()) {
          madnex::raise("muliple input files specified ('" + input_file +
                        "' and '" + a + "'");
        }
        input_file = a;
      } else {
        call_backs.push_back(pc->second.getFunction());
      }
    }
  }
  if (input_file.empty()) {
    std::cerr << "madnex-query: no input file given\n";
    return EXIT_FAILURE;
  }
  for (const auto& c : call_backs) {
    c(input_file);
  }
  return EXIT_SUCCESS;
}