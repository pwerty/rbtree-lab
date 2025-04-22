#include "rbtree.h"

#include <stdlib.h>
/*
다음 기능들을 수행할 수 있도록 RB tree를 구현합니다.!!

tree = new_tree(): RB tree 구조체 생성

여러 개의 tree를 생성할 수 있어야 하며 각각 다른 내용들을 저장할 수 있어야 합니다.
delete_tree(tree): RB tree 구조체가 차지했던 메모리 반환

해당 tree가 사용했던 메모리를 전부 반환해야 합니다. (valgrind로 나타나지 않아야 함)
tree_insert(tree, key): key 추가

구현하는 ADT가 multiset이므로 이미 같은 key의 값이 존재해도 하나 더 추가 합니다.
ptr = tree_find(tree, key)

RB tree내에 해당 key가 있는지 탐색하여 있으면 해당 node pointer 반환
해당하는 node가 없으면 NULL 반환
tree_erase(tree, ptr): RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환

ptr = tree_min(tree): RB tree 중 최소 값을 가진 node pointer 반환

ptr = tree_max(tree): 최대값을 가진 node pointer 반환

tree_to_array(tree, array, n)

RB tree의 내용을 key 순서대로 주어진 array로 변환
array의 크기는 n으로 주어지며 tree의 크기가 n 보다 큰 경우에는 순서대로 n개 까지만 변환
array의 메모리 공간은 이 함수를 부르는 쪽에서 준비하고 그 크기를 n으로 알려줍니다.
*/
void rotateLeft(rbtree *t, node_t *node)
{
  node_t *otherNode = node->right;
  node->right = otherNode->left;

  if(otherNode->left != t->nil)
    otherNode->left->parent = node;

  otherNode->parent = node->parent;
  if(node->parent == t->nil)
    t->root = otherNode;
  else if(node == node->parent->left)
    node->parent->left = otherNode;
  else
    node->parent->right = otherNode;

  otherNode->left = node;
  node->parent = otherNode;
}

void rotateRight(rbtree *t, node_t *node)
{
  node_t *otherNode = node->left;
  node->left = otherNode->right;
  if(otherNode->right != t->nil)
    otherNode->right->parent = node;

  otherNode->parent = node->parent;

  if(node->parent == t->nil)
    t->root = otherNode;
  else if(node == node->parent->left)
    node->parent->left = otherNode;
  else
    node->parent->right = otherNode;

  otherNode->right = node;
  node->parent = otherNode;
}


void insertFixup(rbtree *t, node_t *node)
{
  node_t *tmp;
  while (node->parent->color == RBTREE_RED)
  {
    if(node->parent == node->parent->parent->left)
    {
      tmp = node->parent->parent->right;
      if (tmp != t->nil && tmp->color == RBTREE_RED)
      {
        node->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else if (node == node->parent->right)
      {
        node = node->parent;
        rotateLeft(t, node);
      }

      if(node == t->root)
        break;

      node->parent->color = RBTREE_BLACK;
      node->parent->parent->color = RBTREE_RED;
      rotateRight(t, node->parent->parent);
    }
    else
    {
      tmp = node->parent->parent->left;

      if (tmp != t->nil && tmp->color == RBTREE_RED)
      {
        node->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else if (node == node->parent->left)
      {
        node = node->parent;
        rotateRight(t, node);
      }

      if(node == t->root)
        break;


        node->parent->color = RBTREE_BLACK;
      node->parent->parent->color = RBTREE_RED;
      rotateLeft(t, node->parent->parent);
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

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *prev = t->nil;
  node_t *cur = t->root;
  node_t *newNode = malloc(sizeof(node_t));
  newNode->key = key;
  cur = t->root;

  while(cur != t->nil)
  {
    prev = cur;
    if (newNode->key < cur->key)
      cur = cur->left;
    else
      cur = cur->right;
  }
  newNode->parent = prev;
  if(prev == t->nil)
    t->root = newNode;
  else if (newNode->key < prev->key)
    prev->left = newNode;
  else
    prev->right = newNode;
  
  newNode->left = t->nil;
  newNode->right = t->nil;
  newNode->color = RBTREE_RED;
  insertFixup(t, newNode);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {

  // This RBTree can apply multiply nodes even same key value.
  node_t *cur = t->root;

  while(cur != t->nil)
  {
    if(cur->key == key)
      // 키 값이 같아도 좌우가 다를 수 있다. 이것까지는 고려하기
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
    else if (w->right->color == RBTREE_BLACK)
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
    else
    {
      w = x->parent->left;
    
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotateRight(t, x->parent);
        w = x->parent->right;
      }
  
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else if (w->left->color == RBTREE_BLACK)
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
x->color = RBTREE_BLACK;
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
    // rbtree_min의 호출 양식을 위해 새로운 트리를 정의하여 매개변수로 보내는 선택을 하겠다.
    rbtree *tmpTree = new_rbtree();
    tmpTree->root = p->right;
    q = rbtree_min(tmpTree);
    qBeforeColor = q->color;
    tmp = q->right;
    if(q->parent == p)
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
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  // 중위 순회를 조져서 하나씩 채워야함
  // left - cur - right 순서로 진행, 뭔소리임???
  return 0;
}
