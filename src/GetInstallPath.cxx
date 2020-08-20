/*!
 * \file   GetInstallPath.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/05/2018
 */

#include <stdexcept>
#include <algorithm>
#include <cstdlib>

#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "Madnex/Raise.hxx"
#include "Madnex/GetInstallPath.hxx"
#include "Madnex/GetInstallPath-defines.hxx"

namespace madnex {

#if defined _WIN32 || defined _WIN64
  static bool getValueInRegistry(std::string& value) {
    HKEY hKey;
    char szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    LONG nError;
    LONG lRes = RegOpenKeyEx(HKEY_CLASSES_ROOT, "MADNEXHOME-" VERSION, 0,
                             KEY_READ, &hKey);
    if (ERROR_SUCCESS != lRes) {
      return false;
    }
    nError = RegQueryValueEx(hKey, "", nullptr, nullptr,
                             reinterpret_cast<LPBYTE>(szBuffer), &dwBufferSize);
    RegCloseKey(hKey);
    if (ERROR_SUCCESS == nError) {
      value = szBuffer;
      return true;
    }
    return false;
  }
#endif

  static std::string handleSpace(const std::string& p) {
    if (std::find(p.begin(), p.end(), ' ') != p.end()) {
#if defined _WIN32 || defined _WIN64
      madnex::raise(
          "maxdnex:::handleSpace: "
          "path to MADNEXshall not contain space as "
          "MinGW can't handle it (Found '" +
          p +
          "'). "
          "Please change MADNEX installation directory");
#else
      return '"' + p + '"';
#endif
    }
    return p;
  }

  std::string getInstallPath() {
#if defined _WIN32 || defined _WIN64
    // check in the registry (installation through NSIS)
    std::string rpath;
    if (getValueInRegistry(rpath)) {
      auto path = handleSpace(rpath);
      if (!path.empty()) {
        return path;
      }
    }
#endif
    const auto path = ::getenv("MADNEXHOME");
    if (path != nullptr) {
      return handleSpace(path);
    }
#if defined _WIN32 || defined _WIN64
    return "";
#else
    return PREFIXDIR;
#endif
  }  // end of getInstallPath

}  // end of namespace madnex
