/*!
 * \file   File.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 janv. 2017
 */

#include"Madnex/File.hxx"

namespace madnex{

  Group File::getRoot(){
    return this->openGroup("/");
  }

  Group File::getRoot() const{
    return this->openGroup("/");
  }

  bool File::checkIfGroupIsWritable(const std::string& g) const{
    //    const auto g = this->getRoot();
    return true;
  }

  bool exists(const File& f, const std::string& p) {
    return H5Lexists(f.getId(), p.c_str(), H5P_DEFAULT) > 0;
  }

} // end of namespace madnex
