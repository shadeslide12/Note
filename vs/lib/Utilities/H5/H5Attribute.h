#pragma once
#include <string>
#include "hdf5.h"
#include "Utilities/H5/H5PropertyList.h"
#include "Utilities/H5/H5Dataspace.h"
#include "Utilities/H5/H5Datatype.h"

namespace Utilities
{
    namespace H5
    {
        class Datatype;
        class Dataspace;

        class POST_BASE_API Attribute
        {
        protected:
            hid_t m_id = H5I_INVALID_HID;

        public:
            /// constructors
            Attribute() = default; //!< empty attribute
            Attribute(hid_t id);   //!< converting constructor
            Attribute(hid_t id, const std::string &name,
                      const PropertyList &apl = PropertyList()); //!< open an attribute
            Attribute(const Attribute &rhs) = delete;            //!< copy constructor
            Attribute(Attribute &&rhs);                          //!< move constructor

            /// operations
            void Create(hid_t id, const std::string &name,
                        const Datatype &dtype,
                        const Dataspace &space,
                        const PropertyList &acpl = PropertyList(),
                        const PropertyList &aapl = PropertyList()); //!< create an attribute using handle and relative name
            void Open(hid_t id, const std::string &name,
                      const PropertyList &apl = PropertyList()); //!< open an attribute using handle and relative name
            void Write(const Datatype &memType, void *buf);      //!< write attribute
            void Read(const Datatype &memType, void *buf);       //!< read attribute
            template <typename T>
            void Write(const ndarray<T> &data); //!< write attribute
            template <typename T>
            void Read(ndarray<T> &data); //!< read attribute
            void Close();                //!< close attribute

            /// get functions
            hid_t GetHandle() const { return m_id; } //!< get handle
            std::string GetName() const;             //!< get name
            std::shared_ptr<PropertyList> GetAccessPropertyList() const;
            std::shared_ptr<PropertyList> GetCreatePropertyList() const;
            /// operators
            Attribute &operator=(const Attribute &rhs) = delete; //!< copy assignment
            Attribute &operator=(Attribute &&rhs);               //!< move assignment

            ~Attribute() { Close(); } //!< destructor
        };

        template <typename T>
        std::shared_ptr<Attribute> ObjectBase::CreateAttribute(const std::string &name,
                                                               const ndarray<T> &data,
                                                               const PropertyList &acpl,
                                                               const PropertyList &aapl)
        {
            auto dtype = Datatype::GetNative<T>();
            ndarray<hsize_t> shape(data.GetNumDim());
            for (uint32_t i = 0; i < shape.GetLength(); i++)
                shape(i) = data.GetDim(shape.GetLength() - 1 - i);
            auto space = std::make_shared<Dataspace>(H5Screate_simple(data.GetNumDim(), shape.begin(), nullptr));
            auto ret = CreateAttribute(name, *dtype, *space, acpl, aapl);
            ret->Write(data);
            return ret;
        }

        template <typename T>
        void Attribute::Write(const ndarray<T> &data)
        {
            auto space = Dataspace::CreateFrom(*this);
            int ndim = H5Sget_simple_extent_ndims(space->GetHandle());
            ASSERT((uint32_t)ndim == data.GetNumDim(), "data has to be of the same dimension as the dataset");
            ndarray<hsize_t> dims(ndim);
            ASSERT(H5Sget_simple_extent_dims(space->GetHandle(), dims.begin(), nullptr) >= 0, "Failed to get extent of the dataspace");
            for (uint32_t i = 0; i < (uint32_t)ndim; i++)
                ASSERT(dims(i) == data.GetDim(ndim - 1 - i), "data has to be of the same dimension as the dataset");
            auto dtype = Datatype::GetNative<T>();
            ASSERT(H5Awrite(m_id, dtype->GetHandle(), data.begin()) >= 0, "Failed to write to the attribute");
        }

        template <typename T>
        void Attribute::Read(ndarray<T> &data)
        {
            auto space = Dataspace::CreateFrom(*this);
            int ndim = H5Sget_simple_extent_ndims(space->GetHandle());
            ndarray<hsize_t> dims(ndim);
            ASSERT(H5Sget_simple_extent_dims(space->GetHandle(), dims.begin(), nullptr) >= 0, "Faild to get extent of the dataspace");
            // row major to column major
            ndarray<uint32_t> dims2(ndim);
            for (uint32_t i = 0; i < (uint32_t)ndim; i++)
                dims2(ndim - 1 - i) = dims(i);
            data.Setup(dims2);
            auto dtype = Datatype::GetNative<T>();
            ASSERT(H5Aread(m_id, dtype->GetHandle(), data.begin()) >= 0, "Failed to read from the attribute");
        }

    }
}