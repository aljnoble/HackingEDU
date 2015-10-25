/**
 * Game title screen
 */

#include "results_window.h"

static TextLayer *s_label_layer;
static TextLayer *s_header_layer;
static BitmapLayer *s_icon_layer;

static GBitmap *s_icon_bitmap;

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_LOGO);
  GRect bitmap_bounds = gbitmap_get_bounds(s_icon_bitmap);

  s_header_layer = text_layer_create(GRect(10, 10, 124,
    bounds.size.h - (10 + bitmap_bounds.size.h + 15)));
  text_layer_set_text(s_header_layer, TITLE_WINDOW_HEADER);
  text_layer_set_background_color(s_header_layer, GColorClear);
  text_layer_set_text_alignment(s_header_layer, GTextAlignmentCenter);
  text_layer_set_font(s_header_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  layer_add_child(window_layer, text_layer_get_layer(s_header_layer));

  s_icon_layer = bitmap_layer_create(GRect((bounds.size.w / 2) -
    (bitmap_bounds.size.w / 2), -10, bitmap_bounds.size.w, bitmap_bounds.size.h));
  bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
  bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_icon_layer));

  s_label_layer = text_layer_create(GRect(10, 45 + bitmap_bounds.size.h + 5, 124,
    bounds.size.h - (10 + bitmap_bounds.size.h + 15)));
  text_layer_set_text(s_label_layer, TITLE_WINDOW_MESSAGE);
  text_layer_set_background_color(s_label_layer, GColorClear);
  text_layer_set_text_alignment(s_label_layer, GTextAlignmentCenter);
  text_layer_set_font(s_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));
}

void results_window_unload(Window *window) {
  text_layer_destroy(s_header_layer);
  text_layer_destroy(s_label_layer);
  bitmap_layer_destroy(s_icon_layer);

  gbitmap_destroy(s_icon_bitmap);

  window_destroy(window);
  s_main_window = NULL;
}

void results_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_background_color(s_main_window, GColorTiffanyBlue);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = results_window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}
