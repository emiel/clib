#include <stdint.h>

typedef struct node {
	void        *value;
	struct node *next;
	struct node *prev;
} list_node_t;

typedef struct {
	uint64_t     length;
	struct node *head;
	struct node *tail;
} list_t;

list_t *
list_new(void);

list_t *
list_push_head(list_t *self, void *value);

list_t *
list_push_tail(list_t *self, void *value);

void
list_free(list_t *self);
