#include "rbtree.h"
#include <stdlib.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = calloc(1, sizeof(node_t));
  p->nil->color = 1;
  p->nil->parent = p->nil;
  p->nil->left = p->nil;
  p->nil->right = p->nil;
  
  // root, nill 초기화
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  //루트를 제거하고 그 메모리를 제거하는 로직
  node_t *tmpnode; 
  while(!(t->root==t->nil)){
    node_t *minNode = t->root->right; 
    if(t->root->left == t->nil){
      tmpnode = t->root->right;
      tmpnode->parent = t->nil;
      free(t->root);
      t->root = tmpnode;
    }else if(t->root->right == t->nil){
      tmpnode = t->root->left;
      tmpnode->parent = t->nil;
      free(t->root);
      t->root = tmpnode;
    }else{
      while(minNode->left != t->nil){
        minNode = minNode->left;
      }
      if(minNode != t->root->right){
        minNode->parent->left = minNode->right;
        minNode->right->parent = minNode->parent;
        minNode->right = t->root->right;
        minNode->right->parent = minNode;
      }
      minNode->parent = t->nil;
      minNode->left = t->root->left;
      minNode->parent = minNode;
      free(t->root);
      t->root = minNode;
    }
  }
  free(t->root);
  //free(t->nil);
  //구조체 메모리 해제
  free(t);
}

void rotation_left(rbtree *t, node_t *cur){
  //****좌회전 구현****두 노드의 부모와 left right 조정
  //회전용 노드 설정
  node_t *rotation = cur->right;
  //부모노드(cur)의 right에 삽입노드(rotation)의 좌측 서브트리 인계 설정
  cur->right = rotation->left;
  //삽입노드(rotation)의 좌측이 nil이 아닐 경우만  
  //(부모가 nil 인 경우는 ROOT 외에 없기 때문에 조건문 처리)
  if(rotation->left != t->nil){
    //삽입노드(rotation)의 좌측의 부모를 부모노드(cur)로 지정 서브트리 인계완료
    rotation->left->parent = cur;
  }
  //삽입노드(rotation)의 부모를 부모노드의 부모와 연결
  rotation->parent = cur->parent;
  //부모가 nil 인 경우 부모노드(cur)가 ROOT임으로 삽입노드(rotation)를 ROOT로 지정 
  if(cur->parent == t->nil){
    t->root = rotation;
  }
  //부모노드(cur)가 조부모의 왼쪽에 있는 노드일 경우 
  else if(cur == cur->parent->left){
    cur->parent->left = rotation;
  } 
  else{
  //부모노드(cur)가 조부모의 오른쪽에 있는 노드일 경우 
    cur->parent->right = rotation;
  }
  // 부모노드(cur)를 삽입노드(rotation) 왼쪽에 배치
  rotation->left = cur;
  // 부모노드(cur)의 부모를 삽입노드(rotation)로 설정
  cur->parent = rotation;
}

