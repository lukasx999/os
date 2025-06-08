#ifndef _SERIAL_H
#define _SERIAL_H

void serial_putchar(char c);
void serial_print(const char *str);
void serial_println(const char *str);
void serial_printf(const char *fmt, ...);

#endif // _SERIAL_H
