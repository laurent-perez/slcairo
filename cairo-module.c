#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-ps.h>
#include <cairo/cairo-svg.h>
#include <slang.h>

static const unsigned int _cairo_version = 10000 * CAIRO_VERSION_MAJOR +
  100 * CAIRO_VERSION_MINOR + CAIRO_VERSION_MICRO;

static int Cairo_Error = -1;

int usage_err (int expected_nargs, const char *usage_str)
{
   if (SLang_Num_Function_Args < expected_nargs) 
     {
	int npop = SLstack_depth ();
	if (npop > SLang_Num_Function_Args) npop = SLang_Num_Function_Args;
	SLdo_pop_n (npop);
	SLang_verror (SL_USAGE_ERROR, "Usage : %s", usage_str);
	return -1;
     }
   return 0;
}

SLang_CStruct_Field_Type cairo_text_cluster_t_Layout [] =
{
   MAKE_CSTRUCT_FIELD(cairo_text_cluster_t, num_bytes, "num_bytes", SLANG_INT_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_text_cluster_t, num_glyphs, "num_glyphs", SLANG_INT_TYPE, 0),
   SLANG_END_CSTRUCT_TABLE
};   

SLang_CStruct_Field_Type cairo_font_extents_t_Layout [] =
{
   MAKE_CSTRUCT_FIELD(cairo_font_extents_t, ascent, "ascent", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_font_extents_t, descent, "descent", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_font_extents_t, height, "height", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_font_extents_t, max_x_advance, "max_x_advance", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_font_extents_t, max_y_advance, "max_y_advance", SLANG_DOUBLE_TYPE, 0),
   SLANG_END_CSTRUCT_TABLE
};

SLang_CStruct_Field_Type cairo_glyph_t_Layout [] =
{
   MAKE_CSTRUCT_FIELD(cairo_glyph_t, index, "index", SLANG_ULONG_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_glyph_t, x, "x", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_glyph_t, y, "y", SLANG_DOUBLE_TYPE, 0),
   SLANG_END_CSTRUCT_TABLE
};   

SLang_CStruct_Field_Type cairo_rectangle_t_Layout [] =
{
   MAKE_CSTRUCT_FIELD(cairo_rectangle_t, x, "x", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_rectangle_t, x, "y", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_rectangle_t, x, "width", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_rectangle_t, x, "height", SLANG_DOUBLE_TYPE, 0),
   SLANG_END_CSTRUCT_TABLE
};

SLang_CStruct_Field_Type cairo_rectangle_int_t_Layout [] =
{
   MAKE_CSTRUCT_FIELD(cairo_rectangle_int_t, x, "x", SLANG_INT_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_rectangle_int_t, x, "y", SLANG_INT_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_rectangle_int_t, x, "width", SLANG_INT_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_rectangle_int_t, x, "height", SLANG_INT_TYPE, 0),
   SLANG_END_CSTRUCT_TABLE
};

SLang_CStruct_Field_Type cairo_text_extents_t_Layout [] =
{
   MAKE_CSTRUCT_FIELD(cairo_text_extents_t, x_bearing, "x_bearing", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_text_extents_t, y_bearing, "y_bearing", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_text_extents_t, width, "width", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_text_extents_t, height, "height", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_text_extents_t, x_advance, "x_advance", SLANG_DOUBLE_TYPE, 0),
   MAKE_CSTRUCT_FIELD(cairo_text_extents_t, y_advance, "y_advance", SLANG_DOUBLE_TYPE, 0),
   SLANG_END_CSTRUCT_TABLE
};

#include "cairo_glue.c"

static void sl_cairo_matrix_to_array (void)
{
   cairo_matrix_t *matrix;
   Slirp_Opaque* matrix_o = NULL;
   SLang_Array_Type *at;
   SLindex_Type dims [2];
   
   if (usage_err (1, "Double [2, 3] = cairo_matrix_to_array (Cairo)"))
     return;

   if (-1 == SLang_pop_opaque (CairoMatrix_Type, (void**) &matrix, &matrix_o))
     return;

   dims [0] = 2;
   dims [1] = 3;
   
   at = SLang_create_array (SLANG_DOUBLE_TYPE, 0, NULL, dims, 2);
   if (at == NULL)
     goto cleanup_2;
   
   dims [0] = 0; dims [1] = 0;
   if (-1 == SLang_set_array_element (at, dims, &matrix->xx))
     goto cleanup;
   dims [0] = 0; dims [1] = 1;
   if (-1 == SLang_set_array_element (at, dims, &matrix->xy))
     goto cleanup;
   dims [0] = 0; dims [1] = 2;
   if (-1 == SLang_set_array_element (at, dims, &matrix->x0))
     goto cleanup;
   dims [0] = 1; dims [1] = 0;
   if (-1 == SLang_set_array_element (at, dims, &matrix->yx))
     goto cleanup;
   dims [0] = 1; dims [1] = 1;
   if (-1 == SLang_set_array_element (at, dims, &matrix->yy))
     goto cleanup;
   dims [0] = 1; dims [1] = 2;
   if (-1 == SLang_set_array_element (at, dims, &matrix->y0))
     goto cleanup;
   
   (void) SLang_push_array (at, 0);
   
cleanup:
   SLang_free_array (at);
cleanup_2:   
   SLang_free_opaque (matrix_o);
}

