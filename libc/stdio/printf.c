#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// Function to print the given data.
static bool print(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;
    return true;
}

// Function to convert size_t to string
static void size_to_string(size_t size, char* buffer) {
    char* ptr = buffer;
    if (size == 0) {
        *ptr++ = '0';
    } else {
        size_t temp = size;
        while (temp > 0) {
            temp /= 10;
            ptr++;
        }
        *ptr = '\0'; // Null-terminate the string

        while (size > 0) {
            *--ptr = (size % 10) + '0'; // Convert digit to char
            size /= 10;
        }
    }
}

// Custom printf function
int printf(const char* restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, amount))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format++;

        // Handle char
        if (*format == 'c') {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(&c, sizeof(c)))
                return -1;
            written++;
        } 
        // Handle string
        else if (*format == 's') {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(str, len))
                return -1;
            written += len;
        } 
        // Handle sizeof (new functionality)
        else if (*format == 'z') {  // Use %z for sizeof
            format++;
            size_t size = va_arg(parameters, size_t);
            char buffer[20]; // Buffer to hold string representation of the size
            size_to_string(size, buffer); // Convert size to string
            size_t len = strlen(buffer); // Get length of the string
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(buffer, len)) 
                return -1;
            written += len;
        } 
        else {
            // Unknown format specifier, revert to normal printing
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, len))
                return -1;
            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}
