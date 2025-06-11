#include "../../include/keyboard_input.h"
#include "keypress.h"
#include <stdio.h>

int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
// Bridge function for new interface compatibility
int get_next_keyboard_input(void) {
    return getch();
}
