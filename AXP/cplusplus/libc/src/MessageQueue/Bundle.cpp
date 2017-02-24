
#include "../../include/MessageQueue/Bundle.h"

namespace AXP {
    namespace Libc {
        namespace MessageQueue {
            Sp<Bundle> Bundle::Create(
                IN Int32 capacity)
            {
                Sp<Bundle> b = new Bundle();
                if (b == NULL)
                    return NULL;

                b->mHashTable = new HashTable<PWStr, CObject>(capacity);
                if (b->mHashTable == NULL)
                    return NULL;

                return b;
            }

            Sp<Bundle> Bundle::Create(
                IN CONST Sp<Bundle> & bundle)
            {
                if ((bundle == NULL) || (bundle->mHashTable == NULL))
                    return NULL;

                Sp<Bundle> b = new Bundle();
                if (b == NULL)
                    return NULL;

                b->mHashTable = new HashTable<PWStr, CObject>(*bundle->mHashTable);
                if (b->mHashTable == NULL)
                    return NULL;

                return b;
            }

            Int32 Bundle::Size()
            {
                if (mHashTable == NULL)
                    return 0;

                return mHashTable->Size();
            }

            Boolean Bundle::IsEmpty()
            {
                return ((mHashTable == NULL) || mHashTable->Empty());
            }

            Void Bundle::Clear()
            {
                if (mHashTable != NULL)
                    mHashTable->Clear();
            }

            Boolean Bundle::ContainsKey(
                IN CONST Sp<String> & key)
            {
                if ((key == NULL) && (mHashTable == NULL))
                    return FALSE;

                return mHashTable->Contains((PWStr)key->GetPayload());
            }

            ARESULT Bundle::Put(
                IN CONST Sp<String> & key,
                IN CONST Sp<CObject> & value)
            {
                if ((key == NULL) || (mHashTable == NULL))
                    return AE_INVALIDARG;

                if (mHashTable->InsertEqual((PWStr)key->GetPayload(), value))
                    return AS_OK;

                return AE_FAIL;
            }

            Sp<CObject> Bundle::Get(
                IN CONST Sp<String> & key)
            {
                if ((key == NULL) || (mHashTable == NULL))
                    return NULL;

                return mHashTable->GetValue((PWStr)key->GetPayload());
            }

            Void Bundle::Remove(
                IN CONST Sp<String> & key)
            {
                if (key == NULL)
                    return;

                if (mHashTable != NULL)
                    mHashTable->Remove((PWStr)key->GetPayload());
            }

            Void Bundle::Remove()
            {
                if (mHashTable != NULL)
                    mHashTable->Clear();
            }
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP
