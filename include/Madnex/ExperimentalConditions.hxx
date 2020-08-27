/*!
 * \file   ExperimentalConditions.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   17/01/2017
 */

#ifndef LIB_MADNEX_EXPERIMENTALCONDITIONS_HXX
#define LIB_MADNEX_EXPERIMENTALCONDITIONS_HXX

#include<vector>
#include"Madnex/Config.hxx"
#include"Madnex/HDF5.hxx"

namespace madnex {

  /*!
   * \brief a structure describing the experimental condition
   */
  struct MADNEX_EXPORT ExperimentalCondition
  {
    //! default constructor
    ExperimentalCondition();
    //! move constructor
    ExperimentalCondition(ExperimentalCondition&&);
    //! copy constructor
    ExperimentalCondition(const ExperimentalCondition&);
    //! move assignement
    ExperimentalCondition& operator=(ExperimentalCondition&&);
    //! standard assignement
    ExperimentalCondition& operator=(const ExperimentalCondition&);
    
    std::string name;
    std::string label;
    std::string value;
    std::string unit;
    std::string type;
  }; // end of struct ExperimentalCondition

  //! a simple alias
  using ExperimentalConditions = std::vector<ExperimentalCondition>;
  
  /*!
   * \brief read an experimental conditions in a group
   * \param[out] o: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the data
   */
  MADNEX_EXPORT void
  read(ExperimentalConditions&,
       const Group&,
       const std::string&);
  /*!
   * \brief write an experimental conditions in a group
   * \param[out] g: group
   * \param[in]  n: name of the data
   * \param[in]  o: object to be written
   */
  MADNEX_EXPORT void
  write(Group&,
	const std::string&,
	const ExperimentalConditions&);
  
} // end of namespace madnex

#endif /* LIB_MADNEX_EXPERIMENTALCONDITIONS_HXX */
