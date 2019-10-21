//----------------------------------------------------------------------//
//	環境設定管理のクラス												//
//																		//
//----------------------------------------------------------------------//
#ifndef EnvDataH
#define EnvDataH

//---------------------------------------------------------------------------
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "UIniFile.h"
#include "spiunit.h"

//---------------------------------------------------------------------------
#define SUPPORT_URL		_T("http://nekomimi.la.coocan.jp/")

//---------------------------------------------------------------------------
#define MAX_FRGWIN 40			//最大子ウィンドウ数

#define MAX_FILEHISTORY  20		//最大ファイル履歴数
#define MAX_FINDHISTORY  40		//最大検索履歴数
#define MAX_BGCOL_ITM    20		//背景色選択の最大項目数
#define MAX_JUMPLIST     20		//ジャンプ履歴メニューの最大項目数

#define DEF_TRIM_WD		800
#define DEF_TRIM_HI		600
#define DEF_TRIM_MGN	 50

#define FEXT_WICSTD		".bmp.jpg.jpeg.jpe.png.gif.tif.wdp.hdp"		//WICが標準で表示可能

//個別設定色の取得
#define GET_indcol(id)  ((id==Graphics::clNone)? EV->id : id)

int  ScaledInt(int n);
void ResizeImageList(TImageList *i_list);
void InitUpDown(TUpDown *udp, int n);

//---------------------------------------------------------------------------
class EnvData {
public:
	TForm *env_owner;
	bool IsAdmin;				//管理者権限を持っているか

	//------------------------------
	// オプション
	//------------------------------
	int def_wd, def_hi;			//断片デフォルトサイズ
	int nam_wd, nam_hi;			//表札デフォルトサイズ
	int img_r1, img_r2;			//画像デフォルト倍率
	int frg_mgn;				//断片の余白
	int frg_itl;				//断片の行間
	int plt_mgn;				//枠上表札の余白

	int WicScaleOpt;			//画像の縮小アルゴリズム

	TColor col_defFG, col_defBG;	//デフォルト
	TColor col_Border;
	TColor col_selFG, col_selBG;	//選択
	TColor col_namFG, col_namBG;	//表札
	TColor col_symFG;				//標識
	TColor col_LinLbl;				//ラベル
	TColor col_Title;				//タイトル
	TColor col_backgr, col_selRct;
	TColor col_glstFG, col_glstBG, col_glstOff;
	TColor col_xlstFG, col_xlstBG;
	TColor col_AreaFrm, col_MapBG;
	TColor col_GrpFrm;
	TColor col_GrpLine, col_FrgLine;
	TColor col_DivLine;
	TColor col_Brush;
	TColor col_PosGrid;
	TColor col_OptFind;			//設定項目検索の強調表示

	int    col_Custom[16];

	TFont *frgFont; 			//断片のフォント
	TFont *namFont; 			//表札のフォント
	TFont *symFont; 			//標識のフォント
	TFont *lblFont; 			//関係線ラベルのフォント
	TFont *titFont;				//タイトルのフォント
	TFont *glstFont;			//グループリストのフォント
	TFont *flstFont;			//断片リストのフォント
	TFont *xlstFont;			//拡張検索リストのフォント
	TFont *stbarFont;			//ステータスバーのフォント
	TFont *dialogFont;			//ダイアログのフォント

	int  GLstInterLine;			//グループリストの行間(％)
	int  FLstInterLine;			//断片リストの行間(％)
	int  XLstInterLine;			//拡張検索リストの行間(％)
	int  gfrmLineWidth;			//グループ枠の線幅
	int  gfrmMaxRnd;

	int  new_frg_pos;			//新規作成位置 (1～9)
	bool new_no_group;			//新規作成時に無所属

	TCursor crFragMove; 		//断片移動時のマウスカーソル

