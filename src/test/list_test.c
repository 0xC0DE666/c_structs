#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_structs.h"
#include "utils.h"

// ####################
// list_node_new
// ####################
Test(list_node_new, _1) {
  ListNode* node = list_node_new(point_new(0, 1)).ok;

  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);
  cr_assert_eq(node->next, NULL);
  cr_assert_eq(node->previous, NULL);

  Point* p = node->value;
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 1);

  list_node_free(&node, (FnFree) safe_free);
}

// ####################
// list_node_free
// ####################
Test(list_node_free, _1) {
  ListNode* node = list_node_new(point_new(0, 1)).ok;

  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);

  int e = list_node_free(&node, (FnFree) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(node, NULL);
}

// ####################
// list_size
// ####################
Test(list_size, _1) {
  List* list = list_new().ok;
  int sze = list_size(list);
  cr_assert_eq(sze, 0);

  for (int i = 0; i < 3; ++i) {
    list_append(list, point_new(i, i));
    sze = list_size(list);
    cr_assert_eq(sze, i + 1);
  }

  list_free(&list, (FnFree) safe_free);
}

// ####################
// list_new
// ####################
Test(list_new, _1) {
  List* list = list_new().ok;

  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  list_free(&list, NULL);
}

// ####################
// list_clear
// ####################
Test(list_clear, empty) {
  List* list = list_new().ok;
  int e = list_clear(list, NULL);

  cr_assert_eq(e, 0);
  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  list_free(&list, NULL);
}

Test(list_clear, single) {
  List* list = list_new().ok;
  list->head = list_node_new(point_new(0, 0)).ok;
  list->tail = list->head;

  int e = list_clear(list, (FnFree) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  list_free(&list, (FnFree) safe_free);
}

// TODO: check this, check a b c is freed
Test(list_clear, multiple) {
  List* list = list_new().ok;
  ListNode* a = list_node_new(point_new(0, 0)).ok;
  ListNode* b = list_node_new(point_new(1, 1)).ok;
  ListNode* c = list_node_new(point_new(2, 2)).ok;

  a->next = b;
  b->previous = a;
  b->next = c;
  c->previous = b;
  list->head = a;
  list->tail = c;

  int e = list_clear(list, (FnFree) safe_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  list_free(&list, (FnFree) safe_free);
}


// ####################
// list_free
// ####################
Test(list_free, _1) {
  List* list = list_new().ok;
  ListNode* a = list_node_new(point_new(0, 0)).ok;
  ListNode* b = list_node_new(point_new(1, 1)).ok;
  ListNode* c = list_node_new(point_new(1, 1)).ok;

  a->next = b;
  b->previous = a;
  b->next = c;
  c->previous = b;
  list->head = a;
  list->tail = c;

  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, a);
  cr_assert_eq(list->tail, c);

  list_free(&list, (FnFree) safe_free);
  cr_assert_eq(list, NULL);
}

// ####################
// list_append
// ####################
Test(list_append, empty) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);
  cr_assert_eq(list->head, list->tail);

  Point* p = list->head->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  list_free(&list, (FnFree) safe_free);
}

Test(list_append, not_empty) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  int sze = list_size(list);
  cr_assert_eq(sze, 3);

  ListNode* n = list->head;
  int i = 0;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    ++i;
    n = n->next;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    --i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

// ####################
// list_prepend
// ####################
Test(list_prepend, empty) {
  List* list = list_new().ok;

  int e = list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);
  cr_assert_eq(list->head, list->tail);

  Point* p = list->head->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  list_free(&list, (FnFree) safe_free);
}

Test(list_prepend, not_empty) {
  List* list = list_new().ok;

  int e = list_prepend(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  e = list_prepend(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  int sze = list_size(list);
  cr_assert_eq(sze, 3);

  ListNode* n = list->head;
  int i = 0;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    ++i;
    n = n->next;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    --i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

// ####################
// list_insert_before
// ####################
Test(list_insert_before, before_head) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(1, 1));
  e = list_insert_before(list, list->head, point_new(0, 0));
  cr_assert_eq(e, 0);
  e = list_append(list, point_new(2, 2));

  int sze = list_size(list);
  cr_assert_eq(sze, 3);

  ListNode* n = list->head;
  int i = 0;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    ++i;
    n = n->next;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    --i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

Test(list_insert_before, before_mid_node) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  e = list_append(list, point_new(2, 2));
  e = list_insert_before(list, list->head->next, point_new(1, 1));
  cr_assert_eq(e, 0);
  e = list_append(list, point_new(3, 3));

  int sze = list_size(list);
  cr_assert_eq(sze, 4);

  ListNode* n = list->head;
  int i = 0;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    ++i;
    n = n->next;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    --i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

// ####################
// list_insert_after
// ####################
Test(list_insert_after, after_tail) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  e = list_insert_after(list, list->tail, point_new(1, 1));
  cr_assert_eq(e, 0);
  e = list_append(list, point_new(2, 2));

  int sze = list_size(list);
  cr_assert_eq(sze, 3);

  ListNode* n = list->head;
  int i = 0;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    ++i;
    n = n->next;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    --i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

Test(list_insert_after, after_mid_node) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  e = list_append(list, point_new(1, 1));
  e = list_insert_after(list, list->head->next, point_new(2, 2));
  cr_assert_eq(e, 0);
  e = list_append(list, point_new(3, 3));

  int sze = list_size(list);
  cr_assert_eq(sze, 4);

  ListNode* n = list->head;
  int i = 0;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    ++i;
    n = n->next;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    --i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

// ####################
// list_remove_head
// ####################
Test(list_remove_head, empty) {
  List* list = list_new().ok;
  Result res = list_remove_head(list);
  cr_assert_eq(res.ok, NULL); 
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);

  list_free(&list, NULL);
}

Test(list_remove_head, single) {
  List* list = list_new().ok;

  int e = list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  Result res = list_remove_head(list);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  ListNode* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  list_node_free(&n, (FnFree) safe_free);
  list_free(&list, NULL);
}

Test(list_remove_head, multiple) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = list_remove_head(list);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  ListNode* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);
  list_node_free(&n, (FnFree) safe_free);

  int sze = list_size(list);
  cr_assert_eq(sze, 2);

  n = list->head;
  int i = 0;
  while (n) {
    ++i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->next;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
    --i;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

// ####################
// list_remove_tail
// ####################
Test(list_remove_tail, empty) {
  List* list = list_new().ok;
  Result res = list_remove_tail(list);
  cr_assert_eq(res.ok, NULL); 
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);

  list_free(&list, NULL);
}

