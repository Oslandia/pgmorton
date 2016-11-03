#include <stdio.h>
#include <math.h>
#include "grid.h"

int32_t
grid_from_text(const char* str, MORTON_GRID *grid)
{
    int32_t rc = 0, size;
    double offsetx, offsety, width, height;
    if (sscanf( str, "( %lf, %lf, %lf, %lf, %d )",
                &offsetx, &offsety, &width, &height, &size) == 5)
    {
        grid->offsetx = offsetx;
        grid->offsety = offsety;
        grid->cell_width = width;
        grid->cell_height = height;
        grid->size = size;
    }
    else
        rc = 1;

    return rc;
}

int32_t
grid_to_text(const MORTON_GRID *grid, size_t size, char* str)
{
    snprintf(str, size, "( %lf, %lf, %lf, %lf, %d )",
             grid->offsetx, grid->offsety,
             grid->cell_width, grid->cell_height,
             grid->size);
    return 0;
}

int32_t
grid_as_text(const MORTON_GRID *grid, size_t size, char* str)
{
    snprintf(str, size, "{\"offsetx\": %lf, \"offsety\": %lf, \"cell_width\": %lf, \"cell_height\": %lf, \"size\": %d}",
             grid->offsetx, grid->offsety,
             grid->cell_width, grid->cell_height,
             grid->size);
    return 0;
}

int32_t
grid_coordinates(const MORTON_GRID* grid, double x, double y, int32_t *xg, int32_t *yg)
{
    *xg = floor((x - grid->offsetx) / grid->cell_width);
    *yg = floor((y - grid->offsety) / grid->cell_height);
    return 0;
}

int32_t
grid_from_extent(double xmin, double ymin, double xmax, double ymax, int32_t size, MORTON_GRID *grid)
{
    grid->offsetx = xmin;
    grid->offsety = ymin;
    grid->size = size;
    grid->cell_width = (xmax - xmin) / size;
    grid->cell_height = (ymax - ymin) / size;

    return 0;
}