static void sl_cairo_array_to_matrix (void)
{
   double xx = 1, yx = 0, xy = 0, yy = 1, x0 = 0, y0 = 0;
   cairo_matrix_t *matrix;
   SLang_Array_Type *at = NULL;
   SLindex_Type dims [2];
   
   if (usage_err (0, "(CairoMatrix = cairo_array_to_matrix ([])"))
       return;
   
   if (SLang_Num_Function_Args == 1)
     {
	if (-1 == SLang_pop_array_of_type (&at, SLANG_DOUBLE_TYPE))
	  return;
	if (at->num_dims != 2 || at->dims [0] != 2 || at->dims [1] !=3)
	  {
	     (void) printf ("Array must be Double [2, 3]\n");
	     goto cleanup;
	  }	
	dims [0] = 0; dims [1] = 0;
	if (-1 == SLang_get_array_element (at, dims, &xx))
	  goto cleanup;
	dims [0] = 0; dims [1] = 1;
	if (-1 == SLang_get_array_element (at, dims, &xy))
	  goto cleanup;	
	dims [0] = 0; dims [1] = 2;
	if (-1 == SLang_get_array_element (at, dims, &x0))
	  goto cleanup;
	dims [0] = 1; dims [1] = 0;
	if (-1 == SLang_get_array_element (at, dims, &yx))
	  goto cleanup;
	dims [0] = 1; dims [1] = 1;
	if (-1 == SLang_get_array_element (at, dims, &yy))
	  goto cleanup;
	dims [0] = 1; dims [1] = 2;
	if (-1 == SLang_get_array_element (at, dims, &y0))
	  goto cleanup;
     }
   
   matrix = (cairo_matrix_t *) SLmalloc (sizeof (cairo_matrix_t));
   if (matrix == NULL)
     goto cleanup;
   
   cairo_matrix_init (matrix, xx, yx, xy, yy, x0, y0);
   
   (void) SLang_push_opaque (CairoMatrix_Type, matrix, 1);

cleanup:
   SLang_free_array (at);
}

static void sl_cairo_image_surface_create_for_data (void)
{
   cairo_surface_t* retval;
   cairo_format_t format;
   int width, height, stride;
   SLang_Array_Type *at;

   if (usage_err (1, "Cairo = cairo_image_surface_create_for_data (Array ; qualifiers)"))
     return;
   
   if (-1 == SLang_pop_array (&at))
     return;

   if (at->num_dims != 2)      
     {
	SLang_verror (SL_InvalidParm_Error, "Expecting a 2-d array");
	SLang_free_array (at);
	return;      
     }

   if (-1 == SLang_get_int_qualifier ("format", &format, CAIRO_FORMAT_ARGB32))
     return;
   
   if (-1 == SLang_get_int_qualifier ("width", &width, at->dims [1]))
     return;
   
   height = at->dims [0];
   stride = cairo_format_stride_for_width (format, width);
   
   retval = cairo_image_surface_create_for_data ((unsigned char *) at->data, format, width, height, stride);

   if (CAIRO_STATUS_SUCCESS != cairo_surface_status (retval))
     {
	SLang_verror (Cairo_Error, "Invalid Cairo surface : %s",
		      cairo_status_to_string (cairo_surface_status (retval)));
	SLang_free_array (at);
     }
   (void) SLang_push_opaque (CairoSurface_Type, (void*) retval, 0);
}

