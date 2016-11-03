#ifndef MORTON_H
#define MORTON_H

#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include <limits.h>

int32_t
encode_morton_2d(uint32_t x, uint32_t y, uint32_t *index);

int32_t
encode_morton_3d(uint32_t x, uint32_t y, uint32_t z, uint32_t *index);

int32_t
decode_morton_2d(uint32_t code, uint32_t *x, uint32_t *y);

int32_t
decode_morton_3d(uint32_t code, uint32_t *x, uint32_t *y, uint32_t *z);

int32_t
revert_morton(uint32_t index, uint32_t *revert);

#endif
