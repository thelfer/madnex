/*!
 * \file   Types.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   20 janv. 2017
 */

#include"Madnex/Types.hxx"

namespace madnex
{

  template<>
  HDF5TypeDescription<char> getNativeType<char>()
  {
    return PredType::NATIVE_CHAR;
  }

  template<>
  HDF5TypeDescription<unsigned char> getNativeType<unsigned char>()
  {
    return PredType::NATIVE_UCHAR;
  }

  template<>
  HDF5TypeDescription<short> getNativeType<short>()
  {
    return PredType::NATIVE_SHORT;
  }

  template<>
  HDF5TypeDescription<unsigned short> getNativeType<unsigned short>()
  {
    return PredType::NATIVE_USHORT;
  }

  template<>
  HDF5TypeDescription<int> getNativeType<int>()
  {
    return PredType::NATIVE_INT;
  }

  template<>
  HDF5TypeDescription<unsigned int> getNativeType<unsigned int>()
  {
    return PredType::NATIVE_UINT;
  }

  template<>
  HDF5TypeDescription<long> getNativeType<long>()
  {
    return PredType::NATIVE_LONG;
  }

  template<>
  HDF5TypeDescription<unsigned long> getNativeType<unsigned long>()
  {
    return PredType::NATIVE_ULONG;
  }

  template<>
  HDF5TypeDescription<float> getNativeType<float>()
  {
    return PredType::NATIVE_FLOAT;
  }
  
  template<>
  HDF5TypeDescription<double> getNativeType<double>()
  {
    return PredType::NATIVE_DOUBLE;
  }

  template<>
  HDF5TypeDescription<long double> getNativeType<long double>()
  {
    return PredType::NATIVE_LDOUBLE;
  }

  template<>
  HDF5TypeDescription<std::string> getNativeType<std::string>()
  {
    return {H5::PredType::C_S1,H5T_VARIABLE};
  }
  
} // end of namespace madnex
