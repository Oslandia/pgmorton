#include "morton.h"

// morton code implementation from
// https://fgiesen.wordpress.com/2009/12/13/decoding-morton-codes/

uint32_t
part1_by1(uint32_t x)
{
  x &= 0x0000ffff;
  x = (x ^ (x <<  8)) & 0x00ff00ff;
  x = (x ^ (x <<  4)) & 0x0f0f0f0f;
  x = (x ^ (x <<  2)) & 0x33333333;
  x = (x ^ (x <<  1)) & 0x55555555;
  return x;
}

uint32_t
part1_by2(uint32_t x)
{
  x &= 0x000003ff;
  x = (x ^ (x << 16)) & 0xff0000ff;
  x = (x ^ (x <<  8)) & 0x0300f00f;
  x = (x ^ (x <<  4)) & 0x030c30c3;
  x = (x ^ (x <<  2)) & 0x09249249;
  return x;
}

uint32_t
part_even(uint32_t x)
{
    x = x & 0x5555555555555555;
    x = (x | (x >> 1)) & 0x3333333333333333;
    x = (x | (x >> 2)) & 0x0F0F0F0F0F0F0F0F;
    x = (x | (x >> 4)) & 0x00FF00FF00FF00FF;
    x = (x | (x >> 8)) & 0x0000FFFF0000FFFF;
    return x;
}

int32_t
encode_morton_2d(uint32_t x, uint32_t y, uint32_t *index)
{
  *index =  (part1_by1(y) << 1) + part1_by1(x);
  return 0;
}

int32_t
encode_morton_3d(uint32_t x, uint32_t y, uint32_t z, uint32_t *index)
{
  *index = (part1_by2(z) << 2) + (part1_by2(y) << 1) + part1_by2(x);
  return 0;
}

int32_t
decode_morton_2d(uint32_t code, uint32_t *x, uint32_t *y)
{
  *x = part_even(code);
  *y = part_even(code >> 1);
  return 0;
}

int32_t
decode_morton_3d(uint32_t code, uint32_t *x, uint32_t *y, uint32_t *z)
{
  *x = part_even(code);
  *y = part_even(code >> 1);
  *z = part_even(code >> 2);
  return 0;
}

int32_t
revert_morton(uint32_t index, uint32_t *revert)
{
  index = ((index >> 1) & 0x55555555u) | ((index & 0x55555555u) << 1);
  index = ((index >> 2) & 0x33333333u) | ((index & 0x33333333u) << 2);
  index = ((index >> 4) & 0x0f0f0f0fu) | ((index & 0x0f0f0f0fu) << 4);
  index = ((index >> 8) & 0x00ff00ffu) | ((index & 0x00ff00ffu) << 8);
  index = ((index >> 16) & 0xffffu) | ((index & 0xffffu) << 16);

  *revert = index;
  return 0;
}
