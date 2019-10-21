//----------------------------------------------------------------------//
// オプション設定ダイアログ												//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include "UserFunc.h"
#include "fragset.h"
#include "EnvData.h"
#include "PropDlg.h"
#include "Main.h"
#include "OptDlg.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"

TOptionDlg *OptionDlg;

//---------------------------------------------------------------------
//配色コンボボックスのインデックス
#define IXCL_defFG		0
#define IXCL_defBG		1
#define IXCL_selFG		2
#define IXCL_selBG		3
#define IXCL_namFG		4
#define IXCL_namBG		5
#define IXCL_Border 	6
#define IXCL_selRct 	7
#define IXCL_symFG		8
#define IXCL_backgr 	9
#define IXCL_Title		10
#define IXCL_FrgLine	11
#define IXCL_GrpFrm 	12
#define IXCL_GrpLine	13
#define IXCL_LinLbl 	14
#define IXCL_glstFG 	15
#define IXCL_glstOff	16
#define IXCL_glstBG 	17
#define IXCL_MapBG		18
#define IXCL_AreaFrm	19
#define IXCL_DivLine	20
#define IXCL_PosGrid	21
#define IXCL_xlstFG 	22
#define IXCL_xlstBG 	23
#define IXCL_OptFind 	24

//フォントコンボボックスのインデックス
#define IXFNT_frg	0
#define IXFNT_nam	1
#define IXFNT_sym	2
#define IXFNT_lbl	3
#define IXFNT_tit	4
#define IXFNT_glst	5
#define IXFNT_flst	6
#define IXFNT_xlst	7
#define IXFNT_sbar  8
#define IXFNT_dlg   9

//サイズ／形状コンボボックスのインデックス
#define IXSZ_GfrmLwd	0	//グループ枠の線幅
#define IXSZ_GfrmRnd	1	//グループ枠の最大丸み
#define IXSZ_GlIntLn	2	//グループリストの行間
#define IXSZ_FlIntLn	3	//断片リストの行間
#define IXSZ_ExIntLn	4	//拡張検索リストの行間
#define IXSZ_FrgMgn		5	//断片の余白

