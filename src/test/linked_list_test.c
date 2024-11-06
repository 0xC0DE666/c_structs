#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_structs.h"
#include "utils.h"

// ####################
// node_new
// ####################
Test(node_new, _1) {
  Node* node = node_new(point_new(0, 1)).ok;

  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);
  cr_assert_eq(node->next, NULL);
  cr_assert_eq(node->previous, NULL);

  Point* p = node->value;
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 1);

  node_free(&node, (FreeFn) point_free);
}

// ####################
// node_free
// ####################
Test(node_free, _1) {
  Node* node = node_new(point_new(0, 1)).ok;

  cr_assert_eq(node != NULL, true);
  cr_assert_eq(node->value != NULL, true);

  int e = node_free(&node, (FreeFn) point_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(node, NULL);
}

// ####################
// linked_list_size
// ####################
Test(linked_list_size, _1) {
  LinkedList* list = linked_list_new().ok;
  int sze = linked_list_size(list);
  cr_assert_eq(sze, 0);

  for (int i = 0; i < 3; ++i) {
    linked_list_append(list, point_new(i, i));
    sze = linked_list_size(list);
    cr_assert_eq(sze, i + 1);
  }

  linked_list_free(&list, (FreeFn) point_free);
}

// ####################
// linked_list_new
// ####################
Test(linked_list_new, _1) {
  LinkedList* list = linked_list_new().ok;

  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  linked_list_free(&list, NULL);
}

// ####################
// linked_list_clear
// ####################
Test(linked_list_clear, empty) {
  LinkedList* list = linked_list_new().ok;
  int e = linked_list_clear(list, NULL);

  cr_assert_eq(e, 0);
  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  linked_list_free(&list, NULL);
}

Test(linked_list_clear, single) {
  LinkedList* list = linked_list_new().ok;
  list->head = node_new(point_new(0, 0)).ok;
  list->tail = list->head;

  int e = linked_list_clear(list, (FreeFn) point_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  linked_list_free(&list, (FreeFn) point_free);
}

// TODO: check this, check a b c is freed
Test(linked_list_clear, multiple) {
  LinkedList* list = linked_list_new().ok;
  Node* a = node_new(point_new(0, 0)).ok;
  Node* b = node_new(point_new(1, 1)).ok;
  Node* c = node_new(point_new(2, 2)).ok;

  a->next = b;
  b->previous = a;
  b->next = c;
  c->previous = b;
  list->head = a;
  list->tail = c;

  int e = linked_list_clear(list, (FreeFn) point_free);
  cr_assert_eq(e, 0);
  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  linked_list_free(&list, (FreeFn) point_free);
}


// ####################
// linked_list_free
// ####################
Test(linked_list_free, _1) {
  LinkedList* list = linked_list_new().ok;
  Node* a = node_new(point_new(0, 0)).ok;
  Node* b = node_new(point_new(1, 1)).ok;
  Node* c = node_new(point_new(1, 1)).ok;

  a->next = b;
  b->previous = a;
  b->next = c;
  c->previous = b;
  list->head = a;
  list->tail = c;

  cr_assert_eq(list != NULL, true);
  cr_assert_eq(list->head, a);
  cr_assert_eq(list->tail, c);

  linked_list_free(&list, (FreeFn) point_free);
  cr_assert_eq(list, NULL);
}

// ####################
// linked_list_append
// ####################
Test(linked_list_append, empty) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);
  cr_assert_eq(list->head, list->tail);

  Point* p = list->head->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  linked_list_free(&list, (FreeFn) point_free);
}

Test(linked_list_append, not_empty) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  int sze = linked_list_size(list);
  cr_assert_eq(sze, 3);

  Node* n = list->head;
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

  linked_list_free(&list, (FreeFn) point_free);
}

// ####################
// linked_list_prepend
// ####################
Test(linked_list_prepend, empty) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);
  cr_assert_eq(list->head, list->tail);

  Point* p = list->head->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  linked_list_free(&list, (FreeFn) point_free);
}

Test(linked_list_prepend, not_empty) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_prepend(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  e = linked_list_prepend(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = linked_list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  int sze = linked_list_size(list);
  cr_assert_eq(sze, 3);

  Node* n = list->head;
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

  linked_list_free(&list, (FreeFn) point_free);
}

// ####################
// linked_list_insert_before
// ####################
Test(linked_list_insert_before, before_head) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(1, 1));
  e = linked_list_insert_before(list, list->head, point_new(0, 0));
  cr_assert_eq(e, 0);
  e = linked_list_append(list, point_new(2, 2));

  int sze = linked_list_size(list);
  cr_assert_eq(sze, 3);

  Node* n = list->head;
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

  linked_list_free(&list, (FreeFn) point_free);
}

Test(linked_list_insert_before, before_mid_node) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  e = linked_list_append(list, point_new(2, 2));
  e = linked_list_insert_before(list, list->head->next, point_new(1, 1));
  cr_assert_eq(e, 0);
  e = linked_list_append(list, point_new(3, 3));

  int sze = linked_list_size(list);
  cr_assert_eq(sze, 4);

  Node* n = list->head;
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

  linked_list_free(&list, (FreeFn) point_free);
}

