#include "serial.h"
#include "string.h"
#include "kernel.h"
#include "io.h"

#define COM1 0x3F8

static NO_DISCARD int count_digits(int num) {
    int iters = 0;
    while (num) {
        iters++;
        num /= 10;
    }
    return iters;
}

static void stringify_num(int num, char *buf) {
    size_t i = 0;

    int digits = count_digits(num) - 1;
    while (num) {
        char digit = num % 10;
        buf[digits-i++] = digit + 48;
        num = (int) (num / 10);
    }

}

void serial_putchar(char c) {
    port_out(COM1, c);
}

void serial_print(const char *str) {
    for (const char *c=str; *c; ++c)
        serial_putchar(*c);
}

void serial_println(const char *str) {
    serial_print(str);
    serial_putchar('\n');
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
                case 's': {
                    serial_print(va_arg(va, const char*));
                } break;
            }
            i++;
        } else {
            serial_putchar(fmt[i]);
        }
    }

    va_end(va);
}