//---------------------------------------------------------------------
__fastcall TOptionDlg::TOptionDlg(TComponent* AOwner) : TForm(AOwner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormCreate(TObject *Sender)
{
	TitleFont  = new TFont();
	LinLblFont = new TFont();
	FrgLstFont = new TFont();
	ExLstFont  = new TFont();
	DialogFont = new TFont();

	FindMarkList = new MarkList(this);

	//配色リストボックス
	ColListBox->ItemHeight = abs(ColListBox->Font->Height) * 3 / 2;

	ColListBox->Items->Text =
		"断片の文字(デフォルト)\r\n"
		"断片の背景(デフォルト)\r\n"
		"選択中断片の文字\r\n"
		"選択中断片の背景\r\n"
		"表札の文字(デフォルト)\r\n"
		"表札の背景(デフォルト)\r\n"
		"断片の輪郭\r\n"
		"注目断片の輪郭\r\n"
		"標識の文字(デフォルト)\r\n"
		"ウィンドウの背景\r\n"
		"タイトル\r\n"
		"断片関係線\r\n"
		"グループ枠\r\n"
		"グループ関係線\r\n"
		"関係線ラベル\r\n"
		"グループリストの文字\r\n"
		"非表示グループの文字\r\n"
		"グループリストの背景\r\n"
		"全体図の背景\r\n"
		"全体図のエリア枠\r\n"
		"画面分割線\r\n"
		"配置グリッド\r\n"
		"拡張検索リストの文字\r\n"
		"拡張検索リストの背景\r\n"
		"オプション設定の検索結果強調\r\n";

	//フォントコンボボックス
	str_to_ComboBox(FontComboBox,
		"断片|表札|標識|関係線ラベル|タイトル|グループリスト|断片リスト|拡張検索リスト|ステータスバー|ダイアログ");

	//サイズコンボボックス
	str_to_ComboBox(SizSpComboBox, 
		"グループ枠の線幅|グループ枠の最大丸み|グループリストの行間|"
		"断片リストの行間|拡張検索リストの行間|断片の余白");

	//最小印刷線幅
	str_to_ComboBox(MinPrnLWCombo, "可変 (最小 1)|固定 1|固定 2|固定 3|固定 4|固定 5");

	//画像の拡大・縮小アルゴリズム
	str_to_ComboBox(ScaleOptComboBox,
		"ニアレストネイバー|バイリニア|バイキュービック|ファント・リサンプリング|補完しない");

	//キー選択リスト
	make_key_list(KeyComboBox->Items);

	str_to_ComboBox(PlateAlignComboBox, "左上(右上)|上中央(右中央)|右上(右下)");
	str_to_ComboBox(PlateShapeComboBox, "長方形|角丸形|端丸形");
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormShow(TObject *Sender)
{
	FindEdit->Text = EmptyStr;
	HelpContext = PageControl1->ActivePage->HelpContext;

	gfrmMaxRnd	  = EV->gfrmMaxRnd;
	GLstInterLine = EV->GLstInterLine;
	FLstInterLine = EV->FLstInterLine;
	XLstInterLine = EV->XLstInterLine;
	FrgMargin	  = EV->frg_mgn;

	//サンプル表示
	SamplePanel->Color				  = EV->col_backgr;
	NrmFrgImage->Canvas->Brush->Color = EV->col_defBG;
	NrmFrgImage->Canvas->Pen->Color   = EV->col_Border;
	NrmFrgImage->Canvas->Font->Assign(EV->frgFont);
	NrmFrgImage->Canvas->Font->Color  = EV->col_defFG;
	SelFrgImage->Canvas->Brush->Color = EV->col_selBG;
	SelFrgImage->Canvas->Pen->Color   = EV->col_selRct;
	SelFrgImage->Canvas->Font->Assign(EV->frgFont);
	SelFrgImage->Canvas->Font->Color  = EV->col_selFG;
	PlateImage->Canvas->Brush->Color  = EV->col_namBG;
	PlateImage->Canvas->Pen->Color	  = EV->col_Border;
	PlateImage->Canvas->Font->Assign(EV->namFont);
	PlateImage->Canvas->Font->Color   = EV->col_namFG;
	SymLabel->Font->Assign(EV->symFont);
	SymLabel->Font->Color			  = EV->col_symFG;
	GrpFrmShape->Pen->Color 		  = EV->col_GrpFrm;
	GrpFrmShape->Pen->Width 		  = EV->gfrmLineWidth;
	StatusBar1->Font->Assign(EV->stbarFont);
	StatusBar1->Canvas->Font->Assign(EV->stbarFont);
	StatusBar1->ClientHeight = StatusBar1->Canvas->TextHeight("断片") + 4;
	GrpCheckListBox->Font->Assign(EV->glstFont);
	GrpCheckListBox->Font->Color = EV->col_glstFG;
	GrpCheckListBox->Color		 = EV->col_glstBG;
	GrpCheckListBox->Style		 = lbOwnerDrawFixed;
	GrpCheckListBox->ItemHeight  = get_FontHeightP(EV->glstFont, EV->GLstInterLine);

	TitleFont->Assign(EV->titFont);
	LinLblFont->Assign(EV->lblFont);
	FrgLstFont->Assign(EV->flstFont);
	ExLstFont->Assign(EV->xlstFont);
	DialogFont->Assign(EV->dialogFont);

	colbuf_GrpLine = EV->col_GrpLine;
	colbuf_FrgLine = EV->col_FrgLine;
	colbuf_LinLbl  = EV->col_LinLbl;
	colbuf_Title   = EV->col_Title;
	colbuf_AreaFrm = EV->col_AreaFrm;
	colbuf_MapBG   = EV->col_MapBG;
	colbuf_DivLine = EV->col_DivLine;
	colbuf_PosGrid = EV->col_PosGrid;
	colbuf_glstOff = EV->col_glstOff;
	colbuf_selRect = EV->col_selRct;
	colbuf_xlstFG  = EV->col_xlstFG;
	colbuf_xlstBG  = EV->col_xlstBG;
	colbuf_OptFind = EV->col_OptFind;

	SizSpComboBox->ItemIndex = -1;
	SizValComboBox->Clear();

	SampleGLine.lType = LTYPE_S_ARROW;
	SampleGLine.vx0   = 0;
	SampleGLine.vy0   = GLinePBox->ClientHeight;
	SampleGLine.vx1   = GLinePBox->ClientWidth - 50;
	SampleGLine.vy1   = 0;
	SampleGLine.TextStr = "ラベル";

	SampleFLine.lType = LTYPE_S_ARROW;
	SampleFLine.vx0   = FLinePBox->ClientWidth;
	SampleFLine.vy0   = 0;
	SampleFLine.vx1   = 0;
	SampleFLine.vy1   = FLinePBox->ClientHeight;

	//断片サイズ
	InitUpDown(WdUpDown,	EV->def_wd);
	InitUpDown(HiUpDown,	EV->def_hi);
	InitUpDown(NpWdUpDown,	EV->nam_wd);
	InitUpDown(NpHiUpDown,	EV->nam_hi);
	InitUpDown(ImgR1UpDown,	EV->img_r1);
	InitUpDown(ImgR2UpDown,	EV->img_r2);

	NewPosCombo->ItemIndex		= EV->new_frg_pos - 1;
	AwayPosCombo->ItemIndex 	= EV->away_frg_pos;
	NoGrpCheckBox->Checked		= EV->new_no_group;
	AlwaysShowCheckBox->Checked = EV->AlwaysShowNp;
	AutoExpCheckBox->Checked	= EV->auto_exp;
	WdWrapCheckBox->Checked 	= EV->input_wordwrap;
	ImeOnCheckBox->Checked		= EV->ime_on;
	AcceptCrCheckBox->Checked	= EV->accept_cr;
	CrRetKeyCheckBox->Checked	= EV->cr_returnkey;
	CrRetKeyCheckBox->Enabled	= AcceptCrCheckBox->Checked;
	CtlRetOkCheckBox->Checked	= EV->ok_ctrl_return;
	CtlRetOkCheckBox->Enabled	= CrRetKeyCheckBox->Checked;
	CrStrEdit->Text 			= EV->CR_str;
	LnkIconCheckBox->Checked	= EV->LinkIsIcon;
	LnkRelCheckBox->Checked 	= EV->LinkRelPath;
	CrStrEdit->Enabled			= AcceptCrCheckBox->Checked;
	ShuffleCheckBox->Checked	= EV->SpreadShuffle;
	AlignCheckBox->Checked		= EV->SpreadAlign;
	ScaleOptComboBox->ItemIndex = EV->WicScaleOpt;

	InitUpDown(ArngPtUpDown,	EV->ArrangePt);

	//メニューの親項目
	ParentCombo->Clear();
	for (int i=0; i<IdeaFragMainForm->ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)IdeaFragMainForm->ActionList1->Actions[i];
		if (ParentCombo->Items->IndexOf(ap->Category)==-1)
			ParentCombo->Items->Add(ap->Category);
	}
	for (int i=0; i<FrgPropDlg->ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)FrgPropDlg->ActionList1->Actions[i];
		if (ParentCombo->Items->IndexOf(ap->Category)==-1)
			ParentCombo->Items->Add(ap->Category);
	}

	ParentCombo->ItemIndex = 0;
	//メニュー選択リスト
	ShortCutList->ItemHeight = std::max((abs(ShortCutList->Font->Height) / 2 * 3), 20);
	ShortCutList->Clear();
	UnicodeString ctg_str, tmp;
	for (int i=0; i<IdeaFragMainForm->ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)IdeaFragMainForm->ActionList1->Actions[i];
		int brk = (i>0 && !SameStr(ctg_str, ap->Category))? 1 : 0;
		tmp.sprintf(_T("%s\t%s\t%u"), ap->Caption.c_str(), ShortCutToText(ap->ShortCut).c_str(), ap->ImageIndex);
		ShortCutList->Items->AddObject(tmp, (TObject*)(NativeInt)brk);
		ctg_str = ap->Category;
	}
	for (int i=0; i<FrgPropDlg->ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)FrgPropDlg->ActionList1->Actions[i];
		int brk = (i==0);
		tmp.sprintf(_T("%s\t%s\t-1"), ap->Hint.c_str(), ShortCutToText(ap->ShortCut).c_str());
		ShortCutList->Items->AddObject(tmp, (TObject*)(NativeInt)brk);
	}
	ShortCutList->ItemIndex = 0;

	HomeWinEdit->Text		   = EV->HomeWindow;
	HomWinMaxCheckBox->Checked = EV->HomWinMax;
	ChildMaxCheckBox->Checked  = EV->ChildMax;
	HideTitleCheckBox->Checked = EV->HideTitleMenu;
	RstScrPosCheckBox->Checked = EV->RstScrPos;
	ActDstWinCheckBox->Checked = EV->ActivDistWin;
	OpenLastCheckBox->Checked  = EV->OpenLastFile;

	InitUpDown(ScrWdUpDown,	EV->DefScrWd);
	InitUpDown(ScrHiUpDown,	EV->DefScrHi);
	MovCsrCombo->ItemIndex = (EV->crFragMove==crHandGrabL)? 1 : (EV->crFragMove==crHandGrabR)? 2 : 0;

	PlateOnGfrmCheckBox->Checked	= EV->PlateOnGfrm;
	PlateAlignComboBox->ItemIndex	= EV->PlateAlign;
	PlateShapeComboBox->ItemIndex	= EV->PlateShape;

	NoNpHideGfrmCheckBox->Checked	= EV->NoNpHideGfrm;
	OnlyNpHideGfrmCheckBox->Checked	= EV->OnlyNpHideGfrm;
	GlRightAngCheckBox->Checked 	= EV->GlRightAngle;
	MapVSelRctCheckBox->Checked		= EV->MapViewSelRect;
	FitMapVCheckBox->Checked		= EV->FitMapView;
	MapAlphaEdit->Text				= EV->MapAlpha;
	CopyCLineCheckBox->Checked		= EV->CopyCLine;
	PosSortCheckBox->Checked		= EV->ClipPosSort;
	NewDropPosCheckBox->Checked 	= EV->NewDropPos;
	DropImgCheckBox->Checked		= EV->DropImgFile;
	ShowDstGrpCheckBox->Checked 	= EV->ShowDstGrp;
	ImgIsBakCheckBox->Checked		= EV->ImageIsBack;
	FrmAllSelCheckBox->Checked		= EV->FrameAllSel;
	ChgGrpModeCtrlCheckBox->Checked = EV->ChgGrpModeCtrl;
	RDragScrollCheckBox->Checked	= EV->RDragScroll;
	SelJumpFrgCheckBox->Checked 	= EV->SelJumpFrg;
	NewBlkDblClickCheckBox->Checked = EV->NewBlkDblClick;
	PrintMonoCheckBox->Checked		= EV->PrintMono;
	PrintBgCheckBox->Checked		= EV->PrintBackGr;
	PrintBgCheckBox->Enabled		= !PrintMonoCheckBox->Checked;
	PrintTimeCheckBox->Checked		= EV->PrintTime;
	PrintScrSizeCheckBox->Checked	= EV->PrintScrSize;
	MinPrnLWCombo->ItemIndex		= EV->FixPrnLWidth;
	JpgQTrackBar->Position			= EV->JpegQuality;
	JpgQTrackBarChange(NULL);

	ScrQtyEdit->Text			= EV->WheelScrQty;
	ShiftHorzCheckBox->Checked	= EV->WheelShiftHorz;

	InitUpDown(GridSzUpDown,	EV->GridSize);
	SnapCheckBox->Checked	= EV->SnapGrid;

	TrmSizCheckBox->Checked = EV->TrimScrSize;
	TrmMovCheckBox->Checked	= EV->TrimMovTop;
	TrmFixCheckBox->Checked = EV->TrimFixSize;
	TrmSavCheckBox->Checked	= EV->TrimSaveAs;

	InitUpDown(TrmMgnUpDown,	EV->TrimMargin);
	InitUpDown(TrmWdUpDown,		EV->TrimFixWd);
	InitUpDown(TrmHiUpDown,		EV->TrimFixHi);

	SetSpiInfo(EV->SpiDir);
	UseSpiFirstCheckBox->Checked = EV->UseSpiFirst;

	FragSet *fsp = IdeaFragMainForm->CurFS;
	CurFileName = fsp? fsp->file_name : EmptyStr;
	SetCurHomBtn->Enabled = !CurFileName.IsEmpty();

	AssociateListBox->Items->Assign(EV->AssociateList);
	AsoDelBtn->Enabled = false;

	ExportUtf8CheckBox->Checked = EV->ExportUTF8;

	//エクスポート項目
	ExpItemListBox1->Clear();
	ExpItemListBox1->Items->Add("グループ");
	ExpItemListBox1->Items->Add("種類");
	ExpItemListBox1->Items->Add("テキスト");
	ExpItemListBox1->Items->Add("文字色");
	ExpItemListBox1->Items->Add("背景色");
	ExpItemListBox1->Items->Add("リンク先");
	ExpItemListBox1->Items->Add("作成日時");
	ExpItemListBox1->Items->Add("更新日時");

 	UnicodeString lbuf = EV->CsvFormat;
	lbuf = ReplaceStr(lbuf, "GP", "グループ");
	lbuf = ReplaceStr(lbuf, "TP", "種類");
	lbuf = ReplaceStr(lbuf, "TX", "テキスト");
	lbuf = ReplaceStr(lbuf, "FG", "文字色");
	lbuf = ReplaceStr(lbuf, "BG", "背景色");
	lbuf = ReplaceStr(lbuf, "LK", "リンク先");
	lbuf = ReplaceStr(lbuf, "CT", "作成日時");
	lbuf = ReplaceStr(lbuf, "MT", "更新日時");

	ExpItemListBox2->Clear();

	TStringDynArray itm_buf = get_csv_array(lbuf, 20);
	for (int i=0; i<itm_buf.Length; i++) {
		UnicodeString key = itm_buf[i];
		int idx = ExpItemListBox1->Items->IndexOf(key);
		if (idx!=-1) {
			ExpItemListBox2->Items->Add(ExpItemListBox1->Items->Strings[idx]);
			ExpItemListBox1->Items->Delete(idx);
		}
	}

	TabSheet1Show(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormDestroy(TObject *Sender)
{
	delete TitleFont;
	delete LinLblFont;
	delete FrgLstFont;
	delete ExLstFont;
	delete DialogFont;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::PageControl1Change(TObject *Sender)
{
	HelpContext = PageControl1->ActivePage->HelpContext;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FrgTextOut(TImage *img, UnicodeString s)
{
	TCanvas *cv = img->Canvas;
	int mgn = FrgMargin + 1;
	if (cv->Font->Name[1]=='@')
		vert_textout(cv, img->ClientWidth - mgn, mgn, s);
	else
		cv->TextOut(mgn, mgn, s);
}

//---------------------------------------------------------------------------
TColor __fastcall TOptionDlg::GetIdxColor(int idx)
{
	TColor col;

	switch (idx) {
	case IXCL_defFG:   col = NrmFrgImage->Canvas->Font->Color;  break;
	case IXCL_defBG:   col = NrmFrgImage->Canvas->Brush->Color; break;
	case IXCL_selFG:   col = SelFrgImage->Canvas->Font->Color;  break;
	case IXCL_selBG:   col = SelFrgImage->Canvas->Brush->Color; break;
	case IXCL_namFG:   col = PlateImage->Canvas->Font->Color;   break;
	case IXCL_namBG:   col = PlateImage->Canvas->Brush->Color;  break;
	case IXCL_Border:  col = NrmFrgImage->Canvas->Pen->Color;   break;
	case IXCL_selRct:  col = colbuf_selRect;					break;
	case IXCL_symFG:   col = SymLabel->Font->Color;				break;
	case IXCL_backgr:  col = SamplePanel->Color; 				break;
	case IXCL_Title:   col = colbuf_Title;						break;
	case IXCL_FrgLine: col = colbuf_FrgLine;					break;
	case IXCL_GrpFrm:  col = GrpFrmShape->Pen->Color;		    break;
	case IXCL_GrpLine: col = colbuf_GrpLine;				    break;
	case IXCL_LinLbl:  col = colbuf_LinLbl;					    break;
	case IXCL_glstFG:  col = GrpCheckListBox->Font->Color;	    break;
	case IXCL_glstOff: col = colbuf_glstOff;				    break;
	case IXCL_glstBG:  col = GrpCheckListBox->Color; 		    break;
	case IXCL_MapBG:   col = colbuf_MapBG;						break;
	case IXCL_AreaFrm: col = colbuf_AreaFrm;				    break;
	case IXCL_DivLine: col = colbuf_DivLine;				    break;
	case IXCL_PosGrid: col = colbuf_PosGrid;				    break;
	case IXCL_xlstFG:  col = colbuf_xlstFG;					    break;
	case IXCL_xlstBG:  col = colbuf_xlstBG;					    break;
	case IXCL_OptFind: col = colbuf_OptFind;				    break;
	default:		   col = clBlack;
	}

	return col;
}

//---------------------------------------------------------------------------
//サンプルを描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::TabSheet1Show(TObject *Sender)
{
	TPoint p[5];
	TImage *img = NrmFrgImage;
	p[0] = Point(img->ClientRect.Left,		img->ClientRect.Top);
	p[1] = Point(img->ClientRect.Right - 1,	img->ClientRect.Top);
	p[2] = Point(img->ClientRect.Right - 1,	img->ClientRect.Bottom - 1);
	p[3] = Point(img->ClientRect.Left, 		img->ClientRect.Bottom - 1);
	p[4] = p[0];
	img->Canvas->FillRect(img->ClientRect);
	img->Canvas->Polygon(p, 4);
	FrgTextOut(img, "断片");

	img = SelFrgImage;
	img->Canvas->Pen->Color = colbuf_selRect;
	img->Canvas->FillRect(img->ClientRect);
	img->Canvas->Polygon(p, 4);
	FrgTextOut(img, "選択中");

	img = PlateImage;
	int fh = get_FontHeight(img->Canvas) + FrgMargin*2 + 2;
	if (img->Canvas->Font->Name[1]=='@') {
		img->Height = NrmFrgImage->Width;
		img->Width	= fh;
	}
	else {
		img->Width	= NrmFrgImage->Width;
		img->Height = fh;
	}

	p[0] = Point(img->ClientRect.Left,	 	img->ClientRect.Top);
	p[1] = Point(img->ClientRect.Right - 1,	img->ClientRect.Top);
	p[2] = Point(img->ClientRect.Right - 1,	img->ClientRect.Bottom - 1);
	p[3] = Point(img->ClientRect.Left, 		img->ClientRect.Bottom - 1);
	p[4] = p[0];
	img->Canvas->FillRect(img->ClientRect);
	img->Canvas->Polygon(p, 4);
	FrgTextOut(img, "表札");

	GLinePBox->Repaint();
	FLinePBox->Repaint();
	GrpCheckListBox->Repaint();
	FontComboBoxChange(FontComboBox);
}
//---------------------------------------------------------------------------
//グループ関係線のサンプルを描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::GLinePBoxPaint(TObject *Sender)
{
	GLinePBox->Canvas->Font->Assign(LinLblFont);
	SampleGLine.draw(GLinePBox->Canvas, 0, colbuf_GrpLine, colbuf_LinLbl);
}
//---------------------------------------------------------------------------
//断片関係線のサンプルを描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FLinePBoxPaint(TObject *Sender)
{
	FLinePBox->Canvas->Font->Assign(LinLblFont);
	SampleFLine.draw(FLinePBox->Canvas, 0, colbuf_FrgLine, colbuf_LinLbl);
}

//---------------------------------------------------------------------------
//サンプルのグループリスト項目を描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::GrpCheckListBoxDrawItem(TWinControl *Control,
	int Index, TRect &Rect, TOwnerDrawState State)
{
	TCanvas *cv = ((TListBox*)Control)->Canvas;
	cv->Brush->Color = GrpCheckListBox->Color;
	cv->FrameRect(Rect);

	//チェックボックス
	TRect ckrc = Rect;
	int w = (abs(cv->Font->Height)<14)? 12 : 14;
	ckrc.Left += 2; ckrc.Right = ckrc.Left + w;
	InflateRect(&ckrc, 0, (w - (ckrc.Bottom - ckrc.Top))/2);
	if (Index<2) {
		::DrawFrameControl(cv->Handle, &ckrc, DFC_BUTTON, DFCS_BUTTONCHECK|DFCS_CHECKED);
		cv->Brush->Color = clBtnShadow;
		cv->FrameRect(ckrc);
	}
	else {
		cv->Pen->Color   = clBtnShadow;
		cv->Brush->Color = GrpCheckListBox->Color;
		cv->Rectangle(ckrc.Left, ckrc.Top, ckrc.Right, ckrc.Bottom);
	}
	//テキスト
	if (State.Contains(odSelected)) {
		cv->Brush->Color = clHighlight;
		cv->Font->Color  = clHighlightText;
	}
	else {
		cv->Brush->Color = GrpCheckListBox->Color;
		cv->Font->Color = (Index<2)? GrpCheckListBox->Font->Color : colbuf_glstOff;
	}
	TRect rc = Rect;
	rc.Left += (w + 4);
	cv->FillRect(rc);
	cv->TextOut(rc.Left + 14, rc.Top, ((TListBox*)Control)->Items->Strings[Index]);
	//グループ色を表示
	if (Index<2) {
		TRect grc = rc;
		grc.Right = grc.Left + 6;
		grc.Top++; grc.Bottom--;
		cv->Brush->Color = GrpFrmShape->Pen->Color;
		cv->FillRect(grc);
		grc.Left += 6;
		grc.Right = grc.Left + 6;
		cv->Brush->Color = PlateImage->Canvas->Brush->Color;
		cv->FillRect(grc);
	}
	//フォーカス描画のために背景を戻しておく
	::SetBkColor(cv->Handle, ColorToRGB(GrpCheckListBox->Color));
}

//---------------------------------------------------------------------------
//カラー項目の描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ColListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas *cv = lp->Canvas;
	int yp = Rect.Top;
	if (cv->TextHeight("Q")==abs(cv->Font->Height)) yp += 2;

	cv->Brush->Color = State.Contains(odSelected)? clHighlight : clWindow;
	cv->FillRect(Rect);

	TRect rc = Rect;  rc.Right = rc.Left + 30;
	TColor col = GetIdxColor(Index);
	if (col!=clNone) {
		cv->Brush->Color = col;
		cv->FillRect(rc);
	}
	else {
		cv->Brush->Color = clBtnFace;
		cv->FillRect(rc);
		cv->Font->Color = clWindowText;
		cv->TextOut(Rect.Left + 2, yp, "無効");
	}

	cv->Brush->Color = State.Contains(odSelected)? clHighlight : clWindow;
	cv->Font->Color  = State.Contains(odSelected)? clHighlightText : clWindowText;
	cv->TextOut(Rect.Left + 34, yp, lp->Items->Strings[Index]);
}

//---------------------------------------------------------------------------
//フォントの変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FontComboBoxChange(TObject *Sender)
{
	FontButton->Enabled = (FontComboBox->ItemIndex>=0);
}
//---------------------------------------------------------------------------
//フォント項目の描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FontComboBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TComboBox *cp = (TComboBox*)Control;
	TCanvas *cv = cp->Canvas;
	cv->Font->Assign(cp->Font);

	cv->Brush->Color = clWindow;
	cv->FillRect(Rect);

	cv->Font->Color = clWindowText;
	int xp = Rect.Left + 2;
	int yp = Rect.Top;  if (cv->TextHeight("Q")==abs(cv->Font->Height)) yp += 2;
	cv->TextOut(xp, yp, cp->Items->Strings[Index]);
	xp += (cv->TextWidth("口")*8);

	TFont *f = NULL;
	switch (Index) {
	case IXFNT_frg:  f = NrmFrgImage->Canvas->Font; break;
	case IXFNT_nam:  f = PlateImage->Canvas->Font;  break;
	case IXFNT_sym:  f = SymLabel->Font; 		    break;
	case IXFNT_lbl:  f = LinLblFont;			    break;
	case IXFNT_tit:  f = TitleFont;				    break;
	case IXFNT_glst: f = GrpCheckListBox->Font;	    break;
	case IXFNT_flst: f = FrgLstFont;			    break;
	case IXFNT_xlst: f = ExLstFont;				    break;
	case IXFNT_sbar: f = StatusBar1->Font;		    break;
	case IXFNT_dlg:  f = DialogFont;			    break;
	}

	if (f) {
		//サイズ
		cv->TextOut(xp, yp, f->Size);
		xp += (cv->TextWidth("口")*2);
		//名前
		cv->Font->Assign(f);
		cv->Font->Size	= cp->Font->Size;
		cv->Font->Color = cp->Font->Color;
		int yp = Rect.Top;  if (cv->TextHeight("Q")==abs(cv->Font->Height)) yp += 2;
		cv->TextOut(xp, yp, f->Name);
	}
}

