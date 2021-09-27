//----------------------------------------------------------------------//
// 個別設定ダイアログ													//
//																		//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "EnvData.h"
#include "OptIdDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptIndDlg *OptIndDlg;

//---------------------------------------------------------------------------
//配色コンボボックスのインデックス
#define IIX_backgr	0
#define IIX_DivLine	1
#define IIX_GrpFrm	2
#define IIX_GrpLine	3
#define IIX_FrgLine	4
#define IIX_LineLbl	5
#define IIX_Title	6

//フォントコンボボックスのインデックス
#define IIXFNT_frg	0
#define IIXFNT_nam	1
#define IIXFNT_sym	2
#define IIXFNT_lbl	3
#define IIXFNT_tit	4

//---------------------------------------------------------------------------
__fastcall TOptIndDlg::TOptIndDlg(TComponent* Owner) : TForm(Owner)
{
	FS = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::FormCreate(TObject *Sender)
{
	frgFont = new TFont();
	namFont = new TFont();
	symFont = new TFont();
	lblFont = new TFont();
	titFont = new TFont();
}

//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::FormShow(TObject *Sender)
{
	if (!FS) return;

	//タイトル
	TitleEdit->Text = FS->data_title;
	TitPosComboBox->ItemIndex = FS->title_pos;

	//画面サイズ
	InhChange = true;
	InitUpDown(ScrWdUpDown,	FS->scr_wd);
	InitUpDown(ScrHiUpDown,	FS->scr_hi);
	SetAspectRatio();
	InhChange = false;
	FixRatioCheckBox->Checked = EV->IndFixRatio;

	//画面分割モード
	DivModComboBox->Clear();
	str_to_ComboBox(DivModComboBox,
		"無し|縦横(分割数指定)|縦横(ピクセル指定)|三角形タイル(水平)|"
		"三角形タイル(垂直)|六角形タイル(水平)|六角形タイル(垂直)");

	switch (FS->div_mod) {
	case DVMODE_DVN: DivModComboBox->ItemIndex = 1; break;
	case DVMODE_PXN: DivModComboBox->ItemIndex = 2; break;
	case DVMODE_TRH: DivModComboBox->ItemIndex = 3; break;
	case DVMODE_TRV: DivModComboBox->ItemIndex = 4; break;
	case DVMODE_HXH: DivModComboBox->ItemIndex = 5; break;
	case DVMODE_HXV: DivModComboBox->ItemIndex = 6; break;
	default:		 DivModComboBox->ItemIndex = 0;
	}

	DivLwComboBox->Clear();
	str_to_ComboBox(DivLwComboBox, "1|3|5|7|9");
	DivLwComboBox->ItemIndex = DivLwComboBox->Items->IndexOf(IntToStr(FS->div_lw));

	DivModComboBoxChange(NULL);

	//配色
	col_backgr	= FS->col_backgr;
	col_DivLine = FS->col_DivLine;
	col_FrgLine = FS->col_FrgLine;
	col_GrpFrm	= FS->col_GrpFrm;
	col_GrpLine = FS->col_GrpLine;
	col_LinLbl	= FS->col_LinLbl;
	col_Title	= FS->col_Title;

	ColComboBox->Clear();
	str_to_ComboBox(ColComboBox,
		"画面背景色|画面分割線|グループ枠|グループ関係線|"
		"断片関係線|関係線ラベル|タイトル");
	ColComboBoxChange(ColComboBox);

	//フォント
	FontComboBox->Clear();
	str_to_ComboBox(FontComboBox, "断片|表札|標識|関係線ラベル|タイトル");
	FontComboBoxChange(FontComboBox);

	frgFont->Assign(FS->frgFont);
	namFont->Assign(FS->namFont);
	symFont->Assign(FS->symFont);
	lblFont->Assign(FS->lblFont);
	titFont->Assign(FS->titFont);
}
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::FormDestroy(TObject *Sender)
{
	delete frgFont;
	delete namFont;
	delete symFont;
	delete lblFont;
	delete titFont;
}

//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::SetAspectRatio()
{
	int wd = ScrWdEdit->Text.ToIntDef(EV->DefScrWd);
	int hi = ScrHiEdit->Text.ToIntDef(EV->DefScrHi);
	AspectRatio = (wd>0 && hi>0)? 1.0 * wd / hi : 0.0;
	UnicodeString tmp;
	RatioLabel->Caption = tmp.sprintf(_T("(%6.4f)"), AspectRatio);
}

//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::ColComboBoxChange(TObject *Sender)
{
	switch (ColComboBox->ItemIndex) {
	case IIX_backgr:	DefColBtn->Enabled = (col_backgr  != clNone);	break;
	case IIX_DivLine:	DefColBtn->Enabled = (col_DivLine != clNone);	break;
	case IIX_GrpFrm:	DefColBtn->Enabled = (col_GrpFrm  != clNone);	break;
	case IIX_GrpLine:	DefColBtn->Enabled = (col_GrpLine != clNone);	break;
	case IIX_FrgLine:	DefColBtn->Enabled = (col_FrgLine != clNone);	break;
	case IIX_LineLbl:	DefColBtn->Enabled = (col_LinLbl  != clNone);	break;
	case IIX_Title:		DefColBtn->Enabled = (col_Title   != clNone);	break;
	default:			DefColBtn->Enabled = false;
	}
	ColButton->Enabled = (ColComboBox->ItemIndex!=-1);
}
//---------------------------------------------------------------------------
//カラー項目の描画
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::ColComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TComboBox *cp = (TComboBox*)Control;
	TCanvas *cv = cp->Canvas;

	cv->Brush->Color = clWindow;
	cv->FillRect(Rect);

	switch (Index) {
	case IIX_backgr:  cv->Brush->Color = GET_indcol(col_backgr);	break;
	case IIX_DivLine: cv->Brush->Color = GET_indcol(col_DivLine);	break;
	case IIX_GrpFrm:  cv->Brush->Color = GET_indcol(col_GrpFrm);	break;
	case IIX_GrpLine: cv->Brush->Color = GET_indcol(col_GrpLine);	break;
	case IIX_FrgLine: cv->Brush->Color = GET_indcol(col_FrgLine);	break;
	case IIX_LineLbl: cv->Brush->Color = GET_indcol(col_LinLbl);	break;
	case IIX_Title:   cv->Brush->Color = GET_indcol(col_Title);		break;
	}
	TRect rc = Rect;  rc.Right = rc.Left + 30;
	cv->FillRect(rc);

	cv->Brush->Color = clWindow;
	cv->Font->Color  = clWindowText;
	int yp = Rect.Top;  if (cv->TextHeight("Q")==abs(cv->Font->Height)) yp += 2;
	cv->TextOut(Rect.Left + 34, yp, cp->Items->Strings[Index]);
}

