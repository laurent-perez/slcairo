#!/usr/bin/env slsh

require ("cairo");

define slsh_main () 
{   
   variable surface, cr, size;
   
   size = 500;
   
   surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, size, size);
   cr = cairo_create (surface);
   
   cairo_set_source_rgb (cr, 0, 0, 0);
   cairo_move_to (cr, 0, 0);
   cairo_line_to (cr, size, size);
   cairo_move_to (cr, size, 0);
   cairo_line_to (cr, 0, size);
   cairo_set_line_width (cr, 20);
   cairo_stroke (cr);
   
   cairo_rectangle (cr, 0, 0, size / 2, size / 2);
   cairo_set_source_rgba (cr, 1, 0, 0, 0.80);
   cairo_fill (cr);

   cairo_rectangle (cr, 0, size / 2, size / 2, size / 2);
   cairo_set_source_rgba (cr, 0, 1, 0, 0.60);
   cairo_fill (cr);

   cairo_rectangle (cr, size / 2, 0, size / 2, size / 2);
   cairo_set_source_rgba (cr, 0, 0, 1, 0.40);
   cairo_fill (cr);

   cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size (cr, size / 5);

   cairo_move_to (cr, size / 10, size / 2);
   cairo_show_text (cr, "Hello !");
   
   cairo_surface_write_to_png (surface, "drawing-1.png");
}
