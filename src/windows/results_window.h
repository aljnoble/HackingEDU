#pragma once

#include <pebble.h>

#define TITLE_WINDOW_HEADER " "
#define TITLE_WINDOW_MESSAGE  "Press to start"

extern Window *s_main_window;

void title_window_push();
void title_window_unload();
