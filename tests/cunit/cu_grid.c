#include "cu_suite.h"
#include <stdlib.h>

void test_grid_from_text()
{
    MORTON_GRID grid;
    int32_t rc;
    char * str;

    str = "(23.5 56.4 45.3 89.4 64)";
    rc = grid_from_text(str, &grid);
    CU_ASSERT_EQUAL(rc, 1);

    str = "( 23.5, 56.4, 45.3, 89.4, 64)";
    rc = grid_from_text(str, &grid);

    CU_ASSERT_EQUAL(rc, 0);
    CU_ASSERT_DOUBLE_EQUAL(grid.offsetx, 23.5, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(grid.offsety, 56.4, 0.001);
    CU_ASSERT_EQUAL(grid.size, 64);

    str = "( 3.500000, 2.500000, 2.300000, 3.100000, 4 )";
    rc = grid_from_text(str, &grid);

    CU_ASSERT_EQUAL(rc, 0);
    CU_ASSERT_DOUBLE_EQUAL(grid.offsetx, 3.5, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(grid.offsety, 2.5, 0.001);
    CU_ASSERT_EQUAL(grid.size, 4);

}

void test_grid_to_text()
{
    MORTON_GRID grid;
    grid.offsetx = 23.5;
    grid.offsety = 56.4;
    grid.cell_width = 1;
    grid.cell_height = 2;
    grid.size = 64;

    char *str = (char*) malloc(100);
    int32_t rc = grid_to_text(&grid, 100, str);
    CU_ASSERT_EQUAL(rc, 0);

    rc = strcmp(str, "( 23.500000, 56.400000, 1.000000, 2.000000, 64 )");
    CU_ASSERT_EQUAL(rc, 0 );
}