	bool show_GroupFrame;		//グループ枠を表示
	bool show_GroupLine;		//グループ関係線を表示
	bool show_FragLine;			//断片関係線を表示
	bool show_NamePlate;		//表札を表示
	bool show_LinkMark; 		//リンクマークを表示
	bool show_DivLine;			//画面分割線を表示
	bool show_PosGrid;			//配置グリッドを表示

	TAlign glstAlign;			//グループリストの表示位置

	int  CurGLineType;			//関係線の現在のデフォルト
	int  CurFLineType;
	int  CurGLineWidth;
	int  CurFLineWidth;
	int  CurGArrowType;
	int  CurFArrowType;
	int  CurGArrowSize;
	int  CurFArrowSize;
	bool CurTxOpposite;

	bool GlRightAngle;			//辺同士の関係線をなるべく垂直に

	bool auto_exp;				//デフォルトを越えたら自動的に行数増
	bool input_wordwrap;		//テキスト入力の右端で折り返す
	bool ime_on;				//断片テキスト入力時IMEオン
	bool accept_cr; 			//断片テキスト入力時改行を受け入れる
	bool cr_returnkey;			//改行はリターンキーで
	bool ok_ctrl_return;		//Ctrl + リターンキーで OK
	UnicodeString CR_str;		//改行の変換文字列
	bool LinkIsIcon;			//リンクを関連付けアイコンで表示
	bool LinkRelPath;			//リンクを相対パスで保存

	UnicodeString HomeWindow;	//ホームウィンドウ
	bool HomWinMax; 			//ホームウィンドウは最大化
	UnicodeString LastFile;		//前回最後にアクティブだったファイル
	bool OpenLastFile;			//前回最後にアクティブだったファイルを起動時に開く
	bool ChildMax;				//一個目のウィンドウは最大化
	bool HideTitleMenu; 		//最大化時にタイトルバーとメニューを消す
	bool RstScrPos; 			//スクロールバーを前回の位置に復元
	bool ActivDistWin;			//断片の移動先ウィンドウをアクティブに
	int	 DefScrWd, DefScrHi;	//デフォルトの画面サイズ

	bool SpreadShuffle; 		//展開時にシャッフルする
	bool SpreadAlign;			//展開時に揃えて並べる
	int  StretchMode;			//伸縮モード(基点)  0:中心 1:左上
	bool StretchGrp0;			//無所属断片も連動してグループ伸縮
	int  away_frg_pos;			//片づけ位置(1～4)
	int  ArrangePt;				//片づけ・集合のずらし幅
	bool AlwaysShowNp;			//表札を常に表示
	bool NoNpHideGfrm;			//表札がない場合はグループ枠を表示しない
	bool OnlyNpHideGfrm;		//表札のみの場合グループ枠を表示しない
	bool PlateOnGfrm;			//表札をグループ枠上に表示
	int	 PlateAlign;			//グループ枠上の表札の位置
	int  PlateShape;			//グループ枠上の表札の外形	0:長方形 1:角丸形 2:端丸形
	bool MapViewSelRect;		//全体図に選択枠を表示
	bool FitMapView;			//全体図のウィンドウサイズをフィット
	int  MapAlpha;				//全体図の透明度
	bool CopyCLine;				//切り取り・コピーで関係線も含める
	bool ClipPosSort;			//クリップボードコピー時に位置でソート
	bool DropImgFile;			//画像ファイルドロップ時に画像断片作成
	bool NewDropPos;			//ドロップ位置に新規作成
	bool ImageIsBack;			//画像は背後に表示
	bool ShowDstGrp;			//断片移動先のグループが非表示だったら表示に
	bool FrameAllSel;			//選択枠と交差した断片は全て選択する
	bool ChgGrpModeCtrl;		//グループ化時のCtrlキーの役割を入れ替える
	bool RDragScroll;			//右ドラッグで画面をスクロール
	bool SelJumpFrg;			//ジャンプ先の断片を選択状態にする
	bool PrintMono;				//白黒(2値)印刷
	int  FixPrnLWidth;			//印刷時の最小線幅　0:可変(最小1) 1..:固定
	bool PrintTime;				//印刷日時を印字
	bool PrintBackGr;			//背景色を印刷
	bool PrintScrSize;			//画面サイズに基づいて印刷

