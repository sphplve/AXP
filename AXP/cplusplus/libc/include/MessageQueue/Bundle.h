
#ifndef __BUNDLE_H__
#define __BUNDLE_H__

#include "../../../xplatform/include/stl/hashTable.h"
#include "../../../xplatform/include/linkedlist.h"
#include "../../../xplatform/include/astring.h"

namespace AXP {
    namespace Libc {
        namespace MessageQueue {
            class Bundle : public CObject
            {
            public:

                STATIC Sp<Bundle> Create(
                    IN Int32 capacity);

                STATIC Sp<Bundle> Create(
                    IN CONST Sp<Bundle> & bundle);

                Int32 Size();

                Boolean IsEmpty();

                Void Clear();

                Boolean ContainsKey(
                    IN CONST Sp<String> & key);

                ARESULT Put(
                    IN CONST Sp<String> & key,
                    IN CONST Sp<CObject> & value);

                Sp<CObject> Get(IN CONST Sp<String> & key);

                Void Remove(
                    IN CONST Sp<String> & key);

                Void Remove();

            private:

                Bundle()
                {
                }

                Bundle(
                    IN CONST Bundle & b)
                {
                }

            private:

                Sp<HashTable<PWStr, CObject> > mHashTable;

            };
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP

#endif // __BUNDLE_H__
