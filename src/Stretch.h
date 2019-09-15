//----------------------------------------------------------------------//
// �L�k�_�C�A���O														//
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
__published:	// IDE �Ǘ��̃R���|�[�l���g
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

private:	// ���[�U�[�錾
	int LastBarPos;

public:		// ���[�U�[�錾
	int PosMode;		//0:�f�� 1:�O���[�v
	FragSet *FS;		//�L�k�Ώ�
	double s_ratio;		//�L�k�{��

	__fastcall TStretchDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStretchDlg *StretchDlg;
//---------------------------------------------------------------------------
#endif
