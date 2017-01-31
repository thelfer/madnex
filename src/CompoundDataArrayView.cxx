/*!
 * \file   CompoundDataArrayView.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 janv. 2017
 */

#include"Madnex/CompoundDataArrayView.hxx"

namespace madnex{

  CompoundDataArrayView::CompoundDataArrayView(const DataSet& d)
    : CompoundDataViewBase(d)
  {
    const auto dspace = d.getSpace();
    if(dspace.getSimpleExtentNdims()!=1){
      throw(std::runtime_error("CompoundDataArrayView: "
			       "invalid dimension"));
    }
    hsize_t dims[1];
    dspace.getSimpleExtentDims(dims);
    this->s=dims[0];
    this->rdata.resize(this->ctype.getSize()*(this->s));
    d.read(this->rdata.data(),this->ctype);
  } // end of CompoundDataArrayView::CompoundDataArrayView

  CompoundDataView CompoundDataArrayView::operator[](const size_type i) const
  {
    return CompoundDataView(*this,this->rdata.data()+i*(this->ctype.getSize()));
  } // end of CompoundDataArrayView::operator[]

  CompoundDataView CompoundDataArrayView::at(const size_type i) const
  {
    if(i>=this->size()){
      throw(std::out_of_range("CompoundDataArrayView::at: invalid index"));
    }
    return CompoundDataView(*this,this->rdata.data()+i*(this->ctype.getSize()));
  } // end of CompoundDataArrayView::at
  
  CompoundDataArrayView::size_type CompoundDataArrayView::size() const
  {
    return this->s;
  } // end of CompoundDataArrayView::size() const
  
  CompoundDataArrayView::~CompoundDataArrayView(){
    // const unsigned s = static_cast<unsigned>(this->ctype.getNmembers());
    // for(unsigned i=0;i!=s;++i){
    //   const auto t = this->ctype.getMemberClass(i);
    //   if(t==madnex::getNativeType<std::string>().getClass()){
    // 	const auto ptr =
    // 	  this->data.data()+this->ctype.getMemberOffset(i);
    // 	std::free(ptr);
    //   }
    // }
  } // end of CompoundDataArrayView::~CompoundDataArrayView
  
} // end of namespace madnex
