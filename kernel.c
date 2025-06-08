#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>

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

void something(void);

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

    for (int y=0; y < (int) fb->height; ++y) {
        for (int x=0; x < (int) fb->width; ++x) {
            uint32_t *buf = fb->address;
            buf[y * fb->width + x] = 0x00000000;
        }
    }

    something();

    // __asm__ volatile ("mov $3, %al");
    // __asm__ volatile ("out %al, $0x3F8+3");
    //     COM1 0x3F8
    // ; mov al, 00000011b
    // ; out COM1+3, al

    hang();

}
