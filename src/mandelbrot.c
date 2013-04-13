#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x3D, 0xFB, 0x35, 0x50, 0xC9, 0xF1, 0x43, 0x1E, 0xA0, 0xE6, 0xF7, 0x17, 0x3C, 0x98, 0x2C, 0x82 }
PBL_APP_INFO(MY_UUID,
             "Mandelbrot", "Your Company",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

#define ITERATION_LIMIT 10

Window window;

Layer layer;

int in_set(double initial_x, double initial_y) {
  double x = 0.0;
  double y = 0.0;

  int iteration = 0;
  while (x * x + y * y < 4.0 && iteration < ITERATION_LIMIT) {
    double new_x = (x * x) - (y * y) + initial_x;
    y = (2.0 * x * y) + initial_y;
    x = new_x;
    iteration++;
  }

  return iteration == ITERATION_LIMIT;
}

void render_region(GContext* ctx, double x_start, double y_start, double x_end, double y_end) {
  double x_scale = (x_end - x_start) / 144;
  double y_scale = (y_end - y_start) / 168;

  for(int x = 0; x < 144; x++) {
    for(int y = 0; y < 168; y++) {
      double x0 = (x * x_scale) + x_start;
      double y0 = (y * y_scale) + y_start;

      if(in_set(x0, y0)) {
        graphics_context_set_stroke_color(ctx, GColorBlack);
      } else {
        graphics_context_set_stroke_color(ctx, GColorWhite);
      }

      graphics_draw_pixel(ctx, GPoint(x, y));
    }
  }
}

void mandelbrot_callback(Layer *layer, GContext* ctx) {
  render_region(ctx, -2.5, -2.0, 1.0, 2.0);
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Mandelbrot");
  window_stack_push(&window, false);

  layer_init(&layer, window.layer.frame);
  layer.update_proc = mandelbrot_callback;
  layer_add_child(&window.layer, &layer);
  layer_mark_dirty(&layer);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
