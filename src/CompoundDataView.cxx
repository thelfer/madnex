/*!
 * \file   CompoundDataView.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 janv. 2017
 */

#include"Madnex/CompoundDataView.hxx"

namespace madnex{

  CompoundDataView::CompoundDataView(const Group& g,
				     const std::string& n)
    : CompoundDataView(openDataSet(g,n))
  {} // end of CompoundDataView
  
  CompoundDataView::CompoundDataView(const DataSet& d)
    : CompoundDataViewBase(d)
  {
    const auto dspace = d.getSpace();
    if(dspace.getSimpleExtentNdims()!=1){
      throw(std::runtime_error("CompoundDataView: "
			       "invalid dimension"));
    }
    hsize_t dims[1];
    dspace.getSimpleExtentDims(dims);
    if(dims[0]!=1){
      std::runtime_error("CompoundDataView: invalid "
			 "structure dimension");
    }
    this->rdata = std::vector<char>{};
    auto& ld = this->rdata.get<std::vector<char>>();
    ld.resize(this->ctype.getSize());
    d.read(ld.data(),this->ctype);
  } // end of CompoundDataView::CompoundDataView

  CompoundDataView::CompoundDataView(const CompoundDataViewBase& b,
				     const char* const rd)
    : CompoundDataViewBase(b),
      rdata(rd)
  {} // end of CompoundDataView::CompoundDataView

  CompoundDataView::CompoundDataView(CompoundDataView&&) = default;
  CompoundDataView::CompoundDataView(const CompoundDataView&) = default;
  CompoundDataView& CompoundDataView::operator=(CompoundDataView&&) = default;
  CompoundDataView& CompoundDataView::operator=(const CompoundDataView&) = default;
  
  CompoundDataView::~CompoundDataView(){
    // const unsigned s = static_cast<unsigned>(this->ctype.getNmembers());
    // for(unsigned i=0;i!=s;++i){
    //   const auto t = this->ctype.getMemberClass(i);
    //   if(t==madnex::getNativeType<std::string>().getClass()){
    // 	const auto ptr =
    // 	  this->data.data()+this->ctype.getMemberOffset(i);
    // 	std::free(ptr);
    //   }
    // }
  } // end of CompoundDataView::~CompoundDataView
  
} // end of namespace madnex
