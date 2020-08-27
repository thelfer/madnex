/*!
 * \file   include/Madnex/Config.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   16 janv. 2017
 */

#ifndef LIB_MADNEX_CONFIG_HXX
#define LIB_MADNEX_CONFIG_HXX

#ifdef __cplusplus
#include <cstddef>
#else  /*  __cplusplus */
#include <stddef.h>
#endif /*  __cplusplus */

/*!
 * Macro extracted from :
 * "Why is the new C++ visibility support so useful?"
 * from http://gcc.gnu.org/wiki/Visibility
 */
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#define MADNEX_VISIBILITY_IMPORT __declspec(dllimport)
#define MADNEX_VISIBILITY_EXPORT __declspec(dllexport)
#define MADNEX_VISIBILITY_LOCAL
#else /* defined _WIN32 || defined __CYGWIN__ */
#if (defined __GNUC__) && (! defined __INTEL_COMPILER)
#if __GNUC__ >= 4
#define MADNEX_VISIBILITY_IMPORT __attribute__((visibility("default")))
#define MADNEX_VISIBILITY_EXPORT __attribute__((visibility("default")))
#define MADNEX_VISIBILITY_LOCAL  __attribute__((visibility("hidden")))
#else /* __GNUC__ >= 4 */
#define MADNEX_VISIBILITY_IMPORT
#define MADNEX_VISIBILITY_EXPORT
#define MADNEX_VISIBILITY_LOCAL
#endif /* __GNUC__ >= 4 */
#elif defined __INTEL_COMPILER
#define MADNEX_VISIBILITY_IMPORT __attribute__((visibility("default")))
#define MADNEX_VISIBILITY_EXPORT __attribute__((visibility("default")))
#define MADNEX_VISIBILITY_LOCAL  __attribute__((visibility("hidden")))
#else /* defined __INTEL_COMPILER */
#define MADNEX_VISIBILITY_IMPORT
#define MADNEX_VISIBILITY_EXPORT
#define MADNEX_VISIBILITY_LOCAL
#endif /* defined __INTEL_COMPILER */
#endif /* defined _WIN32 || defined _WIN64 ||defined __CYGWIN__ */

#define MADNEX_INLINE inline
#define MADNEX_NORETURN [[noreturn]]

/*!
 * \def MADNEX_VISIBILITY_EXPORT
 * Define the MADNEX_VISIBILITY_EXPORT macro using cmake conventions.
 * When compiling libMadnex, cmake defines the macro Madnex_EXPORTS.
 * This allows to distinguish import/export definitions of symbols.
 */
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined Madnex_EXPORTS
#    define MADNEX_EXPORT MADNEX_VISIBILITY_EXPORT
#  else
#  ifndef MADNEX_STATIC_BUILD
#    define MADNEX_EXPORT MADNEX_VISIBILITY_IMPORT
#  else 
#    define MADNEX_EXPORT 
#  endif
#  endif
#else
#  define MADNEX_EXPORT MADNEX_VISIBILITY_EXPORT
#endif /* LIB_MADNEX_CONFIG_HXX_ */

#endif /* LIB_MADNEX_CONFIG_HXX */
