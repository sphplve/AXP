
#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "limits.h"
#include "type.h"

namespace AXP {
    class SingleLinkNode;
    class DoubleLinkNode;

#define ForeachSLinkNode(ClassName, pObject, pHead) \
    for (pObject = (ClassName*)((pHead)->First()); \
    (AXP::SingleLinkNode*)pObject != (AXP::SingleLinkNode*)pHead; \
    pObject = (ClassName*)(pObject->m_pNext))

#define ForeachSLinkNodeEx(ClassName, pObject, pTempObject, pHead) \
    for (pObject = (ClassName*)((pHead)->First()), \
    pTempObject = (ClassName*)(pObject->m_pNext); \
    ((AXP::SingleLinkNode*)pObject != (AXP::SingleLinkNode*)pHead) \
    && ((pTempObject = (ClassName*)(pObject->m_pNext)) || TRUE); \
    pObject = pTempObject)

#define ForeachDLinkNode(ClassName, pObject, pHead) \
    for (pObject = (ClassName*)((pHead)->First()); \
    (AXP::DoubleLinkNode*)pObject != (AXP::DoubleLinkNode*)pHead; \
    pObject = (ClassName*)(pObject->m_pNext))

#define ForeachDLinkNode2(ClassName, pObject, pHead) \
    for (ClassName * pObject = (ClassName*)((pHead)->First()); \
    (AXP::DoubleLinkNode*)pObject != (AXP::DoubleLinkNode*)pHead; \
    pObject = (ClassName*)(pObject->m_pNext))

#define ForeachDLinkNodeEx(ClassName, pObject, pTempObject, pHead) \
    for (pObject = (ClassName*)((pHead)->First()), \
    pTempObject = (ClassName*)(pObject->m_pNext); \
    ((AXP::DoubleLinkNode*)pObject != (AXP::DoubleLinkNode*)pHead) \
    && ((pTempObject = (ClassName*)(pObject->m_pNext)) || TRUE); \
    pObject = pTempObject)

#define LL_CONTAINING_RECORD(address, type, field) \
    ((type*)((UInt)(address) - (UInt)(&((type*)0x100000)->field) + 0x100000))

#define NODE_OFFSET(type, field) \
    ((UInt)(&((type*)0x100000)->field) - 0x100000)

#define CONTAINING_RECORD_BY_OFFSET(address, type, offset) \
    ((type*)((ULong)(address) - (UInt)(offset)))

#define CONTAINING_NODE_BY_OFFSET(address, offset) \
    ((UInt)(address) + (UInt)(offset))

    class SingleLinkNode
    {
    public:

        //
        // Non-head-node operations
        //
        // Return the next node.
        SingleLinkNode * Next() const {
            return m_pNext;
        }

        // Insert a new node after me.  Return the new next node.
        SingleLinkNode * InsertNext(SingleLinkNode * pNextNode) {
            pNextNode->m_pNext = m_pNext;
            m_pNext = pNextNode;

            return pNextNode;
        }

        // Remove myself from list. Return myself.
        SingleLinkNode * Detach(SingleLinkNode * pPrevNode) {
            pPrevNode->m_pNext = m_pNext;
            // avoid multiple calls Detach(), the same to Initialize()
            m_pNext = this;

            return this;
        }

        //
        // Head-node operations
        //
        Void Initialize() {
            m_pNext = this;
        }

        Boolean IsEmpty() const {
            return this == m_pNext;
        }

        // Return the first node of list.
        SingleLinkNode * First() const {
            return Next();
        }

        // Insert a new node as the first of list. Return the new first node.
        SingleLinkNode * InsertFirst(SingleLinkNode * pFirstNode) {
            return InsertNext(pFirstNode);
        }

    public:

        SingleLinkNode *         m_pNext;
    };

    class DoubleLinkNode
    {
    public:

        //
        // Non-head-node operations
        //

        DoubleLinkNode * Prev() const
        {
            return m_pPrev;
        }

        DoubleLinkNode * Next() const 
        {
            return m_pNext;
        }

