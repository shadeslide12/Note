#include <utility>
#include "Utilities/H5/H5Attribute.h"
#include "Utilities/ErrorUtils.h"

namespace Utilities
{
    namespace H5
    {
        Attribute::Attribute(hid_t id) : m_id(id)
        {
            ASSERT(H5Iis_valid(m_id) > 0, "The identifier is not valid");
            hid_t flag = H5Iget_type(m_id);
            ASSERT(flag == H5I_ATTR, "The identifier can't be assigned to a attribute");
        }

        Attribute::Attribute(hid_t id, const std::string &name,
                             const PropertyList &apl)
        {
            Open(id, name, apl);
        }

        Attribute::Attribute(Attribute &&rhs)
        {
            (*this) = std::move(rhs);
        }

        void Attribute::Open(hid_t id, const std::string &name,
                             const PropertyList &apl)
        {
            Close();
            m_id = H5Aopen(id, name.c_str(), apl.GetHandle());
            ASSERT(m_id >= 0, ("Failed to Open attribute: " + name).c_str());
        }

        void Attribute::Create(hid_t id, const std::string &name,
                               const Datatype &dtype,
                               const Dataspace &space,
                               const PropertyList &acpl,
                               const PropertyList &aapl)
        {
            Close();
            m_id = H5Acreate(id, name.c_str(), dtype.GetHandle(), space.GetHandle(), acpl.GetHandle(), aapl.GetHandle());
            ASSERT(m_id >= 0, ("Failed to create attribute: " + name).c_str());
        }

        void Attribute::Close()
        {
            if (m_id == H5I_INVALID_HID)
                return;
            else if (H5Iis_valid(m_id) <= 0)
                m_id = H5I_INVALID_HID;
            else
            {
                ASSERT(H5Aclose(m_id) >= 0, "Failed to close attribute");
                m_id = H5I_INVALID_HID;
            }
        }

        std::string Attribute::GetName() const
        {
            ndarray<char> name;
            size_t size = H5Aget_name(m_id, 0, nullptr) + 1;
            ASSERT(size > 1, "Failed to get the filename");
            name.Setup(size);
            H5Aget_name(m_id, size, name.begin());
            return std::string(name.begin());
        }

        std::shared_ptr<PropertyList> Attribute::GetCreatePropertyList() const
        {
            hid_t cpl = H5Aget_create_plist(m_id);
            ASSERT(cpl != H5I_INVALID_HID, "Failed to get create propertylist");
            return std::make_shared<PropertyList>(cpl);
        }

        Attribute &Attribute::operator=(Attribute &&rhs)
        {
            Close();
            m_id = rhs.m_id;
            rhs.m_id = H5I_INVALID_HID;
            ASSERT(H5Iis_valid(m_id) > 0, "Failed to move attribute");
            return *this;
        }

        void Attribute::Write(const Datatype &memType, void *buf)
        {
            ASSERT(H5Awrite(m_id, memType.GetHandle(), buf) >= 0, "Failed to write attribute");
        }

        void Attribute::Read(const Datatype &memType, void *buf)
        {
            ASSERT(H5Aread(m_id, memType.GetHandle(), buf) >= 0, "Failed to write attribute");
        }
    }
}