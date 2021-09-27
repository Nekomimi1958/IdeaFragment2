//----------------------------------------------------------------------//
// プロパティ/新規作成ダイアログ										//
//																		//
//----------------------------------------------------------------------//
#include "usr_id3.h"
#include "UserFunc.h"
#include "fragset.h"
#include "EnvData.h"
#include "Main.h"
#include "PropDlg.h"

//----------------------------------------------------------------------------
#pragma resource "*.dfm"

TFrgPropDlg *FrgPropDlg;

//---------------------------------------------------------------------------
__fastcall TFrgPropDlg::TFrgPropDlg(TComponent *Owner) : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FormCreate(TObject *Sender)
{
	prpFS		= NULL;
	prpFrag 	= NULL;
	swNewFrag	= false;
	swNewCurPos = false;
	swNewPlate	= false;
	swNewGroup	= false;
	swNewLink	= false;
	newLink 	= EmptyStr;
	cur_style	= fgsNormal;

	newGrp = 0;
	for (int i=0; i<MAX_IN_GRP; i++) newGstk.stk[i] = (i==0)? 0 : -1;

	//最小サイズを設定
	Constraints->MinWidth =
		SizeGroupBox->Left + ColGroupBox->Left + ColGroupBox->Width + (Width - ClientWidth);
	Constraints->MinHeight =
		TypeGroupBox->Top + TypeGroupBox->Height + Panel2->Height + (Height - ClientHeight) + 8;

	::DragAcceptFiles(Handle, true);
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FormShow(TObject *Sender)
{
	//ダイアログ位置復元
	EV->load_pos_info("FrgPropDlg", (TForm*)this,
		IdeaFragMainForm->Left + 100, IdeaFragMainForm->Top + 100,
		Constraints->MinWidth, Constraints->MinHeight);

	//対象がなければ中断して閉じる
	if (!prpFS) {
		Perform(WM_CLOSE, 0, (NativeInt)0);	return;
	}

	Fragment *dp = prpFrag? prpFrag : prpFS->get_sel_single();
	UnicodeString tmpstr;

	//グループ・コンボボックスの項目設定
	GroupComboBox->Clear();
	for (int i=0; i<MAX_GROUP; i++) {
		prpFS->GInfs->get_name_plate(i + 1, &tmpstr);
		if (tmpstr.IsEmpty()) tmpstr.sprintf(_T("グループ%2u"), i + 1);
		GroupComboBox->Items->Add(tmpstr);
	}
	GroupComboBox->Items->Add("無所属");
	GroupComboBox->Enabled = true;

	GroupComboBox->Color  = clWindow;
	Memo1->Color		  = clWindow;
	FrgWdEdit->Color	  = clWindow;
	FrgHiEdit->Color	  = clWindow;
	LinkNameEdit->Color   = clWindow;
	BackColorBtn->Caption = "背景";
	CharColorBtn->Caption = "文字";

	Memo1->MaxLength	  = MAX_TEXTLEN;
	Memo1->WordWrap 	  = EV->input_wordwrap;

	FitShape->Visible	  = false;

	//新規作成
	if (swNewFrag) {
		Caption = "新規断片の作成";
		int gn = EV->new_no_group? 0 : prpFS->get_sel_group_ex();
		if (swNewGroup && newGrp>=0) gn = newGrp;
		GroupComboBox->ItemIndex = (gn==0)? MAX_GROUP : (gn - 1);

		if (swNewPlate) {
			PlateRadioBtn->Checked = true;
			TypePltAction->Execute();
		}
		else {
			NormalRadioBtn->Checked = true;
			TypeFrgAction->Execute();
			//単独選択の断片があれば、配色を引き継ぐ
			if (dp && dp->style==fgsNormal) {
				Memo1->Font->Color = dp->colFG;
				Memo1->Color	   = dp->colBG;
			}
		}

		Memo1->Lines->Text = EmptyStr;
		Memo1->ReadOnly    = false;
		if (swNewLink) {
			LinkNameEdit->Text = EV->LinkRelPath? abs_to_reldir(newLink, prpFS->file_name) : newLink;
			swNewLink = false;
		}
		else {
			LinkNameEdit->Text = EmptyStr;
		}
		mltSelected = false;
	}
	//プロパティ変更
	else {
		//単独選択
		if (dp) {
			Caption = tmpstr.sprintf(_T("プロパティ   - [ID:%d]"), dp->ID);
			GroupComboBox->ItemIndex = (dp->group.stk[0]==0)? MAX_GROUP : (dp->group.stk[0] - 1);
			cur_style = dp->style;

			switch (cur_style) {
			case fgsPlate:
				PlateRadioBtn->Checked	= true;
				Memo1->Font->Assign(prpFS->namFont);
				Memo1->Color = dp->colBG;
				break;
			case fgsSymbol:
				SymbolRadioBtn->Checked = true;
				Memo1->Font->Assign(prpFS->symFont);
				Memo1->Color = prpFS->frg_owner->Color;
				break;
			case fgsImage:
				ImageRadioBtn->Checked	= true;
				TrnsChkBox->Checked 	= (dp->colBG==clNone);
				TrnsChkBox->AllowGrayed = false;
				Memo1->Font->Assign(prpFS->frgFont);
				Memo1->Font->Color		= clWindowText;
				Memo1->Color			= clWindow;
				break;
			default:
				NormalRadioBtn->Checked = true;
				Memo1->Font->Assign(prpFS->frgFont);
				Memo1->Color = dp->colBG;
			}

			WdUpDown->Max = SymbolRadioBtn->Checked? MAX_TEXTLEN/2 : MAX_FRG_WD;
			Memo1->Font->Color = dp->colFG;
			Memo1->Lines->Text = ReplaceStr(dp->TextStr, EV->CR_str, "\r\n");
			Memo1->ReadOnly    = false;

			InitUpDown(WdUpDown, dp->wd);
			InitUpDown(HiUpDown, dp->hi);

			LinkNameEdit->Text = EV->LinkRelPath? abs_to_reldir(dp->LinkName, prpFS->file_name) : dp->LinkName;
			mltSelected = false;
			Memo1Change(NULL); FrgHiEditChange(NULL);
		}
		//複数選択
		else {
			Caption = "プロパティ";
			Memo1->Font->Assign(prpFS->frgFont);
			Memo1->Font->Color	= clGrayText;
			Memo1->Color		= clBtnFace;
			Memo1->Lines->Text	= "\r\n *** 内容は編集できません ***";
			Memo1->ReadOnly 	= true;
			LinkNameEdit->Text	= EmptyStr;
			LinkNameEdit->Color = clBtnFace;

			//全て同じスタイルか?
			for (int i=0; i<prpFS->SelList->Count; i++) {
				dp = prpFS->SelList->Items[i];
				if (i==0) {
					cur_style = dp->style;
				}
				else if (cur_style!=dp->style) {
					cur_style = fgsNormal; break;
				}
			}

			switch (cur_style) {
			case fgsPlate:  PlateRadioBtn->Checked	= true; break;
			case fgsSymbol: SymbolRadioBtn->Checked = true; break;
			case fgsImage:  ImageRadioBtn->Checked	= true; break;
			default:		NormalRadioBtn->Checked = true; break;
			}

			if (cur_style == fgsImage) LinkNameEdit->Text = "*.*";
			WdUpDown->Max = SymbolRadioBtn->Checked? MAX_TEXTLEN/2 : MAX_FRG_WD;

			//グループ
			int gn = prpFS->get_sel_group();
			if (gn!=-1) {
				GroupComboBox->ItemIndex = (gn==0)? MAX_GROUP : (gn - 1);
			}
			else {
				GroupComboBox->ItemIndex = -1;
				GroupComboBox->Color	 = clBtnFace;
			}

			//全て同じ文字色か？
			TColor col = clNone;
			for (int i=0; i<prpFS->SelList->Count; i++) {
				dp = prpFS->SelList->Items[i];
				if (col == clNone) {
					col = dp->colFG;
				}
				else if (col!=dp->colFG) {
					col = clNone; break;
				}
			}
			if (col!=clNone) Memo1->Font->Color = col;
			//全て同じ背景色か？
			col = clNone;
			for (int i=0; i<prpFS->SelList->Count; i++) {
				dp = prpFS->SelList->Items[i];
				if (col == clNone) {
					col = dp->colBG;
				}
				else if (col!=dp->colBG) {
					col = clNone; break;
				}
			}
			if (cur_style==fgsImage) {
				TrnsChkBox->AllowGrayed = true;
				TrnsChkBox->State		= cbGrayed;
			}
			else if (col!=clNone) {
				Memo1->Color = col;
			}

			//幅を最大値に設定
			int max_w = 0;
			for (int i=0; i<prpFS->SelList->Count; i++)
				max_w = std::max(prpFS->SelList->Items[i]->wd, max_w);
			InitUpDown(WdUpDown, max_w);
			FrgWdEdit->Color   = clBtnFace;

			//高さを最大値に設定
			int max_h = 0;
			for (int i=0; i<prpFS->SelList->Count; i++)
				max_h = std::max(prpFS->SelList->Items[i]->hi, max_h);
			InitUpDown(HiUpDown, max_h);
			FrgHiEdit->Color   = clBtnFace;
			mltSelected = true;
			mlt_group = mlt_wd = mlt_hi = mlt_colFG = mlt_colBG = false;
		}
	}

	if (Memo1->Font->Name[1]=='@')
		Memo1->Font->Name = Memo1->Font->Name.Delete(1, 1);

	if (!Memo1->ReadOnly) {
		Memo1->SetFocus();
		Memo1Enter(Memo1);

		//カーソルを文末へ
		int p = Memo1->GetTextLen();
		Memo1->SelStart  = p;
		Memo1->SelLength = p;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FormClose(TObject *Sender,
	  TCloseAction &Action)
{
	swNewCurPos = swNewGroup = swNewPlate = false;
	prpFrag 	= NULL;

	//ダイアログ位置・サイズを保存
	EV->save_pos_info("FrgPropDlg", (TForm*)this);
}

//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::GripPaintBoxPaint(TObject *Sender)
{
	//サイズ変更グリップを描画
	RECT rc = Rect(
		GripPaintBox->Width - ::GetSystemMetrics(SM_CXHSCROLL),
		GripPaintBox->Height - ::GetSystemMetrics(SM_CYVSCROLL),
		GripPaintBox->Width, GripPaintBox->Height);
	::DrawFrameControl(GripPaintBox->Canvas->Handle, &rc,
		DFC_SCROLL,	DFCS_SCROLLSIZEGRIP);
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FormResize(TObject *Sender)
{
	Repaint();
}

//---------------------------------------------------------------------------
// ファイルのドロップ
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::WMDropFiles(TWMDropFiles &message)
{
	UINT filecount = DragQueryFile((HDROP)message.Drop, 0xFFFFFFFF, NULL, 0);
	if (filecount>0) {
		UnicodeString filename;
		filename.SetLength(MAX_PATH);
		int len = DragQueryFile((HDROP)message.Drop, 0, filename.c_str(), filename.Length());
		filename.SetLength(len);
		LinkNameEdit->Text
			= EV->LinkRelPath? abs_to_reldir(filename, prpFS->file_name) : filename;
	}
	::DragFinish((HDROP)message.Drop);
}

//---------------------------------------------------------------------
//グループ変更
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::GroupComboBoxChange(TObject *Sender)
{
	if (!prpFS) return;
	int idx = GroupComboBox->ItemIndex;
	int gn	= (idx==MAX_GROUP)? 0 : (idx + 1);
	Fragment *np = prpFS->GInfs->get_name_plate(gn, NULL);
	if (swNewFrag) {
		PlateRadioBtn->Enabled = (gn!=0 && !np);
		if (newGstk.stk[0]!=gn) {
			for (int i=0; i<MAX_IN_GRP; i++) newGstk.stk[i] = (i==0)? gn : -1;
		}
	}
	else {
		Fragment *dp = prpFS->get_sel_single();
		if (dp) PlateRadioBtn->Enabled = (gn!=0 && (!np || np==dp));
		if (mltSelected) {
			mlt_group = true;
			GroupComboBox->Color = clWindow;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::Memo1Enter(TObject *Sender)
{
	if (!Memo1->ReadOnly && EV->ime_on) {
		//IMEオン
		HIMC hIMC = ::ImmGetContext(Memo1->Handle);
		if (hIMC!=0) {
			if (!::ImmGetOpenStatus(hIMC)) ::ImmSetOpenStatus(hIMC, TRUE);
			::ImmReleaseContext(Memo1->Handle, hIMC);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::Memo1Exit(TObject *Sender)
{
	//IMEオフ
	HIMC hIMC = ::ImmGetContext(Memo1->Handle);
	if (hIMC!=0) {
		if (::ImmGetOpenStatus(hIMC)) ::ImmSetOpenStatus(hIMC, FALSE);
		::ImmReleaseContext(Memo1->Handle, hIMC);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::Memo1Change(TObject *Sender)
{
	if (!prpFS) return;
	//標識の場合、桁を自動フィット
	if (SymbolRadioBtn->Checked) {
		int n = (Memo1->Lines->Text.Length() + 1)/2;
		if (n==0) n = 1;
		InitUpDown(WdUpDown, n);
	}
	//デフォルトを超えたら自動的に行数を増やす
	else if (EV->auto_exp) {
		TCanvas *cv = prpFS->frg_owner->Canvas;
		switch (cur_style) {
		case fgsPlate:  cv->Font->Assign(prpFS->namFont); break;
		case fgsSymbol: cv->Font->Assign(prpFS->symFont); break;
		default:		cv->Font->Assign(prpFS->frgFont); break;
		}
		int l = GetFitHeight(cv, Memo1->Lines->Text, FrgWdEdit->Text.ToInt());
		if		(PlateRadioBtn->Checked && l>=EV->nam_hi)	InitUpDown(HiUpDown, l);
		else if (NormalRadioBtn->Checked && l>=EV->def_hi)	InitUpDown(HiUpDown, l);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FrgWdEdit_Change(TObject *Sender)
{
	//範囲チェック
	int fw = FrgWdEdit->Text.ToIntDef(1);
	if		(fw < WdUpDown->Min) FrgWdEdit->Text = WdUpDown->Min;
	else if (fw > WdUpDown->Max) FrgWdEdit->Text = WdUpDown->Max;

	if (mltSelected) {
		mlt_wd = true;
		FrgWdEdit->Color = clWindow;
	}
	else {
		Memo1Change(NULL);
		FrgHiEditChange(NULL);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FrgHiEditChange(TObject *Sender)
{
	//範囲チェック
	int	fh = FrgHiEdit->Text.ToIntDef(1);
	if		(fh < HiUpDown->Min) FrgHiEdit->Text = HiUpDown->Min;
	else if (fh > HiUpDown->Max) FrgHiEdit->Text = HiUpDown->Max;

	//行数がデフォルトを越えたら赤色表示
	FrgHiEdit->Font->Color = clWindowText;
	if (PlateRadioBtn->Checked && FrgHiEdit->Text.ToInt()>EV->nam_hi)
		FrgHiEdit->Font->Color = clRed;
	else if (NormalRadioBtn->Checked && FrgHiEdit->Text.ToInt()>EV->def_hi)
		FrgHiEdit->Font->Color = clRed;
	if (mltSelected) {
		mlt_hi = true;
		FrgHiEdit->Color = clWindow;
	}
}

//---------------------------------------------------------------------------
//フィットする桁数を取得
//現在の桁より小さくならなければ 0 を返す
//---------------------------------------------------------------------------
int __fastcall TFrgPropDlg::GetFitWidth(TCanvas * cv, UnicodeString sbuf, int wd, int hi)
{
	int fitw = 0;
	for (int iw=wd; iw>0; iw--) {
		int  cp = 1;
		int  n  = 1;
		int crp = sbuf.Pos("\r\n");
		int w = abs(cv->Font->Height) * iw;
		bool over = false;
		while (!over) {
			if (EV->accept_cr && crp==n) {
				cp += (n + 1); n = 1;
				crp = sbuf.SubString(cp, MAX_FRG_WD*2).Pos("\r\n");
			}
			else if (cv->TextWidth(sbuf.SubString(cp, n))>w) {
				n--; if (n==0) n = 1;
				cp += n;
				if (cp>sbuf.Length()) break;
				//行途中で改行があれば駄目
				over = true;
			}
			else {
				if ((cp + n - 1)>=sbuf.Length()) break;
				n++;
			}
		}
		if (over) break;
		fitw = iw;
	}

	//1行でリンクマーク付の場合1桁増やす
	if (fitw>0) {
		if (hi==1 && EV->show_LinkMark && !LinkNameEdit->Text.IsEmpty()) fitw ++;
	}
	return fitw;
}
//---------------------------------------------------------------------------
//フィットする行数を取得
//---------------------------------------------------------------------------
int __fastcall TFrgPropDlg::GetFitHeight(TCanvas * cv, UnicodeString sbuf, int wd)
{
	int w = get_FontWidthF(cv) * wd;
	if (w<=0) return EV->def_hi;

	int cp = 1;
	int n  = 1;
	int l  = 1;
	int crp  = sbuf.Pos("\r\n");

	for (;;) {
		if (EV->accept_cr && crp==n) {
			cp += (n + 1); n = 1;
			crp = sbuf.SubString(cp, MAX_FRG_WD*2).Pos("\r\n");
			l++;
		}
		else if (cv->TextWidth(sbuf.SubString(cp, n))>w) {
			if (n>1) n--;
			cp += n;
			if (cp>sbuf.Length()) break;
			n = 1;
			crp = sbuf.SubString(cp, MAX_FRG_WD*2).Pos("\r\n");
			l++;
		}
		else {
			if ((cp + n - 1)>=sbuf.Length()) break;
			n++;
		}
	}
	return l;
}

//----------------------------------------------------------------------------
//透過
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::TrnsChkBoxClick(TObject *Sender)
{
	if (mltSelected) {
		mlt_colBG = true;
		TrnsChkBox->AllowGrayed = false;
	}
}

//---------------------------------------------------------------------
//リンク先ファイル情報取得 ポップアップメニュー設定
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::PopupMenu1Popup(TObject *Sender)
{
	for (int i=0; i<PopupMenu1->Items->Count; i++) {
		TMenuItem *mp = PopupMenu1->Items->Items[i];
		if (mp->Tag!=-1) mp->Visible = false;
	}

	if (!LinkNameEdit->Text.IsEmpty()) {
		UnicodeString fnam = rel_to_absdir(LinkNameEdit->Text, prpFS->file_name);
		if (StartsText("onenote:///", fnam)) {
			PopGetPageItem->Visible = true;
		}
		else {
			UnicodeString ext = ExtractFileExt(fnam);
			if (test_FileExt(ext, ".htm.html.shtml")) {
				PopGetTitItem->Visible = true;
				PopGetDscItem->Visible = true;
				PopGetKeyItem->Visible = true;
			}
			else if (test_FileExt(ext, ".hlp")) {
				PopGetTitItem->Visible = true;
			}
			else if (test_FileExt(ext, ".mp3")) {
				PopGetTrakItem->Visible   = true;
				PopGetArtistItem->Visible = true;
				PopGetAlbumItem->Visible  = true;
				PopGetYearItem->Visible   = true;
				PopGetComItem->Visible	  = true;
			}
			else if (is_text_file(fnam)) {
				PopGetTopItem->Visible = (ext!=".IFD");
			}
		}
	}
}

//---------------------------------------------------------------------------
//タイトルを取得
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::PopGetTitItemClick(TObject *Sender)
{
	UnicodeString fnam = rel_to_absdir(LinkNameEdit->Text, prpFS->file_name);
	UnicodeString ext  = ExtractFileExt(fnam);
	UnicodeString strbuf;
	//HTML文書
	if (test_FileExt(ext, ".htm.html.shtml")) {
		std::unique_ptr<TStringList> hbuf(new TStringList());
		if (get_html_inf(fnam, hbuf.get())) strbuf  = hbuf->Values["title"];
	}
	//ヘルプ
	else if (test_FileExt(ext, ".hlp")) {
		strbuf = get_hlp_title(fnam);
	}

	if (!strbuf.IsEmpty()) Memo1->SelText = strbuf;
}
//---------------------------------------------------------------------------
// HTML文書の META 情報を取得
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::PopGetMetaItemClick(TObject *Sender)
{
	std::unique_ptr<TStringList> hbuf(new TStringList());
	if (get_html_inf(rel_to_absdir(LinkNameEdit->Text, prpFS->file_name), hbuf.get())) {
		UnicodeString strbuf;
		switch (((TComponent*)Sender)->Tag) {
		case 0: strbuf = hbuf->Values["description"]; break;
		case 1: strbuf = hbuf->Values["keywords"]; break;
		}
		if (!strbuf.IsEmpty()) Memo1->SelText = strbuf;
	}
}
//---------------------------------------------------------------------------
//1行目を取得
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::PopGetTopItemClick(TObject *Sender)
{
	UnicodeString strbuf = get_top_textline(rel_to_absdir(LinkNameEdit->Text, prpFS->file_name));
	if (!strbuf.IsEmpty()) Memo1->SelText = strbuf;
}
//---------------------------------------------------------------------------
//MP3 ID3タグ情報取得
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::PopGetMp3ItemClick(TObject *Sender)
{
	std::unique_ptr<TStringList> ibuf(new TStringList());
	if (ID3_GetV1Inf(rel_to_absdir(LinkNameEdit->Text, prpFS->file_name), ibuf.get())) {
		UnicodeString strbuf;
		switch (((TComponent*)Sender)->Tag) {
		case 0: strbuf = ibuf->Values["TrackName"];  break;
		case 1: strbuf = ibuf->Values["ArtistName"]; break;
		case 2: strbuf = ibuf->Values["AlbumName"];  break;
		case 3: strbuf = ibuf->Values["Year"];		 break;
		case 4: strbuf = ibuf->Values["Comment"];	 break;
		}
		if (!strbuf.IsEmpty()) Memo1->SelText = strbuf;
	}
}
//---------------------------------------------------------------------------
//OneNote ページ名を取得
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::PopGetPageItemClick(TObject *Sender)
{
	UnicodeString strbuf = rel_to_absdir(LinkNameEdit->Text, prpFS->file_name);
	if (StartsText("onenote:///", strbuf)) {
		strbuf = get_tkn(get_tkn_r(strbuf, "one#"), "&");
		Memo1->SelText = strbuf;
	}
}

//---------------------------------------------------------------------------
//フルパス付ファイル名
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::PopGetPathItemClick(TObject *Sender)
{
	UnicodeString strbuf = rel_to_absdir(LinkNameEdit->Text, prpFS->file_name);
	if (!strbuf.IsEmpty()) {
		if (StartsText("onenote:///", strbuf)) {	//OneNote
			strbuf = get_tkn(get_tkn(get_tkn_r(strbuf, "onenote:///"), "&"), "#");
			strbuf = ReplaceStr(strbuf, "%20", " ");
		}
		Memo1->SelText = strbuf;
	}
}
//---------------------------------------------------------------------------
//ファイル名
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::PopGetFileItemClick(TObject *Sender)
{
	UnicodeString strbuf = LinkNameEdit->Text;
	if (!strbuf.IsEmpty()) {
		if (StartsText("onenote:///", strbuf)) {	//OneNote
			strbuf = get_tkn(get_tkn(get_tkn_r(strbuf, "onenote:///"), "&"), "#");
			strbuf = ReplaceStr(strbuf, "%20", " ");
		}
		Memo1->SelText = ExtractFileName(strbuf);
	}
}
//---------------------------------------------------------------------------
//拡張子を除いたファイル名
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::PopGetNameItemClick(TObject *Sender)
{
	UnicodeString strbuf = LinkNameEdit->Text;
	if (!strbuf.IsEmpty()) {
		if (StartsText("onenote:///", strbuf)) {	//OneNote
			strbuf = get_tkn(get_tkn(get_tkn_r(strbuf, "onenote:///"), "&"), "#");
			strbuf = ReplaceStr(strbuf, "%20", " ");
		}
		Memo1->SelText = ChangeFileExt(ExtractFileName(strbuf), EmptyStr);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::CMDialogKey(TCMDialogKey &Msg)
{
	TRadioButton *rp[4];
	rp[0] = NormalRadioBtn;
	rp[1] = PlateRadioBtn;
	rp[2] = SymbolRadioBtn;
	rp[3] = ImageRadioBtn;

	int idx;

	switch (Msg.CharCode) {
	case VK_UP: case VK_DOWN: case VK_LEFT: case VK_RIGHT:
		//IME が正しいモードで開かれない現象への苦しまぎれの対処
		//↓↑の処理を自前で行う
		//フォーカス移動せずにチェックを変えればOKらしい
		idx = -1;
		for (int i=0; i<4; i++) if (rp[i]->Focused()) { idx=i; break; }
		if (idx==-1) break;

		switch (Msg.CharCode) {
		case VK_DOWN:
			for (int i=0; i<4; i++) {
				idx = (idx + 1)%4;  if (!rp[idx]->Enabled) continue;
				rp[idx]->Checked = true;
				Msg.Result = 1;
				break;
			}
			break;

		case VK_UP:
			for (int i=0; i<4; i++) {
				idx = (idx + 3)%4;  if (!rp[idx]->Enabled) continue;
				rp[idx]->Checked = true;
				Msg.Result = 1;
				break;
			}
			break;

		default:
			Msg.Result = 1;
		}
		break;

	case VK_TAB:
		//フォーカス移動
		SelectNext(ActiveControl, !is_KeyDown(VK_SHIFT), true);
		Msg.Result = 1;
		break;

	case VK_RETURN:
		{
			bool ok_flag = false;
			if (Memo1->Focused()) {
				if (EV->accept_cr) {
					if (EV->cr_returnkey) {
						if (EV->ok_ctrl_return && is_KeyDown(VK_CONTROL))
							ok_flag = true;
						else
							Memo1->SelText = "\r\n";
					}
					else if (!is_KeyDown(VK_CONTROL)) {
						ok_flag = true;
					}
				}
				else {
					ok_flag = true;
				}
			}
			else {
				if (SameText(ActiveControl->ClassName(), "TButton")) {
					if (OkBtn->Focused()) {
						ok_flag = true;
					}
					else {
						TButton *bp = (TButton*)ActiveControl;
						bp->Click();
					}
				}
				else {
					ok_flag = true;
				}
			}

			if (ok_flag) {
				Msg.Result = 1;
				OkAction->Execute();
			}
		}
		break;

	case 'O':
		if (is_KeyDown(VK_MENU)) {
			Msg.Result = 1;
			OkBtn->Click();
		}
		break;

	case VK_ESCAPE:
		Msg.Result	= 1;
		ModalResult = mrCancel;
		break;
	}
}

//---------------------------------------------------------------------------
//タイプ : 断片
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::TypeFrgActionExecute(TObject *Sender)
{
	cur_style = fgsNormal;

	Memo1->Font->Assign(prpFS->frgFont);
	Memo1->Font->Color = EV->col_defFG;
	Memo1->Color	   = EV->col_defBG;
	WdUpDown->Max	   = MAX_FRG_WD;
	InitUpDown(WdUpDown, EV->def_wd);
	InitUpDown(HiUpDown, EV->def_hi);

	if (Memo1->Font->Name[1]=='@') Memo1->Font->Name = Memo1->Font->Name.Delete(1, 1);
	Memo1Change(NULL); FrgHiEditChange(NULL);

	if (!Memo1->ReadOnly) Memo1->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::TypeFrgActionUpdate(TObject *Sender)
{
	Fragment *dp = prpFrag? prpFrag : prpFS->get_sel_single();
	//新規作成
	if (swNewFrag)
		((TAction*)Sender)->Enabled = !swNewPlate;
	//プロパティ変更
	else
		((TAction*)Sender)->Enabled = (dp!=NULL);

	//他のタイプのもここで更新
	bool isFP = (cur_style==fgsNormal || cur_style==fgsPlate);
	GroupComboBox->Enabled = !swNewPlate;
	SizeGroupBox->Enabled  = (isFP || cur_style==fgsImage);
	SizeGroupBox->Caption  = (cur_style==fgsImage)? "倍率(&S)" : "サイズ(&S)";
	FrgWdEdit->EditLabel->Visible = (cur_style!=fgsImage);
	FrgHiEdit->EditLabel->Visible = (cur_style!=fgsImage);
	SizePer_Label->Visible = (cur_style==fgsImage);
	WdUpDown->Enabled	   = (isFP || cur_style==fgsImage);
	HiUpDown->Enabled	   = (isFP || cur_style==fgsImage);
	FitWdBtn->Visible	   = (cur_style!=fgsImage);
	FitHiBtn->Visible	   = (cur_style!=fgsImage);
	CharColorBtn->Visible  = (cur_style!=fgsImage);
	BackColorBtn->Visible  = (cur_style!=fgsImage);
	BackColorBtn->Enabled  = isFP;
	TrnsChkBox->Visible    = (cur_style==fgsImage);
}

//---------------------------------------------------------------------------
//タイプ : 表札
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::TypePltActionExecute(TObject *Sender)
{
	cur_style = fgsPlate;

	Memo1->Font->Assign(prpFS->namFont);
	Memo1->Font->Color = EV->col_namFG;
	Memo1->Color	   = EV->col_namBG;
	WdUpDown->Max	   = MAX_FRG_WD;
	InitUpDown(WdUpDown, EV->nam_wd);
	InitUpDown(HiUpDown, EV->nam_hi);
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::TypePltActionUpdate(TObject *Sender)
{
	int idx = GroupComboBox->ItemIndex;
	int gn	= (idx==MAX_GROUP)? 0 : (idx + 1);
	Fragment *np = prpFS->GInfs->get_name_plate(gn, NULL);
	//新規作成
	if (swNewFrag) {
		((TAction*)Sender)->Enabled = swNewPlate? true : (gn!=0 && !np);
	}
	//プロパティ変更
	else {
		Fragment *dp = prpFrag? prpFrag : prpFS->get_sel_single();
		((TAction*)Sender)->Enabled = !mltSelected? (gn!=0 && (!np || np==dp)) : false;
	}
}

//---------------------------------------------------------------------------
//タイプ : 標識
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::TypeSymActionExecute(TObject *Sender)
{
	cur_style = fgsSymbol;

	Memo1->Font->Assign(prpFS->symFont);
	Memo1->Font->Color = EV->col_symFG;
	Memo1->Color	   = prpFS->frg_owner->Color;

	WdUpDown->Max	   = MAX_TEXTLEN/2;
	InitUpDown(WdUpDown, 1);
	InitUpDown(HiUpDown, 1);
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::TypeSymActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = swNewFrag? !swNewPlate : !mltSelected;
}

//---------------------------------------------------------------------------
//タイプ : 画像
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::TypeImgActionExecute(TObject *Sender)
{
	cur_style = fgsImage;

	Memo1->Font->Assign(prpFS->frgFont);
	Memo1->Font->Color = clWindowText;
	Memo1->Color	   = clWindow;

	WdUpDown->Max	   = 40;
	InitUpDown(WdUpDown, EV->img_r1);
	InitUpDown(HiUpDown, EV->img_r2);
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::TypeImgActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = swNewFrag? !swNewPlate : !mltSelected;
}

//---------------------------------------------------------------------------
//テキスト内容から桁数フィット
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FitWdActionExecute(TObject *Sender)
{
	if (!prpFS) return;
	TCanvas *cv	= prpFS->frg_owner->Canvas;
	switch (cur_style) {
	case fgsPlate:  cv->Font->Assign(prpFS->namFont); break;
	case fgsSymbol: cv->Font->Assign(prpFS->symFont); break;
	default:		cv->Font->Assign(prpFS->frgFont); break;
	}
	int wd = GetFitWidth(cv, Memo1->Lines->Text, FrgWdEdit->Text.ToInt(), FrgHiEdit->Text.ToInt());
	if (wd>0) InitUpDown(WdUpDown, wd);

	if (is_KeyDown(VK_SHIFT) || is_KeyDown(VK_CONTROL)) {
		InitUpDown(HiUpDown, GetFitHeight(cv, Memo1->Lines->Text, FrgWdEdit->Text.ToInt()));
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FitWdActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !mltSelected? (cur_style==fgsNormal || cur_style==fgsPlate) : false;

	FitShape->Visible = (is_KeyDown(VK_SHIFT) || is_KeyDown(VK_CONTROL));
}

//---------------------------------------------------------------------------
//テキスト内容から行数フィット
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FitHiActionExecute(TObject *Sender)
{
	if (!prpFS) return;
	TCanvas *cv	= prpFS->frg_owner->Canvas;
	switch (cur_style) {
	case fgsPlate:  cv->Font->Assign(prpFS->namFont); break;
	case fgsSymbol: cv->Font->Assign(prpFS->symFont); break;
	default:		cv->Font->Assign(prpFS->frgFont); break;
	}
	InitUpDown(HiUpDown, GetFitHeight(cv, Memo1->Lines->Text, FrgWdEdit->Text.ToInt()));

	if (is_KeyDown(VK_SHIFT) || is_KeyDown(VK_CONTROL)) {
		int wd = GetFitWidth(cv, Memo1->Lines->Text, FrgWdEdit->Text.ToInt(), FrgHiEdit->Text.ToInt());
		if (wd>0) InitUpDown(WdUpDown, wd);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::FitHiActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !mltSelected? (cur_style==fgsNormal || cur_style==fgsPlate) : false;
}

//---------------------------------------------------------------------------
//文字色
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::CharColActionExecute(TObject *Sender)
{
	ColorDialog1->Color	= Memo1->Font->Color;
	EV->set_custom_col(ColorDialog1->CustomColors);
	if (ColorDialog1->Execute()) {
		Memo1->Font->Color = ColorDialog1->Color;
		EV->get_custom_col(ColorDialog1->CustomColors);
		if (mltSelected) {
			CharColorBtn->Caption = "*文字";
			mlt_colFG = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::CharColActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (cur_style!=fgsImage);
}

//---------------------------------------------------------------------------
//背景色
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::BackColActionExecute(TObject *Sender)
{
	ColorDialog1->Color	= Memo1->Color;
	EV->set_custom_col(ColorDialog1->CustomColors);
	if (ColorDialog1->Execute()) {
		Memo1->Color = ColorDialog1->Color;
		EV->get_custom_col(ColorDialog1->CustomColors);
		if (mltSelected) {
			BackColorBtn->Caption = "*背景";
			mlt_colBG = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::BackColActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (cur_style!=fgsImage);
}

//---------------------------------------------------------------------------
//リンク先の参照
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::RefLinkActionExecute(TObject *Sender)
{
	if (cur_style==fgsImage) {
		OpenImgDlg->Title	   = "画像の指定";
		OpenImgDlg->InitialDir = IniFile->ReadString("General","FrgPropDlgImageDir");
		UnicodeString flt_str;
		flt_str = GraphicFilter(__classid(Graphics::TBitmap));
		flt_str.UCAT_TSTR("|JPEGファイル(*.JPG)|*.JPG;*.JPE;*.JPEG");
		flt_str.UCAT_TSTR("|PNGファイル(*.PNG)|*.PNG");
		flt_str.UCAT_TSTR("|GIFファイル(*.GIF)|*.GIF");
		flt_str.UCAT_TSTR("|メタファイル(*.WMF;*.EMF)|*.WMF;*.EMF|");
		flt_str += GraphicFilter(__classid(Graphics::TIcon));
		flt_str.UCAT_TSTR("|MP3ファイル(*.MP3)|*.MP3");
		for (int i=0; i<SPI->PlgList->Count; i++) {
			spi_info *sp = SPI->PlgList->Items[i];
			if (sp->FileExt.IsEmpty()) continue;
			flt_str.cat_sprintf(_T("|%s|%s"), sp->FileType.c_str(), sp->FileExt.c_str());
		}
		OpenImgDlg->Filter = flt_str;
		if (!LinkNameEdit->Text.IsEmpty()) {
			UnicodeString ext = ExtractFileExt(LinkNameEdit->Text);
			OpenImgDlg->FilterIndex =
				test_FileExt(ext,".bmp")? 			1 :
				test_FileExt(ext,".jpg.jpe.jpeg")?	2 :
				test_FileExt(ext,".png")?			3 :
				test_FileExt(ext,".gif")?			4 :
				test_FileExt(ext,".wmf.emf")?		5 :
				test_FileExt(ext,".ico")?			6 :
				test_FileExt(ext,".mp3")?			7 : 1;
		}
		OpenImgDlg->FileName = EmptyStr;
		if (OpenImgDlg->Execute()) {
			LinkNameEdit->Text = EV->LinkRelPath?
					abs_to_reldir(OpenImgDlg->FileName, prpFS->file_name) : OpenImgDlg->FileName;
			IniFile->WriteString("General","FrgPropDlgImageDir",ExtractFileDir(OpenImgDlg->FileName));
			if (test_FileExt(ExtractFileExt(LinkNameEdit->Text), ".wmf.em")) TrnsChkBox->Checked = true;
		}
	}
	else {
		OpenLnkDlg->Title = "リンク先の指定";
		OpenLnkDlg->InitialDir
			= IniFile->ReadString("General","FrgPropDlgLinkDir");
		OpenLnkDlg->Filter
			= "データファイル(*.IFD)|*.IFD|テキスト|*.txt|HTML文書|*.htm*|その他 (*.*)|*.*";
		OpenLnkDlg->FileName = EmptyStr;
		if (OpenLnkDlg->Execute()) {
			LinkNameEdit->Text = EV->LinkRelPath?
					abs_to_reldir(OpenLnkDlg->FileName, prpFS->file_name) : OpenLnkDlg->FileName;
			IniFile->WriteString("General","FrgPropDlgLinkDir",ExtractFileDir(OpenLnkDlg->FileName));
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::RefLinkActionUpdate(TObject *Sender)
{
	LinkNameEdit->Enabled = !mltSelected;
	((TAction*)Sender)->Enabled = !mltSelected;
}

//---------------------------------------------------------------------------
//ＯＫ
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::OkActionExecute(TObject *Sender)
{
	int idx = GroupComboBox->ItemIndex;
	int gn	= (idx==MAX_GROUP)? 0 : (idx + 1);
	int fw	= FrgWdEdit->Text.ToIntDef(1);
	int fh	= FrgHiEdit->Text.ToIntDef(1);
	Fragment *dp;

	prpFS->push_all();

	//新規
	if (swNewFrag) {
		dp = prpFS->new_frag();
		if (dp) {
			if (newGstk.stk[0]!=gn) {
				for (int i=0; i<MAX_IN_GRP; i++) newGstk.stk[i] = (i==0)? gn : -1;
			}
			dp->group	 = newGstk;
			dp->style	 = cur_style;
			dp->LinkName = rel_to_absdir(LinkNameEdit->Text, prpFS->file_name);
			dp->wd		 = fw;
			dp->hi		 = fh;

			//位置指定あり
			if (swNewCurPos) {
				dp->RC.Left	= newPosX + dp->Parent->HorzScrollBar->Position;
				dp->RC.Top	= newPosY + dp->Parent->VertScrollBar->Position;
			}
			//新規作成位置(重なりをさける)
			else {
				int pix = 0;
				for (;;) {
					prpFS->set_new_pos(dp, pix);
					bool flag = false;
					for (int i=0; i<prpFS->FrgList->Count; i++) {
						Fragment *fp = prpFS->FrgList->Items[i];
						if (fp==dp) continue;
						if (fp->RC.Left!=dp->RC.Left) continue;
						if (fp->RC.Top!=dp->RC.Top) continue;
						flag = true; break;
					}
					if (!flag) break;
					pix += 2;
				}
			}
			dp->colFG = Memo1->Font->Color;
			dp->colBG = Memo1->Color;
			dp->TextStr = ReplaceStr(Memo1->Lines->Text, "\r\n", EV->CR_str);
			dp->upd_link();
			prpFS->GInfs->Items[gn].Visible = true;
			dp->Visible = true;
			dp->CreTime = Now();
			dp->ModTime = Now();
			dp->Changed = true;
			if (dp->style!=fgsPlate || !EV->show_GroupFrame || !EV->PlateOnGfrm) dp->show_frag();
		}
	}
	//プロパティ変更
	else {
		dp = prpFrag? prpFrag :prpFS->get_sel_single();
		//単独
		if (dp) {
			dp->era_frag();
			dp->LinkName = rel_to_absdir(LinkNameEdit->Text, prpFS->file_name);
			if (dp->group.stk[0]!=gn) {
				dp->set_group(gn);
				if (EV->ShowDstGrp) prpFS->GInfs->Items[gn].Visible = true;
			}
			dp->style	= cur_style;
			dp->wd		= fw;
			dp->hi		= fh;
			dp->colFG	= Memo1->Font->Color;
			dp->TextStr = ReplaceStr(Memo1->Lines->Text,"\r\n",EV->CR_str);
			if (dp->style==fgsImage)
				dp->colBG = TrnsChkBox->Checked? clNone : clWhite;
			else
				dp->colBG = Memo1->Color;
			dp->upd_link();
			dp->ModTime = Now();
			dp->Changed = true;
			if (dp->Visible) {
				if (dp->style==fgsPlate && EV->PlateOnGfrm) {
					prpFS->GInfs->Items[gn].gfp->Repaint();
				}
				else {
					dp->Selected = true;
					dp->show_frag();
				}
			}
		}
		//複数
		else {
			if (mlt_group) prpFS->mov_sel_group(gn);
			for (int i=0; i<prpFS->SelList->Count; i++) {
				dp = prpFS->SelList->Items[i];
				dp->era_frag();
				if (cur_style==fgsImage) {
					if (mlt_wd || mlt_hi) {
						dp->wd = fw;
						dp->hi = fh;
					}
				}
				else if (dp->style!=fgsImage) {
					if (mlt_wd) dp->wd = fw;
					if (mlt_hi) dp->hi = fh;
				}
				if (cur_style==fgsImage) {
					if (mlt_colBG) dp->colBG = TrnsChkBox->Checked? clNone : clWhite;
				}
				else {
					if (mlt_colFG)	dp->colFG = Memo1->Font->Color;
					if (mlt_colBG)	dp->colBG = Memo1->Color;
				}
				dp->upd_link();
				dp->ModTime = Now();
				dp->Changed = true;
				if (dp->Visible) dp->show_frag();
			}
		}
	}

	prpFS->update_grp_empty();
	prpFS->modify = true;

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::OkActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	switch (cur_style) {
	case fgsSymbol:
		ap->Enabled = (!Memo1->Lines->Text.IsEmpty()); break;
	case fgsImage:
		ap->Enabled = (!LinkNameEdit->Text.IsEmpty()); break;
	default:
		ap->Enabled = true;
	}
}
//---------------------------------------------------------------------------
//リンク先ファイルの各種情報を取得
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::GetLnkInfActionExecute(TObject *Sender)
{
	TPoint p = LinkGrpBox->ClientToScreen(Point(LnkExBtn->Left, LnkExBtn->Top));
	PopupMenu1->Popup(p.x + 8, p.y + 8);
}
//---------------------------------------------------------------------------
void __fastcall TFrgPropDlg::GetLnkInfActionUpdate(TObject *Sender)
{
	if (!mltSelected)
		((TAction*)Sender)->Enabled = !LinkNameEdit->Text.IsEmpty()? (LinkNameEdit->Text[1]!='#') : false;
	else
		((TAction*)Sender)->Enabled = false;
}
//---------------------------------------------------------------------------

