#ifndef GRID_H
#define GRID_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    double offsetx;
    double offsety;
    double cell_width;
    double cell_height;
    int32_t size;
} MORTON_GRID;

int32_t
grid_from_text(const char* str, MORTON_GRID *grid);

int32_t
grid_to_text(const MORTON_GRID *grid, size_t size, char* str);

int32_t
grid_as_text(const MORTON_GRID *grid, size_t size, char* str);

int32_t
grid_coordinates(const MORTON_GRID* grid, double x,  double y, int32_t *xg, int32_t *yg);

int32_t
grid_from_extent(double xmin, double ymin, double xmax, double ymax, int32_t size, MORTON_GRID *grid);

#endif
