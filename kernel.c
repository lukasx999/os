#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdnoreturn.h>

#include <limine.h>

#include "serial.h"

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

noreturn void hang(void) {
    while (1) {
        __asm__("hlt");
    }
}

#define NO_DISCARD \
    __attribute__((warn_unused_result))

void port_out(short port, char value);

#define COM1 0x3F8

void serial_putchar(char c) {
    port_out(COM1, c);
}

NO_DISCARD size_t strlen(const char *str) {
    size_t count = 0;
    for (const char *c=str; *c; ++c)
        count++;
    return count;
}

void serial_print(const char *str) {
    for (const char *c=str; *c; ++c)
        serial_putchar(*c);
}

void serial_println(const char *str) {
    serial_print(str);
    serial_putchar('\n');
}

static NO_DISCARD int count_digits(int num) {
    int iters = 0;
    while (num) {
        iters++;
        num /= 10;
    }
    return iters;
}

void stringify_num(int num, char *buf) {
    size_t i = 0;

    int digits = count_digits(num) - 1;
    while (num) {
        char digit = num % 10;
        buf[digits-i++] = digit + 48;
        num = (int) (num / 10);
    }

}

void serial_printf(const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);

    for (size_t i=0; i < strlen(fmt); ++i) {
        char c = fmt[i];
        if (c == '%') {
            switch (fmt[i+1]) {
                case 'd': {
                    int num = va_arg(va, int);
                    char numbuf[1024] = { 0 };
                    stringify_num(num, numbuf);
                    serial_print(numbuf);
                } break;
            }
            i++;
        } else {
            serial_putchar(fmt[i]);
        }
    }


    va_end(va);
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

    serial_printf("hello, %d!", 45);

    for (int y=0; y < (int) fb->height; ++y) {
        for (int x=0; x < (int) fb->width; ++x) {
            uint32_t *buf = fb->address;
            buf[y * fb->width + x] = 0x000000ff;
        }
    }

    hang();

}
