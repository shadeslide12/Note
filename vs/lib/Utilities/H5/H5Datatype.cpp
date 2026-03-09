#include <utility>
#include "Utilities/H5/H5Datatype.h"

namespace Utilities
{
    namespace H5
    {
        Datatype::Datatype(hid_t id, const std::string &name, const PropertyList &apl)
        {
            Open(id, name, apl);
        }

        Datatype::Datatype(hid_t id)
        {
            m_id = id;
            ASSERT(H5Iis_valid(m_id) > 0, "The identifier is not valid");
            hid_t flag = H5Iget_type(m_id);
            ASSERT(flag == H5I_DATATYPE, "The identifier can't be assigned to a datatype");
        }

        Datatype::Datatype(const Datatype &rhs)
        {
            Copy(rhs.m_id);
        }

        Datatype &Datatype::operator=(const Datatype &rhs)
        {
            Copy(rhs.m_id);
            return *this;
        }

        Datatype::Datatype(Datatype &&rhs)
        {
            (*this) = std::move(rhs);
        }

        Datatype &Datatype::operator=(Datatype &&rhs)
        {
            Close();
            m_id = rhs.m_id;
            rhs.m_id = H5I_INVALID_HID;
            ASSERT(H5Iis_valid(m_id) > 0, "Failed to move the datatype");
            return *this;
        }

        std::shared_ptr<Datatype> Datatype::GetVlenString()
        {
            auto ret = std::make_shared<Datatype>();
            ret->Copy(H5T_C_S1);
            ASSERT(H5Tset_size(ret->m_id, H5T_VARIABLE) >= 0, "Failed to set datatype size");
            return ret;
        }

        void Datatype::Open(hid_t id, const std::string &name, const PropertyList &apl)
        {
            Close();
            m_id = H5Topen(id, name.c_str(), apl.GetHandle());
            ASSERT(m_id >= 0, "Failed to open datatype");
        }

        void Datatype::Create(H5T_class_t cls, size_t size)
        {
            Close();
            m_id = H5Tcreate(cls, size);
            ASSERT(m_id >= 0, "Failed to create datatype");
        }

        void Datatype::Commit(hid_t id,
                              const std::string &name,
                              const PropertyList &tcpl,
                              const PropertyList &tapl,
                              const PropertyList &lcpl)
        {
            Close();
            ASSERT(H5Tcommit(id, name.c_str(), m_id, lcpl.GetHandle(), tcpl.GetHandle(), tapl.GetHandle()) >= 0, "Failed to commit datatype");
        }

        void Datatype::Copy(hid_t id)
        {
            Close();
            m_id = H5Tcopy(id);
            ASSERT(m_id >= 0, "Failed to create datatype");
        }

        std::shared_ptr<PropertyList> Datatype::GetCreatePropertyList() const
        {
            hid_t cpl = H5Tget_create_plist(m_id);
            ASSERT(cpl != H5I_INVALID_HID, "Failed to get create propertylist");
            return std::make_shared<PropertyList>(cpl);
        }

        void Datatype::Close()
        {
            if (m_id == H5I_INVALID_HID)
                return;
            else if (H5Iis_valid(m_id) <= 0)
                m_id = H5I_INVALID_HID;
            else
            {
                ASSERT(H5Tclose(m_id) >= 0, "Failed to close datatype");
                m_id = H5I_INVALID_HID;
            }
        }

        /// frequently used predefined datatypes
        template <>
        std::shared_ptr<Datatype> Datatype::GetNative<bool>()
        {
            auto ret = std::make_shared<Datatype>();
            ret->m_id = H5T_NATIVE_HBOOL;
            return ret;
        }
        template <>
        std::shared_ptr<Datatype> Datatype::GetNative<double>()
        {
            auto ret = std::make_shared<Datatype>();
            ret->m_id = H5T_NATIVE_DOUBLE;
            return ret;
        }
        template <>
        std::shared_ptr<Datatype> Datatype::GetNative<float>()
        {
            auto ret = std::make_shared<Datatype>();
            ret->m_id = H5T_NATIVE_FLOAT;
            return ret;
        }
        template <>
        std::shared_ptr<Datatype> Datatype::GetNative<int>()
        {
            auto ret = std::make_shared<Datatype>();
            ret->m_id = H5T_NATIVE_INT;
            return ret;
        }
        template <>
        std::shared_ptr<Datatype> Datatype::GetNative<uint32_t>()
        {
            auto ret = std::make_shared<Datatype>();
            ret->m_id = H5T_NATIVE_UINT32;
            return ret;
        }
        template <>
        std::shared_ptr<Datatype> Datatype::GetNative<char>()
        {
            auto ret = std::make_shared<Datatype>();
            ret->m_id = H5T_NATIVE_CHAR;
            return ret;
        }

        // Explicit template instantiations for Windows DLL export
        template std::shared_ptr<Datatype> Datatype::GetNative<char>();
        template std::shared_ptr<Datatype> Datatype::GetNative<int>();
        template std::shared_ptr<Datatype> Datatype::GetNative<double>();
    }
}