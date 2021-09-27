//----------------------------------------------------------------------//
//	断片一覧															//
//																		//
//----------------------------------------------------------------------//
#include "EnvData.h"
#include "Main.h"
#include "PropDlg.h"
#include "FrgGrid.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFragGrid *FragGrid;

//---------------------------------------------------------------------------
__fastcall TFragGrid::TFragGrid(TComponent* Owner) : TForm(Owner)
{
	FS = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FormCreate(TObject *Sender)
{
	FrgList = new TFragList();
	SelSkip = DblSkip = false;
	SortSkip   = false;
	SortModeEx = SRTMD_NONE;
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FormShow(TObject *Sender)
{
	//位置・サイズを復元
	EV->load_pos_info("FragList", (TForm*)this,
		(Screen->Width - 600)/2, (Screen->Height - 400)/2, 600, 400);

	//一覧のカラム幅を設定
	TStringGrid *gp = FrgLstGrid;
	gp->Font->Assign(EV->flstFont);
	gp->DefaultRowHeight = get_FontHeightP(EV->flstFont, EV->FLstInterLine + 12);

	UnicodeString sct = "FragList";
	UnicodeString key = "ColWidth";
	gp->ColCount = 7;
	gp->RowCount = 11;
	str_to_GridHeadder(gp, "グループ|種類|テキスト|作成日時|更新日時|リンク先|ID");
	gp->ColWidths[FRGRD_COL_GROUP] = IniFile->ReadInteger(sct, key + IntToStr(FRGRD_COL_GROUP), 70);
	gp->ColWidths[FRGRD_COL_STYLE] = IniFile->ReadInteger(sct, key + IntToStr(FRGRD_COL_STYLE), 50);
	gp->ColWidths[FRGRD_COL_TEXT]  = IniFile->ReadInteger(sct, key + IntToStr(FRGRD_COL_TEXT), 200);
	gp->ColWidths[FRGRD_COL_CRE_T] = IniFile->ReadInteger(sct, key + IntToStr(FRGRD_COL_CRE_T),100);
	gp->ColWidths[FRGRD_COL_ACC_T] = IniFile->ReadInteger(sct, key + IntToStr(FRGRD_COL_ACC_T),100);
	gp->ColWidths[FRGRD_COL_LINK]  = IniFile->ReadInteger(sct, key + IntToStr(FRGRD_COL_LINK), 120);
	gp->ColWidths[FRGRD_COL_ID]    = IniFile->ReadInteger(sct, key + IntToStr(FRGRD_COL_ID),    50);

	SortMode   = IniFile->ReadInteger("FragList", "SortMode", 0);
	SortModeEx = SRTMD_NONE;
	SortSkip   = false;

	GridUpdate(true);

	PopupMenu = IdeaFragMainForm->EditPopupMenu2;
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FormClose(TObject *Sender, TCloseAction &Action)
{
	//位置・サイズを保存
	EV->save_pos_info("FragList", (TForm*)this);
	//一覧のカラム幅を保存
	for (int i=0; i<FrgLstGrid->ColCount; i++)
		IniFile->WriteInteger("FragList", "ColWidth" + IntToStr(i), FrgLstGrid->ColWidths[i]);

	IniFile->WriteInteger("FragList","SortMode",	SortMode);
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FormDestroy(TObject *Sender)
{
	delete FrgList;
}

//---------------------------------------------------------------------------
void __fastcall TFragGrid::FormActivate(TObject *Sender)
{
	if (FrgLstGrid->Focused()) {
		ListPanel->Color = clHighlight;
	}
	else if (DockPanel->DockClientCount>0) {
		TWinControl *cp = (TWinControl*)DockPanel->DockClients[0];
		if (cp->Visible) {
			cp->SetFocus();
			DockPanel->Color = clHighlight;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FormDeactivate(TObject *Sender)
{
	DockPanel->Color = Color;
	ListPanel->Color = Color;
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FrgLstGridEnter(TObject *Sender)
{
	ListPanel->Color = clHighlight;
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FrgLstGridExit(TObject *Sender)
{
	ListPanel->Color = Color;
}

//---------------------------------------------------------------------------
//一覧を更新
//---------------------------------------------------------------------------
void __fastcall TFragGrid::GridUpdate(bool updsw)
{
	if (EV->TimerLockCnt>0 || !Visible) return;

	TStringGrid *gp = FrgLstGrid;

	FS = IdeaFragMainForm->CurFS;
	bool empty = !FS? true : (FS->FrgList->Count==0);
	if (empty) {
		FrgList->Clear();
		gp->RowCount = 2;
		gp->Repaint();
		return;
	}

	if (!updsw && !EV->reqFrgLst) return;
	EV->reqFrgLst = false;

	if (!SortSkip || SortModeEx!=SRTMD_NONE) {
		FrgList->Clear();
		for (int i=0; i<FS->FrgList->Count; i++) {
			Fragment *fp = FS->FrgList->Items[i];
			if (!fp->Visible) continue;
			if (fp->style==fgsJunction) continue;
			FrgList->Add(fp);
		}
		//ソート
		if (SortModeEx!=-1) {
			switch (SortModeEx) {
			case SRTMD_SELECT: FrgList->Sort(FrgCmp_Select);	 break;
			}
		}
		else {
			switch (SortMode) {
			case FRGRD_COL_GROUP: FrgList->Sort(FrgCmp_GroupEx); break;
			case FRGRD_COL_STYLE: FrgList->Sort(FrgCmp_Style);	 break;
			case FRGRD_COL_TEXT:  FrgList->Sort(FrgCmp_Text);	 break;
			case FRGRD_COL_CRE_T: FrgList->Sort(FrgCmp_CreTime); break;
			case FRGRD_COL_ACC_T: FrgList->Sort(FrgCmp_ModTime); break;
			case FRGRD_COL_LINK:  FrgList->Sort(FrgCmp_Link);	 break;
			case FRGRD_COL_ID:    FrgList->Sort(FrgCmp_ID);	 	 break;
			}
		}
		gp->RowCount = FrgList->Count + 1;
	}

	for (int i=0,rn=1; i<FrgList->Count; i++,rn++) {
		Fragment *fp = FrgList->Items[i];
		UnicodeString tmpstr;
		for (int j=0; j<MAX_IN_GRP; j++) {
			if (fp->group.stk[j]==-1) break;
			if (j>0) tmpstr.UCAT_TSTR(",");
			if (fp->group.stk[j] == 0)
				tmpstr.UCAT_TSTR("無所属");
			else
				tmpstr.cat_sprintf(_T("%d"), fp->group.stk[j]);
		}
		gp->Cells[FRGRD_COL_GROUP][rn] = tmpstr;
		switch (fp->style) {
		case fgsPlate:	tmpstr = "表札"; break;
		case fgsSymbol:	tmpstr = "標識"; break;
		case fgsImage:	tmpstr = "画像"; break;
		default:		tmpstr = "断片"; if (!fp->LinkName.IsEmpty()) tmpstr.UCAT_TSTR("→");
		}
		gp->Cells[FRGRD_COL_STYLE][rn] = tmpstr;
		gp->Cells[FRGRD_COL_TEXT][rn]  = fp->TextStr;
		DateTimeToString(tmpstr, "yy/mm/dd hh:nn", fp->CreTime);
		gp->Cells[FRGRD_COL_CRE_T][rn] = tmpstr;
		DateTimeToString(tmpstr, "yy/mm/dd hh:nn", fp->ModTime);
		gp->Cells[FRGRD_COL_ACC_T][rn] = tmpstr;
		gp->Cells[FRGRD_COL_LINK][rn]  = fp->LinkName;
		gp->Cells[FRGRD_COL_ID][rn]    = IntToStr(fp->ID);
	}

	if (SortModeEx==SRTMD_SELECT) gp->TopRow = 1;
	Fragment *fp = FS->get_sel_single();
	if (fp) {
		int idx = FrgList->IndexOf(fp);
		if (idx!=-1) gp->Row = idx + 1;
	}

	gp->Repaint();
}
//---------------------------------------------------------------------------
//グループソートモードの時に指定グループを先頭行へ
//---------------------------------------------------------------------------
void __fastcall TFragGrid::SetGroupTop(int gn)
{
	if (SortMode!=FRGRD_COL_GROUP) return;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (fp->group.stk[0]==gn) {
			FrgLstGrid->TopRow = i + 1;
			FrgLstGrid->Row    = i + 1;
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::SetFrgIndex(Fragment *fp)
{
	int idx = FrgList->IndexOf(fp);
	if (idx!=-1) FrgLstGrid->Row = idx + 1;
}

//---------------------------------------------------------------------------
//一覧のセルを描画
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FrgLstGridDrawCell(TObject *Sender, int ACol,
	int ARow, TRect &Rect, TGridDrawState State)
{
	TStringGrid *gp = FrgLstGrid;
	TCanvas *cv = gp->Canvas;
	cv->Font->Assign(EV->flstFont);
	TRect rc = Rect;
	int x = Rect.Left + 4;
	int y = Rect.Top + (gp->DefaultRowHeight - get_FontHeight(cv))/2;
	UnicodeString cellstr = gp->Cells[ACol][ARow];

	//ヘッダ
	if (ARow==0) {
		cv->Brush->Color = clBtnFace;
		cv->Font->Color  = clBtnText;
		if (ACol==SortMode)
			cv->Font->Style = cv->Font->Style << fsBold;
		else
			cv->Font->Style = cv->Font->Style >> fsBold;
		cv->FillRect(Rect);
		cv->TextRect(Rect, x, y, cellstr);
		::DrawEdge(cv->Handle, &rc, BDR_RAISEDINNER, BF_RECT);
	}
	//セル
	else {
		int idx = ARow - 1;
		if (idx<FrgList->Count) {
			Fragment *fp = FrgList->Items[idx];
			cv->Font->Style = cv->Font->Style >> fsBold;
			if (ACol<FRGRD_COL_CRE_T) {
				switch (fp->style) {
				case fgsSymbol:
					cv->Font->Color	  = fp->Selected? EV->col_selFG : fp->colFG;
					cv->Brush->Color  = (FS->col_backgr==clNone)? EV->col_backgr : FS->col_backgr;
					break;
				case fgsImage:
					cv->Font->Color	  = fp->Selected? EV->col_selFG : EV->col_defFG;
					cv->Brush->Color  = fp->Selected? EV->col_selBG : EV->col_defBG;
					break;
				default:
					if (fp->style==fgsPlate && (ACol==FRGRD_COL_GROUP || ACol==FRGRD_COL_TEXT))
						cv->Font->Style = cv->Font->Style << fsBold;
					cv->Font->Color  = fp->Selected? EV->col_selFG : fp->colFG;
					cv->Brush->Color = fp->Selected? EV->col_selBG : fp->colBG;
				}
			}
			else {
				cv->Font->Color  = fp->Selected? EV->col_selFG : clWindowText;
				cv->Brush->Color = fp->Selected? EV->col_selBG : clWindow;
			}
			cv->FillRect(Rect);

			if (ACol==FRGRD_COL_GROUP) {
				cv->TextRect(Rect, x + 14, y, cellstr);
				TRect grc = Rect;
				if (fp->group.stk[0]>0) {
					//グループ枠の色を表示
					grc.Right = grc.Left + 6;
					cv->Brush->Color = FS->GInfs->Items[fp->group.stk[0]].gfp->Pen->Color;
					cv->FillRect(grc);
					grc.Left += 6;
				}
				//選択時に背景色を少し表示
				if (fp->Selected) {
					grc.Right = Rect.Left + 12;
					cv->Brush->Color = fp->colBG;
					cv->FillRect(grc);
				}
			}
			else {
				cv->TextRect(Rect, x, y, cellstr);
			}

			if (fp->style==fgsPlate) {
				UINT bdr = 0;
				if (ACol>=0 && ACol<=FRGRD_COL_TEXT) bdr = BF_TOP|BF_BOTTOM;
				if (ACol==0) bdr |= BF_LEFT;
				if (ACol==FRGRD_COL_TEXT) 	bdr |= BF_RIGHT;
				if (bdr!=0) ::DrawEdge(cv->Handle, &rc, BDR_RAISEDINNER, bdr);
			}
			if (fp->Noticed && ACol==FRGRD_COL_TEXT) {
				cv->Brush->Color = EV->col_selRct;
				InflateRect(&rc, -1, -1);
				cv->FrameRect(rc);
			}
		}
		else {
			cv->Brush->Color = clAppWorkSpace;
			cv->FillRect(Rect);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FrgLstGridDblClick(TObject *Sender)
{
	FragSet *cfs = IdeaFragMainForm->CurFS;
	if (FS!=cfs) GridUpdate(true);
	if (!FS) return;

	if (!DblSkip) {
		//ジャンプ
		if (is_KeyDown(VK_SHIFT))
			FS->jump_link(FS->get_sel_single());
		//プロパティ
		else 
			IdeaFragMainForm->EditProperty->Execute();
		SelSkip = true;	//DblClick後の選択を回避
	}
	DblSkip = false;
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FrgLstGridMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TStringGrid *gp = FrgLstGrid;
	FragSet *cfs = IdeaFragMainForm->CurFS;
	if (FS!=cfs) GridUpdate(true);
	if (!FS) return;

	if (Button==mbLeft) {
		if (SortModeEx!=SRTMD_NONE) {
			SortModeEx = SRTMD_NONE;
			SortSkip = true;
		}

		//ヘッダ上か?
		int idx = -1;
		for (int i=0; i<gp->ColCount; i++) {
			TRect rc = gp->CellRect(i, 0);
			if (rc.PtInRect(Point(X, Y))) {
				idx = i; break;
			}
		}
		//ソート
		if (idx!=-1) {
			if (SortSkip || SortMode!=idx) {
				EV->FLstSortDir = 1;
				SortMode = idx;
				SortSkip = false;
			}
			else {
				EV->FLstSortDir *= -1;
			}
			GridUpdate(true);
			DblSkip = true;

		}
		//セル上か?
		else if (!SelSkip) {
			idx = -1;
			for (int i=0; i<FrgList->Count; i++) {
				TRect rc = gp->CellRect(0, i + 1);
				rc.right = rc.left + gp->ClientWidth;
				if (rc.PtInRect(Point(X, Y))) {
					idx = i; break;
				}
			}
			if (idx!=-1) {
				//選択
				Fragment *fp = FrgList->Items[idx];
				if (Shift.Contains(ssCtrl))
					FS->group_select(fp->group.stk[0], true);
				else
					FS->one_select_center(fp);
				FS->GInfs->repaint_frame(true);
			}
			gp->Repaint();
			DblSkip = false;
		}
		else {
			SelSkip = DblSkip = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::FrgLstGridKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	TStringGrid *gp = FrgLstGrid;

	if (Key==VK_ESCAPE) {
		IdeaFragMainForm->SetFocus();
	}
	else {
		FragSet *cfs = IdeaFragMainForm->CurFS;
		if (FS!=cfs) GridUpdate(true);	if (!FS) return;

		int idx = -1;
		for (int i=0; i<FrgList->Count; i++) {
			if (FrgList->Items[i]->Selected) {
				idx = i; break;
			}
		}

		switch (Key) {
		case VK_UP: case VK_DOWN:
			if (Key==VK_UP) {
				if (idx>0) {
					idx--;
					gp->Row = idx + 1;
				}
			}
			else {
				if (idx<(FrgList->Count - 1)) {
					idx++;
					gp->Row = idx + 1;
				}
			}
			if (idx!=-1) {
				FS->one_select_center(FrgList->Items[idx]);
				gp->Repaint();
			}
			Key = 0;
			break;

		case VK_LEFT:
			//フォーカスをグループリストへ
			if (!IdeaFragMainForm->GroupListBox->Visible) break;
			if (IdeaFragMainForm->GroupListBox->Floating) break;
			if (IdeaFragMainForm->GroupListBox->HostDockSite!=DockPanel) break;
			IdeaFragMainForm->GroupListBox->SetFocus();
			Key = 0;
			break;

		case VK_RETURN:
			//ジャンプ
			if (is_KeyDown(VK_SHIFT))
				FS->jump_link(FS->get_sel_single());
			//プロパティ
			else if (!FS->read_only && FS->SelList->Count>0)
				IdeaFragMainForm->EditProperty->Execute();
			break;
		}
	}
}

//---------------------------------------------------------------------------
//ドッキング制限
//---------------------------------------------------------------------------
void __fastcall TFragGrid::DockPanelDockOver(TObject *Sender,
	TDragDockObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	if (Source->Control==IdeaFragMainForm->GroupListBox) {
		TControl *cp = (TControl*)Sender;
		if (State==dsDragEnter)
			cp->Width = Source->Control->Width;
		else if (State==dsDragLeave)
			cp->Width = 0;
	}
	else {
		Accept = false;
	}
}
//---------------------------------------------------------------------------
//グループリストがドッキングした
//---------------------------------------------------------------------------
void __fastcall TFragGrid::DockPanelDockDrop(TObject *Sender,
	TDragDockObject *Source, int X, int Y)
{
	TPanel *pp = (TPanel*)Sender;
	pp->BorderWidth = 1;
	pp->Width = IniFile->ReadInteger("GroupList","Width",120);
	IdeaFragMainForm->SaveGrpLstPos();
	EV->LastDockSite = pp;
	Caption = "グループ／断片リスト";
}
//---------------------------------------------------------------------------
//グループリストがアンドックされた
//---------------------------------------------------------------------------
void __fastcall TFragGrid::DockPanelUnDock(TObject *Sender,
	TControl *Client, TWinControl *NewTarget, bool &Allow)
{
	TPanel *pp = (TPanel*)Sender;
	pp->BorderWidth = 0;
	pp->Constraints->MinWidth = 0;
	pp->Width = 1;
	pp->Color = Color;
	Caption = "断片リスト";
}
//---------------------------------------------------------------------------
void __fastcall TFragGrid::DockPanelResize(TObject *Sender)
{
	ListSplitter->Enabled = (((TPanel*)Sender)->Width>1);
	ListSplitter->Beveled = ListSplitter->Enabled;
}
//---------------------------------------------------------------------------

