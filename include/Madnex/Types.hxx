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
#include"Madnex/Config.hxx"
#include"Madnex/FCString.hxx"

/*!
 * \brief macro used to define the partial specialisation of the
 * `HDF5TypeDescription` metafunction for plain old data types.
 * \param X: type
 */
#define MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(X)                     \
  /*!						                           \
   * \param[out] g: group			                           \
   * \param[in]  n: name of the data		                           \
   * \param[in]  o: object to be written	                           \
   */						                           \
  template<>					                           \
  struct MADNEX_EXPORT HDF5TypeDescriptionImplementation< X >{  \
    using type = PredType;			                           \
    /*!                                                                    \
     * \return the native type associated with X                           \
     */                                                                    \
    static type getNativeType(void);                                       \
  }

namespace madnex{

  /*!
   * \brief template class which aims at:
   * - defining the HDF5 type associated with a variable
   * - providing a static `getNativeType` method properly defined.
   * \note partial specialisation are provided for every type
   * supported by the madnex library.
   */
  template<typename T>
  struct HDF5TypeDescriptionImplementation;
  //! Partial specialiation of `HDF5TypeDescriptionImplementation` for `std::string`
  template<>
  struct MADNEX_EXPORT HDF5TypeDescriptionImplementation<std::string>
  {
    //! result
    using type = StrType;
    //! return the native type associated with `std::string`
    static type getNativeType(void);
  };
  /*!
   * \brief Partial specialiation of
   * `HDF5TypeDescriptionImplementation` for `fcstring<N>`
   * \tparam N: fixed string capacity
   */
  template<unsigned short N>
  struct HDF5TypeDescriptionImplementation<fcstring<N>>
  {
    //! result
    using type = StrType;
    //! return the native type associated with `fcstring`
    static type getNativeType(void);
  };
  
  //! a simple alias
  template<typename T>
  using HDF5TypeDescription = typename HDF5TypeDescriptionImplementation<T>::type;
  
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(char);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(std::int32_t);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(std::int64_t);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(float);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(double);
  MADNEX_HDF5_HDF5TYPEDESCRIPTION_DECLARATION(long double);
  
  /*!
   * \return the predefined types associated with the given type
   * \tparam T: type
   */
  template<typename T>
  HDF5TypeDescription<T> getNativeType(void){
    return HDF5TypeDescriptionImplementation<T>::getNativeType();
  }
  
} // end of namespace madnex

#include"Madnex/Types.ixx"

#endif /* LIB_MADNEX_TYPES_HXX */
