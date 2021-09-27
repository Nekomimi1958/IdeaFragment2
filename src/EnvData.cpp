//----------------------------------------------------------------------//
//	環境設定管理のクラス												//
//																		//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "MapView.h"
#include "EnvData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

EnvData *EV = NULL;		//環境設定管理データ
SpiUnit *SPI;			//Susie プラグイン

TCursor crHandGrabL, crHandGrabR, crHandPickL, crHandPickR;
TCursor crLineTool, crGroupTool, crBrushTool, crSpuitTool;

double ScrScale = 1.0;			//画面スケーリング

//---------------------------------------------------------------------------
//スケーリングされた整数値を取得
//---------------------------------------------------------------------------
int ScaledInt(int n)
{
	return (int)(n * ScrScale);
}

//---------------------------------------------------------------------------
//ImageList のスケーリング
//---------------------------------------------------------------------------
void ResizeImageList(TImageList *i_list)
{
	if (ScrScale<=1.0) return;

	int i_wd = i_list->Width;
	int i_hi = i_list->Height;
	int r_wd = ScaledInt(i_wd);
	int r_hi = ScaledInt(i_hi);
	TRect rc = Rect(0, 0, r_wd, r_hi);

	std::unique_ptr<TImageList> r_list(new TImageList(r_wd, r_hi));

	std::unique_ptr<Graphics::TBitmap> i_bmp(new Graphics::TBitmap());
	i_bmp->SetSize(i_wd, i_hi);

	std::unique_ptr<Graphics::TBitmap> r_bmp(new Graphics::TBitmap());
	r_bmp->SetSize(r_wd, r_hi);

	for (int i=0; i<i_list->Count; i++) {
		TCanvas *cv = i_bmp->Canvas;
		cv->Brush->Color = clMaroon;
		cv->FillRect(Rect(0, 0, i_wd, i_hi));

		TCanvas *cv2 = r_bmp->Canvas;
		cv2->Brush->Color = clMaroon;
		cv2->FillRect(rc);

		if (i_list->GetBitmap(i, i_bmp.get())) cv2->StretchDraw(rc, i_bmp.get());

		r_list->AddMasked(r_bmp.get(), clMaroon);
	}

	i_list->Assign(r_list.get());
}

