#!/usr/bin/env slsh

require ("cairo");

define slsh_main ()
{
   variable cr, surface, glyphs, i, num_glyphs, size, x, y;
   
   num_glyphs = 2500; 
   glyphs = cairo_glyph_t [num_glyphs];
   
   i = 0;
   for (y = 0 ; y < 50 ; y ++)
     { 	
	for (x = 0 ; x < 50 ; x ++)
	  {	     
	     glyphs [i].index = i;
	     glyphs [i].x = x * 20 + 20;
	     glyphs [i].y = y * 20 + 20;
	     i ++;
	  }
     }   

   size = 1000;
   
   surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, size, size);
   cr = cairo_create (surface);

   cairo_set_source_rgb (cr, 0, 0, 0);
   cairo_paint (cr);
   
   cairo_set_source_rgb (cr, 0, 1, 0);
   % cairo_select_font_face (cr, "Liberation Sans",
   cairo_select_font_face (cr, "Mono",
   			   CAIRO_FONT_SLANT_NORMAL,
   			   CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size (cr, 10);
   cairo_show_glyphs (cr, glyphs);
   
   cairo_surface_write_to_png (surface, "glyphs.png");
}
