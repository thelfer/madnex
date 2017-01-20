/*!
 * \file   Exceptions.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17/01/2017
 */

#include"Madnex/Exceptions.hxx"

namespace madnex{

  MadnexException::MadnexException() = default;

  MadnexException::MadnexException(const MadnexException&) = default;

  MadnexException::MadnexException(MadnexException&&) = default;

  MadnexException::MadnexException::~MadnexException() = default;

  MadnexRuntimeError::MadnexRuntimeError(const std::string& m)
    : msg(m)
  {} // end of MadnexRuntimeError::MadnexRuntimeError

  MadnexRuntimeError::MadnexRuntimeError(const char* m)
    : msg(m)
  {} // end of MadnexRuntimeError::MadnexRuntimeError
  
  MadnexRuntimeError::MadnexRuntimeError(const MadnexRuntimeError&) = default;

  MadnexRuntimeError::MadnexRuntimeError(MadnexRuntimeError&&) = default;

  MadnexRuntimeError::~MadnexRuntimeError() = default;

} // end of namespace madnex
