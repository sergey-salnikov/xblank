#include <X11/Xlib.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Display *display;

void grab_input() {
  XGrabKeyboard(display,
                DefaultRootWindow(display),
                False,
                GrabModeAsync,
                GrabModeAsync,
                CurrentTime);
  XGrabPointer(display,
               DefaultRootWindow(display),
               False,
               0,
               GrabModeAsync,
               GrabModeAsync,
               None,
               None,
               CurrentTime);
}

void wait_for_password(const char *password) {
  const char *next_char = password;

  while (*next_char) {
    XEvent event;
    XNextEvent(display, &event);
    if (event.type == KeyPress)
      if (event.xkey.keycode == XKeysymToKeycode(display, *next_char)) {
        next_char++;
      } else
        next_char = password;
  }
}

void display_off() {
  system("sudo ddcutil setvcp d6 4");
}

void display_on() {
  system("sudo ddcutil setvcp d6 1");
}

int main(int argc, char **argv) {
  if (argc != 2 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
    fprintf(stderr, "Usage: %s PASSWORD\n", argv[0]);
    return 0;
  }

  display = XOpenDisplay(NULL);
  if (!display)
    error(1, 0, "Can't open display\n");

  grab_input();
  display_off();
  wait_for_password(argv[1]);
  display_on();
}
