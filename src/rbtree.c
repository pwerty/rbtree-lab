#include "rbtree.h"
#include <assert.h>
#include <stdlib.h>
void rotateLeft(rbtree *t, node_t *node)
{
  node_t *other = node->right;
  node->right = other->left;

  if(other->left != t->nil)
    other->left->parent = node;

  other->parent = node->parent;
  if(node->parent == t->nil)
      t->root = other;
  else if(node == node->parent->left)
    node->parent->left = other;
  else
    node->parent->right = other;

  other->left = node;
  node->parent = other;
}

void rotateRight(rbtree *t, node_t *node)
{
  node_t *other = node->left;
  node->left = other->right;
  if(other->right != t->nil)
  other->right->parent = node;

  other->parent = node->parent;

  if(node->parent == t->nil)
    t->root = other;
  else if(node == node->parent->right)
    node->parent->right = other;
  else
    node->parent->left = other;

    other->right = node;
    node->parent = other;

}


void insertFixup(rbtree *t, node_t *z)
{
  node_t *y;
  while (z->parent->color == RBTREE_RED)
  {
    if(z->parent == z->parent->parent->left)
    {
      y = z->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else 
      {
        if (z == z->parent->right)
        {
          z = z->parent;
          rotateLeft(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rotateRight(t, z->parent->parent);
      }

    }
    else
    {
      y = z->parent->parent->left;

      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          rotateRight(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rotateLeft(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *blank = (node_t *)malloc(sizeof(node_t));
  blank->color = RBTREE_BLACK;
  blank->key = 500;
  blank->left = blank;
  blank->parent = blank;
  blank->right = blank;
 
  // TODO: initialize struct if needed

  p->nil = blank;
  p->root = blank;

  return p;
}

void delete_perNode(node_t *node, node_t *nil)
{
  if(node == nil)
    return;
  delete_perNode(node->left, nil);
  delete_perNode(node->right, nil);
  free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_perNode(t->root, t->nil);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // newNode는 key를 포함한 책에서의 z를 가리킴
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *newNode = malloc(sizeof(node_t));
  newNode->key = key;

  newNode->parent = t->nil;

  while(x != t->nil)
  {
    y = x;
    if (newNode->key < y->key)
      x = x->left;
    else 
      x = x->right;
  }
  newNode->parent = y;
  
  if(y == t->nil)
    t->root = newNode;
  else if (newNode->key < y->key)
    y->left = newNode;
  else
    y->right = newNode;
  
    newNode->left = t->nil;
    newNode->right = t->nil;
    newNode->color = RBTREE_RED;

  insertFixup(t, newNode);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {

   node_t *cur = t->root;

  while(cur != t->nil)
  {
    if(cur->key == key)
        return cur;
    else if(cur->key > key)
      cur = cur->left;
    else
      cur = cur->right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *prev = NULL;
  node_t *cur = t->root;

  while(cur != t->nil)
  {
    prev = cur;
    cur = cur->left;
  }
  
  return prev;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *prev = NULL;
  node_t *cur = t->root;

  while(cur != t->nil)
  {
    prev = cur;
    cur = cur->right;
  }
  
  return prev;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;

   v->parent = u->parent;

}

void rbtree_erase_fixup(rbtree *t, node_t *x)
{
  

  node_t *w;

  while(x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      w = x->parent->right;
    
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotateLeft(t, x->parent);
        w = x->parent->right;
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else 
      {
          if (w->right->color == RBTREE_BLACK)
          {
            w->left->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            rotateRight(t, w);
            w = x->parent->right;
          }

          w->color = x->parent->color;
          x->parent->color = RBTREE_BLACK;
          w->right->color = RBTREE_BLACK;
          rotateLeft(t, x->parent);
          x = t->root;
      }
    }
    else
    {
      w = x->parent->left;
    
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotateRight(t, x->parent);
        //
        w = x->parent->left;

      }
  
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {

        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
            if (w->left->color == RBTREE_BLACK)
            {
              w->right->color = RBTREE_BLACK;
              w->color = RBTREE_RED;
              rotateLeft(t, w);
              w = x->parent->left;
            }
            w->color = x->parent->color;
            x->parent->color = RBTREE_BLACK;
            w->left->color = RBTREE_BLACK;
            rotateRight(t, x->parent);
            x = t->root;
      }

    }
}
x->color = RBTREE_BLACK;
}

node_t *rbtree_min_node(const rbtree *t, node_t *str)
{
    while (str->left != t->nil)
    {
      str = str->left;
    }
    return str;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  // 특정한 노드를 지운다. 그럼 특정한 노드를 찾는게 먼저이지?
  node_t *q = p;
  node_t *tmp = NULL;
  color_t qBeforeColor = q->color;
  if(p->left == t->nil)
  {
    tmp = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if(p->right == t->nil)
  {
    tmp = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else
  {
    q = rbtree_min_node(t, p->right);
    // q는 Successor를 찾아온다. 즉, 큰 목록 중 가장 작은 것.
    qBeforeColor = q->color;
    tmp = q->right;
    // 근데 이러면 tmp가 nil이 될텐데..

    if(tmp != t->nil && q->parent == p)
    {
        tmp->parent = q;
    }
    else  
    {
      rbtree_transplant(t, q, q->right);
      q->right = p->right;
      q->right->parent = q;
    }
   rbtree_transplant(t, p, q);
   q->left = p->left;
   q->left->parent = q;
   q->color = p->color;
  }

  if (qBeforeColor == RBTREE_BLACK)
    rbtree_erase_fixup(t, tmp);
  free(p);
  return 0;
}

void callout(node_t *node, key_t *arr, const size_t n, size_t *m, node_t *nil)
{
  if(node == nil || *m >= n) return;
  callout(node->left, arr, n, m, nil);
  if (n > (*m))
    arr[(*m)++] = node->key;
  callout(node->right, arr, n, m, nil);
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t count = 0;
  callout(t->root, arr, n, &count, t->nil);
  return 0;
}
