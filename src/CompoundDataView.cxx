/*!
 * \file   CompoundDataView.cxx
 * \brief
 * \author Thomas Helfer
 * \date   24 janv. 2017
 */

#include "Madnex/Raise.hxx"
#include "Madnex/CompoundDataView.hxx"

namespace madnex {

  CompoundDataView::CompoundDataView(const Group& g, const std::string& n)
      : CompoundDataView(openDataSet(g, n)) {}  // end of CompoundDataView

  CompoundDataView::CompoundDataView(const DataSet& d)
      : CompoundDataViewBase(d) {
    const auto dspace = d.getSpace();
    if (dspace.getSimpleExtentNdims() != 1) {
      raise("CompoundDataView: invalid dimension");
    }
    hsize_t dims[1];
    dspace.getSimpleExtentDims(dims);
    if (dims[0] != 1) {
      raise("CompoundDataView: invalid structure dimension");
    }
    this->data_storage.resize(this->ctype.getSize());
    d.read(this->data_storage.data(), this->ctype);
    this->data_pointer = this->data_storage.data();
  }  // end of CompoundDataView::CompoundDataView

  CompoundDataView::CompoundDataView(const CompoundDataViewBase& b,
                                     const char* const rd)
      : CompoundDataViewBase(b),
        data_pointer(rd) {}  // end of CompoundDataView::CompoundDataView

  CompoundDataView::CompoundDataView(CompoundDataView&&) = default;
  CompoundDataView::CompoundDataView(const CompoundDataView&) = default;
  CompoundDataView& CompoundDataView::operator=(CompoundDataView&&) = default;
  CompoundDataView& CompoundDataView::operator=(const CompoundDataView&) =
      default;

  CompoundDataView::~CompoundDataView() = default;

}  // end of namespace madnex
