/*!
 * \file   MainResults.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17 janv. 2017
 */

#include<cstdlib>
#include<stdexcept>
#include"Madnex/Types.hxx"
#include"Madnex/CompoundDataArrayView.hxx"
#include"Madnex/MainResults.hxx"

namespace madnex{
  
  MainResult::MainResult() = default;
  MainResult::MainResult(MainResult&&) = default;
  MainResult::MainResult(const MainResult&) = default;
  MainResult&
  MainResult::operator=(MainResult&&) = default;
  MainResult&
  MainResult::operator=(const MainResult&) = default;

  // void write(Group& g,
  // 	     const std::string& n,
  // 	     const MainResults& o)
  // {
  //   auto make_copy = [](const MainResult& ec){
  //     CMainResult<const char> r;
  //     r.description  = ec.description.c_str();
  //     r.label = ec.label.c_str();
  //     r.value = ec.value.c_str();
  //     r.unit  = ec.unit.c_str();
  //     r.type  = ec.type.c_str();
  //     return std::move(r);
  //   };
  //   const auto c = getMainResultCompType<const char>();
  //   std::vector<CMainResult<const char>> cecs;
  //   cecs.reserve(o.size());
  //   for(const auto& ec: o){
  //     cecs.emplace_back(make_copy(ec));
  //   }
  //   hsize_t dim[] = {o.size()};   /* Dataspace dimensions */
  //   const auto d = g.createDataSet(n,c,DataSpace(1,dim));
  //   d.write(cecs.data(),c);
  // } // end of write

  void read(MainResults& o,const Group& g,
  	    const std::string& n)
  {
    o.clear();
    try{
      CompoundDataArrayView a(openDataSet(g,n));
      for(decltype(a.size()) i=0;i!=a.size();++i){
	const auto v = a[i];
	MainResult r;
  	extract(r.name ,v,size_t(0));
  	extract(r.label,v,1);
  	extract(r.value,v,2);
  	extract(r.unit ,v,3);
  	extract(r.type ,v,4);
  	o.emplace_back(std::move(r));
      }
    } catch (H5::Exception& e){
      throw(std::runtime_error(e.getDetailMsg()));
    }
  }
  
} // end of namespace madnex
