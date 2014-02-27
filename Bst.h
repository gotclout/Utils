#ifndef _BST_
#define _BST_

#include <iostream>
#include <stdlib.h>

#include "FifoQueue.h"

/**
 * @version 0.1
 * @since February 25, 2014
 * @class tnode
 * @brief This class implements a bst node
 */
template <class K, class V>
class tnode
{
  public:

    /** key value pair **/
    struct { K key; V value; } kv;

    /** depth of this node in tree **/
    size_t ndepth;

    /** parent node **/
    tnode* parent;

    /** left child node **/
    tnode* left;

    /** right child node **/
    tnode* right;

    /**
     * Default Constructor
     */
    tnode() { parent = left = right = 0; ndepth = 1; };

    /**
     * Construct from kv pair
     */
    tnode(const K & kk, const V & vv, tnode* p = 0, tnode* l = 0, tnode* r = 0)
    {
      kv.key   = kk;
      kv.value = vv;
      parent = p;
      left = l;
      right = 0;
    };


    /**
     * Retrieves the key
     *
     * @return const K is a ref to the key
     */
    const K& key() const
    {
      return kv.key;
    };

    /**
     * Retrieves the value
     *
     * @return const V is a ref to the value
     */
    const V& value() const
    {
      return kv.value;
    };

    /**
     * Outputstream operator overload
     *
     * @param ostream o is the output stream
     * @param tnode n is the node to render
     * @return o is the modified output stream
     */
    friend ostream& operator << (ostream & o, tnode & n)
    {
      o << "KEY:    " << n.kv.key << '\n'
        << "VALUE:  " << n.kv.value << '\n'
        << "DEPTH:  " << n.ndepth << '\n';
      if(n.left)
        cout << "LEFT:   " << n.left->key() << '\n';
      else
        cout << "LEFT:   " << "nil\n";
      if(n.right)
        cout << "RIGHT:  " << n.right->key() << '\n';
      else
        cout << "RIGHT:  " << "nil\n";
      if(n.parent)
        cout << "PARENT: " << n.parent->key() << '\n';
      else
        cout << "PARENT: " << "nil\n";
      return o;
    };
};

/**
 * @version 0.1
 * @since February 25, 2014
 * @class tree
 * @brief This class implements a bst datstructure
 */
template <typename K, typename V>
class tree
{
  private:

    typedef tnode<K, V> node;
    typedef const node cnode;

    /** num nodes in this tree**/
    size_t nodes;

    /** depth of tree **/
    size_t tdepth;

    /** root node **/
    node* root;

    /** node pointer for recursive insertions **/
    node* helper;

  public:

    /**
     * Default Constructor
     */
    tree() { root = helper = 0; nodes = 0; tdepth = 0; };

    /**
     * Retrieves the root node
     *
     * @return node* is the root of the tree
     */
    node* getRoot() const
    {
      return root;
    };

    /**
     * Retrieves the depth/height of the tres
     *
     * @return tdepth is the depth of the tree
     */
    const size_t& getDepth() const
    {
      return tdepth;
    };

    /**
     * Insert node with the specified key and value into the tree
     *
     * @param K key
     * @param V value
     * @return bool true if the kv pair is inserted, false otherwise
     */
    bool insert(const K & k, const V & v)
    {
      return insert(k, v, root);
    };

