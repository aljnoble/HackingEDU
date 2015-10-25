#include <pebble.h>
#include "windows/dialog_choice_window.h"
#include "windows/title_window.h"

#define NUM_SAMPLES 1

#define NUM_WINDOWS 10

static Window *s_main_window;
static TextLayer *s_output_layer;
static int32_t max_accel = 0;

static AccelData get_accel_avg(AccelData *data) {
  static AccelData accel_avg;
  accel_avg.x = 0;
  accel_avg.y = 0;
  accel_avg.z = 0;
  for (size_t i = 0; i < NUM_SAMPLES; ++i) {
    accel_avg.x += data[i].x / NUM_SAMPLES;
    accel_avg.y += data[i].y / NUM_SAMPLES;
    accel_avg.z += data[i].z / NUM_SAMPLES;
  }
  return accel_avg;
}

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];
  AccelData avg_accel = get_accel_avg(data);
  int32_t sum = avg_accel.x + avg_accel.y + avg_accel.z;
  if (sum > max_accel)
    max_accel = sum;
  // Compose string of all data
  snprintf(s_buffer, sizeof(s_buffer),
    "N X,Y,Z\n0 %d,%d,%d\n\nsum %lu",
    data[0].x, data[0].y, data[0].z,
    max_accel
  );

  //Show the data
  //text_layer_set_text(s_output_layer, s_buffer);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_layer, "No data yet.");
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  max_accel = 0;
  window_unload(s_main_window);
  dialog_choice_window_push();
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  // window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  // window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_set_click_config_provider(s_main_window, click_config_provider);
  title_window_push();

  // Subscribe to the accelerometer data service
  int num_samples = NUM_SAMPLES;
  accel_data_service_subscribe(num_samples, data_handler);

  // Choose update rate
  accel_service_set_sampling_rate(ACCEL_SAMPLING_100HZ);
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
  accel_data_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
