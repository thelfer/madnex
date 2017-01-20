/*!
 * \file   include/Madnex/Config.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 janv. 2017
 */

#ifndef LIB_MADNEX_CONFIG_HXX
#define LIB_MADNEX_CONFIG_HXX

#include"TFEL/Config/TFELConfig.hxx"

/*!
 * \def MADNEX_VISIBILITY_EXPORT
 * Define the MADNEX_VISIBILITY_EXPORT macro using cmake conventions.
 * When compiling libMadnex, cmake defines the macro Madnex_EXPORTS.
 * This allows to distinguish import/export definitions of symbols.
 */
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined Madnex_EXPORTS
#    define MADNEX_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#  ifndef MADNEX_STATIC_BUILD
#    define MADNEX_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  else 
#    define MADNEX_VISIBILITY_EXPORT 
#  endif
#  endif
#else
#  define MADNEX_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MADNEX_CONFIG_HXX_ */

#endif /* LIB_MADNEX_CONFIG_HXX */