//---------------------------------------------------------------------------
//TUpDown の初期化
//---------------------------------------------------------------------------
void InitUpDown(TUpDown *udp, int n)
{
	udp->Position = n;
	TCustomEdit *ep = dynamic_cast<TCustomEdit *>(udp->Associate);
	if (ep) ep->Text = udp->Position;
}

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
EnvData::EnvData(Classes::TComponent* AOwner)
{
	env_owner = (TForm*)AOwner;

	//管理者権限を持っているか?
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID admin_group;
	BOOL res = ::AllocateAndInitializeSid(
					&NtAuthority, 2,
					SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
					&admin_group); 
	if (res) {
		if (!CheckTokenMembership(NULL, admin_group, &res)) res = FALSE;
		::FreeSid(admin_group); 
	}
	IsAdmin = res;

	ScrScale = Screen->PixelsPerInch / 96.0;	//スケーリングを設定

	AssociateList	= new TStringList();
	FileHistoryList	= new TStringList();
	FindHistoryList	= new TStringList();
	JumpHistoryList	= new TStringList();  HistoryPtr=0;

	//マウスカーソル設定
	crHandGrabL = (TCursor)5;	//左手(移動)
	Screen->Cursors[crHandGrabL] = LoadCursor(HInstance, _T("HNDGRB_L"));
	crHandGrabR = (TCursor)6;	//右手
	Screen->Cursors[crHandGrabR] = LoadCursor(HInstance, _T("HNDGRB_R"));
	crHandPickL = (TCursor)7;	//左手(グループ越え移動)
	Screen->Cursors[crHandPickL] = LoadCursor(HInstance, _T("HNDPIC_L"));
	crHandPickR = (TCursor)8;	//右手
	Screen->Cursors[crHandPickR] = LoadCursor(HInstance, _T("HNDPIC_R"));
	crGroupTool = (TCursor)9;	//グループ化
	Screen->Cursors[crGroupTool] = LoadCursor(HInstance, _T("GROUP_TOOL"));
	crLineTool  = (TCursor)10;	//線引き
	Screen->Cursors[crLineTool]  = LoadCursor(HInstance, _T("LINE_TOOL"));
	crBrushTool = (TCursor)11;	//塗り
	Screen->Cursors[crBrushTool] = LoadCursor(HInstance, _T("PAINT_TOOL"));
	crSpuitTool = (TCursor)12;	//スポイト
	Screen->Cursors[crSpuitTool] = LoadCursor(HInstance, _T("SPUIT_TOOL"));

	//リンクマーク
	LinkMark = new Graphics::TBitmap();
	LinkMark->LoadFromResourceName((int)HInstance, "LINKMARK");

	//リンクアイコン
	LinkIconList  = new TStringList();
	CachedIcoList = new TStringList();

	//フォント情報読み込み
	std::unique_ptr<TFont> defFont(new TFont());
	defFont->Assign(env_owner->Font);
	defFont->Name  = "ＭＳ ゴシック";
	glstFont	   = IniFile->ReadFontInf("GroupList",	defFont.get());
	flstFont	   = IniFile->ReadFontInf("FragList",	defFont.get());
	xlstFont	   = IniFile->ReadFontInf("FindEx",		defFont.get());
	defFont->Name  = "ＭＳ Ｐゴシック";
	stbarFont	   = IniFile->ReadFontInf("StatusBar",	defFont.get());
	frgFont 	   = IniFile->ReadFontInf("Fragment",	defFont.get());
	lblFont 	   = IniFile->ReadFontInf("LineLabel",	defFont.get());
	defFont->Style = defFont->Style << fsBold;
	defFont->Size  = abs(env_owner->Font->Size) + 2;
	namFont 	   = IniFile->ReadFontInf("NamePlate",	defFont.get());
	defFont->Size  = 20;
	titFont 	   = IniFile->ReadFontInf("DataTitle",	defFont.get());
	defFont->Style = defFont->Style >> fsBold;
	symFont 	   = IniFile->ReadFontInf("Symbol",		defFont.get());
	dialogFont	   = IniFile->ReadFontInf("Dialog",		env_owner->Font);
	if (dialogFont->Size>10) dialogFont->Size = 10;
	Application->DefaultFont->Assign(dialogFont);

	//各種設定読み込み
	UnicodeString sct = "General";

	//※廃止セクション、キーの削除
	IniFile->DeleteKey(sct, "FlatToolBar");		//V2.34

	HomeWindow	   = IniFile->ReadString( sct, "HomeWindow");
	HomWinMax	   = IniFile->ReadBool(   sct, "HomWinMaximize");
	LastFile	   = IniFile->ReadString( sct, "LastFile");
	OpenLastFile   = IniFile->ReadBool(   sct, "OpenLastFile");
	ChildMax	   = IniFile->ReadBool(   sct, "ChildWinMaximize");
	HideTitleMenu  = IniFile->ReadBool(   sct, "HideTitleMenu");
	RstScrPos	   = IniFile->ReadBool(   sct, "RestoreScrPos",		true);
	ActivDistWin   = IniFile->ReadBool(   sct, "ActivateDistWin");
	MapViewSelRect = IniFile->ReadBool(   sct, "MapViewSelRect");
	FitMapView	   = IniFile->ReadBool(   sct, "FitMapView");
	MapAlpha	   = IniFile->ReadInteger(sct, "MapAlpha",			255);
	CopyCLine	   = IniFile->ReadBool(   sct, "CopyCLine");
	ClipPosSort    = IniFile->ReadBool(   sct, "ClipPosSort",		true);
	DropImgFile    = IniFile->ReadBool(   sct, "DropImgFile",		true);
	NewDropPos	   = IniFile->ReadBool(   sct, "NewDropPos",		true);
	ShowDstGrp	   = IniFile->ReadBool(   sct, "ShowDstGrp");
	FrameAllSel    = IniFile->ReadBool(   sct, "FrameAllSelect");
	ChgGrpModeCtrl = IniFile->ReadBool(   sct, "ChgGrpModeCtrl");
	RDragScroll    = IniFile->ReadBool(   sct, "RDragScroll");
	SelJumpFrg	   = IniFile->ReadBool(   sct, "SelJumpFrg");
	ImageIsBack    = IniFile->ReadBool(   sct, "ImageIsBack");
	AlwaysShowNp   = IniFile->ReadBool(   sct, "AlwaysShowNp");
	NoNpHideGfrm   = IniFile->ReadBool(   sct, "NoNpHideGfrm");
	OnlyNpHideGfrm = IniFile->ReadBool(   sct, "OnlyNpHideGfrm");
	PlateOnGfrm    = IniFile->ReadBool(   sct, "PlateOnGFrame");
	PlateAlign	   = IniFile->ReadInteger(sct, "PlateAlign",		0);
	PlateShape	   = IniFile->ReadInteger(sct, "PlateShape",		0);

	glstAlign	   = (TAlign)(IniFile->ReadInteger("GroupList","Align",alLeft));
	gfrmLineWidth  = IniFile->ReadInteger(sct, "GFrameLineWidth",	1);
	gfrmMaxRnd	   = IniFile->ReadInteger(sct, "GFrameMaxRnd",		60);

	DefScrWd	   = IniFile->ReadInteger(sct, "DefScrWidth",		2000);
	DefScrHi	   = IniFile->ReadInteger(sct, "DefScrHeight",		1500);

	CurGLineType   = IniFile->ReadInteger(sct, "CurGLineType",		LTYPE_LINE);
	CurGLineWidth  = IniFile->ReadInteger(sct, "CurGLineWidth",		1);
	CurGArrowType  = IniFile->ReadInteger(sct, "CurGArrowType",		ATYPE_SOLID);
	CurGArrowSize  = IniFile->ReadInteger(sct, "CurGArrowSize",		ASIZE_MEDIUM);
	CurFLineType   = IniFile->ReadInteger(sct, "CurFLineType",		LTYPE_LINE);
	CurFLineWidth  = IniFile->ReadInteger(sct, "CurFLineWidth",		1);
	CurFArrowType  = IniFile->ReadInteger(sct, "CurFArrowType",		ATYPE_SOLID);
	CurFArrowSize  = IniFile->ReadInteger(sct, "CurFArrowSize",		ASIZE_MEDIUM);
	CurTxOpposite  = IniFile->ReadBool(   sct, "CurTxOpposite");

	GlRightAngle   = IniFile->ReadBool(   sct, "GLineRightAngle",	true);

	PrintMono	   = IniFile->ReadBool(   sct, "PrintMonochrome");
	PrintTime	   = IniFile->ReadBool(   sct, "PrintTime");
	FixPrnLWidth   = IniFile->ReadInteger(sct, "FixPrnLWidth",		1);
	PrintBackGr    = IniFile->ReadBool(   sct, "PrintBackGr");
	PrintScrSize   = IniFile->ReadBool(   sct, "PrintScrSize");

	FindMode	   = IniFile->ReadInteger(sct, "FindMode",			0);
	FindAndSw	   = FindNotSw = FindRegSw = false;
	FindCaseSw	   = FindFupSw = false;

	ImgFilterIdx   = IniFile->ReadInteger(sct, "ImgFilterIdx", 		1);
	JpegQuality    = IniFile->ReadInteger(sct, "JpegQuality", 		100);

	WheelScrQty	   = IniFile->ReadInteger(sct, "WheelScrQty",		16);
	WheelShiftHorz = IniFile->ReadBool(   sct, "WheelShiftHorz",	true);

	GridSize	   = std::max(IniFile->ReadInteger(sct, "GridSize", 16), 4);
	SnapGrid	   = IniFile->ReadBool(   sct, "SnapGrid");

	TrimMovTop	   = IniFile->ReadBool(   sct, "TrimMovTop",		true);
	TrimScrSize    = IniFile->ReadBool(   sct, "TrimScrSize",		true);
	TrimFixSize    = IniFile->ReadBool(   sct, "TrimFixSize");
	TrimSaveAs	   = IniFile->ReadBool(   sct, "TrimSaveAs");
	TrimMargin	   = IniFile->ReadInteger(sct, "TrimMargin",		DEF_TRIM_MGN);
	TrimFixWd	   = IniFile->ReadInteger(sct, "TrimFixWd",			DEF_TRIM_WD);
	TrimFixHi	   = IniFile->ReadInteger(sct, "TrimFixHi",			DEF_TRIM_HI);

	NewBlkDblClick = IniFile->ReadBool(   sct, "NewBlankDblClick");
	SpiDir		   = IniFile->ReadString( sct, "SpiDir");
	UseSpiFirst    = IniFile->ReadBool(   sct, "UseSpiFirst");

	StretchMode    = IniFile->ReadInteger(sct, "StretchMode",		0);
	StretchGrp0    = IniFile->ReadBool(   sct, "StretchGrp0");

	CsvFormat	   = IniFile->ReadString( sct, "CsvFormat",			"GP,TP,TX,LK,CT,MT");

	ExportUTF8	   = IniFile->ReadBool(   sct, "ExportUTF8");

	IndFixRatio    = IniFile->ReadBool(   sct, "IndFixRatio");

	sct = "ViewInfo";
	show_GroupFrame= IniFile->ReadBool(   sct, "GroupFrame",	true);
	show_GroupLine = IniFile->ReadBool(   sct, "GroupLine",		true);
	show_FragLine  = IniFile->ReadBool(   sct, "FragLine",		true);
	show_NamePlate = IniFile->ReadBool(   sct, "NamePlate",		true);
	show_LinkMark  = IniFile->ReadBool(   sct, "LinkMark",		true);
	show_DivLine   = IniFile->ReadBool(   sct, "DivLine",		true);
	show_PosGrid   = IniFile->ReadBool(   sct, "PosGrid");

	sct = "GroupList";
	GLstInterLine  = IniFile->ReadInteger(sct, "ListInterLine",	0);
	HideBlkGrpItem = IniFile->ReadBool(   sct, "HideBlkGrpItem"	);

	sct = "FragList";
	FLstInterLine  = IniFile->ReadInteger(sct, "ListInterLine",	0);
	FLstSortDir    = IniFile->ReadInteger(sct, "SortDirec",		1);
	if (FLstSortDir==0) FLstSortDir = 1;

	sct = "FindEx";
	XLstInterLine  = IniFile->ReadInteger(sct, "ListInterLine",	0);

	sct = "Fragment";
	def_wd		   = IniFile->ReadInteger(sct, "FrgWidth",		12);
	def_hi		   = IniFile->ReadInteger(sct, "FrgHeight",		3);
	nam_wd		   = IniFile->ReadInteger(sct, "NamWidth",		10);
	nam_hi		   = IniFile->ReadInteger(sct, "NamHeight",		1);
	img_r1		   = IniFile->ReadInteger(sct, "ImgRatio1",		1);
	img_r2		   = IniFile->ReadInteger(sct, "ImgRatio2",		1);
	new_frg_pos	   = IniFile->ReadInteger(sct, "NewPosition",	1);
	new_no_group   = IniFile->ReadBool(   sct, "NewNoGroup",	true);
	auto_exp	   = IniFile->ReadBool(   sct, "AutoExpand");
	input_wordwrap = IniFile->ReadBool(   sct, "InputWordWrap",	true);
	ime_on		   = IniFile->ReadBool(   sct, "IME_On");
	accept_cr	   = IniFile->ReadBool(   sct, "Accept_CR");
	cr_returnkey   = IniFile->ReadBool(   sct, "CR_ReturnKey");
	ok_ctrl_return = IniFile->ReadBool(   sct, "OK_CtrlReturn");
	CR_str		   = IniFile->ReadString(sct, "CR_String");
	if (CR_str.IsEmpty()) CR_str = "＼";
	LinkIsIcon	   = IniFile->ReadBool(   sct, "LinkIsIcon",	true);
	LinkRelPath	   = IniFile->ReadBool(   sct, "LinkRelPath");
	crFragMove	   = (TCursor)IniFile->ReadInteger(sct, "MoveCurosr",	crHandGrabR);
	SpreadAlign    = IniFile->ReadBool(   sct, "SpreadAlign",	true);
	SpreadShuffle  = IniFile->ReadBool(   sct, "SpreadShuffle");
	away_frg_pos   = IniFile->ReadInteger(sct, "AwayPosition",	0);
	ArrangePt	   = IniFile->ReadInteger(sct, "ArrangePt",		1);
	frg_mgn 	   = IniFile->ReadInteger(sct, "Margin",		3);
	frg_itl 	   = IniFile->ReadInteger(sct, "InterLine",		2);
	plt_mgn 	   = IniFile->ReadInteger(sct, "PlateMargin",	3);
	WicScaleOpt	   = IniFile->ReadInteger(sct, "WicScaleOpt",	1);

	sct = "Color";
	col_defFG	   = IniFile->ReadColor(sct, "DefFrgFG", 		clBlack);
	col_defBG	   = IniFile->ReadColor(sct, "DefFrgBG", 		clWhite);
	col_selFG	   = IniFile->ReadColor(sct, "SelFrgFG", 		clWhite);
	col_selBG	   = IniFile->ReadColor(sct, "SelFrgBG", 		clNavy);
	col_namFG	   = IniFile->ReadColor(sct, "NamFrgFG", 		clBlack);
	col_namBG	   = IniFile->ReadColor(sct, "NamFrgBG", 		clSilver);
	col_Border	   = IniFile->ReadColor(sct, "FrgBorder",		clBlack);
	col_symFG	   = IniFile->ReadColor(sct, "SymbolFG", 		clWhite);
	col_LinLbl	   = IniFile->ReadColor(sct, "LineLabel", 		clBlack);
	col_Title	   = IniFile->ReadColor(sct, "DataTitle", 		clBlack);
	col_backgr	   = IniFile->ReadColor(sct, "Background",		clTeal);
	col_selRct	   = IniFile->ReadColor(sct, "SelectRect",		clRed);
	col_glstFG	   = IniFile->ReadColor(sct, "GroupListFG",		clWindowText);
	col_glstBG	   = IniFile->ReadColor(sct, "GroupListBG",		clWindow);
	col_glstOff    = IniFile->ReadColor(sct, "GroupListOFF",	clWindowText);
	col_xlstFG	   = IniFile->ReadColor(sct, "FindExListFG",	clWindowText);
	col_xlstBG	   = IniFile->ReadColor(sct, "FindExListBG",	clWindow);
	col_AreaFrm    = IniFile->ReadColor(sct, "AreaFrame",		clRed);
	col_MapBG	   = IniFile->ReadColor(sct, "MapView",			Graphics::clNone);
	col_GrpFrm	   = IniFile->ReadColor(sct, "GroupFrame",		clYellow);
	col_GrpLine    = IniFile->ReadColor(sct, "GroupLine",		clAqua);
	col_FrgLine    = IniFile->ReadColor(sct, "FragLine",		clAqua);
	col_DivLine    = IniFile->ReadColor(sct, "DivLine",			clDkGray);
	col_Brush	   = IniFile->ReadColor(sct, "BrushTool",		col_defBG);
	col_PosGrid    = IniFile->ReadColor(sct, "PosGrid",			clNavy);
	col_OptFind    = IniFile->ReadColor(sct, "OptFind",			clYellow);

	//カスタムカラー
	sct = "CustomColors";
	col_Custom[0]  = IniFile->ReadInteger(sct, "ColorA", 0xa0a0ff);
	col_Custom[1]  = IniFile->ReadInteger(sct, "ColorB", 0x80c0ff);
	col_Custom[2]  = IniFile->ReadInteger(sct, "ColorC", 0xa0ffff);
	col_Custom[3]  = IniFile->ReadInteger(sct, "ColorD", 0xa0ffa0);
	col_Custom[4]  = IniFile->ReadInteger(sct, "ColorE", 0xffffc0);
	col_Custom[5]  = IniFile->ReadInteger(sct, "ColorF", 0xffc0c0);
	col_Custom[6]  = IniFile->ReadInteger(sct, "ColorG", 0xffc0ff);
	col_Custom[7]  = IniFile->ReadInteger(sct, "ColorH", 0xd0d0d0);
	col_Custom[8]  = IniFile->ReadInteger(sct, "ColorI", 0xffffff);
	col_Custom[9]  = IniFile->ReadInteger(sct, "ColorJ", 0xffffff);
	col_Custom[10] = IniFile->ReadInteger(sct, "ColorK", 0xffffff);
	col_Custom[11] = IniFile->ReadInteger(sct, "ColorL", 0xffffff);
	col_Custom[12] = IniFile->ReadInteger(sct, "ColorM", 0xffffff);
	col_Custom[13] = IniFile->ReadInteger(sct, "ColorN", 0xffffff);
	col_Custom[14] = IniFile->ReadInteger(sct, "ColorO", 0xffffff);
	col_Custom[15] = IniFile->ReadInteger(sct, "ColorP", 0xffffff);

	//関連づけ
	IniFile->ReadSection("Associate", AssociateList);

	//ファイル履歴を読み込む
	UnicodeString histstr;
	for (int i=0; i<MAX_FILEHISTORY; i++) {
		histstr = IniFile->ReadString("FileHistory", "File" + IntToStr(i));
		if (histstr.IsEmpty()) break;
		if (FileExists(histstr)) FileHistoryList->Add(histstr);
	}
	//検索履歴を読み込む
	for (int i=0; i<MAX_FINDHISTORY; i++) {
		histstr = trim_ex(IniFile->ReadString("FindHistory", "Find" + IntToStr(i)));
		if (histstr.IsEmpty()) break;
		if (FindHistoryList->IndexOf(histstr)==-1) FindHistoryList->Add(histstr);
	}

	//Susieプラグイン初期化
	SPI = new SpiUnit(SpiDir);

	LastDockSite = NULL;

	reqUpdate	 = false;
	reqFrgLst	 = false;
	printer_ok	 = false;
	TimerLockCnt = 1;
}

