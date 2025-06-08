#include <stdbool.h>
#include <limine.h>

#include "fb.h"

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

int fb_init(Framebuffer *fb) {

    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
        return -1;

    // framebuffer has RGB-format with 32-bit wide pixels
    struct limine_framebuffer *limine_fb = framebuffer_request.response->framebuffers[0];
    fb->addr   = (uint32_t*) limine_fb->address;
    fb->width  = limine_fb->width;
    fb->height = limine_fb->height;

    return 0;
}

void fb_draw_pixel(Framebuffer *fb, int x, int y, Color color) {
    fb->addr[y * fb->width + x] = color;
}

void fb_fill(Framebuffer *fb, Color color) {
    for (uint64_t y=0; y < fb->height; ++y) {
        for (uint64_t x=0; x < fb->width; ++x) {
            fb_draw_pixel(fb, x, y, color);
        }
    }
}

void fb_draw_circle(Framebuffer *fb, int x, int y, int r, Color color) {
    for (uint64_t yi=0; yi < fb->height; ++yi) {
        for (uint64_t xi=0; xi < fb->width; ++xi) {
            int dist = (y-yi)*(y-yi) + (x-xi)*(x-xi);
            if (dist <= r*r)
                fb_draw_pixel(fb, xi, yi, color);
        }
    }
}
