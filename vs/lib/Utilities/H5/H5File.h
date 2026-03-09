#pragma once
#include "Utilities/H5/H5ObjectBase.h"

/**
 * @brief The wrapper should include basic functionality including:
 * 1. file create/read/write
 * 2. file check
 * 3. dataset create/read/write
 * 4. dataspace hyperslab slections
 * 5. file/dataset access property list, including MPIO
 * 6. Attribute create/read/write
 * 7. group create/read/write
 * 8. dataset compression and chunk
 * 9. wide datatype support
 */
namespace Utilities
{
    namespace H5
    {
        enum FileAccessFlag
        {
            EXCLUSIVE, // create new, fail if aready exists
            OVERWRITE, // create new, overwrite if already exists
            READONLY,  // open exisiting file, read only
            READWRITE, // open exisiting file, read and write
            APPEND     // open file, create if doesn't exist
        };

        class POST_BASE_API File : public ObjectBase
        {
        public:
            /// constructors
            File() = default; //!< empty file
            File(hid_t id);   //!< converting constructor
            File(const std::string &name,
                 FileAccessFlag flag,
                 const PropertyList &apl = PropertyList(),
                 const PropertyList &cpl = PropertyList()); //!< create or open a file
            File(const File &rhs) = delete;                 //!< copy constructor
            File(File &&rhs);                               //!< move constructor

            /// operations
            virtual void Open(hid_t id, const std::string &name, const PropertyList &apl = PropertyList()){FATAL_ERROR("File doesn't support open using handle and name")};
            void Open(const std::string &name,
                      FileAccessFlag flag,
                      const PropertyList &apl = PropertyList()); //!< open a file
            void Create(const std::string &name,
                        FileAccessFlag flag,
                        const PropertyList &apl = PropertyList(),
                        const PropertyList &cpl = PropertyList()); //!< create a file
            virtual std::string GetName() const;                   //!< get the full name
            virtual std::shared_ptr<PropertyList> GetAccessPropertyList() const;
            virtual std::shared_ptr<PropertyList> GetCreatePropertyList() const;
            void Delete();        //!< delete a file
            virtual void Close(); //!< close file

            /// operators
            File &operator=(const File &rhs) = delete; //!< copy assignment
            File &operator=(File &&rhs);               //!< move assignment

            ~File() { Close(); } //!< destructor
        };
    } // namespace H5

} // namespace Utilities
