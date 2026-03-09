#include <utility>
#include "Utilities/H5/H5Group.h"
namespace Utilities
{
    namespace H5
    {
        Group::Group(hid_t id,
                     const std::string &name,
                     const PropertyList &apl)
        {
            Open(id, name, apl);
        }

        Group::Group(hid_t id)
        {
            m_id = id;
            ASSERT(H5Iis_valid(m_id) > 0, "The identifier is not valid");
            hid_t flag = H5Iget_type(m_id);
            ASSERT(flag == H5I_GROUP, "The identifier can't be assigned to a group");
        }

        Group::Group(Group &&rhs)
        {
            (*this) = std::move(rhs);
        }

        void Group::Open(hid_t id,
                         const std::string &name,
                         const PropertyList &apl)
        {
            Close();
            m_id = H5Gopen(id, name.c_str(), apl.GetHandle());
            ASSERT(m_id >= 0, ("Failed to Open group: " + name).c_str());
        }

        void Group::Create(hid_t id,
                           const std::string &name,
                           const PropertyList &gcpl,
                           const PropertyList &gapl,
                           const PropertyList &lcpl)
        {
            Close();
            m_id = H5Gcreate(id, name.c_str(), lcpl.GetHandle(), gcpl.GetHandle(), gapl.GetHandle());
            ASSERT(m_id >= 0, "Failed to create group");
        }

        Group &Group::operator=(Group &&rhs)
        {
            Close();
            m_id = rhs.m_id;
            rhs.m_id = H5I_INVALID_HID;
            ASSERT(H5Iis_valid(m_id) > 0, "Failed to move group");
            return *this;
        }

        std::shared_ptr<PropertyList> Group::GetCreatePropertyList() const
        {
            hid_t cpl = H5Gget_create_plist(m_id);
            ASSERT(cpl != H5I_INVALID_HID, "Failed to get create propertylist");
            return std::make_shared<PropertyList>(cpl);
        }

        void Group::Close()
        {
            if (m_id == H5I_INVALID_HID)
                return;
            else if (H5Iis_valid(m_id) <= 0)
                m_id = H5I_INVALID_HID;
            else
            {
                ASSERT(H5Gclose(m_id) >= 0, "Failed to close group");
                m_id = H5I_INVALID_HID;
            }
        }
    }
}
