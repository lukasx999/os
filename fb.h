#ifndef _FB_H
#define _FB_H

#include <stddef.h>
#include <limine.h>

typedef enum {
    FB_WHITE = 0xffffff,
    FB_BLACK = 0x000000,
    FB_RED   = 0xff0000,
    FB_BLUE  = 0x0000ff,
    FB_GREEN = 0x00ff00,
} Color;

typedef struct {
    uint32_t *addr;
    uint64_t width, height;
} Framebuffer;

// returns 0 on success
int fb_init(Framebuffer *fb);
void fb_draw_pixel(Framebuffer *fb, int x, int y, Color color);
void fb_fill(Framebuffer *fb, Color color);
void fb_draw_circle(Framebuffer *fb, int x, int y, int r, Color color);

#endif // _FB_H
