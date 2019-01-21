//----------------------------------------------------------------------//
// ÉOÉãÅ[ÉvògÉRÉìÉgÉçÅ[Éã												//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include <algorithm>

#include "UserFunc.h"
#include "EnvData.h"
#include "grpframe.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TGrpFrame::TGrpFrame(TComponent* AOwner) : TGraphicControl(AOwner)
{
	Width		= 50;
	Height		= 50;
	Plate		= NULL;
	FPlateAlign = 0;
	FrmRect 	= Rect(0, 0, 0, 0);
	PlateRect	= Rect(0, 0, 0, 0);
	set_octrect(&FrmRect, &FrmOctRect);

	FPen   = new TPen();
	FFont  = new TFont();
}

//---------------------------------------------------------------------------
__fastcall TGrpFrame::~TGrpFrame()
{
	delete FPen;
	delete FFont;
}

//---------------------------------------------------------------------------
//ògÇÃí∑ï˚å`Çê›íË
//---------------------------------------------------------------------------
void __fastcall TGrpFrame::SetVRect(TRect rc)
{
	BoundsRect = rc;
	TRect prc  = PlateRect;
	bool pflag = !prc.IsEmpty();
	bool vflag = (FFont->Name[1]=='@');
	int dh = vflag? prc.Width()/2 : prc.Height()/2;
	int x0 = FPen->Width/2;
	int y0 = x0;
	int w  = Width  - FPen->Width + 1;
	int h  = Height - FPen->Width + 1;
	if (FPen->Width==0) { --w; --h; }
	int x1 = x0 + w;
	int y1 = y0 + h;
	if (pflag) {
		if (vflag) x1 -= dh; else y0 += dh;
	}

	int r;
	if (EV->gfrmMaxRnd>1) {
		r = ((w<h)? w : h)/4;
		r = std::min(r, EV->gfrmMaxRnd);
		if (r%2==0) r++;
		x1--; y1--;
	}
	else {
		r = 0;
		x1--; y1--;
	}

	FrmRect = Rect(x0, y0, x1, y1);
	FrmRect.Offset(Left, Top);
	set_octrect(&FrmRect, &FrmOctRect);
	if (r>0) {
		int dr = r * 0.1464;	//(1-cos(45))/2
		FrmOctRect.lt.x += dr;
		FrmOctRect.lt.y += dr;
		FrmOctRect.rt.x -= dr;
		FrmOctRect.rt.y += dr;
		FrmOctRect.rb.x -= dr;
		FrmOctRect.rb.y -= dr;
		FrmOctRect.lb.x += dr;
		FrmOctRect.lb.y -= dr;
	}

	if (pflag) {
		//ï\éDÇÃÇ†ÇÈï”ÇÃíÜì_í≤êÆ
		if (vflag) {
			int yc = Height/2;
			if (yc>prc.Top && yc<prc.Bottom) FrmOctRect.rc.x = BoundsRect.Right;
		}
		else {
			int xc = Width/2;
			if (xc>prc.Left && xc<prc.Right) FrmOctRect.ct.y = BoundsRect.Top;
		}
	}
}

//---------------------------------------------------------------------------
//ògÇï`âÊ
//---------------------------------------------------------------------------
void __fastcall TGrpFrame::Paint()
{
	Canvas->Pen  = FPen;
	Canvas->Font = FFont;
	bool vflag = (Canvas->Font->Name[1]=='@');

	TRect prc = PlateRect;
	bool pflag = (Plate && !prc.IsEmpty());
	int dh = vflag? prc.Width()/2 : prc.Height()/2;
	int x0 = Canvas->Pen->Width/2;
	int y0 = x0;
	int w  = Width  - Canvas->Pen->Width + 1;
	int h  = Height - Canvas->Pen->Width + 1;
	if (Canvas->Pen->Width==0) { --w; --h; }
	int x1 = x0 + w;
	int y1 = y0 + h;
	if (pflag) {
		if (vflag) x1 -= dh; else y0 += dh;
	}

	if (EV->gfrmMaxRnd>1) {
		int r = ((w<h)? w : h)/4;
		r = std::min(r, EV->gfrmMaxRnd);
		if (r%2==0) r++;
		int rh = r/2 + 1;
		Canvas->Arc(x0, y0, x0 + r, y0 + r,  x0 + rh, y0, x0, y0 + rh);
		Canvas->Arc(x1 - r, y0, x1, y0 + r,  x1, y0 + rh, x1 - rh, y0);
		Canvas->Arc(x1 - r, y1 - r, x1, y1,  x1 - rh, y1, x1, y1 - rh);
		Canvas->Arc(x0, y1-r, x0+r, y1,  x0, y1 - rh, x0 + rh, y1);
		x1--; y1--; rh--;
		Canvas->MoveTo(x0 + rh, y0); Canvas->LineTo(x1 - rh, y0);
		Canvas->MoveTo(x1, y0 + rh); Canvas->LineTo(x1, y1 - rh);
		Canvas->MoveTo(x0 + rh, y1); Canvas->LineTo(x1 - rh - 1, y1);
		Canvas->MoveTo(x0, y0 + rh); Canvas->LineTo(x0, y1 - rh);
	}
	else {
		x1--; y1--;
		Canvas->MoveTo(x0, y0);
		Canvas->LineTo(x1, y0);
		Canvas->LineTo(x1, y1);
		Canvas->LineTo(x0, y1);
		Canvas->LineTo(x0, y0);
	}

	//ï\éDÇï`âÊ
	if (pflag) {
		//îwåiÅEó÷äs
		int r;
		switch (EV->PlateShape) {
		case 1: case 2:
			//äpä€í∑ï˚å`
			r = (EV->PlateShape==2)? dh*2 : dh;
			Canvas->Brush->Color = Plate->Selected? EV->col_selBG : Plate->colBG;
			Canvas->Pen->Width = 1;
			Canvas->Pen->Color = Plate->Noticed? EV->col_selRct : EV->col_Border;
			Canvas->RoundRect(prc.Left, prc.Top, prc.Right, prc.Bottom, r, r);
			break;
		default:
			//í∑ï˚å`
			Canvas->Brush->Color = Plate->Selected? EV->col_selBG : Plate->colBG;
			Canvas->FillRect(prc);
			Canvas->Brush->Color = Plate->Noticed? EV->col_selRct : EV->col_Border;
			Canvas->FrameRect(prc);
		}
		//ï∂éö
		Canvas->Brush->Color = Plate->Selected? EV->col_selBG : Plate->colBG;
		Canvas->Font->Color  = Plate->Selected? EV->col_selFG : Plate->colFG;
		//ècèëÇ´
		if (vflag)
			vert_textout(Canvas, prc.Right - EV->plt_mgn, prc.Top + dh, Plate->TextStr);
		//â°èëÇ´
		else {
			int ymgn = ((prc.Bottom - prc.Top) - Canvas->TextHeight(" "))/2;
			Canvas->TextOut(prc.Left + dh, prc.Top + ymgn, Plate->TextStr);
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TGrpFrame::SetPen(TPen* Value)
{
	FPen->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TGrpFrame::SetFont(TFont* Value)
{
	FFont->Assign(Value);
}
//---------------------------------------------------------------------------
