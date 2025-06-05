#include "linked_list.h"
#include <stdio.h>

// Function pointers
static void *(*malloc_fptr)(size_t size) = NULL;
static void (*free_fptr)(void *addr) = NULL;

// Linked list operations
struct linked_list *linked_list_create(void) {
  struct linked_list *ll = malloc_fptr(sizeof(struct linked_list));
  if (!ll)
    return NULL;
  ll->head = NULL;
  return ll;
}

bool linked_list_delete(struct linked_list *ll) {
  if (!ll)
    return false;
  struct node *current = ll->head;

  while (current != NULL) {
    struct node *next = current->next;
    free_fptr(current);
    current = next;
  }
  free_fptr(ll);
  return true;
}

size_t linked_list_size(struct linked_list *ll) {
  if (!ll)
    return SIZE_MAX;
  struct node *current = ll->head;
  size_t size = 0;
  while (current != NULL) {
    current = current->next;
    size++;
  }
  return size;
}

bool linked_list_insert_end(struct linked_list *ll, unsigned int data) {

  // Move pointer to the end
  if (!ll)
    return false;
  struct node *current = ll->head;

  // Handle empty list
  if (!ll->head) {
    ll->head = malloc_fptr(sizeof(struct node));
    if (!ll->head)
      return false;
    ll->head->data = data;
    ll->head->next = NULL;
    return true;
  }

  while (current->next != NULL) {
    current = current->next;
  }

  struct node *new_node = malloc_fptr(sizeof(struct node));
  if (!new_node)
    return false;
  current->next = new_node;
  new_node->data = data;
  new_node->next = NULL;
  return true;
}

bool linked_list_insert_front(struct linked_list *ll, unsigned int data) {
  if (!ll)
    return false;

  struct node *new_head = malloc_fptr(sizeof(struct node));
  if (!new_head)
    return false;

  new_head->data = data;
  new_head->next = ll->head;
  ll->head = new_head;

  return true;
}

bool linked_list_insert(struct linked_list *ll, size_t index,
                        unsigned int data) {
  if (!ll)
    return NULL;
  struct node *current = ll->head;
  if (!current)
    return NULL;

  size_t current_idx = 0;
  while (current != NULL) {
    if (current_idx == index) {
      current->data = data;
      return true;
    }
    current = current->next;
    current_idx++;
  }

  return false;
}

size_t linked_list_find(struct linked_list *ll, unsigned int data) {
  struct node *current = ll->head;
  if (!current)
    return SIZE_MAX;
  size_t index = 0;
  while (current != NULL) {
    if (current->data == data) {
      return index;
    }
    index++;
  }
  return SIZE_MAX;
}

bool linked_list_remove(struct linked_list *ll, size_t index) {
  if (!ll || !ll->head)
    return false;
  struct node *current = ll->head;
  struct node *prev = NULL;

  if (index == 0) {
    ll->head = current->next;
    free_fptr(current);
    return true;
  }

  size_t idx = 0;
  while (current != NULL || idx < index) {
    prev = current;
    current = current->next;
    idx++;
  }

  if (current == NULL) {
    return false;
  }

  prev->next = current->next;
  free_fptr(current);
  return true;
}

// Iterator operations
struct iterator *linked_list_create_iterator(struct linked_list *ll,
                                             size_t index) {

  if (!ll)
    return NULL;

  struct node *current_node = ll->head;
  if (!current_node)
    return NULL;

  // Move ptr to the target index
  size_t current_idx = 0;
  while (current_idx < index) {

    // Exit loop if node at target index NULL
    if (!current_node->next)
      return NULL;

    current_node = current_node->next;
    current_idx++;
  }

  // Create the iterator if checks passed
  struct iterator *iter = malloc_fptr(sizeof(struct iterator));
  if (!iter)
    return NULL;
  iter->current_index = index;
  iter->current_node = current_node;
  iter->data = current_node->data;
  iter->ll = ll;
  return iter;
}

bool linked_list_delete_iterator(struct iterator *iter) {
  if (!iter)
    return false;
  free_fptr(iter);
  return true;
}

bool linked_list_iterate(struct iterator *iter) {
  if (!iter || !iter->current_node || !iter->current_node->next) {
    return false;
  }
  iter->current_node = iter->current_node->next;
  iter->current_index++;
  iter->data = iter->current_node->data;
  return true;
}

bool linked_list_register_malloc(void *(*malloc_func)(size_t)) {
  if (!malloc_func)
    return false;
  malloc_fptr = malloc_func;
  return true;
}

bool linked_list_register_free(void (*free_func)(void *)) {
  if (!free_func)
    return false;
  free_fptr = free_func;
  return true;
}
