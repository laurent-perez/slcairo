#!/usr/bin/env slsh

require ("cairo");

define slsh_main ()
{   
   variable surface, cr, a, width, height, stride, argb, retval, err;

   width = 600;
   height = 300;
   argb = 4 * width;

   % cairo surface from 32 bits data
   a = UInt_Type [height, width];
   % upper third part of the flag is semi-transparent red
   a [[0:height / 3 - 1], *] = 0x80800000;
   % middle part is semi-transparent green
   a [[height / 3:height / 3 * 2 - 1], *] = 0x80008000;
   % lower third part is semi-transparent blue
   a [[height / 3 * 2:height - 1], *] = 0x80000080;
   
   try (err)
     {	
	surface = cairo_image_surface_create_for_data (a);
	cairo_surface_write_to_png (surface, "surface_from_ints.png");
     }
   catch CairoError:
     {
	vmessage ("%s : %s", err.descr, err.message);
     }
   
   % cairo surface from 4 * 1 byte data
   a = UChar_Type [height, argb];
   % upper third part of the flag is semi-transparent red
   a [[0:height / 3 - 1], [0:argb - 1:4]] = 0x00;
   a [[0:height / 3 - 1], [1:argb - 1:4]] = 0x00;
   a [[0:height / 3 - 1], [2:argb - 1:4]] = 0x80;
   a [[0:height / 3 - 1], [3:argb - 1:4]] = 0x80;
   % middle part is semi-transparent green
   a [[height / 3:height / 3 * 2 - 1], [0:argb - 1:4]] = 0x00;
   a [[height / 3:height / 3 * 2 - 1], [1:argb - 1:4]] = 0x80;
   a [[height / 3:height / 3 * 2 - 1], [2:argb - 1:4]] = 0x00;
   a [[height / 3:height / 3 * 2 - 1], [3:argb - 1:4]] = 0x80;
   % lower third part is semi-transparent blue
   a [[height / 3 * 2:height - 1], [0:argb - 1:4]] = 0x80;
   a [[height / 3 * 2:height - 1], [1:argb - 1:4]] = 0x00;
   a [[height / 3 * 2:height - 1], [2:argb - 1:4]] = 0x00;
   a [[height / 3 * 2:height - 1], [3:argb - 1:4]] = 0x80;

   try (err)
     {	
	% we need to specify image width which is 1/4 of array width
	surface = cairo_image_surface_create_for_data (a ; "width" = width);
	cairo_surface_write_to_png (surface, "surface_from_bytes.png");
     }
   catch CairoError:
     {
	vmessage ("%s : %s", err.descr, err.message);
     }

   % cairo surface from float data (R, G, B, A)
   a = Float_Type [height, argb];
   % upper third part of the flag is semi-transparent red
   a [[0:height / 3 - 1], [0:argb - 1:4]] = .5;
   a [[0:height / 3 - 1], [1:argb - 1:4]] = 0;
   a [[0:height / 3 - 1], [2:argb - 1:4]] = 0;
   a [[0:height / 3 - 1], [3:argb - 1:4]] = .5;
   % middle part is semi-transparent green
   a [[height / 3:height / 3 * 2 - 1], [0:argb - 1:4]] = 0;
   a [[height / 3:height / 3 * 2 - 1], [1:argb - 1:4]] = .5;
   a [[height / 3:height / 3 * 2 - 1], [2:argb - 1:4]] = 0;
   a [[height / 3:height / 3 * 2 - 1], [3:argb - 1:4]] = .5;
   % lower third part is semi-transparent blue
   a [[height / 3 * 2:height - 1], [0:argb - 1:4]] = 0;
   a [[height / 3 * 2:height - 1], [1:argb - 1:4]] = 0;
   a [[height / 3 * 2:height - 1], [2:argb - 1:4]] = .5;
   a [[height / 3 * 2:height - 1], [3:argb - 1:4]] = .5;

   try (err)
     {	
	surface = cairo_image_surface_create_for_data (a ; "format" = CAIRO_FORMAT_RGBA128F, "width" = width);
	cairo_surface_write_to_png (surface, "surface_from_floats.png");	
     }
   catch CairoError:
     {
	vmessage ("%s : %s", err.descr, err.message);
     }
}
