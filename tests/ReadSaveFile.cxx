/*!
 * \file   tests/MadnexPOD.cxx
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
#include <iostream>

#include "Madnex/HDF5.hxx"
#include "Madnex/File.hxx"

/*!
 * \brief read a floatting_point value.
 * \param[in] g: group
 * \param[in] n: variable name
 * \param[in] v: expected value
 */
template <typename T>
static void read_floatting_point_number(const madnex::Group& g,
                                        const std::string& n,
                                        const T v) {
  const auto d = madnex::read<T>(g, n);
  assert(std::abs(d - v) < v * std::numeric_limits<T>::epsilon());
  auto catched = false;
  try {
    madnex::read<std::string>(g, n);
  } catch (std::runtime_error&) {
    catched = true;
  }
  assert(catched);
}

/*!
 * \brief this method create a file and stores various POD type in
 * it.
 */
static void write_file() {
  using namespace madnex;
  auto f = File("MadnexPOD.madnex", H5F_ACC_TRUNC);
  auto r = f.getRoot();
  write(r, "boolean_value", true);
  write(r, "char_value", 'c');
  write(r, "integer_value", static_cast<std::int32_t>(12));
  write(r, "float_value", 12.f);
  write(r, "double_value", 12.);
  write(r, "long_double_value", static_cast<long double>(12));
  write(r, "string_value", "lorem ipsum");
}

/*!
 * \brief this method reads the file written by the `write_file`
 * method and checks that all the data can be retrieved without loss.
 */
static void read_file() {
  using namespace madnex;
  const auto f = File("MadnexPOD.madnex", H5F_ACC_RDONLY);
  const auto r = f.getRoot();
  assert(read<char>(r, "char_value") == 'c');
  assert(read<std::int32_t>(r, "integer_value") == 12);
  {
    auto catched = false;
    try {
      read<double>(r, "boolean_value");
    } catch (std::runtime_error&) {
      catched = true;
    }
    assert(catched);
  }
  read_floatting_point_number<float>(r, "float_value", 12.f);
  read_floatting_point_number<double>(r, "double_value", 12.);
  read_floatting_point_number<long double>(r, "long_double_value", 12.);
  assert(read<std::string>(r, "string_value") == "lorem ipsum");
  {
    auto catched = false;
    try {
      read<double>(r, "string_value");
    } catch (std::runtime_error&) {
      catched = true;
    }
    assert(catched);
  }
}

int main() {
  write_file();
  read_file();
  return EXIT_SUCCESS;
}
