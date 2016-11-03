import matplotlib.pyplot as plt
import matplotlib.patches as patches
from math import ceil

from session import Session

SIDE = 50


def display_npatchs(n, col):
    res = Session().first(n, col)

    npatchs = Session().get_patchs_number()[0]

    fig, ax = plt.subplots(1)

    minx = 9999999999999
    miny = 9999999999999
    for id in range(1, npatchs):
        mx = Session().get_patch_min(id)['x']
        my = Session().get_patch_min(id)['y']
        if mx < minx:
            minx = mx
        if my < miny:
            miny = my

    width_max = 0
    height_max = 0
    for id in range(1, npatchs):
        # build points
        mi = Session().get_patch_min(id)
        ma = Session().get_patch_max(id)

        # build rectangle
        x = mi['x'] - minx
        y = mi['y'] - miny

        width = ma['x'] - mi['x']
        height = ma['y'] - mi['y']

        cl = 'white'
        if id in res:
            cl = 'red'

        r = patches.Rectangle((x, y), width, height,
                              ec='black', lw=1, fill='true', color=cl)
        ax.add_patch(r)

        if (width + x) > width_max:
            width_max = width + x
        if (height + y) > height_max:
            height_max = height + y

    ax.set_xlim(0, ceil(width_max))
    ax.set_ylim(0, ceil(height_max))

    plt.title('PC Patch')
    plt.xlabel('x')
    plt.ylabel('y')

    plt.savefig("{0}_{1}.png".format(col, n))

Session()
display_npatchs(0, 'morton_64')
display_npatchs(100, 'morton_64')
display_npatchs(500, 'morton_64')
display_npatchs(1000, 'morton_64')
display_npatchs(5000, 'morton_64')
display_npatchs(10000, 'morton_64')

display_npatchs(0, 'morton_revert_4')
display_npatchs(100, 'morton_revert_4')
display_npatchs(500, 'morton_revert_4')
display_npatchs(1000, 'morton_revert_4')
display_npatchs(5000, 'morton_revert_4')
display_npatchs(10000, 'morton_revert_4')

display_npatchs(0, 'morton_revert_16')
display_npatchs(100, 'morton_revert_16')
display_npatchs(500, 'morton_revert_16')
display_npatchs(1000, 'morton_revert_16')
display_npatchs(5000, 'morton_revert_16')
display_npatchs(10000, 'morton_revert_16')

display_npatchs(0, 'morton_revert_64')
display_npatchs(100, 'morton_revert_64')
display_npatchs(500, 'morton_revert_64')
display_npatchs(1000, 'morton_revert_64')
display_npatchs(5000, 'morton_revert_64')
display_npatchs(10000, 'morton_revert_64')

display_npatchs(0, 'morton_revert_256')
display_npatchs(100, 'morton_revert_256')
display_npatchs(500, 'morton_revert_256')
display_npatchs(1000, 'morton_revert_256')
display_npatchs(5000, 'morton_revert_256')
display_npatchs(10000, 'morton_revert_256')
