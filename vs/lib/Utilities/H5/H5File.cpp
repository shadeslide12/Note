#include <utility>
#include <sys/types.h>
#include "Utilities/H5/H5File.h"
#include "Utilities/FileUtils.h"
#include <cstdio>

namespace Utilities
{
    namespace H5
    {
        File::File(const std::string &name,
                   FileAccessFlag flag,
                   const PropertyList &apl,
                   const PropertyList &cpl)
        {
            bool exist = Utilities::File::PathExists(name);
            switch (flag)
            {
            case FileAccessFlag::READONLY:
            case FileAccessFlag::READWRITE:
                ASSERT(exist, "Failed to open file, check access flag");
                Open(name, flag, apl);
                break;
            case FileAccessFlag::EXCLUSIVE:
                ASSERT(!exist, "Failed to open file, check access flag");
            case FileAccessFlag::OVERWRITE:
                Create(name, flag, apl, cpl);
                break;
            case FileAccessFlag::APPEND:
                if (exist)
                    Open(name, flag, apl);
                else
                    Create(name, flag, apl, cpl);
                break;
            default:
                FATAL_ERROR("Unknown access flag");
                break;
            }
        }

        File::File(hid_t id)
        {
            m_id = id;
            ASSERT(H5Iis_valid(m_id) > 0, "The identifier is not valid");
            hid_t flag = H5Iget_type(m_id);
            ASSERT(flag == H5I_FILE, "The identifier can't be assigned to a file");
        }

        File::File(File &&rhs)
        {
            (*this) = std::move(rhs);
        }

        void File::Open(const std::string &name,
                        FileAccessFlag flag,
                        const PropertyList &apl)
        {
            Close();
            uint32_t real_flag;
            if (flag == FileAccessFlag::READWRITE || flag == FileAccessFlag::APPEND)
                real_flag = H5F_ACC_RDWR;
            else if (flag == FileAccessFlag::READONLY)
                real_flag = H5F_ACC_RDONLY;
            else
                FATAL_ERROR("Unsupported access flag for file opening");
            m_id = H5Fopen(name.c_str(), real_flag, apl.GetHandle());
            ASSERT(m_id >= 0, "Failed to open file");
        }

        void File::Create(const std::string &name,
                          FileAccessFlag flag,
                          const PropertyList &apl,
                          const PropertyList &cpl)
        {
            Close();
            uint32_t real_flag;
            if (flag == FileAccessFlag::EXCLUSIVE || flag == FileAccessFlag::APPEND)
                real_flag = H5F_ACC_EXCL;
            else if (flag == FileAccessFlag::OVERWRITE)
                real_flag = H5F_ACC_TRUNC;
            else
                FATAL_ERROR("Unsupported access flag for file creation");
            m_id = H5Fcreate(name.c_str(), real_flag, cpl.GetHandle(), apl.GetHandle());
            ASSERT(m_id >= 0, "Failed to create file");
        }

        void File::Delete()
        {
            // get file name
            std::string fname = GetName();
            // get access propertylist
            auto apl = GetAccessPropertyList();
            // close file
            Close();
            // delete file
#if H5_VERSION_GE(1, 12, 0)
            ASSERT(H5Fdelete(fname.c_str(), apl->GetHandle()) >= 0, "Failed to delete the file");
#else
            ASSERT(std::remove(fname.c_str()) == 0, "Failed to delete the file");
#endif
        }

        std::string File::GetName() const
        {
            ndarray<char> name;
            size_t size = H5Fget_name(m_id, nullptr, 0) + 1;
            ASSERT(size > 1, "Failed to get the filename");
            name.Setup(size);
            H5Fget_name(m_id, name.begin(), size);
            return std::string(name.begin());
        }

        std::shared_ptr<PropertyList> File::GetAccessPropertyList() const
        {
            hid_t apl = H5Fget_access_plist(m_id);
            ASSERT(apl != H5I_INVALID_HID, "Failed to get access propertylist");
            return std::make_shared<PropertyList>(apl);
        }

        std::shared_ptr<PropertyList> File::GetCreatePropertyList() const
        {
            hid_t cpl = H5Fget_create_plist(m_id);
            ASSERT(cpl != H5I_INVALID_HID, "Failed to get create propertylist");
            return std::make_shared<PropertyList>(cpl);
        }

        File &File::operator=(File &&rhs)
        {
            Close();
            m_id = rhs.m_id;
            rhs.m_id = H5I_INVALID_HID;
            ASSERT(H5Iis_valid(m_id) > 0, "Failed to move file");
            return *this;
        }

        void File::Close()
        {
            if (m_id == H5I_INVALID_HID)
                return;
            else if (H5Iis_valid(m_id) <= 0)
                m_id = H5I_INVALID_HID;
            else
            {
                ASSERT(H5Fclose(m_id) >= 0, "Failed to close file");
                m_id = H5I_INVALID_HID;
            }
        }
    } // namespace H5
} // namespace Utilities
