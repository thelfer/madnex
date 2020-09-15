/*!
 * \file   File.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 janv. 2017
 */

#include"Madnex/File.hxx"

namespace madnex{

  File::File(File&&) = default;
  File::File(const File&) = default;
  File& File::operator=(File&&) = default;
  File& File::operator=(const File&) = default;
  
  Group File::getRoot(){
    return this->openGroup("/");
  }

  Group File::getRoot() const{
    return this->openGroup("/");
  }

  bool File::checkIfGroupIsWritable(const std::string&) const{
    //    const auto g = this->getRoot();
    return true;
  }

  bool exists(const File& f, const std::string& p) {
    return H5Lexists(f.getId(), p.c_str(), H5P_DEFAULT) > 0;
  }

  File::~File() = default;
  
} // end of namespace madnex
