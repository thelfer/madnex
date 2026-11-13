/*!
 * \file   Exceptions.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#ifndef LIB_MADNEX_EXCEPTIONS_HXX
#define LIB_MADNEX_EXCEPTIONS_HXX

#include <string>
#include <stdexcept>
#include "Madnex/Config.hxx"

namespace madnex {

  /*!
   * \brief base class for all exception throw by the `Madnex` library
   *
   * \note this class derives from `std::runtime_error` so that the
   * exceptions thrown by the library can still be caught by the code
   * written against the previous versions of the library, which used to
   * throw `std::runtime_error` objects.
   */
  struct MADNEX_EXPORT Exception : public std::runtime_error {
    //! default constructor
    Exception();
    //! copy constructor
    Exception(const Exception&);
    //! move constructor
    Exception(Exception&&);
    //! destructor
    virtual ~Exception();
  };  // end of struct Exception

  /*!
   * \brief a generic exception
   */
  struct MADNEX_EXPORT RuntimeError : public Exception {
    //! default constructor
    RuntimeError();
    /*!
     * \brief constructor from std::string
     * \param[in] m: error message
     */
    RuntimeError(const std::string&);
    /*!
     * \brief constructor from C-string
     * \param[in] m: error message
     */
    RuntimeError(const char*);
    //! copy constructor
    RuntimeError(RuntimeError&&);
    //! move constructor
    RuntimeError(const RuntimeError&);
    //! destructor
    virtual ~RuntimeError();
    /*!
     * \return the error message
     */
    const char* what() const noexcept override;

   private:
    std::string msg;
  };  // end of struct RuntimeError

}  // end of namespace madnex

#endif /* LIB_MADNEX_EXCEPTIONS_HXX */
