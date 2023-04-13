import ("cairo");

typedef struct
{
   x, y, width, height
} cairo_rectangle_t;

typedef struct
{
   x, y, width, height
} cairo_rectangle_int_t;

typedef struct
{
   ascent, descent, height, max_x_advance, max_y_advance
} cairo_font_extents_t;

typedef struct
{
   index, x, y
} cairo_glyph_t;

typedef struct
{
   x_bearing, y_bearing, width, height, x_advance, y_advance
} cairo_text_extents_t;

define cairo_rectangle_new (x, y, w, h) 
{
   variable r = @cairo_rectangle_t;
   r.x = x;
   r.y = y;
   r.width = w;
   r.height = h;
   return r; 
}
 
provide ("cairo");
