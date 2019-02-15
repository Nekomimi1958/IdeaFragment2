//----------------------------------------------------------------------//
//	断片セットのクラス													//
//																		//
//----------------------------------------------------------------------//
#ifndef FragSetH
#define FragSetH

//---------------------------------------------------------------------------
#include "arwline.h"
#include "fragment.h"
#include "fraglist.h"
#include "grpframe.h"
#include "grpinfs.h"
#include "fragbuf.h"

//---------------------------------------------------------------------------
#define MAX_GRP_ITM	  40		//グループ関係の最大メニュー項目数
#define MAX_TEXTLEN	2000		//最大テキスト長
#define MAX_UNDO	 100		//最大アンドゥ回数

#define DSMODE_NON	0
#define DSMODE_GEN	1
#define DSMODE_FRG	2
#define DSMODE_CON	3
#define DSMODE_GRP	4
#define DSMODE_OPT	5

//画面分割モード
#define DVMODE_NON	0	//無し
#define DVMODE_DVN	1	//縦横分割(分割数指定)
#define DVMODE_PXN	2	//縦横分割(ピクセル指定)
#define DVMODE_TRH	3	//三角形タイル(水平)
#define DVMODE_TRV	4	//三角形タイル(垂直)
#define DVMODE_HXH  5	//六角形タイル(水平)
#define DVMODE_HXV  6	//六角形タイル(垂直)

//関係線モード (CLineMode)
#define CLMODE_NON		0	//無効
#define CLMODE_READY	1	//待機
#define CLMODE_CONCT	2	//設定中
#define CLMODE_RECON	3	//接続変更中


//---------------------------------------------------------------------------
//断片セットのクラス
//---------------------------------------------------------------------------
class FragSet {
private:
	int  MaxZ;						//(最大)最前面Z
	int  MaxID;						//最大ID

	Fragment *jump_fp;				//ジャンプ中断片
	Fragment *find_fp;				//検索等で注目中の断片
	Fragment *catched_fp;			//移動時につかんでいる断片

	bool movskip;					//DblClick後の移動を回避

	FragBuffer *UndoBuf[MAX_UNDO];

	void del_frag(Fragment *fp);
	void reset_notice();
	int  import_core(TStringList *TextBuff, int gn);
	bool erase_sym_frm();
	void visible_update();
	void draw_prn_img(TCanvas *cv, int wd, int hi, int foot_hi);

public:
	TForm* frg_owner;				//断片セットのオーナー
	UnicodeString file_name;		//断片ファイル名
	UnicodeString data_title;		//タイトル

	TFragList *FrgList; 			//断片のリスト
	TFragList *SelList; 			//選択中断片のリスト

	GrpInfs   *GInfs;				//グループ情報

	TArrowList *CLineList;			//関係線リスト
	int  CLineMode;					//関係線モード
	ArrowLine *CurLp;				//選択中関係線

	int  scr_wd, scr_hi;			//画面の全体サイズ

	int  div_x, div_y;				//画面分割数
	int  div_mod;					//画面分割モード
	int  div_lw;					//画面分割線幅

	int  title_pos;					//タイトルの表示位置 (1～9, 0:非表示)

	bool inh_Grid;					//グリッド描画の抑止

	TControlScrollBar *HscrBar;		//水平スクロールバー
	TControlScrollBar *VscrBar;		//垂直スクロールバー

	TColor col_backgr;				//背景色(個別)
	TColor col_DivLine;				//分割線の色(個別)
	TColor col_GrpFrm;				//グループ枠の色(個別)
	TColor col_GrpLine;				//関係線の色(個別)
	TColor col_FrgLine;
	TColor col_LinLbl;				//関係線ラベルの色(個別)
	TColor col_Title;				//タイトル色(個別)

	TFont *frgFont; 				//断片のフォント(個別)
	TFont *namFont; 				//表札のフォント(個別)
	TFont *symFont; 				//標識のフォント(個別)
	TFont *lblFont; 				//ラベルのフォント(個別)
	TFont *titFont;					//タイトルのフォント(個別)

	bool ParentFrgFont;
	bool ParentNamFont;
	bool ParentSymFont;
	bool ParentLblFont;
	bool ParentTitFont;

