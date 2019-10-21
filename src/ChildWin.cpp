//----------------------------------------------------------------------//
//	子ウィンドウ														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include <tchar.h>
#include "UserFunc.h"
#include "EnvData.h"
#include "Main.h"
#include "PropDlg.h"
#include "MapView.h"
#include "ChildWin.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"

//---------------------------------------------------------------------
__fastcall TMDIChild::TMDIChild(TComponent *Owner) : TForm(Owner)
{
}
//---------------------------------------------------------------------
void __fastcall TMDIChild::FormCreate(TObject *Sender)
{
	FS = new FragSet(this);		//断片セット作成
	FS->VscrBar->Range = FS->scr_hi + 20;
	FS->HscrBar->Range = FS->scr_wd + 20;

	Selecting  = false;
	SelSkip    = DblSkip = false;
	cur_fp	   = NULL;
	ScrMoving  = false;
	moved	   = false;

	Color = EV->col_backgr;

	VertScrollBar->Increment = 16;
	VertScrollBar->Tracking  = true;
	HorzScrollBar->Increment = 16;
	HorzScrollBar->Tracking  = true;

	ScrollTimer->Interval = 100;		//***
	ScrollTimer->Enabled  = false;
}

//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ((EV->RstScrPos && (FS->LastHPos!=FS->HscrBar->Position || FS->LastVPos!=FS->VscrBar->Position))
		&& !FS->file_name.IsEmpty() && !FS->read_only && !FS->modify)
	{
		//スクロール以外変更無しの場合タイムスタンプは変更しないで保存
		FS->modify	  = true;
		FS->forcesave = true;
		FS->save_file(FS->file_name, true);
	}
	else if (!FS->read_only && FS->modify) {
		if (!FS->forcesave) {
			UnicodeString  tmpstr = "[";
			if (!FS->file_name.IsEmpty())
				tmpstr += FS->file_name;
			else
				tmpstr.UCAT_TSTR("無題");
			tmpstr.UCAT_TSTR("]変更内容を保存しますか");
			switch (msgbox_Y_N_C(tmpstr)) {
			case IDYES:		FS->forcesave = true; break;
			case IDCANCEL:	CanClose = false;	  break;
			}
		}
		if (FS->forcesave) {
			//保存
			if (!FS->file_name.IsEmpty()) {
				FS->save_file(FS->file_name);
			}
			else {
				SaveDialog->FileName="*.IFD";
				if (SaveDialog->Execute()) {
					FS->file_name = SaveDialog->FileName;
					FS->save_file(FS->file_name);
				}
				else {
					FS->forcesave = false;
					CanClose	  = false;
				}
			}
		}
	}

	//ファイル履歴を更新
	if (CanClose && !FS->file_name.IsEmpty()) EV->add_file_history(FS->file_name);
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
	ScrollTimer->Enabled = false;

	Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormDestroy(TObject *Sender)
{
	EV->TimerLockCnt++;
		delete FS;	FS = NULL;
	EV->TimerLockCnt--;

	MapViewer->MapUpdate(NULL);
}