//---------------------------------------------------------------------------
//サンプルのクリックで色・フォントのコンボボックスを選択
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SamplePanelMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TPoint p = Point(X, Y);
	TRect  rc;

	do {
		//断片
		rc = NrmFrgImage->BoundsRect;
		if (pt_on_border(X, Y, rc, 4)) {
			FontComboBox->ItemIndex = IXFNT_frg;
			ColListBox->ItemIndex	= IXCL_Border;	//断片の輪郭
			break;
		}
		else if (rc.PtInRect(p)) {
			FontComboBox->ItemIndex = IXFNT_frg;
			//文字の上か?
			rc.right  = rc.left + NrmFrgImage->Canvas->TextWidth("断片");
			rc.bottom = rc.top  + NrmFrgImage->Canvas->TextHeight(" ");
			ColListBox->ItemIndex = (rc.PtInRect(p)? IXCL_defFG : IXCL_defBG);
			break;
		}
		//選択中断片
		rc = SelFrgImage->BoundsRect;
		if (pt_on_border(X, Y, (TRect)rc, 4)) {
			FontComboBox->ItemIndex = IXFNT_frg;
			ColListBox->ItemIndex	= IXCL_selRct;	//注目断片の輪郭
			break;
		}
		else if (rc.PtInRect(p)) {
			FontComboBox->ItemIndex = IXFNT_frg;
			//文字の上か?
			rc.right  = rc.left + SelFrgImage->Canvas->TextWidth("選択中");
			rc.bottom = rc.top  + SelFrgImage->Canvas->TextHeight(" ");
			ColListBox->ItemIndex = (rc.PtInRect(p)? IXCL_selFG : IXCL_selBG);
			break;
		}
		//表札
		rc = PlateImage->BoundsRect;
		if (pt_on_border(X, Y, (TRect)rc, 4)) {
			FontComboBox->ItemIndex = IXFNT_nam;
			ColListBox->ItemIndex	= IXCL_Border;	//断片の輪郭
			break;
		}
		else if (rc.PtInRect(p)) {
			FontComboBox->ItemIndex = IXFNT_nam;
			//文字の上か?
			rc.right  = rc.left + PlateImage->Canvas->TextWidth("表札");
			rc.bottom = rc.top  + PlateImage->Canvas->TextHeight(" ");
			ColListBox->ItemIndex = (rc.PtInRect(p)? IXCL_namFG : IXCL_namBG);
			break;
		}

		//グループ枠
		rc = GrpFrmShape->BoundsRect;
		if (pt_on_border(X, Y, rc, 8)) {
			ColListBox->ItemIndex	 = IXCL_GrpFrm;
			FontComboBox->ItemIndex  = -1;
			SizSpComboBox->ItemIndex = pt_on_corner(X, Y, rc, 16)? IXSZ_GfrmRnd : IXSZ_GfrmLwd;
			SizSpComboBoxChange(SizSpComboBox);
			break;
		}

		ColListBox->ItemIndex	= IXCL_backgr;
		FontComboBox->ItemIndex = -1;
	} while (false);

	FontComboBoxChange(FontComboBox);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SymLabelClick(TObject *Sender)
{
	FontComboBox->ItemIndex = IXFNT_sym;
	ColListBox->ItemIndex	= IXCL_symFG;
	FontComboBoxChange(FontComboBox);
}
//---------------------------------------------------------------------------
//サンプルのグループ関係線をクリック
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::GLinePBoxMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (SampleGLine.pt_in_line(X, Y))
		ColListBox->ItemIndex = IXCL_GrpLine;
	else
		ColListBox->ItemIndex = IXCL_LinLbl;
	FontComboBox->ItemIndex = IXFNT_lbl;
	FontComboBoxChange(FontComboBox);
}
//---------------------------------------------------------------------------
//サンプルの断片関係線をクリック
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FLinePBoxMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (SampleFLine.pt_in_line(X, Y))
		ColListBox->ItemIndex = IXCL_FrgLine;
	else
		ColListBox->ItemIndex = IXCL_LinLbl;
	FontComboBox->ItemIndex = IXFNT_lbl;
	FontComboBoxChange(FontComboBox);
}

