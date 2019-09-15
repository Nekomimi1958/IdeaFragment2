//----------------------------------------------------------------------//
// 個別設定ダイアログ													//
//																		//
//----------------------------------------------------------------------//
#ifndef OptIdDlgH
#define OptIdDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include "fragset.h"

//---------------------------------------------------------------------------
class TOptIndDlg : public TForm
{
__published:	// IDE 管理のコンポーネント
	TButton *CancelBtn;
	TButton *ColButton;
	TButton *DefColBtn;
	TButton *DefFontBtn;
	TButton *FontButton;
	TButton *OKBtn;
	TCheckBox *FixRatioCheckBox;
	TColorDialog *ColorDialog1;
	TComboBox *ColComboBox;
	TComboBox *DivLwComboBox;
	TComboBox *DivModComboBox;
	TComboBox *FontComboBox;
	TComboBox *TitPosComboBox;
	TEdit *TitleEdit;
	TFontDialog *FontDialog1;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TGroupBox *GroupBox5;
	TLabel *RatioLabel;
	TLabeledEdit *DivPrm1Edit;
	TLabeledEdit *DivPrm2Edit;
	TLabeledEdit *ScrHiEdit;
	TLabeledEdit *ScrWdEdit;
	TUpDown *DivPrm1UpDown;
	TUpDown *DivPrm2UpDown;
	TUpDown *ScrHiUpDown;
	TUpDown *ScrWdUpDown;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall DivEditChange(TObject *Sender);
	void __fastcall ColButtonClick(TObject *Sender);
	void __fastcall ColComboBoxChange(TObject *Sender);
	void __fastcall DefColBtnClick(TObject *Sender);
	void __fastcall FontButtonClick(TObject *Sender);
	void __fastcall FontComboBoxChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall DefFontBtnClick(TObject *Sender);
	void __fastcall DivModComboBoxChange(TObject *Sender);
	void __fastcall ScrWdEdit_Change(TObject *Sender);
	void __fastcall ScrHiEdit_Change(TObject *Sender);
	void __fastcall NumberEditExit(TObject *Sender);
	void __fastcall ColComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall FontComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);

private:	// ユーザー宣言
	double AspectRatio;
	bool   InhChange;

	void __fastcall SetAspectRatio();

public:		// ユーザー宣言
	FragSet *FS;	//設定対象

	TColor col_backgr;
	TColor col_DivLine;
	TColor col_FrgLine;
	TColor col_GrpFrm;
	TColor col_GrpLine;
	TColor col_LinLbl;
	TColor col_Title;

	TFont *frgFont;
	TFont *namFont;
	TFont *symFont;
	TFont *lblFont;
	TFont *titFont;

	__fastcall TOptIndDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptIndDlg *OptIndDlg;
//---------------------------------------------------------------------------
#endif
