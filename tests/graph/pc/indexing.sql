CREATE index on patchs using gist(geometry(pa));

ALTER TABLE patchs add column morton_64 bigint;
SELECT morton_update('patchs', 'pa::geometry', 'morton_64', 64);
CREATE index on patchs(morton_64);

ALTER TABLE patchs add column morton_revert_4 bigint;
SELECT morton_update('patchs', 'pa::geometry', 'morton_revert_4', 4, TRUE);
CREATE index on patchs(morton_revert_4);

ALTER TABLE patchs add column morton_revert_16 bigint;
SELECT morton_update('patchs', 'pa::geometry', 'morton_revert_16', 16, TRUE);
CREATE index on patchs(morton_revert_16);

ALTER TABLE patchs add column morton_revert_64 bigint;
SELECT morton_update('patchs', 'pa::geometry', 'morton_revert_64', 64, TRUE);
CREATE index on patchs(morton_revert_64);

ALTER TABLE patchs add column morton_revert_256 bigint;
SELECT morton_update('patchs', 'pa::geometry', 'morton_revert_256', 256, TRUE);
CREATE index on patchs(morton_revert_256);
