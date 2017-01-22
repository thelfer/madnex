/*!
 * \file   CompoundExtractor.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   22 janv. 2017
 */

#include<stdexcept>
#include"Madnex/CompoundExtractor.hxx"

namespace madnex{

  CompoundExtractor::CompoundExtractor(const DataSet& d)
  {
    const auto dtype = d.getDataType();
    if(dtype.getClass()!=H5T_COMPOUND){
      throw(std::runtime_error("CompoundExtractor: invalid data set"));
    }
    this->ctype = H5::CompType(d);
    this->data.resize(ctype.getSize());
    d.read(this->data.data(),ctype);
  }

  CompoundExtractor::~CompoundExtractor() = default;
  
} // end of namespace madnex
