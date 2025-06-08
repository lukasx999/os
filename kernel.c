#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdnoreturn.h>

#include "fb.h"
#include "serial.h"



noreturn void hang(void) {
    while (1) {
        __asm__("hlt");
    }
}


void kernel_main(void) {

    serial_println("starting kernel...");

    Framebuffer fb = { 0 };
    if (fb_init(&fb))
        hang();

    hang();

}
