//----------------------------------------------------------------------//
//	�S�̐}																//
//																		//
//----------------------------------------------------------------------//
#ifndef MapViewH
#define MapViewH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include "fragset.h"

//---------------------------------------------------------------------------
class TMapViewer : public TForm
{
__published:	// IDE �Ǘ��̃R���|�[�l���g
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall MapUpdate(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// ���[�U�[�錾
	double v_ratio;	//�\���{��

	void __fastcall WmExitSizeMove(TMessage &msg);
	bool __fastcall SetRatio();

public:		// ���[�U�[�錾
	FragSet *FS;	//�\���Ώ�

	void __fastcall AdjustWinHeight();

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_EXITSIZEMOVE,	TMessage,	WmExitSizeMove)
	END_MESSAGE_MAP(TForm);

	__fastcall TMapViewer(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMapViewer *MapViewer;
//---------------------------------------------------------------------------
#endif
