//----------------------------------------------------------------------//
// グループ情報のクラス													//
//																		//
//----------------------------------------------------------------------//
#ifndef GrpInfsH
#define GrpInfsH
//---------------------------------------------------------------------------
#include "fragment.h"
#include "fraglist.h"
#include "grpframe.h"

//---------------------------------------------------------------------------
#define MAX_GROUP	 100		//最大グループ数

struct grp_inf {
	TGrpFrame *gfp;		//グループ枠
	TRect RC;			//グループ枠の絶対位置
	TRect cRC;			//グループ枠内の断片範囲
	int  Area;			//グループ枠の面積
	TColor col_GrpFrm;	//グループ枠の個別色
	int  LnWidth;		//グループ枠の個別線幅
	bool Visible;
	bool Selected;		//グループ内のどれかが選択されている
	bool AllSelected;	//グループ全体が選択されている
	bool RepaintFrm;	//グループ枠表示更新要求
	bool RepaintLn;		//関係線表示更新要求
	bool Empty;
	int  IncludeGrp;
	int  TotalSize;		//全断片数
	Fragment *Plate;	//表札
};

//---------------------------------------------------------------------------
// グループ情報
//---------------------------------------------------------------------------
class GrpInfs {
private:
	TCanvas *gcv;

public:
	grp_inf Items[MAX_GROUP + 1];	//グループ情報 0:無所属/ 1～:グループ

	int  cur_group; 			//カレントグループ
	int  cur_gfrm;

	bool exist_img;				//画像有り

	TColor col_GrpFrm;
	TFont *namFont;

	TFragList *FrgList;

	GrpInfs(TForm* AOwner, TFragList *lst);
	~GrpInfs();

	void __fastcall update_empty();
	int  __fastcall get_empty_idx();
	int  __fastcall get_sel_max();
	Fragment * __fastcall get_name_plate(int gn, UnicodeString *name);
	void __fastcall calc_frame(int gn, bool pickuped = false, TRect *crc = NULL);
	void __fastcall update_frame(bool pickuped);
	void __fastcall repaint_frame(bool sw = false);
	int  __fastcall group_from_pos(int X, int Y);
	int  __fastcall gstk_from_pos(int X, int Y, grp_stk *sp);
	int  __fastcall gfrm_from_pos(int X, int Y);
	int  __fastcall compare_group(int gn0, int gn1);
	void __fastcall stretch(double ratio, bool sw);
};
//---------------------------------------------------------------------------
#endif