void rotation_right(rbtree *t, node_t *cur){
  //****우회전 구현****두 노드의 부모와 left right 조정
  //회전용 노드 설정
  node_t *rotation = cur->left;
  //부모노드(cur)의 left에 삽입노드(rotation)의 우측 서브트리 인계 설정
  cur->left = rotation->right;
  //삽입노드(rotation)의 우측이 nil이 아닐 경우만  
  //(부모가 nil 인 경우는 ROOT 외에 없기 때문에 조건문 처리)
  if(rotation->right != t->nil){
    //삽입노드(rotation)의 우측의 부모를 부모노드(cur)로 지정 서브트리 인계완료
    rotation->right->parent = cur;
  }
  //삽입노드(rotation)의 부모를 부모노드의 부모와 연결
  rotation->parent = cur->parent;
  //부모가 nil 인 경우 부모노드(cur)가 ROOT임으로 삽입노드(rotation)를 ROOT로 지정 
  if(cur->parent == t->nil){
    t->root = rotation;
  }
  //부모노드(cur)가 조부모의 왼쪽에 있는 노드일 경우 
  else if(cur == cur->parent->left){
    cur->parent->left = rotation;
  } 
  else{
  //부모노드(cur)가 조부모의 오른쪽에 있는 노드일 경우 
    cur->parent->right = rotation;
  }
  // 부모노드(cur)를 삽입노드(rotation) 왼쪽에 배치
  rotation->right = cur;
  // 부모노드(cur)의 부모를 삽입노드(rotation)로 설정
  cur->parent = rotation;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // key 셋팅할 노드 생성
  node_t *inNode = calloc(1, sizeof(node_t));
  // 트리 순회 노드 생성
  node_t *cur;
  inNode->key = key;
  inNode->color = 0;
  inNode->left = t->nil;
  inNode->right = t->nil;
  inNode->parent = t->nil;
  //루트가 없는 경우 루트 설정
  if(t->root == t->nil){
    //루트는 부모는 nill
    inNode->parent = t->nil;
    //루트는 무조건 검정 : 1
    inNode->color = 1;
    //루트 설정
    t->root = inNode;

    return t->root;
  }
  // 현재 노드 설정
  cur = t->root;
  while(1){
    // 키 값 비교 후 좌/우 중 하위 노드로 이동
    if(cur->key > inNode->key){
      // 다음 노드가 NULL인 경우 정지
      if(cur->left == t->nil){
        //삽입노드 부모 설정
        inNode->parent = cur;
        //다음 노드에 자신을 추가
        cur->left = inNode;
        //커서를 삽입한 노드로 이동
        cur = cur->left;
        break;
      }
      cur = cur->left;
    }
    else{
      // 다음 노드가 NULL인 경우 정지 
      if(cur->right == t->nil){
        //삽입노드 부모 설정
        inNode->parent = cur;
        //다음 노드에 자신을 추가
        cur->right = inNode;
        //커서를 삽입한 노드로 이동
        cur = cur->right;
        break;
      }
      cur = cur->right;
    }
  }
  //삼촌 노드 생성
  node_t * ppchild;
  // ****현재 노드 재정렬****
  // 부모색이  RED 인 경우 레드의 자식은 레드일 수 없는 특성이 깨짐으로 재조정 처리
  while(cur->parent->color == 0){
    //****현재 노드가 조부모의 좌측 서브트리인 경우
    if(cur->parent->parent->left == cur->parent){
      // 삼촌 지정
      ppchild = cur->parent->parent->right;
      // 삼촌 색깔이 레드인 경우 검증 
      if(ppchild->color == 0){
      // 5번 조건을 사용 색을 변경
        cur->parent->color = 1;
        ppchild->color = 1;
        cur->parent->parent->color = 0;
        //커서는 조부모로 이동
        cur = cur->parent->parent;
      }
      //삼촌이 블랙일 때
      else{
        // 현재노드가 좌측서브트리이고 안쪽으로 꺽였을 때
        if(cur == cur->parent->right){
          //부모노드로 커서 이동
          cur = cur->parent;
          rotation_left(t, cur);
        }
        //4, 5번 조건을 위해 회전 시 색 변경
        cur->parent->color = 1;
        cur->parent->parent->color = 0;
        //조부모 노드로 커서 이동
        cur = cur->parent->parent;
        rotation_right(t, cur);
      }
    }
    //****현재 노드가 조부모의 우측 서브트리인 경우(좌측의 좌우 반대로 동작)
    else{
      // 삼촌 지정
      ppchild = cur->parent->parent->left;
      // 삼촌 색깔이 레드인 경우 검증 
      if(ppchild->color == 0){
      // 5번 조건을 사용 색을 변경
        cur->parent->color = 1;
        ppchild->color = 1;
        cur->parent->parent->color = 0;
        //커서는 조부모로 이동
        cur = cur->parent->parent;
      }
      //삼촌이 블랙일 때
      else{
        // 현재노드가 우측서브트리이고 안쪽으로 꺽였을 때
        if(cur == cur->parent->left){
          //부모노드로 커서 이동
          cur = cur->parent;
          rotation_right(t, cur);
        }
        //4, 5번 조건을 위해 회전 시 색 변경
        cur->parent->color = 1;
        cur->parent->parent->color = 0;
        
        //조부모 노드로 커서 이동
        cur = cur->parent->parent;
        rotation_left(t, cur);
      }
    }
  }
  t->root->color = 1;
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur = t->root;
  while(1){
    // 키 값 비교 후 좌/우 중 하위 노드로 이동
    if(cur->key >= key){
      if(cur == t->nil){
        // cur = NULL;
        return NULL;
      }
      if(cur->key == key){
        return cur;
      } 
      // 다음 노드가 NULL인 경우 정지
      if(cur->left == t->nil){
        // cur = NULL;
        return NULL;
      }
      cur = cur->left;
    }
    else{
      if(cur == t->nil){
        // cur = NULL;
        return NULL;
      }
      // 다음 노드가 NULL인 경우 정지 
      if(cur->right == t->nil){
        // cur = NULL;
        return NULL;
      }
      cur = cur->right;
    }
  }
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *minNode = t->root;
  while(minNode->left != t->nil){
    minNode = minNode->left;
  }
  return minNode;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *maxNode = t->root;
  while(maxNode->right != t->nil){
    maxNode = maxNode->right;
  }
  return maxNode;
}

