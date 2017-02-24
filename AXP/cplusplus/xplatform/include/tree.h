
#ifndef __TREE_H__
#define __TREE_H__

#include "type.h"

namespace AXP {
    class TreeNode
    {
    public:

        TreeNode()
        {
            Initialize();
        }

        ~TreeNode()
        {
            Detach();
        }

    public:

        Void Initialize()
        {
            pParent = NULL;
            pPrev = NULL;
            pNext = NULL;
            pChild = NULL;
        }

        Void Attach(
            IN TreeNode * pChild)
        {
            if (this->pChild) {
                pChild->pNext = this->pChild;
                this->pChild->pPrev = pChild;
            }

            this->pChild = pChild;
            pChild->pParent = this;
        }

        Void Detach()
        {
            if (pPrev) pPrev->pNext = pNext;
            if (pNext) pNext->pPrev = pPrev;
            if (!pPrev && !pNext)
                if (pParent) pParent->pChild = NULL;
                else if (!pPrev)
                    if (pParent) pParent->pChild = pNext;

            Initialize();
        }

        Boolean IsEmpty() CONST
        {
            return (!pParent);
        }

    public:

        TreeNode * pParent;
        TreeNode * pPrev;
        TreeNode * pNext;
        TreeNode * pChild;
    };
} // namespace AXP

#endif // __TREE_H__

