
#ifndef __IPARCELABLE_H__
#define __IPARCELABLE_H__

#include "type.h"
#include "aresult.h"
#include "refBase.h"

namespace AXP {
    class CParcel;

    class IParcelable
    {
    public:

        VIRTUAL ARESULT ReadFromParcel(
            IN CONST Sp<CParcel> & parcel) = 0;

        VIRTUAL ARESULT WriteToParcel(
            IN CONST Sp<CParcel> & parcel) = 0;
    };
} // namespace AXP

#endif // __IPARCELABLE_H__;