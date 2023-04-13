#!/usr/bin/env slsh

require ("cairo");

define show_matrix (m)
{
   vmessage ("xx = %.3f ; xy = %.3f ; x0 = %.3f", m [0, 0], m [0, 1], m [0, 2]);
   vmessage ("yx = %.3f ; yy = %.3f ; y0 = %.3f", m [1, 0], m [1, 1], m [1, 2]);
}

define slsh_main ()
{   
   variable surface, cr, matrix, x, y, err;

   surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 1000, 500);
   cr = cairo_create (surface);
   cairo_set_source_rgb (cr, 0, 1, 0);
   cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size (cr, 20.0);

   matrix = cairo_array_to_matrix ();
   cairo_get_matrix (cr, matrix);
   message ("initial matrix :");
   show_matrix (cairo_matrix_to_array (matrix));

   cairo_move_to (cr, 50, 400);
   cairo_show_text (cr, "move_to (50, 400)");
   cairo_get_matrix (cr, matrix);
   message ("move_to (50, 400)");
   show_matrix (cairo_matrix_to_array (matrix));

   cairo_rotate (cr, -PI/4);
   cairo_show_text (cr, "rotate (-PI/4)");
   cairo_get_matrix (cr, matrix);
   message ("rotate (-PI/4)");
   show_matrix (cairo_matrix_to_array (matrix));

   cairo_translate (cr, 100, -200);
   cairo_show_text (cr, "translate (100, -200)");
   cairo_get_matrix (cr, matrix);
   message ("translate (100, -200)");
   show_matrix (cairo_matrix_to_array (matrix));

   cairo_matrix_init_translate (matrix, 50, -100);
   cairo_set_matrix (cr, matrix);
   cairo_show_text (cr, "translate (50, -100)");
   cairo_get_matrix (cr, matrix);
   message ("translate (50, -100)");
   show_matrix (cairo_matrix_to_array (matrix));
   
   cairo_matrix_init_rotate (matrix, PI / 2);
   cairo_set_matrix (cr, matrix);
   cairo_show_text (cr, "rotate (PI/2)");
   cairo_get_matrix (cr, matrix);
   message ("rotate (PI/2)");
   show_matrix (cairo_matrix_to_array (matrix));

   try (err)
     cairo_surface_write_to_png (surface, "transform.png");
   catch CairoError:
     {
	vmessage ("%s : %s", err.descr, err.message);
     }
   
   message ("");
   
   cairo_matrix_init_rotate (matrix, PI / 4);
   (x, y) = cairo_matrix_transform_distance (matrix, 1, 1);
   vmessage ("x = %.3f ; y = %.3f", x, y);   
   (x, y) = cairo_matrix_transform_point (matrix, 1, 1);
   vmessage ("x = %.3f ; y = %.3f", x, y);
   
   (x, y) = cairo_user_to_device (cr, 1, 1);
   vmessage ("x = %.3f ; y = %.3f", x, y);
   (x, y) = cairo_device_to_user (cr, x, y);
   vmessage ("x = %.3f ; y = %.3f", x, y);

   (x, y) = cairo_user_to_device_distance (cr, 1, 1);
   vmessage ("x = %.3f ; y = %.3f", x, y);
   (x, y) = cairo_device_to_user_distance (cr, x, y);
   vmessage ("x = %.3f ; y = %.3f", x, y);
}
