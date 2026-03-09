#pragma once
#include <vector>
#include <memory>
#include "hdf5.h"
#include "post_base_export.h"

namespace Utilities
{
    namespace H5
    {
        class POST_BASE_API PropertyList
        {
        private:
            hid_t m_id = H5P_DEFAULT;
        public:
            /// constructors
            PropertyList() = default;              //!< create default propertylist of any kind
            PropertyList(hid_t id);                //!< converting constructor
            PropertyList(const PropertyList &rhs); //!< copy constructor
            PropertyList(PropertyList &&rhs);      //!< move constructor

            /// creator functions
            static std::shared_ptr<PropertyList> Create(hid_t cls_id); //!< create a default propertylist of cls_id type
            
            /// operations
            void Close(); //!< release memory

            /// get functions
            bool IsClass(hid_t cls_id) const;        //! if the present propertylist is of cls_id class
            hid_t GetHandle() const { return m_id; } //!< get the handle of the property list

            // set functions
            void SetChunk(const std::vector<hsize_t> &dims);     //!< set the chunk size of the dataset
            void SetCloseDegree(H5F_close_degree_t degree);      //!< set the close level of a file
            void SetDriver_core(bool writeOnClose = false,       /*!< Optionally save to storage when the file is closed. */
                                size_t bytesPerMalloc = 10240000 /*!< Size, in bytes, of memory increments. */
            );                                                   //!< set the file drive to core
            void SetDriver_sec2();                               //!< set the file drive to sec2
            void SetDriver_stdio();                              //!< set the file drive to stdio
#ifdef H5_HAVE_PARALLEL
            void SetDriver_mpio(MPI_Comm comm = MPI_COMM_WORLD, MPI_Info info = MPI_INFO_NULL); //!< set the file drive to mpi io
            void SetDataTransfer_mpio(H5FD_mpio_xfer_t mode = H5FD_MPIO_COLLECTIVE);            //!< set the mpio data transfer mode
#endif
            void SetDeflate(uint32_t level = 0); //!< set the compression level of a dataset

            /// operators
            PropertyList &operator=(const PropertyList &rhs); //!< copy assignment
            PropertyList &operator=(PropertyList &&rhs);      //!< move assignment

            ~PropertyList() { Close(); }//!< destructor
        };
    } // namespace H5
} // namespace Utilities