        DoubleLinkNode * InsertPrev(
            IN DoubleLinkNode * pPrevNode) 
        {
            pPrevNode->m_pPrev = this->m_pPrev;
            pPrevNode->m_pNext = this;
            this->m_pPrev->m_pNext = pPrevNode;
            this->m_pPrev = pPrevNode;

            return pPrevNode;
        }

        DoubleLinkNode * InsertNext(
            IN DoubleLinkNode * pNextNode) 
        {
            pNextNode->m_pPrev = this;
            pNextNode->m_pNext = this->m_pNext;
            this->m_pNext->m_pPrev = pNextNode;
            this->m_pNext = pNextNode;

            return pNextNode;
        }

        DoubleLinkNode * Detach() 
        {
            this->m_pPrev->m_pNext = this->m_pNext;
            this->m_pNext->m_pPrev = this->m_pPrev;
            // avoid multiple calls Detach(), the same to Initialize()
            m_pPrev = m_pNext = this;

            return this;
        }

        //
        // Head-node operations
        //

        Void Initialize() 
        {
            m_pPrev = m_pNext = this;
        }

        Boolean IsEmpty() const 
        {
            return (this == m_pNext);
        }

        DoubleLinkNode * First() const 
        {
            return Next();
        }

        DoubleLinkNode * Last() const 
        {
            return Prev();
        }

        DoubleLinkNode * InsertFirst(
            IN DoubleLinkNode * pFirstNode) 
        {
            return InsertNext(pFirstNode);
        }

        DoubleLinkNode * InsertLast(
            IN DoubleLinkNode * pLastNode) 
        {
            return InsertPrev(pLastNode);
        }

    public:

        DoubleLinkNode * m_pPrev;
        DoubleLinkNode * m_pNext;
    };

    template <typename T>
    class CSLinkList : public SingleLinkNode
    {
    public:

        CSLinkList() {
            u32ItemCount = 0;
            SingleLinkNode::Initialize();
        }

        ~CSLinkList() {
            Clean();
        }

        T * InsertFirst(
            IN T * pFirstNode) {
                T * pRet = (T*)SingleLinkNode::InsertNext(pFirstNode);
                u32ItemCount++;

                return pRet;
        }

        T * InsertLast(
            IN T * pLastNode) {
                T * pRet;
                SingleLinkNode * pNode = (SingleLinkNode*)Last();
                if (pNode) {
                    pRet = pNode->InsertNext((SingleLinkNode*)pLastNode);
                    u32ItemCount++;
                } else {
                    pRet = InsertFirst(pLastNode);
                }

                return pRet;
        }

        T * Detach(
            IN T * pDetachNode) {
                if (!pDetachNode->IsEmpty()) u32ItemCount--;
                return (T*)pDetachNode->Detach();
        }

        T * operator [](
            IN UInt32 u32ItemIndex) {
                T * pNode;
                UInt32 i = 0;

                ForeachSLinkNode(T, pNode, this) {
                    if (i++ == u32ItemIndex) {
                        return pNode;
                    }
                }

                return NULL;
        }

        Void Clean() {
            T *pT, *pTempT;

            ForeachSLinkNodeEx(T, pT, pTempT, this) {
                this->Detach(pT);
                delete pT;
            }
        }

        Void Initialize() {
            SingleLinkNode::Initialize();
        }

        Boolean IsEmpty() const {
            return SingleLinkNode::IsEmpty();
        }

        T * First() const {
            return (T*)SingleLinkNode::First();
        }

        T * Last() const {
            T * pNode = NULL;
            ForeachSLinkNode(T, pNode, this);
            return pNode;
        }

        UInt32 GetItemCount() {
            return u32ItemCount;
        }

    public:

        UInt32 u32ItemCount;
    };

    template <typename T>
    class CSLinkListEx : public SingleLinkNode
    {
    public:

        CSLinkListEx(
            IN UInt uLinkeNodeOffset) 
        {
            u32ItemCount = 0;
            SingleLinkNode::Initialize();
            uOffset = uLinkeNodeOffset;
        }

        T * InsertFirst(
            IN T * pT) 
        {
            SingleLinkNode * pFirstNode =
                (SingleLinkNode*)CONTAINING_NODE_BY_OFFSET(pT, uOffset);
            SingleLinkNode::InsertNext(pFirstNode);
            u32ItemCount++;

            return pT;
        }

