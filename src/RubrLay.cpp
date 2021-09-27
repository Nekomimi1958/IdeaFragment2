//----------------------------------------------------------------------//
// 浮遊配置ツール														//
//																		//
//----------------------------------------------------------------------//
#include "EnvData.h"
#include "Main.h"
#include "RubrLay.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TRubLayDlg *RubLayDlg;

//---------------------------------------------------------------------
__fastcall TRubLayDlg::TRubLayDlg(TComponent* AOwner) : TForm(AOwner)
{
	FS = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TRubLayDlg::FormShow(TObject *Sender)
{
	UnicodeString sct = "RubberLay";
	EV->load_pos_info(sct, (TForm*)this,
		IdeaFragMainForm->Left + 200, IdeaFragMainForm->Top + 200, 0, 0);

	LenTrackBar->Position = IniFile->ReadInteger(sct, "LenPos",	100);
	SpcTrackBar->Position = IniFile->ReadInteger(sct, "SpcPos",	200);

	FS = IdeaFragMainForm->CurFS;
	if (FS) FS->push_all();
	Timer1->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TRubLayDlg::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	Timer1->Enabled = false;

	FS = IdeaFragMainForm->CurFS;
	if (FS) {
		FS->floating = false;
		FS->modify = true;
	}

	UnicodeString sct = "RubberLay";
	EV->save_pos_info(sct, (TForm*)this);
	IniFile->WriteInteger(sct, "LenPos",	LenTrackBar->Position);
	IniFile->WriteInteger(sct, "SpcPos",	SpcTrackBar->Position);
}

//---------------------------------------------------------------------
void __fastcall TRubLayDlg::Timer1Timer(TObject *Sender)
{
	FS = IdeaFragMainForm->CurFS;	if (!FS) return;

	if (RunBtn->Down) {
		FS->rubber_sel(LenTrackBar->Position, SpcTrackBar->Position);
		FS->floating = true;
	}
	else {
		FS->floating = false;
	}
}

//---------------------------------------------------------------------------
void __fastcall TRubLayDlg::HideBtnClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
//有効
//---------------------------------------------------------------------------
void __fastcall TRubLayDlg::RunActionExecute(TObject *Sender)
{
	FS = IdeaFragMainForm->CurFS;
	if (FS) FS->push_all();
}
//---------------------------------------------------------------------------
void __fastcall TRubLayDlg::RunActionUpdate(TObject *Sender)
{
	FS = IdeaFragMainForm->CurFS;
	((TAction*)Sender)->Enabled = FS? !FS->read_only : false;
}

//---------------------------------------------------------------------------
//待機
//---------------------------------------------------------------------------
void __fastcall TRubLayDlg::ReadyActionExecute(TObject *Sender)
{
	FS = IdeaFragMainForm->CurFS;	if (!FS) return;
	FS->floating = false;
	FS->frg_owner->Invalidate();
	FS->modify = true;
}

//---------------------------------------------------------------------------
//クイック移動
//---------------------------------------------------------------------------
void __fastcall TRubLayDlg::QuickActionExecute(TObject *Sender)
{
	FS = IdeaFragMainForm->CurFS;	if (!FS) return;
	switch (((TComponent*)Sender)->Tag) {
	case   1: FS->quick_mov_sel(0); break;
	case   2: FS->quick_mov_sel(1); break;
	case  90: FS->quick_mov_sel(2, M_PI_2);  break;
	case -90: FS->quick_mov_sel(2, -M_PI_2); break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TRubLayDlg::QuickActionUpdate(TObject *Sender)
{
	FS = IdeaFragMainForm->CurFS;
	((TAction*)Sender)->Enabled = FS? (!FS->read_only && (FS->SelList->Count>1)) : false;
}
//---------------------------------------------------------------------------

