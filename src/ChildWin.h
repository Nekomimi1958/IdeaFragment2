//----------------------------------------------------------------------//
//	子ウィンドウ														//
//																		//
//----------------------------------------------------------------------//
#ifndef ChildWinH
#define ChildWinH
//----------------------------------------------------------------------------
#include <Winapi.Windows.hpp>
#include <System.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "fragset.h"

//----------------------------------------------------------------------------
class TMDIChild : public TForm
{
__published:
	TSaveDialog *SaveDialog;
	TShape *PopPosShape;
	TTimer *ScrollTimer;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void __fastcall FormDblClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ScrollTimerTimer(TObject *Sender);
	void __fastcall FormDeactivate(TObject *Sender);

private:
	bool ScrMoving;		//画面移動中
	TPoint lastP;		//画面移動中の前回のマウス位置

	bool moved;			//マウス移動あり

	bool SelSkip;
	int  orgX, orgY;	//原点

	Fragment *FromFp;	//始点の断片
	Fragment *ToFp;		//終点の断片
	int       FromGn;	//始点のグループ
	int       ToGn;		//終点のグループ

	bool DblSkip;

	void __fastcall WmGetMinMaxInfo(TWMGetMinMaxInfo &Msg);

public:
	FragSet   *FS;		//断片セット
	Fragment  *cur_fp;

	bool  Selecting;	//枠選択中
	TRect SelRect;		//選択枠

	virtual __fastcall TMDIChild(TComponent *Owner);
	void __fastcall ScrollProc(int dx, int dy);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_GETMINMAXINFO,	TWMGetMinMaxInfo,	WmGetMinMaxInfo)
	END_MESSAGE_MAP(TForm);
};
//----------------------------------------------------------------------------
#endif
