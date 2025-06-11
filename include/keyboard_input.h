/**
 * @file keyboard_input.h
 * @brief Low-level keyboard input handling for framebuffer applications
 */

#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include <termios.h>
#include <unistd.h>

/**
 * @brief Get next keyboard input without blocking terminal
 * @return ASCII code of pressed key
 */
int get_next_keyboard_input(void);

#endif

