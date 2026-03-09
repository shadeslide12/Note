#pragma once
#include "Utilities/H5/H5ObjectBase.h"

namespace Utilities
{
    namespace H5
    {
        class POST_BASE_API Group : public ObjectBase
        {
        public:
            /// constructors
            Group() = default;                //!< empty group
            Group(hid_t id);                  //!< converting constructor
            Group(const Group &rhs) = delete; //!< copy constructor
            Group(Group &&rhs);               //!< move constructor
            Group(hid_t id,
                  const std::string &name,
                  const PropertyList &apl = PropertyList()); //!< open a group using handle and abs/rel name

            /// operations
            virtual void Open(hid_t id,
                              const std::string &name,
                              const PropertyList &apl = PropertyList()); //!< open a group using handle and abs/rel name
            void Create(hid_t id,
                        const std::string &name,
                        const PropertyList &gcpl = PropertyList(),
                        const PropertyList &gapl = PropertyList(),
                        const PropertyList &lcpl = PropertyList()); //!< create a group using handle and abs/rel name
            virtual void Close(); //!< close a group

            /// get functions
            virtual std::shared_ptr<PropertyList> GetCreatePropertyList() const;

            /// operators
            Group &operator=(const Group &rhs) = delete; //!< copy assignment
            Group &operator=(Group &&rhs);               //!< move assignment

            ~Group() { Close(); } //!< destructor
        };
    }
}