/*!
 * \file   File.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 janv. 2017
 */

#include"Madnex/Raise.hxx"
#include"Madnex/File.hxx"

namespace madnex{

  File::File(File&&) = default;
  File::File(const File&) = default;
  File& File::operator=(File&&) = default;
  File& File::operator=(const File&) = default;
  
  Group File::getRoot(){
    try {
      return this->openGroup("/");
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

  Group File::getRoot() const{
    try {
      return this->openGroup("/");
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }

  bool File::checkIfGroupIsWritable(const std::string& p) const {
    try {
      // check that the file is open in read/write mode.
#if ((H5_VERS_MAJOR == 1) && (H5_VERS_MINOR <= 10))
      // up to HDF5 1.10, `H5Fget_intent` takes an `unsigned`
      auto intent = unsigned{};
#else   // HDF5 version
      auto intent = H5F_intent_t{};
#endif  // HDF5 version
      if (H5Fget_intent(this->getId(), &intent) < 0) {
        raise("madnex::File::checkIfGroupIsWritable: can't get the intent "
              "of the file associated with group '" +
              p + "'");
      }
      if (intent != H5F_ACC_RDWR) {
        return false;
      }
      // if an object is already stored at the given path, it must be a
      // group
      if (H5Lexists(this->getId(), p.c_str(), H5P_DEFAULT) > 0) {
        try {
          this->openGroup(p);
        } catch (H5::Exception&) {
          return false;
        }
      }
      return true;
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of File::checkIfGroupIsWritable

  bool exists(const File& f, const std::string& p) {
    return H5Lexists(f.getId(), p.c_str(), H5P_DEFAULT) > 0;
  }

  File::~File() = default;
  
} // end of namespace madnex
