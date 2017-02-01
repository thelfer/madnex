/*!
 * \file   Exceptions.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#ifndef LIB_MADNEX_EXCEPTIONS_HXX
#define LIB_MADNEX_EXCEPTIONS_HXX

#include<string>
#include<stdexcept>
#include"TFEL/Exception/TFELException.hxx"
#include"Madnex/Config.hxx"

namespace madnex {

  /*!
   * \brief base class for all exception throw by the `Madnex` library
   */
  struct MADNEX_VISIBILITY_EXPORT Exception
    : public tfel::exception::TFELException
  {
    //! default constructor
    Exception();
    //! copy constructor
    Exception(const Exception&);
    //! move constructor
    Exception(Exception&&);
    //! destructor
    virtual ~Exception();
  }; // end of struct Exception

  /*!
   * \brief a generic exception
   */
  struct MADNEX_VISIBILITY_EXPORT RuntimeError
    : public Exception
  {
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
  private:
    std::string msg;
  }; // end of struct RuntimeError
  
} // end of namespace madnex

#endif /* LIB_MADNEX_EXCEPTIONS_HXX */
