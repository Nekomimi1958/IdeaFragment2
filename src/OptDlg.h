//----------------------------------------------------------------------//
// オプション設定ダイアログ												//
//																		//
//----------------------------------------------------------------------//
#ifndef OptDlgH
#define OptDlgH
//----------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.ActnList.hpp>
#include "arwline.h"
#include "MarkList.h"

//----------------------------------------------------------------------------
class TOptionDlg : public TForm
{
__published:
	TAction *AddExpItemAction;
	TAction *ConfigSpiAction;
	TAction *DelExpItemAction;
	TAction *DisableColAction;
	TAction *DowExpItemAction;
	TAction *OkAction;
	TAction *UpExpItemAction;
	TActionList *ActionList1;
	TButton *AddExpItemBtn;
	TButton *AsoAddBtn;
	TButton *AsoChgBtn;
	TButton *AsoDelBtn;
	TButton *AsoRefBtn;
	TButton *CancelBtn;
	TButton *ClrBtn;
	TButton *ColButton;
	TButton *ConfSpiBtn;
	TButton *DelExpItemBtn;
	TButton *DelExtBtn;
	TButton *DisColBtn;
	TButton *DowExpItemBtn;
	TButton *FontButton;
	TButton *HomWinBtn;
	TButton *OKBtn;
	TButton *RegExtBtn;
	TButton *SKChgBtn;
	TButton *SpiRefBtn;
	TButton *UpExpItemBtn;
	TButton	*SetCurHomBtn;
	TCheckBox *AcceptCrCheckBox;
	TCheckBox *ActDstWinCheckBox;
	TCheckBox *AlignCheckBox;
	TCheckBox *AltCheckBox;
	TCheckBox *AlwaysShowCheckBox;
	TCheckBox *AutoExpCheckBox;
	TCheckBox *ChgGrpModeCtrlCheckBox;
	TCheckBox *ChildMaxCheckBox;
	TCheckBox *CopyCLineCheckBox;
	TCheckBox *CrRetKeyCheckBox;
	TCheckBox *CtlRetOkCheckBox;
	TCheckBox *CtrlCheckBox;
	TCheckBox *DropImgCheckBox;
	TCheckBox *ExportUtf8CheckBox;
	TCheckBox *FitMapVCheckBox;
	TCheckBox *FrmAllSelCheckBox;
	TCheckBox *GlRightAngCheckBox;
	TCheckBox *HideTitleCheckBox;
	TCheckBox *HomWinMaxCheckBox;
	TCheckBox *ImeOnCheckBox;
	TCheckBox *ImgIsBakCheckBox;
	TCheckBox *LnkIconCheckBox;
	TCheckBox *LnkRelCheckBox;
	TCheckBox *MapVSelRctCheckBox;
	TCheckBox *NewBlkDblClickCheckBox;
	TCheckBox *NewDropPosCheckBox;
	TCheckBox *NoGrpCheckBox;
	TCheckBox *NoNpHideGfrmCheckBox;
	TCheckBox *OnlyNpHideGfrmCheckBox;
	TCheckBox *OpenLastCheckBox;
	TCheckBox *PlateOnGfrmCheckBox;
	TCheckBox *PosSortCheckBox;
	TCheckBox *PrintBgCheckBox;
	TCheckBox *PrintMonoCheckBox;
	TCheckBox *PrintScrSizeCheckBox;
	TCheckBox *PrintTimeCheckBox;
	TCheckBox *RDragScrollCheckBox;
	TCheckBox *RstScrPosCheckBox;
	TCheckBox *SelJumpFrgCheckBox;
	TCheckBox *ShiftCheckBox;
	TCheckBox *ShiftHorzCheckBox;
	TCheckBox *ShowDstGrpCheckBox;
	TCheckBox *ShuffleCheckBox;
	TCheckBox *SnapCheckBox;
	TCheckBox *TrmFixCheckBox;
	TCheckBox *TrmMovCheckBox;
	TCheckBox *TrmSavCheckBox;
	TCheckBox *TrmSizCheckBox;
	TCheckBox *UseSpiFirstCheckBox;
	TCheckBox *WdWrapCheckBox;
	TColorDialog *ColorDialog1;
	TComboBox *AwayPosCombo;
	TComboBox *FontComboBox;
	TComboBox *KeyComboBox;
	TComboBox *MinPrnLWCombo;
	TComboBox *MovCsrCombo;
	TComboBox *NewPosCombo;
	TComboBox *ParentCombo;
	TComboBox *PlateAlignComboBox;
	TComboBox *PlateShapeComboBox;
	TComboBox *ScaleOptComboBox;
	TComboBox *SizSpComboBox;
	TComboBox *SizValComboBox;
	TEdit *ArngPtEdit;
	TEdit *AsoAppEdit;
	TEdit *HomeWinEdit;
	TEdit *ImgR1Edit;
	TEdit *ImgR2Edit;
	TFontDialog *FontDialog1;
	TGroupBox *ColorGroupBox;
	TGroupBox *ExportGroupBox;
	TGroupBox *FontGroupBox;
	TGroupBox *FragSizeGroupBox;
	TGroupBox *GridGroupBox;
	TGroupBox *HomeGroupBox;
	TGroupBox *ImageGroupBox;
	TGroupBox *KeyGroupBox;
	TGroupBox *PaintGroupBox;
	TGroupBox *PlateSizeGroupBox;
	TGroupBox *RegIfdGroupBox;
	TGroupBox *ReturnGroupBox;
	TGroupBox *ScrSizeGroupBox;
	TGroupBox *SizeGroupBox;
	TGroupBox *SusieGroupBox;
	TGroupBox *TrimGroupBox;
	TGroupBox *WheelGroupBox;
	TImage *NrmFrgImage;
	TImage *PlateImage;
	TImage *SelFrgImage;
	TLabel *AdminLabel;
	TLabel *JpgQLabel;
	TLabel *SymLabel;
	TLabeledEdit *AsoExtEdit;
	TLabeledEdit *CrStrEdit;
	TLabeledEdit *DefHiEdit;
	TLabeledEdit *DefWdEdit;
	TLabeledEdit *FindEdit;
	TLabeledEdit *GridSzEdit;
	TLabeledEdit *MapAlphaEdit;
	TLabeledEdit *NamHiEdit;
	TLabeledEdit *NamWdEdit;
	TLabeledEdit *ScrHiEdit;
	TLabeledEdit *ScrQtyEdit;
	TLabeledEdit *ScrWdEdit;
	TLabeledEdit *SpiDirEdit;
	TLabeledEdit *TrmHiEdit;
	TLabeledEdit *TrmMgnEdit;
	TLabeledEdit *TrmWdEdit;
	TListBox *AssociateListBox;
	TListBox *ColListBox;
	TListBox *ExpItemListBox1;
	TListBox *ExpItemListBox2;
	TListBox *GrpCheckListBox;
	TListBox *ShortCutList;
	TListBox *SpiListBox;
	TOpenDialog *OpenDialog1;
	TPageControl *PageControl1;
	TPaintBox *FLinePBox;
	TPaintBox *GLinePBox;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *SamplePanel;
	TShape *GrpFrmShape;
	TSplitter *Splitter1;
	TStatusBar *StatusBar1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TTabSheet *TabSheet5;
	TTabSheet *TabSheet6;
	TTabSheet *TabSheet7;
	TTabSheet *TabSheet8;
	TTabSheet *TabSheet9;
	TTrackBar *JpgQTrackBar;
	TUpDown *ArngPtUpDown;
	TUpDown *GridSzUpDown;
	TUpDown *HiUpDown;
	TUpDown *ImgR1UpDown;
	TUpDown *ImgR2UpDown;
	TUpDown *NpHiUpDown;
	TUpDown *NpWdUpDown;
	TUpDown *ScrHiUpDown;
	TUpDown *ScrWdUpDown;
	TUpDown *TrmHiUpDown;
	TUpDown *TrmMgnUpDown;
	TUpDown *TrmWdUpDown;
	TUpDown *WdUpDown;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall SKChgBtnClick(TObject *Sender);
	void __fastcall ClrBtnClick(TObject *Sender);
	void __fastcall HomWinBtnClick(TObject *Sender);
	void __fastcall SetCurHomBtnClick(TObject *Sender);
	void __fastcall TabSheet1Show(TObject *Sender);
	void __fastcall AcceptCrCheckBoxClick(TObject *Sender);
	void __fastcall ColButtonClick(TObject *Sender);
	void __fastcall FontButtonClick(TObject *Sender);
	void __fastcall FontComboBoxChange(TObject *Sender);
	void __fastcall GrpCheckListBoxDrawItem(TWinControl *Control, int Index,
		  TRect &Rect, TOwnerDrawState State);
	void __fastcall GrpCheckListBoxClick(TObject *Sender);
	void __fastcall SamplePanelMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SymLabelClick(TObject *Sender);
	void __fastcall StatusBar1Click(TObject *Sender);
	void __fastcall GLinePBoxPaint(TObject *Sender);
	void __fastcall GLinePBoxMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall ShortCutListClick(TObject *Sender);
	void __fastcall ParentComboClick(TObject *Sender);
	void __fastcall SpiRefBtnClick(TObject *Sender);
	void __fastcall FLinePBoxMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FLinePBoxPaint(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall SizSpComboBoxChange(TObject *Sender);
	void __fastcall SizValComboBoxChange(TObject *Sender);
	void __fastcall JpgQTrackBarChange(TObject *Sender);
	void __fastcall AssociateListBoxClick(TObject *Sender);
	void __fastcall AsoAddBtnClick(TObject *Sender);
	void __fastcall AsoChgBtnClick(TObject *Sender);
	void __fastcall AsoDelBtnClick(TObject *Sender);
	void __fastcall AsoRefBtnClick(TObject *Sender);
	void __fastcall OkActionExecute(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);
	void __fastcall RegExtBtnClick(TObject *Sender);
	void __fastcall DelExtBtnClick(TObject *Sender);
	void __fastcall DefWdEditChange(TObject *Sender);
	void __fastcall DefHiEditChange(TObject *Sender);
	void __fastcall NamWdEditChange(TObject *Sender);
	void __fastcall NamHiEditChange(TObject *Sender);
	void __fastcall ImgR1EditChange(TObject *Sender);
	void __fastcall ImgR2EditChange(TObject *Sender);
	void __fastcall ScrWdEditChange(TObject *Sender);
	void __fastcall ScrHiEditChange(TObject *Sender);
	void __fastcall ArngPtEditChange(TObject *Sender);
	void __fastcall GridSzEditChange(TObject *Sender);
	void __fastcall TrmMgnEditChange(TObject *Sender);
	void __fastcall TrmWdEditChange(TObject *Sender);
	void __fastcall TrmHiEditChange(TObject *Sender);
	void __fastcall AddExpItemActionExecute(TObject *Sender);
	void __fastcall AddExpItemActionUpdate(TObject *Sender);
	void __fastcall DelExpItemActionExecute(TObject *Sender);
	void __fastcall DelExpItemActionUpdate(TObject *Sender);
	void __fastcall UpExpItemActionExecute(TObject *Sender);
	void __fastcall UpExpItemActionUpdate(TObject *Sender);
	void __fastcall DowExpItemActionExecute(TObject *Sender);
	void __fastcall DowExpItemActionUpdate(TObject *Sender);
	void __fastcall NumberEditExit(TObject *Sender);
    void __fastcall PrintMonoCheckBoxClick(TObject *Sender);
	void __fastcall PageControl1DrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect, bool Active);
	void __fastcall FindBtnClick(TObject *Sender);
	void __fastcall ConfigSpiActionExecute(TObject *Sender);
	void __fastcall ConfigSpiActionUpdate(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall FontComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall FindEditChange(TObject *Sender);
	void __fastcall ShortCutListDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall DisableColActionExecute(TObject *Sender);
	void __fastcall DisableColActionUpdate(TObject *Sender);
	void __fastcall ColListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);

private:
	ArrowLine SampleGLine;
	ArrowLine SampleFLine;

	UnicodeString CurFileName;

	TColor colbuf_MapBG;
	TColor colbuf_AreaFrm;
	TColor colbuf_glstOff;
	TColor colbuf_GrpLine;
	TColor colbuf_Title;
	TColor colbuf_FrgLine;
	TColor colbuf_LinLbl;
	TColor colbuf_selRect;
	TColor colbuf_DivLine;
	TColor colbuf_PosGrid;
	TColor colbuf_xlstFG;
	TColor colbuf_xlstBG;
	TColor colbuf_OptFind;

	int gfrmMaxRnd;
	int GLstInterLine;
	int FLstInterLine;
	int XLstInterLine;
	int FrgMargin;

	TFont *TitleFont;
	TFont *LinLblFont;
	TFont *FrgLstFont;
	TFont *ExLstFont;
	TFont *DialogFont;

	MarkList *FindMarkList;

	TColor __fastcall GetIdxColor(int idx);
	void __fastcall SetSpiInfo(UnicodeString spidir);
	void __fastcall FrgTextOut(TImage *img, UnicodeString s);

public:
	virtual __fastcall TOptionDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TOptionDlg *OptionDlg;
//----------------------------------------------------------------------------
#endif