//---------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------
EnvData::~EnvData()
{
	//後片づけ
	delete frgFont;
	delete namFont;
	delete symFont;
	delete lblFont;
	delete titFont;
	delete glstFont;
	delete flstFont;
	delete xlstFont;
	delete stbarFont;
	delete dialogFont;

	delete LinkMark;

	for (int i=0; i<LinkIconList->Count; i++) delete (TIcon*)LinkIconList->Objects[i];
	delete LinkIconList;

	for (int i=0; i<CachedIcoList->Count; i++) delete (TIcon*)CachedIcoList->Objects[i];
	delete CachedIcoList;

	delete AssociateList;
	delete FileHistoryList;
	delete FindHistoryList;
	delete JumpHistoryList;

	delete SPI;
}

//---------------------------------------------------------------------
//位置情報読み込み
//---------------------------------------------------------------------------
void __fastcall EnvData::load_pos_info(UnicodeString sct, TForm *frm,
	int x, int y, int w, int h)
{
	frm->Left = IniFile->ReadInteger(sct, "WinLeft", x);
	frm->Top  = IniFile->ReadInteger(sct, "WinTop",  y);
	if (frm->BorderStyle!=bsDialog && w>0 && h>0) {
		int ww = IniFile->ReadInteger(sct, "WinWidth",  w);
		if (ww<=0) ww = w;
		int wh = IniFile->ReadInteger(sct, "WinHeight", h);
		if (wh<=0) wh = h;
		frm->Width	= ww;
		frm->Height = wh;
	}

	if (frm->BorderStyle==bsSizeable)
		frm->WindowState = (TWindowState)IniFile->ReadInteger(sct, "WinState", (int)wsNormal);
}
//---------------------------------------------------------------------
//位置情報保存
//---------------------------------------------------------------------------
void __fastcall EnvData::save_pos_info(UnicodeString sct, TForm *frm)
{
	if (frm->BorderStyle==bsSizeable) {
		if (frm->WindowState==wsMinimized) frm->WindowState = wsNormal;
		IniFile->WriteInteger(sct, "WinState",	(int)frm->WindowState);
		if (frm->WindowState==wsMaximized) frm->WindowState = wsNormal;
	}
	IniFile->WriteInteger(sct, "WinLeft", frm->Left);
	IniFile->WriteInteger(sct, "WinTop",  frm->Top);
	if (frm->BorderStyle!=bsDialog) {
		IniFile->WriteInteger(sct, "WinWidth",  frm->Width);
		IniFile->WriteInteger(sct, "WinHeight", frm->Height);
	}
}

