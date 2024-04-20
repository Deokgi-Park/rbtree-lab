#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // nil 노드 초기화 진행
  node_t *inNode = calloc(1,sizeof(node_t));
  inNode->color = 0;
  inNode->parent = NULL;
  inNode->left = NULL;
  inNode->right = NULL;
  
  // root, nill 초기화
  p->root=NULL;
  p->nil=inNode;
  
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // key 셋팅할 노드 생성
  node_t *inNode = calloc(1,sizeof(node_t));
  // 트리 순회 노드 생성
  node_t *cur;
  inNode->key = key;
  inNode->color = 0;
  inNode->parent = t->nil;
  inNode->left = NULL;
  inNode->right = NULL;

  //루트가 없는 경우 루트 설정
  if(t->root == NULL){
    t->root = inNode;
    //루트는 무조건 검정 : 1
    t->root->color = 1;
    
    return t->root;
  }

  // 현재 노드 설정
  cur = t->root;
  while(1){
    if(cur->key > inNode->key){
      if(cur->left == NULL){
        inNode->parent = cur;
        cur->left = inNode;
        cur = cur->left;
        break;
      }
      cur = cur->left;
    }
    else{
      if(cur->right == NULL){
        inNode->parent = cur;
        cur->right = inNode;
        cur = cur->right;
        break;
      }
      cur = cur->right;
    }
  }
  
  while(cur->parent->color == 1){
    if(cur->parent->left == cur){

    }
    else{

    }

  }

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

// int main(){
//   rbtree *t = new_rbtree();
//   rbtree_insert(t , 50);
//   rbtree_insert(t , 10);
//   rbtree_insert(t , 70);
//   if(t != NULL)
//     printf("%d", t->root->color);
// }