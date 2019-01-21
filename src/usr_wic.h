//----------------------------------------------------------------------//
// WIC ŠÖ˜A																//
//																		//
//----------------------------------------------------------------------//
#ifndef UserWicH
#define UserWicH

//---------------------------------------------------------------------------
#include <shlobj.h>

//---------------------------------------------------------------------------
#define WICIMG_FRAME		0
#define WICIMG_PREVIEW		1
#define WICIMG_THUMBNAIL	2

//---------------------------------------------------------------------------
extern UnicodeString WIC_FextStr;

//---------------------------------------------------------------------------
UnicodeString WIC_get_fext_str();
bool WIC_get_from_memory(TMemoryStream *ms, Graphics::TBitmap *o_bmp, GUID fmt);
bool WIC_load_image(UnicodeString fnam, Graphics::TBitmap *o_bmp, int img_type = WICIMG_PREVIEW);
bool WIC_save_image(UnicodeString fnam, Graphics::TBitmap *i_bmp, 
	int img_q = 100, int ycrcb = 0, int cmp_mod = 0);
bool WIC_resize_image(Graphics::TBitmap *i_bmp, Graphics::TBitmap *o_bmp,
	float ratio, unsigned int s_wd = 0, unsigned int s_hi = 0, int s_opt = 0);
bool WIC_rotate_image(Graphics::TBitmap *i_bmp, int rot_opt);
bool WIC_grayscale_image(Graphics::TBitmap *i_bmp);
bool WIC_get_img_size(UnicodeString fnam, unsigned int *wd, unsigned int *hi);

//---------------------------------------------------------------------------

#endif
