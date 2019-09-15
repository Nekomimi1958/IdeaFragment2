//----------------------------------------------------------------------//
// 伸縮ダイアログ														//
//																		//
//----------------------------------------------------------------------//
#ifndef StretchH
#define StretchH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "fragset.h"

//---------------------------------------------------------------------------
class TStretchDlg : public TForm
{
__published:	// IDE 管理のコンポーネント
	TButton *ApplyBtn;
	TButton *CancelBtn;
	TButton *OKBtn;
	TButton *SetBtn1;
	TButton *SetBtn2;
	TButton *SetBtn3;
	TButton *SetBtn4;
	TButton *SetBtn5;
	TCheckBox *Grp0CheckBox;
	TLabel *Label1;
	TLabel *Label2;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TRadioGroup *RadioGroup1;
	TTrackBar *TrackBar1;

	void __fastcall TrackBar1Change(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall RadioGroup1Click(TObject *Sender);
	void __fastcall SetBtnXClick(TObject *Sender);
	void __fastcall ApplyBtnClick(TObject *Sender);

private:	// ユーザー宣言
	int LastBarPos;

public:		// ユーザー宣言
	int PosMode;		//0:断片 1:グループ
	FragSet *FS;		//伸縮対象
	double s_ratio;		//伸縮倍率

	__fastcall TStretchDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStretchDlg *StretchDlg;
//---------------------------------------------------------------------------
#endif
