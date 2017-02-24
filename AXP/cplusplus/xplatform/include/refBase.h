
#ifndef __REF_BASE_H__
#define __REF_BASE_H__

#include "type.h"
#include "atom.h"

namespace AXP {
    class RefBase
    {
    public:

        RefBase()
            : mCount(0)
        {
        }

        INLINE Int32 AddRef()
        {
            return CInterlocked::Increment(&mCount);
        }

        INLINE Int32 Release()
        {
            Int32 lRet = CInterlocked::Decrement(&mCount);
            if (lRet == 0)
                delete this;

            return lRet;
        }

        INLINE Int32 GetRefCount() const
        {
            return mCount;
        }

    protected:

        INLINE VIRTUAL ~RefBase() { }

    private:

        VOLATILE Int32 mCount;
    };

    template<typename T> class Wp;

    template <typename T>
    class Sp
    {
    public:

        INLINE Sp() : mPtr(NULL) { }

        Sp(T * other);
        Sp(CONST Sp<T> & other);
        Sp(CONST Wp<T> & other);

        ~Sp();

        template<typename U>
        Sp(CONST Sp<U> & u)
        {
            mPtr = (T*)u.Get();
            if (mPtr)
                mPtr->AddRef();
        }

        Sp & operator = (T * other);
        Sp & operator = (CONST Sp<T> & other);
        Sp & operator = (CONST Wp<T> & other);

        INLINE T & operator* () CONST{ return *mPtr; }
        INLINE T * operator-> () CONST{ return mPtr; }
        INLINE T * Get() CONST{ return mPtr; }
        INLINE T ** GetPtr() { return (T**)&mPtr; }

        INLINE operator T * () CONST{ return mPtr; }
        INLINE Boolean operator == (CONST T * t) CONST{ return (mPtr == t); }
        INLINE Boolean operator != (CONST T * t) CONST{ return (mPtr != t); }
        INLINE Boolean operator == (CONST Sp<T> & t) CONST{ return (mPtr == t.Get()); }
        INLINE Boolean operator != (CONST Sp<T> & t) CONST{ return (mPtr != t.Get()); }

    private:

        T * mPtr;
    };

    template<typename T>
    Sp<T>::Sp(T * other)
        : mPtr(other)
    {
        if (other)
            other->AddRef();
    }

    template<typename T>
    Sp<T>::Sp(CONST Sp<T> & other)
        : mPtr(other.Get())
    {
        if (mPtr)
            mPtr->AddRef();
    }

    template<typename T>
    Sp<T>::Sp(CONST Wp<T> & other)
        : mPtr(other.Get())
    {
        if (mPtr)
            mPtr->AddRef();
    }

    template<typename T>
    Sp<T>::~Sp()
    {
        if (mPtr) {
            mPtr->Release();
            mPtr = NULL;
        }
    }

    template<typename T>
    Sp<T> & Sp<T>::operator = (CONST Sp<T> & other)
    {
        T * otherPtr(other.Get());
        if (otherPtr)
            otherPtr->AddRef();

        if (mPtr)
            mPtr->Release();

        mPtr = otherPtr;

        return *this;
    }

    template<typename T>
    Sp<T> & Sp<T>::operator = (T * other)
    {
        if (other)
            other->AddRef();

        if (mPtr)
            mPtr->Release();

        mPtr = other;

        return *this;
    }

    template<typename T>
    Sp<T> & Sp<T>::operator = (CONST Wp<T> & other)
    {
        T * otherPtr(other.Get());
        if (otherPtr)
            otherPtr->AddRef();

        if (mPtr)
            mPtr->Release();

        mPtr = otherPtr;

        return *this;
    }

    template <typename T>
    class Wp
    {
    public:

        inline Wp() : mPtr(NULL) { }

        Wp(T * other);
        Wp(CONST Wp<T> & other);
        Wp(CONST Sp<T> & other);

        ~Wp();

        Wp & operator = (T * other);
        Wp & operator = (CONST Wp<T> & other);
        Wp & operator = (CONST Sp<T> & other);

        INLINE T & operator* () CONST{ return *mPtr; }
        INLINE T * operator-> () CONST{ return mPtr; }
        INLINE T * Get() CONST{ return mPtr; }
        INLINE operator CONST T * () CONST{ return mPtr; }
        INLINE operator T * () CONST{ return mPtr; }
        INLINE operator CONST T & () CONST{ return mPtr; }
        INLINE operator T & () CONST{ return mPtr; }
        INLINE bool operator == (CONST T * t) CONST{ return (mPtr == t); }
        INLINE bool operator != (CONST T * t) CONST{ return (mPtr != t); }

    private:

        T * mPtr;
    };

    template<typename T>
    Wp<T>::Wp(T * other)
        : mPtr(other)
    {
    }

    template<typename T>
    Wp<T>::Wp(CONST Wp<T> & other)
        : mPtr(other.Get())
    {
    }

    template<typename T>
    Wp<T>::Wp(CONST Sp<T> & other)
        : mPtr(other.Get())
    {
    }

    template<typename T>
    Wp<T>::~Wp()
    {
        if (mPtr)
            mPtr = NULL;
    }

    template<typename T>
    Wp<T> & Wp<T>::operator = (T * other)
    {
        mPtr = other;

        return *this;
    }

    template<typename T>
    Wp<T> & Wp<T>::operator = (CONST Wp<T> & other)
    {
        T * otherPtr(other.Get());
        mPtr = otherPtr;

        return *this;
    }

    template<typename T>
    Wp<T> & Wp<T>::operator = (CONST Sp<T> & other)
    {
        T * otherPtr(other.Get());
        mPtr = otherPtr;

        return *this;
    }
} // namespace AXP

#endif // __REF_BASE_H__