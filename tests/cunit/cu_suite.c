#include "cu_suite.h"

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

int main()
{
    CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite, "test of morton_encode", test_morton_encode)) ||
        (NULL == CU_add_test(pSuite, "test of morton_decode", test_morton_decode)) ||
        (NULL == CU_add_test(pSuite, "test of morton_decode", test_morton_decode)) ||
        (NULL == CU_add_test(pSuite, "test of grid_from_text", test_grid_from_text)) ||
        (NULL == CU_add_test(pSuite, "test of grid_to_text", test_grid_to_text)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

