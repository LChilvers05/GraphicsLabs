/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

/* This is the code you will need to replace for Lab 1.
 *
 * It contains two simple implementations that loop over the longest axis adding
 * the gradient to the position on the other axis at each step. The objective is
 * for you to remove the use of floating point variables and work entirely with
 * integers. You should use Bresenhams Line Algorithm to achieve this.
 *
 * Note this code does not do clipping. Keep the object inside the viewport.
 */

#include "linedrawer.h"
#include <iostream>

void increment_x_y(int *u, int *w, int *f, 
                    const int u_dir, const int v_dir,
                    const int du, const int dv) {
    *u += u_dir;
    if (*f > 0) {
        *w += v_dir;
        *f += 2 * (dv - du);
    } else {
        *f += 2 * dv;
    }
}

int plot_line_low(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    int dx = ex - sx;
    int dy = ey - sy;

    int dir = (dx < 0) ? -1 : 1;
    int yi = (dy < 0) ? -1 : 1;
    if (dy < 0) { dy = -dy; }

    int fy = (2 * dy) - dx;
    int wy = sy; int x = sx;

    while (x != ex) {
        fb->plotPixel(x, wy, 1.0f, 1.0f, 1.0f);
        increment_x_y(&x, &wy, &fy, dir, yi, dx, dy);
    }

    return 0;
}

int plot_line_high(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    int dx = ex - sx;
    int dy = ey - sy;

    int dir = (dy < 0) ? -1 : 1;
    int xi = (dx < 0) ? -1 : 1;
    if (dx < 0) { dx = -dx; }

    int fx = (2 * dx) - dy;
    int wx = sx; int y = sy;

    while (y != ey) {
        fb->plotPixel(wx, y, 1.0f, 1.0f, 1.0f);
        increment_x_y(&y, &wx, &fx, dir, xi, dy, dx);
    }

    return 0;
}

int draw_x_line(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    if (sx > ex) {
        return plot_line_low(fb, ex, ey, sx, sy);
    } else {
        return plot_line_low(fb, sx, sy, ex, ey);
    }
}

int draw_y_line(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    if (sy > ey) {
        return plot_line_high(fb, ex, ey, sx, sy);
    } else {
        return plot_line_high(fb, sx, sy, ex, ey);
    }
}

int draw_line(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    if ((sx == ex) && (sy == ey)) {
        return fb->plotPixel(sx, sy, 1.0f, 1.0f, 1.0f);
    } else if (((ex - sx) * (ex - sx)) >= ((ey - sy) * (ey - sy))) {
        return draw_x_line(fb, sx, sy, ex, ey);
    } else {
        return draw_y_line(fb, sx, sy, ex, ey);
    }
}