        T * InsertLast(
            IN T * pT) 
        {
            SingleLinkNode * pLastNode =
                (SingleLinkNode*)CONTAINING_NODE_BY_OFFSET(pT, uOffset);
            SingleLinkNode * pNode = (SingleLinkNode*)Last();
            if (pNode) {
                pNode->InsertNext((SingleLinkNode*)pLastNode);
                u32ItemCount++;
            } else {
                InsertFirst(pLastNode);
            }

            return pT;
        }

        T * Detach(
            IN T * pT) 
        {
            SingleLinkNode * pDetachNode =
                (SingleLinkNode*)CONTAINING_NODE_BY_OFFSET(pT, uOffset);
            if (pDetachNode->IsEmpty())
                return pT;
            
            SingleLinkNode * pNode;
            
            ForeachSLinkNode(SingleLinkNode, pNode, this) {
                if (pNode->Next() == pDetachNode) {
                    pDetachNode->Detach(pNode);
                    return pT;
                }
            }
            
            u32ItemCount--;

            return pT;
        }

        T * operator [](
            IN UInt u32ItemIndex) 
        {
            SingleLinkNode * pNode;
            UInt32 i = 0;

            ForeachSLinkNode(SingleLinkNode, pNode, this) {
                if (i++ == u32ItemIndex) {
                    return CONTAINING_RECORD_BY_OFFSET(pNode, T, uOffset);
                }
            }

            return NULL;
        }

        Void Clean() 
        {
            SingleLinkNode *pNode, *pTempNode;
            T * pT;

            ForeachSLinkNodeEx(SingleLinkNode, pNode, pTempNode, this) {
                pT = CONTAINING_RECORD_BY_OFFSET(pNode, T, uOffset);
                this->Detach(pNode);
                delete pT;
            }
        }

        Void DetachAll() 
        {
            SingleLinkNode *pNode, *pTempNode;

            ForeachDLinkNodeEx(SingleLinkNode, pNode, pTempNode, this) {
                this->Detach(pNode);
            }
        }

        T * GetFirst() 
        {
            SingleLinkNode * pNode = First();
            return CONTAINING_RECORD_BY_OFFSET(pNode, T, uOffset);
        }

        T * GetLast() 
        {
            SingleLinkNode * pNode = Last();
            return CONTAINING_RECORD_BY_OFFSET(pNode, T, uOffset);
        }

        Void Initialize() 
        {
            SingleLinkNode::Initialize();
        }

        Boolean IsEmpty() const 
        {
            return SingleLinkNode::IsEmpty();
        }

        SingleLinkNode * First() const 
        {
            return SingleLinkNode::First();
        }

        SingleLinkNode * Last() const 
        {
            T * pNode = NULL;
            ForeachSLinkNode(T, pNode, this);
            return pNode;
        }

        UInt32 GetItemCount() 
        {
            return u32ItemCount;
        }

    private:

        CSLinkListEx() {}

    public:

        UInt32 u32ItemCount;
        UInt uOffset;
    };

    template <typename T>
    class CDLinkList : public DoubleLinkNode
    {
    public:

        CDLinkList() 
        {
            u32ItemCount = 0;
            DoubleLinkNode::Initialize();
        }

        ~CDLinkList() 
        {
            Clean();
        }

        T * InsertFirst(
            IN T * pFirstNode) 
        {
            T * pRet = (T*)DoubleLinkNode::InsertNext(pFirstNode);
            u32ItemCount++;

            return pRet;
        }

        T * InsertLast(
            IN T * pLastNode) 
        {
            T * pRet = (T*)DoubleLinkNode::InsertPrev(pLastNode);
            u32ItemCount++;

            return pRet;
        }

        T * Detach(
            IN T * pDetachNode) 
        {
            if (!pDetachNode->IsEmpty()) u32ItemCount--;
            return (T*)pDetachNode->Detach();
        }

        T * operator [](
            IN UInt32 u32ItemIndex) 
        {
            T * pNode;
            UInt32 i = 0;

            ForeachDLinkNode(T, pNode, this) {
                if (i++ == u32ItemIndex) {
                    return pNode;
                }
            }

            return NULL;
        }

