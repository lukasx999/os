#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <limine.h>

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

// void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
//     uint8_t *restrict pdest = (uint8_t *restrict)dest;
//     const uint8_t *restrict psrc = (const uint8_t *restrict)src;
//
//     for (size_t i = 0; i < n; i++) {
//         pdest[i] = psrc[i];
//     }
//
//     return dest;
// }
//
// void *memset(void *s, int c, size_t n) {
//     uint8_t *p = (uint8_t *)s;
//
//     for (size_t i = 0; i < n; i++) {
//         p[i] = (uint8_t)c;
//     }
//
//     return s;
// }
//
// void *memmove(void *dest, const void *src, size_t n) {
//     uint8_t *pdest = (uint8_t *)dest;
//     const uint8_t *psrc = (const uint8_t *)src;
//
//     if (src > dest) {
//         for (size_t i = 0; i < n; i++) {
//             pdest[i] = psrc[i];
//         }
//     } else if (src < dest) {
//         for (size_t i = n; i > 0; i--) {
//             pdest[i-1] = psrc[i-1];
//         }
//     }
//
//     return dest;
// }
//
// int memcmp(const void *s1, const void *s2, size_t n) {
//     const uint8_t *p1 = (const uint8_t *)s1;
//     const uint8_t *p2 = (const uint8_t *)s2;
//
//     for (size_t i = 0; i < n; i++) {
//         if (p1[i] != p2[i]) {
//             return p1[i] < p2[i] ? -1 : 1;
//         }
//     }
//
//     return 0;
// }

static void hang(void) {
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
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    for (size_t y=0; y < framebuffer->height; ++y) {
        for (size_t x=0; x < framebuffer->width; ++x) {
            uint32_t *fb_ptr = (uint32_t*) framebuffer->address;
            fb_ptr[y * framebuffer->width + x] = 0x0000ff;
        }
    }

    hang();

}
