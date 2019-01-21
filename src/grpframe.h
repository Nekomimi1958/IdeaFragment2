//----------------------------------------------------------------------//
// グループ枠コントロール												//
//----------------------------------------------------------------------//
#ifndef RndFrameH
#define RndFrameH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include "Fragment.h"

//---------------------------------------------------------------------------
class TGrpFrame : public TGraphicControl
{
private:
	TPen  *FPen;
	TFont *FFont;

	Fragment *FPlate;
	int  FPlateAlign;	//表札の位置	0:左上/ 1:上中央/ 2:右上

	void  __fastcall SetPen(TPen* Value);
	void  __fastcall SetFont(TFont* Value);

protected:
	virtual void __fastcall Paint();

public:
	TRect   FrmRect;		//枠の長方形
	octrect FrmOctRect;		//枠の8点長方形
	TRect	PlateRect;

	__fastcall TGrpFrame(TComponent* Owner);
	__fastcall virtual ~TGrpFrame();
	void __fastcall SetVRect(TRect rc);

	__property TPen  *Pen		= {read = FPen, 		write = SetPen};
	__property TFont *Font		= {read = FFont, 		write = SetFont};
	__property Fragment *Plate	= {read = FPlate,		write = FPlate};
	__property int   PlateAlign	= {read = FPlateAlign,	write = FPlateAlign};
};

//---------------------------------------------------------------------------
#endif

