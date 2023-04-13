#!/usr/bin/env slsh

require ("cairo");

define slsh_main () 
{   
   variable surface, cr;

   surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 600, 400);
   cr = cairo_create (surface);

   cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size (cr, 20.0);
   cairo_set_source_rgba (cr, 0, 1, 0, 1);
   cairo_move_to (cr, 10.0, 50.0);
   cairo_show_text (cr, "Hello !");

   cairo_new_sub_path (cr);
   cairo_set_source_rgba (cr, 1, 0, 0, 1);
   cairo_arc (cr, 300, 200, 50, 0, 2 * PI);
   cairo_set_line_width (cr, 5);
   cairo_stroke_preserve (cr);
   cairo_set_source_rgba (cr, 0, 0, 1, .5);
   cairo_fill (cr);
   
   cairo_surface_write_to_png (surface, "drawing-2.png");
}