void rbtree_relocation(rbtree* t, node_t* remove, node_t* move){
  //삭제대상이 루트인 경우
  if(remove->parent == t->nil){
    t->root = move;
  }
  else if(remove == remove->parent->left){
  //삭제대상의 부모 연결, 왼쪽 노드인 경우
    remove->parent->left = move;
  }
  else{
  //삭제대상의 부모 연결, 오른쪽 노드인 경우
    remove->parent->right = move;
  }
  //삭제 대상의 부모노드 설정 변경
  move->parent = remove->parent;
}

void rbtree_del_fixed(rbtree* T, node_t* x){
  //형제 검증 노드 생성
  node_t* w;
  //노드가 루트가 아니고 현재 노드 색이 검정일 때
  // 노드일때 종료, 혹은 Red and black 일때 종료
  while(x != T->root && x->color == 1){
    //노드가 부모의 왼쪽에 있을때
    if(x == x->parent->left){
      //형제 검증 노드 설정
      w = x->parent->right;
      //형제 검증 노드가 빨강 => case 1
      if(w->color == 0){
        //색 변경
        w->color = 1;
        //부모색 변경
        x->parent->color = 0;
        //돌리기
        rotation_left(T,x->parent);
        w = x->parent->right;
      }
      //case1 이후 검증노드는 검정색
      //형제 검증 노드의 왼쪽,오른쪽이 블랙일때 => case 2
      if(w->left->color == 1 && w->right->color == 1){
        w->color = 0;
        x = x->parent;
        
      }
      //case 3, 4 수행
      else {
      if(w->right->color == 1){
        w->left->color = 1;
        w->color = 0;
        rotation_right(T,w);
        w = x->parent->right;
      }
      w->color = x->parent->color;
      x->parent->color = 1;
      w->right->color = 1;
      rotation_left(T,x->parent);
      x = T->root;
    }
    }
    //노드가 부모의 오른쪽에 있을때
    else{
      //형제 검증 노드 설정
      w = x->parent->left;
      //형제 검증 노드가 빨강 => case 1
      if(w->color == 0){
        //색 변경
        w->color = 1;
        //부모색 변경
        x->parent->color = 0;
        //돌리기
        rotation_right(T,x->parent);
        w = x->parent->left;
      }
      //case1 이후 검증노드는 검정색
      //형제 검증 노드의 왼쪽,오른쪽이 블랙일때 => case 2
      if(w->right->color == 1 && w->left->color == 1){
        w->color = 0;
        x = x->parent;
        
      }
      //case 3, 4 수행
      else {
      if(w->left->color == 1){
        w->right->color = 1;
        w->color = 0;
        rotation_left(T,w);
        w = x->parent->left;
      }
      w->color = x->parent->color;
      x->parent->color = 1;
      w->left->color =1;
      rotation_right(T,x->parent);
      x = T->root;
    }
    }
  }
  x->color = 1;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  //삭제 대상 정보 저장
  node_t *remove;
  color_t remove_org_color;
  //삭제 후 삭제 위치로 이동 발생 대상
  node_t *move;
  remove = p;
  remove_org_color = remove->color;

  //삭제 대상의 하위 노드가 오른쪽 노드 하나인 경우 => 삭제
  if(p->left == t->nil){
    // 이동 대상 설정
    move = p->right;
    // 이동 수행
    rbtree_relocation(t,p,p->right);
  }
  //삭제 대상의 하위 노드가 왼쪽 노드 하나인 경우
  else if(p->right == t->nil){
    // 이동 대상 설정
    move = p->left;
    // 이동 수행
    rbtree_relocation(t,p,p->left);
  }
  //하위 노드가 둘 다 있는 경우 => 삭제 노드를 제거하고 후임자가 차지
  else{
    //후임자 탐색
    node_t* minNode = p->right;
    while(minNode->left != t->nil){
      minNode = minNode->left;
    }
    //후임자를 설정
    remove = minNode;
    //후임자의 컬러 저장
    remove_org_color = remove->color;
    //후임자의 우측을 움직일 노드(커서)로 설정
    move = remove->right;
    //후임자의 부모가 삭제노드인 경우
    if(remove->parent == p){
      //움직일 노드의 부모를 후임자로 설정
      //이미 설정되있지 않나?? => nil 일때 상정? 필요할 것 같음***
      move->parent = remove;
    }
    //후임자의 부모가 삭제노드가 아닌 경우 
    //=> 후임자의 오른쪽과 후임자 부모노드와 연결 rbtree_relocation
    //=> 후임자와 삭제노드와의 연결
    else{
      //후임자 삭제 처리 및 재연결
      rbtree_relocation(t,remove,remove->right);
      // 후임자의 우측이, 삭제 노드의 right 연결
      remove->right = p->right;
      //우측 부모를 후임자와 연결
      remove->right->parent = remove;
    }
    //삭제 노드와 후임자 교환
    rbtree_relocation(t,p,remove);
    remove->left = p->left;
    remove->left->parent = remove;
    remove->color = p->color;
  }

  free(p);

  if(remove_org_color == 1){
    rbtree_del_fixed(t, move);
  }
  return 0;
}

