#pragma once
#include <string>
#include <memory>
#include "hdf5.h"
#include "Utilities/ErrorUtils.h"
#include "Utilities/H5/H5PropertyList.h"
#include "Utilities/Containers/ndarray.h"

namespace Utilities
{
    namespace H5
    {
        /// forward declaration
        class Group;
        class Attribute;
        class Datatype;
        class Dataspace;
        class Dataset;

        class POST_BASE_API ObjectBase
        {
        protected:
            hid_t m_id = H5I_INVALID_HID; //!< handle of the object, -1 means closed
        public:
            /// constructor
            ObjectBase() = default;                                                                  //!< empty object
            ObjectBase(hid_t id);                                                                    //!< converting constructor
            ObjectBase(hid_t id, const std::string &name, const PropertyList &apl = PropertyList()); //!< open an object using handle and relative name
            ObjectBase(const ObjectBase &rhs) = delete;                                              //!< copy constructor deleted
            ObjectBase(ObjectBase &&rhs);                                                            //!< move constructor

            /// operations
            virtual void Close();                                                                           //!< close the object
            bool Exists(const std::string &name) const;                                                     //!< recursively verify if an object with abs/relative name exists
            virtual void Open(hid_t id, const std::string &name, const PropertyList &apl = PropertyList()); //!< open an object using handle and relative name

            /// creator functions
            std::shared_ptr<Group> CreateGroup(const std::string &name,
                                               const PropertyList &gcpl = PropertyList(),
                                               const PropertyList &gapl = PropertyList(),
                                               const PropertyList &lcpl = PropertyList());

            std::shared_ptr<Dataset> CreateDataset(const std::string &name,
                                                   const Datatype &dtype,
                                                   const Dataspace &space,
                                                   const PropertyList &dcpl = PropertyList(),
                                                   const PropertyList &dapl = PropertyList(),
                                                   const PropertyList &lcpl = PropertyList()); //!< create dataset

            template <typename T>
            std::shared_ptr<Dataset> CreateDataset(const std::string &name,
                                                   const ndarray<T> &data,
                                                   const PropertyList &dcpl = PropertyList(),
                                                   const PropertyList &dapl = PropertyList(),
                                                   const PropertyList &lcpl = PropertyList(),
                                                   const PropertyList &xferpl = PropertyList()); //!< create dataset using an ndarray

            std::shared_ptr<Attribute> CreateAttribute(const std::string &name,
                                                       const Datatype &dtype,
                                                       const Dataspace &space,
                                                       const PropertyList &acpl = PropertyList(),
                                                       const PropertyList &aapl = PropertyList());

            template <typename T>
            std::shared_ptr<Attribute> CreateAttribute(const std::string &name,
                                                       const ndarray<T> &data,
                                                       const PropertyList &acpl = PropertyList(),
                                                       const PropertyList &aapl = PropertyList()); //!< create attribute with ndarray
            void DeleteAttribute(const std::string &name);

            /// get functions
            hid_t GetHandle() const { return m_id; }
            std::shared_ptr<Group> GetGroup(const std::string &name, const PropertyList &gapl = PropertyList()) const;         //!< get group using absolute or relative name
            std::shared_ptr<Dataset> GetDataset(const std::string &name, const PropertyList &dapl = PropertyList()) const;     //!< get dataset using absolute or relative name
            std::shared_ptr<Attribute> GetAttribute(const std::string &name, const PropertyList &aapl = PropertyList()) const; //!< get dataset using absolute or relative name
            H5I_type_t GetType() const;                                                                                        //!< get type of the object                                                                                        //!< get type of this object
            virtual std::shared_ptr<PropertyList> GetAccessPropertyList() const
            {
                FATAL_ERROR("Can't get access propertylist from object");
                return std::shared_ptr<PropertyList>();
            }
            virtual std::shared_ptr<PropertyList> GetCreatePropertyList() const
            {
                FATAL_ERROR("Can't get access propertylist from object");
                return std::shared_ptr<PropertyList>();
            }
            virtual std::string GetName() const; //!< get the full name of the object                                                                                      //!< get the absolute name of the object
            std::string GetBaseName() const;     //!< get base name of the object                                                  //!< get the base name of the object

            /// operators
            ObjectBase &operator=(const ObjectBase &rhs) = delete; //!< copy assignment
            ObjectBase &operator=(ObjectBase &&rhs);               //!< move assignment

            virtual ~ObjectBase() { Close(); } //!< destructor
        };
    }
}