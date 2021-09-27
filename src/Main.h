//----------------------------------------------------------------------//
//	IdeaFragment2														//
//	メインフォーム														//
//----------------------------------------------------------------------//
#ifndef MainH
#define MainH

//----------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.StdActns.hpp>
#include "usr_shell.h"
#include "EnvData.h"
#include "ChildWin.h"

//----------------------------------------------------------------------------
#define APPCOMMAND_CLOSE	31

//---------------------------------------------------------------------------
typedef HWND (WINAPI *FUNC_HtmlHelp)(HWND, LPCWSTR, UINT, DWORD);

//----------------------------------------------------------------------------
class TIdeaFragMainForm : public TForm
{
__published:
	TAction *CsvExport;
	TAction *DispStatus;
	TAction *EditAway;
	TAction *EditCopy;
	TAction *EditCut;
	TAction *EditDelete;
	TAction *EditFind;
	TAction *EditGather;
	TAction *EditGrpStretch;
	TAction *EditMovePos;
	TAction *EditNew;
	TAction *EditNewClip;
	TAction *EditNextSel;
	TAction *EditPaste;
	TAction *EditPasteEx;
	TAction *EditPrevSel;
	TAction *EditProperty;
	TAction *EditRedo;
	TAction *EditSelAll;
	TAction *EditSelBgCol;
	TAction *EditSpread;
	TAction *EditStretch;
	TAction *EditTrim;
	TAction *EditUndo;
	TAction *ExCopyAnchor;
	TAction *ExMovBottom;
	TAction *ExMoveCenter;
	TAction *ExMovLeft;
	TAction *ExMovRight;
	TAction *ExMovTop;
	TAction *FileAllSaveEnd;
	TAction *FileClose;
	TAction *FileImgOut;
	TAction *FileMerge;
	TAction *FileNew;
	TAction *FileOpen;
	TAction *FilePrint;
	TAction *FileSave;
	TAction *FileSaveAs;
	TAction *GoBack;
	TAction *GoForward;
	TAction *GoHome;
	TAction *GoJump;
	TAction *HelpAbout;
	TAction *HelpContents;
	TAction *HelpHomePage;
	TAction *OptionIndividual;
	TAction *OptionSetup;
	TAction *RubberLay;
	TAction *ShowDivLine;
	TAction *ShowFLine;
	TAction *ShowFrgGrid;
	TAction *ShowGFrame;
	TAction *ShowGLine;
	TAction *ShowGrpLst;
	TAction *ShowLinkMark;
	TAction *ShowMapViewer;
	TAction *ShowNamePlate;
	TAction *ShowPosGrid;
	TAction *ShowStatusBar;
	TAction *TextExport;
	TAction *TextImport;
	TAction *ToolBrush;
	TAction *ToolFindEx;
	TAction *ToolGLine;
	TAction *ToolGruop;
	TAction *ToolNrm;
	TAction *TsvExport;
	TAction *WindowActMax;
	TAction *WindowArrange;
	TActionList *ActionList1;
	TActionList *ActionList2;
	TApplicationEvents *ApplicationEvents1;
	TColorDialog *ColorDialog1;
	TControlBar *ToolPanel;
	TFileExit *FileExit1;
	TImageList	*BtnImgList;
	TListBox *GroupListBox;
	TMainMenu *MainMenu1;
	TMenuItem  *Brush_s1;
	TMenuItem  *Brush_s2;
	TMenuItem  *BrushChsBgColItem;
	TMenuItem  *BrushChsColItem;
	TMenuItem  *BrushRetItem;
	TMenuItem  *BrushSelBgColItem;
	TMenuItem  *Gfrm_s1;
	TMenuItem  *Gfrm_s2;
	TMenuItem  *GFrmLw0Item;
	TMenuItem  *GFrmLw1Item;
	TMenuItem  *GFrmLw2Item;
	TMenuItem  *GFrmLw3Item;
	TMenuItem  *GFrmLw4Item;
	TMenuItem  *GFrmLw5Item;
	TMenuItem  *GFrmLw6Item;
	TMenuItem  *GFrmLw7Item;
	TMenuItem  *GFrmLw8Item;
	TMenuItem  *GFrmLw9Item;
	TMenuItem  *GFrmLw10Item;
	TMenuItem  *GFrmLwidthItem;
	TMenuItem  *GFrmNameItem;
	TMenuItem  *GFrmRelItem;
	TMenuItem  *GFrmRetItem;
	TMenuItem  *GFrmSelItem;
	TMenuItem  *Gl_s1;
	TMenuItem  *Gl_s2;
	TMenuItem  *Gl_s4;
	TMenuItem  *Gl_s5;
	TMenuItem  *GLine_s1;
	TMenuItem  *GLine_s2;
	TMenuItem  *GLine_s3;
	TMenuItem  *GLine_s4;
	TMenuItem  *GLine_s5;
	TMenuItem  *GLineAsItem;
	TMenuItem  *GLineAsLItem;
	TMenuItem  *GLineAsMItem;
	TMenuItem  *GLineAsSItem;
	TMenuItem  *GLineAtItem;
	TMenuItem  *GLineAtLinItem;
	TMenuItem  *GLineAtSldItem;
	TMenuItem  *GLineDelItem;
	TMenuItem  *GLineDotItem;
	TMenuItem  *GLineLblItem;
	TMenuItem  *GLineRetItem;
	TMenuItem  *GLineType0Item;
	TMenuItem  *GLineType1Item;
	TMenuItem  *GLineType2Item;
	TMenuItem  *GLineType3Item;
	TMenuItem  *GLineType4Item;
	TMenuItem  *GLineWd0Item;
	TMenuItem  *GLineWd1Item;
	TMenuItem  *GLineWd2Item;
	TMenuItem  *GLineWd3Item;
	TMenuItem  *GListBottomItem;
	TMenuItem  *GListDockFlist;
	TMenuItem  *GListDownItem;
	TMenuItem  *GListFloatItem;
	TMenuItem  *GListHideItem;
	TMenuItem  *GListLeftItem;
	TMenuItem  *GListNotBlankItem;
	TMenuItem  *GListPosItem;
	TMenuItem  *GListRightItem;
	TMenuItem  *GListShowAllItem;
	TMenuItem  *GListTopItem;
	TMenuItem  *GListUpItem;
	TMenuItem  *GoListItem;
	TMenuItem  *P_s1;
	TMenuItem  *P_s2;
	TMenuItem  *P_s3;
	TMenuItem  *P_s4;
	TMenuItem  *P_s5;
	TMenuItem  *P2_s1;
	TMenuItem  *P2_s2;
	TMenuItem  *P2_s3;
	TMenuItem  *PopAwayItem;
	TMenuItem  *PopAwayItem2;
	TMenuItem  *PopCopyItem;
	TMenuItem  *PopCopyItem2;
	TMenuItem  *PopCutItem;
	TMenuItem  *PopCutItem2;
	TMenuItem  *PopDeleteItem;
	TMenuItem  *PopDeleteItem2;
	TMenuItem  *PopFindItem;
	TMenuItem  *PopFindItem2;
	TMenuItem  *PopGatherItem;
	TMenuItem  *PopGrpStretchItem;
	TMenuItem  *PopJumpItem;
	TMenuItem  *PopJumpItem2;
	TMenuItem  *PopMoveToGroupItem;
	TMenuItem  *PopMoveToWinItem;
	TMenuItem  *PopNewClipItem;
	TMenuItem  *PopNewItem;
	TMenuItem  *PopPasteExItem;
	TMenuItem  *PopPasteItem;
	TMenuItem  *PopPropertyItem;
	TMenuItem  *PopPropertyItem2;
	TMenuItem  *PopRedoPosItem;
	TMenuItem  *PopRedoPosItem2;
	TMenuItem  *PopSelAllItem;
	TMenuItem  *PopSelGroupItem;
	TMenuItem  *PopSpreadItem;
	TMenuItem  *PopStretchItem;
	TMenuItem  *PopUndoPosItem;
	TMenuItem  *PopUndoPosItem2;
	TMenuItem *CopyItem;
	TMenuItem *CsvExportItem;
	TMenuItem *CutItem;
	TMenuItem *E_s1;
	TMenuItem *E_s2;
	TMenuItem *E_s3;
	TMenuItem *E_s4;
	TMenuItem *E_s5;
	TMenuItem *E_s6;
	TMenuItem *E_s7;
	TMenuItem *Edit1;
	TMenuItem *EditAwayItem;
	TMenuItem *EditDeleteItem;
	TMenuItem *EditFindItem;
	TMenuItem *EditGatherItem;
	TMenuItem *EditGrpStretchItem;
	TMenuItem *EditMovePosItem;
	TMenuItem *EditMoveToGroupItem;
	TMenuItem *EditMoveToWinItem;
	TMenuItem *EditNewClipItem;
	TMenuItem *EditNewItem;
	TMenuItem *EditNextSelItem;
	TMenuItem *EditPrevSelItem;
	TMenuItem *EditPropertyItem;
	TMenuItem *EditRedoPosItem;
	TMenuItem *EditSelAllItem;
	TMenuItem *EditSelBgColItem;
	TMenuItem *EditSelGroupItem;
	TMenuItem *EditSpreadItem;
	TMenuItem *EditStretchItem;
	TMenuItem *EditTrimItem;
	TMenuItem *EditUndoPosItem;
	TMenuItem *F_s1;
	TMenuItem *F_s2;
	TMenuItem *F_s3;
	TMenuItem *File1;
	TMenuItem *FileAllSaveEndItem;
	TMenuItem *FileCloseItem;
	TMenuItem *FileExitItem;
	TMenuItem *FileExportItem;
	TMenuItem *FileHistoryItem;
	TMenuItem *FileImgOutItem;
	TMenuItem *FileImportItem;
	TMenuItem *FileMergeItem;
	TMenuItem *FileNewItem;
	TMenuItem *FileOpenItem;
	TMenuItem *FileSaveAsItem;
	TMenuItem *FileSaveItem;
	TMenuItem *G_s1;
	TMenuItem *GFrmLcolorItem;
	TMenuItem *Gl_s3;
	TMenuItem *GLineColItem;
	TMenuItem *GLineJncItem;
	TMenuItem *GLineOpsItem;
	TMenuItem *GLineRevItem;
	TMenuItem *GLineType5Item;
	TMenuItem *GListClrAllItem;
	TMenuItem *GListHideBlkItem;
	TMenuItem *Go1;
	TMenuItem *GoBackItem;
	TMenuItem *GoForwardItem;
	TMenuItem *GoHomeItem;
	TMenuItem *GoJumpItem;
	TMenuItem *H_s1;
	TMenuItem *H_s2;
	TMenuItem *Help1;
	TMenuItem *HelpAboutItem;
	TMenuItem *HelpContentsItem;
	TMenuItem *HelpHomePageItem;
	TMenuItem *IDI1;
	TMenuItem *IDI2;
	TMenuItem *O_s1;
	TMenuItem *Option1;
	TMenuItem *OptionScrSizeItem;
	TMenuItem *OptionSetupItem;
	TMenuItem *P_s6;
	TMenuItem *P2_s4;
	TMenuItem *PasteExItem;
	TMenuItem *PasteItem;
	TMenuItem *PopMovePosItem;
	TMenuItem *PrintItem;
	TMenuItem *Show1;
	TMenuItem *ShowDivLineItem;
	TMenuItem *ShowFLineItem;
	TMenuItem *ShowFrgGridItem;
	TMenuItem *ShowGFrameItem;
	TMenuItem *ShowGLineItem;
	TMenuItem *ShowGrpLstItem;
	TMenuItem *ShowLinkMarkItem;
	TMenuItem *ShowMapViewerItem;
	TMenuItem *ShowNamePlateItem;
	TMenuItem *ShowPosGridItem;
	TMenuItem *ShowStatusBarItem;
	TMenuItem *ShowTool1Item;
	TMenuItem *ShowTool2Item;
	TMenuItem *ShowTool3Item;
	TMenuItem *ShowToolBarItem;
	TMenuItem *T_s1;
	TMenuItem *TABT1;
	TMenuItem *TextExportItem;
	TMenuItem *TextImportItem;
	TMenuItem *Tool1;
	TMenuItem *ToolBrushItem;
	TMenuItem *ToolFindExItem;
	TMenuItem *ToolGLineItem;
	TMenuItem *ToolGroupItem;
	TMenuItem *ToolNrmItem;
	TMenuItem *ToolRubberLayItem;
	TMenuItem *V_s1;
	TMenuItem *V_s2;
	TMenuItem *W_s1;
	TMenuItem *Window1;
	TMenuItem *WindowActMaxItem;
	TMenuItem *WindowArrangeItem;
	TMenuItem *WindowCascadeItem;
	TMenuItem *WindowTileHItem;
	TMenuItem *WindowTileVItem;
	TOpenDialog *OpenDialog;
	TPanel *DockPanel1;
	TPanel *DockPanel2;
	TPanel *DockPanel3;
	TPanel *DockPanel4;
	TPanel		*BrushPanel;
	TPopupMenu *BgColPopupMenu;
	TPopupMenu *BrushPopupMenu;
	TPopupMenu *EditPopupMenu;
	TPopupMenu *EditPopupMenu2;
	TPopupMenu *GFrmPopupMenu;
	TPopupMenu *GLinePopupMenu;
	TPopupMenu *GlistPopupMenu;
	TPrintDialog *PrintDialog1;
	TSaveDialog *SaveDialog;
	TSplitter *Splitter1;
	TStatusBar *StatusBar;
	TTimer *Timer1;
	TToolBar	*ToolBar1;
	TToolBar	*ToolBar2;
	TToolBar	*ToolBar3;
	TToolButton *CopyBtn;
	TToolButton *CutBtn;
	TToolButton *EditAwayBtn;
	TToolButton *EditBrushBtn;
	TToolButton *EditFindBtn;
	TToolButton *EditGatherBtn;
	TToolButton *EditGLineBtn;
	TToolButton *EditGroupBtn;
	TToolButton *EditGStretchBtn;
	TToolButton *EditNewBtn;
	TToolButton *EditNextSelBtn;
	TToolButton *EditNormalBtn;
	TToolButton *EditPrevSelBtn;
	TToolButton *EditRedoPosBtn;
	TToolButton *EditSpreadBtn;
	TToolButton *EditStretchBtn;
	TToolButton *EditUndoPosBtn;
	TToolButton *GoBackBtn;
	TToolButton *GoForwardBtn;
	TToolButton *GoHomeBtn;
	TToolButton *NewBtn;
	TToolButton *OpenBtn;
	TToolButton *PasteBtn;
	TToolButton *PrintBtn;
	TToolButton *SaveBtn;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TToolButton *ToolButton5;
	TToolButton *ToolButton8;
	TToolButton *ToolButton10;
	TWindowCascade *WindowCascade;
	TWindowTileHorizontal *WindowTileH;
	TWindowTileVertical *WindowTileV;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall UpdateMenuGrpLst(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);

