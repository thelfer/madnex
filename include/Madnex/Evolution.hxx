/*!
 * \file   Evolution.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   28/05/2018
 */

#ifndef LIB_MADNEX_EVOLUTION_HXX
#define LIB_MADNEX_EVOLUTION_HXX

#include<vector>
#include<string>

namespace madnex{

  struct MADNEX_EXPORT Evolution {
    //! list of the parameters' names
    std::vector<std::string> title;
    //! list of the parameters' units
    std::vector<std::string> unit;
    //! list of the parameters' descriptions
    std::vector<std::string> description;
    //! if 'true', the data types are heterogeneous
    bool structured;
  };  // end of struct Evolution

} // end of namespace madnex

#endif /* LIB_MADNEX_EVOLUTION_HXX */
