
#ifndef __LIST_H__
#define __LIST_H__

#include "type.h"
#include "object.h"

namespace AXP {
    template<typename T>
    class List : public CObject
    {
    public:

        class Node
        {
        public:

            explicit Node(CONST Sp<T> & val)
                : mValue(val)
            {
            }

            INLINE Sp<T> GetValue()
            {
                return mValue;
            }

            INLINE CONST Sp<T> GetValue() CONST
            {
                return mValue;
            }

            INLINE Node * GetPrev() CONST
            {
                return mPrev;
            }

            INLINE Node * GetNext() CONST
            {
                return mNext;
            }

            INLINE Void SetValue(CONST Sp<T> & val)
            {
                mValue = val;
            }

            INLINE Void SetPrev(Node * ptr)
            {
                mPrev = ptr;
            }

            INLINE Void SetNext(Node * ptr)
            {
                mNext = ptr;
            }

        private:

            FRIEND class List;
            Sp<T> mValue;
            Node * mPrev;
            Node * mNext;
        };

        // 迭代器
        class Iterator : public CObject
        {
        public:

            Boolean MoveNext()
            {
                if (!mNextNode || (mNextNode == mNode))
                    return FALSE;

                mCurrentNode = mNextNode;
                mNextNode = mCurrentNode->GetNext();

                return TRUE;
            }

            Void Reset()
            {
                mCurrentNode = mNode;
                if (mCurrentNode)
                    mNextNode = mCurrentNode->GetNext();
                else
                    mNextNode = NULL;
            }

            Sp<T> GetValue()
            {
                if (mCurrentNode)
                    return mCurrentNode->GetValue();
                else
                    return NULL;
            }

        private:

            explicit Iterator(Node * node)
            {
                mNode = node;
                mCurrentNode = node;
                mNextNode = NULL;
                if (mCurrentNode)
                    mNextNode = mCurrentNode->GetNext();
            }

            Iterator(CONST Iterator & x)
            {
                mNode = x.mNode;
                mCurrentNode = x.mCurrentNode;
                mNextNode = x.mNextNode;
                if (mCurrentNode)
                    mNextNode = mCurrentNode->GetNext();
            }

        private:

            FRIEND class List;
            Node * mNode;
            Node * mCurrentNode;
            Node * mNextNode;
        };

    public:

        List()
            : mMiddle(NULL)
        {
            Initialize();
        }

        List(CONST List<T> & src)
            : mMiddle(NULL)
        {
            Initialize();

            Node * node = src.Begin();
            Int32 count = src.GetCount();
            for (Int32 n = 0; (node) && (n < count); node = node->GetNext(), n++) {
                T * value = node->GetValue();
                Insert(End(), value);
            }
        }

        STATIC Sp<List> Clone(
            CONST Sp<List> & src)
        {
            if (src == NULL)
                return NULL;

            Node * first = src->Begin();
            Node * last = src->End();
            Sp<List> ret = new List();
            if (ret == NULL)
                return NULL;

            for (; (first) && (first != last); first = first->GetNext()) {
                Sp<T> value = first->GetValue();
                if (!ret->PushBack(value))
                    return NULL;
            }
            return ret;
        }

        VIRTUAL ~List()
        {
            Clear();
        }

        INLINE Boolean IsEmpty() const
        {
            return (mMiddle.GetNext() == &mMiddle);
        }

        Int32 GetCount() CONST
        {
            return mCount;
        }

        Boolean PushFront(CONST Sp<T> & val)
        {
            return Insert(Begin(), val);
        }

        Boolean PushBack(CONST Sp<T> & val)
        {
            return Insert(End(), val);
        }

        Boolean PopFront()
        {
            Node * node = Begin();
            Node * last = End();
            if (node == last)
                return FALSE;

            return Detach(node->GetValue());
        }

        Boolean PopBack()
        {
            Node * last = End();
            if (NULL == last)
                return FALSE;

            Node * node = last->GetPrev();
            if (node == last)
                return FALSE;

            return Detach(node->GetValue());
        }

