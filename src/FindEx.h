//----------------------------------------------------------------------//
//	拡張検索															//
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
//検索用、断片データの抜粋
struct q_fragment {
	UnicodeString FileName;		//データファイル名
	UnicodeString DataTitle;	//タイトル
	int  	   ID;				//ID(永久固定)
	grp_stk	   group;			//所属グループ
	FragStyle  style;			//スタイル
	int		   wd;				//桁幅
	int		   hi;				//行数
	TColor 	   colFG;			//文字色
	TColor 	   colBG;			//背景色
	UnicodeString TextStr;		//テキスト内容
	UnicodeString LinkName;		//リンク先
	TDateTime  CreTime;			//作成日時
	TDateTime  ModTime;			//最終アクセス
};

//検索結果リスト (TList から継承)
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
__published:	// IDE 管理のコンポーネント
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

private:	// ユーザー宣言
	TProgressBar *ProgressBar1;

	TQFragList  *QList;
	TStringList *DataFileList;
	bool swAND, swNOT, swREG, swCASE;	//検索モード

	int  __fastcall QIndexOf(int id, UnicodeString fnam);
	bool __fastcall CheckListAndWnd();
	void __fastcall UpdateListRow(int ridx);
	int  __fastcall search_file(UnicodeString fnam, UnicodeString wd);

public:		// ユーザー宣言
	__fastcall TFindExForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFindExForm *FindExForm;
//---------------------------------------------------------------------------
#endif