	int  LastHPos, LastVPos;		//前回の位置
	UnicodeString LastAnc;			//前回開いたときの位置

	bool read_only; 				//編集禁止
	bool forcesave; 				//確認なしで強制保存
	bool modify;					//変更された
	bool moving;					//移動中
	bool moved; 					//移動あり
	bool movgroup;					//+Ctrlキーでグループ選択後に移動中
	bool pickuped;					//移動中にグループから引き上げ
	bool floating;					//浮遊中
	int  notice_pos; 				//選択断片内の注目位置(0～選択数-1)

	TPoint lastP;					//断片移動中の前回のマウス位置

	int UndoN, RedoN, UndoPtr;

	FragSet(TForm* AOwner);
	~FragSet();

	Fragment *new_frag(bool id_sw = true);
	void update_grp_empty();
	void del_sel_frag();
	Fragment *get_sel_single();
	int  mak_sel_list(bool show = false);
	Fragment *get_sel_center();
	TRect  get_sel_union(bool tmp_sw = false);
	TPoint get_sel_org_pos(int mode, bool tmp_sw = false);
	int  get_sel_group();
	int  get_sel_group_ex();
	Fragment *get_linked_frag(UnicodeString fnam);
	int  get_intr_flist(Fragment *fp, TFragList *lst);
	int  get_con_flist(Fragment *fp, TFragList *lst, bool sel_sw = false);
	void sel_connected(Fragment *fp);
	Fragment *frag_from_pos(int X, int Y);

	ArrowLine *set_group_line(int gn0, int gn1);
	ArrowLine *set_frag_line(int id0, int id1);
	ArrowLine *set_fg_line(int tp, int id0, int id1);
	int  has_lines(Fragment *fp);
	Fragment *put_junction(int x, int y);
	bool add_junction();
	void clr_iso_junction();

	void exchange_group(int gn0, int gn1);
	void mov_sel_group(int gn, bool add_sw = false);
	void rel_group(int gn);

	void bring_to_front(Fragment *fp)
	{
		if (fp) fp->Z = ++MaxZ;
	}

	void update_group_frame();
	void update_frg_line();
	void update_fg_line();
	void to_center(Fragment *fp, bool sw, int x = -1, int y = -1);
	void to_anchor(UnicodeString anam);
	void set_new_pos(Fragment *fp, int idx, int x = -1, int y = -1);

	void push_all();
	void pop_all(int idx);
	void undo();
	void redo();

	void set_titlebar();
	int  load_file(UnicodeString fnam, bool mg_sw);
	bool save_file(UnicodeString fnam, bool ts_sw = false);
	int  import_text(UnicodeString fnam);
	int  export_text(UnicodeString fnam);
	int  export_file(UnicodeString fnam, UnicodeString separator);
	int  new_from_clipboard();

	void get_sel_str(UnicodeString* cpybuf);
	void copy_buf(bool cutsw=false);
	void paste_buf(int x = -1, int y = -1);

	void all_select(bool sw = true);
	void rect_select(TRect rc);
	void group_select(int gn, bool sw);
	void bgcol_select(TColor col);
	void trim_select();
	void one_select_center(Fragment *fp);
	void gather_sel(bool sw, int pt = 1);
	void spread_sel();
	void move_sel(int x, int y);
	void stretch_sel(double ratio, int smod);
	void stretch_grp(double ratio, bool sw);
	void rubber_sel(int len, int spc);
	void quick_mov_sel(int mode, double angle = 0.0);
	void draw_divln(TCanvas *cv, int w, int h, double ratio = 1.0);
	void draw_grid();
	void draw_title(TCanvas *cv, double ratio, int w, int h);
	void all_update(bool upd_img = false);
	int  find_str(UnicodeString wd, bool sel_sw);

	void jump_link(Fragment *fp);
	void jump_sel_link();

	void printout();
	bool imageout(UnicodeString fnam);

	//マウスイベント
	void __fastcall FragMouseDown(Fragment *sp, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void __fastcall FragMouseMove(Fragment *sp, TShiftState Shift, int X, int Y);
	void __fastcall FragMouseUp(Fragment *sp, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void __fastcall FragDblClick(Fragment *sp);
};

//---------------------------------------------------------------------------
#endif
