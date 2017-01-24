/*!
 * \file   CompoundDataView.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 janv. 2017
 */

#include"Madnex/CompoundDataView.hxx"

namespace madnex{

  CompoundDataView::CompoundDataView(const DataSet& d)
  {
    const auto dtype = d.getDataType();
    if(dtype.getClass()!=H5T_COMPOUND){
      throw(std::runtime_error("CompoundExtractor: invalid data set"));
    }
    this->ctype = H5::CompType(d);
    this->rdata.resize(ctype.getSize());
    d.read(this->rdata.data(),ctype);
  }

  size_t CompoundDataView::getMemberIndex(const char * n) const{
    return this->ctype.getMemberIndex(n);
  } // end of CompoundDataView::getMemberIndex

  size_t CompoundDataView::getMemberIndex(const std::string& n) const{
    return this->ctype.getMemberIndex(n);
  } // end of CompoundDataView::getMemberIndex
  
  H5T_class_t CompoundDataView::getMemberClass(const size_t i) const
  {
    return this->ctype.getMemberClass(i);
  } // end of CompoundDataView::getMemberClass

  size_t CompoundDataView::getMemberOffset(const size_t i) const{
    return this->ctype.getMemberOffset(i);
  } // end of CompoundDataView::getMemberOffset

  StrType CompoundDataView::getMemberStrType(const size_t i) const{
    return this->ctype.getMemberStrType(i);
  } // end of CompoundDataView::getMemberStrType
  
  const char* CompoundDataView::data() const{
    return this->rdata.data();
  } // end of CompoundDataView::data

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