//---------------------------------------------------------------------------
//サンプルのグループリストをクリック
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::GrpCheckListBoxClick(TObject *Sender)
{
 	FontComboBox->ItemIndex = IXFNT_glst;
	ColListBox->ItemIndex	= (GrpCheckListBox->ItemIndex<2)? IXCL_glstFG : IXCL_glstOff;
	FontComboBoxChange(FontComboBox);
	SizSpComboBox->ItemIndex = IXSZ_GlIntLn;
	SizSpComboBoxChange(SizSpComboBox);
}
//---------------------------------------------------------------------------
//サンプルのステータスバーをクリック
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::StatusBar1Click(TObject *Sender)
{
 	FontComboBox->ItemIndex = IXFNT_sbar;
	FontComboBoxChange(FontComboBox);
}

//---------------------------------------------------------------------------
//配色の設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ColButtonClick(TObject *Sender)
{
	int idx = ColListBox->ItemIndex;
	if (idx>=0) {
		TColor colbuf = GetIdxColor(idx);
		ColorDialog1->Color = colbuf;
		EV->set_custom_col(ColorDialog1->CustomColors);
		if (ColorDialog1->Execute()) {
			colbuf = ColorDialog1->Color;
			EV->get_custom_col(ColorDialog1->CustomColors);
			switch (idx) {
			case IXCL_defFG: NrmFrgImage->Canvas->Font->Color  = colbuf; break;
			case IXCL_defBG: NrmFrgImage->Canvas->Brush->Color = colbuf; break;
			case IXCL_selFG: SelFrgImage->Canvas->Font->Color  = colbuf; break;
			case IXCL_selBG: SelFrgImage->Canvas->Brush->Color = colbuf; break;
			case IXCL_namFG: PlateImage->Canvas->Font->Color   = colbuf; break;
			case IXCL_namBG: PlateImage->Canvas->Brush->Color  = colbuf; break;
			case IXCL_Border:
				NrmFrgImage->Canvas->Pen->Color = colbuf;
				SelFrgImage->Canvas->Pen->Color = colbuf;
				PlateImage->Canvas->Pen->Color	= colbuf;
				break;
			case IXCL_symFG:   SymLabel->Font->Color		= colbuf; break;
			case IXCL_backgr:  SamplePanel->Color			= colbuf; break;
			case IXCL_Title:   colbuf_Title 				= colbuf; break;
			case IXCL_selRct:  colbuf_selRect				= colbuf; break;
			case IXCL_GrpFrm:  GrpFrmShape->Pen->Color		= colbuf; break;
			case IXCL_GrpLine: colbuf_GrpLine				= colbuf; break;
			case IXCL_FrgLine: colbuf_FrgLine				= colbuf; break;
			case IXCL_LinLbl:  colbuf_LinLbl				= colbuf; break;
			case IXCL_glstFG:  GrpCheckListBox->Font->Color = colbuf; break;
			case IXCL_glstOff: colbuf_glstOff				= colbuf; break;
			case IXCL_glstBG:  GrpCheckListBox->Color		= colbuf; break;
			case IXCL_MapBG:   colbuf_MapBG					= colbuf; break;
			case IXCL_AreaFrm: colbuf_AreaFrm				= colbuf; break;
			case IXCL_DivLine: colbuf_DivLine				= colbuf; break;
			case IXCL_PosGrid: colbuf_PosGrid				= colbuf; break;
			case IXCL_xlstFG:  colbuf_xlstFG				= colbuf; break;
			case IXCL_xlstBG:  colbuf_xlstBG				= colbuf; break;
			case IXCL_OptFind: colbuf_OptFind				= colbuf; break;
			}
			ColListBox->Repaint();
		}
	}
	TabSheet1Show(NULL);
}
//---------------------------------------------------------------------------
//フォントの設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FontButtonClick(TObject *Sender)
{
	int idx = FontComboBox->ItemIndex;
	int fh;
	if (idx>=0) {
		switch (idx) {
		case IXFNT_frg:  FontDialog1->Font->Assign(NrmFrgImage->Canvas->Font); break;
		case IXFNT_nam:  FontDialog1->Font->Assign(PlateImage->Canvas->Font);  break;
		case IXFNT_sym:  FontDialog1->Font->Assign(SymLabel->Font); 		   break;
		case IXFNT_lbl:  FontDialog1->Font->Assign(LinLblFont); 			   break;
		case IXFNT_tit:  FontDialog1->Font->Assign(TitleFont);				   break;
		case IXFNT_glst: FontDialog1->Font->Assign(GrpCheckListBox->Font);	   break;
		case IXFNT_flst: FontDialog1->Font->Assign(FrgLstFont); 			   break;
		case IXFNT_xlst: FontDialog1->Font->Assign(ExLstFont);				   break;
		case IXFNT_sbar: FontDialog1->Font->Assign(StatusBar1->Font);		   break;
		case IXFNT_dlg:  FontDialog1->Font->Assign(DialogFont);				   break;
		}

		if (idx==IXFNT_glst)
			FontDialog1->Options << fdFixedPitchOnly;	//グループリストは固定ピッチ
		else
			FontDialog1->Options >> fdFixedPitchOnly;

		if (FontDialog1->Execute()) {
			switch (idx) {
			case IXFNT_frg:
				{
					NrmFrgImage->Canvas->Font->Assign(FontDialog1->Font);
					TColor org_col = SelFrgImage->Canvas->Font->Color;
					SelFrgImage->Canvas->Font->Assign(FontDialog1->Font);
					SelFrgImage->Canvas->Font->Color = org_col;
				}
				break;
			case IXFNT_nam:
				PlateImage->Canvas->Font->Assign(FontDialog1->Font); break;
			case IXFNT_sym:
				SymLabel->Font->Assign(FontDialog1->Font); break;
			case IXFNT_lbl:
				LinLblFont->Assign(FontDialog1->Font); break;
			case IXFNT_tit:
				TitleFont->Assign(FontDialog1->Font); break;
			case IXFNT_glst:
				GrpCheckListBox->Font->Assign(FontDialog1->Font);
				GrpCheckListBox->ItemHeight = get_FontHeightP(GrpCheckListBox->Font, GLstInterLine);
				break;
			case IXFNT_flst:
				FrgLstFont->Assign(FontDialog1->Font); break;
			case IXFNT_xlst:
				ExLstFont->Assign(FontDialog1->Font); break;
			case IXFNT_sbar:
				StatusBar1->Font->Assign(FontDialog1->Font);
				StatusBar1->Canvas->Font->Assign(EV->stbarFont);
				StatusBar1->ClientHeight = StatusBar1->Canvas->TextHeight(" ") + 4;
				break;
			case IXFNT_dlg:
				DialogFont->Assign(FontDialog1->Font);
				if (DialogFont->Size>10) DialogFont->Size = 10;
				Application->DefaultFont->Assign(DialogFont);
				break;
			}
			FontComboBox->Repaint();
		}
	}
	TabSheet1Show(NULL);
}

