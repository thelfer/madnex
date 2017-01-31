/*!
 * \file   CompoundDataArrayView.ixx
 * \brief    
 * \author Helfer Thomas
 * \date   31 janv. 2017
 */

#ifndef LIB_MADNEX_COMPOUNDDATAARRAYVIEW_IXX
#define LIB_MADNEX_COMPOUNDDATAARRAYVIEW_IXX

namespace madnex{

  template<typename T>
  std::vector<T> CompoundDataArrayView::extract(const char *n) const{
    return this->template extract<T>(this->getMemberIndex(n));
  } // end of CompoundDataArrayView::extract

  template<typename T>
  std::vector<T> CompoundDataArrayView::extract(const std::string& n) const{
    return this->template extract<T>(this->getMemberIndex(n));
  } // end of CompoundDataArrayView::extract
  
  template<typename T>
  std::vector<T> CompoundDataArrayView::extract(const size_t i) const{
    std::vector<T> r;
    r.reserve(this->s);
    const auto cs = this->ctype.getSize();
    for(decltype(this->s) e=0;e!=this->s;++e){
      r.push_back(CompoundDataViewBase::template extract<T>(this->rdata.data()+e*cs,i));
    }
    return r;
  } // end of CompoundDataArrayView::extract

} // end of namespace madnex

#endif /* LIB_MADNEX_COMPOUNDDATAARRAYVIEW_IXX */
