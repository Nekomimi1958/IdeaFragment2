//----------------------------------------------------------------------//
//	�S�̐}																//
//																		//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "EnvData.h"
#include "Main.h"
#include "ChildWin.h"
#include "MapView.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMapViewer *MapViewer;

//---------------------------------------------------------------------------
__fastcall TMapViewer::TMapViewer(TComponent* Owner) : TForm(Owner)
{
	FS		= NULL;
	v_ratio = 1.0;
}

//---------------------------------------------------------------------------
//�{���̐ݒ�
//---------------------------------------------------------------------------
bool __fastcall TMapViewer::SetRatio()
{
	FS = IdeaFragMainForm->CurFS;	if (!FS) return false;
	if (FS->scr_wd==0 || FS->scr_hi==0) return false;

	double r0 = 0.999*ClientWidth/FS->scr_wd;
	double r1 = 0.999*ClientHeight/FS->scr_hi;
	v_ratio = std::min(r0, r1);
	if (v_ratio==0) v_ratio = 0.1;

	return true;
}

//---------------------------------------------------------------------------
void __fastcall TMapViewer::FormShow(TObject *Sender)
{
	AlphaBlendValue = EV->MapAlpha;

	//�ʒu�E�T�C�Y�𕜌�
	EV->load_pos_info("MapViewer", (TForm*)this,
		(Screen->Width - 640)/2 + 12, (Screen->Height - 480)/2 + 330, 160, 120);

	if (SetRatio() && EV->TimerLockCnt==0) {
		//�J�[�\����g������
		TForm* op = FS->frg_owner;
		TPoint mp;
		mp.x = (op->HorzScrollBar->Position + op->ClientWidth/2) * v_ratio;
		mp.y = (op->VertScrollBar->Position + op->ClientHeight/2) * v_ratio;
		mp = ClientToScreen(mp);
		::SetCursorPos(mp.x, mp.y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMapViewer::FormClose(TObject *Sender, TCloseAction &Action)
{
	//�ʒu�E�T�C�Y��ۑ�
	EV->save_pos_info("MapViewer", (TForm*)this);
}

//---------------------------------------------------------------------------
void __fastcall TMapViewer::WmExitSizeMove(TMessage &msg)
{
	if (FS && EV->FitMapView) {
		int w = round_i(FS->scr_wd * v_ratio) + 2;
		int h = round_i(FS->scr_hi * v_ratio) + 2;
		if (abs(ClientWidth - w) > 2)	ClientWidth  = w;
		if (abs(ClientHeight - h) > 2)	ClientHeight = h;
	}
}

//---------------------------------------------------------------------------
//�E�B���h�E���ɍ��킹�č����𒲐�
//---------------------------------------------------------------------------
void __fastcall TMapViewer::AdjustWinHeight()
{
	if (FS && FS->scr_wd>0) {
		v_ratio = 0.999*ClientWidth/FS->scr_wd;
		ClientHeight = round_i(FS->scr_hi * v_ratio) + 2;
	}
}

//---------------------------------------------------------------------------
//�\�����X�V
//---------------------------------------------------------------------------
void __fastcall TMapViewer::MapUpdate(TObject *Sender)
{
	if (!Visible) return;

	//�������}���邽�߈�U��������ɕ`��
	if (SetRatio()) {
		std::unique_ptr<Graphics::TBitmap> img(new Graphics::TBitmap());
		img->SetSize(ClientWidth, ClientHeight);
		TCanvas *cv = img->Canvas;

		//�S�̃T�C�Y�̈��`��
		int w = round_i(FS->scr_wd * v_ratio) + 2;
		int h = round_i(FS->scr_hi * v_ratio) + 2;

		cv->Brush->Color = (EV->col_MapBG!=clNone)? EV->col_MapBG : FS->frg_owner->Color;
		cv->FillRect(Rect(0, 0, w, h));
		cv->Brush->Color = clAppWorkSpace;
		if (w < ClientWidth)  cv->FillRect(Rect(w, 0, ClientWidth, ClientHeight));
		if (h < ClientHeight) cv->FillRect(Rect(0, h, w, ClientHeight));

		//��ʕ�������`��
		FS->draw_divln(cv, FS->scr_wd, FS->scr_hi, v_ratio);

		//�f�Ђ��k���`��
		for (int i=0; i<FS->FrgList->Count; i++) {
			Fragment *fp = FS->FrgList->Items[i];
			if (fp->Visible) fp->view_frag(cv, v_ratio);
		}

		//�f�Њ֌W����`��
		if (EV->show_FragLine) {
			for (int i=0; i<FS->CLineList->Count; i++) {
				ArrowLine *ap = FS->CLineList->Items[i];
				if (ap->tType!=TDIR_F2F) continue;
				TColor col = ap->lColor;
				if (col==clNone) col = FS->col_FrgLine;
				if (col==clNone) col = EV->col_FrgLine;
				ap->view_line(cv, v_ratio, col);
			}
		}

		//�O���[�v�g��`��
		cv->Pen->Style = psSolid;
		cv->Font->Assign(FS->namFont);
		bool vflag = (FS->namFont->Name[1]=='@');
		int th = std::max((int)(abs(cv->Font->Height)*v_ratio + EV->plt_mgn*2*v_ratio), 3);
		for (int gn=1; gn<=MAX_GROUP; gn++) {
			grp_inf *gp = &FS->GInfs->Items[gn];
			if (gp->Empty || !gp->gfp->Visible) continue;
			TRect rc = gp->RC;
			scale_rect(&rc, v_ratio);
			if (EV->PlateOnGfrm && gp->Plate) rc.top += (th/2);
			int r  = std::min(rc.Width(), rc.Height())/4;
			r = std::min(r, (int)(EV->gfrmMaxRnd * v_ratio));
			cv->Brush->Style = bsClear;
			TColor col = gp->col_GrpFrm;
			if (col==clNone) col = FS->col_GrpFrm;
			if (col==clNone) col = EV->col_GrpFrm;
			cv->Pen->Color = col;
			cv->RoundRect(rc.Left, rc.Top, rc.Right, rc.Bottom, r, r);
			if (!EV->PlateOnGfrm || !gp->Plate) continue;
			//�\�D
			int tw = std::max((int)(cv->TextWidth(gp->Plate->TextStr)*v_ratio + th), 3);
			int y0 = rc.top;
			TRect prc;
			//�c����
			if (vflag) {
				int x0 = rc.Right;
				int y1 = rc.Bottom;
				int rh = r/2;
				int yt0;
				switch (EV->PlateAlign) {
				case 1:	 yt0 = y0 + (y1-y0 - tw)/2;							break;	//����
				case 2:  yt0 = y0 + (int)(y1 - y0 - tw -rh - 4*v_ratio); 	break;	//��
				default: yt0 = y0 + (int)(rh + 4*v_ratio);							//��
				}
				prc = Rect(x0 - std::max(th/2, 2), yt0, x0 + th/2, yt0 + tw);
			}
			//������
			else {
				int x0 = rc.Left;
				int x1 = rc.Right;
				int rh = r/2;
				int xt0;
				switch (EV->PlateAlign) {
				case 1:	 xt0 = x0 + (x1 - x0 - tw)/2; 						break;	//����
				case 2:  xt0 = x0 + (int)(x1 - x0 - tw -rh - 4*v_ratio); 	break;	//�E
				default: xt0 = x0 + (int)(rh + 4*v_ratio);							//��
				}
				prc = Rect(xt0, y0 - std::max(th/2, 2), xt0 + tw, y0 + th/2);
			}
			//�w�i
			cv->Brush->Style = bsSolid;
			cv->Brush->Color = gp->Plate->Selected? EV->col_selBG : gp->Plate->colBG;
			cv->FillRect(prc);
			//�֊s
			cv->Brush->Color = EV->col_Border;
			cv->FrameRect(prc);
		}

		if (EV->show_GroupLine) {
			for (int i=0; i<FS->CLineList->Count; i++) {
				ArrowLine *ap = FS->CLineList->Items[i];
				//�O���[�v�֌W����`��
				if (ap->tType==TDIR_G2G) {
					TColor col = ap->lColor;
					if (col==clNone) col = FS->col_GrpLine;
					if (col==clNone) col = EV->col_GrpLine;
					ap->view_line(cv, v_ratio, col);
				}
				//�f�ЁE�O���[�v�Ԋ֌W����`��
				else if (ap->tType==TDIR_F2G || ap->tType==TDIR_G2F) {
					TColor col = ap->lColor;
					if (col==clNone) col = FS->col_FrgLine;
					if (col==clNone) col = EV->col_FrgLine;
					ap->view_line(cv, v_ratio, col);
				}
			}
		}

		UnicodeString msg;
		if (FS->scr_wd>1 && FS->scr_hi>1) {
			//�G���A�g
			TMDIChild* op = (TMDIChild*)FS->frg_owner;
			int hp = op->HorzScrollBar->Position;
			int vp = op->VertScrollBar->Position;
			int l = hp * v_ratio;
			int t = vp * v_ratio;
			int w = op->ClientWidth  * v_ratio;
			int h = op->ClientHeight * v_ratio;
			TRect rc = Rect(l, t, (l + w), (t + h));
			cv->Brush->Color = EV->col_AreaFrm;
			cv->FrameRect(rc);

			if (v_ratio>0) {
				TPoint p = rc.CenterPoint();
				int xp = round_i(p.x/v_ratio);
				int yp = round_i(p.y/v_ratio);
				msg.sprintf(_T("x:%u, y:%u (%u%%, %u%%)"), xp, yp, round_i(100.0*xp/FS->scr_wd), round_i(100.0*yp/FS->scr_hi));
			}

			//�I��g
			if (EV->MapViewSelRect && op->Selecting) {
				int l = (op->SelRect.Left + hp)   * v_ratio;
				int t = (op->SelRect.Top + vp)    * v_ratio;
				int r = (op->SelRect.Right + hp)  * v_ratio;
				int b = (op->SelRect.Bottom + vp) * v_ratio;
				cv->Pen->Mode	 = pmNot;
				cv->Brush->Style = bsClear;
				cv->Rectangle(l, t, r, b);
				cv->Pen->Mode	 = pmCopy;
				cv->Brush->Style = bsSolid;
			}
		}
		IdeaFragMainForm->StatusBar->Panels->Items[2]->Text = msg;

		if (img) {
			Canvas->CopyMode = cmSrcCopy;
			Canvas->Draw(0, 0, img.get());
		}
	}
	//�f�Ђ������ꍇ
	else {
		Canvas->Brush->Color = clAppWorkSpace;
		Canvas->FillRect(Rect(0, 0, ClientWidth, ClientHeight));
	}
}
//---------------------------------------------------------------------------
//�N���b�N�ʒu�ֈړ�
//---------------------------------------------------------------------------
void __fastcall TMapViewer::FormMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	FS = IdeaFragMainForm->CurFS;	if (!FS) return;
	if (Button!=mbLeft) return;

	TForm* op = FS->frg_owner;
	op->HorzScrollBar->Position = (int)(X/v_ratio) - op->ClientWidth/2;
	op->VertScrollBar->Position = (int)(Y/v_ratio) - op->ClientHeight/2;
	op->Invalidate();
}
//---------------------------------------------------------------------------
//�}�E�X�ړ��ɂ��킹�ăX�N���[��
//---------------------------------------------------------------------------
void __fastcall TMapViewer::FormMouseMove(TObject *Sender,
	  TShiftState Shift, int X, int Y)
{
	FS = IdeaFragMainForm->CurFS;	if (!FS) return;
	if (!Shift.Contains(ssLeft)) return;

	if (X>=0 && Y>=0 && X<=ClientWidth && Y<=ClientHeight) {
		TForm* op = FS->frg_owner;
		op->HorzScrollBar->Position = (int)(X/v_ratio) - op->ClientWidth/2;
		op->VertScrollBar->Position = (int)(Y/v_ratio) - op->ClientHeight/2;
		op->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMapViewer::FormMouseUp(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	FS = IdeaFragMainForm->CurFS;	if (!FS) return;
	FS->frg_owner->Invalidate();
}

//---------------------------------------------------------------------------
//�L�[����
//---------------------------------------------------------------------------
void __fastcall TMapViewer::FormKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	switch (Key) {
	case VK_ESCAPE: case VK_CONTROL: case VK_MENU:
		IdeaFragMainForm->SetFocus(); //break;	//���C�����A�N�e�B�u��
	}
}
//---------------------------------------------------------------------------

