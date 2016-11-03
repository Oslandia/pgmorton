import matplotlib.pyplot as plt
import matplotlib.patches as patches

import session

SIDE = 50


def display(morton, f):
    pos = []
    for e in morton:
        pos.append(s.decode_morton(e)[0])

    # build graph
    fig, ax = plt.subplots(1)
    ax.set_xlim(0, SIDE)
    ax.set_ylim(0, SIDE)

    for x in range(0, SIDE):
        for y in range(0, SIDE):
            if [x, y] in pos:
                r = patches.Rectangle((x, y), 1,
                                      1, ec='r', lw=2, fc='blue',
                                      fill='true')
            else:
                r = patches.Rectangle((x, y), 1,
                                      1, ec='r', lw=2, fc='none',
                                      fill='true')
            ax.add_patch(r)

    plt.savefig(f)
    print("Display saved in {0}".format(f))


def first_morton(n, f):
    l = s.first_morton(n)
    display(l, f)


def first_morton_revert(n, f):
    l = s.first_revert_morton(n)
    display(l, f)


# init database connection
s = session.Session()

# build grid
for x in range(0, SIDE):
    for y in range(0, SIDE):
        s.insert(x, y)

# save
first_morton(91, "first_morton.png")

first_morton_revert(91, "first_morton_revert.png")
