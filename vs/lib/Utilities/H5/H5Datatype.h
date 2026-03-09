#pragma once
#include "Utilities/H5/H5ObjectBase.h"

namespace Utilities
{
    namespace H5
    {
        class POST_BASE_API Datatype : public ObjectBase
        {
        public:
            /// constructors
            Datatype() = default;                                                                  //!< empty datatype
            Datatype(hid_t id);                                                                    //!< converting constructor
            Datatype(const Datatype &rhs);                                                         //!< copy constructor
            Datatype(Datatype &&rhs);                                                              //!< move constructor
            Datatype(hid_t id, const std::string &name, const PropertyList &apl = PropertyList()); //!< open datatype using handle and abs/ref name

            /// operations
            virtual void Open(hid_t id, const std::string &name, const PropertyList &apl = PropertyList()); //!< open datatype using handle and abs/ref name
            void Create(H5T_class_t cls, size_t size);                                                      //!< create datatype from class and size
            void Commit(hid_t id,
                        const std::string &name,
                        const PropertyList &tcpl = PropertyList(),
                        const PropertyList &tapl = PropertyList(),
                        const PropertyList &lcpl = PropertyList()); //!< commit datatype using handle and abs/ref name
            void Copy(hid_t id);                                    //!< copy datatype from handle or predefined
            virtual void Close();                                   //!< close datatype

            /// creator functions
            template <typename T>
            static std::shared_ptr<Datatype> GetNative();     //!< initialize with a native datatype
            static std::shared_ptr<Datatype> GetVlenString(); //!< initialize to a variable length string

            /// get functions
            virtual std::shared_ptr<PropertyList> GetCreatePropertyList() const;

            /// operators
            Datatype &operator=(const Datatype &rhs); //!< copy assignment
            Datatype &operator=(Datatype &&rhs);      //!< move assignment

            ~Datatype() { Close(); } //!< destructor
        };

        // Explicit template instantiation declarations for DLL export
        extern template POST_BASE_API std::shared_ptr<Datatype> Datatype::GetNative<char>();
        extern template POST_BASE_API std::shared_ptr<Datatype> Datatype::GetNative<int>();
        extern template POST_BASE_API std::shared_ptr<Datatype> Datatype::GetNative<double>();
    }
}