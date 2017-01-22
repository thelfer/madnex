/*! 
 * \file   include/Madnex/HDF5.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  16/01/2017
 */

#ifndef LIB_MADNEX_HDF5_H
#define LIB_MADNEX_HDF5_H 

#include<string>
#include<vector>
#include<H5Cpp.h>

#include"Madnex/Config.hxx"

/*!
 * \brief macro used to define the `read`/`write`
 * associated to plain old data types.
 * \param X: type
 */
#define MADNEX_HDF5_POD_DECLARATION(X)		\
  /*!						\
   * \param[out] g: group			\
   * \param[in]  n: name of the data		\
   * \param[in]  o: object to be written	\
   */						\
  MADNEX_VISIBILITY_EXPORT void			\
  write(Group&,                                 \
        const std::string&,			\
	const X&);			        \
  /*!						\
   * \param[out] o: object to be read		\
   * \param[in]  g: group			\
   * \param[in]  n: name of the data		\
   */						\
  MADNEX_VISIBILITY_EXPORT void			\
  read(X&,					\
       const Group&,                            \
       const std::string&);			\
  /*!						\
   * \return the object read		        \
   * \param[in] g: group			\
   * \param[in] n: name of the data		\
   */						\
  template<>                                    \
  MADNEX_VISIBILITY_EXPORT X			\
  read(const Group&,                            \
       const std::string&);		        \
  /*!						\
   * \param[out] o: attribute value to be read	\
   * \param[in]  g: group			\
   * \param[in]  n: name of the data		\
   */						\
  MADNEX_VISIBILITY_EXPORT void			\
  getAttributeValue(X&,		                \
                    const Group&,	        \
		    const std::string&);	\
  /*!						\
   * \param[out] o: attribute value to be read	\
   * \param[in]  g: data set			\
   * \param[in]  n: name of the data		\
   */						\
  MADNEX_VISIBILITY_EXPORT void			\
  getAttributeValue(X&,		                \
                    const DataSet&,	        \
		    const std::string&)

namespace madnex
{

  using H5::PredType;
  using H5::Group;
  using H5::DataSet;
  using H5::Attribute;
  using H5::Exception;
  using H5::StrType;
  using H5::DataSpace;

