#pragma once

#include <fcntl.h>

typedef struct cbuf
{
	char *addr;
	size_t size;			// buffer size in bytes
	off_t write_offset;
	off_t read_offset;
} cbuf_t;

signed
cbuf_init(cbuf_t *self, unsigned long order);

signed
cbuf_destroy(cbuf_t *self);

char *
cbuf_write_addr(cbuf_t *self);

void
cbuf_write_advance(cbuf_t *self, size_t size);

char *
cbuf_read_addr(cbuf_t *self);

void
cbuf_read_advance(cbuf_t *self, size_t size);

size_t
cbuf_count(cbuf_t *self);

size_t
cbuf_count_free(cbuf_t  *self);

void
cbuf_clear(cbuf_t *self);