// ####################
// linked_list_insert_after
// ####################
Test(linked_list_insert_after, after_tail) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  e = linked_list_insert_after(list, list->tail, point_new(1, 1));
  cr_assert_eq(e, 0);
  e = linked_list_append(list, point_new(2, 2));

  int sze = linked_list_size(list);
  cr_assert_eq(sze, 3);

  Node* n = list->head;
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

  linked_list_free(&list, (FreeFn) point_free);
}

Test(linked_list_insert_after, after_mid_node) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  e = linked_list_append(list, point_new(1, 1));
  e = linked_list_insert_after(list, list->head->next, point_new(2, 2));
  cr_assert_eq(e, 0);
  e = linked_list_append(list, point_new(3, 3));

  int sze = linked_list_size(list);
  cr_assert_eq(sze, 4);

  Node* n = list->head;
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

  linked_list_free(&list, (FreeFn) point_free);
}

// ####################
// linked_list_remove_head
// ####################
Test(linked_list_remove_head, empty) {
  LinkedList* list = linked_list_new().ok;
  Result res = linked_list_remove_head(list);
  cr_assert_eq(res.ok, NULL); 
  cr_assert_eq(res.error, NULL); 

  linked_list_free(&list, NULL);
}

Test(linked_list_remove_head, single) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  Result res = linked_list_remove_head(list);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error, NULL);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  Node* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  node_free(&n, (FreeFn) point_free);
  linked_list_free(&list, NULL);
}

Test(linked_list_remove_head, multiple) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = linked_list_remove_head(list);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error, NULL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  Node* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);
  node_free(&n, (FreeFn) point_free);

  int sze = linked_list_size(list);
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

  linked_list_free(&list, (FreeFn) point_free);
}

// ####################
// linked_list_remove_tail
// ####################
Test(linked_list_remove_tail, empty) {
  LinkedList* list = linked_list_new().ok;
  Result res = linked_list_remove_tail(list);
  cr_assert_eq(res.ok, NULL); 
  cr_assert_eq(res.error, NULL); 

  linked_list_free(&list, NULL);
}

Test(linked_list_remove_tail, single) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  Result res = linked_list_remove_tail(list);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error, NULL);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  Node* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);

  node_free(&n, (FreeFn) point_free);
  linked_list_free(&list, NULL);
}

Test(linked_list_remove_tail, multiple) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = linked_list_remove_tail(list);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error, NULL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  Node* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 2);
  cr_assert_eq(p->y, 2);

  node_free(&n, (FreeFn) point_free);

  int sze = linked_list_size(list);
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

  linked_list_free(&list, (FreeFn) point_free);
}

// ####################
// linked_list_remove
// ####################
Test(linked_list_remove, single) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_prepend(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  Result res = linked_list_remove(list, list->head);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error, NULL);
  cr_assert_eq(list->head, NULL);
  cr_assert_eq(list->tail, NULL);

  Node* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  node_free(&n, (FreeFn) point_free);
  linked_list_free(&list, NULL);
}

Test(linked_list_remove, multiple_head) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = linked_list_remove(list, list->head);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error, NULL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  Node* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 0);
  cr_assert_eq(p->y, 0);
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);
  node_free(&n, (FreeFn) point_free);

  int sze = linked_list_size(list);
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

  linked_list_free(&list, (FreeFn) point_free);
}

Test(linked_list_remove, multiple_tail) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = linked_list_remove(list, list->tail);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error, NULL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  Node* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 2);
  cr_assert_eq(p->y, 2);
  node_free(&n, (FreeFn) point_free);

  int sze = linked_list_size(list);
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

  linked_list_free(&list, (FreeFn) point_free);
}

Test(linked_list_remove, multiple_mid) {
  LinkedList* list = linked_list_new().ok;

  int e = linked_list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(1, 1));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  Result res = linked_list_remove(list, list->head->next);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error, NULL);
  cr_assert_eq(list->head != NULL, true);
  cr_assert_eq(list->tail != NULL, true);

  Node* n = res.ok;
  cr_assert_eq(n->next, NULL);
  cr_assert_eq(n->previous, NULL);

  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 1);
  cr_assert_eq(p->y, 1);
  node_free(&n, (FreeFn) point_free);

  int sze = linked_list_size(list);
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

  linked_list_free(&list, (FreeFn) point_free);
}

// ####################
// linked_list_find
// ####################
bool node_fun(Node* n) {
  Point* p = n->value;
  return p->x == 69 && p->y == 420;
}

Test(linked_list_find, not_found) {
  LinkedList* list = linked_list_new().ok;
  
  Result res = linked_list_find(list, (PredicateFn) node_fun);
  cr_assert_eq(res.ok, NULL);
  cr_assert_eq(res.error, NULL);

  linked_list_free(&list, NULL);
}

Test(linked_list_find, found) {
  LinkedList* list = linked_list_new().ok;
  int e = linked_list_append(list, point_new(0, 0));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(69, 420));
  cr_assert_eq(e, 0);

  e = linked_list_append(list, point_new(2, 2));
  cr_assert_eq(e, 0);

  
  Result res = linked_list_find(list, (PredicateFn) node_fun);
  cr_assert_eq(res.ok != NULL, true);
  cr_assert_eq(res.error, NULL);

  Node* n = res.ok;
  Point* p = n->value;
  cr_assert_eq(p != NULL, true);
  cr_assert_eq(p->x, 69);
  cr_assert_eq(p->y, 420);

  linked_list_free(&list, (FreeFn) point_free);
}