  /*!
   * \brief create a new group
   * \param[in] g: parent group
   * \param[in] n: group name
   */
  MADNEX_VISIBILITY_EXPORT Group
  createGroup(const Group&,
	      const std::string&);
  /*!
   * \brief open a new group
   * \param[in] g: parent group
   * \param[in] n: group name
   */
  MADNEX_VISIBILITY_EXPORT Group
  openGroup(const Group&,
	    const std::string&);
  /*!
   * \brief open a data set
   * \param[in] g: parent group
   * \param[in] n: data set name
   */
  MADNEX_VISIBILITY_EXPORT DataSet
  openDataSet(const Group&,
	      const std::string&);
  /*!
   * \brief open an attribute of a data set
   * \param[in] d: data set
   * \param[in] n: attribute name
   */
  MADNEX_VISIBILITY_EXPORT Attribute
  openAttribute(const DataSet&,
		const std::string&);
  /*!
   * \brief open an attribute of a group
   * \param[in] g: group
   * \param[in] n: attribute name
   */
  MADNEX_VISIBILITY_EXPORT Attribute
  openAttribute(const Group&,
		const std::string&);
  /*!
   * \param[in]  g: group
   * \param[in]  b: boolean allowing other objects than groups to be
   * inside the given group
   */
  MADNEX_VISIBILITY_EXPORT std::vector<std::string>
  getSubGroupNames(const Group&,
		   const bool = false);
  /*!
   * \param[out] n: names
   * \param[in]  g: group
   * \param[in]  b: boolean allowing other objects than groups to be
   * inside the given group
   */
  MADNEX_VISIBILITY_EXPORT void
  getSubGroupNames(std::vector<std::string>&,
		   const Group&,
		   const bool = false);
  /*!
   * \return all the dataset names in a give group
   * \param[in]  g: group
   */
  MADNEX_VISIBILITY_EXPORT std::vector<std::string>
  getDataSetNames(const Group&);
  /*!
   * \return all the dataset names in a give group
   * \param[out] n: names
   * \param[in]  g: group
   */
  MADNEX_VISIBILITY_EXPORT void
  getDataSetNames(std::vector<std::string>&,
		  const Group&);
  /*!
   * \return true if the given group contains an object with the
   * given name
   * \param[in] g: group
   * \param[in] n: name
   */
  MADNEX_VISIBILITY_EXPORT bool
  contains(const Group&,
	   const std::string&);
  /*!
   * \brief set an attribute stating that the data set is empty
   * \param[in,out] d: data set
   */
  MADNEX_VISIBILITY_EXPORT void
  writeEmptyObjectAttribute(DataSet&);
  /*!
   * \return true of the data set is the empty attribute
   * \param[in,out] d: data set
   */
  MADNEX_VISIBILITY_EXPORT bool
  hasEmptyObjectAttribute(const DataSet&);
  /*!
   * \return true if the given group has an attribute with the given
   * name
   * \param[in] g: group
   * \param[in] n: name
   */
  MADNEX_VISIBILITY_EXPORT bool
  hasAttribute(const DataSet&,
	       const std::string&);
  /*!
   * \brief retrieve the names of the attributes of a group
   * \param[out] n: attributes' names
   * \param[in]  g: group
   */
  MADNEX_VISIBILITY_EXPORT void
  getAttributeNames(std::vector<std::string>&,
		    const Group&);
  /*!
   * \return    the names of the attributes of a group
   * \param[in] g: group
   */
  MADNEX_VISIBILITY_EXPORT std::vector<std::string>
  getAttributeNames(const Group&);
  /*!
   * \brief retrieve the names of the attributes of a data set
   * \param[out] n: attributes' names
   * \param[in]  d: data set
   */
  MADNEX_VISIBILITY_EXPORT void
  getAttributeNames(std::vector<std::string>&,
		    const DataSet&);
  /*!
   * \return    the names of the attributes of a data set
   * \param[in] g: data set
   */
  MADNEX_VISIBILITY_EXPORT std::vector<std::string>
  getAttributeNames(const DataSet&);
  /*!
   * \brief retrieve the value of an attribute
   * \tparam    T: type of the value to be retrieved
   * \param[in] g: group
   * \param[in] n: attribute name
   */
  template<typename T>
  T getAttributeValue(const Group&,
		      const std::string&);
  /*!
   * \brief retrieve the value of an attribute
   * \param[in] g: group
   * \param[in] n: attribute name
   */
  template<>
  MADNEX_VISIBILITY_EXPORT std::string
  getAttributeValue<std::string>(const Group&,
				 const std::string&);
  /*!
   * \brief retrieve the value of an attribute
   * \param[out] v: value read
   * \param[in]  g: group
   * \param[in]  n: attribute name
   */
  MADNEX_VISIBILITY_EXPORT void
  getAttributeValue(std::string&,
		    const Group&,
		    const std::string&);
  /*!
   * \brief retrieve the value of an attribute
   * \tparam    T: type of the value to be retrieved
   * \param[in] d: data set
   * \param[in] n: attribute name
   */
  template<typename T>
  T getAttributeValue(const DataSet&,
		      const std::string&);
  /*!
   * \brief retrieve the value of an attribute
   * \param[in] d: data set
   * \param[in] n: attribute name
   */
  template<>
  MADNEX_VISIBILITY_EXPORT std::string
  getAttributeValue<std::string>(const DataSet&,
				 const std::string&);
  /*!
   * \brief retrieve the value of an attribute
   * \param[out] v: value read
   * \param[in]  d: data set
   * \param[in]  n: attribute name
   */
  MADNEX_VISIBILITY_EXPORT void
  getAttributeValue(std::string&,
		    const DataSet&,
		    const std::string&);
  /*!
   * \return the type attribute of a group.
   * \param[in]  g: group
   */
  MADNEX_VISIBILITY_EXPORT std::string
  getTypeAttribute(const Group&);
  /*!
   * \return the type attribute of a group or a dataset which has the
   * given name and which is part of the given group.
   * \param[in]  g: group
   * \param[in]  n: group or dataset name
   */
  MADNEX_VISIBILITY_EXPORT std::string
  getTypeAttribute(const Group&,
		   const std::string&);
  /*!
   * get the type attribute of a group or a dataset which has the
   * given name and which is part of the given group.
   *
   * \param[out] t: type
   * \param[in]  g: group
   * \param[in]  n: group or dataset name
   */
  MADNEX_VISIBILITY_EXPORT void
  getTypeAttribute(std::string&,
		   const Group&,
		   const std::string&);
  /*!
   * \brief get the type attribute of a group
   * \param[out] t: type
   * \param[in]  g: group
   */
  MADNEX_VISIBILITY_EXPORT void
  getTypeAttribute(std::string&,
		   const Group&);
  /*!
   * \return get the type attribute of a dataset
   * \param[in] d: dataset
   */
  MADNEX_VISIBILITY_EXPORT std::string
  getTypeAttribute(const DataSet&);
  /*!
   * \brief get the type attribute of a dataset
   * \param[out] t: type
   * \param[in]  d: dataset
   */
  MADNEX_VISIBILITY_EXPORT void
  getTypeAttribute(std::string&,
		   const DataSet&);
  /*!
   * \brief write the type attribute to a data set
   * \param[in,out] d: data set
   * \param[in]     t: type
   */
  MADNEX_VISIBILITY_EXPORT void
  writeTypeAttribute(DataSet&,
		     const std::string&);
  /*!
   * \brief write the type attribute to a group
   * \param[in,out] g: group
   * \param[in]     t: type
   */
  MADNEX_VISIBILITY_EXPORT void
  writeTypeAttribute(Group&,
		     const std::string&);
  /*!
   * \brief write the type attribute to a group
   * \tparam T: type
   * \param[in,out] g: group
   */
  template<typename T>
  void writeTypeAttribute(Group&);
  /*!
   * \brief write the type attribute to a data set
   * \tparam T: type
   * \param[in,out] d: group
   */
  template<typename T>
  void writeTypeAttribute(DataSet&);
  /*!
   * check the type attribute of a group
   * \param[in] g: group
   * \param[in] t: expected type
   * \throw std::runtime_error if the type is not what is expected
   */
  MADNEX_VISIBILITY_EXPORT void
  checkTypeAttribute(const Group&,
		     const std::string&);

