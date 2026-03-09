#include <utility>
#include "Utilities/H5/H5ObjectBase.h"
#include "Utilities/H5/H5Group.h"
#include "Utilities/H5/H5Dataset.h"
#include "Utilities/H5/H5Attribute.h"

namespace Utilities
{
    namespace H5
    {
        ObjectBase::ObjectBase(hid_t id, const std::string &name, const PropertyList &apl)
        {
            Open(id, name, apl);
        }

        ObjectBase::ObjectBase(ObjectBase &&rhs)
        {
            (*this) = std::move(rhs);
        }

        ObjectBase::ObjectBase(hid_t id) : m_id(id)
        {
            ASSERT(H5Iis_valid(m_id) > 0, "The identifier is not valid");
            hid_t flag = H5Iget_type(m_id);
            if (flag != H5I_FILE && flag != H5I_GROUP && flag != H5I_DATASET && flag != H5I_DATATYPE)
                FATAL_ERROR("The identifier can't be assigned to an object");
        }

        void ObjectBase::Open(hid_t id, const std::string &name, const PropertyList &apl)
        {
            Close();
            m_id = H5Oopen(id, name.c_str(), apl.GetHandle());
            ASSERT(m_id >= 0, "Failed to open object");
        }

        void ObjectBase::Close()
        {
            if (m_id == H5I_INVALID_HID)
                return;
            else if (H5Iis_valid(m_id) <= 0)
                m_id = H5I_INVALID_HID;
            else
            {
                ASSERT(H5Oclose(m_id) >= 0, "Failed to close object");
                m_id = H5I_INVALID_HID;
            }
        }

        std::string ObjectBase::GetName() const
        {
            ndarray<char> name;
            size_t size = H5Iget_name(m_id, nullptr, 0) + 1;
            ASSERT(size > 1, "Name not found!");
            name.Setup(size);
            H5Iget_name(m_id, name.begin(), size);
            return std::string(name.begin());
        }

        std::string ObjectBase::GetBaseName() const
        {
            std::string name = GetName();
            size_t pos = name.find_last_of("/\\");
            if (pos == std::string::npos)
                return name;
            else
                return name.substr(pos + 1);
        }

        std::shared_ptr<Group> ObjectBase::CreateGroup(const std::string &name,
                                                       const PropertyList &gcpl,
                                                       const PropertyList &gapl,
                                                       const PropertyList &lcpl)
        {
            auto ret = std::make_shared<Group>();
            ret->Create(m_id, name, gcpl, gapl, lcpl);
            return ret;
        }

        std::shared_ptr<Group> ObjectBase::GetGroup(const std::string &name,
                                                    const PropertyList &gapl) const
        {
            return std::make_shared<Group>(m_id, name, gapl);
        }

        std::shared_ptr<Dataset> ObjectBase::CreateDataset(const std::string &name,
                                                           const Datatype &dtype,
                                                           const Dataspace &space,
                                                           const PropertyList &dcpl,
                                                           const PropertyList &dapl,
                                                           const PropertyList &lcpl)
        {
            auto ret = std::make_shared<Dataset>();
            ret->Create(m_id, name, dtype, space, dcpl, dapl, lcpl);
            return ret;
        }

        std::shared_ptr<Dataset> ObjectBase::GetDataset(const std::string &name,
                                                        const PropertyList &dapl) const
        {
            return std::make_shared<Dataset>(m_id, name, dapl);
        }

        std::shared_ptr<Attribute> ObjectBase::CreateAttribute(const std::string &name,
                                                               const Datatype &dtype,
                                                               const Dataspace &space,
                                                               const PropertyList &acpl,
                                                               const PropertyList &aapl)
        {
            auto ret = std::make_shared<Attribute>();
            ret->Create(m_id, name, dtype, space, acpl, aapl);
            return ret;
        }

        void ObjectBase::DeleteAttribute(const std::string &name)
        {
            ASSERT(H5Adelete(m_id, name.c_str()) >= 0, "Failed to delete attribute");
        }

        std::shared_ptr<Attribute> ObjectBase::GetAttribute(const std::string &name,
                                                            const PropertyList &aapl) const
        {
            return std::make_shared<Attribute>(m_id, name, aapl);
        }

        ObjectBase &ObjectBase::operator=(ObjectBase &&rhs)
        {
            Close();
            m_id = rhs.m_id;
            rhs.m_id = H5I_INVALID_HID;
            ASSERT(H5Iis_valid(m_id) > 0, "Failed to move object");
            return *this;
        }

        H5I_type_t ObjectBase::GetType() const
        {
            ASSERT(H5Iis_valid(m_id) > 0, "The identifier is not valid");
            H5I_type_t ret = H5Iget_type(m_id);
            ASSERT(ret != H5I_BADID, "Invalid object type");
            return ret;
        }

        bool ObjectBase::Exists(const std::string &name) const
        {
            if (name == "/" || name == "")
                return true; // root return true

            size_t pos = name.find_last_of("/");

            if (pos == std::string::npos) // is base name, test current
            {
                if (H5Lexists(m_id, name.c_str(), H5P_DEFAULT) <= 0)
                    return false;
                if (H5Oexists_by_name(m_id, name.c_str(), H5P_DEFAULT) <= 0)
                    return false;
            }
            else // test the parent before test the current
            {
                if (Exists(name.substr(0, pos)))
                {
                    if (H5Lexists(m_id, name.c_str(), H5P_DEFAULT) <= 0)
                        return false;
                    if (H5Oexists_by_name(m_id, name.c_str(), H5P_DEFAULT) <= 0)
                        return false;
                }
                else
                    return false;
            }
            return true;
        }
    }
}