        Void Clean() 
        {
            T *pT, *pTempT;

            ForeachDLinkNodeEx(T, pT, pTempT, this) {
                this->Detach(pT);
                delete pT;
            }
        }

        Void DetachAll() 
        {
            T *pT, *pTempT;

            ForeachDLinkNodeEx(T, pT, pTempT, this) {
                this->Detach(pT);
            }
        }

        Void Initialize() 
        {
            DoubleLinkNode::Initialize();
        }

        Boolean IsEmpty() const 
        {
            return DoubleLinkNode::IsEmpty();
        }

        T * First() const 
        {
            return (T*)DoubleLinkNode::First();
        }

        T * Last() const 
        {
            return (T*)DoubleLinkNode::Last();
        }

        UInt32 GetItemCount() 
        {
            return u32ItemCount;
        }

    public:

        UInt32 u32ItemCount;
    };

    template <typename T>
    class CDLinkListEx : public DoubleLinkNode
    {
    public:

        CDLinkListEx(
            IN UInt uLinkeNodeOffset) 
        {
            u32ItemCount = 0;
            DoubleLinkNode::Initialize();
            uOffset = uLinkeNodeOffset;
        }

        T * InsertFirst(
            IN T * pT) 
        {
            DoubleLinkNode * pFirstNode =
                (DoubleLinkNode*)CONTAINING_NODE_BY_OFFSET(pT, uOffset);
            DoubleLinkNode::InsertNext(pFirstNode);
            u32ItemCount++;

            return pT;
        }

        T * InsertLast(
            IN T * pT) 
        {
            DoubleLinkNode * pLastNode =
                (DoubleLinkNode*)CONTAINING_NODE_BY_OFFSET(pT, uOffset);
            DoubleLinkNode::InsertPrev(pLastNode);
            u32ItemCount++;

            return pT;
        }

        T * Detach(
            IN T * pT) 
        {
            DoubleLinkNode * pDetachNode =
                (DoubleLinkNode*)CONTAINING_NODE_BY_OFFSET(pT, uOffset);
            if (!pDetachNode->IsEmpty()) u32ItemCount--;
            pDetachNode->Detach();

            return pT;
        }

        T * operator [](
            IN UInt u32ItemIndex) 
        {
            DoubleLinkNode * pNode;
            UInt32 i = 0;

            ForeachDLinkNode(DoubleLinkNode, pNode, this) {
                if (i++ == u32ItemIndex) {
                    return CONTAINING_RECORD_BY_OFFSET(pNode, T, uOffset);
                }
            }

            return NULL;
        }

        Void Clean() 
        {
            DoubleLinkNode *pNode, *pTempNode;
            T * pT;

            ForeachDLinkNodeEx(DoubleLinkNode, pNode, pTempNode, this) {
                pT = CONTAINING_RECORD_BY_OFFSET(pNode, T, uOffset);
                this->Detach(pNode);
                delete pT;
            }
        }

        Void DetachAll() 
        {
            DoubleLinkNode *pNode, *pTempNode;

            ForeachDLinkNodeEx(DoubleLinkNode, pNode, pTempNode, this) {
                this->Detach(pNode);
            }
        }

        T * GetFirst() 
        {
            DoubleLinkNode * pNode = First();
            return CONTAINING_RECORD_BY_OFFSET(pNode, T, uOffset);
        }

        T * GetLast() 
        {
            DoubleLinkNode * pNode = Last();
            return CONTAINING_RECORD_BY_OFFSET(pNode, T, uOffset);
        }

        Void Initialize() 
        {
            DoubleLinkNode::Initialize();
        }

        Boolean IsEmpty() const 
        {
            return DoubleLinkNode::IsEmpty();
        }

        DoubleLinkNode * First() const 
        {
            return DoubleLinkNode::First();
        }

        DoubleLinkNode * Last() const 
        {
            return DoubleLinkNode::Last();
        }

        UInt32 GetItemCount() 
        {
            return u32ItemCount;
        }

    private:

        CDLinkListEx() {}

    public:

        UInt32 u32ItemCount;
        UInt32 uOffset;
    };
} // namespace AXP

#endif // __LINKED_LIST_H__