    /**
     * Insert node with the specified key and value into the tree
     *
     * @param K key
     * @param V value
     * @return bool true if the kv pair is inserted, false otherwise
     */
    bool insert(const K & k, const V & v, node* & pn)
    {
      bool retVal = true;
      node* n = 0;
      if(!pn)
      {
        n = new node(k, v);

        if(!root) root = n;
        else
        {
          n->parent = helper;
          n->ndepth = helper->ndepth + 1;
        }

        pn = n;
      }
      else if(!pn->right && !pn->left)
      {
        if(k == pn->key()) retVal = false;
        else
        {
          n = new node(k, v, pn);
          k < pn->key() ? pn->left = n : pn->right = n;
        }
      }
      else if(!pn->left && k < pn->key())
      {
        n = new node(k, v, pn);
        pn->left = n;//new node(k, v, pn);
      }
      else if(!pn->right && k > pn->key())
      {
        n = new node(k, v, pn);
        pn->right = n;//new node(k, v, pn);
      }
      else if(!pn->right && pn->left && k < pn->key() && pn != root)
      {
        if(pn->left->key() == k) retVal = false;
        else
        {
          n = new node(k, v, pn->parent);
          helper = pn;
          if(pn->parent->left == pn) pn->parent->left = n;
          else pn->parent->right = n;
          n->left = helper->left;
          n->right = helper;
          helper->left = 0;
          n->left->parent = n->right->parent = n;
        }
      }
      else if(!pn->left && pn->right && k > pn->key() && pn != root)
      {
        if(pn->right->key() == k) retVal = false;
        else
        {
          n = new node(k, v, pn->parent);
          helper = pn;
          if(pn->parent->left == pn) pn->parent->left = n;
          else pn->parent->right = n;
          n->right = helper->right;
          n->left = helper;
          helper->right = 0;
          n->right->parent = n->left->parent = n;
        }
      }
      else
      {
        helper = pn;
        if(k < pn->key()) retVal = insert(k, v, pn->left);
        else if(k > pn->key()) retVal = insert(k, v, pn->right);
        else retVal = false;
      }

      if(retVal)
      {
        ++nodes;
        if(n && n != root)
        {
          n->ndepth = n->parent->ndepth + 1;
          if(n->ndepth > tdepth) tdepth = n->ndepth;
        }
      }
      return retVal;
    };

    /**
     * Removes the specified node from the tree
     *
     * @param node* n is the node to be deleted
     * @return bool true if the specified node is deleted, false otherwise
     */
    bool deletenode(node* n)
    {
      bool retVal = true;
      node* target = findnode(n->kv.key);

      if(target)
      {
        cout << "\nTARGET NODE\n" << *n << '\n';
        if(n->ndepth < tdepth)
          --tdepth;
        if(target == root) //root node
        {
          if(target->left && target->right)
          {
            root = helper = target->left;
            while(helper->right) helper = helper->right;
            helper->right = target->right;
          }
          else if(target->left)
            root = target->left;
          else if(target->right)
            root = target->right;
          else
            root = NULL;
          if(root)
            root->parent = NULL;
        }
        else if(!target->left && !target->right) //leaf node
        {
          target->parent->right = target->parent->left = NULL;
        }
        else if(target == target->parent->right) //right child
        {
          if(target->left && target->right)
          {
            helper = target->right;
            while(helper->left) helper = helper->left;
            helper->left = target->left;
            helper->left->parent = helper;
          }
          else
          {
            helper = (target->left) ? target->left : target->right;
            helper->parent = target->parent;
            target->parent->left = helper;
          }
        }
        else if(target == target->parent->left) //left child
        {
          if(target->left && target->right)
          {
            helper = target->parent->right = target->right;
            target->right->parent = target->parent;
            while(helper->left) helper = helper->left;
            helper->left = target->left;
            target->left->parent = helper;
          }
          else
          {
            helper = (target->left) ? target->left : target->right;
            helper->parent = target->parent;
            target->parent->right = helper;
          }
        }

        delete target;
      }
      else
        retVal = false;

      return retVal;
    };

    /**
     * Retrieves the node following the specified node
     *
     * @param node* n is the specified node
     * @return node* the next node
     */
    node* nextnode(node* n)
    {
      node* next = n;

      cout << "SEARCH NODE:   " << n->kv.key << endl;

      if(next->right && next->right->kv.key > n->kv.key)
      {
        next = next->right;
        while(next->left && next->left->kv.key < n->kv.key) next = next->left;
      }
      else if(next->parent)
      {
        next = next->parent;
        while(next != root && next->kv.key < n->kv.key) next = next->parent;
      }

      if(next == root)
        cout << "NEXT NODE: " << next->kv.key << endl;
      else if(next)
        cout << "NEXT NODE: " << next->kv.key << endl;
      return next;
    };

