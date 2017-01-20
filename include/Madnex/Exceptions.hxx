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
  struct MADNEX_VISIBILITY_EXPORT MadnexException
    : public tfel::exception::TFELException
  {
    //! default constructor
    MadnexException();
    //! copy constructor
    MadnexException(const MadnexException&);
    //! move constructor
    MadnexException(MadnexException&&);
    //! destructor
    virtual ~MadnexException();
  }; // end of struct MadnexException

  /*!
   * \brief a generic exception
   */
  struct MADNEX_VISIBILITY_EXPORT MadnexRuntimeError
    : public MadnexException
  {
    /*!
     * \brief constructor from std::string
     * \param[in] m: error message
     */
    MadnexRuntimeError(const std::string&);
    /*!
     * \brief constructor from C-string
     * \param[in] m: error message
     */
    MadnexRuntimeError(const char*);
    //! copy constructor
    MadnexRuntimeError(MadnexRuntimeError&&);
    //! move constructor
    MadnexRuntimeError(const MadnexRuntimeError&);
    //! destructor
    virtual ~MadnexRuntimeError();
  private:
    std::string msg;
  }; // end of struct MadnexRuntimeError
  
} // end of namespace madnex

#endif /* LIB_MADNEX_EXCEPTIONS_HXX */
