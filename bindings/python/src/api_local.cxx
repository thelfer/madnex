/*!
 * \file   bindings/python/src/api_local.cxx
 * \brief
 * \author Helfer Thomas
 * \date   31 janv. 2017
 */

#include <boost/python.hpp>

void declareFile();

BOOST_PYTHON_MODULE(api_local) { declareFile(); }