//---------------------------------------------------------------------------
//画面の更新
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormPaint(TObject *Sender)
{
	EV->TimerLockCnt++;
		FS->all_update();
	EV->TimerLockCnt--;
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormResize(TObject *Sender)
{
	MapViewer->MapUpdate(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormActivate(TObject *Sender)
{
	//グループリストからフォーカスを奪う
	IdeaFragMainForm->DefocusControl(IdeaFragMainForm->GroupListBox, true);
	IdeaFragMainForm->SetFocus();

	if		(IdeaFragMainForm->EditGroupBtn->Down) Cursor = crGroupTool;
	else if (IdeaFragMainForm->EditGLineBtn->Down) Cursor = crLineTool;
	else if (IdeaFragMainForm->EditBrushBtn->Down) Cursor = crBrushTool;
	else										   Cursor = crDefault;

	EV->reqUpdate = true;

	ScrollTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormDeactivate(TObject *Sender)
{
	ScrollTimer->Enabled = false;
	PopPosShape->Visible = false;
}

//---------------------------------------------------------------------------
// 最大化情報を補正(タイトルバーとメニューを隠すオプション用)
//---------------------------------------------------------------------------
void __fastcall TMDIChild::WmGetMinMaxInfo(TWMGetMinMaxInfo &Msg)
{
	TForm::Dispatch(&Msg);
	if (EV->HideTitleMenu)
		Msg.MinMaxInfo->ptMaxTrackSize.y = IdeaFragMainForm->Height;
}


//---------------------------------------------------------------------------
// マウス操作
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (!SelSkip) {
		//グループリストからフォーカスを奪う
		IdeaFragMainForm->DefocusControl(IdeaFragMainForm->GroupListBox, true);
		IdeaFragMainForm->SetFocus();
	}

	lastP = Point(X, Y);

	if (Shift.Contains(ssAlt) && Button==mbLeft) {
		if (!SelSkip) {
			//画面移動開始
			Screen->Cursor = EV->crFragMove;
			ScrMoving = true;
		}
		else {
			SelSkip = false;
		}
	}
	else {
		int hx = X, hy = Y;
		if (IdeaFragMainForm->EditBrushBtn->Down) { hx += 4; hy += 28; }

		Fragment *cfp = FS->frag_from_pos(hx, hy);
		int       cgn = FS->GInfs->group_from_pos(hx, hy);

		ArrowLine *ap = FS->CLineList->lp_from_pos(hx, hy);
		bool jflag = cfp? (cfp->style==fgsJunction) : false;
		if (!jflag && ap!=FS->CurLp) {
			FS->CurLp = ap;
			FS->update_group_frame();
			FS->update_frg_line();
			FS->update_fg_line();
		}

		if (!cfp) {
			int lst_gfrm = FS->GInfs->cur_gfrm;
			FS->GInfs->cur_gfrm = FS->GInfs->gfrm_from_pos(hx, hy);
			if (FS->GInfs->cur_gfrm!=lst_gfrm) {
				FS->GInfs->Items[lst_gfrm].RepaintFrm = true;
				FS->GInfs->repaint_frame(true);
			}
		}

		//◆関係線モード◆
		if (IdeaFragMainForm->EditGLineBtn->Down) {
			PopupMenu = IdeaFragMainForm->GLinePopupMenu;
			if (Button==mbLeft) {
				FromFp = ToFp = NULL;
				FromGn = ToGn = 0;
				if (FS->CurLp) {
					//既存関係線上
					if (FS->CurLp->pt_in_frm(X, Y)) {
						//始点上
						switch (FS->CurLp->tType) {
						case TDIR_F2F: case TDIR_G2F:
							ToFp = FS->FrgList->id_to_fp(FS->CurLp->toTag);	break;
						default:
							ToGn = ap->toTag;
						}
						orgX = FS->CurLp->vx1;
						orgY = FS->CurLp->vy1;
					}
					//終点上
					else if (FS->CurLp->pt_in_to(X, Y)) {
						switch (FS->CurLp->tType) {
						case TDIR_F2F: case TDIR_F2G:
							FromFp = FS->FrgList->id_to_fp(FS->CurLp->fromTag);	break;
						default:
							FromGn = ap->fromTag;
						}
						orgX = FS->CurLp->vx0;
						orgY = FS->CurLp->vy0;
					}
					if (FromFp || ToFp || FromGn>0 || ToGn>0) {
						FS->all_select(false);
						FS->CLineMode = CLMODE_RECON;
						IdeaFragMainForm->StatusBar->Panels->Items[2]->Text = "関係線の接続変更中...";
					}
				}
				else {
					orgX = X;
					orgY = Y;
					FS->all_select(false);
					if (cfp) {
						if (cfp->style==fgsPlate && cfp->OnGfrm) cfp = NULL;
					}
					//断片関係線開始
					if (cfp) {
						FromFp = cfp;
						FS->CLineMode = CLMODE_CONCT;
						IdeaFragMainForm->StatusBar->Panels->Items[2]->Text = "断片から関係線を設定中...";
					}
					//グループ関係線開始
					else if (cgn>0) {
						FromGn = cgn;
						FS->CLineMode = CLMODE_CONCT;
						IdeaFragMainForm->StatusBar->Panels->Items[2]->Text = "グループから関係線を設定中...";
					}
					//端点から関係線開始
					else {
						int xx = X + HorzScrollBar->Position;
						int yy = Y + VertScrollBar->Position;
						cfp = FS->put_junction(xx, yy);
						if (cfp) {
							FromFp = cfp;
							FS->CLineMode = CLMODE_CONCT;
							IdeaFragMainForm->StatusBar->Panels->Items[2]->Text = "端点から関係線を設定中...";
						}
					}
				}
			}
		}
		//◆色塗りモード◆
		else if (IdeaFragMainForm->EditBrushBtn->Down) {
			if (cfp) {
				//断片上
				PopupMenu = NULL;
				//断片の背景色を設定
				if (Button==mbLeft) {
					FS->push_all();
					Fragment *np = NULL;
					if (cfp->Selected) {
						for (int i=0; i<FS->SelList->Count; i++) {
							Fragment *fp = FS->SelList->Items[i];
							fp->colBG	 = EV->col_Brush;
							fp->ModTime  = Now();
							fp->Changed  = true;
							fp->show_frag();
							if (fp->style==fgsPlate) np = fp;
						}
					}
					else {
						cfp->colBG	 = EV->col_Brush;
						cfp->ModTime = Now();
						cfp->Changed = true;
						cfp->show_frag();
						if (cfp->style==fgsPlate) np = cfp;
					}
					FS->all_select(false);
					//グループ枠上の表札を更新
					if (np && EV->show_GroupFrame && EV->PlateOnGfrm) {
						for (int i=0; i<MAX_IN_GRP; i++) {
							int gn = np->group.stk[i]; if (gn==-1) break;
							if (gn>0) FS->GInfs->Items[gn].RepaintFrm = true;
						}
						FS->GInfs->repaint_frame();
					}
					FS->modify = true;
				}
				//断片の背景色を取り込みむ
				else if (Button==mbRight) {
					Cursor = crSpuitTool;
					EV->col_Brush = cfp->colBG;
					IdeaFragMainForm->BrushPanel->Color = EV->col_Brush;
				}
			}
			//関係線上
			else if (FS->CurLp) {
				PopupMenu = NULL;
				//関係線の個別色を設定
				if (Button==mbLeft) {
					FS->push_all();
					FS->CurLp->lColor = EV->col_Brush;
					FS->CurLp = NULL;
					FS->update_frg_line();
					FS->update_fg_line();
					FS->update_group_frame();
					FS->modify = true;
				}
				//関係線の色を取り込む
				else if (Button==mbRight) {
					Cursor = crSpuitTool;
					EV->col_Brush = (FS->CurLp->lColor!=clNone)?
						FS->CurLp->lColor : FS->CurLp->lCurCol;
					IdeaFragMainForm->BrushPanel->Color = EV->col_Brush;
				}
			}
			//グループ枠上
			else if (cgn>0) {
				PopupMenu = NULL;
				grp_inf *gp = &FS->GInfs->Items[cgn];
				//グループ枠個別色を設定
				if (Button==mbLeft) {
					TColor fc = EV->col_Brush;
					if (gp->gfp->Pen->Color != fc) {
						FS->push_all();
						FS->GInfs->cur_gfrm = 0;
						gp->gfp->Pen->Color = fc;
						if (fc==FS->col_GrpFrm || fc==EV->col_GrpFrm) fc = clNone;
						gp->col_GrpFrm = fc;
						gp->RepaintFrm = true;
						gp->gfp->Repaint();
						IdeaFragMainForm->GroupListBox->Repaint();
						FS->modify = true;
					}
				}
				//グループ枠色を取り込む
				else if (Button==mbRight) {
					Cursor = crSpuitTool;
					TColor fc;
					if (gp->col_GrpFrm==clNone)
  						fc = (FS->col_GrpFrm==clNone)? EV->col_GrpFrm : FS->col_GrpFrm;
					else
						fc = gp->col_GrpFrm;
					EV->col_Brush = fc;
					IdeaFragMainForm->BrushPanel->Color = fc;
				}

			}
			else {
				PopupMenu = IdeaFragMainForm->BrushPopupMenu;
				if (Button==mbLeft) FS->all_select(false);
			}
		}
		else {
			//◆グループ化モード◆
			if (IdeaFragMainForm->EditGroupBtn->Down) {
				PopupMenu = IdeaFragMainForm->GFrmPopupMenu;
				IdeaFragMainForm->StatusBar->Panels->Items[2]->Text
					= "グループ化の対象を選択中...";
			}
			//◆通常編集モード
			else {
				if (FS->CurLp)
					PopupMenu = IdeaFragMainForm->GLinePopupMenu;
				else if (FS->GInfs->cur_gfrm>0)
					PopupMenu = IdeaFragMainForm->GFrmPopupMenu;
				else
					PopupMenu = IdeaFragMainForm->EditPopupMenu;
			}
			//断片上
			if (cfp) {
				FS->bring_to_front(cfp);
				FS->FragMouseDown(cfp, Button, Shift, X, Y);
				DblSkip = Shift.Contains(ssCtrl);
			}
			//背景
			else if (Button==mbLeft) {
				if (!Shift.Contains(ssShift) && FS->SelList->Count>0 && FS->GInfs->cur_gfrm==0)
					FS->all_select(false);
				if (!SelSkip) {
					//枠選択開始
					Selecting = true;
					SelRect   = Rect(X, Y, X, Y);
					orgX	  = X;
					orgY	  = Y;
				}
				else {
					SelSkip = false;
				}
			}
		}
	}
	moved = false;
}
//---------------------------------------------------------------------
void __fastcall TMDIChild::FormMouseMove(TObject *Sender,
	  TShiftState Shift, int X, int Y)
{
	if (!ScrMoving && EV->RDragScroll && Shift.Contains(ssRight)) {
		PopupMenu = NULL;
		Screen->Cursor = EV->crFragMove;
		ScrMoving = true;
	}

	//画面移動中
	if (ScrMoving) {
		HorzScrollBar->Position += (lastP.x - X);
		VertScrollBar->Position += (lastP.y - Y);
	}
	else {
		if (IdeaFragMainForm->EditGroupBtn->Down) {
			if (Cursor!=crGroupTool) Cursor = crGroupTool;
		}
		else if (IdeaFragMainForm->EditGLineBtn->Down) {
			if (Cursor!=crLineTool) Cursor = crLineTool;
		}
		else if (IdeaFragMainForm->EditBrushBtn->Down) {
			if (Cursor!=crBrushTool && Cursor!=crSpuitTool) Cursor = crBrushTool;
		}

		//枠選択中
		if (Selecting) {
			Canvas->Pen->Width	 = 1;
			Canvas->Pen->Mode	 = pmNot;
			Canvas->Pen->Style   = psSolid;
			Canvas->Brush->Style = bsClear;
			Canvas->Rectangle(SelRect.Left, SelRect.Top, SelRect.Right, SelRect.Bottom);
			set_rect_ex(&SelRect, orgX, orgY, X, Y);
			Canvas->Rectangle(SelRect.Left, SelRect.Top, SelRect.Right, SelRect.Bottom);
			Canvas->Pen->Mode	 = pmCopy;
			Canvas->Brush->Style = bsSolid;

			//画面右・下端でのスクロール処理
			int  dx = X - lastP.x;
			int  dy = Y - lastP.y;
			int  x_scr_area = ClientWidth  - 32;
			int  y_scr_area = ClientHeight - 32;
			bool r_scrolled = (X>x_scr_area && lastP.x>x_scr_area && dx>0);
			bool b_scrolled = (Y>y_scr_area && lastP.y>y_scr_area && dy>0);
			if		(r_scrolled) ScrollProc(16, 0);
			else if (b_scrolled) ScrollProc(0, 16);

			//画面左・上端でのスクロール処理
			bool l_scrolled = (HorzScrollBar->Position>0 && X<32 && lastP.x<32 && dx<0);
			bool t_scrolled = (VertScrollBar->Position>0 && Y<32 && lastP.y<32 && dy<0);
			if		(l_scrolled) ScrollProc(-16, 0);
			else if (t_scrolled) ScrollProc(0, -16);

			if (EV->MapViewSelRect) MapViewer->MapUpdate(NULL);
		}
		//関係線設定中／接続変更中
		else if (FS->CLineMode==CLMODE_CONCT || FS->CLineMode==CLMODE_RECON) {
			if (!moved) FS->push_all();
			Canvas->Pen->Color = Color;
			if (FS->CLineMode==CLMODE_RECON && FS->CurLp) {
				Canvas->Pen->Width = abs(FS->CurLp->lWidth);
				Canvas->Pen->Style = (FS->CurLp->lWidth==-1)? psDot : psSolid;
			}
			else {
				Canvas->Pen->Width = abs(EV->CurGLineWidth);
				Canvas->Pen->Style = (EV->CurGLineWidth==-1)? psDot : psSolid;
			}
			Canvas->Brush->Style = bsClear;
			Canvas->MoveTo(orgX, orgY);
			Canvas->LineTo(lastP.x, lastP.y);
			TColor c = (FromGn>0)? FS->col_GrpLine : FS->col_FrgLine;
			if (c==clNone) c = (FromGn>0)? EV->col_GrpLine : EV->col_FrgLine;
			Canvas->Pen->Color = c;
			Canvas->MoveTo(orgX, orgY);
			Canvas->LineTo(X, Y);
			int gn = FS->GInfs->group_from_pos(X, Y);
			if (gn>0) FS->GInfs->Items[gn].RepaintFrm = true;

		}
		//その他
		else {
			Fragment *cfp = FS->frag_from_pos(X, Y);
			FS->FragMouseMove(cfp, Shift, X, Y);
			if (!FS->moving) {
				if (cfp) {
					if (cur_fp!= cfp) {
						cur_fp = cfp;
						//リンク先(画像の場合はテキスト)をヒント表示
						Application->CancelHint();
						UnicodeString tstr = ReplaceStr(cfp->TextStr, EV->CR_str, "\r\n");
						UnicodeString lnam = cfp->LinkName;
						if (EV->LinkRelPath) lnam = abs_to_reldir(lnam, FS->file_name);
						Hint = (cfp->style==fgsImage && !tstr.IsEmpty())? tstr : lnam;
						if (!Hint.IsEmpty()) {
							TMessage mm;
							mm.Msg		= WM_LBUTTONDOWN;
							mm.WParam	= MK_LBUTTON;
							mm.LParamHi = (Word)Y;
							mm.LParamLo = (Word)X;
							Application->HintMouseMessage(this, mm);
							mm.Msg	    = WM_LBUTTONUP;
							mm.WParam   = 0;
							Application->HintMouseMessage(this, mm);
						}
						else {
							Application->CancelHint();
						}
					}
				}
				else {
					cur_fp = NULL;
					Hint   = EmptyStr;
					Application->CancelHint();
				}
			}
			else {
				Application->CancelHint();
			}
		}
	}

	lastP = Point(X, Y);
	moved = true;

	UnicodeString msg;
	if (FS->scr_wd>0 && FS->scr_hi>0) {
		int xp = FS->HscrBar->Position + X;
		int yp = FS->VscrBar->Position + Y;
		msg.sprintf(_T("x:%u, y:%u (%u%%, %u%%)"), xp, yp, round_i(100.0*xp/FS->scr_wd), round_i(100.0*yp/FS->scr_hi));
	}
	IdeaFragMainForm->StatusBar->Panels->Items[2]->Text = msg;
}

//---------------------------------------------------------------------
void __fastcall TMDIChild::FormMouseUp(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	PopPosShape->Visible = false;

	//画面移動終了
	if (ScrMoving) {
		Screen->Cursor = crDefault;
		ScrMoving = false;
	}
	else {
		Fragment *fp = FS->frag_from_pos(X, Y);
		int gn;
		//◆関係線モード◆
		if (IdeaFragMainForm->EditGLineBtn->Down) {
			gn = FS->GInfs->group_from_pos(X, Y);
			if (FS->CLineMode==CLMODE_CONCT || FS->CLineMode==CLMODE_RECON) {
				Canvas->Pen->Color = Color;
				if (FS->CLineMode==CLMODE_RECON && FS->CurLp) {
					Canvas->Pen->Width = abs(FS->CurLp->lWidth);
					Canvas->Pen->Style = (FS->CurLp->lWidth==-1)? psDot : psSolid;
				}
				else {
					Canvas->Pen->Width = abs(EV->CurGLineWidth);
					Canvas->Pen->Style = (EV->CurGLineWidth==-1)? psDot : psSolid;
				}
				Canvas->Brush->Style = bsClear;
				Canvas->MoveTo(orgX, orgY);
				Canvas->LineTo(lastP.x, lastP.y);

				if (X!=orgX || Y!=orgY) {
					int xx0 = orgX + HorzScrollBar->Position;
					int yy0 = orgY + VertScrollBar->Position;
					int xx1	= X + HorzScrollBar->Position;
					int yy1	= Y + VertScrollBar->Position;

					if (FS->CLineMode==CLMODE_CONCT) {
						//関係線設定を終了
						//断片 →
						if (FromFp) {
							if (fp) {
								FS->set_frag_line(FromFp->ID, fp->ID);
							}
							// → グループ
							else if (gn>0 && FromFp->in_group(gn)==-1) {
								FS->set_fg_line(TDIR_F2G, FromFp->ID, gn);
							}
							// → 端点
							else {
								Fragment *jp = FS->put_junction(xx1, yy1);
								if (jp) FS->set_frag_line(FromFp->ID, jp->ID);
							}
						}
						//グループ →
						else if (FromGn>0) {
							if (fp) {
								// → 断片
								if (fp->in_group(FromGn)==-1) {
									FS->set_fg_line(TDIR_G2F, FromGn, fp->ID);
								}
								// >端点 → 断片
								else {
									Fragment *jp = FS->put_junction(xx0, yy0);
									if (jp) FS->set_frag_line(jp->ID, fp->ID);
								}
							}
							else if (gn>0) {
								// >端点 → 端点
								if (FromGn==gn) {
									Fragment *jp0 = FS->put_junction(xx0, yy0);
									Fragment *jp1 = FS->put_junction(xx1, yy1);
									if (jp0 && jp1) FS->set_frag_line(jp0->ID, jp1->ID);
								}
								// → 端点
								else if (FS->GInfs->compare_group(FromGn, gn)==-1) {
									Fragment *jp = FS->put_junction(xx1, yy1);
									if (jp) FS->set_fg_line(TDIR_G2F, FromGn, jp->ID);
								}
								// >端点 → グループ
								else if (FS->GInfs->compare_group(FromGn, gn)==1) {
									Fragment *jp = FS->put_junction(xx0, yy0);
									if (jp) FS->set_fg_line(TDIR_F2G, jp->ID, gn);
								}
								// → グループ
								else {
									FS->set_group_line(FromGn, gn);
								}
							}
							// → 端点
							else {
								Fragment *jp = FS->put_junction(xx1, yy1);
								if (jp) FS->set_fg_line(TDIR_G2F, FromGn, jp->ID);
							}
						}
					}
					//関係線の接続変更完了
					else {
						ArrowLine *ap = FS->CurLp;
						//断片 →
						if (FromFp) {
							// → 断片
							if (fp) {
								ap->tType	= TDIR_F2F;
								ap->toTag	= fp->ID;
							}
							// → グループ
							else if (gn>0 && FromFp->in_group(gn)==-1) {
								ap->tType	= TDIR_F2G;
								ap->toTag	= gn;
							}
							// → 端点
							else {
								Fragment *jp = FS->put_junction(xx1, yy1);
								if (jp) {
									ap->tType	= TDIR_F2F;
									ap->toTag	= jp->ID;
								}
							}
						}
						//グループ →
						else if (FromGn>0) {
							// → 断片
							if (fp) {
								ap->tType	= TDIR_G2F;
								ap->toTag	= fp->ID;
							}
							// → グループ
							else if (gn>0 && FS->GInfs->compare_group(FromGn, gn)!=-1) {
								ap->tType	= TDIR_G2G;
								ap->toTag	= gn;
							}
							// → 端点
							else {
								Fragment *jp = FS->put_junction(xx1, yy1);
								if (jp) {
									ap->tType	= TDIR_G2F;
									ap->toTag	= jp->ID;
								}
							}
						}
						//→ 断片
						else if (ToFp) {
							if (fp) {
								//断片 →
								ap->tType	= TDIR_F2F;
								ap->fromTag = fp->ID;
							}
							//グループ →
							else if (gn>0 && ToFp->in_group(gn)==-1) {
								ap->tType	= TDIR_G2F;
								ap->fromTag = gn;
							}
							//端点  →
							else {
								Fragment *jp = FS->put_junction(xx1, yy1);
								if (jp) {
									ap->tType	= TDIR_F2F;
									ap->fromTag = jp->ID;
								}
							}
						}
						//→ グループ
						else if (ToGn>0) {
							//断片 →
							if (fp) {
								ap->tType	= TDIR_F2G;
								ap->fromTag = fp->ID;
							}
							//グループ →
							else if (gn>0 && FS->GInfs->compare_group(ToGn, gn)!=-1) {
								ap->tType	= TDIR_G2G;
								ap->fromTag = gn;
							}
							//端点 →
							else {
								Fragment *jp = FS->put_junction(xx1, yy1);
								if (jp) {
									ap->tType	= TDIR_F2G;
									ap->fromTag = jp->ID;
								}
							}
						}
					}
				}
				FS->clr_iso_junction();
				FS->modify	  = true;
				FS->CLineMode = CLMODE_READY;
				FS->all_update();
			}
		}
		else if (IdeaFragMainForm->EditBrushBtn->Down) {
			Cursor = crBrushTool;
		}
		//枠選択終了
		else if (Selecting) {
			Canvas->Pen->Width	 = 1;
			Canvas->Pen->Mode	 = pmNot;
			Canvas->Brush->Style = bsClear;
			Canvas->Rectangle(SelRect.Left, SelRect.Top, SelRect.Right, SelRect.Bottom);
			Canvas->Pen->Mode	 = pmCopy;
			Canvas->Brush->Style = bsSolid;

			FS->rect_select(SelRect);

			if (IdeaFragMainForm->EditGroupBtn->Down) {
				//◆グループ化モード◆
				bool mflag = Shift.Contains(ssCtrl);
				if (EV->ChgGrpModeCtrl) mflag = !mflag;
				//多重
				if (mflag) {
					do {
						if (FS->SelList->Count==0) break;
						gn = FS->GInfs->get_empty_idx();  if (gn==-1) break;
						grp_inf *gp = &FS->GInfs->Items[gn];
						gp->LnWidth    = 0;
						gp->col_GrpFrm = clNone;
						FS->mov_sel_group(gn, true);
						gp->Visible = true;
						FS->all_select(false);
					} while (0);

				}
				//通常
				else {
					do {
						if (FS->SelList->Count==0) break;
						gn = FS->GInfs->get_sel_max();
						if (gn!=-1 && gn!=0) {
							//すでにひとつのグループになっているか?
							if (FS->GInfs->Items[gn].TotalSize == FS->SelList->Count) break;
						}
						else {
							gn = FS->GInfs->get_empty_idx();  if (gn==-1) break;
						}
						grp_inf *gp = &FS->GInfs->Items[gn];
						gp->LnWidth    = 0;
						gp->col_GrpFrm = clNone;
						FS->mov_sel_group(gn);
						gp->Visible = true;
						FS->all_select(false);
					} while (0);
				}
			}
			Selecting = false;
		}
		else {
			FS->FragMouseUp(fp, Button, Shift, X, Y);
		}

		IdeaFragMainForm->StatusBar->Panels->Items[2]->Text = EmptyStr;

		FS->CLineList->erase_line(TDIR_G2G);
		FS->update_group_frame();
		FS->GInfs->repaint_frame();

		MapViewer->MapUpdate(NULL);
	}
}

//---------------------------------------------------------------------
void __fastcall TMDIChild::FormDblClick(TObject *Sender)
{
	if (!IdeaFragMainForm->EditBrushBtn->Down && !Selecting && !DblSkip) {
		TPoint mp = ScreenToClient(Mouse->CursorPos);
		Fragment *fp = FS->frag_from_pos(mp.x, mp.y);
		//断片上
		if (fp) {
			FS->FragDblClick(fp);
		}
		else if (!FS->read_only) {
			//関係線上
			if (FS->CurLp) {
				UnicodeString inpstr = FS->CurLp->TextStr;
				if (InputQuery("関係線ラベル", EmptyStr, inpstr)) {
					FS->push_all();
					FS->CurLp->erase(FS->frg_owner);
					FS->CurLp->TextStr = inpstr;
					FS->update_frg_line();
					FS->update_group_frame();
					FS->update_fg_line();
					FS->modify	= true;
				}
			}
			//背景 (新規断片)
			else {
				FrgPropDlg->prpFS		= FS;
				FrgPropDlg->swNewFrag	= true;
				FrgPropDlg->swNewCurPos = true;	//クリック位置に作成
				FrgPropDlg->newPosX 	= mp.x + 1;
				FrgPropDlg->newPosY 	= mp.y + 1;
				FrgPropDlg->swNewGroup	= true;
				FrgPropDlg->newGrp		= FS->GInfs->gstk_from_pos(mp.x, mp.y, &FrgPropDlg->newGstk);
				FrgPropDlg->ShowModal();
				FS->moving = false;
				IdeaFragMainForm->CloseIME();
			}
		}
		SelSkip = true;	//DblClick後の枠選択・画面移動を回避
	}
	DblSkip = false;
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	int dx, dy;

	switch (Key) {
	//ウィンドウのスクロール
	case VK_UP:    case VK_DOWN:
	case VK_PRIOR: case VK_NEXT:
	case VK_LEFT:  case VK_RIGHT:
		dx = dy = 0;
		switch (Key) {
		case VK_UP:    dy = -16; break;
		case VK_DOWN:  dy = +16; break;
		case VK_PRIOR: dy = -64; break;
		case VK_NEXT:  dy = +64; break;
		case VK_LEFT:  dx = -16; break;
		case VK_RIGHT: dx = +16; break;
		}
		ScrollProc(dx, dy);
		break;

	//グループリストをアクティブ
	case VK_ESCAPE:
		if (IdeaFragMainForm->GroupListBox->Visible)
			IdeaFragMainForm->GroupListBox->SetFocus();
		break;

	//移動中にグループから引き上げ
	case VK_CONTROL:
		if (!FS->moving)  break;
		if (FS->movgroup) break;
		if (Shift.Contains(ssShift)) break;
		if		(Screen->Cursor==crHandGrabL) Screen->Cursor = crHandPickL;
		else if (Screen->Cursor==crHandGrabR) Screen->Cursor = crHandPickR;
		FS->pickuped = true;
		FS->update_grp_empty();
	 	FS->update_group_frame();
		MapViewer->MapUpdate(NULL);
		break;

	case VK_DELETE:
		//選択中関係線の削除
		if (FS->CurLp)
			IdeaFragMainForm->GLineDelItemClick(NULL);
		//選択中断片の削除
		else if (FS->SelList->Count>0)
			FS->del_sel_frag();
		break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch (Key) {
	//移動中にグループへ戻す
	case VK_CONTROL:
		if (!FS->moving)  break;
		FS->movgroup = FS->pickuped = false;
		if		(Screen->Cursor==crHandPickL) Screen->Cursor = crHandGrabL;
		else if (Screen->Cursor==crHandPickR) Screen->Cursor = crHandGrabR;
		FS->update_grp_empty();
	 	FS->update_group_frame();
		MapViewer->MapUpdate(NULL);
		break;
	}
}

//---------------------------------------------------------------------------
// スクロール処理
//---------------------------------------------------------------------------
void __fastcall TMDIChild::ScrollProc(int dx, int dy)
{
	if (dx==0 && dy==0) return;

	if (Selecting) {
		//選択枠を一旦消す
		Canvas->Pen->Width	 = 1;
		Canvas->Pen->Mode	 = pmNot;
		Canvas->Pen->Style   = psSolid;
		Canvas->Brush->Style = bsClear;
		Canvas->Rectangle(SelRect.Left, SelRect.Top, SelRect.Right, SelRect.Bottom);
		Canvas->Pen->Mode	 = pmCopy;
		Canvas->Brush->Style = bsSolid;
	}
	else if (IdeaFragMainForm->EditGLineBtn->Down
		&& (FS->CLineMode==CLMODE_CONCT || FS->CLineMode==CLMODE_RECON))
	{
		//関係線設定中なら線を消す
		Canvas->Pen->Color = Color;
		if (FS->CLineMode==CLMODE_RECON && FS->CurLp) {
			Canvas->Pen->Width = abs(FS->CurLp->lWidth);
			Canvas->Pen->Style = (FS->CurLp->lWidth==-1)? psDot : psSolid;
		}
		else {
			Canvas->Pen->Width = abs(EV->CurGLineWidth);
			Canvas->Pen->Style = (EV->CurGLineWidth==-1)? psDot : psSolid;
		}
		Canvas->Brush->Style = bsClear;
		Canvas->MoveTo(orgX, orgY);
		Canvas->LineTo(lastP.x, lastP.y);
	}

	//スクロール
	if (dx!=0) {
		int lx = HorzScrollBar->Position;
		HorzScrollBar->Position += dx;
		dx = (HorzScrollBar->Position - lx);
		orgX -= dx;
	}
	if (dy!=0) {
		int ly = VertScrollBar->Position;
		VertScrollBar->Position += dy;
		dy = (VertScrollBar->Position - ly);
		orgY -= dy;
	}

	TPoint mp = ScreenToClient(Mouse->CursorPos);
	//断片移動中
	if (FS->moving) {
		FS->lastP.x -= dx;
		FS->lastP.y -= dy;
		TShiftState sh;
		if (is_KeyDown(VK_SHIFT)) 	sh << ssShift;
		if (is_KeyDown(VK_CONTROL))	sh << ssCtrl;
		if (is_KeyDown(VK_MENU))	sh << ssAlt;
		FS->FragMouseMove(NULL, sh, mp.x, mp.y);
	}
	//選択枠を再描画
	else if (Selecting) {
		Repaint();
		set_rect_ex(&SelRect, orgX, orgY, mp.x, mp.y);
		Canvas->Pen->Mode	 = pmNot;
		Canvas->Brush->Style = bsClear;
		Canvas->Rectangle(SelRect.Left, SelRect.Top, SelRect.Right, SelRect.Bottom);
		Canvas->Pen->Mode	 = pmCopy;
		Canvas->Brush->Style = bsSolid;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::ScrollTimerTimer(TObject *Sender)
{
	if (FS->moving) {
		ScrollTimer->Enabled = false;

		TPoint p = ScreenToClient(Mouse->CursorPos);
		//画面右・下端での自動スクロール処理
		if (p.x>=ClientWidth)  ScrollProc(32, 0);
		if (p.y>=ClientHeight) ScrollProc(0, 32);
		//画面左・上端での自動スクロール処理
		if (HorzScrollBar->Position>0 && p.x<=0) ScrollProc(-32, 0);
		if (VertScrollBar->Position>0 && p.y<=0) ScrollProc(0, -32);

		ScrollTimer->Enabled = true;
	}
}
//---------------------------------------------------------------------------

