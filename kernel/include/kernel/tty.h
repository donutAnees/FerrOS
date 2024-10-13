#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

/**
 * @brief Initializes the terminal with space characters and color.
 */
void terminal_initialize(void);

/**
 * @brief Writes a character to the terminal at the current cursor.
 *
 * @param c The character to write to the screen.
 */
void terminal_putchar(char c);

/** 
 * @brief Write a specified number of charaters to the terminal.
 *
 * @param data Pointer to the array of character to write.
 * @param size The size of the array.
 */
void terminal_write(const char* data, size_t size);

/**
 * @brief Write a null terminated string to the terminal.
 *
 * @param data Pointer to a null terminated string.
 */
void terminal_writestring(const char* data);

/**
 * @brief Move the cursor to the next line.
 */
void terminal_newline();

/*
*@brief Remove the last character from the terminal
*/
void terminal_rmchar();

#endif
