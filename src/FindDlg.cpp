//----------------------------------------------------------------------//
//	�����_�C�A���O														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include <tchar.h>
#include "UserFunc.h"
#include "EnvData.h"
#include "Main.h"
#include "FindDlg.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"

TFindSetDlg *FindSetDlg;

//---------------------------------------------------------------------
__fastcall TFindSetDlg::TFindSetDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::FormCreate(TObject *Sender)
{
	FindModeComboBox->Items->Add("�����ꂩ���܂�");
	FindModeComboBox->Items->Add("�S�Ă��܂�");
	FindModeComboBox->Items->Add("�����ꂩ���܂܂Ȃ�");
	FindModeComboBox->Items->Add("�S�Ă��܂܂Ȃ�");
	FindModeComboBox->Items->Add("���K�\��");
}
//---------------------------------------------------------------------
void __fastcall TFindSetDlg::FormShow(TObject *Sender)
{
	//�_�C�A���O�ʒu����
	EV->load_pos_info("FindDlg", (TForm*)this,
		IdeaFragMainForm->Left + 120, IdeaFragMainForm->Top + 120, 0, 0);

	UnicodeString sct = "General";
	CaseChkBox->Checked   = IniFile->ReadBool(sct, "FindCaseSw");
	FlUpCheckBox->Checked = IniFile->ReadBool(sct, "FindFupSw");

	EV->FindCaseSw = CaseChkBox->Checked;
	EV->FindFupSw  = FlUpCheckBox->Checked;

	FindModeComboBox->ItemIndex = EV->FindMode;
	switch (FindModeComboBox->ItemIndex) {
	case 0:	EV->FindAndSw = false; EV->FindNotSw = false; EV->FindRegSw = false; break;
	case 1:	EV->FindAndSw = true;  EV->FindNotSw = false; EV->FindRegSw = false; break;
	case 2:	EV->FindAndSw = false; EV->FindNotSw = true;  EV->FindRegSw = false; break;
	case 3:	EV->FindAndSw = true;  EV->FindNotSw = true;  EV->FindRegSw = false; break;
	case 4: EV->FindAndSw = false; EV->FindNotSw = false; EV->FindRegSw = true;  break;
	}

	TStringList *lp = EV->FindHistoryList;
	if (lp->Count>0) {
		FindComboBox->Items->Assign(lp);
		FindComboBox->Text = lp->Strings[0];
	}
	else {
		FindComboBox->Text = EmptyStr;
	}

	FindComboBox->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	EV->save_pos_info("FindDlg", (TForm*)this);

	UnicodeString sct = "General";
	IniFile->WriteBool(sct, "FindCaseSw",	CaseChkBox->Checked);
	IniFile->WriteBool(sct, "FindFupSw",	FlUpCheckBox->Checked);

	EV->FindCaseSw = CaseChkBox->Checked;
	EV->FindFupSw  = FlUpCheckBox->Checked;
	EV->FindMode   = FindModeComboBox->ItemIndex;

	switch (FindModeComboBox->ItemIndex) {
	case 0:	EV->FindAndSw = false; EV->FindNotSw = false; EV->FindRegSw = false; break;
	case 1:	EV->FindAndSw = true;  EV->FindNotSw = false; EV->FindRegSw = false; break;
	case 2:	EV->FindAndSw = false; EV->FindNotSw = true;  EV->FindRegSw = false; break;
	case 3:	EV->FindAndSw = true;  EV->FindNotSw = true;  EV->FindRegSw = false; break;
	case 4: EV->FindAndSw = false; EV->FindNotSw = false; EV->FindRegSw = true;  break;
	}
}

//---------------------------------------------------------------------------
//�����ꗓ�̕ҏW
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::SrcCutItemClick(TObject *Sender)
{
	FindComboBox->Perform(WM_CUT, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::SrcCopyItemClick(TObject *Sender)
{
	FindComboBox->Perform(WM_COPY, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::SrcPasteItemClick(TObject *Sender)
{
	FindComboBox->Perform(WM_PASTE, 0, (NativeInt)0);
}

//---------------------------------------------------------------------------
//�����������\�[�g
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::SortFindHistActionExecute(TObject *Sender)
{
	EV->FindHistoryList->Sort();
	FindComboBox->Items->Assign(EV->FindHistoryList);
	FindComboBox->Text = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::SortFindHistActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (FindComboBox->Items->Count>2);
	SrcCutItem->Enabled   = !FindComboBox->SelText.IsEmpty();
	SrcCopyItem->Enabled  = !FindComboBox->SelText.IsEmpty();
	SrcPasteItem->Enabled = Clipboard()->HasFormat(CF_TEXT);
}
//---------------------------------------------------------------------------
//������𗚗�����폜
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::DelFromHistActionExecute(TObject *Sender)
{
	UnicodeString swd = FindComboBox->Text.Trim();
	if (!swd.IsEmpty()) {
		int idx = EV->FindHistoryList->IndexOf(swd);
		if (idx!=-1) EV->FindHistoryList->Delete(idx);
		FindComboBox->Items->Assign(EV->FindHistoryList);
		FindComboBox->Text = EmptyStr;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::DelFromHistActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !FindComboBox->Text.Trim().IsEmpty();
}
//---------------------------------------------------------------------------
//�������������ׂč폜
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::ClrFindHistActionExecute(TObject *Sender)
{
	if (msgbox_Y_N("�������������ׂč폜���Ă������ł����H")) {
		EV->FindHistoryList->Clear();
		FindComboBox->Clear();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::ClrFindHistActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (FindComboBox->Items->Count>0);
}
//---------------------------------------------------------------------------
//���K�\���̎Q��
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::RefRegItemClick(TObject *Sender)
{
	Clipboard()->AsText = get_tkn(((TMenuItem*)Sender)->Caption, " ");
	FindComboBox->Perform(WM_PASTE, 0, (NativeInt)0);
}

//---------------------------------------------------------------------------
//�����J�n
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::StartActionExecute(TObject *Sender)
{
	if (!FindComboBox->Text.IsEmpty()) {
		UnicodeString wd = trim_ex(FindComboBox->Text);
		TStringList *lp = EV->FindHistoryList;
		int idx = lp->IndexOf(wd);
		if (idx!=0) {
			if (idx>0) lp->Delete(idx);
			lp->Insert(0, wd);	//�����̐擪�ɒǉ�
			FindComboBox->Items->Assign(lp);
		}
		FindComboBox->Text = wd;
		ModalResult = mrOk;
	}
	else {
		ModalResult = mrCancel;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindSetDlg::StartActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	bool reg_ng = (FindModeComboBox->ItemIndex==4) && !FindComboBox->Text.IsEmpty() && !chk_RegExPtn(FindComboBox->Text);
	FindComboBox->Font->Color = reg_ng? clRed : clWindowText;
	ap->Enabled = !reg_ng;
}
//---------------------------------------------------------------------------

