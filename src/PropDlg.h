//----------------------------------------------------------------------//
// �v���p�e�B/�V�K�쐬�_�C�A���O										//
//																		//
//----------------------------------------------------------------------//
#ifndef PropDlgH
#define PropDlgH
//----------------------------------------------------------------------------
#include <Winapi.Windows.hpp>
#include <Winapi.Messages.hpp>
#include <System.Actions.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.ActnList.hpp>
#include "fragset.h"

//----------------------------------------------------------------------------
class TFrgPropDlg : public TForm
{
__published:
	TAction *BackColAction;
	TAction *CharColAction;
	TAction *FitHiAction;
	TAction *FitWdAction;
	TAction *GetLnkInfAction;
	TAction *OkAction;
	TAction *RefLinkAction;
	TAction *TypeFrgAction;
	TAction *TypeImgAction;
	TAction *TypePltAction;
	TAction *TypeSymAction;
	TActionList *ActionList1;
	TButton *BackColorBtn;
	TButton *CancelBtn;
	TButton *CharColorBtn;
	TButton *FitHiBtn;
	TButton *FitWdBtn;
	TButton *LinkRefBtn;
	TButton *LnkExBtn;
	TButton *OkBtn;
	TCheckBox *TrnsChkBox;
	TColorDialog *ColorDialog1;
	TComboBox *GroupComboBox;
	TEdit *LinkNameEdit;
	TGroupBox *ColGroupBox;
	TGroupBox *LinkGrpBox;
	TGroupBox *SizeGroupBox;
	TGroupBox *TypeGroupBox;
	TLabel *GroupLabel;
	TLabel *Label1;
	TLabel *SizePer_Label;
	TLabeledEdit *FrgHiEdit;
	TLabeledEdit *FrgWdEdit;
	TMemo *Memo1;
	TMenuItem *N1;
	TMenuItem *PopGetAlbumItem;
	TMenuItem *PopGetArtistItem;
	TMenuItem *PopGetComItem;
	TMenuItem *PopGetDscItem;
	TMenuItem *PopGetFileItem;
	TMenuItem *PopGetKeyItem;
	TMenuItem *PopGetNameItem;
	TMenuItem *PopGetPageItem;
	TMenuItem *PopGetPathItem;
	TMenuItem *PopGetTitItem;
	TMenuItem *PopGetTopItem;
	TMenuItem *PopGetTrakItem;
	TMenuItem *PopGetYearItem;
	TOpenDialog *OpenLnkDlg;
	TOpenPictureDialog *OpenImgDlg;
	TPaintBox *GripPaintBox;
	TPanel *Panel1;
	TPanel *Panel2;
	TPopupMenu *PopupMenu1;
	TRadioButton *ImageRadioBtn;
	TRadioButton *NormalRadioBtn;
	TRadioButton *PlateRadioBtn;
	TRadioButton *SymbolRadioBtn;
	TUpDown *HiUpDown;
	TUpDown *WdUpDown;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Memo1Change(TObject *Sender);
	void __fastcall OkActionExecute(TObject *Sender);
	void __fastcall FrgHiEditChange(TObject *Sender);
	void __fastcall FrgWdEdit_Change(TObject *Sender);
	void __fastcall GroupComboBoxChange(TObject *Sender);
	void __fastcall PopGetTitItemClick(TObject *Sender);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall PopGetTopItemClick(TObject *Sender);
	void __fastcall PopGetMetaItemClick(TObject *Sender);
	void __fastcall PopGetMp3ItemClick(TObject *Sender);
	void __fastcall TrnsChkBoxClick(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall GripPaintBoxPaint(TObject *Sender);
	void __fastcall PopGetPathItemClick(TObject *Sender);
	void __fastcall PopGetFileItemClick(TObject *Sender);
	void __fastcall PopGetNameItemClick(TObject *Sender);
	void __fastcall Memo1Enter(TObject *Sender);
	void __fastcall Memo1Exit(TObject *Sender);
	void __fastcall FitWdActionExecute(TObject *Sender);
	void __fastcall FitWdActionUpdate(TObject *Sender);
	void __fastcall FitHiActionExecute(TObject *Sender);
	void __fastcall FitHiActionUpdate(TObject *Sender);
	void __fastcall TypeFrgActionExecute(TObject *Sender);
	void __fastcall TypeFrgActionUpdate(TObject *Sender);
	void __fastcall TypePltActionExecute(TObject *Sender);
	void __fastcall TypePltActionUpdate(TObject *Sender);
	void __fastcall TypeSymActionExecute(TObject *Sender);
	void __fastcall TypeSymActionUpdate(TObject *Sender);
	void __fastcall TypeImgActionExecute(TObject *Sender);
	void __fastcall TypeImgActionUpdate(TObject *Sender);
	void __fastcall CharColActionExecute(TObject *Sender);
	void __fastcall CharColActionUpdate(TObject *Sender);
	void __fastcall BackColActionExecute(TObject *Sender);
	void __fastcall BackColActionUpdate(TObject *Sender);
	void __fastcall RefLinkActionExecute(TObject *Sender);
	void __fastcall RefLinkActionUpdate(TObject *Sender);
	void __fastcall GetLnkInfActionExecute(TObject *Sender);
	void __fastcall GetLnkInfActionUpdate(TObject *Sender);
	void __fastcall PopGetPageItemClick(TObject *Sender);

private:
	bool mltSelected;
	bool mlt_group;
	bool mlt_wd;
	bool mlt_hi;
	bool mlt_colFG;
	bool mlt_colBG;

	int  __fastcall GetFitWidth(TCanvas * cv, UnicodeString sbuf, int wd, int hi);
	int  __fastcall GetFitHeight(TCanvas * cv, UnicodeString sbuf, int wd);
	void __fastcall WMDropFiles(TWMDropFiles &Msg);
	void __fastcall CMDialogKey(TCMDialogKey &Msg);

public:
	FragSet *prpFS;			//�ΏۂƂȂ�f�ЃZ�b�g
	Fragment *prpFrag;		//�ΏۂƂȂ�f��

	bool swNewFrag;			//�V�K�쐬
	bool swNewCurPos;		//���݈ʒu�ɍ쐬
	int  newPosX, newPosY;	//swNewCurPos = true ���̍쐬�ʒu
	bool swNewLink;			//�����N����w��
	UnicodeString newLink;	//swNewLink = true ���̃����N��
	bool swNewGroup;		//�O���[�v���w��
	int  newGrp;			//swNewCurPos/swNewGroup = true ���̃O���[�v
	grp_stk newGstk;		//swNewCurPos/swNewGroup = true ���̃O���[�v�X�^�b�N

	bool swNewPlate;		//�\�D��V�K�쐬
	FragStyle cur_style;	//�I�𒆂̃X�^�C��

	virtual __fastcall TFrgPropDlg(TComponent *Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
		VCL_MESSAGE_HANDLER(CM_DIALOGKEY, TCMDialogKey, CMDialogKey)
	END_MESSAGE_MAP(TForm);
};
//----------------------------------------------------------------------------
extern PACKAGE TFrgPropDlg *FrgPropDlg;
//----------------------------------------------------------------------------
#endif
