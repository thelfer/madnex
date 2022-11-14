/*!
 * \file   include/Madnex/TypeName.ixx
 * \brief
 * \author THOMAS HELFER
 * \date   16/01/2017
 */

#ifndef LIB_MADNEX_TYPENAME_IXX
#define LIB_MADNEX_TYPENAME_IXX

#include <string>
#include <cstdint>
#include <vector>

/*!
 * \brief macro used to specialize the `TypeName`
 * metafunction to plain old data types.
 * \param X: type
 * \param Y: name
 */
#define MADNEX_TYPENAME_STDTYPE(X, Y)      \
  /*!                                      \
   * \brief partial specialisation of      \
   * the typename metafunction for X       \
   */                                      \
  template <>                              \
  struct TypeName<X> {                     \
    /*!                                    \
     * \brief name associated with X       \
     */                                    \
    static constexpr const char* name = Y; \
  }

namespace madnex {

  /*
   * POD type begins with @
   * type declare in the std namespace begins with !
   */

  MADNEX_TYPENAME_STDTYPE(char, "@c");
  MADNEX_TYPENAME_STDTYPE(std::int32_t, "@i32");
  MADNEX_TYPENAME_STDTYPE(std::int64_t, "@i32");
  MADNEX_TYPENAME_STDTYPE(float, "@f");
  MADNEX_TYPENAME_STDTYPE(double, "@d");
  MADNEX_TYPENAME_STDTYPE(long double, "@ld");
  MADNEX_TYPENAME_STDTYPE(std::string, "!s");

  MADNEX_TYPENAME_STDTYPE(std::vector<char>, "@vector@c");
  MADNEX_TYPENAME_STDTYPE(std::vector<std::int32_t>, "@vector@i32");
  MADNEX_TYPENAME_STDTYPE(std::vector<std::int64_t>, "@vector@i32");
  MADNEX_TYPENAME_STDTYPE(std::vector<float>, "@vector@f");
  MADNEX_TYPENAME_STDTYPE(std::vector<double>, "@vector@d");
  MADNEX_TYPENAME_STDTYPE(std::vector<long double>, "@vector@ld");
  MADNEX_TYPENAME_STDTYPE(std::vector<std::string>, "@vector!s");

}  // end of namespace madnex

#endif /* LIB_MADNEX_TYPENAME_IXX */
