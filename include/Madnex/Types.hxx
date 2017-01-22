/*!
 * \file   Types.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   20 janv. 2017
 */

#ifndef LIB_MADNEX_TYPES_HXX
#define LIB_MADNEX_TYPES_HXX

#include<array>
#include<cstdint>
#include"Madnex/HDF5.hxx"

/*!
 * \brief macro used to define the partial specialisation of the
 * `HDF5TypeDescription` metafunction for plain old data types.
 * \param X: type
 */
#define MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(X) \
  /*!						       \
   * \param[out] g: group			       \
   * \param[in]  n: name of the data		       \
   * \param[in]  o: object to be written	       \
   */						       \
  template<>					       \
  struct HDF5TypeDescriptionMetaFunction< X >{	       \
    using type = PredType;			       \
  }

namespace madnex{

  template<typename T>
  struct HDF5TypeDescriptionMetaFunction
  {
    //! result
    using type = StrType;
  };

  //! a simple alias
  template<typename T>
  using HDF5TypeDescription = typename HDF5TypeDescriptionMetaFunction<T>::type;
  
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(bool);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(char);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(short);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(int);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(long);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(unsigned short);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(unsigned int);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(unsigned long);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(float);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(double);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(long double);
  
  /*!
   * \return the predefined types associated with the given type
   * \tparam T: type
   */
  template<typename T>
  HDF5TypeDescription<T> getNativeType(void);
  //! partial specialisation for char
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<char>
  getNativeType<char>(void);
  //! partial specialisation for unsigned char
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<unsigned char>
  getNativeType<unsigned char>(void);
  //! partial specialisation for short
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<short>
  getNativeType<short>(void);
  //! partial specialisation for unsigned shot
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<unsigned short>
  getNativeType<unsigned short>(void);
  //! partial specialisation for int
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<int>
  getNativeType<int>(void);
  //! partial specialisation for unsigned int
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<unsigned int>
  getNativeType<unsigned int>(void);
  //! partial specialisation for long
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<long>
  getNativeType<long>(void);
  //! partial specialisation for unsigned long
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<unsigned long>
  getNativeType<unsigned long>(void);
  //! partial specialisation for float
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<float>
  getNativeType<float>(void);
  //! partial specialisation for double
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<double>
  getNativeType<double>(void);
  //! partial specialisation for long double
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<long double>
  getNativeType<long double>(void);
  //! partial specialisation for std::string
  template<>
  MADNEX_VISIBILITY_EXPORT HDF5TypeDescription<std::string>
  getNativeType<std::string>(void);
  
} // end of namespace madnex

#endif /* LIB_MADNEX_TYPES_HXX */