Test(list_remove_tail, single) {
  List* list = list_new().ok;

  int e = list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  Result res = list_remove_tail(list);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  ListNode* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  list_node_free(&n, (FnFree) safe_free);
  list_free(&list, NULL);
}

Test(list_remove_tail, multiple) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = list_remove_tail(list);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  ListNode* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 2);
  cr_assert_eq(p->y, 2);

  list_node_free(&n, (FnFree) safe_free);

  int sze = list_size(list);
  cr_assert_eq(sze, 2);

  n = list->head;
  int i = 0;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->next;
    ++i;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    --i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

// ####################
// list_remove
// ####################
Test(list_remove, single) {
  List* list = list_new().ok;

  int e = list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  Result res = list_remove(list, list->head);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  ListNode* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  list_node_free(&n, (FnFree) safe_free);
  list_free(&list, NULL);
}

Test(list_remove, multiple_head) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = list_remove(list, list->head);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  ListNode* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);
  list_node_free(&n, (FnFree) safe_free);

  int sze = list_size(list);
  cr_assert_eq(sze, 2);

  n = list->head;
  int i = 0;
  while (n) {
    ++i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->next;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
    --i;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

Test(list_remove, multiple_tail) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = list_remove(list, list->tail);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  ListNode* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 2);
  cr_assert_eq(p->y, 2);
  list_node_free(&n, (FnFree) safe_free);

  int sze = list_size(list);
  cr_assert_eq(sze, 2);

  n = list->head;
  int i = 0;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->next;
    ++i;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    --i;
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n = n->previous;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

Test(list_remove, multiple_mid) {
  List* list = list_new().ok;

  int e = list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = list_remove(list, list->head->next);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  ListNode* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 1);
  cr_assert_eq(p->y, 1);
  list_node_free(&n, (FnFree) safe_free);

  int sze = list_size(list);
  cr_assert_eq(sze, 2);

  n = list->head;
  int i = 0;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n->next ? i += 2 : 0;
    n = n->next;
  }
  cr_assert_eq(i, sze);

  n = list->tail;
  i = sze;
  while (n) {
    cr_assert_eq(n != NULL, true);
    Point* p = n->value;
    cr_assert_eq(p->x, i);
    cr_assert_eq(p->y, i);
    n->previous ? i -= 2 : 0;
    n = n->previous;
  }
  cr_assert_eq(i, 0);

  list_free(&list, (FnFree) safe_free);
}

// ####################
// list_find
// ####################
bool is_69_420(ListNode* n) {
  Point* p = n->value;
  return p->x == 69 && p->y == 420;
}

Test(list_find, not_found) {
  List* list = list_new().ok;
  
  Result res = list_find(list, (FnPredicate) is_69_420);
  cr_assert_eq(res.ok, NULL);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);

  list_free(&list, NULL);
}

Test(list_find, found) {
  List* list = list_new().ok;
  int e = list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(69, 420));
  cr_assert_eq(e, 0);

  e = list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  
  Result res = list_find(list, (FnPredicate) is_69_420);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error.code, SUC_CODE_GENERAL);

  ListNode* n = res.ok;
  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 69);
  cr_assert_eq(p->y, 420);

  list_free(&list, (FnFree) safe_free);
}

// ####################
// list_to_string
// ####################
Test(list_to_string, _1) {
  List* list = list_new(3).ok;

  char* result = list_to_string(list, (ToStringFn) NULL).ok;
  char* expected = "{}";
  cr_assert_eq(strcmp(result, expected), 0);
  safe_free((void**) &result);

  list_append(list, point_new(0, 0));
  list_append(list, point_new(1, 1));

  result = list_to_string(list, (ToStringFn) point_to_string).ok;
  expected = "{(0, 0), (1, 1)}";
  cr_assert_eq(strcmp(result, expected), 0);

  safe_free((void**) &result);
  list_free(&list, (FnFree) safe_free);
}
