//----------------------------------------------------------------------//
//	矢印線のクラス														//
//																		//
//----------------------------------------------------------------------//
#ifndef arwlineH
#define arwlineH

//---------------------------------------------------------------------------
#include "UserFunc.h"

//---------------------------------------------------------------------------
class ArrowLine {
private:
	bool FVisible;

public:
	int  tType;
	int  fromTag, toTag;

	int  lType;					//線タイプ
	int  lWidth;				//線の太さ	(-1:点線, 1,3,5:実線)
	TColor lColor;				//線の個別色 (clNoneの場合無視)
	TColor lCurCol;
	int  aType;					//矢のタイプ
	int	 aSize;					//矢のサイズ
	int  x0, y0, x1, y1;		//両端絶対位置
	int  vx0, vy0, vx1, vy1;	//画面上での両端相対位置
	UnicodeString TextStr;		//テキスト内容
	bool tOpposite;				//テキスト位置が反対側

	bool tmpFlag0, tmpFlag1;

	ArrowLine();
	void assign(ArrowLine* ap);
	bool pt_in_line(int X, int Y);
	bool pt_in_frm(int X, int Y);
	bool pt_in_to(int X, int Y);

	void draw(TCanvas *cv, double ratio, TColor lcol, TColor tcol, bool sel_sw = false, bool era_sw = false);
	void erase(TForm *frm);
	void view_line(TCanvas *v_cv, double ratio, TColor col);
	bool set_pos(octrect xrc0, octrect xrc1, int ofsx, int ofsy, bool j0sw = false, bool j1sw = false);
	void reverse();

	__property bool Visible = {read = FVisible}; //, write = FVisible};
};

//(tType);
#define TDIR_NONE		0	//無効
#define TDIR_G2G		1	//グループ --> グループ
#define TDIR_F2F		2	//断片     --> 断片
#define TDIR_F2G		3	//断片     --> グループ
#define TDIR_G2F		4	//グループ --> 断片

//線タイプ (lType)
#define LTYPE_LINE		1	//線		----
#define LTYPE_S_ARROW	2	//矢印		--->
#define LTYPE_D_ARROW	3	//両矢印	<-->
#define LTYPE_CD_ARROW	4	//逆両矢印	>--<
#define LTYPE_D_LINE	5	//二重線	====
#define LTYPE_C_ARROW	6	//逆矢印	---<

//矢の形 (aType);
#define ATYPE_LINE		0	//線
#define ATYPE_SOLID		1	//塗りつぶし

//矢の大きさ(aSize)
#define ASIZE_SMALL		 8	//小
#define ASIZE_MEDIUM	12	//中
#define ASIZE_LARGE		16	//大

#define DLINE_WIDTH		 3	//二重線の間隔


//---------------------------------------------------------------------------
//矢印線リスト (TList から継承)
//---------------------------------------------------------------------------
class TArrowList : public TList {
private:
	TForm* frg_owner;

	ArrowLine * __fastcall Get(int Index);
	void __fastcall Put(int Index, ArrowLine *Item);

public:
	__fastcall TArrowList(Classes::TComponent* AOwner);
	ArrowLine *	__fastcall lp_from_pos(int X, int Y);
	ArrowLine *	__fastcall lp_from_id(int tp, int id0, int id1);
	void __fastcall erase_line(int tp = TDIR_NONE);
	void __fastcall del_all();
	void __fastcall new_copy(TArrowList *lst);
	void __fastcall clr_tmp_flag();
	void __fastcall change_frg_id(int id0, int id1);

	__property ArrowLine * Items[int Index] = {read=Get, write=Put};
};

//---------------------------------------------------------------------------
#endif