//---------------------------------------------------------------------------
//ツールバー情報読み込み
//---------------------------------------------------------------------------
void __fastcall EnvData::load_tool_info(UnicodeString sct, TToolBar *tb)
{
	if (IniFile->ReadBool(sct, "Floating", false)) {
		tb->ManualFloat(
			Rect(IniFile->ReadInteger(sct, "FltLeft",	100),
				 IniFile->ReadInteger(sct, "FltTop",	100),
				 IniFile->ReadInteger(sct, "FltRight",	200),
				 IniFile->ReadInteger(sct, "FltBottom",	200))
			);
	}
	else {
		tb->Top  = IniFile->ReadInteger(sct, "DockTop",	tb->Top);
		tb->Left = IniFile->ReadInteger(sct, "DockLeft",tb->Left);
	}
	tb->Visible = IniFile->ReadBool(sct, "Visible", true);
}
//---------------------------------------------------------------------------
//ツールバー情報保存
//---------------------------------------------------------------------------
void __fastcall EnvData::save_tool_info(UnicodeString sct, TToolBar *tb)
{
	IniFile->WriteBool(sct, "Visible", 	tb->Visible);
	IniFile->WriteBool(sct, "Floating",	tb->Floating);
	if (tb->Floating) {
		TControl *st = tb->HostDockSite;
		TRect rc = st->BoundsRect;
		IniFile->WriteInteger(sct, "FltLeft",	rc.Left);
		IniFile->WriteInteger(sct, "FltTop",	rc.Top);
		IniFile->WriteInteger(sct, "FltRight",	rc.Right);
		IniFile->WriteInteger(sct, "FltBottom",	rc.Bottom);
	}
	else {
		IniFile->WriteInteger(sct, "DockTop",	tb->Top);
		IniFile->WriteInteger(sct, "DockLeft",	tb->Left);
	}
}