//---------------------------------------------------------------------------
//グループ関係線の矢のサイズを変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AcceptCrCheckBoxClick(TObject *Sender)
{
	CrRetKeyCheckBox->Enabled = AcceptCrCheckBox->Checked;
	CrStrEdit->Enabled		  = AcceptCrCheckBox->Checked;
}
//---------------------------------------------------------------------------
//メニューの親項目を選択
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ParentComboClick(TObject *Sender)
{
	bool flag = false;
	for (int i=0; i<IdeaFragMainForm->ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)IdeaFragMainForm->ActionList1->Actions[i];
		if (ap->Category == ParentCombo->Items->Strings[ParentCombo->ItemIndex]) {
			ShortCutList->TopIndex	= i;
			ShortCutList->ItemIndex = i;
			flag = true;
			break;
		}
	}
	if (!flag) {
		for (int i=0; i<FrgPropDlg->ActionList1->ActionCount; i++) {
			TAction *ap = (TAction*)FrgPropDlg->ActionList1->Actions[i];
			if (ap->Category == ParentCombo->Items->Strings[ParentCombo->ItemIndex]) {
				ShortCutList->TopIndex	= i + IdeaFragMainForm->ActionList1->ActionCount;
				ShortCutList->ItemIndex = i + IdeaFragMainForm->ActionList1->ActionCount;
				break;
			}
		}
	}
}

//---------------------------------------------------------------------------
//ショートカットキー一覧の表が
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ShortCutListDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	cv->Brush->Color = State.Contains(odSelected)? clHighlight : clWindow;
	cv->Font->Color  = State.Contains(odSelected)? clHighlightText : clWindowText;
	cv->FillRect(Rect);

	TStringDynArray i_buf = SplitString(lp->Items->Strings[Index], "\t");
	if (i_buf.Length==3) {
		int xp = Rect.Left + 2;
		int yp = Rect.Top + 1;
		//アイコン
		int idx = i_buf[2].ToIntDef(-1);
		if (idx!=-1) IdeaFragMainForm->BtnImgList->Draw(cv, xp, yp - 1, idx);
		//名前
		xp += ScaledInt(24);
		cv->TextOut(xp, yp, i_buf[0]);
		//キー
		xp += (LOWORD(::GetDialogBaseUnits()) * 30);
		cv->TextOut(xp, yp, i_buf[1]);
	}

	//カテゴリー区切り線
	if ((int)lp->Items->Objects[Index]) {
		cv->Pen->Color = clGrayText;
		cv->Pen->Style = psSolid;
		cv->Pen->Width = 1;
		cv->MoveTo(Rect.Left  + 2, Rect.Top);
		cv->LineTo(Rect.Right - 2, Rect.Top);
	}
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ShortCutListClick(TObject *Sender)
{
	int idx = ShortCutList->ItemIndex;
	if (idx!=-1) {
		int idxn0 = IdeaFragMainForm->ActionList1->ActionCount;
		TAction *ap = (idx<idxn0)? 
			(TAction*)IdeaFragMainForm->ActionList1->Actions[idx] : 
			(TAction*)FrgPropDlg->ActionList1->Actions[idx - idxn0];
		ParentCombo->ItemIndex = ParentCombo->Items->IndexOf(ap->Category);

		UnicodeString sk_str = ShortCutToText(ap->ShortCut);
		ShiftCheckBox->Checked = ContainsText(sk_str, "Shift+");
		sk_str = ReplaceStr(sk_str, "Shift+", EmptyStr);
		CtrlCheckBox->Checked  = ContainsText(sk_str, "Ctrl+");
		sk_str = ReplaceStr(sk_str, "Ctrl+", EmptyStr);
		AltCheckBox->Checked   = ContainsText(sk_str, "Alt+");
		sk_str = ReplaceStr(sk_str, "Alt+", EmptyStr);
		KeyComboBox->ItemIndex = KeyComboBox->Items->IndexOf(sk_str);
	}
}
//---------------------------------------------------------------------------
//ショートカットキー変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SKChgBtnClick(TObject *Sender)
{
	int idx = ShortCutList->ItemIndex;
	if (idx!=-1) {
		TStringDynArray i_buf = SplitString(ShortCutList->Items->Strings[idx], "\t");
		i_buf.Length = 3;

		UnicodeString sk_str;
		if (ShiftCheckBox->Checked) sk_str.UCAT_TSTR("Shift+");
		if (CtrlCheckBox->Checked)  sk_str.UCAT_TSTR("Ctrl+");
		if (AltCheckBox->Checked)   sk_str.UCAT_TSTR("Alt+");
		sk_str += KeyComboBox->Text;

		//重複チェック
		try {
			TActionList *aclst = IdeaFragMainForm->ActionList1;
			for (int i=0; i<aclst->ActionCount; i++) {
				TAction *ap = (TAction*)aclst->Actions[i];
				if (i==idx) continue;
				if (ap->ShortCut == TextToShortCut(sk_str)) Abort();
			}
			aclst = FrgPropDlg->ActionList1;
			for (int i=0; i<aclst->ActionCount; i++) {
				TAction *ap = (TAction*)aclst->Actions[i];
				if (i==idx) continue;
				if (ap->ShortCut == TextToShortCut(sk_str)) Abort();
			}

			ShortCutList->Items->Strings[idx]
				= UnicodeString().sprintf(_T("%s\t%s\t%s"), i_buf[0].c_str(), sk_str.c_str(), i_buf[2].c_str());

			int idxn0 = IdeaFragMainForm->ActionList1->ActionCount;
			TAction *ap = (idx<idxn0)?
				(TAction*)IdeaFragMainForm->ActionList1->Actions[idx] :
				(TAction*)FrgPropDlg->ActionList1->Actions[idx - idxn0];
			ap->ShortCut = TextToShortCut(sk_str);
		}
		catch (EAbort &e) {
			msgbox_WARN("ショートカットキーが重複しています");
		}
	}
}

//---------------------------------------------------------------------------
//ショートカットキー解除
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ClrBtnClick(TObject *Sender)
{
	int idx = ShortCutList->ItemIndex;
	if (idx!=-1) {
		TStringDynArray i_buf = SplitString(ShortCutList->Items->Strings[idx], "\t");
		i_buf.Length = 3;

		ShortCutList->Items->Strings[idx]
			= UnicodeString().sprintf(_T("%s\t\t%s"), i_buf[0].c_str(), i_buf[2].c_str());

		int idxn0 = IdeaFragMainForm->ActionList1->ActionCount;
		TAction *ap = (idx<idxn0)?
			(TAction*)IdeaFragMainForm->ActionList1->Actions[idx] :
			(TAction*)IdeaFragMainForm->ActionList1->Actions[idx - idxn0];
		ap->ShortCut = TextToShortCut(EmptyStr);

		ShiftCheckBox->Checked = false;
		CtrlCheckBox->Checked  = false;
		AltCheckBox->Checked   = false;
		KeyComboBox->ItemIndex = -1;
	}
}
//---------------------------------------------------------------------------
//ホームウィンドウの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::HomWinBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "ホームウィンドウ";
	OpenDialog1->Filter		= "データファイル(*.IFD)|*.IFD";
	OpenDialog1->DefaultExt	= "IFD";
	OpenDialog1->FileName	= "*.IFD";
	if (OpenDialog1->Execute()) {
		if (test_ideafrag2(OpenDialog1->FileName))
			HomeWinEdit->Text = OpenDialog1->FileName;
		else
			msgbox_WARN("[" + OpenDialog1->FileName + "]はデータファイルではありません");
	}
}
//---------------------------------------------------------------------------
//現在のファイルを指定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SetCurHomBtnClick(TObject *Sender)
{
	HomeWinEdit->Text = CurFileName;
}

