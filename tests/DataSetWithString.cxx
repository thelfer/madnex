/*!
 * \file   DataSetWithString.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19/01/2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cassert>

#include "Madnex/HDF5.hxx"
#include "Madnex/Types.hxx"
#include "Madnex/File.hxx"

//! a simple example of C-struct with a string
struct CString {
  //! pointer to a C-string
  const char* value;
};  // end of struct CString

/*!
 * \brief this method create a file and stores various
 * DataSetWithString type in
 * it.
 */
static void write_file() {
  using namespace madnex;
  auto f = File("DataSetWithString.madnex", H5F_ACC_TRUNC);
  auto r = f.getRoot();
  const auto stype = getNativeType<std::string>();
  const auto test = "my test value";
  // data set creation
  hsize_t dim[] = {1}; /* Dataspace dimensions */
  const auto d = r.createDataSet("test", stype, DataSpace(1, dim));
  d.write(&test, stype);
  f.close();
}

/*!
 * \brief this method reads the file written by the `write_file`
 * method and checks that all the data can be retrieved without loss.
 */
static void read_file() {
  using namespace madnex;
  auto f = File("DataSetWithString.madnex", H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  // compound description
  const auto stype = getNativeType<std::string>();
  char* test;
  // reading
  const auto d = r.openDataSet("test");
  d.read(&test, stype);
  assert(std::strcmp(test, "my test value") == 0);
  std::free(test);
  f.close();
}

int main() {
  write_file();
  read_file();
  return EXIT_SUCCESS;
}