//---------------------------------------------------------------------------
//ファイル履歴を追加
//---------------------------------------------------------------------------
void __fastcall EnvData::add_file_history(UnicodeString fnam)
{
	int idx = -1;
	for (int i=0; i<FileHistoryList->Count; i++) {
		if (SameText(FileHistoryList->Strings[i], fnam)) {
			idx	= i; break;
		}
	}
	if (idx!=-1) FileHistoryList->Delete(idx);	//前の履歴を消して
	FileHistoryList->Insert(0, fnam);			//先頭に追加
}

//---------------------------------------------------------------------------
//カスタムカラーを設定 (TColorDialog::CustomColors)
//---------------------------------------------------------------------------
void __fastcall EnvData::set_custom_col(TStrings *cust_col)
{
	cust_col->Clear();
	for (int i=0; i<16; i++)
		cust_col->Add(UnicodeString().sprintf(_T("Color%c=%06X"), 'A' + i, col_Custom[i]));
}
//---------------------------------------------------------------------------
//カスタムカラーを取得 (TColorDialog::CustomColors)
//---------------------------------------------------------------------------
void __fastcall EnvData::get_custom_col(TStrings *cust_col)
{
	for (int i=0; i<cust_col->Count; i++) {
		UnicodeString tmpstr = "0x" + cust_col->Strings[i].SubString(8, 6);
		EV->col_Custom[i] = tmpstr.ToIntDef(0xffffff);
	}
}

