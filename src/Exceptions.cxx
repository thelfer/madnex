/*!
 * \file   Exceptions.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17/01/2017
 */

#include"Madnex/Exceptions.hxx"

namespace madnex{

  Exception::Exception() : std::runtime_error("madnex error") {}

  Exception::Exception(const Exception&) = default;

  Exception::Exception(Exception&&) = default;

  Exception::Exception::~Exception() = default;

  RuntimeError::RuntimeError() = default;

  RuntimeError::RuntimeError(const std::string& m)
    : Exception(), msg(m)
  {} // end of RuntimeError::RuntimeError

  RuntimeError::RuntimeError(const char* m)
    : Exception(), msg(m)
  {} // end of RuntimeError::RuntimeError
  
  RuntimeError::RuntimeError(const RuntimeError&) = default;

  RuntimeError::RuntimeError(RuntimeError&&) = default;

  RuntimeError::~RuntimeError() = default;

  const char* RuntimeError::what() const noexcept {
    return this->msg.c_str();
  } // end of RuntimeError::what

} // end of namespace madnex
