#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>

#include <yarl.h>

#include <limine.h>

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3)

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER


static noreturn void hang(void) {
    while (1) {
        __asm__("hlt");
    }
}

void kernel_main(void) {

    if (
        framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1 ||
        LIMINE_BASE_REVISION_SUPPORTED == false
    ) {
        hang();
    }

    // framebuffer has RGB-format with 32-bit wide pixels
    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];

    Yarl *yarl = yarl_init(
        (unsigned char*) fb->address,
        fb->width,
        fb->height,
        YARL_COLOR_FORMAT_BGRA,
        hang
    );

    yarl_fill(yarl, YARL_RED);
    yarl_draw_rect(yarl, 50, 50, 100, 100, YARL_YELLOW);
    // int o = 100;
    // yarl_draw_triangle(yarl, fb->width/2, o, fb->width-o, fb->height-o, o, fb->height-o, YARL_BLUE);
    yarl_draw_circle(yarl, fb->width/2, fb->height/2, 50, YARL_BLUE);
    // yarl_draw_line(yarl, 0, 0, 50, 50, YARL_BLUE);

    hang();

}
