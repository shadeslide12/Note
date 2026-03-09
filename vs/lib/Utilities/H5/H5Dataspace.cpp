#include <utility>
#include "Utilities/H5/H5Dataspace.h"
#include "Utilities/H5/H5Attribute.h"
#include "Utilities/H5/H5Dataset.h"
#include "Utilities/ErrorUtils.h"
namespace Utilities
{
    namespace H5
    {
        Dataspace::Dataspace(hid_t id) : m_id(id)
        {
            ASSERT(H5Iis_valid(m_id) > 0, "The identifier is not valid");
            hid_t flag = H5Iget_type(m_id);
            ASSERT(flag == H5I_DATASPACE, "The identifier can't be assigned to a dataspace");
        }

        Dataspace::Dataspace(const Dataspace &rhs)
        {
            (*this) = rhs;
        }

        Dataspace &Dataspace::operator=(const Dataspace &rhs)
        {
            Close();
            m_id = H5Scopy(rhs.m_id);
            ASSERT(m_id >= 0, "Failed to create dataspace");
            return (*this);
        }

        Dataspace::Dataspace(Dataspace &&rhs)
        {
            (*this) = std::move(rhs);
        }

        std::shared_ptr<Dataspace> Dataspace::CreateSimple(std::initializer_list<hsize_t> dim, std::initializer_list<hsize_t> max_dim)
        {
            if (max_dim.size() != 0)
                ASSERT(dim.size() == max_dim.size(), "Different size between dim and max_dim");
            auto ret = std::make_shared<Dataspace>();
            ret->m_id = H5Screate_simple(dim.size(), dim.begin(), max_dim.size() == 0 ? nullptr : max_dim.begin());
            ASSERT(ret->m_id >= 0, "Failed to create dataspace");
            return ret;
        }

        std::shared_ptr<Dataspace> Dataspace::CreateScalar()
        {
            auto ret = std::make_shared<Dataspace>();
            ret->m_id = H5Screate(H5S_SCALAR);
            ASSERT(ret->m_id >= 0, "Failed to create dataspace");
            return ret;
        }

        Dataspace &Dataspace::operator=(Dataspace &&rhs)
        {
            Close();
            m_id = rhs.m_id;
            rhs.m_id = H5I_INVALID_HID;
            ASSERT(H5Iis_valid(m_id) > 0, "Failed to move the dataspace");
            return *this;
        }

        void Dataspace::SelectNone()
        {
            ASSERT(H5Sselect_none(m_id) >= 0, "Failed to reset selection");
        }

        void Dataspace::SelectAll()
        {
            ASSERT(H5Sselect_all(m_id) >= 0, "Failed to select all");
        }

        void Dataspace::SelectHyperslab(H5S_seloper_t op,
                                        const hsize_t *start,
                                        const hsize_t *count,
                                        const hsize_t *stride,
                                        const hsize_t *block)
        {
            ASSERT(H5Sselect_hyperslab(m_id, op, start, stride, count, block) >= 0, "Failed to select hyperslab");
        }

        void Dataspace::SelectHyperslab(H5S_seloper_t op,
                                        std::initializer_list<hsize_t> start,
                                        std::initializer_list<hsize_t> count,
                                        std::initializer_list<hsize_t> stride,
                                        std::initializer_list<hsize_t> block)
        {
            ASSERT(H5Sselect_hyperslab(m_id, op, start.begin(), stride.begin(), count.size() == 0 ? nullptr : count.begin(), block.size() == 0 ? nullptr : block.begin()) >= 0, "Failed to select hyperslab");
        }

        void Dataspace::Close()
        {
            if (m_id == H5I_INVALID_HID)
                return;
            else if (H5Iis_valid(m_id) <= 0)
                m_id = H5I_INVALID_HID;
            else
            {
                ASSERT(H5Sclose(m_id) >= 0, "Failed to close dataspace");
                m_id = H5I_INVALID_HID;
            }
        }

        // template function specification
        template <>
        std::shared_ptr<Dataspace> Dataspace::CreateFrom(Attribute &rhs)
        {
            auto ret = std::make_shared<Dataspace>();
            ret->m_id = H5Aget_space(rhs.GetHandle());
            ASSERT(ret->m_id >= 0, "Failed to get dataspace");
            return ret;
        }

        template <>
        std::shared_ptr<Dataspace> Dataspace::CreateFrom(Dataset &rhs)
        {
            auto ret = std::make_shared<Dataspace>();
            ret->m_id = H5Dget_space(rhs.GetHandle());
            ASSERT(ret->m_id >= 0, "Failed to get dataspace");
            return ret;
        }

        // Explicit template instantiation for Windows DLL export
        template std::shared_ptr<Dataspace> Dataspace::CreateFrom<Dataset>(Dataset&);
    } // namespace H5

} // namespace Utilities
