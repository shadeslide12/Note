#include <utility>
#include "Utilities/H5/H5Dataset.h"
namespace Utilities
{
    namespace H5
    {

        Dataset::Dataset(hid_t id)
        {
            m_id = id;
            ASSERT(H5Iis_valid(m_id) > 0, "The identifier is not valid");
            hid_t flag = H5Iget_type(m_id);
            ASSERT(flag == H5I_DATASET, "The identifier can't be assigned to a dataset");
        }

        Dataset::Dataset(hid_t id,
                         const std::string &name,
                         const PropertyList &apl)
        {
            Open(id, name, apl);
        }

        Dataset::Dataset(Dataset &&rhs)
        {
            (*this) = std::move(rhs);
        }

        void Dataset::Open(hid_t id,
                           const std::string &name,
                           const PropertyList &apl)
        {
            Close();
            m_id = H5Dopen(id, name.c_str(), apl.GetHandle());
            ASSERT(m_id >= 0, ("Failed to Open dataset: " + name).c_str());
        }

        void Dataset::Create(hid_t id, const std::string &name,
                             const Datatype &dtype,
                             const Dataspace &space,
                             const PropertyList &dcpl,
                             const PropertyList &dapl,
                             const PropertyList &lcpl)
        {
            Close();
            m_id = H5Dcreate(id, name.c_str(), dtype.GetHandle(), space.GetHandle(), lcpl.GetHandle(), dcpl.GetHandle(), dapl.GetHandle());
            ASSERT(m_id >= 0, ("Failed to create dataset: " + name).c_str());
        }

        void Dataset::Write(void *buf,
                            const Datatype &memType,
                            const Dataspace &memSpace,
                            const Dataspace &fileSpace,
                            const PropertyList &xferpl)
        {
            ASSERT(H5Dwrite(m_id, memType.GetHandle(), memSpace.GetHandle(), fileSpace.GetHandle(), xferpl.GetHandle(), buf) >= 0, "Failed to write dataset");
        }

        void Dataset::Read(void *buf,
                           const Datatype &memType,
                           const Dataspace &memSpace,
                           const Dataspace &fileSpace,
                           const PropertyList &xferpl)
        {
            ASSERT(H5Dread(m_id, memType.GetHandle(), memSpace.GetHandle(), fileSpace.GetHandle(), xferpl.GetHandle(), buf) >= 0, "Failed to read dataset");
        }

        std::shared_ptr<PropertyList> Dataset::GetAccessPropertyList() const
        {
            hid_t apl = H5Dget_access_plist(m_id);
            ASSERT(apl != H5I_INVALID_HID, "Failed to get access propertylist");
            return std::make_shared<PropertyList>(apl);
        }

        std::shared_ptr<PropertyList> Dataset::GetCreatePropertyList() const
        {
            hid_t cpl = H5Dget_create_plist(m_id);
            ASSERT(cpl != H5I_INVALID_HID, "Failed to get create propertylist");
            return std::make_shared<PropertyList>(cpl);
        }

        void Dataset::Close()
        {
            if (m_id == H5I_INVALID_HID)
                return;
            else if (H5Iis_valid(m_id) <= 0)
                m_id = H5I_INVALID_HID;
            else
            {
                ASSERT(H5Dclose(m_id) >= 0, "Failed to close dataset");
                m_id = H5I_INVALID_HID;
            }
        }

        Dataset &Dataset::operator=(Dataset &&rhs)
        {
            Close();
            m_id = rhs.m_id;
            rhs.m_id = H5I_INVALID_HID;
            ASSERT(H5Iis_valid(m_id) > 0, "Failed to move dataset");
            return *this;
        }

        // Explicit template instantiations for Windows DLL export
        template void Dataset::Read<char>(ndarray<char>&, const PropertyList&);
        template void Dataset::Read<int>(ndarray<int>&, const PropertyList&);
        template void Dataset::Read<double>(ndarray<double>&, const PropertyList&);
    } // namespace H5

} // namespace Utilities