//---------------------------------------------------------------------------
//関連づけ
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RegExtBtnClick(TObject *Sender)
{
	try {
		std::unique_ptr<TRegistry> Registry(new TRegistry());
		//既存の値を一旦削除
		Registry->RootKey = HKEY_CURRENT_USER;
		Registry->DeleteKey("\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.ifd");
		Registry->RootKey = HKEY_CLASSES_ROOT;
		if (Registry->OpenKey("\\.ifd", false)) {
			UnicodeString vstr = Registry->ReadString(EmptyStr);
			if (!vstr.IsEmpty()) Registry->DeleteKey("\\" + vstr);
		}
		Registry->DeleteKey("\\.ifd");
		Registry->DeleteKey("\\Applications\\IdeaFrg2.exe");

		//関連付け設定
		Registry->OpenKey("\\.ifd", true);
		Registry->WriteString(EmptyStr, "IF2datafile");
		//  ファイルの種類
		Registry->OpenKey("\\IF2datafile", true);
		Registry->WriteString(EmptyStr, "断片データファイル");
		//  アイコンを指定
		Registry->OpenKey("\\IF2datafile\\DefaultIcon", true);
		Registry->WriteString(EmptyStr, Application->ExeName + ",0");
		//  開くアプリケーションを指定
		Registry->OpenKey("\\IF2datafile\\shell\\open\\command", true);
		Registry->WriteString(EmptyStr, "\"" + Application->ExeName + "\" \"%1\"");

		msgbox_OK("関連付けを行いました");
	}
	catch (...) {
		msgbox_ERR("設定に失敗しました");
	}
}
//---------------------------------------------------------------------------
//関連づけを解除
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DelExtBtnClick(TObject *Sender)
{
	std::unique_ptr<TRegistry> Registry(new TRegistry());
	Registry->RootKey = HKEY_CURRENT_USER;
	Registry->DeleteKey("\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.ifd");
	Registry->RootKey = HKEY_CLASSES_ROOT;
	if (Registry->OpenKey("\\.ifd", false)) {
		UnicodeString vstr = Registry->ReadString(EmptyStr);
		if (!vstr.IsEmpty()) Registry->DeleteKey("\\" + vstr);
	}
	Registry->DeleteKey("\\.ifd");
	Registry->DeleteKey("\\Applications\\IdeaFrg2.exe");
	msgbox_OK("関連付けを解除しました");
}