//---------------------------------------------------------------------------
//配色の設定
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::ColButtonClick(TObject *Sender)
{
	int idx = ColComboBox->ItemIndex;
	if (idx>=0) {
		TColor colbuf;
		switch (idx) {
		case IIX_backgr:  colbuf = col_backgr;	break;
		case IIX_DivLine: colbuf = col_DivLine;	break;
		case IIX_GrpFrm:  colbuf = col_GrpFrm;	break;
		case IIX_GrpLine: colbuf = col_GrpLine;	break;
		case IIX_FrgLine: colbuf = col_FrgLine;	break;
		case IIX_LineLbl: colbuf = col_LinLbl;	break;
		case IIX_Title:   colbuf = col_Title;	break;
		}
		ColorDialog1->Color = colbuf;
		EV->set_custom_col(ColorDialog1->CustomColors);
		if (ColorDialog1->Execute()) {
			colbuf = ColorDialog1->Color;
			EV->get_custom_col(ColorDialog1->CustomColors);
			switch (idx) {
			case IIX_backgr:  col_backgr  = colbuf; break;
			case IIX_DivLine: col_DivLine = colbuf; break;
			case IIX_GrpFrm:  col_GrpFrm  = colbuf; break;
			case IIX_GrpLine: col_GrpLine = colbuf; break;
			case IIX_FrgLine: col_FrgLine = colbuf; break;
			case IIX_LineLbl: col_LinLbl  = colbuf; break;
			case IIX_Title:   col_Title   = colbuf; break;
			}
			ColComboBoxChange(NULL);
			ColComboBox->Repaint();
		}
	}
}
//---------------------------------------------------------------------------
//デフォルト色に戻す
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::DefColBtnClick(TObject *Sender)
{
	switch (ColComboBox->ItemIndex) {
	case IIX_backgr:  col_backgr  = clNone;	break;
	case IIX_DivLine: col_DivLine = clNone;	break;
	case IIX_GrpFrm:  col_GrpFrm  = clNone;	break;
	case IIX_GrpLine: col_GrpLine = clNone;	break;
	case IIX_FrgLine: col_FrgLine = clNone;	break;
	case IIX_LineLbl: col_LinLbl  = clNone;	break;
	case IIX_Title:   col_Title   = clNone;	break;
	}
	DefColBtn->Enabled = false;
	ColComboBox->Repaint();
}