static void sl_cairo_image_surface_get_data (void)
{
   cairo_surface_t* surface;
   Slirp_Opaque* surface_o = NULL;
   int height, width, stride, fmt;
   unsigned char *data;
   SLang_Array_Type *at;
   SLindex_Type dims [2];
   
   if (usage_err (1, "(Array = cairo_image_surface_get_data (Cairo, int)"))
     return;

   if (-1 == SLang_pop_int ((int*) &fmt))
     return;
   if (-1 == SLang_pop_opaque (CairoSurface_Type, (void**) &surface, &surface_o))
     return;

   /* flush to ensure all writing to the image was done */
   cairo_surface_flush (surface);
   data = cairo_image_surface_get_data (surface);
   height = cairo_image_surface_get_height (surface);
   width = cairo_image_surface_get_width (surface);
   stride = cairo_image_surface_get_stride (surface);

   dims [0] = height;
   if (fmt == 8)
     {
	dims [1] = stride;	
	at = SLang_create_array (SLANG_UCHAR_TYPE, 0, (VOID_STAR) data, dims, 2);
	if (at == NULL)
	  goto cleanup;
     }
   else if (fmt == 32)
     {
	dims [1] = width;
	at = SLang_create_array (SLANG_UINT_TYPE, 0, (VOID_STAR) data, dims, 2);
	if (at == NULL)
	  goto cleanup;
     }   

   (void) SLang_push_array (at, 0);
   
cleanup:
   SLang_free_opaque (surface_o);
}

static void sl_cairo_matrix_transform_distance (void)
{
   cairo_matrix_t *matrix;
   Slirp_Opaque* matrix_o = NULL;
   double dx, dy;
   
   if (-1 == SLang_pop_double (&dy))
     return;
   if (-1 == SLang_pop_double (&dx))
     return;
   if (-1 == SLang_pop_opaque (CairoMatrix_Type, (void**) &matrix, &matrix_o))
     return;   
   
   cairo_matrix_transform_distance (matrix, &dx, &dy);

   (void) SLang_push_double (dx);
   (void) SLang_push_double (dy);
   
   SLang_free_opaque (matrix_o);
}

static void sl_cairo_matrix_transform_point (void)
{
   cairo_matrix_t *matrix;
   Slirp_Opaque* matrix_o = NULL;
   double dx, dy;
   
   if (-1 == SLang_pop_double (&dy))
     return;
   if (-1 == SLang_pop_double (&dx))
     return;
   if (-1 == SLang_pop_opaque (CairoMatrix_Type, (void**) &matrix, &matrix_o))
     return;   
   
   cairo_matrix_transform_point (matrix, &dx, &dy);

   (void) SLang_push_double (dx);
   (void) SLang_push_double (dy);
   
   SLang_free_opaque (matrix_o);
}

static void sl_cairo_user_to_device (void)
{
   cairo_t *cr;
   Slirp_Opaque* cr_o = NULL;
   double dx, dy;
   
   if (-1 == SLang_pop_double (&dy))
     return;
   if (-1 == SLang_pop_double (&dx))
     return;
   if (-1 == SLang_pop_opaque (Cairo_Type, (void**) &cr, &cr_o))
     return;   
   
   cairo_user_to_device (cr, &dx, &dy);

   (void) SLang_push_double (dx);
   (void) SLang_push_double (dy);
   
   SLang_free_opaque (cr_o);
}

static void sl_cairo_user_to_device_distance (void)
{
   cairo_t *cr;
   Slirp_Opaque* cr_o = NULL;
   double dx, dy;
   
   if (-1 == SLang_pop_double (&dy))
     return;
   if (-1 == SLang_pop_double (&dx))
     return;
   if (-1 == SLang_pop_opaque (Cairo_Type, (void**) &cr, &cr_o))
     return;   
   
   cairo_user_to_device_distance (cr, &dx, &dy);

   (void) SLang_push_double (dx);
   (void) SLang_push_double (dy);
   
   SLang_free_opaque (cr_o);
}

static void sl_cairo_device_to_user (void)
{
   cairo_t *cr;
   Slirp_Opaque* cr_o = NULL;
   double dx, dy;
   
   if (-1 == SLang_pop_double (&dy))
     return;
   if (-1 == SLang_pop_double (&dx))
     return;
   if (-1 == SLang_pop_opaque (Cairo_Type, (void**) &cr, &cr_o))
     return;   
   
   cairo_device_to_user (cr, &dx, &dy);

   (void) SLang_push_double (dx);
   (void) SLang_push_double (dy);
   
   SLang_free_opaque (cr_o);
}

static void sl_cairo_device_to_user_distance (void)
{
   cairo_t *cr;
   Slirp_Opaque* cr_o = NULL;
   double dx, dy;
   
   if (-1 == SLang_pop_double (&dy))
     return;
   if (-1 == SLang_pop_double (&dx))
     return;
   if (-1 == SLang_pop_opaque (Cairo_Type, (void**) &cr, &cr_o))
     return;   
   
   cairo_device_to_user_distance (cr, &dx, &dy);

   (void) SLang_push_double (dx);
   (void) SLang_push_double (dy);
   
   SLang_free_opaque (cr_o);
}

