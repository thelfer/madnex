/*!
 * \file   CompoundTypeWithString2.cxx
 * \brief
 * \author Thomas Helfer
 * \date   18/01/2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cstring>
#include <cstdlib>
#include <cassert>
#include <iostream>

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
 * CompoundDataSetWithString2 type in
 * it.
 */
static void write_file() {
  using namespace madnex;
  auto f = File("CompoundDataSetWithString2.madnex", H5F_ACC_TRUNC);
  auto r = f.getRoot();
  // compound description
  H5::CompType c(sizeof(CString));
  const auto stype = getNativeType<std::string>();
  c.insertMember("value", HOFFSET(CString, value), stype);
  // value to be stored
  std::string test = "my test value";
  // place holder
  CString ctest;
  ctest.value = test.c_str();
  // data set creation
  hsize_t dim[] = {1}; /* Dataspace dimensions */
  const auto d = r.createDataSet("test", c, DataSpace(1, dim));
  d.write(&ctest, c);
}

/*!
 * \brief this method reads the file written by the `write_file`
 * method and checks that all the data can be retrieved without loss.
 */
static void read_file() {
  using namespace madnex;
  const auto f = File("CompoundDataSetWithString2.madnex", H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  // compound description
  H5::CompType c(sizeof(CString));
  const auto stype = getNativeType<std::string>();
  c.insertMember("value", HOFFSET(CString, value), stype);
  // value to be read
  CString test;
  // reading
  const auto d = r.openDataSet("test");
  d.read(&test, c);
  assert(std::strcmp(test.value, "my test value") == 0);
}

int main() {
  write_file();
  read_file();
  return EXIT_SUCCESS;
}
