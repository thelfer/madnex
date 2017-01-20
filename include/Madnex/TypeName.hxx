/*!
 * \file   include/Madnex/TypeName.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16/01/2017
 */

#ifndef LIB_MADNEX_TYPENAME_HXX
#define LIB_MADNEX_TYPENAME_HXX

namespace madnex{

  /*!
   * \brief a metafunction returning a name associated with the given
   * type
   * \tparam T: type
   * \note This metafunction must be specialized for each type that
   * will be stored in a madnex file
   */
  template<typename T>
  struct TypeName;

}

#include"Madnex/TypeName.ixx"

#endif /* LIB_MADNEX_TYPENAMES_HXX */