	void __fastcall File1Click(TObject *Sender);
	void __fastcall Edit1Click(TObject *Sender);
	void __fastcall Window1Click(TObject *Sender);

	void __fastcall FileNewExecute(TObject *Sender);
	void __fastcall FileCloseExecute(TObject *Sender);
	void __fastcall FileOpenExecute(TObject *Sender);
	void __fastcall FileHstItemClick(TObject *Sender);
	void __fastcall FileSaveExecute(TObject *Sender);
	void __fastcall FileSaveAsExecute(TObject *Sender);
	void __fastcall FileAllSaveEndExecute(TObject *Sender);
	void __fastcall FileMergeExecute(TObject *Sender);
	void __fastcall TextImportExecute(TObject *Sender);
	void __fastcall TextExportExecute(TObject *Sender);
	void __fastcall FliePrintExecute(TObject *Sender);
	void __fastcall FileImgOutExecute(TObject *Sender);
	void __fastcall EditCutExecute(TObject *Sender);
	void __fastcall EditCopyExecute(TObject *Sender);
	void __fastcall EditPasteExecute(TObject *Sender);
	void __fastcall EditPasteExExecute(TObject *Sender);
	void __fastcall EditPropertyExecute(TObject *Sender);
	void __fastcall EditNewExecute(TObject *Sender);
	void __fastcall EditNewClipExecute(TObject *Sender);
	void __fastcall EditDeleteExecute(TObject *Sender);
	void __fastcall EditSelAllExecute(TObject *Sender);
	void __fastcall EditSelGroupItemClick(TObject *Sender);
	void __fastcall EditSelGrpItemClick(TObject *Sender);
	void __fastcall EditSelBgColExecute(TObject *Sender);
	void __fastcall SelBgColItemDrawItem(TObject *Sender, TCanvas *ACanvas, const TRect &ARect, bool Selected);
	void __fastcall SelBgColItemMeasureItem(TObject *Sender, TCanvas *ACanvas, int &Width, int &Height);
	void __fastcall SelBgColItemClick(TObject *Sender);
	void __fastcall EditGatherExecute(TObject *Sender);
	void __fastcall EditPrevSelExecute(TObject *Sender);
	void __fastcall EditNextSelExecute(TObject *Sender);
	void __fastcall EditSpreadExecute(TObject *Sender);
	void __fastcall EditStretchExecute(TObject *Sender);
	void __fastcall EditGrpStretchExecute(TObject *Sender);
	void __fastcall EditAwayExecute(TObject *Sender);
	void __fastcall EditUndoExecute(TObject *Sender);
	void __fastcall EditRedoExecute(TObject *Sender);
	void __fastcall EditMoveToGroupItemClick(TObject *Sender);
	void __fastcall MoveToGroupItemClick(TObject *Sender);
	void __fastcall EditMoveToWinItemClick(TObject *Sender);
	void __fastcall MovToWinItemClick(TObject *Sender);
	void __fastcall EditFindExecute(TObject *Sender);
	void __fastcall GoJumpExecute(TObject *Sender);
	void __fastcall GoHomeExecute(TObject *Sender);
	void __fastcall GoBackExecute(TObject *Sender);
	void __fastcall GoForwardExecute(TObject *Sender);
	void __fastcall JumpListItemClick(TObject *Sender);
	void __fastcall Show1Click(TObject *Sender);
	void __fastcall ShowFLineExecute(TObject *Sender);
	void __fastcall ShowGrpLstUpdate(TObject *Sender);
	void __fastcall ShowFrgGridExecute(TObject *Sender);
	void __fastcall ShowGrpLstExecute(TObject *Sender);
	void __fastcall ShowStatusBarExecute(TObject *Sender);
	void __fastcall ShowToolItemClick(TObject *Sender);
	void __fastcall ShowMapViewerExecute(TObject *Sender);
	void __fastcall ShowNamePlateExecute(TObject *Sender);
	void __fastcall ShowLinkMarkExecute(TObject *Sender);
	void __fastcall ShowGFrameExecute(TObject *Sender);
	void __fastcall ShowGLineExecute(TObject *Sender);
	void __fastcall ShowDivLineExecute(TObject *Sender);
	void __fastcall WindowCascadeExecute(TObject *Sender);
	void __fastcall WindowTileHExecute(TObject *Sender);
	void __fastcall WindowTileVExecute(TObject *Sender);
	void __fastcall WindowArrangeExecute(TObject *Sender);
	void __fastcall SelWinItemClick(TObject *Sender);
	void __fastcall OptionSetupExecute(TObject *Sender);
	void __fastcall OptionIndividualExecute(TObject *Sender);
	void __fastcall HelpContentsExecute(TObject *Sender);
	void __fastcall HelpHomePageExecute(TObject *Sender);
	void __fastcall HelpAboutExecute(TObject *Sender);

