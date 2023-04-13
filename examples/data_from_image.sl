#!/usr/bin/env slsh

require ("cairo");
require ("png");

define slsh_main ()
{   
   variable err, surface, cr, data_8, data_32, dims, data;

   % read an image file
   surface = cairo_image_surface_create_from_png ("cairo-banner.png");
   
   % get image data as an array of SLANG_UCHAR_TYPE
   data_8 = cairo_image_surface_get_data (surface, 8);
      
   dims = array_shape (data_8);
   
   % image format (for this example) is CAIRO_FORMAT_ARGB32
   % with a little-endian architecture, each pixel is a 4 bytes value
   % 0xBB 0xGG 0xRR 0xAA
   % lets opacify the last byte of each pixel value (A = 0xff)
   data_8 [*, [3:dims [1] - 1:4]] = 0xff;
   % tell cairo that we modified the data buffer
   cairo_surface_mark_dirty (surface);
   % created image should be cairo banner with a black background
   try (err)
     cairo_surface_write_to_png (surface, "banner-2.png");
   catch CairoError:
     {
	vmessage ("%s : %s", err.descr, err.message);
     }   
   
   % get image data as an array of SLANG_UINT_TYPE
   data_32 = cairo_image_surface_get_data (surface, 32);
   % all "uncolored" opaque pixels are now semi-tranparent green
   % pre-multiplied alpha is used : 0x80008000 , not 0x8000ff00
   data_32 [where (data_32 == 0xff000000)] = 0x80008000;
   cairo_surface_mark_dirty (surface);
   % created image should be cairo banner with a semi-tranparent green background
   try (err)
     cairo_surface_write_to_png (surface, "banner-3.png");
   catch CairoError:
     {
	vmessage ("%s : %s", err.descr, err.message);
     }   
   
   % lets make a copy of data_32
   data = @data_32;
   % we don't need the cairo surface any more
   % cairo_surface_destroy (surface);

   % the PNG library don't use pre-multiplied alpha, so "banner-4.png"
   % will not be the same image as "banner-3.png"
   png_write ("banner-4.png", data, 1);
   % lets replace those greenish pixels with fully green semi-transparent ones
   data [where (data == 0x80008000)] = 0x8000ff00;
   png_write ("banner-5.png", data, 1);
}
