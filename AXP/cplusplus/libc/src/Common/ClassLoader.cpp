
#include "../../include/Common/ClassLoader.h"

using namespace AXP::Libc::Common::ClassLoader;

STATIC AXP::HashTable<AXP::PCWStr, AXP::CObject> * sClassTable;

class ClassCreatorHolder : public AXP::CObject
{
public:

    ClassCreatorHolder(PCreator creator)
    {
        mCreator = creator;
    }

    PCreator mCreator;

private:

    ClassCreatorHolder()
    {
    }
};

AXP::Sp<AXP::CObject> AXP::Libc::Common::ClassLoader::GetClassRef(IN CONST AXP::PCWStr key)
{
    AXP::Sp<ClassCreatorHolder> obj = sClassTable->GetValue(key);
    if (obj == NULL)
        return NULL;

    return obj->mCreator();
}

AXP::Boolean AXP::Libc::Common::ClassLoader::RegisterClassCreator(
    IN CONST AXP::PCWStr key,
    IN PCreator value)
{
    if ((!key) || (!value))
        return FALSE;

    if (sClassTable == NULL) {
        sClassTable = new AXP::HashTable<AXP::PCWStr, AXP::CObject>(50);
        if (sClassTable == NULL)
            return FALSE;
    }

    AXP::Sp<ClassCreatorHolder> obj = new ClassCreatorHolder(value);
    if (obj == NULL)
        return FALSE;

    return sClassTable->InsertUnique(key, obj);
}