	void __fastcall EditPopupMenuPopup(TObject *Sender);
	void __fastcall GlistPopupMenuPopup(TObject *Sender);
	void __fastcall GListPosItemClick(TObject *Sender);
	void __fastcall GListFloatItemClick(TObject *Sender);
	void __fastcall GListUpItemClick(TObject *Sender);
	void __fastcall GListDownItemClick(TObject *Sender);
	void __fastcall GListHideItemClick(TObject *Sender);

	void __fastcall GroupListBoxClick(TObject *Sender);
	void __fastcall GroupListBoxDblClick(TObject *Sender);
	void __fastcall GroupListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall GroupListBoxStartDock(TObject *Sender, TDragDockObject *&DragObject);
	void __fastcall GroupListBoxEndDock(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall DockPanelDockDrop(TObject *Sender, TDragDockObject *Source, int X, int Y);
	void __fastcall DockPanelUnDock(TObject *Sender, TControl *Client, TWinControl *NewTarget, bool &Allow);
	void __fastcall GroupListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);

	void __fastcall GLineDelItemClick(TObject *Sender);
	void __fastcall GLineTypeItemClick(TObject *Sender);

	void __fastcall ShowHint(TObject *Sender);
	void __fastcall GLinePopupMenuPopup(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ToolPanelDockOver(TObject *Sender, TDragDockObject *Source, int X, int Y, TDragState State, bool &Accept);
	void __fastcall DockPanelDockOver(TObject *Sender, TDragDockObject *Source, int X, int Y, TDragState State, bool &Accept);
	void __fastcall Tool1Click(TObject *Sender);
	void __fastcall ToolNormalExecute(TObject *Sender);
	void __fastcall ToolGLineExecute(TObject *Sender);
	void __fastcall ToolFindExExecute(TObject *Sender);
	void __fastcall ToolBrushExecute(TObject *Sender);
	void __fastcall BrushPanelClick(TObject *Sender);
	void __fastcall GroupListBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall BrushChsBgColItemClick(TObject *Sender);

	void __fastcall ChildUpdate(TObject *Sender);
	void __fastcall EditMultiSelUpdate(TObject *Sender);
	void __fastcall EditChildUpdate(TObject *Sender);
	void __fastcall EditSelUpdate(TObject *Sender);
	void __fastcall CanOpenUpdate(TObject *Sender);
	void __fastcall SelUpdate(TObject *Sender);
	void __fastcall EditPasteUpdate(TObject *Sender);
	void __fastcall EditPasteExUpdate(TObject *Sender);
	void __fastcall UndoPosUpdate(TObject *Sender);
	void __fastcall RedoPosUpdate(TObject *Sender);
	void __fastcall FilePrintUpdate(TObject *Sender);
	void __fastcall NewClipUpdate(TObject *Sender);
	void __fastcall PrevSelUpdate(TObject *Sender);
	void __fastcall NextSelUpdate(TObject *Sender);
	void __fastcall GoBackUpdate(TObject *Sender);
	void __fastcall GoForwardUpdate(TObject *Sender);
	void __fastcall GoHomeUpdate(TObject *Sender);
	void __fastcall DispStatusUpdate(TObject *Sender);
	void __fastcall ExMoveExecute(TObject *Sender);
	void __fastcall GroupListBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall GroupListBoxDragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall GroupListBoxDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept);
	void __fastcall GroupListBoxEndDrag(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall ToolGroupExecute(TObject *Sender);
	void __fastcall DockPanelGetSiteInfo(TObject *Sender,
		  TControl *DockClient, TRect &InfluenceRect, TPoint &MousePos, bool &CanDock);
	void __fastcall CsvExportExecute(TObject *Sender);
	void __fastcall EditGrpStretchUpdate(TObject *Sender);
	void __fastcall GListDispItemClick(TObject *Sender);
	void __fastcall GroupListBoxMeasureItem(TWinControl *Control, int Index, int &Height);
	void __fastcall GListHideBlkItemClick(TObject *Sender);
	void __fastcall GroupListBoxEnter(TObject *Sender);
	void __fastcall GroupListBoxExit(TObject *Sender);
	void __fastcall GFrmRelItemClick(TObject *Sender);
	void __fastcall GFrmSelItemClick(TObject *Sender);
	void __fastcall GFrmNameItemClick(TObject *Sender);
	void __fastcall GFrmPopupMenuPopup(TObject *Sender);
	void __fastcall GLineLblItemClick(TObject *Sender);
	void __fastcall GFrmSetLwItemClick(TObject *Sender);
	void __fastcall WindowActMaxExecute(TObject *Sender);
	void __fastcall ShowPosGridExecute(TObject *Sender);
	void __fastcall Go1Click(TObject *Sender);
	void __fastcall CopyAnchorActionExecute(TObject *Sender);
	void __fastcall EditSelOneUpdate(TObject *Sender);
	void __fastcall FileSaveUpdate(TObject *Sender);
	void __fastcall GFrmLcolorItemClick(TObject *Sender);
	void __fastcall GLineColItemClick(TObject *Sender);
	void __fastcall EditTrimExecute(TObject *Sender);
	void __fastcall RubberLayExecute(TObject *Sender);
	void __fastcall GLineOpsItemClick(TObject *Sender);
	void __fastcall TsvExportExecute(TObject *Sender);
	void __fastcall GLineJncItemClick(TObject *Sender);
	void __fastcall EditPropertyUpdate(TObject *Sender);
	void __fastcall GLineRevItemClick(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall ToolBarsStartDock(TObject *Sender, TDragDockObject *&DragObject);
	void __fastcall ApplicationEvents1Message(tagMSG &Msg, bool &Handled);
	void __fastcall EditMovePosExecute(TObject *Sender);
	void __fastcall ActionList1Execute(TBasicAction *Action, bool &Handled);
	void __fastcall ApplicationEvents1ModalBegin(TObject *Sender);
	bool __fastcall ApplicationEvents1Help(WORD Command, NativeInt Data, bool &CallHelp);
	void __fastcall ApplicationEvents1ModalEnd(TObject *Sender);

private:
	UnicodeString StartFile;		//コマンドラインで指定の開始ファイル
	bool  GlDblSkip;
	TRect LastItemRect;
	TPoint PopupPos;				//ポップアップメニュー表示時のマウス位置

	void __fastcall UpdateActiveChild();
	void __fastcall FloatGrpLst();
	void __fastcall SetGrpLstPos(TPanel *pSite);
	void __fastcall DockGrpFrgList();
	void __fastcall WmCopyData(TMessage &msg);
	void __fastcall WmQueryEndSession(TMessage &msg);
	void __fastcall WmGetMinMaxInfo(TWMGetMinMaxInfo &Msg);
	void __fastcall WmAppCommand(TMessage &msg);
	void __fastcall WmDropped(TMessage &msg);

	FragSet * __fastcall GetCurFS();

public:
	virtual __fastcall TIdeaFragMainForm(TComponent *Owner);
	TMDIChild *__fastcall CreateMDIChild(const String Name);
	void __fastcall CloseIME();
	void __fastcall UpdateGrpLst();
	void __fastcall SaveGrpLstPos();

	__property FragSet *CurFS = {read = GetCurFS};

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_COPYDATA, 		TMessage,			WmCopyData)
		VCL_MESSAGE_HANDLER(WM_QUERYENDSESSION,	TMessage,			WmQueryEndSession)
		VCL_MESSAGE_HANDLER(WM_GETMINMAXINFO,	TWMGetMinMaxInfo,	WmGetMinMaxInfo)
		VCL_MESSAGE_HANDLER(WM_APPCOMMAND,		TMessage,			WmAppCommand)
		VCL_MESSAGE_HANDLER(WM_FORM_DROPPED,	TMessage,			WmDropped)
	END_MESSAGE_MAP(TForm);
};
//----------------------------------------------------------------------------
extern TIdeaFragMainForm  *IdeaFragMainForm;

extern FragBuffer *FSbuf;		//カットバッファ

//----------------------------------------------------------------------------
#endif
