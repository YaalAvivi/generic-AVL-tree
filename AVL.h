#ifndef AVL_H
#define AVL_H

#include <iostream>

using namespace std;

//Class AVL node
//The node which represents each vertex in the AVL tree
template <class T>
class AvlNode
{
    public:
    int left_tree_height;
    int right_tree_height;
    T data;
    AvlNode *left;
    AvlNode *right;

    AvlNode(const T data) : left_tree_height(0), right_tree_height(0) ,
                            data(data), left(NULL), right(NULL){}
};

//Class AVL tree
//Implements a generic AVL tree,with nodes of data T
template <class T>
class AvlTree {
    private:
        AvlNode<T>* root;

        /**
        * aux_copy_ctor: insert each node from the other tree to the new tree
        */
        void aux_copy_ctor(AvlTree<T>* new_tree,const AvlNode<T>* current)
        {
            if(current!=NULL)
            {
                new_tree->insert(current->data);
                aux_copy_ctor(new_tree,current->left);
                aux_copy_ctor(new_tree,current->right);
            }
        }

        /**
        * Destructor_aux: deletes each node from the tree
        */
        void Destructor_aux(AvlNode<T>* vertex)
        {
            if(vertex==NULL)
                return;
            Destructor_aux(vertex->left);
            Destructor_aux(vertex->right);
            delete vertex;
        }

        /**
        * max: returns the bigger number
        * @param:
        *       a,b
        */
        static int max(int a, int b)
        {   
            return a<b? b:a;
        }

        /**
        * height: returns the height of given vertex
        * @param:
        *       vertex
        * return: returns the height of given vertex
        * Complexity: O(1)
        */
        int height(const AvlNode<T>* vertex) const
        {
            if(vertex==NULL)
            {
                return 0;
            }
            return max(vertex->left_tree_height, vertex->right_tree_height)+1;
        }

        /**
        * factorBalance: returns the factorBalance of given vertex
        * @param:
        *       vertex
        * return: returns the factorBalance of given vertex
        * Complexity: O(1)
        */
        int factorBalance(const AvlNode<T> *vertex) const
        {
            int balance_factor = vertex->left_tree_height - vertex->right_tree_height;
            return balance_factor;
        }

        /**
        * RR_roll: do a right right roll on the given vertex
        * @param:
        *       unbalanced_vertex - the vertex which need to be rolled
        * return: returns the vertex which replaced unbalanced_vertex
        * Complexity: O(1)
        */
        AvlNode<T>* RR_roll(AvlNode<T>* unbalanced_vertex)
        {
            AvlNode<T>* child;
            child = unbalanced_vertex->right;
            unbalanced_vertex->right = child->left;
            child->left = unbalanced_vertex;
            if(this->root==unbalanced_vertex)
            {
                this->root=child;
            }
            unbalanced_vertex->right_tree_height=child->left_tree_height;
            child->left_tree_height=height(unbalanced_vertex);
            return child;
        }

        /**
        * LL_roll: do a left left roll on the given vertex
        * @param:
        *       unbalanced_vertex - the vertex which need to be rolled
        * return: returns the vertex which replaced unbalanced_vertex
        * Complexity: O(1)
        */
        AvlNode<T>* LL_roll(AvlNode<T>* unbalanced_vertex)
        {
            AvlNode<T>* child;
            child = unbalanced_vertex->left;
            unbalanced_vertex->left = child->right;
            child->right = unbalanced_vertex;
            if(this->root==unbalanced_vertex)
            {
                this->root=child;
            }
            unbalanced_vertex->left_tree_height=child->right_tree_height;
            child->right_tree_height=height(unbalanced_vertex);
            return child;
        }

                /**
        * LR_roll: do a left right roll on the given vertex
        * @param:
        *       unbalanced_vertex - the vertex which need to be rolled
        * return: returns the vertex which replaced unbalanced_vertex
        * Complexity: O(1)
        */
        AvlNode<T>* LR_roll(AvlNode<T>* unbalanced_vertex) {
            AvlNode<T>* child;
            child = unbalanced_vertex->left;
            unbalanced_vertex->left = RR_roll(child);
            return LL_roll(unbalanced_vertex);
        }

        /**
        * RL_roll: do a right left roll on the given vertex
        * @param:
        *       unbalanced_vertex - the vertex which need to be rolled
        * return: returns the vertex which replaced unbalanced_vertex
        * Complexity: O(1)
        */
        AvlNode<T>* RL_roll(AvlNode<T>* unbalanced_vertex)
        {
            AvlNode<T>* child;
            child = unbalanced_vertex->right;
            unbalanced_vertex->right = LL_roll(child);
            return RR_roll(unbalanced_vertex);
        }
        
