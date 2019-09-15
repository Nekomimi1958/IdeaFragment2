//----------------------------------------------------------------------//
//	汎用共通関数														//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrFuncH
#define UsrFuncH

//---------------------------------------------------------------------------
#include <System.StrUtils.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>

//---------------------------------------------------------------------------
#define UCAT_TSTR(str)	cat_sprintf(_T("%s"), _T(str))

//---------------------------------------------------------------------------
//8点長方形(四隅と各辺の中点)
struct octrect {
	union {
		struct {
			POINT p[8];
		};
		struct {
			POINT lt;
			POINT ct;
			POINT rt;
			POINT rc;
			POINT rb;
			POINT cb;
			POINT lb;
			POINT lc;
		};
	};
};

//---------------------------------------------------------------------------
void set_octrect(TRect *rc, octrect *xrc);
bool is_octrect_empty(octrect *xrc);
void inflate_octrect(octrect *xrc, int mgx, int mgy);

//---------------------------------------------------------------------------
bool pt_on_border(int x, int y, TRect rc, int w);
bool pt_on_corner(int x, int y, TRect rc, int w);
void scale_rect(TRect *rc, double ratio);
void set_rect_ex(TRect *rc, int x0, int y0, int x1, int y1);

int  get_FontHeight(TCanvas *cv, int mgn = 0);
int  get_FontHeight(TFont *font, int mgn = 0);
int  get_FontHeightP(TFont *font, int per = 0);
int  get_FontWidthF(TCanvas *cv);

int  round_i(double v);

UnicodeString get_tkn(UnicodeString s, UnicodeString sp);
UnicodeString get_tkn_r(UnicodeString s, UnicodeString sp);
UnicodeString split_tkn(UnicodeString &s, UnicodeString sp);
bool remove_text(UnicodeString &s, UnicodeString w);
UnicodeString trim_ex(UnicodeString src);
int  pos_i(UnicodeString wd, UnicodeString s);
int  pos_r(UnicodeString wd, UnicodeString s);
UnicodeString exclude_quot(UnicodeString s);
UnicodeString add_quot_if_spc(UnicodeString s);

bool chk_RegExPtn(UnicodeString ptn);

void get_find_wd_list(UnicodeString wd, TStringList *lst);
bool find_mlt(UnicodeString  wd, UnicodeString s, bool and_sw, bool not_sw, bool case_sw = false);

void color_textout(TCanvas *cv, int xp, int yp, UnicodeString s, TColor *fg, TColor *bg);
void color_textout_v(TCanvas *cv, int xp, int yp, UnicodeString s, TColor *fg, TColor *bg);
void vert_textout(TCanvas *cv, int xp, int yp, UnicodeString s);

UnicodeString get_tag_atr(UnicodeString s, UnicodeString t, UnicodeString a);
bool get_html_inf(UnicodeString fnam, TStringList* lst);

UnicodeString get_hlp_title(UnicodeString fnam);

bool is_text_file(UnicodeString fnam);
UnicodeString get_top_textline(UnicodeString fnam);

TStringDynArray get_csv_array(UnicodeString src, int size);
TStringDynArray get_prm_array(UnicodeString src);

void get_font_from_str(TFont *f, UnicodeString s);

bool equal_font(TFont *f0, TFont *f1);

bool test_FileExt(UnicodeString fext, UnicodeString list);

void get_files(UnicodeString pnam, UnicodeString mask, TStrings *lst, bool subsw = false);

bool test_ideafrag2(UnicodeString fnam);
UnicodeString rel_to_absdir(UnicodeString fnam, UnicodeString rnam);
UnicodeString abs_to_reldir(UnicodeString fnam, UnicodeString rnam);

TDateTime str_to_date_time(UnicodeString str);

bool check_TUpDown(TUpDown *udp);

void str_to_ComboBox(TComboBox *cp, UnicodeString s, UnicodeString sep = "|");
void str_to_GridHeadder(TStringGrid *gp, UnicodeString s, UnicodeString sp = "|");

void make_key_list(TStrings *lst);
bool is_KeyDown(WORD key);

UnicodeString get_VersionStr(UnicodeString fnam, bool is_num = false);
bool is_SnapToDefBtn();

void msgbox_ERR(  UnicodeString msg, UnicodeString tit = "エラー");
void msgbox_WARN( UnicodeString msg, UnicodeString tit = "警告");
void msgbox_OK(   UnicodeString msg, UnicodeString tit = "確認");
bool msgbox_Y_N(  UnicodeString msg, UnicodeString tit = "確認");
int  msgbox_Y_N_C(UnicodeString msg, UnicodeString tit = "確認");

//---------------------------------------------------------------------------
typedef HWND (WINAPI *FUNC_HtmlHelp)(HWND, LPCWSTR, UINT, DWORD);
extern  FUNC_HtmlHelp lpfHtmlHelp;
extern DWORD  ProcessId;

HWND get_HelpWnd();
void HtmlHelpClose(bool only_fg = false);
void HtmlHelpContext(int idx, HWND hWnd = NULL);

//---------------------------------------------------------------------------
#endif
