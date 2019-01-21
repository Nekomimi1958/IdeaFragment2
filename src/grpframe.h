//----------------------------------------------------------------------//
// �O���[�v�g�R���g���[��												//
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
	int  FPlateAlign;	//�\�D�̈ʒu	0:����/ 1:�㒆��/ 2:�E��

	void  __fastcall SetPen(TPen* Value);
	void  __fastcall SetFont(TFont* Value);

protected:
	virtual void __fastcall Paint();

public:
	TRect   FrmRect;		//�g�̒����`
	octrect FrmOctRect;		//�g��8�_�����`
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