        Void Clear()
        {
            Node * pCurrent = mMiddle.GetNext();
            Node * pNext;

            while (pCurrent != &mMiddle) {
                pNext = pCurrent->GetNext();
                delete pCurrent;
                pCurrent = pNext;
            }

            mMiddle.SetPrev(&mMiddle);
            mMiddle.SetNext(&mMiddle);
            mCount = 0;
        }

        INLINE Node * Begin() CONST
        {
            return mMiddle.GetNext();
        }

        INLINE Node * End()
        {
            return &mMiddle;
        }

        Boolean Detach(CONST Sp<T> & value)
        {
            Node * node = Begin();
            Node * last = End();
            for (; (node) && (node != last); node = node->GetNext()) {
                Sp<T> temp = node->GetValue();
                if (value == temp)
                    break;
            }

            if (node == last)
                return FALSE;

            Node * pNext = node->GetNext();
            Node * pPrev = node->GetPrev();
            pPrev->SetNext(pNext);
            pNext->SetPrev(pPrev);

            delete node;
            mCount--;

            return TRUE;
        }

        Sp<Iterator> GetIterator()
        {
            return new Iterator(&mMiddle);
        }

        Sp<T> operator [](Int32 ItemIndex)
        {
            Node * first = Begin();
            Node * last = End();
            for (Int32 i = 0; (first) && (first != last); first = first->GetNext()) {
                if (i++ == ItemIndex) {
                    return first->GetValue();
                }
            }

            return NULL;
        }

        Sp<T> Get(Int32 ItemIndex)
        {
            Node * first = Begin();
            Node * last = End();
            for (Int32 i = 0; (first) && (first != last); first = first->GetNext()) {
                if (i++ == ItemIndex) {
                    return first->GetValue();
                }
            }

            return NULL;
        }

        Void SetValue(Int32 ItemIndex, CONST Sp<T> & value)
        {
            Node * first = Begin();
            Node * last = End();
            for (Int32 i = 0; (first) && (first != last); first = first->GetNext()) {
                if (i++ == ItemIndex) {
                    first->SetValue(value);
                    return;
                }
            }
        }

    private:

        Void Initialize()
        {
            mMiddle.SetPrev(&mMiddle);
            mMiddle.SetNext(&mMiddle);
            mCount = 0;
        }

        Boolean Insert(Node * node, CONST Sp<T> & val)
        {
            if (!node)
                return FALSE;

            Node * newNode = new Node(val);
            if (!newNode)
                return FALSE;

            newNode->SetNext(node);
            newNode->SetPrev(node->GetPrev());
            node->GetPrev()->SetNext(newNode);
            node->SetPrev(newNode);
            mCount++;

            return TRUE;
        }

    private:

        Node mMiddle;
        Int32 mCount;
    };

#ifdef COMPILER_MSVC
#define Foreach(clazz, var, list) \
    if (AXP::Sp<clazz> var = NULL) {} \
    else \
    for (AXP::List<clazz>::Node * var##var = ((list)->Begin()), \
    * next##var = NULL, *last##var = ((list)->End()); \
    (var##var) && (var##var != last##var) && ((next##var = var##var->GetNext()) || TRUE) \
    && (((var) = var##var->GetValue()) || TRUE); \
    var##var = next##var)
#elif defined(COMPILER_GCC)
#define Foreach(clazz, var, list) \
    if (AXP::Sp<clazz> var = NULL) {} \
    else \
    for (typename AXP::List<clazz>::Node * var##var = ((list)->Begin()), \
    * next##var = NULL, * last##var = ((list)->End()); \
    (var##var) && (var##var != last##var) && ((next##var = var##var->GetNext()) || TRUE) \
    && ((var) = var##var->GetValue()); \
    var##var = next##var)
#endif
} // namespace AXP

#endif // __LIST_H__
