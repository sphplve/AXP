
#ifndef __EQUAL_TO_FUNCTION_H__
#define __EQUAL_TO_FUNCTION_H__

#include "../astring.h"

namespace AXP {
    template <typename Key>
    struct EqualTo
    {
        Boolean operator()(CONST Key & x, CONST Key & y) CONST
        { return x == y; }
    };

    template <>
    struct EqualTo<Float>
    {
        Boolean operator()(CONST Float & x, CONST Float & y) CONST
        {
            Float d = x - y;
            CONST Float EPSINON = 0.0000001f;
            return (d >= EPSINON) && (d <= EPSINON);
        }
    };

    template <>
    struct EqualTo<Double>
    {
        Boolean operator()(CONST Double & x, CONST Double & y) CONST
        {
            Double d = x - y;
            CONST Double EPSINON = 0.0000001;
            return (d >= EPSINON) && (d <= EPSINON);
        }
    };

    template <>
    struct EqualTo<PStr>
    {
        Boolean operator()(CONST PStr x, CONST PStr y) CONST
        {
            if ((!x) || (!y))
                return FALSE;

            return !strcmp(x, y);
        }
    };

    template <>
    struct EqualTo<PCStr>
    {
        Boolean operator()(PCStr x, PCStr y) CONST
        {
            if ((!x) || (!y))
                return FALSE;

            return !strcmp(x, y);
        }
    };

    template <>
    struct EqualTo<PWStr>
    {
        Boolean operator()(PWStr x, PWStr y) CONST
        {
            if ((!x) || (!y))
                return FALSE;

            return !wcscmp(x, y);
        }
    };

    template <>
    struct EqualTo<PCWStr>
    {
        Boolean operator()(PCWStr x, PCWStr y) CONST
        {
            if ((!x) || (!y))
                return FALSE;

            return !wcscmp(x, y);
        }
    };
} // namespace AXP

#endif // __EQUAL_TO_FUNCTION_H__