//---------------------------------------------------------------------------
//フォント名表示を更新
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::FontComboBoxChange(TObject *Sender)
{
	switch (FontComboBox->ItemIndex) {
	case IIXFNT_frg: DefFontBtn->Enabled = (!equal_font(frgFont, EV->frgFont));	break;
	case IIXFNT_nam: DefFontBtn->Enabled = (!equal_font(namFont, EV->namFont));	break;
	case IIXFNT_sym: DefFontBtn->Enabled = (!equal_font(symFont, EV->symFont));	break;
	case IIXFNT_lbl: DefFontBtn->Enabled = (!equal_font(lblFont, EV->lblFont));	break;
	case IIXFNT_tit: DefFontBtn->Enabled = (!equal_font(titFont, EV->titFont));	break;
	default:		 DefFontBtn->Enabled = false;
	}
	FontButton->Enabled = (FontComboBox->ItemIndex>=0);
	FontComboBox->Repaint();
}
//---------------------------------------------------------------------------
//フォント項目の描画
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::FontComboBoxDrawItem(TWinControl *Control, int Index,
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
	case IIXFNT_frg: f = frgFont; break;
	case IIXFNT_nam: f = namFont; break;
	case IIXFNT_sym: f = symFont; break;
	case IIXFNT_lbl: f = lblFont; break;
	case IIXFNT_tit: f = titFont; break;
	}
	if (f) {
		cv->Font->Assign(f);
		cv->Font->Size	= cp->Font->Size;
		cv->Font->Color = cp->Font->Color;
		int yp = Rect.Top;  if (cv->TextHeight("Q")==abs(cv->Font->Height)) yp += 2;
		cv->TextOut(xp, yp, f->Name);
	}
}

//---------------------------------------------------------------------------
//フォントの設定
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::FontButtonClick(TObject *Sender)
{
	int idx = FontComboBox->ItemIndex;
	if (idx>=0) {
		switch (idx) {
		case IIXFNT_frg: FontDialog1->Font->Assign(frgFont); break;
		case IIXFNT_nam: FontDialog1->Font->Assign(namFont); break;
		case IIXFNT_sym: FontDialog1->Font->Assign(symFont); break;
		case IIXFNT_lbl: FontDialog1->Font->Assign(lblFont); break;
		case IIXFNT_tit: FontDialog1->Font->Assign(titFont); break;
		}
		if (FontDialog1->Execute()) {
			switch (idx) {
			case IIXFNT_frg: frgFont->Assign(FontDialog1->Font); break;
			case IIXFNT_nam: namFont->Assign(FontDialog1->Font); break;
			case IIXFNT_sym: symFont->Assign(FontDialog1->Font); break;
			case IIXFNT_lbl: lblFont->Assign(FontDialog1->Font); break;
			case IIXFNT_tit: titFont->Assign(FontDialog1->Font); break;
			}
			FontComboBoxChange(FontComboBox);
		}
	}
}
//---------------------------------------------------------------------------
//デフォルトフォントに戻す
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::DefFontBtnClick(TObject *Sender)
{
	switch (FontComboBox->ItemIndex) {
	case IIXFNT_frg: frgFont->Assign(EV->frgFont); break;
	case IIXFNT_nam: namFont->Assign(EV->namFont); break;
	case IIXFNT_sym: symFont->Assign(EV->symFont); break;
	case IIXFNT_lbl: lblFont->Assign(EV->lblFont); break;
	case IIXFNT_tit: titFont->Assign(EV->titFont); break;
	}
	DefFontBtn->Enabled = false;
	FontComboBoxChange(FontComboBox);
	FontComboBox->Repaint();
}

