/*!
 * \file   Types.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   20 janv. 2017
 */

#include"Madnex/Types.hxx"

namespace madnex
{

  HDF5TypeDescriptionImplementation<char>::type
  HDF5TypeDescriptionImplementation<char>::getNativeType()
  {
    return PredType::NATIVE_CHAR;
  }

  HDF5TypeDescriptionImplementation<std::int32_t>::type
  HDF5TypeDescriptionImplementation<std::int32_t>::getNativeType()
  {
    return PredType::NATIVE_INT32;
  }

  HDF5TypeDescriptionImplementation<std::int64_t>::type
  HDF5TypeDescriptionImplementation<std::int64_t>::getNativeType()
  {
    return PredType::NATIVE_INT64;
  }
  
  HDF5TypeDescriptionImplementation<float>::type
  HDF5TypeDescriptionImplementation<float>::getNativeType()
  {
    return PredType::NATIVE_FLOAT;
  }
  
  HDF5TypeDescriptionImplementation<double>::type
  HDF5TypeDescriptionImplementation<double>::getNativeType()
  {
    return PredType::NATIVE_DOUBLE;
  }

  HDF5TypeDescriptionImplementation<long double>::type
  HDF5TypeDescriptionImplementation<long double>::getNativeType()
  {
    return PredType::NATIVE_LDOUBLE;
  }

  HDF5TypeDescriptionImplementation<std::string>::type
  HDF5TypeDescriptionImplementation<std::string>::getNativeType()
  {
    return {H5::PredType::C_S1,H5T_VARIABLE};
  }
  
} // end of namespace madnex