  /*!
   * check the type attribute of a dataset
   * \param[in] d: dataset
   * \param[in] t: expected type
   * \throw std::runtime_error if the type is not what is expected
   */
  MADNEX_VISIBILITY_EXPORT void
  checkTypeAttribute(const DataSet&,
		     const std::string&);
  /*!
   * \brief check the type attribute to a group
   * \tparam    T: type
   * \param[in] g: group
   */
  template<typename T>
  void checkTypeAttribute(const Group&);
  /*!
   * \brief check the type attribute to a data set
   * \tparam    T: type
   * \param[in] d: group
   */
  template<typename T>
  void checkTypeAttribute(const DataSet&);

  /*!
   * \return the object read
   * \param d: name of the dataset
   * \param g: group
   */
  template<typename T>
  T read(const Group&,
	 const std::string&);
  
  MADNEX_HDF5_POD_DECLARATION(bool);
  MADNEX_HDF5_POD_DECLARATION(char);
  MADNEX_HDF5_POD_DECLARATION(short);
  MADNEX_HDF5_POD_DECLARATION(int);
  MADNEX_HDF5_POD_DECLARATION(long);
  MADNEX_HDF5_POD_DECLARATION(unsigned short);
  MADNEX_HDF5_POD_DECLARATION(unsigned int);
  MADNEX_HDF5_POD_DECLARATION(unsigned long);
  MADNEX_HDF5_POD_DECLARATION(float);
  MADNEX_HDF5_POD_DECLARATION(double);
  MADNEX_HDF5_POD_DECLARATION(long double);

  /*!
   * \brief write a string
   * \param[out] g: group
   * \param[in]  n: name of the data
   * \param[in]  o: object to be written
   */
  MADNEX_VISIBILITY_EXPORT
  void write(Group&,
	     const std::string&,
	     const std::string&);
  /*!
   * \brief read a string
   * \param[out] o: object to be read
   * \param[in]  g: group
   * \param[in]  n: name of the data
   */
  MADNEX_VISIBILITY_EXPORT
  void read(std::string&,
	    const Group&,
	    const std::string&);
  /*!
   * \brief write a C-string. It is stored and retrieved like a
   * `std::string`.
   * \param[out] g: group
   * \param[in]  n: name of the data
   * \param[in]  o: object to be written
   */
  MADNEX_VISIBILITY_EXPORT
  void write(Group&,
	     const std::string&,
	     const char* const);
  
} // end of namespace madnex

#include"Madnex/HDF5.ixx"

#endif /* LIB_MADNEX__HDF5_H */