//---------------------------------------------------------------------------
//分割タイプ変更
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::DivModComboBoxChange(TObject *Sender)
{
	bool ispix = (DivModComboBox->ItemIndex!=1);
	DivPrm1UpDown->Max = ispix? 9999 : 100;
	DivPrm2UpDown->Max = ispix? 9999 : 100;

	int p1, p2;
	if (!Sender) {
		p1 = FS->div_x;
		p2 = FS->div_y;
	}
	else {
		if (ispix) {
			p1 = (DivPrm1UpDown->Position<=100)? 400 : DivPrm1UpDown->Position;
			p2 = (DivPrm2UpDown->Position<=100)? 400 : DivPrm2UpDown->Position;
		}
		else {
			p1 = (DivPrm1UpDown->Position>=100)? 2 : DivPrm1UpDown->Position;
			p2 = (DivPrm2UpDown->Position>=100)? 2 : DivPrm2UpDown->Position;
		}
	}

	if (DivModComboBox->ItemIndex==0) {
		DivPrm1Edit->EditLabel->Caption	= EmptyStr;
		DivPrm1Edit->Color		= clBtnFace;
		DivPrm1Edit->Enabled	= false;
		InitUpDown(DivPrm1UpDown, 0);
		DivPrm1UpDown->Enabled	= false;

		DivPrm2Edit->EditLabel->Caption = EmptyStr;
		DivPrm2Edit->Color		= clBtnFace;
		DivPrm2Edit->Enabled	= false;
		InitUpDown(DivPrm2UpDown, 0);
		DivPrm2UpDown->Enabled	= false;

		DivLwComboBox->Enabled	= false;
	}
	else if (DivModComboBox->ItemIndex>2) {
		DivPrm1Edit->EditLabel->Caption = "辺";
		DivPrm1Edit->Color		= clWindow;
		DivPrm1Edit->Enabled	= true;
		InitUpDown(DivPrm1UpDown, p1);
		DivPrm1UpDown->Enabled	= true;

		DivPrm2Edit->EditLabel->Caption = EmptyStr;
		DivPrm2Edit->Color		= clBtnFace;
		DivPrm2Edit->Enabled	= false;
		InitUpDown(DivPrm2UpDown, 0);
		DivPrm2UpDown->Enabled	= false;
		DivLwComboBox->Enabled	= true;
	}
	else {
		DivPrm1Edit->EditLabel->Caption = "横";
		DivPrm1Edit->Color		= clWindow;
		DivPrm1Edit->Enabled	= true;
		InitUpDown(DivPrm1UpDown, p1);
		DivPrm1UpDown->Enabled	= true;

		DivPrm2Edit->EditLabel->Caption = "縦";
		DivPrm2Edit->Color		= clWindow;
		DivPrm2Edit->Enabled	= true;
		InitUpDown(DivPrm2UpDown, p2);
		DivPrm2UpDown->Enabled	= true;
		DivLwComboBox->Enabled	= true;
	}
}

//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::DivEditChange(TObject *Sender)
{
	if (DivPrm1Edit->Text.ToIntDef(0)==0 && DivPrm2Edit->Text.ToIntDef(0)==0) {
		DivModComboBox->ItemIndex = 0;
		DivModComboBoxChange(DivModComboBox);
	}
}

