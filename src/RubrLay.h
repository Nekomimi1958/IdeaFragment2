//----------------------------------------------------------------------//
// 浮遊配置ツール														//
//																		//
//----------------------------------------------------------------------//
#ifndef RubrLayH
#define RubrLayH
//----------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include "fragset.h"

//----------------------------------------------------------------------------
class TRubLayDlg : public TForm
{
__published:
	TAction *InvHAcion;
	TAction *InvVAction;
	TAction *ReadyAction;
	TAction *RotLAction;
	TAction *RotRAction;
	TAction *RunAction;
	TActionList *ActionList1;
	TButton *HideBtn;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TSpeedButton *RdyBtn;
	TSpeedButton *RunBtn;
	TTimer *Timer1;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TTrackBar *LenTrackBar;
	TTrackBar *SpcTrackBar;

	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall RunActionExecute(TObject *Sender);
	void __fastcall RunActionUpdate(TObject *Sender);
	void __fastcall ReadyActionExecute(TObject *Sender);
	void __fastcall QuickActionExecute(TObject *Sender);
	void __fastcall QuickActionUpdate(TObject *Sender);
	void __fastcall HideBtnClick(TObject *Sender);

private:

public:
	FragSet *FS;		//浮遊対象

	virtual __fastcall TRubLayDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TRubLayDlg *RubLayDlg;
//----------------------------------------------------------------------------
#endif
