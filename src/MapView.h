//----------------------------------------------------------------------//
//	全体図																//
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
__published:	// IDE 管理のコンポーネント
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall MapUpdate(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// ユーザー宣言
	double v_ratio;	//表示倍率

	void __fastcall WmExitSizeMove(TMessage &msg);
	bool __fastcall SetRatio();

public:		// ユーザー宣言
	FragSet *FS;	//表示対象

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