        /**
        * balance: balance vertex if needed
        * @param:
        *       vertex - the vertex which may need to be balanced
        * return: returns the vertex which replaced vertex after the balance
        * Complexity: O(1)
        */
        AvlNode<T>* balance(AvlNode<T>* vertex)
        {
            int balance_factor = factorBalance(vertex);
            if (balance_factor > 1)
            {
                if (factorBalance(vertex->left) >= 0)
                    vertex = LL_roll(vertex);
                else
                    vertex = LR_roll(vertex);
            }
            else if (balance_factor < -1)
            {
                if (factorBalance(vertex->right) > 0)
                    vertex = RL_roll(vertex);
                else
                    vertex = RR_roll(vertex);
            }
        return vertex;
        }

        /**
        * aux_insert: insert the new node in the tree and balance the tree after the insert
        * @param:
        *       current - the current vertex in the tree
        *       node - the node that need to be inserted
        * return: returns the vertex which replaced current after the insert and balance (if balance was needed)
        * Complexity: O(log(n)) (total, not for each call)
        */
        AvlNode<T>* aux_insert(AvlNode<T>* current, AvlNode<T>* node)
        {
            if((*current).data < (*node).data)
            {
                if(current->right==NULL)
                {
                    current->right=node;
                }
                else
                {
                    current->right=aux_insert(current->right,node); 
                }
                current->right_tree_height=height(current->right);
            }
            else
            {
                if(current->left==NULL)
                {
                    current->left=node;
                }
                else
                {
                    current->left=aux_insert(current->left, node);
                }
                current->left_tree_height=height(current->left);
            }
            current=balance(current);
            return current;
        }

        /**
        * RemoveIfNo2Child: removes current from the tree if it has 1 or 0 children
        * @param:
        *       parent - the parent of current
        *       current - the vertex that need to be removed
        *       data - the data of current
        *       * Assumptions: operations = of T .
        * Complexity: O(1)
        */
        void RemoveIfNo2Child(AvlNode<T>* parent, AvlNode<T>* current, const T& data)
        {
            AvlNode<T>* child=NULL;
            if(current->left!=NULL)
                {
                    child= current->left;
                }
            else
            {
                child= current->right;
            }
            if(parent->left!=NULL && ((parent->left)->data)==data)//if current was left child
            {
                parent->left=child;
                parent->left_tree_height=height(parent->left);
            }
            if(parent->right!=NULL && (parent->right)->data==data)//if current was right child
            {
                parent->right=child;
                parent->right_tree_height=height(parent->right);
            }
            delete current;
        }
        
        /**
        * aux_remove: find and removes the node with data from the tree, and balance the tree afterwards
        * @param:
        *       parent - the parent of current
        *       current - the vertex of the tree we are currently on
        *       data - the data of the vertex that need to be removed
        *       * Assumptions: comparison of T operations.
        * Complexity: O(log(n)) (total, not for each call)
        */
        void aux_remove(AvlNode<T>* parent, AvlNode<T>* current, const T& data)
        {
            if(current->data == data)//if we found data
            {
                if(current->left==NULL || current->right==NULL)//if current has less then 2 children
                {
                    RemoveIfNo2Child(parent,current,data);
                    findAndBalance(parent->data,root);
                }
                else//if current has 2 children, current need to be switched with the next inorder node
                {
                    AvlNode<T>* switch_child=current->right;
                    AvlNode<T>* switch_parent=current;
                    if(switch_child->left==NULL)
                    {
                        switch_child->left=current->left;
                        current->right=switch_child->right;
                        switch_child->right=current;
                        switch_parent=switch_child;
                    }
                    else
                    {
                        while(switch_child->left!=NULL)
                            {
                                switch_parent=switch_child;
                                switch_child=switch_child->left;
                            }
                        switch_child->left=current->left;
                        AvlNode<T>* current_right=current->right;
                        current->right=switch_child->right;
                        switch_child->right=current_right;
                        switch_parent->left=current;
                    }
                    current->left=NULL;
                    switch_child->left_tree_height=height(switch_child->left);
                    if(parent->left!=NULL && (parent->left)->data==data)
                    {
                        parent->left=switch_child;
                        parent->left_tree_height=height(parent->left);
                    }
                    if(parent->right!=NULL && (parent->right)->data==data)
                    {
                        parent->right=switch_child;
                        parent->right_tree_height=height(parent->right);
                    }
                    RemoveIfNo2Child(switch_parent,current,data);
                    findAndBalance(switch_parent->data, root);
                }
            }
            else if((*current).data < data)
            {
                aux_remove(current,current->right,data);
            }
            else
            {
                aux_remove(current,current->left,data);
            }
        }

