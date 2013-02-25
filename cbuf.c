#include "cbuf.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

signed
cbuf_init(cbuf_t *self, unsigned long order)
{
	assert(order >= 12); // At least 4096b buffer

	char path[] = "/dev/shm/cbuf-XXXXXX";
	int fd;
	if ((fd = mkstemp(path)) == -1) {
		perror("mkstemp");
		goto fail;
	}

	if (unlink(path) == -1) {
		perror("unlink");
		goto fail;
	}

	self->size = 1ULL << order;
	self->write_offset = 0;
	self->read_offset = 0;

	if ((ftruncate(fd, self->size)) == -1) {
		perror("ftruncate");
		goto fail;
	}

	// mmap anonymous (twice the desired size)
	self->addr = mmap(NULL, self->size << 1,
			PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

	if (self->addr == MAP_FAILED) {
		perror("mmap");
		goto fail;
	}

	// mmap first
	char *addr;
	addr = mmap(self->addr, self->size,
			PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0);
	if (self->addr == MAP_FAILED) {
		perror("mmap");
		goto fail;
	}

	if (addr != self->addr) {
		fprintf(stderr, "not contiguous\n");
		goto fail;
	}

	// mmap second
	addr = mmap(self->addr + self->size, self->size,
			PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0);
	if (self->addr == MAP_FAILED) {
		perror("mmap");
		goto fail;
	}

	if (addr != self->addr + self->size) {
		fprintf(stderr, "not contiguous\n");
		goto fail;
	}

	if (close(fd) == -1) {
		perror("close");
		goto fail;
	}

	return 0;

fail:
	return -1;
}

signed
cbuf_destroy(cbuf_t *self)
{
	if (munmap(self->addr, self->size << 1) == -1) {
		perror("munmap");
		return -1;
	}
	return 0;
}

char *
cbuf_write_addr(cbuf_t *self)
{
	return self->addr + self->write_offset;
}

void
cbuf_write_advance(cbuf_t *self, size_t size)
{
	self->write_offset += size;
}

char *
cbuf_read_addr(cbuf_t *self)
{
	return self->addr + self->read_offset;
}

void
cbuf_read_advance(cbuf_t *self, size_t size)
{
	self->read_offset += size;

	if (self->read_offset >= self->size) {
  		self->read_offset -= self->size;
		self->write_offset -= self->size;
	}
}

size_t
cbuf_count(cbuf_t *self)
{
	return self->write_offset - self->read_offset;
}

size_t
cbuf_count_free(cbuf_t  *self)
{
	return self->size - cbuf_count(self);
}

void
cbuf_clear(cbuf_t *self)
{
  	  self->write_offset = 0;
  	  self->read_offset = 0;
}
