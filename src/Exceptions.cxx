/*!
 * \file   Exceptions.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17/01/2017
 */

#include"Madnex/Exceptions.hxx"

namespace madnex{

  Exception::Exception() = default;

  Exception::Exception(const Exception&) = default;

  Exception::Exception(Exception&&) = default;

  Exception::Exception::~Exception() = default;

  RuntimeError::RuntimeError(const std::string& m)
    : msg(m)
  {} // end of RuntimeError::RuntimeError

  RuntimeError::RuntimeError(const char* m)
    : msg(m)
  {} // end of RuntimeError::RuntimeError
  
  RuntimeError::RuntimeError(const RuntimeError&) = default;

  RuntimeError::RuntimeError(RuntimeError&&) = default;

  RuntimeError::~RuntimeError() = default;

} // end of namespace madnex
