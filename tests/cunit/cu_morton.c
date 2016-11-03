#include "cu_suite.h"

void test_morton_encode()
{
    uint32_t code;
    int32_t res = encode_morton_2d(0, 0, &code);
    CU_ASSERT_EQUAL(code, 0);

    res = encode_morton_2d(6, 7, &code);
    CU_ASSERT_EQUAL(code, 62);
}

void test_morton_decode()
{
    uint32_t x, y;
    int32_t res = decode_morton_2d(62, &x, &y);
    CU_ASSERT_EQUAL(x, 6);
    CU_ASSERT_EQUAL(y, 7);

    res = decode_morton_2d(0, &x, &y);
    CU_ASSERT_EQUAL(x, 0);
    CU_ASSERT_EQUAL(y, 0);
}

void test_morton_revert()
{
    uint32_t code;
    int32_t res = revert_morton(0, &code);
    CU_ASSERT_EQUAL(code, 0);

    res = revert_morton(62, &code);
    CU_ASSERT_EQUAL(code, 2080374784);
}
