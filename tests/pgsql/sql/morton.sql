CREATE EXTENSION postgis;
CREATE EXTENSION morton;

SELECT Morton_Script_Version();

SELECT Morton_Encode(-1, 2);
SELECT Morton_Encode(6, 7);
SELECT Morton_Encode(8.7, 10.7, Morton_GridMake(3.5, 2.5, 12.7, 14.9, 4));

SELECT Morton_Decode(62);
SELECT Morton_Decode(morton_encode(6, 7));

SELECT Morton_Revert(62);
SELECT Morton_Revert(morton_encode(6, 7));

SELECT Morton_GridMake(1.2, 3.5, 63.2, 45.2, 64);
SELECT Morton_GridMake(728262.803178, 4676439.353454, 728998.135178, 4677014.685454, 64);
SELECT Morton_GridMake(Box2d(ST_GeomFromText('LINESTRING(3.5 2.5, 12.7 11.8)')), 4);

SELECT Morton_GridAsText(Morton_GridMake(Box2d(ST_GeomFromText('LINESTRING(3.5 2.5, 12.7 11.8)')), 4));

SELECT Morton_Gridded(8.7, 10.7, Morton_GridMake(3.5, 2.5, 12.7, 14.9, 4));