void rbtree_to_array_recursion(int *cnt, node_t *node, key_t *arr, size_t n, node_t *nil){
  if(*cnt>n || node == nil)
    return;
  rbtree_to_array_recursion(cnt, node->left, arr, n, nil);
  if(node != nil){
    *(arr+*cnt) = node->key;
    *cnt += 1;
  }
  rbtree_to_array_recursion(cnt, node->right, arr, n, nil);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int cnt = 0;
  int *cnt_add = &cnt;
  rbtree_to_array_recursion(cnt_add, t->root, arr, n, t->nil);
  return 0;
}



// // 트리를 출력하는 함수
// void print_rbtree(rbtree *t, node_t *node, int space)
// {
//   if (node == t->nil)
//     return;

//   space += 10;
//   print_rbtree(t, node->right, space);

//   printf("\n");
//   for (int i = 10; i < space; i++)
//     printf(" ");
//   printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");

//   print_rbtree(t, node->left, space);
// }

// int main(){
//   rbtree *t = new_rbtree();
//   int key;
//   printf("노드를 삽입하려면 키 값을 입력하세요 (음수를 입력하면 종료):\n");
//   while (scanf("%d", &key) && key != 999)
//   {
//     printf("노드를 삽입하려면 키 값을 입력하세요 (음수를 입력하면 종료):\n");
//     rbtree_insert(t, key);
//   }
//   print_rbtree(t, t->root, 0);
//   printf("노드 삭제 (음수를 입력하면 종료):\n");
//   while(scanf("%d", &key) && key != 999){
//     printf("노드 삭제 (음수를 입력하면 종료):\n");
//     node_t *p = rbtree_find(t, key);
//     rbtree_erase(t, p);
//     print_rbtree(t, t->root, 0);
//   }
// }

// int main(){
//   rbtree *t = new_rbtree();
//   rbtree_insert(t , 1227918265);
//   rbtree_insert(t , 3978157);
//   rbtree_insert(t , 263514239);
//   rbtree_insert(t , 1969574147);
//   const key_t arr[] = {1227918265, 3978157, 263514239, 1969574147};
//   int length = sizeof(arr) / sizeof(arr[0]);
//   for (int i = 0; i < length; i++) {
//     node_t *p = rbtree_find(t, arr[i]);
//     rbtree_erase(t, p);
//     print_rbtree(t, t->root, 0);
//   }
//   for (int i = 0; i < length; i++) {
//     node_t *p = rbtree_find(t, arr[i]);
//     printf("%d \n", (p = NULL));
//   }
//   delete_rbtree(t);
// }