//---------------------------------------------------------------------------
//Susie プラグインのフォルダ参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SpiRefBtnClick(TObject *Sender)
{
	UnicodeString dirstr = EV->SpiDir;
	if (SelectDirectory("Susie プラグインのフォルダ", EmptyStr, dirstr, TSelectDirExtOpts() << sdNewUI, this)) {
		EV->SpiDir = dirstr;
		delete SPI;
		SPI = new SpiUnit(EV->SpiDir);
		SetSpiInfo(dirstr);
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SetSpiInfo(UnicodeString spidir)
{
	SpiDirEdit->Text = spidir;
	SpiListBox->Clear();
	for (int i=0; i<SPI->PlgList->Count; i++) {
		spi_info *sp = SPI->PlgList->Items[i];
		SpiListBox->Items->Add(
			UnicodeString().sprintf(_T("%-16s %s"), sp->FileName.c_str(), sp->FileType.c_str()));
	}
	SpiListBox->Repaint();
}
//---------------------------------------------------------------------------
//Sunis プラグインの設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ConfigSpiActionExecute(TObject *Sender)
{
	int idx = SpiListBox->ItemIndex;
	if (idx!=-1) {
		spi_info *sp = SPI->PlgList->Items[idx];
		if (sp->ConfigurationDlg) sp->ConfigurationDlg(this->Handle, 1);
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ConfigSpiActionUpdate(TObject *Sender)
{
	int idx = SpiListBox->ItemIndex;
	if (idx!=-1) {
		spi_info *sp = SPI->PlgList->Items[idx];
		((TAction*)Sender)->Enabled = (sp->ConfigurationDlg!=NULL);
	}
	else {
		((TAction*)Sender)->Enabled = false;
	}
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AssociateListBoxClick(TObject *Sender)
{
	int idx = AssociateListBox->ItemIndex;  if (idx==-1) return;
	UnicodeString s = AssociateListBox->Items->Strings[idx];
	int p = s.Pos("=");  if (p==0) return;
	AsoExtEdit->Text = s.SubString(1, p - 1);
	s.Delete(1, p);
	AsoAppEdit->Text = exclude_quot(s);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AsoAddBtnClick(TObject *Sender)
{
	if (AsoExtEdit->Text.IsEmpty() || AsoAppEdit->Text.IsEmpty()) return;
	UnicodeString s = AsoExtEdit->Text.LowerCase();
	if (s[1]=='.') s.Delete(1, 1);
	s.cat_sprintf(_T("=\"%s\""), AsoAppEdit->Text.c_str());
	AssociateListBox->Items->Add(s);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AsoChgBtnClick(TObject *Sender)
{
	int idx = AssociateListBox->ItemIndex;  if (idx==-1) return;
	if (AsoExtEdit->Text.IsEmpty() || AsoAppEdit->Text.IsEmpty()) return;
	UnicodeString s = AsoExtEdit->Text.LowerCase();
	if (s[1]=='.') s.Delete(1, 1);
	s.cat_sprintf(_T("=\"%s\""), AsoAppEdit->Text.c_str());
	AssociateListBox->Items->Strings[idx] = s;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AsoDelBtnClick(TObject *Sender)
{
	int idx = AssociateListBox->ItemIndex;  if (idx==-1) return;
	AssociateListBox->Items->Delete(idx);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AsoRefBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "関連づけ";
	OpenDialog1->Filter		= "実行ファイル(*.exe *.com *.bat)|*.EXE;*.COM;*.BAT";
	OpenDialog1->DefaultExt	= EmptyStr;
	OpenDialog1->FileName	= EmptyStr;
	if (OpenDialog1->Execute()) {
		AsoAppEdit->Text = OpenDialog1->FileName;
	}
}
//---------------------------------------------------------------------------
//サイズ／形状を変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SizValComboBoxChange(TObject *Sender)
{
	int idx = SizValComboBox->ItemIndex;
	int fh;
	switch (SizSpComboBox->ItemIndex) {
	case IXSZ_GfrmLwd:
		GrpFrmShape->Pen->Width = idx + 1; break;
	case IXSZ_GfrmRnd:
		gfrmMaxRnd = idx * 10;	break;
	case IXSZ_GlIntLn:
		GLstInterLine = idx*25;
		GrpCheckListBox->ItemHeight = get_FontHeightP(GrpCheckListBox->Font, GLstInterLine);
		break;
	case IXSZ_FlIntLn:
		FLstInterLine = idx*25;	break;
	case IXSZ_ExIntLn:
		XLstInterLine = idx*25;	break;
	case IXSZ_FrgMgn:
		FrgMargin	  = idx;	break;
	}
	TabSheet1Show(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::PrintMonoCheckBoxClick(TObject *Sender)
{
	PrintBgCheckBox->Enabled = !PrintMonoCheckBox->Checked;
}

//---------------------------------------------------------------------------
//JPEG保存品質変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::JpgQTrackBarChange(TObject *Sender)
{
	JpgQLabel->Caption = "JPEG保存品質 " + IntToStr(JpgQTrackBar->Position);
}

//---------------------------------------------------------------------------
//サイズ／形状の設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SizSpComboBoxChange(TObject *Sender)
{
	SizValComboBox->Clear();
	if (SizSpComboBox->ItemIndex==-1) return;

	TStrings *ip = SizValComboBox->Items;
	int i;

	switch (SizSpComboBox->ItemIndex) {
	case IXSZ_GfrmLwd:
		for (i=1; i<=10; i++) ip->Add(IntToStr(i));
		break;
	case IXSZ_GfrmRnd:
		for (i=0; i<=12; i++) ip->Add(IntToStr(i*10));
		break;
	case IXSZ_GlIntLn:  case IXSZ_FlIntLn: case IXSZ_ExIntLn:
		ip->Add("0%");
		ip->Add("25%");
		ip->Add("50%");
		ip->Add("75%");
		ip->Add("100%");
		break;
	case IXSZ_FrgMgn:
		for (i=0; i<=20; i++) ip->Add(IntToStr(i));
		break;
	}

	int idx = 0;
	switch (SizSpComboBox->ItemIndex) {
	case IXSZ_GfrmLwd:	idx = GrpFrmShape->Pen->Width - 1;	break;
	case IXSZ_GfrmRnd:	idx = gfrmMaxRnd/10;	break;
	case IXSZ_GlIntLn:	idx = GLstInterLine/25;	break;
	case IXSZ_FlIntLn:	idx = FLstInterLine/25;	break;
	case IXSZ_ExIntLn:	idx = XLstInterLine/25;	break;
	case IXSZ_FrgMgn:	idx = FrgMargin;		break;
	}

	if (idx<0 || idx>=ip->Count) idx = 1;
	SizValComboBox->ItemIndex = idx;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DefWdEditChange(TObject *Sender)
{
	DefWdEdit->Font->Color = check_TUpDown(WdUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DefHiEditChange(TObject *Sender)
{
	DefHiEdit->Font->Color = check_TUpDown(HiUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::NamWdEditChange(TObject *Sender)
{
	NamWdEdit->Font->Color = check_TUpDown(NpWdUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::NamHiEditChange(TObject *Sender)
{
	NamHiEdit->Font->Color = check_TUpDown(NpHiUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ImgR1EditChange(TObject *Sender)
{
	ImgR1Edit->Font->Color = check_TUpDown(ImgR1UpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ImgR2EditChange(TObject *Sender)
{
	ImgR2Edit->Font->Color = check_TUpDown(ImgR2UpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ScrWdEditChange(TObject *Sender)
{
	ScrWdEdit->Font->Color = check_TUpDown(ScrWdUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ScrHiEditChange(TObject *Sender)
{
	ScrHiEdit->Font->Color = check_TUpDown(ScrHiUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ArngPtEditChange(TObject *Sender)
{
	ArngPtEdit->Font->Color = check_TUpDown(ArngPtUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::GridSzEditChange(TObject *Sender)
{
	GridSzEdit->Font->Color = check_TUpDown(GridSzUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::TrmMgnEditChange(TObject *Sender)
{
	TrmMgnEdit->Font->Color = check_TUpDown(TrmMgnUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::TrmWdEditChange(TObject *Sender)
{
	TrmWdEdit->Font->Color = check_TUpDown(TrmWdUpDown)? clWindowText : clRed;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::TrmHiEditChange(TObject *Sender)
{
	TrmHiEdit->Font->Color = check_TUpDown(TrmHiUpDown)? clWindowText : clRed;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::NumberEditExit(TObject *Sender)
{
	TLabeledEdit *ep = (TLabeledEdit*)Sender;
	if (ep->Font->Color == clRed) {
		msgbox_WARN("入力した数値[" + ep->Text + " ]は範囲を超えています");
		ep->SetFocus();
	}
}

//---------------------------------------------------------------------------
//エクスポート項目の追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddExpItemActionExecute(TObject *Sender)
{
	int i = 0;
	while (i<ExpItemListBox1->Items->Count) {
		if (ExpItemListBox1->Selected[i]) {
			ExpItemListBox2->Items->Add(ExpItemListBox1->Items->Strings[i]);
			ExpItemListBox1->Items->Delete(i);
		}
		else {
			i++;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddExpItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (ExpItemListBox1->SelCount>0);
}
//---------------------------------------------------------------------------
//エクスポート項目の解除
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DelExpItemActionExecute(TObject *Sender)
{
	int i = 0;
	while (i<ExpItemListBox2->Items->Count) {
		if (ExpItemListBox2->Selected[i]) {
			ExpItemListBox1->Items->Add(ExpItemListBox2->Items->Strings[i]);
			ExpItemListBox2->Items->Delete(i);
		}
		else {
			i++;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DelExpItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (ExpItemListBox2->SelCount>0);
}

//---------------------------------------------------------------------------
//エクスポート項目を上へ
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::UpExpItemActionExecute(TObject *Sender)
{
	int idx = ExpItemListBox2->ItemIndex;
	if (idx>0) {
		ExpItemListBox2->Items->Move(idx, idx - 1);
		idx--;
		ExpItemListBox2->Selected[idx] = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::UpExpItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (ExpItemListBox2->ItemIndex>0 && ExpItemListBox2->SelCount==1);
}
//---------------------------------------------------------------------------
//エクスポート項目を下へ
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DowExpItemActionExecute(TObject *Sender)
{
	int idx = ExpItemListBox2->ItemIndex;
	if (idx>=0 && idx<ExpItemListBox2->Items->Count-1) {
		ExpItemListBox2->Items->Move(idx, idx + 1);
		idx++;
		ExpItemListBox2->Selected[idx] = true;
	}
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DowExpItemActionUpdate(TObject *Sender)
{
	int idx = ExpItemListBox2->ItemIndex;
	((TAction*)Sender)->Enabled
		= (idx>=0 && idx < ExpItemListBox2->Items->Count-1 && ExpItemListBox2->SelCount==1);
}

//---------------------------------------------------------------------------
//タブの描画 (検索結果の強調表示)
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::PageControl1DrawTab(TCustomTabControl *Control, int TabIndex,
	const TRect &Rect, bool Active)
{
	TTabControl *tp = (TTabControl*)Control;

	//背景
	TCanvas *cv = tp->Canvas;
	cv->Brush->Color = (PageControl1->Pages[TabIndex]->Tag==0)? clBtnFace : EV->col_OptFind;
	cv->FillRect(Rect);
	//タイトル
	cv->Font->Color = clWindowText;
	UnicodeString tstr = tp->Tabs->Strings[TabIndex];
	int xp = Rect.Left + (Rect.Width() - cv->TextWidth(tstr))/2;
	cv->TextOut(xp, Rect.Top + 4, tstr);
}

//---------------------------------------------------------------------------
//検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindEditChange(TObject *Sender)
{
	TPageControl *pp = PageControl1;
	for (int i=0; i<pp->PageCount; i++) pp->Pages[i]->Tag = 0;
	pp->Repaint();
	FindMarkList->ClearAll();
	FindMarkList->MarkColor = EV->col_OptFind;

	UnicodeString wd = FindEdit->Text;
	if (!wd.IsEmpty()) {
		//タブシート毎に検索
		TPageControl *pp = PageControl1;
		for (int i=0; i<pp->PageCount; i++) {
			TTabSheet *sp = pp->Pages[i];
			sp->Tag = (FindMarkList->SearchControl((TWinControl*)sp, wd)>0 || pos_i(wd, sp->Caption)>0)? 1 : 0;
				//見つかったら Tag=1 に設定
		}
		FindMarkList->ShowMark();
		PageControl1->Repaint();
		//見つかった項目を含むタブをアクティブに
		for (int i=0; i<pp->PageCount; i++) {
			if (pp->Pages[i]->Tag==1) {
				pp->ActivePage = pp->Pages[i];
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindBtnClick(TObject *Sender)
{
	TPageControl *pp = PageControl1;
	for (int i=0; i<pp->PageCount; i++) pp->Pages[i]->Tag = 0;
	pp->Repaint();

	FindMarkList->ClearAll();
	FindMarkList->MarkColor = EV->col_OptFind;

	UnicodeString wd = Trim(InputBox("設定項目の検索","検索語",EmptyStr));
	if (!wd.IsEmpty()) {
		//タブシート毎に検索
		TPageControl *pp = PageControl1;
		for (int i=0; i<pp->PageCount; i++) {
			TTabSheet *sp = pp->Pages[i];
			sp->Tag = (FindMarkList->SearchControl((TWinControl*)sp, wd)>0 || pos_i(wd, sp->Caption)>0)? 1 : 0;
				//見つかったら Tag=1 に設定
		}
		FindMarkList->ShowMark();
		PageControl1->Repaint();
		//見つかった項目を含むタブをアクティブに
		for (int i=0; i<pp->PageCount; i++) {
			if (pp->Pages[i]->Tag==1) {
				pp->ActivePage = pp->Pages[i];
				break;
			}
		}
	}
}

//---------------------------------------------------------------------------
// OK
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::OkActionExecute(TObject *Sender)
{
	if (!Visible) return;	//おまじない?

	EV->frgFont->Assign(NrmFrgImage->Canvas->Font);
	EV->col_defFG	  = NrmFrgImage->Canvas->Font->Color;
	EV->col_defBG	  = NrmFrgImage->Canvas->Brush->Color;
	EV->col_selFG	  = SelFrgImage->Canvas->Font->Color;
	EV->col_selBG	  = SelFrgImage->Canvas->Brush->Color;
	EV->namFont->Assign(PlateImage->Canvas->Font);
	EV->col_namFG	  = PlateImage->Canvas->Font->Color;
	EV->col_namBG	  = PlateImage->Canvas->Brush->Color;
	EV->col_Border	  = NrmFrgImage->Canvas->Pen->Color;
	EV->symFont->Assign(SymLabel->Font);
	EV->col_symFG	  = SymLabel->Font->Color;
	EV->col_backgr	  = SamplePanel->Color;
	EV->col_selRct	  = colbuf_selRect;
	EV->col_GrpFrm	  = GrpFrmShape->Pen->Color;
	EV->col_GrpLine   = colbuf_GrpLine;
	EV->col_FrgLine   = colbuf_FrgLine;
	EV->col_LinLbl	  = colbuf_LinLbl;
	EV->col_Title	  = colbuf_Title;
	EV->col_DivLine   = colbuf_DivLine;
	EV->col_PosGrid   = colbuf_PosGrid;

	EV->titFont->Assign(TitleFont);
	EV->lblFont->Assign(LinLblFont);
	EV->flstFont->Assign(FrgLstFont);
	EV->xlstFont->Assign(ExLstFont);
	EV->dialogFont->Assign(DialogFont);
	EV->stbarFont->Assign(StatusBar1->Font);
	EV->gfrmLineWidth = GrpFrmShape->Pen->Width;

	EV->gfrmMaxRnd	  = gfrmMaxRnd;
	EV->GLstInterLine = GLstInterLine;
	EV->FLstInterLine = FLstInterLine;
	EV->XLstInterLine = XLstInterLine;
	EV->frg_mgn 	  = FrgMargin;

	if (check_TUpDown(WdUpDown))    EV->def_wd = DefWdEdit->Text.ToIntDef(EV->def_wd);
	if (check_TUpDown(HiUpDown))    EV->def_hi = DefHiEdit->Text.ToIntDef(EV->def_hi);
	if (check_TUpDown(NpWdUpDown))  EV->nam_wd = NamWdEdit->Text.ToIntDef(EV->nam_wd);
	if (check_TUpDown(NpHiUpDown))  EV->nam_hi = NamHiEdit->Text.ToIntDef(EV->nam_hi);
	if (check_TUpDown(ImgR1UpDown)) EV->img_r1 = ImgR1Edit->Text.ToIntDef(EV->img_r1);
	if (check_TUpDown(ImgR2UpDown)) EV->img_r2 = ImgR2Edit->Text.ToIntDef(EV->img_r2);

	EV->glstFont->Assign(GrpCheckListBox->Font);
	EV->col_glstFG	   = GrpCheckListBox->Font->Color;
	EV->col_glstBG	   = GrpCheckListBox->Color;
	EV->col_AreaFrm    = colbuf_AreaFrm;
	EV->col_MapBG	   = colbuf_MapBG;
	EV->col_glstOff    = colbuf_glstOff;
	EV->col_xlstFG	   = colbuf_xlstFG;
	EV->col_xlstBG	   = colbuf_xlstBG;
	EV->col_OptFind    = colbuf_OptFind;

	EV->new_frg_pos    = NewPosCombo->ItemIndex + 1;
	EV->away_frg_pos   = AwayPosCombo->ItemIndex;
	EV->new_no_group   = NoGrpCheckBox->Checked;
	EV->AlwaysShowNp   = AlwaysShowCheckBox->Checked;
	EV->auto_exp	   = AutoExpCheckBox->Checked;
	EV->input_wordwrap = WdWrapCheckBox->Checked;
	EV->ime_on		   = ImeOnCheckBox->Checked;
	FrgPropDlg->Memo1->ImeMode = (EV->ime_on? imOpen : imDontCare);
	EV->accept_cr	   = AcceptCrCheckBox->Checked;
	EV->cr_returnkey   = CrRetKeyCheckBox->Checked;
	EV->ok_ctrl_return = CtlRetOkCheckBox->Checked;
	EV->CR_str		   = CrStrEdit->Text;
	EV->LinkIsIcon	   = LnkIconCheckBox->Checked;
	EV->LinkRelPath	   = LnkRelCheckBox->Checked;
	EV->SpreadShuffle  = ShuffleCheckBox->Checked;
	EV->SpreadAlign    = AlignCheckBox->Checked;
	if (check_TUpDown(ArngPtUpDown)) EV->ArrangePt = ArngPtEdit->Text.ToIntDef(EV->ArrangePt);
	EV->WicScaleOpt    = ScaleOptComboBox->ItemIndex;

	EV->HomeWindow	   = HomeWinEdit->Text;
	EV->HomWinMax	   = HomWinMaxCheckBox->Checked;
	EV->ChildMax	   = ChildMaxCheckBox->Checked;
	EV->HideTitleMenu  = HideTitleCheckBox->Checked;
	EV->RstScrPos	   = RstScrPosCheckBox->Checked;
	EV->ActivDistWin   = ActDstWinCheckBox->Checked;
	EV->OpenLastFile   = OpenLastCheckBox->Checked;

	if (check_TUpDown(ScrWdUpDown)) EV->DefScrWd = ScrWdEdit->Text.ToIntDef(EV->DefScrWd);
	if (check_TUpDown(ScrHiUpDown)) EV->DefScrHi = ScrHiEdit->Text.ToIntDef(EV->DefScrHi);

	switch (MovCsrCombo->ItemIndex) {
	case 1:  EV->crFragMove = crHandGrabL; break;
	case 2:  EV->crFragMove = crHandGrabR; break;
	default: EV->crFragMove = crDefault;
	}

	EV->PlateOnGfrm    = PlateOnGfrmCheckBox->Checked;
	EV->PlateAlign	   = PlateAlignComboBox->ItemIndex;
	EV->PlateShape	   = PlateShapeComboBox->ItemIndex;
	EV->NoNpHideGfrm   = NoNpHideGfrmCheckBox->Checked;
	EV->OnlyNpHideGfrm = OnlyNpHideGfrmCheckBox->Checked;
	EV->GlRightAngle   = GlRightAngCheckBox->Checked;
	EV->MapViewSelRect = MapVSelRctCheckBox->Checked;
	EV->FitMapView	   = FitMapVCheckBox->Checked;
	EV->MapAlpha	   = MapAlphaEdit->Text.ToIntDef(255);
	EV->CopyCLine	   = CopyCLineCheckBox->Checked;
	EV->ClipPosSort    = PosSortCheckBox->Checked;
	EV->NewDropPos	   = NewDropPosCheckBox->Checked;
	EV->DropImgFile    = DropImgCheckBox->Checked;
	EV->ShowDstGrp	   = ShowDstGrpCheckBox->Checked;
	EV->ImageIsBack    = ImgIsBakCheckBox->Checked;
	EV->FrameAllSel    = FrmAllSelCheckBox->Checked;
	EV->ChgGrpModeCtrl = ChgGrpModeCtrlCheckBox->Checked;
	EV->RDragScroll    = RDragScrollCheckBox->Checked;
	EV->SelJumpFrg	   = SelJumpFrgCheckBox->Checked;
	EV->NewBlkDblClick = NewBlkDblClickCheckBox->Checked;
	EV->PrintMono	   = PrintMonoCheckBox->Checked;
	EV->PrintBackGr    = PrintBgCheckBox->Checked;
	EV->PrintTime	   = PrintTimeCheckBox->Checked;
	EV->PrintScrSize   = PrintScrSizeCheckBox->Checked;
	EV->FixPrnLWidth   = MinPrnLWCombo->ItemIndex;
	EV->JpegQuality    = JpgQTrackBar->Position;
	EV->WheelScrQty    = ScrQtyEdit->Text.ToIntDef(16);
	EV->WheelShiftHorz = ShiftHorzCheckBox->Checked;
	if (check_TUpDown(GridSzUpDown)) EV->GridSize = GridSzEdit->Text.ToIntDef(EV->GridSize);
	EV->SnapGrid	   = SnapCheckBox->Checked;
	EV->TrimMovTop	   = TrmMovCheckBox->Checked;
	EV->TrimScrSize    = TrmSizCheckBox->Checked;
	EV->TrimFixSize    = TrmFixCheckBox->Checked;
	EV->TrimSaveAs	   = TrmSavCheckBox->Checked;
	if (check_TUpDown(TrmMgnUpDown)) EV->TrimMargin = TrmMgnEdit->Text.ToIntDef(DEF_TRIM_MGN);
	if (check_TUpDown(TrmWdUpDown))  EV->TrimFixWd	= TrmWdEdit->Text.ToIntDef(DEF_TRIM_WD);
	if (check_TUpDown(TrmHiUpDown))  EV->TrimFixHi	= TrmHiEdit->Text.ToIntDef(DEF_TRIM_HI);

	if (EV->SpiDir!=SpiDirEdit->Text) {
		EV->SpiDir = SpiDirEdit->Text;
		delete SPI;
		SPI = new SpiUnit(EV->SpiDir);
	}
	EV->UseSpiFirst = UseSpiFirstCheckBox->Checked;

	EV->AssociateList->Assign(AssociateListBox->Items);

	for (int i=0; i<EV->LinkIconList->Count; i++) delete (TIcon*)EV->LinkIconList->Objects[i];
	EV->LinkIconList->Clear();

	EV->ExportUTF8 = ExportUtf8CheckBox->Checked;

	//エクスポート項目
	UnicodeString lbuf;
	for (int i=0; i<ExpItemListBox2->Items->Count; i++) {
		if (i>0) lbuf.UCAT_TSTR(",");
		lbuf += ExpItemListBox2->Items->Strings[i];
	}
	lbuf = ReplaceStr(lbuf, "グループ", "GP");
	lbuf = ReplaceStr(lbuf, "種類",	 	"TP");
	lbuf = ReplaceStr(lbuf, "テキスト", "TX");
	lbuf = ReplaceStr(lbuf, "文字色",	"FG");
	lbuf = ReplaceStr(lbuf, "背景色",	"BG");
	lbuf = ReplaceStr(lbuf, "リンク先", "LK");
	lbuf = ReplaceStr(lbuf, "作成日時", "CT");
	lbuf = ReplaceStr(lbuf, "更新日時", "MT");

	EV->CsvFormat = lbuf;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::OkActionUpdate(TObject *Sender)
{
	CtlRetOkCheckBox->Enabled = CrRetKeyCheckBox->Checked;

	bool flag = (TrmMovCheckBox->Checked || TrmSizCheckBox->Checked);
	TrmMgnEdit->Enabled   = flag;
	TrmMgnUpDown->Enabled = flag;

	TrmFixCheckBox->Enabled = TrmSizCheckBox->Checked;

	flag = (TrmSizCheckBox->Checked && TrmFixCheckBox->Checked);
	TrmWdEdit->Enabled	  = flag;
	TrmWdUpDown->Enabled  = flag;
	TrmHiEdit->Enabled	  = flag;
	TrmHiUpDown->Enabled  = flag;

	int idx = AssociateListBox->ItemIndex;
	flag = (!AsoExtEdit->Text.IsEmpty() && !AsoAppEdit->Text.IsEmpty());
	AsoAddBtn->Enabled = flag;
	AsoChgBtn->Enabled = (flag && AssociateListBox->ItemIndex!=-1);
	AsoDelBtn->Enabled = (idx!=-1);

	SizValComboBox->Enabled = (SizSpComboBox->ItemIndex!=-1);

	RegExtBtn->Enabled = EV->IsAdmin;
	DelExtBtn->Enabled = EV->IsAdmin;
	AdminLabel->Font->Color = clRed;
	AdminLabel->Caption = !EV->IsAdmin? "管理者権限がありません" : "";
}

//---------------------------------------------------------------------------
//配色の無効化
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DisableColActionExecute(TObject *Sender)
{
	switch (ColListBox->ItemIndex) {
	case IXCL_MapBG: colbuf_MapBG = clNone; break;
	}
	ColListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DisableColActionUpdate(TObject *Sender)
{
	int idx = ColListBox->ItemIndex;
	((TAction*)Sender)->Enabled = (idx==IXCL_MapBG);
	ColButton->Enabled = (idx!=-1);
}
//---------------------------------------------------------------------------

