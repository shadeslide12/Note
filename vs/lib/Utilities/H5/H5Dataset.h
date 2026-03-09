#pragma once
#include <algorithm>
#include "Utilities/H5/H5ObjectBase.h"
#include "Utilities/H5/H5Dataspace.h"
#include "Utilities/H5/H5Datatype.h"

namespace Utilities
{
    namespace H5
    {
        class POST_BASE_API Dataset : public ObjectBase
        {
        public:
            /// constructors
            Dataset() = default; //!< default constructor
            Dataset(hid_t id);   //!< converting constructor
            Dataset(hid_t id,
                    const std::string &name,
                    const PropertyList &apl = PropertyList()); //!< open a dataset using handle and relative/absolute name
            Dataset(const Dataset &rhs) = delete;              //!< copy constructor
            Dataset(Dataset &&rhs);                            //!< moving constructor

            /// operations
            virtual void Open(hid_t id,
                              const std::string &name,
                              const PropertyList &apl = PropertyList()); //!< open a dataset using handle and relative/absolute name
            void Create(hid_t id, const std::string &name,
                        const Datatype &dtype,
                        const Dataspace &space,
                        const PropertyList &dcpl = PropertyList(),
                        const PropertyList &dapl = PropertyList(),
                        const PropertyList &lcpl = PropertyList()); //!< create a dataset using handle and relative/absolute name
            void Write(void *buf,
                       const Datatype &memType,
                       const Dataspace &memSpace = Dataspace(),
                       const Dataspace &fileSpace = Dataspace(),
                       const PropertyList &xferpl = PropertyList()); //!< write to dataset, last dimension fastest
            void Read(void *buf,
                      const Datatype &memType,
                      const Dataspace &memSpace = Dataspace(),
                      const Dataspace &fileSpace = Dataspace(),
                      const PropertyList &xferpl = PropertyList()); //!< read buffer, last dimension fastest
            template <typename T>
            void Write(const ndarray<T> &data, const PropertyList &xferpl = PropertyList()); //!< write to dataset from ndarray, requires dim(data^T)=dim(dataset)
            template <typename T>
            void Read(ndarray<T> &data, const PropertyList &xferpl = PropertyList()); //!< read dataset to ndarray, dim(data^T)=dim(dataset)
            virtual void Close();                                                     //!< close dataset

            /// get functions
            virtual std::shared_ptr<PropertyList> GetAccessPropertyList() const;
            virtual std::shared_ptr<PropertyList> GetCreatePropertyList() const;
            /// operators
            Dataset &operator=(const Dataset &rhs) = delete; //!< copy assignment
            Dataset &operator=(Dataset &&rhs);               //!< move assignment

            ~Dataset() { Close(); } //!< destructor
        };

        template <typename T>
        std::shared_ptr<Dataset> ObjectBase::CreateDataset(const std::string &name,
                                                           const ndarray<T> &data,
                                                           const PropertyList &dcpl,
                                                           const PropertyList &dapl,
                                                           const PropertyList &lcpl,
                                                           const PropertyList &xferpl)
        {
            auto dtype = Datatype::GetNative<T>();
            ndarray<hsize_t> shape(data.GetNumDim());
            for (uint32_t i = 0; i < shape.GetLength(); i++)
                shape(i) = data.GetDim(shape.GetLength() - 1 - i);
            auto space = std::make_shared<Dataspace>(H5Screate_simple(data.GetNumDim(), shape.begin(), nullptr));
            auto ret = CreateDataset(name, *dtype, *space, dcpl, dapl, lcpl);
            ret->Write(data, xferpl);
            return ret;
        }

        template <typename T>
        void Dataset::Write(const ndarray<T> &data, const PropertyList &xferpl)
        {
            auto space = Dataspace::CreateFrom(*this);
            int ndim = H5Sget_simple_extent_ndims(space->GetHandle());
            ASSERT(uint32_t(ndim) == data.GetNumDim(), "data has to be of the same dimension as the dataset");
            ndarray<hsize_t> dims(ndim);
            ASSERT(H5Sget_simple_extent_dims(space->GetHandle(), dims.begin(), nullptr) >= 0, "Failed to get extent of the dataspace");
            for (uint32_t i = 0; i < (uint32_t)ndim; i++)
                ASSERT(dims(i) == data.GetDim(ndim - 1 - i), "data has to be of the same dimension as the dataset");
            auto dtype = Datatype::GetNative<T>();
            ASSERT(H5Dwrite(m_id, dtype->GetHandle(), H5S_ALL, H5S_ALL, xferpl.GetHandle(), data.begin()) >= 0, "Failed to write to the dataset");
        }

        template <typename T>
        void Dataset::Read(ndarray<T> &data, const PropertyList &xferpl)
        {
            auto space = Dataspace::CreateFrom(*this);
            int ndim = H5Sget_simple_extent_ndims(space->GetHandle());
            ndarray<hsize_t> dims(ndim);
            ASSERT(H5Sget_simple_extent_dims(space->GetHandle(), dims.begin(), nullptr) >= 0, "Failed to get extent of the dataspace");
            // row major to column major
            ndarray<uint32_t> dims2(ndim);
            for (uint32_t i = 0; i < (uint32_t)ndim; i++)
                dims2(ndim - 1 - i) = dims(i);
            data.Setup(dims2);
            auto dtype = Datatype::GetNative<T>();
            ASSERT(H5Dread(m_id, dtype->GetHandle(), H5S_ALL, H5S_ALL, xferpl.GetHandle(), data.begin()) >= 0, "Failed to read dateset");
        }

        // Explicit template instantiation declarations for DLL export
        extern template POST_BASE_API void Dataset::Read<char>(ndarray<char>&, const PropertyList&);
        extern template POST_BASE_API void Dataset::Read<int>(ndarray<int>&, const PropertyList&);
        extern template POST_BASE_API void Dataset::Read<double>(ndarray<double>&, const PropertyList&);
    }
}