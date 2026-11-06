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
    try {
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
    } catch (H5::Exception& e) {
      raise(e.getDetailMsg());
    }
  }  // end of CompoundDataView::CompoundDataView

  CompoundDataView::CompoundDataView(const CompoundDataViewBase& b,
                                     const char* const rd)
      : CompoundDataViewBase(b),
        data_pointer(rd) {}  // end of CompoundDataView::CompoundDataView

  // \note `data_pointer` refers to `data_storage` when this object owns
  // its data (i.e. when `data_storage` is not empty) and to an external
  // buffer otherwise (a view over a `CompoundDataArrayView` element).
  // The special members below are hand-written so that the pointer is
  // re-pointed to the new owner's storage on copy (the defaulted
  // implementation kept it addressing the source's buffer, leading to a
  // use-after-free once the source was destroyed) and reset to `nullptr`
  // on move of an owning view.
  CompoundDataView::CompoundDataView(CompoundDataView&& other)
      : CompoundDataViewBase(std::move(other)),
        data_storage(std::move(other.data_storage)) {
    if (!this->data_storage.empty()) {  // the ownership was transferred
      this->data_pointer = this->data_storage.data();
      other.data_pointer = nullptr;
    } else {  // 'other' was a view over an external buffer
      this->data_pointer = other.data_pointer;
    }
  }
  CompoundDataView::CompoundDataView(const CompoundDataView& other)
      : CompoundDataViewBase(other),
        data_storage(other.data_storage),
        data_pointer(other.data_pointer) {
    if (!this->data_storage.empty()) {
      this->data_pointer = this->data_storage.data();
    }
  }
  CompoundDataView& CompoundDataView::operator=(CompoundDataView&& other) {
    if (this != &other) {
      CompoundDataViewBase::operator=(std::move(other));
      this->data_storage = std::move(other.data_storage);
      if (!this->data_storage.empty()) {  // the ownership was transferred
        this->data_pointer = this->data_storage.data();
        other.data_pointer = nullptr;
      } else {  // 'other' was a view over an external buffer
        this->data_pointer = other.data_pointer;
      }
    }
    return *this;
  }
  CompoundDataView& CompoundDataView::operator=(const CompoundDataView& other) {
    if (this != &other) {
      CompoundDataViewBase::operator=(other);
      this->data_storage = other.data_storage;
      this->data_pointer = other.data_pointer;
      if (!this->data_storage.empty()) {
        this->data_pointer = this->data_storage.data();
      }
    }
    return *this;
  }

  CompoundDataView::~CompoundDataView() = default;

}  // end of namespace madnex
