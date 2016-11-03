#include "pgsql.h"
#include "morton.h"
#include "grid.h"

PG_MODULE_MAGIC;

Datum mortongrid_in(PG_FUNCTION_ARGS);
Datum mortongrid_out(PG_FUNCTION_ARGS);

Datum morton_encode(PG_FUNCTION_ARGS);
Datum morton_decode(PG_FUNCTION_ARGS);
Datum morton_revert(PG_FUNCTION_ARGS);

Datum morton_grid_make(PG_FUNCTION_ARGS);
Datum morton_gridded(PG_FUNCTION_ARGS);
Datum morton_grid_astext(PG_FUNCTION_ARGS);


PG_FUNCTION_INFO_V1(mortongrid_in);
Datum mortongrid_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);

    if ( str[0] != '\0' )
    {
        if ( str[0] != '0' )
        {
            MORTON_GRID grid;
            if ( grid_from_text(str, &grid) == 0)
            {
                MORTON_GRID *result = palloc(sizeof(MORTON_GRID));
                memcpy(result, &grid, sizeof(grid));
                PG_RETURN_POINTER(result);
            }
        }
        else
        {
            // TODO
            // grid = morton_grid_from_hexwkb((uint8_t*) str, strlen(str));
            ereport(ERROR, (errmsg("invalid format")));
        }
    }
    else
        ereport(ERROR, (errmsg("empty string")));

    PG_RETURN_NULL();
}

PG_FUNCTION_INFO_V1(mortongrid_out);
Datum mortongrid_out(PG_FUNCTION_ARGS)
{
    MORTON_GRID *grid = (MORTON_GRID*) PG_GETARG_POINTER(0);

    size_t size = 100;
    char *result = palloc(size);
    if ( grid_to_text(grid, size, result) == 0)
    {
        PG_RETURN_CSTRING(result);
    }

    pfree(result);
    PG_RETURN_NULL();
}

PG_FUNCTION_INFO_V1(morton_encode);
Datum morton_encode(PG_FUNCTION_ARGS)
{
    int32 x = PG_GETARG_INT32(0);
    int32 y = PG_GETARG_INT32(1);

    if ( x < 0 || y < 0 )
        ereport(ERROR, (errmsg("coordinates are negatives")));

    uint32_t index;
    encode_morton_2d(x, y, &index);

    PG_RETURN_INT64(index);
}

PG_FUNCTION_INFO_V1(morton_decode);
Datum morton_decode(PG_FUNCTION_ARGS)
{
    uint32_t code = PG_GETARG_INT64(0);

    uint32_t x, y;
    decode_morton_2d(code, &x, &y);

    Datum *datum = (Datum * )palloc(2 * sizeof(Datum));
    datum[0] = Int32GetDatum(x);
    datum[1] = Int32GetDatum(y);

    ArrayType *array = construct_array(datum, 2, INT4OID, 4, true, 'i');

    PG_RETURN_ARRAYTYPE_P(array);
}

PG_FUNCTION_INFO_V1(morton_revert);
Datum morton_revert(PG_FUNCTION_ARGS)
{
    uint32_t code = PG_GETARG_INT64(0);

    uint32_t revert;
    revert_morton(code, &revert);

    PG_RETURN_INT64(revert);
}

PG_FUNCTION_INFO_V1(morton_grid_make);
Datum morton_grid_make(PG_FUNCTION_ARGS)
{
    float8 xmin = PG_GETARG_FLOAT8(0);
    float8 ymin = PG_GETARG_FLOAT8(1);
    float8 xmax = PG_GETARG_FLOAT8(2);
    float8 ymax = PG_GETARG_FLOAT8(3);
    int32 size = PG_GETARG_INT32(4);

    MORTON_GRID *grid = palloc(sizeof(MORTON_GRID));
    grid_from_extent(xmin, ymin, xmax, ymax, size, grid);

    PG_RETURN_POINTER(grid);
}

PG_FUNCTION_INFO_V1(morton_gridded);
Datum morton_gridded(PG_FUNCTION_ARGS)
{
    float8 x = PG_GETARG_FLOAT8(0);
    float8 y = PG_GETARG_FLOAT8(1);
    MORTON_GRID* grid = (MORTON_GRID*) PG_GETARG_POINTER(2);

    int32_t xg, yg;
    grid_coordinates(grid, x, y, &xg, &yg);

    Datum *datum = (Datum * )palloc(2 * sizeof(Datum));
    datum[0] = Int32GetDatum(xg);
    datum[1] = Int32GetDatum(yg);

    ArrayType *array = construct_array(datum, 2, INT4OID, 4, true, 'i');

    PG_RETURN_ARRAYTYPE_P(array);
}

PG_FUNCTION_INFO_V1(morton_grid_astext);
Datum morton_grid_astext(PG_FUNCTION_ARGS)
{
    MORTON_GRID *grid = (MORTON_GRID*) PG_GETARG_POINTER(0);

    size_t size = 300;
    char *result = palloc(size);
    grid_as_text(grid, size, result);

    PG_RETURN_TEXT_P( cstring_to_text(result) );
}

