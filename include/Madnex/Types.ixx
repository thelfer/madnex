/*!
 * \file   Types.ixx
 * \brief    
 * \author Helfer Thomas 
 * \date   31 janv. 2017
 */

#ifndef LIB_MADNEX_TYPES_IXX
#define LIB_MADNEX_TYPES_IXX

namespace madnex{

  //! partial specialisation for fcstring
  template<unsigned short N>
  typename HDF5TypeDescriptionImplementation<fcstring<N>>::type
  HDF5TypeDescriptionImplementation<fcstring<N>>::getNativeType()
  {
    return StrType(0,N);
  } // end of getNativeType

  
} // end of namespace madnex

#endif /* LIB_MADNEX_TYPES_IXX */
