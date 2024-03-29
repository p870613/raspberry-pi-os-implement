#include <stdarg.h>
#include <stdio.h>
#include <peripheral/uart.h>
#include <debug_mode.h>
void ltoa (unsigned long val, char *buf)
{
    unsigned long scan = 10000000000000000000u;
    int met_num = 0;
    while (scan != 0)
    {
        *buf = (val / scan) % 10;
        if (*buf != 0)
            met_num = 1;
        if (met_num)
            *buf++ += '0';
        scan /= 10;
    }
    if (!met_num)
        *buf++ = '0';
    *buf = '\0';
}

void ftoa (double val, char *buf)
{
    double scan = 1000000;
    int cnt = 0;
    int met_num = 0;

    while (cnt < 6)
    {
        *buf = (long) (val / scan) % 10;
        if (*buf != 0)
            met_num = 1;
        *buf += '0';
        if (met_num)
            buf++;
        if (scan < 1)
            cnt++;
        if (scan == 1)
        {
            if (!met_num)
            {
                *buf++ = '0';
                met_num = 1;
            }
            *buf++ = '.';
        }
        scan /= 10;
    }
    *buf = 0;
}

void fdivtoa (unsigned long dividend, unsigned long divisor, char *buf)
{
    unsigned long quot = dividend / divisor;
    unsigned long remainder = dividend % divisor;
    int cnt = 0;

    ltoa (quot, buf);
    while (*buf != '\0')
        ++buf;
    *buf++ = '.';
    // postfix 6 points
    remainder *= 1000000;
    buf += 6;
    *buf-- = '\0';
    quot = remainder / divisor;
    for (cnt = 0; cnt < 6; cnt++)
    {
        *buf-- = quot % 10 + '0';
        quot /= 10;
    }
}

int printk(const char *format, ...) {
#ifndef DEBUG
    return -1;
#endif
    char buf[STRBUF_SIZE];
    va_list args;
    double val_double;
    unsigned long val_ulong;
    unsigned long val_hex;
    unsigned long dividend, divisor;
    va_start (args, format);
    while (*format != '\0')
    {
        if (*format == '%')
        {
            ++format;
            switch (*format)
            {
                case 'd':
                    val_ulong = va_arg (args, unsigned long);
                    ltoa (val_ulong, buf);
                    uart_put (buf);
                    break;
                case 'q':
                    // kernel space float point
                    dividend = va_arg (args, unsigned long);
                    divisor = va_arg (args, unsigned long);
                    fdivtoa (dividend, divisor, buf);
                    uart_put (buf);
                    break;
                case 'f':
                    // user space float point
                    val_double = va_arg (args, double);
                    ftoa (val_double, buf);
                    uart_put (buf);
                    break;
                case 's':
                    uart_put (va_arg (args, char *));
                    break;
                case '%':
                    uart_send ('%');
                    break;
                case 'x':
                    val_hex = va_arg (args, unsigned long);
                    uart_hex (val_hex);
                    break;
                case 'p':
                    val_hex = va_arg (args, unsigned long);
                    uart_hex (val_hex >> 32);
                    uart_hex (val_hex);
                    break;
                default:
                    buf[0] = '\0';
                    break;
            }
        }
        else
        {
            if(*format == '\n')
                uart_send('\r');
            uart_send (*format);
        }
        ++format;
    }
    va_end (args);
    return 0;
}

int printf (const char *format, ...)
{
    char buf[STRBUF_SIZE];
    va_list args;
    double val_double;
    unsigned long val_ulong;
    unsigned long val_hex;
    unsigned long dividend, divisor;
    va_start (args, format);
    while (*format != '\0')
    {
        if (*format == '%')
        {
            ++format;
            switch (*format)
            {
                case 'd':
                    val_ulong = va_arg (args, unsigned long);
                    ltoa (val_ulong, buf);
                    uart_put (buf);
                    break;
                case 'q':
                    // kernel space float point
                    dividend = va_arg (args, unsigned long);
                    divisor = va_arg (args, unsigned long);
                    fdivtoa (dividend, divisor, buf);
                    uart_put (buf);
                    break;
                case 'f':
                    // user space float point
                    val_double = va_arg (args, double);
                    ftoa (val_double, buf);
                    uart_put (buf);
                    break;
                case 's':
                    uart_put (va_arg (args, char *));
                    break;
                case '%':
                    uart_send ('%');
                    break;
                case 'x':
                    val_hex = va_arg (args, unsigned long);
                    uart_hex (val_hex);
                    break;
                case 'p':
                    val_hex = va_arg (args, unsigned long);
                    uart_hex (val_hex >> 32);
                    uart_hex (val_hex);
                    break;
                default:
                    buf[0] = '\0';
                    break;
            }
        }
        else
        {
            if(*format == '\n')
                uart_send('\r');
            uart_send (*format);
        }
        ++format;
    }
    va_end (args);
    return 0;
}