    /**
     * Retrieves the node prior to the specified node
     *
     * @param node* n is the specified node
     * @return node* the previous node
     */
    node* prevnode(node* n)
    {
      node* next = n;

      cout << "SEARCH NODE:   " << n->kv.key << endl;

      if(next->right && next->right->kv.key < n->kv.key)
      {
        next = next->right;
        while(next->left && next->left->kv.key > n->kv.key) next = next->left;
      }
      else if(next->parent)
      {
        next = next->parent;
        while(next != root && next->kv.key > n->kv.key) next = next->parent;
      }

      if(next == root)
        cout << "PREV NODE: " << next->kv.key << endl;
      else if(next)
        cout << "PREV NODE: " << next->kv.key << endl;
      return next;
    };

    /**
     * Locates a node with the specified key in a tree
     *
     * @parm k is the key for the specified node
     * @return tnode* a pointer to the specified node, null if not found
     */
    node* findnode(K & k)
    {
      cout << "FIND NODE: " << k << endl;

      int i = 1;
      node* cur = root;

      while(cur)
      {
        if(k == cur->kv.key)
        {
          cout << "LOOKUPS: " << i << "\n\n";
          return cur;
        }
        else if(k < cur->kv.key)
          cur = cur->left;
        else
          cur = cur->right;
        ++i;
      }

      return NULL;
    };

    /**
     * Renders the breadth first traversal of a tree
     */
    void printbfs()
    {
      queue <node*> q;
      node* tcur = root;

      if(root)
      {
        q.enqueue(tcur);
        qnode<node*>* lcur = q.front();

        while(lcur)
        {
          if(tcur->left)
            q.enqueue(tcur->left);
          if(tcur->right)
            q.enqueue(tcur->right);
          lcur = lcur->next;
          if(lcur)
            tcur = (node*)lcur->data;
        }

        while(!q.empty())
          cout << "BFS NODE\n" << *(q.dequeue()) << '\n';
      }
    };

    /**
     * Renders the pre order traversal of a tree
     *
     * @param node* n is a pointer to the current node
     */
    void printpreorder(node* n) const
    {
      if(n)
      {
        cout << "NODE VALUE: " << n->kv.value << endl;
        if(n->left)
          printpreorder(n->left);
        if(n->right)
          printpreorder(n->right);
      }
    };

    /**
     * Renders the in order traversal of a tree
     *
     * @param node* n is a pointer to the current node
     */
    void printinorder(node* n) const
    {
      if(n)
      {
        if(n->left)
          printinorder(n->left);
        cout << "NODE VALUE: " << n->kv.value << endl;
        if(n->right)
          printinorder(n->right);
      }
    };

    /**
     * Renders the post order traversal of a tree
     *
     * @param node* n is a pointer to the current node
     */
    void printpostorder(node* n) const
    {
      if(n)
      {
        if(n->left)
          printinorder(n->left);
        if(n->right)
          printinorder(n->right);
        cout << "NODE VALUE: " << n->kv.value << endl;
      }
    };

    /**
     * Retrieve the tree size
     *
     * @return const size_t is the number of nodes in the tree
     */
    const size_t& size() const
    {
      return nodes;
    }

    /**
     * Indicates whether or not the tree is empty
     *
     * @return bool true is the tree has 0 nodes, false otherwise
     */
    bool empty()
    {
      return nodes == 0;
    }

    /**
     * Destructor
     */
    ~tree()
    {
      while(root)
      {
        deletenode(root);
        cout << "TREE DEPTH: " << tdepth << endl;
      }
    };
};
#endif//_BST_