//---------------------------------------------------------------------------
//関連づけを取得
//---------------------------------------------------------------------------
UnicodeString __fastcall EnvData::get_associate(UnicodeString fnam)
{
	UnicodeString ext = get_tkn_r(ExtractFileExt(fnam).LowerCase(), ".");
	return (ext.IsEmpty()? EmptyStr : AssociateList->Values[ext]);
}

//---------------------------------------------------------------------------
//設定をINIファイルに保存
//---------------------------------------------------------------------------
bool __fastcall EnvData::save_ini_file()
{
	IniFile->WriteFontInf("Fragment",	frgFont);
	IniFile->WriteFontInf("NamePlate",	namFont);
	IniFile->WriteFontInf("Symbol",		symFont);
	IniFile->WriteFontInf("LineLabel",	lblFont);
	IniFile->WriteFontInf("DataTitle",	titFont);
	IniFile->WriteFontInf("GroupList",	glstFont);
	IniFile->WriteFontInf("FragList",	flstFont);
	IniFile->WriteFontInf("FindEx",		xlstFont);
	IniFile->WriteFontInf("StatusBar",	stbarFont);
	IniFile->WriteFontInf("Dialog",		dialogFont);

	UnicodeString sct = "General";
	IniFile->WriteString( sct, "HomeWindow",		HomeWindow);
	IniFile->WriteBool(   sct, "HomWinMaximize",	HomWinMax);
	IniFile->WriteString( sct, "LastFile",			LastFile);
	IniFile->WriteBool(   sct, "OpenLastFile",		OpenLastFile);
	IniFile->WriteBool(   sct, "ChildWinMaximize",	ChildMax);
	IniFile->WriteBool(	  sct, "HideTitleMenu",		HideTitleMenu);
	IniFile->WriteBool(   sct, "RestoreScrPos",		RstScrPos);
	IniFile->WriteBool(   sct, "ActivateDistWin",	ActivDistWin);
	IniFile->WriteBool(   sct, "MapViewSelRect",	MapViewSelRect);
	IniFile->WriteBool(   sct, "FitMapView",		FitMapView);
	IniFile->WriteInteger(sct, "MapAlpha",			MapAlpha);
	IniFile->WriteBool(   sct, "CopyCLine",			CopyCLine);
	IniFile->WriteBool(   sct, "ClipPosSort",		ClipPosSort);
	IniFile->WriteBool(   sct, "DropImgFile",		DropImgFile);
	IniFile->WriteBool(   sct, "NewDropPos",		NewDropPos);
	IniFile->WriteBool(   sct, "ShowDstGrp",		ShowDstGrp);
	IniFile->WriteBool(   sct, "FrameAllSelect",	FrameAllSel);
	IniFile->WriteBool(   sct, "ChgGrpModeCtrl",	ChgGrpModeCtrl);
	IniFile->WriteBool(   sct, "RDragScroll",		RDragScroll);
	IniFile->WriteBool(   sct, "SelJumpFrg",		SelJumpFrg);

	IniFile->WriteBool(   sct, "ImageIsBack",		ImageIsBack);
	IniFile->WriteBool(   sct, "AlwaysShowNp",		AlwaysShowNp);
	IniFile->WriteBool(   sct, "NoNpHideGfrm",		NoNpHideGfrm);
	IniFile->WriteBool(   sct, "OnlyNpHideGfrm",	OnlyNpHideGfrm);
	IniFile->WriteBool(   sct, "PlateOnGFrame",		PlateOnGfrm);
	IniFile->WriteInteger(sct, "PlateAlign",		PlateAlign);
	IniFile->WriteInteger(sct, "PlateShape",		PlateShape);

	IniFile->WriteInteger("GroupList","Align", 		glstAlign);
	IniFile->WriteInteger(sct, "GFrameLineWidth",	gfrmLineWidth);
	IniFile->WriteInteger(sct, "GFrameMaxRnd", 		gfrmMaxRnd);

	IniFile->WriteInteger(sct, "DefScrWidth",		DefScrWd);
	IniFile->WriteInteger(sct, "DefScrHeight",		DefScrHi);

	IniFile->WriteInteger(sct, "CurGLineType", 		CurGLineType);
	IniFile->WriteInteger(sct, "CurGLineWidth",		CurGLineWidth);
	IniFile->WriteInteger(sct, "CurGLineType",		CurGLineType);
	IniFile->WriteInteger(sct, "CurGLineWidth",		CurGLineWidth);
	IniFile->WriteInteger(sct, "CurGArrowType",		CurGArrowType);
	IniFile->WriteInteger(sct, "CurGArrowSize",		CurGArrowSize);
	IniFile->WriteInteger(sct, "CurFLineType",		CurFLineType);
	IniFile->WriteInteger(sct, "CurFLineWidth",		CurFLineWidth);
	IniFile->WriteInteger(sct, "CurFArrowType",		CurFArrowType);
	IniFile->WriteInteger(sct, "CurFArrowSize",		CurFArrowSize);
	IniFile->WriteBool(   sct, "CurTxOpposite",		CurTxOpposite);

	IniFile->WriteBool(   sct, "GLineRightAngle",	GlRightAngle);

	IniFile->WriteBool(   sct, "PrintMonochrome",	PrintMono);
	IniFile->WriteBool(   sct, "PrintTime",			PrintTime);
	IniFile->WriteInteger(sct, "FixPrnLWidth",		FixPrnLWidth);
	IniFile->WriteBool(   sct, "PrintBackGr",		PrintBackGr);
	IniFile->WriteBool(   sct, "PrintScrSize",		PrintScrSize);

	IniFile->WriteInteger(sct, "FindMode",			FindMode);

	IniFile->WriteInteger(sct, "ImgFilterIdx",		ImgFilterIdx);
	IniFile->WriteInteger(sct, "JpegQuality",		JpegQuality);

	IniFile->WriteInteger(sct, "WheelScrQty",		WheelScrQty);
	IniFile->WriteBool(   sct, "WheelShiftHorz",	WheelShiftHorz);

	IniFile->WriteInteger(sct, "GridSize",			GridSize);
	IniFile->WriteBool(   sct, "SnapGrid",			SnapGrid);

	IniFile->WriteBool(   sct, "TrimMovTop",		TrimMovTop);
	IniFile->WriteBool(   sct, "TrimScrSize",		TrimScrSize);
	IniFile->WriteBool(   sct, "TrimFixSize",		TrimFixSize);
	IniFile->WriteBool(   sct, "TrimSaveAs",		TrimSaveAs);
	IniFile->WriteInteger(sct, "TrimMargin",		TrimMargin);
	IniFile->WriteInteger(sct, "TrimFixWd",			TrimFixWd);
	IniFile->WriteInteger(sct, "TrimFixHi",			TrimFixHi);

	IniFile->WriteBool(   sct, "NewBlankDblClick",	NewBlkDblClick);

	IniFile->WriteString( sct, "SpiDir",			SpiDir);
	IniFile->WriteBool(   sct, "UseSpiFirst",		UseSpiFirst);

	IniFile->WriteInteger(sct, "StretchMode",	  	StretchMode);
	IniFile->WriteBool(   sct, "StretchGrp0",  		StretchGrp0);

	IniFile->WriteString( sct, "CsvFormat",			CsvFormat);

	IniFile->WriteBool(	  sct, "ExportUTF8",		ExportUTF8);

	IniFile->WriteBool(	  sct, "IndFixRatio",		IndFixRatio);

	sct = "ViewInfo";
	IniFile->WriteBool(   sct, "GroupFrame",		show_GroupFrame);
	IniFile->WriteBool(   sct, "GroupLine",			show_GroupLine);
	IniFile->WriteBool(   sct, "FragLine",			show_FragLine);
	IniFile->WriteBool(   sct, "NamePlate",			show_NamePlate);
	IniFile->WriteBool(   sct, "LinkMark",			show_LinkMark);
	IniFile->WriteBool(   sct, "DivLine",			show_DivLine);
	IniFile->WriteBool(   sct, "PosGrid",			show_PosGrid);

	sct = "GroupList";
	IniFile->WriteInteger(sct, "ListInterLine",		GLstInterLine);
	IniFile->WriteBool(   sct, "HideBlkGrpItem",	HideBlkGrpItem);

	sct = "FragList";
	IniFile->WriteInteger(sct, "ListInterLine",		FLstInterLine);
	IniFile->WriteInteger(sct, "SortDirec",			FLstSortDir);

	sct = "FindEx";
	IniFile->WriteInteger(sct, "ListInterLine",		XLstInterLine);

	sct = "Fragment";
	IniFile->WriteInteger(sct, "FrgWidth",	 	def_wd);
	IniFile->WriteInteger(sct, "FrgHeight",	 	def_hi);
	IniFile->WriteInteger(sct, "NamWidth",	 	nam_wd);
	IniFile->WriteInteger(sct, "NamHeight",	 	nam_hi);
	IniFile->WriteInteger(sct, "ImgRatio1",	 	img_r1);
	IniFile->WriteInteger(sct, "ImgRatio2",	 	img_r2);
	IniFile->WriteInteger(sct, "NewPosition",	new_frg_pos);
	IniFile->WriteBool(   sct, "NewNoGroup",	new_no_group);
	IniFile->WriteBool(   sct, "AutoExpand",	auto_exp);
	IniFile->WriteBool(   sct, "InputWordWrap",	input_wordwrap);
	IniFile->WriteBool(   sct, "IME_On",		ime_on);
	IniFile->WriteBool(   sct, "Accept_CR",	 	accept_cr);
	IniFile->WriteBool(   sct, "CR_ReturnKey", 	cr_returnkey);
	IniFile->WriteBool(   sct, "OK_CtrlReturn",	ok_ctrl_return);
	IniFile->WriteString( sct, "CR_String",	 	CR_str);
	IniFile->WriteBool(   sct, "LinkIsIcon",	LinkIsIcon);
	IniFile->WriteBool(   sct, "LinkRelPath",	LinkRelPath);
	IniFile->WriteInteger(sct, "MoveCurosr",	crFragMove);
	IniFile->WriteBool(   sct, "SpreadAlign",	SpreadAlign);
	IniFile->WriteBool(   sct, "SpreadShuffle",	SpreadShuffle);
	IniFile->WriteInteger(sct, "AwayPosition", 	away_frg_pos);
	IniFile->WriteInteger(sct, "ArrangePt",	 	ArrangePt);
	IniFile->WriteInteger(sct, "Margin",	 	frg_mgn);
	IniFile->WriteInteger(sct, "InterLine", 	frg_itl);
	IniFile->WriteInteger(sct, "PlateMargin", 	plt_mgn);
	IniFile->WriteInteger(sct, "WicScaleOpt",	WicScaleOpt);

	sct = "Color";
	IniFile->WriteInteger(sct, "DefFrgFG",	 	col_defFG);
	IniFile->WriteInteger(sct, "DefFrgBG",	 	col_defBG);
	IniFile->WriteInteger(sct, "SelFrgFG",	 	col_selFG);
	IniFile->WriteInteger(sct, "SelFrgBG",	 	col_selBG);
	IniFile->WriteInteger(sct, "NamFrgFG",		col_namFG);
	IniFile->WriteInteger(sct, "NamFrgBG",		col_namBG);
	IniFile->WriteInteger(sct, "FrgBorder",		col_Border);
	IniFile->WriteInteger(sct, "SymbolFG",		col_symFG);
	IniFile->WriteInteger(sct, "LineLabel",		col_LinLbl);
	IniFile->WriteInteger(sct, "DataTitle",		col_Title);
	IniFile->WriteInteger(sct, "Background", 	col_backgr);
	IniFile->WriteInteger(sct, "SelectRect", 	col_selRct);
	IniFile->WriteInteger(sct, "GroupListFG",	col_glstFG);
	IniFile->WriteInteger(sct, "GroupListBG",	col_glstBG);
	IniFile->WriteInteger(sct, "GroupListOff",	col_glstOff);
	IniFile->WriteInteger(sct, "FindExListFG",	col_xlstFG);
	IniFile->WriteInteger(sct, "FindExListBG",	col_xlstBG);
	IniFile->WriteInteger(sct, "AreaFrame",		col_AreaFrm);
	IniFile->WriteInteger(sct, "MapView",		col_MapBG);
	IniFile->WriteInteger(sct, "GroupFrame", 	col_GrpFrm);
	IniFile->WriteInteger(sct, "GroupLine",	 	col_GrpLine);
	IniFile->WriteInteger(sct, "FragLine",	 	col_FrgLine);
	IniFile->WriteInteger(sct, "DivLine",	 	col_DivLine);
	IniFile->WriteInteger(sct, "BrushTool",	 	col_Brush);
	IniFile->WriteInteger(sct, "PosGrid",	 	col_PosGrid);
	IniFile->WriteInteger(sct, "OptFind",		col_OptFind);

	//カスタムカラー
	for (int i=0; i<16; i++) {
		IniFile->WriteInteger("CustomColors", UnicodeString().sprintf(_T("Color%c"), 'A' + i), col_Custom[i]);
	}

	//関連づけ
	IniFile->AssignSection("Associate", AssociateList);

	//ファイル履歴を保存
	for (int i=0; i<MAX_FILEHISTORY; i++) {
		IniFile->WriteString("FileHistory", "File" + IntToStr(i),
			(i<FileHistoryList->Count)? FileHistoryList->Strings[i] : EmptyStr);
	}

	//検索履歴を保存
	for (int i=0; i<MAX_FINDHISTORY; i++) {
		IniFile->WriteString("FindHistory", "Find" + IntToStr(i),
			(i<FindHistoryList->Count)? FindHistoryList->Strings[i] : EmptyStr);
	}

	return IniFile->UpdateFile();
}
//---------------------------------------------------------------------------
