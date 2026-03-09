#include <utility>
#include "Utilities/PrintUtils.h"
#include "Utilities/ErrorUtils.h"
#include "Utilities/H5/H5PropertyList.h"

namespace Utilities
{
    namespace H5
    {
        std::shared_ptr<PropertyList> PropertyList::Create(hid_t cls_id)
        {
            auto ret = std::make_shared<PropertyList>();
            ret->m_id = H5Pcreate(cls_id);
            ASSERT(ret->m_id != H5I_INVALID_HID, "Unrecognized class type");
            return ret;
        }

        PropertyList::PropertyList(hid_t id) : m_id(id)
        {
            ASSERT(H5Pget_class(m_id) >= 0, "The identifier is not a propertylist");
        }

        PropertyList::PropertyList(const PropertyList &rhs)
        {
            if (rhs.m_id == H5P_DEFAULT)
            {
                m_id = rhs.m_id;
            }
            else
            {
                m_id = H5Pcopy(rhs.m_id);
                ASSERT(m_id >= 0, "Failed to copy the propertylist");
            }
        }

        PropertyList::PropertyList(PropertyList &&rhs)
        {
            (*this) = std::move(rhs);
        }

        bool PropertyList::IsClass(hid_t cls_id) const
        {
            if (m_id == H5P_DEFAULT)
                return true;
            hid_t temp_cls = H5Pget_class(m_id);
            if (temp_cls < 0)
                return false;
            return H5Pequal(temp_cls, cls_id) > 0;
        }

        void PropertyList::SetChunk(const std::vector<hsize_t> &dim)
        {
            if (m_id == H5P_DEFAULT)
                m_id = H5Pcreate(H5P_DATASET_CREATE);
            ASSERT(IsClass(H5P_DATASET_CREATE), "Wrong type of property list!")
            ASSERT(H5Pset_chunk(m_id, dim.size(), dim.data()) >= 0, "Failed to set chunk!");
        }

        void PropertyList::SetCloseDegree(H5F_close_degree_t degree)
        {
            if (m_id == H5P_DEFAULT)
                m_id = H5Pcreate(H5P_FILE_ACCESS);
            ASSERT(IsClass(H5P_FILE_ACCESS), "Wrong type of property list!")
            ASSERT(H5Pset_fclose_degree(m_id, degree) >= 0, "Failed to set file close degree");
        }

        void PropertyList::SetDriver_core(bool writeOnClose, size_t bytesPerMalloc)
        {
            if (m_id == H5P_DEFAULT)
                m_id = H5Pcreate(H5P_FILE_ACCESS);
            ASSERT(IsClass(H5P_FILE_ACCESS), "Wrong type of property list!")
            ASSERT(H5Pset_fapl_core(m_id, bytesPerMalloc, static_cast<hbool_t>(writeOnClose)) >= 0, "Failed to set file driver to core");
        }

        void PropertyList::SetDriver_sec2()
        {
            if (m_id == H5P_DEFAULT)
                m_id = H5Pcreate(H5P_FILE_ACCESS);
            ASSERT(IsClass(H5P_FILE_ACCESS), "Wrong type of property list!")
            ASSERT(H5Pset_fapl_sec2(m_id) >= 0, "Failed to set file driver to SEC2");
        }

        void PropertyList::SetDriver_stdio()
        {
            if (m_id == H5P_DEFAULT)
                m_id = H5Pcreate(H5P_FILE_ACCESS);
            ASSERT(IsClass(H5P_FILE_ACCESS), "Wrong type of property list!")
            ASSERT(H5Pset_fapl_stdio(m_id) >= 0, "Failed to set file driver to stdio");
        }

#ifdef H5_HAVE_PARALLEL
        void PropertyList::SetDriver_mpio(MPI_Comm comm, MPI_Info info)
        {
            if (m_id == H5P_DEFAULT)
                m_id = H5Pcreate(H5P_FILE_ACCESS);
            ASSERT(IsClass(H5P_FILE_ACCESS), "Wrong type of property list!")
            ASSERT(H5Pset_fapl_mpio(m_id, comm, info) >= 0, "Failed to set file driver to mpio");
        }

        void PropertyList::SetDataTransfer_mpio(H5FD_mpio_xfer_t mode)
        {
            if (m_id == H5P_DEFAULT)
                m_id = H5Pcreate(H5P_DATASET_XFER);
            ASSERT(IsClass(H5P_DATASET_XFER), "Wrong type of property list!")
            ASSERT(H5Pset_dxpl_mpio(m_id, mode) >= 0, "Failed to set mpio data transfer mode");
        }

#endif
        void PropertyList::SetDeflate(uint32_t level)
        {
            if (m_id == H5P_DEFAULT)
                m_id = H5Pcreate(H5P_DATASET_CREATE);
            ASSERT(IsClass(H5P_DATASET_CREATE), "Wrong type of property list!")
            if (level > 9)
            {
                level = 9;
                PRINT("WARNING: compression level set to 9");
            }
            ASSERT(H5Pset_deflate(m_id, level) >= 0, "Failed to set compression level");
        }

        PropertyList &PropertyList::operator=(const PropertyList &rhs)
        {
            // close present propertylist and copy the rhs
            if (&rhs != this)
            {
                Close();
                if (rhs.m_id == H5P_DEFAULT)
                {
                    m_id = rhs.m_id;
                }
                else
                {
                    m_id = H5Pcopy(rhs.m_id);
                    ASSERT(m_id >= 0, "Failed to copy propertylist");
                }
            }
            return *this;
        }

        PropertyList &PropertyList::operator=(PropertyList &&rhs)
        {
            // close present propertylist and copy the rhs
            Close();
            m_id = rhs.m_id;
            rhs.m_id = H5I_INVALID_HID;
            if (m_id != H5P_DEFAULT)
                ASSERT(H5Iis_valid(m_id) > 0, "Failed to move propertylist");
            return *this;
        }

        void PropertyList::Close()
        {
            if (m_id == H5I_INVALID_HID)
                return;
            else if (m_id == H5P_DEFAULT || H5Iis_valid(m_id) <= 0)
                m_id = H5I_INVALID_HID;
            else
            {
                ASSERT(H5Pclose(m_id) >= 0, "Failed to close propertylist");
                m_id = H5I_INVALID_HID;
            }
        }
    }
}