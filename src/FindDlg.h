//----------------------------------------------------------------------//
//	検索ダイアログ														//
//																		//
//----------------------------------------------------------------------//
#ifndef FindDlgH
#define FindDlgH
//----------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ActnList.hpp>

//----------------------------------------------------------------------------
class TFindSetDlg : public TForm
{
__published:
	TAction *ClrFindHistAction;
	TAction *DelFromHistAction;
	TAction *SortFindHistAction;
	TAction *StartAction;
	TActionList *ActionList1;
	TButton *CanButton;
	TButton *OKBtn;
	TCheckBox *CaseChkBox;
	TCheckBox *FlUpCheckBox;
	TComboBox *FindComboBox;
	TComboBox *FindModeComboBox;
	TLabel *Label1;
	TMenuItem *N1;
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
	TMenuItem *RegExpItem;
	TMenuItem *Sep_src_1;
	TMenuItem *Sep_src_2;
	TMenuItem *SrcCopyItem;
	TMenuItem *SrcCutItem;
	TMenuItem *SrcDelHistItem;
	TMenuItem *SrcPasteItem;
	TMenuItem *SrcSortHistItem;
	TPopupMenu *SrcPopupMenu;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SrcCutItemClick(TObject *Sender);
	void __fastcall SrcCopyItemClick(TObject *Sender);
	void __fastcall SrcPasteItemClick(TObject *Sender);
	void __fastcall SortFindHistActionExecute(TObject *Sender);
	void __fastcall SortFindHistActionUpdate(TObject *Sender);
	void __fastcall DelFromHistActionExecute(TObject *Sender);
	void __fastcall DelFromHistActionUpdate(TObject *Sender);
	void __fastcall ClrFindHistActionExecute(TObject *Sender);
	void __fastcall ClrFindHistActionUpdate(TObject *Sender);
	void __fastcall RefRegItemClick(TObject *Sender);
	void __fastcall StartActionExecute(TObject *Sender);
	void __fastcall StartActionUpdate(TObject *Sender);

private:

public:
	virtual __fastcall TFindSetDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TFindSetDlg *FindSetDlg;
//----------------------------------------------------------------------------
#endif
