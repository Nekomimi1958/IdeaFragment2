//----------------------------------------------------------------------//
//	断片一覧															//
//																		//
//----------------------------------------------------------------------//
#ifndef FrgGridH
#define FrgGridH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "fragset.h"

//---------------------------------------------------------------------------
//カラム項目ID ( = SortMode);
#define FRGRD_COL_GROUP	0
#define FRGRD_COL_STYLE	1
#define FRGRD_COL_TEXT	2
#define FRGRD_COL_CRE_T	3
#define FRGRD_COL_ACC_T	4
#define FRGRD_COL_LINK	5
#define FRGRD_COL_ID	6

#define SRTMD_NONE		-1
#define SRTMD_SELECT	1

//---------------------------------------------------------------------------
class TFragGrid : public TForm
{
__published:	// IDE 管理のコンポーネント
	TPanel *DockPanel;
	TSplitter *ListSplitter;
    TPanel *ListPanel;
	TStringGrid *FrgLstGrid;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormDeactivate(TObject *Sender);
    void __fastcall FrgLstGridEnter(TObject *Sender);
    void __fastcall FrgLstGridExit(TObject *Sender);
	void __fastcall FrgLstGridDrawCell(TObject *Sender, int ACol,
		  int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall FrgLstGridDblClick(TObject *Sender);
	void __fastcall FrgLstGridMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FrgLstGridKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift);
	void __fastcall DockPanelDockOver(TObject *Sender,
		TDragDockObject *Source, int X, int Y, TDragState State, bool &Accept);
	void __fastcall DockPanelDockDrop(TObject *Sender,
		  TDragDockObject *Source, int X, int Y);
	void __fastcall DockPanelUnDock(TObject *Sender, TControl *Client,
		  TWinControl *NewTarget, bool &Allow);
	void __fastcall DockPanelResize(TObject *Sender);

private:	// ユーザー宣言
	bool SelSkip;
	bool DblSkip;

public:		// ユーザー宣言
	FragSet   *FS;			//表示対象
	TFragList *FrgList; 	//断片のリスト

	int  SortMode;
	int  SortModeEx;
	bool SortSkip;

	__fastcall TFragGrid(TComponent* Owner);
	void __fastcall GridUpdate(bool updsw = false);
	void __fastcall SetGroupTop(int gn);
	void __fastcall SetFrgIndex(Fragment *fp);
};
//---------------------------------------------------------------------------
extern PACKAGE TFragGrid *FragGrid;
//---------------------------------------------------------------------------
#endif