static void sl_cairo_show_glyphs (void)
{
   unsigned long index;
   double x, y;
   cairo_glyph_t *glyphs;
   cairo_t *cr;
   Slirp_Opaque* cr_o = NULL;
   SLang_Array_Type *at = NULL;
   SLindex_Type i;
   SLang_Struct_Type *st;
   
   if (-1 == SLang_pop_array_of_type (&at, SLANG_STRUCT_TYPE))
     return;
   if (-1 == SLang_pop_opaque (Cairo_Type, (void**) &cr, &cr_o))
     goto cleanup_3;

   glyphs = (cairo_glyph_t *) SLmalloc (at->num_elements * sizeof (cairo_glyph_t));
   if (glyphs == NULL)
     goto cleanup_2;

   for (i = 0 ; i < at->num_elements ; i ++)
     {
	if (-1 == SLang_get_array_element (at, &i, &st))
	  goto cleanup;
	if (-1 == SLang_push_struct_field (st, "index"))
	  goto cleanup;
	if (-1 == SLang_pop_ulong ((unsigned long *) &index))
	  goto cleanup;
	glyphs [i].index = index;
	if (-1 == SLang_push_struct_field (st, "x"))
	  goto cleanup;
	if (-1 == SLang_pop_double ((double *) &x))
	  goto cleanup;
	glyphs [i].x = x;
	if (-1 == SLang_push_struct_field (st, "y"))
	  goto cleanup;
	if (-1 == SLang_pop_double ((double *) &y))
	  goto cleanup;
	glyphs [i].y = y;
     }

   cairo_show_glyphs (cr, glyphs, i);   
   
cleanup:
   SLfree ((char *) glyphs);
cleanup_2:
   SLang_free_opaque (cr_o);
cleanup_3:
   SLang_free_array (at);
}

static SLang_Intrin_Fun_Type cairo_t_Funcs [] =
{
   MAKE_INTRINSIC_0 ("cairo_matrix_to_array", sl_cairo_matrix_to_array, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_array_to_matrix", sl_cairo_array_to_matrix, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_image_surface_create_for_data", sl_cairo_image_surface_create_for_data, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_image_surface_get_data", sl_cairo_image_surface_get_data, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_matrix_transform_distance", sl_cairo_matrix_transform_distance, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_matrix_transform_point", sl_cairo_matrix_transform_point, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_user_to_device", sl_cairo_user_to_device, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_user_to_device_distance", sl_cairo_user_to_device_distance, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_device_to_user", sl_cairo_device_to_user, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_device_to_user_distance", sl_cairo_device_to_user_distance, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_0 ("cairo_show_glyphs", sl_cairo_show_glyphs, SLANG_VOID_TYPE),
   /* MAKE_INTRINSIC_0 ("", sl_, SLANG_VOID_TYPE), */
   SLANG_END_INTRIN_FUN_TABLE
};

static SLang_Intrin_Var_Type cairo_Vars[] =
{
   MAKE_VARIABLE("_cairo_version", &_cairo_version, SLANG_UINT_TYPE, 1),
   SLANG_END_INTRIN_VAR_TABLE
};

SLANG_MODULE(cairo);
int init_cairo_module_ns(char *ns_name)	/* {{{ */
{
   SLang_NameSpace_Type *ns = NULL;

   if (slang_abi_mismatch()) return -1;
   if (ns_name != NULL) {
	ns = SLns_create_namespace (ns_name);
       if (ns == NULL ||
          (slns = SLmalloc(strlen(ns_name)+1)) == NULL)
          return -1;
       strcpy(slns, ns_name);
   }

   if ((-1 == Cairo_Error)
       && (-1 == (Cairo_Error = SLerr_new_exception (SL_RunTime_Error, "CairoError", "cairo library error"))))
     return -1;
   
   (void) &ref_get_size; /* avoid compile warn if unused */

   if (allocate_reserved_opaque_types() == -1) return -1;
   if (allocate_cairo_opaque_types() == -1) return -1;

#ifdef HAVE_OPAQUE_IVARS
   if (-1 == set_opaque_ivar_types(cairo_Opaque_IVars) ||
       -1 == SLns_add_intrin_var_table(ns,cairo_Opaque_IVars,NULL))
	return -1;
#endif

   if ( -1 == SLns_add_iconstant_table(ns,cairo_IConsts,NULL) ||
	-1 == SLns_add_intrin_var_table(ns,cairo_IVars,NULL) ||
	-1 == SLns_add_intrin_var_table(ns,cairo_Vars,NULL) ||
	-1 == SLns_add_intrin_fun_table (ns,cairo_t_Funcs,(char*)"__cairo__") ||
	-1 == SLns_add_intrin_fun_table (ns,cairo_Funcs,(char*)"__cairo__"))
	return -1;

   return 0;
} /* }}} */