//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::ScrWdEdit_Change(TObject *Sender)
{
	ScrWdEdit->Font->Color = check_TUpDown(ScrWdUpDown)? clWindowText : clRed;
	if (FixRatioCheckBox->Checked) {	//縦横比固定
		if (!InhChange) {
			if (AspectRatio!=0) {
				InhChange = true;
				int wd = ScrWdEdit->Text.ToIntDef(EV->DefScrWd);
				int hi = floor(wd / AspectRatio);
				InitUpDown(ScrHiUpDown, hi);
				InhChange = false;
			}
		}
	}
	else {
		SetAspectRatio();
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::ScrHiEdit_Change(TObject *Sender)
{
	ScrHiEdit->Font->Color = check_TUpDown(ScrHiUpDown)? clWindowText : clRed;
	if (FixRatioCheckBox->Checked) {	//縦横比固定
		if (!InhChange) {
			if (AspectRatio!=0) {
				InhChange = true;
				int hi = ScrHiEdit->Text.ToIntDef(EV->DefScrHi);
				int wd = floor(hi * AspectRatio);
				InitUpDown(ScrWdUpDown, wd);
				InhChange = false;
			}
		}
	}
	else {
		SetAspectRatio();
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::NumberEditExit(TObject *Sender)
{
	TLabeledEdit *ep = (TLabeledEdit*)Sender;
	if (ep->Font->Color == clRed) {
		msgbox_WARN("入力した数値[" + ep->Text + "]は範囲を超えています");
		ep->SetFocus();
	}
}

//---------------------------------------------------------------------------
// OK
//---------------------------------------------------------------------------
void __fastcall TOptIndDlg::OKBtnClick(TObject *Sender)
{
	if (FS) {
		//タイトル
		FS->data_title = TitleEdit->Text.Trim();
		FS->title_pos  = TitPosComboBox->ItemIndex;

		//画面サイズ
		FS->scr_wd = ScrWdEdit->Text.ToIntDef(EV->DefScrWd);
		FS->scr_hi = ScrHiEdit->Text.ToIntDef(EV->DefScrHi);
		FS->VscrBar->Range = FS->scr_hi + 20;
		FS->HscrBar->Range = FS->scr_wd + 20;
		EV->IndFixRatio = FixRatioCheckBox->Checked;

		//画面分割
		switch (DivModComboBox->ItemIndex) {
		case 1: FS->div_mod = DVMODE_DVN; break;
		case 2: FS->div_mod = DVMODE_PXN; break;
		case 3: FS->div_mod = DVMODE_TRH; break;
		case 4: FS->div_mod = DVMODE_TRV; break;
		case 5: FS->div_mod = DVMODE_HXH; break;
		case 6: FS->div_mod = DVMODE_HXV; break;
		default:
				FS->div_mod = DVMODE_NON; break;
		}
		FS->div_x  = DivPrm1Edit->Text.ToIntDef(0);
		FS->div_y  = DivPrm2Edit->Text.ToIntDef(0);
		FS->div_lw = DivLwComboBox->Text.ToIntDef(1);

		//配色
		FS->col_backgr		 = col_backgr;
		FS->frg_owner->Color = (FS->col_backgr==clNone)? EV->col_backgr : FS->col_backgr;
		FS->col_DivLine = col_DivLine;
		FS->col_backgr	= col_backgr;
		FS->col_DivLine = col_DivLine;
		FS->col_GrpFrm	= col_GrpFrm;
		FS->col_GrpLine = col_GrpLine;
		FS->col_FrgLine = col_FrgLine;
		FS->col_LinLbl	= col_LinLbl;
		FS->col_Title	= col_Title;

		//フォント
		FS->frgFont->Assign(frgFont);
		FS->ParentFrgFont = equal_font(frgFont, EV->frgFont);
		FS->namFont->Assign(namFont);
		FS->ParentNamFont = equal_font(namFont, EV->namFont);
		FS->symFont->Assign(symFont);
		FS->ParentSymFont = equal_font(symFont, EV->symFont);
		FS->lblFont->Assign(lblFont);
		FS->ParentLblFont = equal_font(lblFont, EV->lblFont);
		FS->titFont->Assign(titFont);
		FS->ParentTitFont = equal_font(titFont, EV->titFont);
		for (int i=0; i<FS->FrgList->Count; i++) {
			Fragment *fp = FS->FrgList->Items[i];
			fp->frgFont = FS->frgFont;
			fp->namFont = FS->namFont;
			fp->symFont = FS->symFont;
		}
		FS->set_titlebar();
	}
}
//---------------------------------------------------------------------------

