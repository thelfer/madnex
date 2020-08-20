/*!
 * \file   include/Madnex/Raise.ixx
 * \brief
 * \author Thomas Helfer
 * \date   20/08/2020
 */

#ifndef LIB_MADNEX_RAISE_IXX
#define LIB_MADNEX_RAISE_IXX

namespace madnex {

  template <typename ExceptionType>
  MADNEX_NORETURN void raise() {
    ExceptionType e;
    throw(std::move(e));
  }  // end of raise

  template <typename ExceptionType, typename... Args>
  MADNEX_NORETURN void raise(Args&&... a) {
    ExceptionType e(std::forward<Args...>(a...));
    throw(std::move(e));
  }  // end of raise

  template <typename ExceptionType>
  void raise_if(const bool c) {
    if (c) {
      raise<ExceptionType>();
    }
  }  // end of raise

  template <typename ExceptionType, typename... Args>
  void raise_if(const bool c, Args&&... a) {
    if (c) {
      raise<ExceptionType>(std::forward<Args...>(a...));
    }
  }  // end of raise

}  // end of namespace madnex

#endif /* LIB_MADNEX_RAISE_IXX */
