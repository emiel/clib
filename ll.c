#include "ll.h"

#include <stdlib.h>

list_t *
list_new(void)
{
	list_t *list;

	if ((list = malloc(sizeof(list_t))) == NULL)
		return NULL;

	list->length = 0;
	list->head = NULL;
	list->tail = NULL;

	return list;
}

list_t *
list_push_head(list_t *self, void *value)
{
	list_node_t *node;

	if (self == NULL)
		return NULL;

	if ((node = malloc(sizeof(*node))) == NULL)
		return NULL;

	node->next = NULL;
	node->prev = NULL;
	node->value = value;

	if (self->length == 0) {
		self->head = node;
		self->tail = node;
	} else {
		node->next = self->head;
		self->head->prev = node;
		self->head = node;
	}

	self->length++;

	return self;
}

list_t *
list_push_tail(list_t *self, void *value)
{
	list_node_t *node;

	if (self == NULL)
		return NULL;

	if ((node = malloc(sizeof(*node))) == NULL)
		return NULL;

	node->next = NULL;
	node->prev = NULL;
	node->value = value;

	if (self->length == 0) {
		self->head = node;
		self->tail = node;
	} else {
		node->prev = self->tail;
		self->tail->next = node;
		self->tail = node;
	}

	self->length++;

	return self;
}

void
list_free(list_t *self)
{
	uint64_t length = self->length;
	list_node_t *current = self->head;
	list_node_t *next;

	while (length--) {
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}

	free(self);
	self = NULL;
}
