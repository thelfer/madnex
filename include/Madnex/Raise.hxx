/*!
 * \file   include/Madnex/Raise.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/08/2020
 */

#ifndef LIB_MADNEX_RAISE_HXX
#define LIB_MADNEX_RAISE_HXX

#include <utility>
#include <stdexcept>
#include "Madnex/Config.hxx"

namespace madnex {

  /*!
   * \brief a small wrapper used to build the exception outside the
   * `throw` statement. As most exception's classes constructors may
   * throw, this avoids undefined behaviour as reported by the
   * `cert-err60-cpp` warning of `clang-tidy` (thrown exception type
   * is not nothrow copy constructible).
   * \tparam ExceptionType: type of the exception to be thrown.
   */
  template <typename ExceptionType = std::runtime_error>
  MADNEX_NORETURN MADNEX_VISIBILITY_LOCAL MADNEX_INLINE void raise();

  /*!
   * \brief a small wrapper used to build the exception outside the
   * `throw` statement. As most exception's classes constructors may
   * throw, this avoids undefined behaviour as reported by the
   * `cert-err60-cpp` warning of `clang-tidy` (thrown exception type
   * is not nothrow copy constructible).
   * \tparam ExceptionType: type of the exception to be thrown.
   * \tparam Args: type of the arguments passed to the exception'
   * constructor.
   * \param[in] a: arguments passed to the exception' constructor.
   */
  template <typename ExceptionType = std::runtime_error, typename... Args>
  MADNEX_NORETURN MADNEX_VISIBILITY_LOCAL MADNEX_INLINE void raise(Args&&...);

  /*!
   * \brief raise an exception if the first argument is `true`.
   * \tparam ExceptionType: type of the exception to be thrown.
   * \param[in] b: condition to be checked. If `true`, an exception is
   * thrown.
   */
  template <typename ExceptionType = std::runtime_error>
  MADNEX_VISIBILITY_LOCAL MADNEX_INLINE void raise_if(const bool);

  /*!
   * \brief raise an exception if the first argument is `true`.
   * \tparam ExceptionType: type of the exception to be thrown.
   * \tparam Args: type of the arguments passed to the exception'
   * constructor.
   * \param[in] b: condition to be checked. If `true`, an exception is
   * thrown.
   * \param[in] a: arguments passed to the exception' constructor.
   */
  template <typename ExceptionType = std::runtime_error, typename... Args>
  MADNEX_VISIBILITY_LOCAL MADNEX_INLINE void raise_if(const bool, Args&&...);

}  // end of namespace madnex

#include "Madnex/Raise.ixx"

#endif /* LIB_MADNEX_RAISE_HXX */