        /**
        * findAndBalance: find and balances the node with data in the tree
        * @param:
        *       data - the data of the vertex that need to be balanced
        *       current - the vertex of the tree we are currently on
        *       * Assumptions: comparison of T operations.   
        * Complexity: O(log(n)) (total, not for each call)
        */
        AvlNode<T>* findAndBalance(const T& data, AvlNode<T>* current)
        {
            if(current->data < data)
            {
                current->right=findAndBalance(data, current->right);
                current->right_tree_height=height(current->right);
            }
            else if(current->data > data)
            {
                current->left=findAndBalance(data, current->left);
                current->left_tree_height=height(current->left);
            }
            current=balance(current);
            return current;
        }
        
        /**
        * removeRoot: removes the root from the tree, and balance the tree afterwards
        * @param:
        *       node_to_delete - the data of the vertex that need to be removed
        *       * Assumptions: comparison of T operations.
        * Complexity: O(log(n))
        */       
        void removeRoot(const T& node_to_delete)
        {
            AvlNode<T>* temp=root;
            if(root->left==NULL || root->right==NULL)
            {
                if(root->left==NULL)
                {
                    root=root->right;
                }
                else
                {
                    root=root->left;
                }
                delete temp;
            }
            else
            {
                AvlNode<T>* switch_child=root->right;
                AvlNode<T>* switch_parent=root;
                if(switch_child->left==NULL)
                {
                    switch_child->left=root->left;
                    root->right=switch_child->right;
                    switch_child->right=root;
                    switch_parent=switch_child;
                }
                else
                {
                    while(switch_child->left != NULL)
                    {
                        switch_parent=switch_child;
                        switch_child=switch_child->left;
                    }
                    switch_child->left=root->left;
                    AvlNode<T>* current_right=root->right;
                    root->right=switch_child->right;
                    switch_child->right=current_right;
                    switch_parent->left=root;
                }
                root->left=NULL;
                root=switch_child;
                root->left_tree_height=height(root->left);
                RemoveIfNo2Child(switch_parent, temp, node_to_delete);
                findAndBalance(switch_parent->data,root);
            }
        }

        /**
        * findNode: find the node of the given data in tree
        * @param:
        *       to_find_data - the data of the vertex that need to be found
        *       current - the vertex of the tree we are currently on   
        *       * Assumptions: comparison of T operations.
        * Complexity: O(log(n))  (total, not for each call)
        */       
        AvlNode<T>* findNode(const T& to_find_data,AvlNode<T>* current) const
        {
            if(current==NULL)
            {
                return NULL;
            }
            if(current->data < to_find_data)
            {
                return findNode(to_find_data,current->right);
            }
            else if(current->data > to_find_data)
            {
                return findNode(to_find_data,current->left);
            }
            else
            {
                return current;
            }   
        }
        
        /**
        * aux_inorder: prints the tree inorder
        * @param:
        *       root - the root of the current tree
        *       * Assumptions: operation << of T.
        * Complexity: O(n)
        */      
        void aux_inorder(const AvlNode<T>* root) const
        {
            if(root==NULL)
            {
                return;
            }
            aux_inorder(root->left);
            cout << root->data << "->";
            aux_inorder(root->right);
        }

    public:
        /**
         * AVl c'tor:
         * Creates a new empty AVL tree
         */
        AvlTree() : root(NULL){}

        /**
         * AVl copy c'tor:
         * Creates a new AVL tree with the same values as other AVL tree
         */
        AvlTree(const AvlTree& other): root(NULL)
        {
            aux_copy_ctor(this, other.root);
        }

        /**
         * AVl d'tor:
         * Destroys the AVL tree
         */
        ~AvlTree()
        {
            Destructor_aux(root);
        }
        
        /**
        * getRoot: returns the root of the tree
        * return: the root of the AVL tree
        */
        AvlNode<T>* getRoot() const
        {
            return root;
        }

        /**
        * isEmpty: checks if the tree is empty
        * return: return true if the AVL tree is empty
        */
        bool isEmpty() const
        {
            return (root==NULL);
        }

