#pragma once

#include <pebble.h>

#define RESULTS_WINDOW_HEADER "PUNCH\nNOW"
#define RESULTS_WINDOW_MESSAGE  ""

//extern Window *s_main_window;
extern int32_t max_accel;

void set_results_speed(double speed);
void results_window_push();
void results_window_unload();
