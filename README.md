 [![Build Status](https://secure.travis-ci.org/Oslandia/pgmorton.png)]
 (http://travis-ci.org/Oslandia/pgmorton)

# pgmorton

PostgreSQL extension for Morton and Revert Morton indexing.

## Build/Install

### Requirements

- PostgreSQL and PostgreSQL development packages must be installed
- Postgis must be installed
- CUnit packages must be installed if you want to run unit tests


### Build

To clone, build and install *pgmorton*:

    # git clone https://github.com/Oslandia/pgmorton
    # cd pgmorton
    # mkdir build && cd build
    # cmake ..
    # make
    # sudo make install

### Activate

To activate the *pgmorton* extension:

    # creatdb mydb
    # psql -d mydb
    > mydb=# create extension postgis;
    CREATE EXTENSION
    > mydb=# create extension morton;
    CREATE EXTENSION


## Concepts

A Morton code, or Z-order, is a function which computes a z-value for a point
while preserving locality of the data.

<p align="center">
<img align="center" src="https://upload.wikimedia.org/wikipedia/commons/c/cd/Four-level_Z.svg" width="350">
</p>

The computation is real efficient by interleaving the binary coordinate values.

<p align="center">
<img align="center" src="https://upload.wikimedia.org/wikipedia/commons/3/30/Z-curve.svg" width="350">
</p>

*Source*: https://en.wikipedia.org/wiki/Z-order_curve

## Morton Objects

### MortonGrid

The MortonGrid object describes the grid on which the Morton code has to be
computed:
- `offsetx`/`offsety`: the bottom left corner of the grid (double values)
- `cell_width`/`cell_height`: the size of a cell (double values).
- `size`: the number of cells per side. So the total number of cells per grid is `size*size`


>      SELECT Morton_GridAsText(Morton_GridMake(ST_Extent(pa::geometry), 4)) FROM patchs;
>
>      {"offsetx": 728262.803178, "offsety": 4676439.353454, "cell_width": 183.833000, "cell_height": 143.833000, "size": 4}


## Functions

**Morton_GridMake(xmin float8, ymin float8, cell_width float8, cell_height float8, size integer)** returns **MortonGrid**

> Build a `MortonGrid` object thanks to values given in parameters.
>
>     SELECT Morton_GridMake(728262.803178, 4676439.353454, 728998.135178, 4677014.685454, 64);
>
>     ( 728262.803178, 4676439.353454, 11.489562, 8.989562, 64 )




**Morton_GridMake(extent box2d, size integer)** returns **MortonGrid**

> Build a `MortonGrid` object thanks to an extent and a `size`.
>
>     SELECT Morton_GridMake(Box2d(ST_GeomFromText('LINESTRING(3.5 2.5, 12.7 11.8)')), 4);
>
>     ( 3.500000, 2.500000, 2.300000, 2.325000, 4 )
>
>     SELECT Morton_GridMake(ST_Extent(pa::geometry), 4) FROM patchs;
>
>     ( 728262.803178, 4676439.353454, 183.833000, 143.833000, 4 )




**Morton_GridMake(table_name regclass, geom_col text, size integer)** returns **MortonGrid**

> Compute `MortonGrid` for the geometry column `geom_col` considering the grid
> `size`.
>
>     SELECT Morton_GridAsText(Morton_GridMake('patchs', 'pa::geometry', 64));
>
>      {"offsetx": 728262.803178, "offsety": 4676439.353454, "cell_width": 11.489562, "cell_height": 8.989563, "size": 64}





**Morton_Gridded(x float8, y float8, grid MortonGrid)** returns **integer[2]**

> Return gridded coordinates from floating values.
>
>     SELECT Morton_Gridded(8.7, 10.7, Morton_GridMake(3.5, 2.5, 12.7, 14.9, 4));
>
>     {2,2}



**Morton_Gridded(geom geometry, grid MortonGrid)** returns **integer[2]**

> Return gridded coordinates for a geometry with a specific `grid`.
>
>     SELECT Morton_Gridded(pa::geometry, foo.grid) FROM patchs,
>       (SELECT Morton_GridMake(ST_Extent(pa::geometry), 64) AS grid FROM patchs) AS foo
>       LIMIT 4;
>
>     {0,0}
>     {0,0}
>     {0,1}
>     {1,0}




**Morton_Encode(x integer, y integer)** returns **bigint**

> Return the Morton code for grid coordinates `(x, y)`.
>
>     SELECT Morton_Encode(2, 2)
>
>     12




**Morton_Encode(x float8, y float8, grid MortonGrid)** returns **bigint**

> Return the Morton code for coordinates `(x, y)` according to the `grid`.
>
>     SELECT Morton_Encode(8.7, 10.7, Morton_GridMake(3.5, 2.5, 12.7, 14.9, 4));
>
>     12



**Morton_Encode(geom geometry, grid MortonGrid)** returns **bigint**

> Return the Morton code for a geometry according to the `grid`.
>
>     SELECT Morton_Encode(pa::geometry, foo.grid) FROM patchs,
>       (SELECT Morton_GridMake(ST_Extent(pa::geometry), 64) AS grid FROM patchs)
>       AS foo LIMIT 4;
>
>     0
>     0
>     2
>     1




**Morton_Decode(code bigint)** returns **integer[2]**

> Return the coordinates (x, y) corresponding to the Morton `code`.
>
>     SELECT Morton_Decode(62)
>
>     {6,7}




**Morton_Revert(code bigint)** returns **bigint**

> Return the reverted Morton code for `code`.
>
>     SELECT Morton_Revert(62)
>
>     2080374784




**Morton_Update(table text, geom text, morton text, size integer, revert boolean default False)** returns **void**

> Utility function to compute a Morton code for each geometry within the `geom`
> column based on a regular grid with `side*side` cells. The Morton code is
> stored in the `index` column. If `revert` is True, then a revert Morton
> code is computed.
>
>     ALTER TABLE patchs add column morton_index bigint;
>     SELECT Morton_Update('patchs', 'pa::geometry', 'morton_index', 64)
>     CREATE index ON patchs(morton_index);

## Examples

These examples are available in *tests/graph* directory.

### Regular grid

The Morton code is very useful when you want to keep a neighbourhood relation
between geometries.

For example on a regular grid where a Morton code is computed for each cell, we
are able to retrieve the top 91 cells.

>   SELECT * FROM mytable ORDER BY morton ASC LIMIT 91

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/regular_grid/results/first_morton.png" width="350">
</p>

But, if on the contrary you want to retrieve the top 91 cells with a good
spatialization, you can use the revert Morton code.

>   SELECT * FROM mytable ORDER BY morton_revert(morton) ASC LIMIT 91

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/regular_grid/results/first_morton_revert.png" width="350">
</p>

### Real life

Imagine you have billions of points coming FROM a LIDAR sensor! Mmmmm... We
cannot store each single point within the database...

But fortunately, the [pointcloud](https://github.com/pgpointcloud/pointcloud "pointcloud")
extension is here for that. You just have to manipulate patchs of points
instead of points themselves.

    # createdb pgmorton_pc
    # psql pgmorton_pc
    > create extension postgis;
    > create extension pointcloud;
    > create extension pointcloud_postgis;

And thanks to [PDAL](https://github.com/PDAL/PDAL "PDAL"), populate the database
is easy peasy with the pointcloud plugin. You just have to build a json pipeline:

    {
      "pipeline":[
        {
          "type":"readers.las",
          "filename":"terrain.las",
          "spatialreference":"EPSG:32616"
        },
        {
          "type":"filters.chipper",
          "capacity":500
        },
        {
          "type":"writers.pgpointcloud",
          "connection":"dbname=pgmorton_pc",
          "table":"patchs",
          "compression":"laz",
          "srid":"32616",
          "overwrite":"false"
        }
      ]
    }

Then, let PDAL do the hard work:

    # pdal pipeline -i mypipe.json

But finally, we just want to extract some representative patchs of the
database... But what is the most efficient way...? Revert morton code of course!

    # psql pgmorton_pc
    > CREATE EXTENSION morton;
    > ALTER TABLE patchs ADD COLUMN morton_revert bigint;
    > SELECT Morton_Update('patchs', 'pa::geometry', 'morton_revert', 64, TRUE);
    > CREATE INDEX ON patchs(morton_revert);

The following is just child's play:

>     SELECT id FROM patchs ORDER BY morton_revert ASC LIMIT 0

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_0.png" width="350">
</p>

>     SELECT id FROM patchs ORDER BY morton_revert ASC LIMIT 100

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_100.png" width="350">
</p>

>     SELECT id FROM patchs ORDER BY morton_revert ASC LIMIT 500

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_500.png" width="350">
</p>

>     SELECT id FROM patchs ORDER BY morton_revert ASC LIMIT 1000

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_1000.png" width="350">
</p>

>     SELECT id FROM patchs ORDER BY morton_revert ASC LIMIT 5000

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_5000.png" width="350">
</p>

>     SELECT id FROM patchs ORDER BY morton_revert ASC LIMIT 10000

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_10000.png" width="350">
</p>

### What influence has grid size have on results?

Result with a revert Morton code computed on a grid of size 4x4:

>     SELECT id FROM pacths ORDER BY by morton_revert LIMIT 5000;

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_4_5000.png" width="350">
</p>

Result with a revert Morton code computed on a grid of size 16x16:

>     SELECT id FROM pacths ORDER BY morton_revert LIMIT 5000;

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_16_5000.png" width="350">
</p>

Result with a revert Morton code computed on a grid of size 64x64:

>     SELECT id FROM pacths ORDER BY morton_revert LIMIT 5000;

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_64_5000.png" width="350">
</p>

Result with a revert Morton code computed on a grid of size 256x256:

>     SELECT id FROM pacths ORDER BY morton_revert LIMIT 5000;

<p align="center">
<img align="center" src="https://github.com/Oslandia/pgmorton/blob/master/tests/graph/pc/results/morton_revert_256_5000.png" width="350">
</p>

## License

pgmorton is distributed under LGPL2 or later.