        /**
        * find: finds the given data in the tree
        * @param:
        *       data - the data to search for in the AVL tree
                    * 
                    * Assumptions: comparison of T operations.
        * return: return the Node of the data if exists in the tree
        *           NULL otherwise
        * Complexity: O(log(n))
        */
        AvlNode<T>* find(const T& data) const
        {
            return findNode(data,root);
        }

        /**
        * insert: inserts the given data to the tree
        * @param:
        *       data - the data to insert to the AVL tree
                    * 
                    * Assumptions: comparison of T operations.
        * return: return the Node of the insert data if the data didn't exists in the AVL tree
        *           NULL if data already exists in the tree
        * Complexity: O(log(n))
        */
        AvlNode<T>* insert(const T& data)
        {
            AvlNode<T>* inserted = findNode(data,root);
            if(inserted)
            {
                return NULL;
            }
            AvlNode<T>* new_node = new AvlNode<T>(data);
            if (root == NULL)
            {  
                root=new_node;
                return root;
            }
            AvlNode<T>* current_root=root;
            aux_insert(current_root, new_node);
            return new_node;
        }

        /**
        * remove: remove the node with the given data from the tree
        * @param:
        *       data - the data to remove from the AVL tree
                    * 
                    * Assumptions: comparison of T operations.
        * Complexity: O(log(n))
        */
        void remove(const T& node_to_delete)
        {
            if(findNode(node_to_delete,root)==NULL)//if it doesnt exists in the tree
            {
                return;
            }
            if (root->data == node_to_delete)//if the root need to be remvoed
            {
                removeRoot(node_to_delete);
            }
            else
            {
                if(root->data < node_to_delete)
                {
                    aux_remove(root,root->right,node_to_delete);
                }
                else
                {
                    aux_remove(root,root->left,node_to_delete);
                }
            }
            return;
        }
        
                /**
        * inorder: print the data of each node in AVL tree by order of inorder
        * Complexity: O(n)
        */
        void inorder() const
        {
            AvlNode<T>* current_root=root;
            aux_inorder(current_root);
        }

        /**
        * findNextInOrder: returns the next Node after given data in tree
        * @param:
        *       data - the data we want to find the next node of
                    * 
                    * Assumptions: comparison of T operations.
        * return: return the Node of the next data inorder
        * Complexity: O(log(n))
        */
        AvlNode<T>* findNextInOrder(const T& data) const
        {
            AvlNode<T>* next_node_in_order=NULL;
            AvlNode<T>* current=root;
            if(root==NULL)
            {
                return NULL;
            }
            while(current->data != data)//find data
            {
                if(current->data < data)
                {
                    current = current->right;
                }
                else
                {
                    next_node_in_order=current;//keep the node if we went left
                    current=current->left;
                }
            }
            if (current->right == NULL)
            {
                return next_node_in_order;
            }
            else
            {
                current=current->right;
                while(current->left!=NULL)
                {
                    current=current->left;
                }
            }
            return current;
        }

        /**
        * findPreviousInOrder: returns the pevious Node after given data in tree
        * @param:
        *       data - the data we want to find the pevious node of
                    * 
                    * Assumptions: comparison of T operations.
        * return: return the Node of the pevious data inorder
        * Complexity: O(log(n))
        */
        AvlNode<T>* findPreviousInOrder(const T& data) const
        {
            AvlNode<T>* previous_node_in_order=NULL;
            AvlNode<T>* current=root;
            if(root==NULL)
            {
                return NULL;
            }
            while(current->data != data)//find data
            {
                if(current->data < data)
                {
                    previous_node_in_order=current;//keep the node if we went right
                    current = current->right;
                }
                else
                {
                    current=current->left;
                }
            }
            if (current->left == NULL)
            {
                return previous_node_in_order;
            }
            else
            {
                current=current->left;
                while(current->right!=NULL)
                {
                    current=current->right;
                }
            }
            return current;
        }



    //self check if tree is balanced
    
        bool aux_check_balance(AvlNode<T>* current)
        {
            if(current==NULL)
            {
                return true;
            }
            if((factorBalance(current)>=2) || (factorBalance(current)<=-2))
            {
                return false;
            }
            return (factorBalance(current)>=-1 && factorBalance(current)<=1 && aux_check_balance(current->right) && aux_check_balance(current->left) );
        }

        bool CheckIfBalanced()
        {
            return aux_check_balance(root);
        }
    };


#endif //AVL_H
