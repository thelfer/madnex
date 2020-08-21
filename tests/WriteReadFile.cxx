/*!
 * \file   tests/WriteReadFile.cxx
 * \brief
 * \author Thomas Helfer
 * \date   16/01/2017
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cmath>
#include <limits>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <iostream>

#include "Madnex/HDF5.hxx"
#include "Madnex/File.hxx"


/*!
 * \brief this method create a file and stores various POD type in
 * it.
 * \param[in] fn: file name
 */
static void write_file(const char* const fn) {
  using namespace madnex;
  auto f = File("MadnexReadWriteFile.madnex", H5F_ACC_TRUNC);
  auto r = f.getRoot();
  // read the file and stores it in a std::string
  std::ifstream in(fn);
  std::string content(std::istreambuf_iterator<char>{in}, {});
  write(r, "source", content);
}

/*!
 * \brief this method reads the file written by the `write_file`
 * method and checks that all the data can be retrieved without loss.
 *
 * \param[in] fn: file name
 */
static void read_file(const char* const fn) {
  using namespace madnex;
  const auto f = File("MadnexReadWriteFile.madnex", H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  std::ifstream in(fn);
  std::string content(std::istreambuf_iterator<char>{in}, {});
  assert(read<std::string>(r, "source") == content);
}

int main(const int argc, const char* const* argv) {
  if (argc != 2) {
    std::cout << "argc: " << argc << '\n';
    std::cerr << "WriteReadFile: invalid number of arguments\n"
              << "usage: WriteReadFile file.mfront\n";
    return EXIT_FAILURE;
  }
  write_file(argv[1]);
  read_file(argv[1]);
  return EXIT_SUCCESS;
}
