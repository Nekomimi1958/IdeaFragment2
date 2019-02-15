//----------------------------------------------------------------------//
//	断片のクラス														//
//																		//
//----------------------------------------------------------------------//
#ifndef FragmentH
#define FragmentH
//---------------------------------------------------------------------------
#define MAX_FRG_WD	 40		//断片の最大文字幅
#define MAX_FRG_HI	 50		//断片の行数

#define MAX_IN_GRP	 6		//所属できる最大グループ数

//---------------------------------------------------------------------------
enum FragStyle {
	fgsNone, fgsNormal, fgsPlate, fgsSymbol, fgsImage, fgsJunction
};

struct grp_stk {
	int stk[MAX_IN_GRP];	//所属グループ番号
	int levle;				//(未使用)
};

//---------------------------------------------------------------------------
int get_FragFitHi(TCanvas * cv, UnicodeString s, int wd);

//---------------------------------------------------------------------------
class Fragment {
private:
	Graphics::TBitmap *ImgBuff;	//表示イメージ用バッファ
	TMetafile *MetaBuff;		//メタファイル用バッファ

	TCanvas *cv;				//断片を描画するCanvas
	bool Initialized;			//初期化
	bool Erased;

	bool FSelected;
	void __fastcall SetSelected(bool Value)
	{
		if (FSelected != Value) {
			FSelected = Value;
			Changed   = true;
		}
	}

	void out_frag_text(TCanvas *t_cv, TRect tRC, int mgn, int itl);
	bool draw_icon(UnicodeString fnam, TCanvas *cv, int x, int y, bool act = false);

public:
	TForm   *Parent;		//親ウィンドウ
	int     wd, hi;			//桁幅・行数
	TRect   RC;				//断片の四隅の位置
	TRect   vRC;			//画面上での四隅の表示位置
	TRect	tmpRC;			//位置待避(伸縮等で使用)
	int     Z;				//Zオーダ
	int		img_wd, img_hi;
	bool	Changed;		//表示イメージ要変更
	bool	OnGfrm;			//グループ枠上にアタッチ(表札のみ)
	double	sX, sY;			//浮遊計算用サム

	int  	ID;				//ID(永久固定)	!NEW!
	TDateTime CreTime;		//作成日時		!NEW!
	TDateTime ModTime;		//最終アクセス	!NEW!

	TFont *frgFont; 		//断片のフォント(個別)
	TFont *namFont; 		//表札のフォント(個別)
	TFont *symFont; 		//標識のフォント(個別)

	TColor colFG, colBG;	//文字・背景色
	UnicodeString TextStr;	//テキスト内容
	UnicodeString LinkName;	//リンク先(絶対パス)

	grp_stk group;			//所属グループ
	FragStyle style;		//スタイル
	bool Visible;			//表示中
	bool Jumping;			//ジャンプ中
	bool Noticed;			//注目中
	bool Matched;			//検索でマッチ

	__property bool Selected = {read = FSelected, write = SetSelected};	//選択中

	Fragment(TForm *pf);
	~Fragment();

	void upd_link();
	void era_frag();
	void show_frag();
	void print_frag(TCanvas *p_cv, double ratio);
	void view_frag(TCanvas *v_cv, double ratio);
	void assign(Fragment *fp,  bool id_sw = false, bool sel_sw = false);
	int  in_group(int gn);
	int  get_top_group();
	bool set_group(int gn, bool add_sw = false, bool top_sw = false);
	int  rel_group(int gn);
};
//---------------------------------------------------------------------------
#endif
