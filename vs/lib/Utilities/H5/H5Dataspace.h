#pragma once
#include <initializer_list>
#include <memory>
#include "hdf5.h"
#include "Utilities/ErrorUtils.h"
#include "post_base_export.h"

namespace Utilities
{
    namespace H5
    {
        class POST_BASE_API Dataspace
        {
        protected:
            hid_t m_id = H5S_ALL;

        public:
            /// constructors
            Dataspace() = default;           //!< default H5S_ALL
            Dataspace(const Dataspace &rhs); //!< copy constructor
            Dataspace(Dataspace &&rhs);      //!< move constructor
            Dataspace(hid_t id);             //!< converting constructor

            /// operations
            void SelectNone(); //!< clear selection in dataspace
            void SelectAll();  //!< select all elements in dataspace
            void SelectHyperslab(H5S_seloper_t op,
                                 const hsize_t *start,
                                 const hsize_t *count,
                                 const hsize_t *stride = nullptr,
                                 const hsize_t *block = nullptr); //!< select hyperslab
            void SelectHyperslab(H5S_seloper_t op,
                                 std::initializer_list<hsize_t> start,
                                 std::initializer_list<hsize_t> count,
                                 std::initializer_list<hsize_t> stride = std::initializer_list<hsize_t>(),
                                 std::initializer_list<hsize_t> block = std::initializer_list<hsize_t>());
            void Close(); //!< close dataspace

            /// creator functions
            template <typename T>
            static std::shared_ptr<Dataspace> CreateFrom(T &rhs); //!< create dataspace from object
            static std::shared_ptr<Dataspace> CreateSimple(std::initializer_list<hsize_t> dim,
                                                           std::initializer_list<hsize_t> max_dim = std::initializer_list<hsize_t>()); //!< create simple dataspace
            static std::shared_ptr<Dataspace> CreateScalar();                                                                          //!< create scalar dataspace
            static std::shared_ptr<Dataspace> CreateNull();                                                                            //!< create null dataspace

            /// get functions
            hid_t GetHandle() const { return m_id; } //!< get handle

            /// operators
            Dataspace &operator=(const Dataspace &rhs); //!< copy assignment
            Dataspace &operator=(Dataspace &&rhs);      //!< move assignment

            ~Dataspace() { Close(); } //!< destructor
        };

        // Forward declaration for explicit template instantiation
        class Dataset;
        
        // Explicit template instantiation declaration for DLL export
        extern template POST_BASE_API std::shared_ptr<Dataspace> Dataspace::CreateFrom<Dataset>(Dataset&);
    }
}