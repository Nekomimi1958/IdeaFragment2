//----------------------------------------------------------------------//
//	伸縮ダイアログ														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include "EnvData.h"
#include "Main.h"
#include "Stretch.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStretchDlg *StretchDlg;

//---------------------------------------------------------------------------
__fastcall TStretchDlg::TStretchDlg(TComponent* Owner)
        : TForm(Owner)
{
	FS		   = NULL;
	PosMode    = 0;
	LastBarPos = 0;
}
//---------------------------------------------------------------------------
void __fastcall TStretchDlg::FormShow(TObject *Sender)
{
	FS->push_all();

	//ダイアログ位置復元
	EV->load_pos_info("Stretch", (TForm*)this,
		IdeaFragMainForm->Left + 140, IdeaFragMainForm->Top + 140, 0, 0);

	//断片
	if (PosMode==0) {
		Caption = "断片位置の伸縮";
		Panel3->Visible = false;
		Panel2->Visible = true;
		ClientHeight = Panel1->Height + Panel2->Height + Panel4->Height;
		RadioGroup1->ItemIndex = EV->StretchMode;
		RadioGroup1Click(NULL);
		SetBtn1->Enabled = true;
		SetBtn2->Enabled = true;
	}
	//グループ
	else {
		Caption = "グループ位置の伸縮";
		Panel2->Visible = false;
		Panel3->Visible = true;
		ClientHeight = Panel1->Height + Panel3->Height + Panel4->Height;
		Grp0CheckBox->Checked = EV->StretchGrp0;
 		TrackBar1->Position   = 100;
		SetBtn1->Enabled = false;
		SetBtn2->Enabled = false;
	}

	TrackBar1->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TStretchDlg::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	EV->save_pos_info("Stretch", (TForm*)this);
	if (PosMode==0)
		EV->StretchMode = RadioGroup1->ItemIndex;
	else
		EV->StretchGrp0 = Grp0CheckBox->Checked;

	if (ModalResult==mrOk)
		FS->modify = true;
	else
		FS->undo();
	FS->frg_owner->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TStretchDlg::TrackBar1Change(TObject *Sender)
{
	s_ratio = TrackBar1->Position/100.0;
	//断片
	if (PosMode==0)
		FS->stretch_sel(s_ratio, RadioGroup1->ItemIndex);
	//グループ
	else {
		if (s_ratio>=0)
			FS->stretch_grp(s_ratio, Grp0CheckBox->Checked);
		else
			TrackBar1->Position = 0;
	}
	LastBarPos = TrackBar1->Position;
}
//---------------------------------------------------------------------------
//伸縮モードの選択
//---------------------------------------------------------------------------
void __fastcall TStretchDlg::RadioGroup1Click(TObject *Sender)
{
	int smod = RadioGroup1->ItemIndex;
	TrackBar1->Position = (smod==4 || smod==5)? 0 : 100;
	LastBarPos = TrackBar1->Position;
	TrackBar1Change(Sender);
}
//---------------------------------------------------------------------------
//一発設定ボタン
//---------------------------------------------------------------------------
void __fastcall TStretchDlg::SetBtnXClick(TObject *Sender)
{
	TrackBar1->Position = (((TComponent*)Sender)->Tag - 2) * 100;
	TrackBar1Change(TrackBar1);
}
//---------------------------------------------------------------------------
//適用ボタン
//---------------------------------------------------------------------------
void __fastcall TStretchDlg::ApplyBtnClick(TObject *Sender)
{
	FS->push_all();
	FS->modify = true;

	if (PosMode==0)
		TrackBar1->Position = ((RadioGroup1->ItemIndex==4)? 0 : 100);
	else
		TrackBar1->Position = 100;

}
//---------------------------------------------------------------------------

