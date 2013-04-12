#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x3D, 0xFB, 0x35, 0x50, 0xC9, 0xF1, 0x43, 0x1E, 0xA0, 0xE6, 0xF7, 0x17, 0x3C, 0x98, 0x2C, 0x82 }
PBL_APP_INFO(MY_UUID,
             "Mandelbrot", "Your Company",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;

Layer layer;

void mandelbrot_callback(Layer *layer, GContext* ctx) {
  (void)layer;

  double xScale = 3.5 / 144;
  double yScale = 2.0 / 168;

  for(int x = 0; x < 144; x++) {
    for(int y = 0; y < 168; y++) {
      double x0 = (x * xScale) - 2.5;
      double y0 = (y * yScale) - 1.0;

      double xBlah = 0.0;
      double yBlah = 0.0;

      int iteration = 0;
      while((xBlah * xBlah) + (yBlah * yBlah) < 4.0 && iteration < 10) {
        double xTemp = (xBlah * xBlah) - (yBlah * yBlah) + x0;
        yBlah = (2.0 * xBlah * yBlah) + y0;
        xBlah = xTemp;
        iteration++;
      }

      if(iteration < 10) {
        graphics_context_set_stroke_color(ctx, GColorWhite);
      } else {
        graphics_context_set_stroke_color(ctx, GColorBlack);
      }

      graphics_draw_pixel(ctx, GPoint(x, y));
    }
  }
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
