
#ifndef __STL_TREE_H__
#define __STL_TREE_H__

#include "../type.h"
#include "../aresult.h"
#include "../object.h"
#include "../list.h"

namespace AXP {
    namespace STL {
        template<typename T>
        class TreeNode : public CObject
        {
        public:

            class ITreeNodeListener : public CObject
            {
            public:

                VIRTUAL Void OnTraverseTreeNode(
                    IN CONST Sp<TreeNode<T> > & node) = 0;
            };

        public:

            STATIC Sp<TreeNode<T> > Create()
            {
                Sp<TreeNode<T> > node = new TreeNode<T>();
                if (node == NULL)
                    return NULL;

                if (AFAILED(node->Initialize()))
                    return NULL;

                return node;
            }

            STATIC Sp<TreeNode<T> > Create(
                IN CONST Sp<T> & value)
            {
                Sp<TreeNode<T> > node = new TreeNode<T>();
                if (node == NULL)
                    return NULL;

                if (AFAILED(node->Initialize()))
                    return NULL;

                node->SetValue(value);

                return node;
            }

        public:

            ARESULT Initialize()
            {
                mChildren = new List<TreeNode>();
                if (mChildren == NULL)
                    return AE_OUTOFMEMORY;
                else
                    return AS_OK;
            }

            Sp<T> GetValue()
            {
                return mValue;
            }

            Void SetValue(
                IN CONST Sp<T> & value)
            {
                mValue = value;
            }

            Sp<TreeNode<T> > GetParent()
            {
                return mParent;
            }

            Sp<TreeNode<T> > AddChild(
                IN CONST Sp<T> & child)
            {
                if (child == NULL)
                    return NULL;

                Sp<TreeNode<T> > node = TreeNode<T>::Create();
                if (node == NULL)
                    return NULL;

                node->mParent = this;
                node->mValue = child;

                if (!mChildren->PushBack(node))
                    return NULL;

                return node;
            }

            Sp<TreeNode<T> > AddChildFront(
                IN CONST Sp<T> & child)
            {
                if (child == NULL)
                    return NULL;

                Sp<TreeNode<T> > node = TreeNode<T>::Create();
                if (node == NULL)
                    return NULL;

                node->mParent = this;
                node->mValue = child;

                if (!mChildren->PushFront(node))
                    return NULL;

                return node;
            }

            Boolean RemoveChild(
                IN CONST Sp<TreeNode<T> > & node)
            {
                return mChildren->Detach(node);
            }

            Boolean Detach()
            {
                if (mParent == NULL)
                    return FALSE;
                else
                    return mParent->RemoveChild(this);
            }

            Void Traverse(
                IN CONST Sp<ITreeNodeListener> & listener)
            {
                if (listener != NULL)
                    listener->OnTraverseTreeNode(this);

                Foreach(TreeNode<T>, node, mChildren) {
                    node->Traverse(listener);
                }
            }

            Sp<List<TreeNode<T> > > GetChildren()
            {
                return mChildren;
            }

            Void RemoveAllChild()
            {
                mChildren->Clear();
            }

        public:

            Boolean IsLastChild()
            {
                if (mParent == NULL)
                    return TRUE;

                if (mParent->mChildren->IsEmpty())
                    return TRUE;

                return (mParent->mChildren->End()->GetPrev()->GetValue().Get() == this);
            }

        private:

            Wp<TreeNode> mParent;
            Sp<List<TreeNode<T> > > mChildren;
            Sp<T> mValue;
        };
    } // namespace STL
} // namespace AXP

#endif // __STL_TREE_H__