	bool HideBlkGrpItem;		//空グループはグループリストに表示しない

	int  GridSize;				//配置グリッドのサイズ
	bool SnapGrid;				//グリッドに引き込み

	bool TrimMovTop;			//トリミング時に左上端に移動
	bool TrimScrSize;			//トリミング時にリサイズ
	bool TrimFixSize;			//トリミング時に変更サイズを固定
	bool TrimSaveAs;			//トリミング後に名前をつけて保存
	int  TrimMargin;			//トリミング時の余白
	int  TrimFixWd;				//固定サイズ
	int  TrimFixHi;

	bool NewBlkDblClick;		//グループリストで、空グループをダブルクリックで新規断片

	UnicodeString CsvFormat;	//エクスポート項目

	bool ExportUTF8;			//UTF8でエクスポート

	UnicodeString FindWord;		//検索語
	int  FindMode;				//検索モード
	bool FindAndSw;				//検索: AND
	bool FindNotSw;				//検索: NOT
	bool FindRegSw;				//検索: 正規表現
	bool FindCaseSw;			//検索: 大小文字区別
	bool FindFupSw;				//検索: 断片リストで先頭に集める

	int  ImgFilterIdx;			//フィルタのインデックス(1,2)
	int  JpegQuality;			//Jpeg画像保存時の品質

	int  WheelScrQty;			//ホイールのスクロール量
	bool WheelShiftHorz;		//Shit + ホイールで横スクロール

	bool IndFixRatio;			//個別設定で縦横比を固定

	//------------------------------
	int TimerLockCnt;			//タイマー処理抑止用カウンタ
								// >0 の場合タイマー処理を行わない

	bool reqUpdate;				//各種表示更新要求
	bool reqFrgLst;				//断片リストの更新要求

	TStringList *AssociateList;		//関連づけリスト

	TStringList *JumpHistoryList;	//ジャンプ履歴
	int HistoryPtr;

	TStringList *FindHistoryList;	//検索履歴
	TStringList *FileHistoryList;	//ファイル履歴

	bool printer_ok;

	Graphics::TBitmap *LinkMark;	//リンクマーク
	TStringList *LinkIconList;		//リンクアイコンリスト
	TStringList *CachedIcoList;		//ファイル依存のアイコンキャッシュリスト

	UnicodeString SpiDir;			//Susieプラグインのディレクトリ
	bool UseSpiFirst;				//Susieプラグインを優先的に使用

	TControl *LastDockSite;		//グループリストが最後にドッキングしたサイト

	int FLstSortDir;			//断片リストのソート方向 1:昇順 -1:降順

	//------------------------------
	EnvData(Classes::TComponent* AOwner);
	~EnvData();

	UnicodeString __fastcall read_ini_str(UnicodeString sct, UnicodeString key);
	void __fastcall load_pos_info(UnicodeString sct, TForm *frm,
		int x, int y, int w, int h);
	void __fastcall save_pos_info(UnicodeString sct, TForm *frm);
	void __fastcall load_tool_info(UnicodeString sct, TToolBar *tb);
	void __fastcall save_tool_info(UnicodeString sct, TToolBar *tb);
	void __fastcall add_file_history(UnicodeString fnam);
	void __fastcall set_custom_col(TStrings *cust_col);
	void __fastcall get_custom_col(TStrings *cust_col);
	UnicodeString __fastcall get_associate(UnicodeString fnam);
	bool __fastcall save_ini_file();
};

//---------------------------------------------------------------------------
extern EnvData  *EV;
extern SpiUnit  *SPI;

extern TCursor crHandGrabL, crHandGrabR, crHandPickL, crHandPickR;
extern TCursor crLineTool, crGroupTool, crBrushTool, crSpuitTool;

extern double ScrScale;

//---------------------------------------------------------------------------
#endif
