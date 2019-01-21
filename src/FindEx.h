//----------------------------------------------------------------------//
//	�g������															//
//																		//
//----------------------------------------------------------------------//
#ifndef FindExH
#define FindExH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.FileCtrl.hpp>
#include "fragset.h"

//---------------------------------------------------------------------------
//�����p�A�f�Ѓf�[�^�̔���
struct q_fragment {
	UnicodeString FileName;		//�f�[�^�t�@�C����
	UnicodeString DataTitle;	//�^�C�g��
	int  	   ID;				//ID(�i�v�Œ�)
	grp_stk	   group;			//�����O���[�v
	FragStyle  style;			//�X�^�C��
	int		   wd;				//����
	int		   hi;				//�s��
	TColor 	   colFG;			//�����F
	TColor 	   colBG;			//�w�i�F
	UnicodeString TextStr;		//�e�L�X�g���e
	UnicodeString LinkName;		//�����N��
	TDateTime  CreTime;			//�쐬����
	TDateTime  ModTime;			//�ŏI�A�N�Z�X
};

//�������ʃ��X�g (TList ����p��)
class TQFragList : public TList {
private:
	q_fragment * __fastcall Get(int Index);
	void __fastcall Put(int Index, q_fragment *Item);
public:
	__fastcall TQFragList();
	__property q_fragment * Items[int Index] = {read=Get, write=Put};
};

//---------------------------------------------------------------------------
class TFindExForm : public TForm
{
__published:	// IDE �Ǘ��̃R���|�[�l���g
	TAction *ClrFindHistAction;
	TAction *ClrListAction;
	TAction *CopyAnchorAction;
	TAction *CopyListAction;
	TAction *DelFromHistAction;
	TAction *JumpAction;
	TAction *PropertyAction;
	TAction *SortFindHistAction;
	TAction *StartAction;
	TActionList *ActionList1;
	TBevel *Bevel1;
	TButton *HideBtn;
	TButton *RefDirBtn;
	TButton *StartBtn;
	TCheckBox *CaseChkBox;
	TComboBox *ExeModeComboBox;
	TComboBox *FindComboBox;
	TComboBox *FindModeComboBox;
	TComboBox *ObjDirComboBox;
	TComboBox *ObjModeComboBox;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TMenuItem *CopyAnchorItem;
	TMenuItem *JumpItem;
	TMenuItem *P_s1;
	TMenuItem *P_s2;
	TMenuItem *PopReg001Item;
	TMenuItem *PopReg002Item;
	TMenuItem *PopReg003Item;
	TMenuItem *PopReg004Item;
	TMenuItem *PopReg005Item;
	TMenuItem *PopReg006Item;
	TMenuItem *PopReg007Item;
	TMenuItem *PopReg010Item;
	TMenuItem *PopReg011Item;
	TMenuItem *PopReg012Item;
	TMenuItem *PopReg013Item;
	TMenuItem *PopReg014Item;
	TMenuItem *PopReg015Item;
	TMenuItem *PopReg016Item;
	TMenuItem *PopReg017Item;
	TMenuItem *PopReg018Item;
	TMenuItem *PopReg019Item;
	TMenuItem *PopReg020Item;
	TMenuItem *PopReg021Item;
	TMenuItem *PopReg022Item;
	TMenuItem *PopReg023Item;
	TMenuItem *PopReg024Item;
	TMenuItem *PopReg025Item;
	TMenuItem *PopRegSep1Item1;
	TMenuItem *PopRegSep2Item1;
	TMenuItem *PropertyItem;
	TMenuItem *QListClrItem;
	TMenuItem *QListCopyItem;
	TMenuItem *RegExpItem;
	TMenuItem *Sep_src_1;
	TMenuItem *Sep_src_2;
	TMenuItem *SrcClrHistItem;
	TMenuItem *SrcCopyItem;
	TMenuItem *SrcCutItem;
	TMenuItem *SrcDelHistItem;
	TMenuItem *SrcPasteItem;
	TMenuItem *SrcSortHistItem;
	TPanel *Panel1;
	TPanel *Panel2;
	TPopupMenu *PopupMenu1;
	TPopupMenu *SrcPopupMenu;
	TStatusBar *StatusBar1;
	TStringGrid *ResultGrid;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall StartActionUpdate(TObject *Sender);
	void __fastcall StartActionExecute(TObject *Sender);
	void __fastcall ResultGridClick(TObject *Sender);
	void __fastcall RefDirBtnClick(TObject *Sender);
	void __fastcall ResultGridDrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State);
	void __fastcall CopyListActionExecute(TObject *Sender);
	void __fastcall ListActionUpdate(TObject *Sender);
	void __fastcall ListSelOneUpdate(TObject *Sender);
	void __fastcall ClrListActionExecute(TObject *Sender);
	void __fastcall ClearStatusMsg(TObject *Sender);
	void __fastcall CopyAnchorActionExecute(TObject *Sender);
	void __fastcall PropertyActionExecute(TObject *Sender);
	void __fastcall ResultGridDblClick(TObject *Sender);
	void __fastcall JumpActionExecute(TObject *Sender);
    void __fastcall ResultGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DelFromHistActionExecute(TObject *Sender);
	void __fastcall DelFromHistActionUpdate(TObject *Sender);
	void __fastcall SortFindHistActionExecute(TObject *Sender);
	void __fastcall SortFindHistActionUpdate(TObject *Sender);
	void __fastcall SrcCutItemClick(TObject *Sender);
	void __fastcall SrcCopyItemClick(TObject *Sender);
	void __fastcall SrcPasteItemClick(TObject *Sender);
	void __fastcall RefRegItemClick(TObject *Sender);
	void __fastcall ClrFindHistActionExecute(TObject *Sender);
	void __fastcall ClrFindHistActionUpdate(TObject *Sender);
	void __fastcall HideBtnClick(TObject *Sender);

private:	// ���[�U�[�錾
	TProgressBar *ProgressBar1;

	TQFragList  *QList;
	TStringList *DataFileList;
	bool swAND, swNOT, swREG, swCASE;	//�������[�h

	int  __fastcall QIndexOf(int id, UnicodeString fnam);
	bool __fastcall CheckListAndWnd();
	void __fastcall UpdateListRow(int ridx);
	int  __fastcall search_file(UnicodeString fnam, UnicodeString wd);

public:		// ���[�U�[�錾
	__fastcall TFindExForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFindExForm *FindExForm;
//---------------------------------------------------------------------------
#endif
