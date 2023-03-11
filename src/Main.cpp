//----------------------------------------------------------------------//
//IdeaFragment2															//
//	���C���t�H�[��														//
//																		//
//----------------------------------------------------------------------//
#include "usr_wic.h"
#include "UIniFile.h"
#include "UserFunc.h"
#include "about.h"
#include "OptDlg.h"
#include "PropDlg.h"
#include "MapView.h"
#include "FrgGrid.h"
#include "FindDlg.h"
#include "Stretch.h"
#include "OptIdDlg.h"
#include "FindEx.h"
#include "RubrLay.h"
#include "Main.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"

TIdeaFragMainForm *IdeaFragMainForm;		//���C���t�H�[��

//---------------------------------------------------------------------------
//���[�J���t�b�N
//---------------------------------------------------------------------------
HHOOK hDlgHook;
//---------------------------------------------------------------------------
LRESULT CALLBACK DlgHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code<0) return ::CallNextHookEx(hDlgHook, code, wParam, lParam);

	if (code==HC_ACTION) {
		PCWPSTRUCT sp = (PCWPSTRUCT)lParam;
		if (sp->message==WM_ACTIVATE && (LOWORD(sp->wParam)==WA_ACTIVE)) {
			//�|�C���^�[�������I�ɋK��̃{�^����Ɉړ�
			if (is_SnapToDefBtn()) {
				TForm *frm = NULL;
				for (int i=0; i<Screen->FormCount && !frm; i++) {
					TForm *fp = Screen->Forms[i];
					if (fp->Handle==sp->hwnd) frm = fp;
				}
				if (frm && frm!=Application->MainForm) {
					for (int i=frm->ComponentCount - 1; i>=0;  i--) {
						TComponent *cp = frm->Components[i];
						if (cp->ClassNameIs("TButton")) {
							TButton *bp = (TButton *)cp;
							if (bp->Default) {
								Mouse->CursorPos = Point(bp->ClientOrigin.x + bp->Width/2, bp->ClientOrigin.y + bp->Height/2);
								break;
							}
						}
					}
				}
			}
		}
	}

	return ::CallNextHookEx(hDlgHook, code, wParam, lParam);
}

//---------------------------------------------------------------------------
HINSTANCE hHHctrl = NULL;
DWORD	  Cookie  = NULL;

FragBuffer *FSbuf;		//�J�b�g�o�b�t�@

//---------------------------------------------------------------------------
__fastcall TIdeaFragMainForm::TIdeaFragMainForm(TComponent *Owner) : TForm(Owner)
{
}

//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FormCreate(TObject *Sender)
{
	::GetWindowThreadProcessId(Handle, &ProcessId);		//�v���Z�XID���擾

	hDlgHook = ::SetWindowsHookEx(WH_CALLWNDPROC, DlgHookProc, NULL, ::GetCurrentThreadId());

	DropInitialize(Handle);

	//�N���I�v�V�����̏���
	StartFile = EmptyStr;
	UnicodeString ini_path;
	UnicodeString cdstr = IncludeTrailingPathDelimiter(GetCurrentDir());

	int i = 1;
	while (!ParamStr(i).IsEmpty()) {
		UnicodeString prmbuf = ParamStr(i++).Trim();
		//�I�v�V����
		if (StartsStr("-", prmbuf)) {
			if (prmbuf.SubString(2, 1).UpperCase()=="I") {
				prmbuf.Delete(1, 2);
				if (prmbuf.IsEmpty())
					ini_path = cdstr + "IdeaFrag.INI";	//�J�����g�f�B���N�g�� + "IdeaFrag.INI"
				else {
					if (ExtractFilePath(prmbuf).IsEmpty())
						ini_path = cdstr + prmbuf;		//�J�����g�f�B���N�g�� + INI�t�@�C����
					else
						ini_path = prmbuf;				//�w��f�B���N�g�� + INI�t�@�C����
				}
			}
		}
		//�t�@�C����
		else {
			if (!StartFile.IsEmpty()) StartFile += "\t";
			StartFile += prmbuf;
		}
	}

	//exe�f�B���N�g�� + "IdeaFrag.INI" (�f�t�H���g)
	if (ini_path.IsEmpty()) ini_path = ChangeFileExt(Application->ExeName, ".INI");

	//INI�t�@�C���擾
	IniFile = new UsrIniFile(ini_path);

	EV	  = new EnvData(this);
	FSbuf = new FragBuffer();

	WIC_FextStr = FEXT_WICSTD + WIC_get_fext_str();

	Application->OnHint = ShowHint;
	Screen->OnActiveFormChange = UpdateMenuGrpLst;

	EV->printer_ok = (Printer()->Printers->Count>0);

	//�w���v�t�@�C��������
	Application->HelpFile = ChangeFileExt(Application->ExeName, ".CHM");
	hHHctrl = ::LoadLibrary(_T("hhctrl.ocx"));
	if (hHHctrl) {
		lpfHtmlHelp = (FUNC_HtmlHelp)::GetProcAddress(hHHctrl, "HtmlHelpW");
		if (lpfHtmlHelp) lpfHtmlHelp(NULL, NULL, HH_INITIALIZE, (DWORD)&Cookie);
	}

	GlDblSkip = false;
	PopupPos  = Point(-1, -1);

	//�C���[�W�̃X�P�[�����O
	ResizeImageList(BtnImgList);
}

//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FormShow(TObject *Sender)
{
	CloseIME();

	//���C���E�B���h�E�̕\����Ԃ𕜌�
	EV->load_pos_info("General", (TForm*)this,
		(Screen->Width - Width)/2, (Screen->Height - Height)/2, 800, 600);

	StatusBar->Font->Assign(EV->stbarFont);
	StatusBar->Canvas->Font->Assign(EV->stbarFont);
	StatusBar->ClientHeight			   = StatusBar->Canvas->TextHeight("�f��") + 4;
	StatusBar->Panels->Items[0]->Width = StatusBar->Canvas->TextWidth("00/00 ");
	StatusBar->Panels->Items[1]->Width = StatusBar->Canvas->TextWidth("�f�А�=1000  /�I��=1000 ");
	StatusBar->Panels->Items[2]->Width = StatusBar->Canvas->TextWidth("x:99999,y:99999 (100%,100%) ");
	StatusBar->Visible = IniFile->ReadBool("StatusBar","Visible", true);
	DockPanel4->Align = alTop;		//�X�e�[�^�X�o�[�Ƃ̓���ւ��h�~
	DockPanel4->Align = alBottom;

	//�c�[���o�[�̕\����Ԃ𕜌�
	EV->load_tool_info("Tool1", ToolBar1);
	EV->load_tool_info("Tool2", ToolBar2);
	EV->load_tool_info("Tool3", ToolBar3);

	//�O���[�v���X�g��������
	GroupListBox->TabWidth	  = 20;
	GroupListBox->Font->Assign(EV->glstFont);
	GroupListBox->Font->Color = EV->col_glstFG;
	GroupListBox->Color 	  = EV->col_glstBG;
	GroupListBox->Style 	  = lbOwnerDrawVariable;
	GroupListBox->Clear();
	for (int gn=1; gn<=MAX_GROUP; gn++)
		GroupListBox->Items->Add(UnicodeString().sprintf(_T("___: �O���[�v%2u"), gn));
	GroupListBox->Items->Add("___: ������");
	GroupListBox->ItemIndex = 0;
	UnicodeString sct = "GroupList";
	GroupListBox->UndockWidth  = IniFile->ReadInteger(sct, "UnDockWidth",  200);
	GroupListBox->UndockHeight = IniFile->ReadInteger(sct, "UnDockHeight", 200);
	if (!IniFile->ReadBool(sct, "Floating")) {
		//�h�b�L���O
		if (SameText(IniFile->ReadString(sct, "DockSite"), "FragList")) {
			//�f�Ѓ��X�g�ƃh�b�L���O
			DockGrpFrgList();
		}
		else {
			//���C���Ƀh�b�L���O
			switch (EV->glstAlign) {
			case alLeft:   SetGrpLstPos(DockPanel1); break;
			case alTop:    SetGrpLstPos(DockPanel2); break;
			case alRight:  SetGrpLstPos(DockPanel3); break;
			case alBottom: SetGrpLstPos(DockPanel4); break;
			default:	   ;
			}
		}
	}
	//����
	else {
		FloatGrpLst();
	}

	GroupListBox->Visible = IniFile->ReadBool(sct, "Vislble", true);
	if (!GroupListBox->Visible) {
		if (FragGrid->DockPanel->Width>1) {
			FragGrid->DockPanel->Constraints->MinWidth = 0;
			FragGrid->DockPanel->Width = 1;
		}
	}

	//�t�@�C���������j���[���ڂ�������
	for (int i=0; i<MAX_FILEHISTORY; i++) {
		TMenuItem *mp = new TMenuItem(FileHistoryItem);
		mp->OnClick   = FileHstItemClick;
		mp->Tag 	  = i;
		FileHistoryItem->Add(mp);
	}
	//�O���[�v�I�����j���[���ڂ�������
	for (int i=0; i<MAX_GRP_ITM; i++) {
		TMenuItem *mp1 = new TMenuItem(EditSelGroupItem);
		TMenuItem *mp2 = new TMenuItem(PopSelGroupItem);
		mp1->OnClick   = EditSelGrpItemClick;
		mp2->OnClick   = EditSelGrpItemClick;
		mp1->Tag	   = i + 1;
		mp2->Tag	   = i + 1;
		EditSelGroupItem->Add(mp1);
		PopSelGroupItem->Add(mp2);
	}
	//���O���[�v�ֈړ����j���[���ڂ�������
	for (int i=0; i<MAX_GRP_ITM; i++) {
		TMenuItem *mp1 = new TMenuItem(EditMoveToGroupItem);
		TMenuItem *mp2 = new TMenuItem(PopMoveToGroupItem);
		mp1->OnClick   = MoveToGroupItemClick;
		mp2->OnClick   = MoveToGroupItemClick;
		mp1->Tag	   = i + 1;
		mp2->Tag	   = i + 1;
		EditMoveToGroupItem->Add(mp1);
		PopMoveToGroupItem->Add(mp2);
	}
	//���E�B���h�E�ֈړ����j���[���ڂ�������
	for (int i=0; i<MAX_FRGWIN; i++) {
		TMenuItem *mp1 = new TMenuItem(EditMoveToWinItem);
		TMenuItem *mp2 = new TMenuItem(PopMoveToWinItem);
		mp1->Break	   = (i==20)? mbBarBreak : mbNone;
		mp2->Break	   = mp1->Break;
		mp1->OnClick   = MovToWinItemClick;
		mp2->OnClick   = MovToWinItemClick;
		mp1->Tag	   = i;
		mp2->Tag	   = i;
		EditMoveToWinItem->Add(mp1);
		PopMoveToWinItem->Add(mp2);
	}
	for (int i=0; i<MAX_JUMPLIST; i++) {
		TMenuItem *mp = new TMenuItem(GoListItem);
		mp->OnClick   = JumpListItemClick;
		GoListItem->Add(mp);
	}

	//�E�B���h�E�I�����j���[���ڂ�������
	for (int i=0; i<MAX_FRGWIN; i++) {
		TMenuItem *mp = new TMenuItem(Window1);
		mp->OnClick   = SelWinItemClick;
		mp->Tag 	  = i;
		mp->Break	  = (i==20)? mbBarBreak : mbNone;
		Window1->Add(mp);
	}
	//�w�i�F�I�����j���[���ڂ�������
	UnicodeString tmpstr;
	for (int i=0; i<MAX_BGCOL_ITM; i++) {
		TMenuItem *mp = new TMenuItem(BgColPopupMenu->Items);
		if (i<10) tmpstr.sprintf(_T("&%u:"), (i + 1)%10);
		mp->Caption 	  = tmpstr;
		mp->OnMeasureItem = SelBgColItemMeasureItem;
		mp->OnDrawItem	  = SelBgColItemDrawItem;
		mp->OnClick 	  = SelBgColItemClick;
		BgColPopupMenu->Items->Add(mp);
	}

	EditNormalBtn->Down = true;
	BrushPanel->Color	= EV->col_Brush;

	//�V���[�g�J�b�g��ݒ�
	for (int i=0; i<ActionList1->ActionCount; i++) {
		TAction *ap  = (TAction*)ActionList1->Actions[i];
		ap->ShortCut = TextToShortCut(IniFile->ReadString("ShortCut", ap->Name + "Item"));
	}
	for (int i=0; i<FrgPropDlg->ActionList1->ActionCount; i++) {
		TAction *ap  = (TAction*)FrgPropDlg->ActionList1->Actions[i];
		ap->ShortCut = TextToShortCut(IniFile->ReadString("ShortCut2", ap->Name + "Item"));
	}
	GListUpItem->ShortCut   = TextToShortCut("Alt+Up");
	GListDownItem->ShortCut = TextToShortCut("Alt+Down");

	//---------------------------------------------------------------
	//�R�}���h���C���w��̃t�@�C��
	if (!StartFile.IsEmpty()) {
		TStringDynArray flst = SplitString(StartFile, "\t");
		for (int i=0; i<flst.Length; i++) CreateMDIChild(flst[i]);
	}
	//�O��Ō�ɃA�N�e�B�u�������t�@�C��
	else if (EV->OpenLastFile && !EV->LastFile.IsEmpty()) {
		CreateMDIChild(EV->LastFile);
	}
	//�z�[���E�B���h�E
	else if (!EV->HomeWindow.IsEmpty()) {
		TMDIChild *cp = CreateMDIChild(EV->HomeWindow);
		if (cp && EV->HomWinMax) cp->WindowState = wsMaximized;
	}

	//�S�̐}
	if (IniFile->ReadBool("MapViewer","Visible",true)) MapViewer->Show();
	//�f�Јꗗ
	if (IniFile->ReadBool("FragList","Visible")) FragGrid->Show();

	SetFocus();

	//�^�C�}�[�X�^�[�g
	EV->TimerLockCnt = 0;
	Timer1->Enabled  = true;

}	//<--- FormShow

//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FormCloseQuery(TObject *Sender,
	bool &CanClose)
{
	EV->LastFile = (CurFS)? CurFS->file_name : EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	//�^�C�}�[�X�g�b�v
	EV->TimerLockCnt = 1;
	Timer1->Enabled  = false;

	Screen->OnActiveFormChange = NULL;

	StatusBar->Panels->Items[3]->Text = "�I��������..."; StatusBar->Repaint();

	//�V���[�g�J�b�g�ۑ�
	for (int i=0; i<ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)ActionList1->Actions[i];
		IniFile->WriteString("ShortCut", ap->Name + "Item", ShortCutToText(ap->ShortCut));
	}
	for (int i=0; i<FrgPropDlg->ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)FrgPropDlg->ActionList1->Actions[i];
		IniFile->WriteString("ShortCut2", ap->Name + "Item", ShortCutToText(ap->ShortCut));
	}

	//��ʏ�Ԃ�ۑ�
	IniFile->WriteBool("StatusBar", "Visible",	StatusBar->Visible);
	IniFile->WriteBool("GroupList", "Vislble",	GroupListBox->Visible);
	IniFile->WriteBool("GroupList", "Floating",	GroupListBox->Floating);
	SaveGrpLstPos();

	IniFile->WriteBool("MapViewer", "Visible",	MapViewer->Visible);
	if (MapViewer->Visible)  MapViewer->Close();

	IniFile->WriteBool("FragList", "Visible",	FragGrid->Visible);
	if (FragGrid->Visible)   FragGrid->Close();

	if (FindExForm->Visible) FindExForm->Close();
	if (RubLayDlg->Visible)  RubLayDlg->Close();

	//�c�[���o�[�̕\����Ԃ�ۑ�
	EV->save_tool_info("Tool1", ToolBar1);
	EV->save_tool_info("Tool2", ToolBar2);
	EV->save_tool_info("Tool3", ToolBar3);

	EV->save_pos_info("General", (TForm*)this);

	//�e��ݒ��ۑ�
	if (!EV->save_ini_file()) msgbox_ERR("INI�t�@�C���̕ۑ��Ɏ��s���܂���");
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FormDestroy(TObject *Sender)
{
	::UnhookWindowsHookEx(hDlgHook);

	//��Еt��
	delete FSbuf;
	delete EV;
	delete IniFile;

	DropUninitialize();

	if (hHHctrl) {
		if (lpfHtmlHelp) {
			lpfHtmlHelp(NULL, NULL, HH_CLOSE_ALL, 0) ;
			lpfHtmlHelp(NULL, NULL, HH_UNINITIALIZE, (DWORD)Cookie);
		}
		::FreeLibrary(hHHctrl);
	}
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::WmQueryEndSession(TMessage& msg)
{
	Perform(WM_CLOSE, 0, (NativeInt)0);
	msg.Result = 1;
}

//---------------------------------------------------------------------------
//�w���v�A�z�C�[���Ȃǂ̏���
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ApplicationEvents1Message(tagMSG &Msg, bool &Handled)
{
	if (!Timer1->Enabled) return;

	//�w���v�̃L�[���b�Z�[�W����
	if (lpfHtmlHelp) {
		//ESC�L�[�ŕ���
		if (Msg.message==WM_KEYDOWN && (WORD)Msg.wParam==VK_ESCAPE) HtmlHelpClose(true);
		//���̑��̃��b�Z�[�W����
		if (Msg.message==WM_KEYDOWN || Msg.message==WM_SYSKEYDOWN) {
			Handled = (lpfHtmlHelp(NULL, NULL, HH_PRETRANSLATEMESSAGE, (DWORD)&Msg) != NULL);
			if (Handled) return;
		}
	}

	//�z�C�[��
	if (Msg.message==WM_MOUSEWHEEL) {
		int delta = GET_WHEEL_DELTA_WPARAM(Msg.wParam);

		int kmd = 0;
		int key = GET_KEYSTATE_WPARAM(Msg.wParam);
		if (key & MK_CONTROL) kmd |= 1;
		if (key & MK_SHIFT)   kmd |= 2;

		TMDIChild *cp = (TMDIChild*)ActiveMDIChild;
		if (cp) {
			if (Msg.hwnd==cp->Handle || Msg.hwnd==MapViewer->Handle) {
				//Shift+ �ŉ��X�N���[��
				if (EV->WheelShiftHorz && kmd==2) {
					if (cp->HorzScrollBar->Visible) cp->ScrollProc(((delta<0)? EV->WheelScrQty : -EV->WheelScrQty), 0);
				}
				//�c�X�N���[��
				else {
					if (cp->VertScrollBar->Visible) cp->ScrollProc(0, ((delta<0)? EV->WheelScrQty : -EV->WheelScrQty));
				}
				Handled = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
//���[�_���t�H�[�����J����
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ApplicationEvents1ModalBegin(TObject *Sender)
{
	HtmlHelpClose();
}
//---------------------------------------------------------------------------
//���[�_���t�H�[�������
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ApplicationEvents1ModalEnd(TObject *Sender)
{
	HtmlHelpClose();
}

//---------------------------------------------------------------------------
//�w���v
//---------------------------------------------------------------------------
bool __fastcall TIdeaFragMainForm::ApplicationEvents1Help(WORD Command, NativeInt Data,
	bool &CallHelp)
{
	if (Command==HELP_CONTEXT || Command==HELP_CONTEXTPOPUP) {
		//�V���[�g�J�b�g��F1���w�肳��Ă�����󋵊��m�^�w���v��}�~
		bool flag = true;
		for (int i=0; i<ActionList1->ActionCount; i++) {
			TAction *ap = (TAction*)ActionList1->Actions[i];
			if (ap->ShortCut == ShortCut(VK_F1, TShiftState())) {
				flag = false;	break;
			}
		}
		if (flag) {
			if (GroupListBox->Focused())
				HtmlHelpContext(Data, GroupListBox->Handle);
			else
				HtmlHelpContext(Data);
		}
		CallHelp = false;
	}
	return true;
}

//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FormPaint(TObject *Sender)
{
	//�e�[�}���L�����ƃc�[���o�[�̔w�i���ς���Ă��܂��̂�h��?
	TRect rc = ClientRect;
	rc.top += ToolPanel->Height;
	Canvas->Brush->Color = clAppWorkSpace;
	Canvas->FillRect(rc);
}

//---------------------------------------------------------------------------
//��d�N�����ꂽ IdeaFragment2 ����̏��ɂ��t�@�C�����J��
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::WmCopyData(TMessage& msg)
{
	COPYDATASTRUCT *cd = (COPYDATASTRUCT*)(msg.LParam);
	if (cd->dwData==1) {
		//�R�}���h���C���̉�� (�N�����I�v�V�����͖���)
		TStringDynArray prm_lst = get_prm_array((LPTSTR)cd->lpData);
		for (int i=0; i<prm_lst.Length; i++) {
			if (!StartsStr("-", prm_lst[i])) CreateMDIChild(prm_lst[i]);
		}
	}
	msg.Result = 0;
}

//---------------------------------------------------------------------
// �ő剻����␳(�^�C�g���o�[�ƃ��j���[���B���I�v�V�����p)
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::WmGetMinMaxInfo(TWMGetMinMaxInfo &Msg)
{
	TForm::Dispatch(&Msg);
	if (EV && EV->HideTitleMenu) {
		RECT rc;
		::GetWindowRect(GetDesktopWindow(),&rc);
		::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);
		rc.bottom -= 2;
		rc.top	  += 2;
		Msg.MinMaxInfo->ptMaxSize.y		 = rc.bottom - rc.top;
		Msg.MinMaxInfo->ptMaxTrackSize.y = rc.bottom - rc.top;
		Msg.MinMaxInfo->ptMaxPosition.y	 = rc.top;
	}
}

//---------------------------------------------------------------------------
//IntelliPoint �ւ̑Ή�
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::WmAppCommand(TMessage &msg)
{
	switch (GET_APPCOMMAND_LPARAM(msg.LParam)) {
	case APPCOMMAND_BROWSER_FORWARD:	//�i��
		GoForward->Execute();	break;
	case APPCOMMAND_BROWSER_BACKWARD:	//�߂�
		GoBack->Execute();		break;
	case APPCOMMAND_CLOSE:				//����
		FileClose->Execute();	break;
	}
}

//---------------------------------------------------------------------
// �q�E�B���h�E�̍쐬
//---------------------------------------------------------------------
TMDIChild *__fastcall TIdeaFragMainForm::CreateMDIChild(
	UnicodeString Name)		//�t�@�C���� [#ID [\t������]] or [#������]
{
	EV->TimerLockCnt++;

	TMDIChild *cp = NULL;

	if (SameText(Name, "����")) {
	//�V�K�쐬
		if (MDIChildCount<MAX_FRGWIN) {
			cp = new TMDIChild(Application);
			cp->Caption	= Name;
			cp->Tag		= MDIChildCount;
			if (MDIChildCount==1 && EV->ChildMax) cp->WindowState = wsMaximized;
		}
	}
	else {
		UnicodeString fnam = Name;
		UnicodeString anam;
		int p = pos_r(".IFD#", fnam.UpperCase());
		if (p>0) {
			anam = fnam.SubString(p + 5, fnam.Length() - (p + 4));
			fnam = fnam.SubString(1, p + 3);
		}
		UnicodeString xnam = ExtractFileExt(fnam);
		if (xnam.IsEmpty()) {
			if (StartsText("#", fnam)) {
				anam = fnam.SubString(2, fnam.Length() - 1);
				FragSet *fsp = CurFS;
				fnam = fsp? fsp->file_name : EmptyStr;
			}
		}

		//���łɊJ���Ă��邩?
		if (!fnam.IsEmpty()) {
			bool exist = false;
			for (int i=MDIChildCount-1; i>=0; i--) {
				cp = (TMDIChild*)MDIChildren[i];
				if (SameText(cp->FS->file_name, fnam)) {
					cp->Show();
					if (MDIChildCount==1 && EV->ChildMax)
						cp->WindowState = wsMaximized;
					else if (cp->WindowState==wsMinimized)
						cp->WindowState = wsNormal;
					//�A���J�[�ʒu��
					if (!anam.IsEmpty()) cp->FS->to_anchor(anam);
					exist = true;
					break;
				}
			}

			//�܂��Ȃ�V���ɊJ��
			if (!exist && MDIChildCount<MAX_FRGWIN) {
				bool flag = true;
				UnicodeString msg;
				if (!SameText(xnam, ".IFD")) {
					msgbox_WARN(msg.sprintf(_T("[%s]�͒f�Ѓt�@�C���ł͂���܂���\n"), fnam.c_str()));
					flag = false;
				}
				else if (!FileExists(fnam)) {
					flag = msgbox_Y_N(msg.sprintf(_T("[%s]�͑��݂��܂���\n�V���ɍ쐬���܂���?"), fnam.c_str()));
				}

				if (flag) {
					cp = new TMDIChild(Application);
					cp->Caption	= fnam;
					cp->Tag		= MDIChildCount;
					if (MDIChildCount==1 && EV->ChildMax) cp->WindowState = wsMaximized;

					//�f�Ѓt�@�C���ǂݍ���
					cp->FS->load_file(fnam, false);

					//�A���J�[�ʒu��
					if (!anam.IsEmpty()) {
						UpdateActiveChild();
						cp->FS->to_anchor(anam);
					}
					//�O��̈ʒu��
					else if (EV->RstScrPos) {
						cp->HorzScrollBar->Position = cp->FS->LastHPos;
						cp->VertScrollBar->Position = cp->FS->LastVPos;
					}
				}
			}
		}
	}

	EV->TimerLockCnt--;
	return cp;
}

//---------------------------------------------------------------------------
//�h���b�v�̎󂯓���
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::WmDropped(TMessage &msg)
{
	FragSet *fsp = CurFS;

	if ((DroppedMode==DROPEFFECT_COPY || DroppedMode==DROPEFFECT_LINK)) {
		TPoint mp = fsp? fsp->frg_owner->ScreenToClient(Mouse->CursorPos) : Point(-1, -1);

		for (int i=0; i<DroppedList->Count; i++) {
			UnicodeString fnam = DroppedList->Strings[i];
			//�����N����
			if (DroppedMode==DROPEFFECT_LINK) {
				if (fsp) {
					UnicodeString tit = split_tkn(fnam, "\t");
					Fragment *fp = fsp->new_frag();
					fp->style	 = fgsNormal;
					fp->TextStr  = tit;
					fp->LinkName = fnam;
					fp->wd		 = EV->def_wd;
					fp->hi		 = EV->def_hi;

					if (EV->NewDropPos) {
						fsp->GInfs->gstk_from_pos(mp.x, mp.y, &fp->group);
						fsp->set_new_pos(fp, i, mp.x, mp.y);
					}
					else {
						fp->group.stk[0] = EV->new_no_group? 0 : fsp->get_sel_group_ex();
						fsp->set_new_pos(fp, i);
					}

					fsp->GInfs->Items[fp->group.stk[0]].Visible = true;
					fp->Visible  = true;
					fsp->update_grp_empty();
				}
			}
			//�f�[�^�t�@�C��
			else if (test_ideafrag2(fnam)) {
				if (MDIChildCount<MAX_FRGWIN) {
					CreateMDIChild(fnam);
				}
				else {
					msgbox_WARN("����ȏ�J���܂���");
					break;
				}
			}
			//�摜�E���̑�
			else if (fsp) {
				bool mk_img = false;
				if (EV->DropImgFile) {
					//�Ή����Ă���摜�t�@�C�����`�F�b�N
					UnicodeString fext = ExtractFileExt(fnam);
					mk_img = SPI->TestFExt(fext) || test_FileExt(fext, ".wmf.emf" + WIC_FextStr);
				}

				//�摜�f�Ѝ쐬
				if (mk_img) {
					Fragment *fp = fsp->new_frag();
					fp->style	 = fgsImage;
					fp->TextStr  = "#�摜";
					fp->LinkName = fnam;
					fp->wd		 = EV->img_r1;
					fp->hi		 = EV->img_r2;
					fp->colBG	 = clWhite;

					if (EV->NewDropPos) {
						fsp->GInfs->gstk_from_pos(mp.x, mp.y, &fp->group);
						fsp->set_new_pos(fp, i, mp.x, mp.y);
					}
					else {
						fp->group.stk[0] = EV->new_no_group? 0 : fsp->get_sel_group_ex();
						fsp->set_new_pos(fp, i);
					}

					fsp->GInfs->Items[fp->group.stk[0]].Visible = true;
					fp->Visible  = true;
					fp->upd_link();
					fsp->update_grp_empty();
				}
				//�����N�f�Ѝ쐬
				else {
					if (fsp->get_linked_frag(fnam)) {
						if (!msgbox_Y_N("[" + fnam + "]�͂��łɃ����N����Ă��܂�\n����Ƀ����N�f�Ђ��쐬���܂���?")) continue;
					}

					FrgPropDlg->prpFS	  = fsp;
					FrgPropDlg->swNewFrag = true;
					FrgPropDlg->swNewLink = true;
					FrgPropDlg->newLink   = fnam;

					if (EV->NewDropPos) {
						FrgPropDlg->swNewCurPos = true;	//�h���b�v�ʒu�ɍ쐬
						FrgPropDlg->newPosX 	= mp.x + 1;
						FrgPropDlg->newPosY 	= mp.y + 1;
						FrgPropDlg->swNewGroup	= true;
						FrgPropDlg->newGrp		= fsp->GInfs->gstk_from_pos(mp.x, mp.y, &FrgPropDlg->newGstk);
					}

					FrgPropDlg->ShowModal();
					CloseIME();
				}
			}
		}
	}

	DroppedList->Clear();
	UpdateActiveChild();
}

//---------------------------------------------------------------------
// �A�N�e�B�u�E�B���h�E�̒f�ЃZ�b�g���擾(�v���p�e�B)
//---------------------------------------------------------------------
FragSet * __fastcall TIdeaFragMainForm::GetCurFS()
{
	TMDIChild *cp = (TMDIChild*)ActiveMDIChild;
	return (cp? cp->FS : NULL);
}

//---------------------------------------------------------------------
// �A�N�e�B�u�E�B���h�E�̉�ʂ��X�V
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::UpdateActiveChild()
{
	if (!Timer1->Enabled) return;

	FragSet *fsp = CurFS;
	if (fsp) {
		fsp->all_update();
		fsp->GInfs->repaint_frame();
	}
	UpdateGrpLst();
	FragGrid->GridUpdate(true);

	if (EV->FitMapView && MapViewer->Visible) MapViewer->AdjustWinHeight();
}

//---------------------------------------------------------------------------
//�X�e�[�^�X�o�[�̕\�����X�V
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::DispStatusUpdate(TObject *Sender)
{
	UnicodeString sttstr;
	FragSet *fsp = CurFS;
	if (fsp) {
		int n = 0;
		for (int i=MDIChildCount-1; i>=0; i--) if (((TMDIChild*)MDIChildren[i])->FS->modify) n++;
		StatusBar->Panels->Items[0]->Text = sttstr.sprintf(_T("%u/%u "), n, MDIChildCount);
		sttstr.sprintf(_T("�f�А�=%u  /�I��=%u"), fsp->FrgList->Count, fsp->SelList->Count);
	}
	else {
		StatusBar->Panels->Items[0]->Text = UnicodeString().sprintf(_T("0/%u"), MDIChildCount);
	}
	StatusBar->Panels->Items[1]->Text = sttstr;
}

//---------------------------------------------------------------------------
//�A�N�V�������X�g�X�V
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ChildUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (MDIChildCount>0);
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditMultiSelUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (!fsp->read_only && fsp->SelList->Count>1) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditChildUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? !fsp->read_only : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditSelUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (!fsp->read_only && fsp->SelList->Count>0) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditSelOneUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Visible = fsp? (!fsp->read_only && fsp->SelList->Count==1) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::CanOpenUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (MDIChildCount<MAX_FRGWIN);
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::SelUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (fsp->SelList->Count>0) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditPasteUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (!fsp->read_only && FSbuf->FrgList->Count>0) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditPasteExUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (!fsp->read_only && FSbuf->FrgList->Count>0) : false;
	((TAction*)Sender)->Visible = (Screen->ActiveForm != FragGrid);
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::UndoPosUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (fsp->UndoN>0) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::RedoPosUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (fsp->RedoN>0) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FilePrintUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (MDIChildCount>0 && EV->printer_ok);
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::NewClipUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (!fsp->read_only && Clipboard()->HasFormat(CF_TEXT)) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::PrevSelUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (fsp->notice_pos>0 && fsp->SelList->Count>1) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::NextSelUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (fsp->notice_pos<fsp->SelList->Count-1 && fsp->SelList->Count>1) : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GoBackUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (EV->HistoryPtr>0);
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GoForwardUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (EV->HistoryPtr+1 < EV->JumpHistoryList->Count);
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GoHomeUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (!EV->HomeWindow.IsEmpty());
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditGrpStretchUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? !fsp->read_only : false;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowGrpLstUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (GroupListBox->Floating ||
					EV->LastDockSite!=FragGrid->DockPanel ||
				   (EV->LastDockSite==FragGrid->DockPanel && FragGrid->Visible));
}

//---------------------------------------------------------------------
//���j���[���ڂ̍X�V
//---------------------------------------------------------------------
//�t�@�C�����j���[
void __fastcall TIdeaFragMainForm::File1Click(TObject *Sender)
{
	//�t�@�C���������j���[�ݒ�
	TStringList *lp = EV->FileHistoryList;
	UnicodeString tmpstr;
	for (int i=0; i<FileHistoryItem->Count; i++) {
		TMenuItem *mp = FileHistoryItem->Items[i];
		mp->Visible = false;
		if (i<lp->Count) {
			if (!lp->Strings[i].IsEmpty()) {
				if (i<10) tmpstr.sprintf(_T("&%u: "), (i + 1)%10); else tmpstr = "   ";
				mp->Caption = tmpstr + lp->Strings[i];
				mp->Visible = true;
			}
		}
	}

	FileHistoryItem->Enabled = (MDIChildCount<MAX_FRGWIN && lp->Count>0);
}
//---------------------------------------------------------------------
//�ҏW���j���[
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::Edit1Click(TObject *Sender)
{
	FragSet *fsp = CurFS;
	if (fsp)
		EditMoveToWinItem->Enabled = (!fsp->read_only && fsp->SelList->Count>0 && MDIChildCount>1);
	else
		EditMoveToWinItem->Enabled = false;
	PopMoveToWinItem->Enabled = EditMoveToWinItem->Enabled;

	PopupPos = Point(-1, -1);
}
//---------------------------------------------------------------------
//�W�����v���j���[
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::Go1Click(TObject *Sender)
{
	//�t�@�C���������j���[�ݒ�
	int topix = (GoListItem->Count>MAX_JUMPLIST)? GoListItem->Count - MAX_JUMPLIST : 0;
	TStringList *lp = EV->JumpHistoryList;
	UnicodeString tmpstr;
	for (int i=0; i<GoListItem->Count; i++) {
		TMenuItem *mp = GoListItem->Items[i];
		mp->Visible = false;
		if (i<lp->Count) {
			if (i<10) tmpstr.sprintf(_T("&%u: "), (i + 1)%10); else tmpstr = "   ";
			mp->Caption = tmpstr + ReplaceStr(lp->Strings[topix + i], "\t", "  ");
			mp->Tag 	= topix + i;
			mp->Visible = true;
			mp->Checked = (mp->Tag == EV->HistoryPtr);
		}
	}

	GoListItem->Enabled = (lp->Count>0);
}
//---------------------------------------------------------------------
//�c�[�����j���[
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::Tool1Click(TObject *Sender)
{
	ToolNrmItem->Checked   = EditNormalBtn->Down;
	ToolGLineItem->Checked = EditGLineBtn->Down;
	ToolBrushItem->Checked = EditBrushBtn->Down;
}
//---------------------------------------------------------------------
//�E�B���h�E���j���[
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::Window1Click(TObject *Sender)
{
	W_s1->Visible = (MDIChildCount>0);
	UnicodeString tmpstr;
	for (int i=6; i<Window1->Count; i++) {
		int ix = i - 6;	//�E�C���h�E�I�����ڂ̃g�b�v��
		TMenuItem *mp = Window1->Items[i];
		if (ix<MDIChildCount) {
			if (ix<10) tmpstr.sprintf(_T("&%u: "), (ix + 1)%10); else if (ix<20) tmpstr = "   ";
			mp->Caption = tmpstr + MDIChildren[ix]->Caption;
			mp->Visible = true;
			mp->Checked = (MDIChildren[ix]==ActiveMDIChild);
		}
		else {
			mp->Visible = false;
		}
	}
}

//---------------------------------------------------------------------------
//�A�N�V�������s�O�Ƀ|�b�v�A�b�v�}�[�N������
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ActionList1Execute(TBasicAction *Action, bool &Handled)
{
	TMDIChild *cp = (TMDIChild*)ActiveMDIChild;
	if (cp) cp->PopPosShape->Visible = false;
}

//---------------------------------------------------------------------
// �A�N�e�B�u�E�B���h�E���ς�������̏���
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::UpdateMenuGrpLst(TObject *Sender)
{
	if (EV->TimerLockCnt>0) return;
	if (!Timer1->Enabled) return;

	UpdateActiveChild();
}

//---------------------------------------------------------------------
// �^�C�}�[����
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::Timer1Timer(TObject *Sender)
{
	if (EV->TimerLockCnt>0) return;
	if (!Application->Active) return;

	//�O���[�v���X�g��[�~]�{�^�����ŕ���ꂽ�ꍇ�̏���
	if (!GroupListBox->Visible) {
		if (Splitter1->Visible) {
			DockPanel1->Constraints->MinWidth  = 0;		DockPanel1->Width  = 0;
			DockPanel2->Constraints->MinHeight = 0;		DockPanel2->Height = 0;
			DockPanel3->Constraints->MinWidth  = 0;		DockPanel3->Width  = 0;
			DockPanel4->Constraints->MinHeight = 0;		DockPanel4->Height = 0;
			Splitter1->Visible = false;
		}
		if (FragGrid->DockPanel->Width>1) {
			FragGrid->DockPanel->Constraints->MinWidth = 0;
			FragGrid->DockPanel->Width = 1;
		}
	}

	//�v��������Ή�ʍX�V
	if (EV->reqUpdate) {
		UpdateActiveChild();
		EV->reqUpdate = EV->reqFrgLst = false;
	}
	else if (EV->reqFrgLst) {
		FragGrid->GridUpdate();
	}
}

//---------------------------------------------------------------------
//�q���g�\��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowHint(TObject *Sender)
{
	FragSet *fsp = CurFS; if (!fsp) return;
	if (fsp->CLineMode>0) return;
	StatusBar->Panels->Items[3]->Text = Application->Hint;
}

//---------------------------------------------------------------------
//IME�����
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::CloseIME()
{
	HIMC hIMC = ImmGetContext(Handle);
	if (hIMC) {
		if (::ImmGetOpenStatus(hIMC)) ::ImmSetOpenStatus(hIMC, FALSE);
		::ImmReleaseContext(Handle, hIMC);
	}
}


//===========================================================================
//�y�t�@�C�����j���[�z
//===========================================================================
//�V�K
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FileNewExecute(TObject *Sender)
{
	CreateMDIChild("����");
}
//---------------------------------------------------------------------
//�J��
void __fastcall TIdeaFragMainForm::FileOpenExecute(TObject *Sender)
{
	OpenDialog->Title	   = "�J��";
	OpenDialog->Filter	   = "�f�[�^�t�@�C�� (*.IFD)|*.IFD";
	OpenDialog->DefaultExt = "IFD";
	OpenDialog->FileName   = "*.IFD";
	if (OpenDialog->Execute()) CreateMDIChild(OpenDialog->FileName);
}
//---------------------------------------------------------------------
//�ŋߊJ�����t�@�C��
void __fastcall TIdeaFragMainForm::FileHstItemClick(TObject *Sender)
{
	TStringList *lp = EV->FileHistoryList;
	int n = ((TComponent*)Sender)->Tag;
	if (n < lp->Count) CreateMDIChild(lp->Strings[n]);
}
//---------------------------------------------------------------------
//�㏑���ۑ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FileSaveExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	if (!fsp->file_name.IsEmpty()) {
		fsp->save_file(fsp->file_name);
		StatusBar->Panels->Items[3]->Text = fsp->file_name + " ��ۑ����܂���";
	}
	else {
		FileSaveAs->Execute();	//���O��t���ĕۑ�
	}
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FileSaveUpdate(TObject *Sender)
{
	FragSet *fsp = CurFS;
	((TAction*)Sender)->Enabled = fsp? (!fsp->read_only && fsp->modify) : false;
}
//---------------------------------------------------------------------
//���O��t���ĕۑ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FileSaveAsExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	SaveDialog->Title	   = "���O��t���ĕۑ�";
	SaveDialog->Filter	   = "�f�[�^�t�@�C��(*.IFD)|*.IFD";
	SaveDialog->DefaultExt = "IFD";
	SaveDialog->FileName   = fsp->file_name;
	if (SaveDialog->Execute()) {
		if (fsp->save_file(SaveDialog->FileName))
			fsp->frg_owner->Caption = fsp->file_name;
		else
			msgbox_ERR("�ۑ��ł��܂���ł���");
	}
}
//---------------------------------------------------------------------
//����
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FileCloseExecute(TObject *Sender)
{
	if (ActiveMDIChild) ActiveMDIChild->Close();
}
//---------------------------------------------------------------------
//�S�ĕۑ����ďI��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FileAllSaveEndExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;
	EV->LastFile = fsp? fsp->file_name : EmptyStr;

	//�q�E�B���h�E��S�ċ����ۑ����ĕ���
	for (int i=MDIChildCount-1; i>=0; i--) {
		TMDIChild *cp	  = (TMDIChild*)MDIChildren[i];
		cp->FS->forcesave = true;
		cp->Close();
	}

	//�I��
	Close();
}

//---------------------------------------------------------------------
//�}�[�W
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FileMergeExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	OpenDialog->Title	   = "�}�[�W";
	OpenDialog->Filter	   = "�f�[�^�t�@�C��(*.IFD)|*.IFD";
	OpenDialog->DefaultExt = "IFD";
	OpenDialog->FileName   = "*.IFD";
	if (OpenDialog->Execute()) {
		if (test_ideafrag2(OpenDialog->FileName))
			fsp->load_file(OpenDialog->FileName, true);
		else
			msgbox_WARN("[" + OpenDialog->FileName + "]�̓f�[�^�t�@�C���ł͂���܂���");
	}
}
//---------------------------------------------------------------------
//�e�L�X�g��荞��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::TextImportExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	OpenDialog->Title	   = "�e�L�X�g��荞��";
	OpenDialog->Filter	   = "�e�L�X�g (*.txt)|*.TXT";
	OpenDialog->DefaultExt = "TXT";
	OpenDialog->FileName   = "*.TXT";
	if (OpenDialog->Execute()) {
		Screen->Cursor = crHourGlass;
		fsp->import_text(OpenDialog->FileName);
		Screen->Cursor = crDefault;
	}
}
//---------------------------------------------------------------------
//�e�L�X�g�����o��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::TextExportExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	SaveDialog->Title		= "�e�L�X�g�����o��";
	SaveDialog->Filter		= "�e�L�X�g (*.txt)|*.TXT";
	SaveDialog->DefaultExt	= "TXT";
	SaveDialog->FileName	= ChangeFileExt(fsp->file_name, ".TXT");
	if (SaveDialog->Execute()) {
		Screen->Cursor = crHourGlass;
		fsp->export_text(SaveDialog->FileName);
		Screen->Cursor = crDefault;
	}
}
//---------------------------------------------------------------------------
//CSV�G�N�X�|�[�g
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::CsvExportExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	SaveDialog->Title		= "CSV�t�@�C���o��";
	SaveDialog->Filter		= "CSV�t�@�C�� (*.csv)|*.CSV";
	SaveDialog->DefaultExt	= "CSV";
	SaveDialog->FileName	= ChangeFileExt(fsp->file_name, ".CSV");
	if (SaveDialog->Execute()) {
		Screen->Cursor = crHourGlass;
		fsp->export_file(SaveDialog->FileName, ",");
		Screen->Cursor = crDefault;
	}
}
//---------------------------------------------------------------------
//TSV�G�N�X�|�[�g
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::TsvExportExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	SaveDialog->Title		= "TAB��؂�t�@�C���o��";
	SaveDialog->Filter		= "TAB��؂�t�@�C�� (*.txt)|*.TXT";
	SaveDialog->DefaultExt	= "TXT";
	SaveDialog->FileName	= ChangeFileExt(fsp->file_name, ".TXT");
	if (SaveDialog->Execute()) {
		Screen->Cursor = crHourGlass;
		fsp->export_file(SaveDialog->FileName, "\t");
		Screen->Cursor = crDefault;
	}
}

//---------------------------------------------------------------------
//���
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FliePrintExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	if (EV->printer_ok) {
		//������������ɐݒ�
		StatusBar->Panels->Items[3]->Text = "�v�����^�ݒ蒆..."; StatusBar->Repaint();
		Printer()->Orientation = poLandscape;
		StatusBar->Panels->Items[3]->Text = EmptyStr; StatusBar->Repaint();
		if (PrintDialog1->Execute()) fsp->printout();
	}
}
//---------------------------------------------------------------------------
//�摜�Ƃ��ĕۑ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FileImgOutExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	SaveDialog->Title = "�摜�Ƃ��ĕۑ�";
	UnicodeString flt = GraphicFilter(__classid(Graphics::TBitmap));
				  flt.UCAT_TSTR("|JPEG�t�@�C��(*.JPG)|*.JPG");
				  flt.UCAT_TSTR("|PNG�t�@�C��(*.PNG)|*.PNG");
				  flt.UCAT_TSTR("|GIF�t�@�C��(*.GIF)|*.GIF");
				  flt.UCAT_TSTR("|TIF�t�@�C��(*.TIF)|*.TIF");
	SaveDialog->Filter		= flt;
	SaveDialog->FilterIndex = EV->ImgFilterIdx;
	switch (SaveDialog->FilterIndex) {
	case 2:  SaveDialog->DefaultExt	= "JPG"; break;
	case 3:  SaveDialog->DefaultExt	= "PNG"; break;
	case 4:  SaveDialog->DefaultExt	= "GIF"; break;
	case 5:  SaveDialog->DefaultExt	= "TIF"; break;
	default: SaveDialog->DefaultExt	= "BMP";
	}
	SaveDialog->FileName = ChangeFileExt(fsp->file_name, "." + SaveDialog->DefaultExt);
	if (SaveDialog->Execute()) {
		Application->ProcessMessages();
		Screen->Cursor = crHourGlass;
		StatusBar->Panels->Items[3]->Text = "�摜�ۑ���..."; StatusBar->Repaint();
		if (!fsp->imageout(SaveDialog->FileName)) msgbox_ERR("�ۑ��Ɏ��s���܂���");
		Screen->Cursor = crDefault;
		StatusBar->Panels->Items[3]->Text = EmptyStr;
		UnicodeString fext = ExtractFileExt(SaveDialog->FileName);
		if		(SameText(fext, ".JPG")) EV->ImgFilterIdx = 2;
		else if (SameText(fext, ".PNG")) EV->ImgFilterIdx = 3;
		else if (SameText(fext, ".GIF")) EV->ImgFilterIdx = 4;
		else if (SameText(fext, ".TIF")) EV->ImgFilterIdx = 5;
		else							 EV->ImgFilterIdx = 1;
	}
}

//===========================================================================
//�y�ҏW���j���[�z
//===========================================================================
//�؂���
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditCutExecute(TObject *Sender)
{
	if (!CurFS) return;

	UnicodeString tmpstr;
	CurFS->get_sel_str(&tmpstr);
	if (!tmpstr.IsEmpty()) Clipboard()->AsText = tmpstr;
	CurFS->copy_buf(true);
}
//---------------------------------------------------------------------
//�R�s�[
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditCopyExecute(TObject *Sender)
{
	if (!CurFS) return;

	UnicodeString tmpstr;
	CurFS->get_sel_str(&tmpstr);
	if (!tmpstr.IsEmpty()) Clipboard()->AsText = tmpstr;
	CurFS->copy_buf();
}
//---------------------------------------------------------------------
//�\��t��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditPasteExecute(TObject *Sender)
{
	if (CurFS) CurFS->paste_buf();
}
//---------------------------------------------------------------------
//�}�E�X�ʒu�ɓ\��t��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditPasteExExecute(TObject *Sender)
{
	TMDIChild *cp = (TMDIChild*)ActiveMDIChild;
	if (cp) {
		TPoint p = PopupPos;
		if (p.x==-1 || p.y==-1) p = cp->ScreenToClient(Mouse->CursorPos);
		cp->FS->paste_buf(p.x, p.y);
		PopupPos = Point(-1, -1);
	}
}
//---------------------------------------------------------------------
//�V�K�f��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditNewExecute(TObject *Sender)
{
	if (CurFS) {
		FrgPropDlg->prpFS	  = CurFS;
		FrgPropDlg->swNewFrag = true;
		FrgPropDlg->ShowModal();
		CloseIME();
	}
}
//---------------------------------------------------------------------
//�N���b�v�{�[�h����V�K�f��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditNewClipExecute(TObject *Sender)
{
	if (CurFS) CurFS->new_from_clipboard();
}
//---------------------------------------------------------------------
//�폜
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditDeleteExecute(TObject *Sender)
{
	if (CurFS) CurFS->del_sel_frag();
}
//---------------------------------------------------------------------
//�S�đI��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditSelAllExecute(TObject *Sender)
{
	if (CurFS) {
		CurFS->all_select();
		FragGrid->GridUpdate(true);
	}
}
//---------------------------------------------------------------------
//�O���[�v�I���T�u���j���[�ݒ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditSelGroupItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;
	UnicodeString tmpstr, namstr;
	int n = 0;
	for (int i=0; i<MAX_GROUP && n<MAX_GRP_ITM; i++) {
		int gn = i + 1;
		if (!fsp) continue;
		fsp->GInfs->get_name_plate(gn, &namstr);
		if (n<10) tmpstr.sprintf(_T("&%u: "), (n + 1)%10); else tmpstr = "   ";
		tmpstr += namstr;
		if (fsp->GInfs->Items[gn].Empty || !fsp->GInfs->Items[gn].Visible) continue;
		TMenuItem *mp1 = EditSelGroupItem->Items[n];
		TMenuItem *mp2 = PopSelGroupItem->Items[n];
		mp1->Tag	 = gn;
		mp2->Tag	 = gn;
		mp1->Hint	 = namstr + "�̒f�Ђ�I�����܂�";
		mp2->Hint	 = mp1->Hint;
		mp1->Caption = tmpstr;
		mp2->Caption = tmpstr;
		mp1->Break	 = (n==20)? mbBarBreak : mbNone;
		mp2->Break	 = mp1->Break;
		mp1->Visible = true;
		mp2->Visible = true;
		n++;
	}

	for (int i=n; i<MAX_GRP_ITM; i++) {
		EditSelGroupItem->Items[i]->Visible = false;
		PopSelGroupItem->Items[i]->Visible	= false;
	}
}
//---------------------------------------------------------------------
//�O���[�v�I��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditSelGrpItemClick(TObject *Sender)
{
	int gn = ((TComponent*)Sender)->Tag;
	if (CurFS) {
		CurFS->group_select(gn, true);
		FragGrid->SetGroupTop(gn);
	}
}

//---------------------------------------------------------------------
//�w�i�F�I���|�b�v�A�b�v���j���[�ݒ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditSelBgColExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;

	TMenuItem *pmp = BgColPopupMenu->Items;
	for (int i=0; i<pmp->Count; i++) pmp->Items[i]->Tag = (int)clNone;

	for (int i=0; i<fsp->FrgList->Count; i++) {
		int c = (int)(fsp->FrgList->Items[i]->colBG);
		bool found = false;
		for (int j=0; j<pmp->Count; j++) {
			if (pmp->Items[j]->Tag == c) {
				found = true;  break;
			}
		}
		if (found) continue;
		for (int j=0; j<pmp->Count; j++) {
			TMenuItem *mp = pmp->Items[j];
			if (mp->Tag == (int)clNone) {
				mp->Tag = c;  break;
			}
		}
	}

	for (int i=0; i<pmp->Count; i++) {
		TMenuItem *mp = pmp->Items[i];
		mp->Visible   = (mp->Tag != clNone);
	}

	BgColPopupMenu->Tag = ((TComponent*)Sender)->Tag;
	BgColPopupMenu->Popup(Mouse->CursorPos.x, Mouse->CursorPos.y);
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::SelBgColItemMeasureItem(
	TObject *Sender, TCanvas *ACanvas, int &Width, int &Height)
{
	Width = Height * 5;
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::SelBgColItemDrawItem(
	TObject *Sender, TCanvas *ACanvas, const TRect &ARect, bool Selected)
{
	TMenuItem * mp = (TMenuItem*)Sender;
	TColor col = (TColor)mp->Tag;
	ACanvas->Brush->Color = Selected? clHighlight : clBtnFace;
	ACanvas->FillRect(ARect);

	TRect rc = ARect;
	rc.left += 6;
	::DrawText(ACanvas->Handle, mp->Caption.c_str(), mp->Caption.Length(), &rc, DT_LEFT);

	rc = ARect;
	InflateRect(&rc,-2,-2);
	rc.left += 30;
	ACanvas->Brush->Color = col;
	ACanvas->FillRect(rc);
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::SelBgColItemClick(TObject *Sender)
{
	TColor col = (TColor)(((TComponent*)Sender)->Tag);
	if (CurFS) {
		if (BgColPopupMenu->Tag==0) {
			//�w�i�F�őI��
			CurFS->bgcol_select(col);
		}
		else {
			//�����w�i�F����h��F��ݒ�
		    EV->col_Brush	  = col;
			BrushPanel->Color = EV->col_Brush;
		}
	}
}

//---------------------------------------------------------------------
//�W��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditGatherExecute(TObject *Sender)
{
	if (CurFS) CurFS->gather_sel(false, EV->ArrangePt);
}

//---------------------------------------------------------------------
//�W�J
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditSpreadExecute(TObject *Sender)
{
	if (CurFS) CurFS->spread_sel();
}
//---------------------------------------------------------------------
//�L�k
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditStretchExecute(TObject *Sender)
{
	if (CurFS) {
		StretchDlg->FS = CurFS;
		StretchDlg->PosMode = 0;
		StretchDlg->ShowModal();
	}
}
//---------------------------------------------------------------------
//�O���[�v�L�k
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditGrpStretchExecute(TObject *Sender)
{
	if (CurFS) {
		StretchDlg->FS = CurFS;
		StretchDlg->PosMode = 1;
		StretchDlg->ShowModal();
	}
}
//---------------------------------------------------------------------
//�Еt��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditAwayExecute(TObject *Sender)
{
	if (CurFS) CurFS->gather_sel(true);
}
//---------------------------------------------------------------------------
//�}�E�X�ʒu�Ɉړ�
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditMovePosExecute(TObject *Sender)
{
	TMDIChild *cp = (TMDIChild*)ActiveMDIChild;
	if (cp) {
		TPoint p = PopupPos;
		if (p.x==-1 || p.y==-1) p = cp->ScreenToClient(Mouse->CursorPos);
		cp->FS->move_sel(p.x, p.y);
		PopupPos = Point(-1, -1);
	}
}

//---------------------------------------------------------------------
//���̈ʒu�ɖ߂�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditUndoExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	for (int i=0; i<fsp->FrgList->Count; i++) fsp->FrgList->Items[i]->era_frag();
	fsp->undo();
	UpdateGrpLst();
	EV->reqUpdate = true;
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditRedoExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	for (int i=0; i<fsp->FrgList->Count; i++) fsp->FrgList->Items[i]->era_frag();
	fsp->redo();
	UpdateGrpLst();
	EV->reqUpdate = true;
}
//---------------------------------------------------------------------
//���O���[�v�ֈړ��T�u���j���[�ݒ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditMoveToGroupItemClick(TObject *Sender)
{
	UnicodeString tmpstr, namstr;
	bool flag;
	FragSet *fsp = CurFS;
	for (int i=0; i<EditMoveToGroupItem->Count; i++) {
		TMenuItem *mp1 = EditMoveToGroupItem->Items[i];
		TMenuItem *mp2 = PopMoveToGroupItem->Items[i];
		if (i<10) tmpstr.sprintf(_T("&%u: "), (i + 1)%10); else tmpstr = "   ";
		if (fsp) {
			fsp->GInfs->get_name_plate(i + 1, &namstr);
			tmpstr += namstr;
			fsp->mak_sel_list();
			flag = (!fsp->GInfs->Items[i + 1].Selected && fsp->SelList->Count>0);
		}
		else {
			tmpstr.sprintf(_T("�O���[�v %2u"), i + 1);
			flag = false;
		}
		if (flag)
			mp1->Hint = namstr + "�֑I�𒆂̒f�Ђ��ړ����܂�";
		else
			mp1->Hint = "�I�𒆂̃O���[�v�ւ͈ړ��ł��܂���";

		mp2->Hint	 = mp1->Hint;
		mp1->Caption = tmpstr;
		mp2->Caption = tmpstr;
		mp1->Enabled = flag;
		mp2->Enabled = flag;
		mp1->Break	 = (i==20)? mbBarBreak : mbNone;
		mp2->Break	 = (i==20)? mbBarBreak : mbNone;
	}
}
//---------------------------------------------------------------------
//���O���[�v�ֈړ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::MoveToGroupItemClick(TObject *Sender)
{
	if (CurFS) CurFS->mov_sel_group(((TComponent*)Sender)->Tag);
}

//---------------------------------------------------------------------
//���E�B���h�E�ֈړ��T�u���j���[�ݒ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditMoveToWinItemClick(TObject *Sender)
{
	UnicodeString tmpstr;
	int n = 0;
	for (int i=0; i<EditMoveToWinItem->Count; i++) {
		TMenuItem *mp1 = EditMoveToWinItem->Items[i];
		TMenuItem *mp2 = PopMoveToWinItem->Items[i];
		if (i<MDIChildCount && MDIChildren[i]!=ActiveMDIChild) {
			TMDIChild * cp = (TMDIChild*)MDIChildren[i];
			if (n<10) tmpstr.sprintf(_T("&%u: "), (n + 1)%10); else tmpstr = "   ";
			tmpstr += MDIChildren[i]->Caption;
			mp1->Caption = tmpstr;
			mp2->Caption = tmpstr;
			mp1->Visible = true;
			mp2->Visible = true;
			mp1->Enabled = !cp->FS->read_only;
			mp2->Enabled = !cp->FS->read_only;
			n++;
		}
		else {
			mp1->Visible = false;
			mp2->Visible = false;
		}
	}
}
//---------------------------------------------------------------------
//���E�B���h�E�ֈړ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::MovToWinItemClick(TObject *Sender)
{
	int n = ((TComponent*)Sender)->Tag;
	FragSet *dstFS = (n>=0 && n<MDIChildCount)? ((TMDIChild*)MDIChildren[n])->FS : NULL;

	if (CurFS && dstFS) {
		CurFS->copy_buf(true);
		dstFS->paste_buf(false);
		if (EV->ActivDistWin) {
			SelWinItemClick(Sender);	//�ړ�����A�N�e�B�u��
			dstFS->to_center(dstFS->get_sel_center(), false);
		}
	}
}

//---------------------------------------------------------------------
//�v���p�e�B
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditPropertyExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	FrgPropDlg->prpFS	  = fsp;
	FrgPropDlg->swNewFrag = false;
	FrgPropDlg->ShowModal();
	CloseIME();
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditPropertyUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	FragSet *fsp = CurFS;
	if (fsp) {
		if (fsp->read_only) {
			ap->Enabled = false;
		}
		else {
			if (fsp->SelList->Count==1)
				ap->Enabled = (fsp->SelList->Items[0]->style!=fgsJunction);
			else
				ap->Enabled = (fsp->SelList->Count>0);
		}
	}
	else {
		ap->Enabled = false;
	}
}
//---------------------------------------------------------------------
//�t�@�C����#ID���R�s�[
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::CopyAnchorActionExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	Fragment *fp = fsp->get_sel_single();
	if (fp) Clipboard()->AsText = fsp->file_name + "#" + IntToStr(fp->ID);
}

//---------------------------------------------------------------------
//����
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditFindExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	if (fsp->SelList->Count==1) fsp->all_select(false);
	if (FindSetDlg->ShowModal()==mrOk) {
		Screen->Cursor = crHourGlass;
		UnicodeString kwd = trim_ex(FindSetDlg->FindComboBox->Text);
		EV->FindWord = kwd;
		int n = fsp->find_str(kwd, fsp->SelList->Count>0);
		Screen->Cursor = crDefault;
		UnicodeString msg;
		if (n>0) {
			msg.sprintf(_T("%u���̒f�Ђ�������܂��� [%s]"), n, kwd.c_str());
			if (FragGrid->Visible && EV->FindFupSw) {
				FragGrid->SortModeEx = 1;
				FragGrid->GridUpdate(true);
			}
		}
		else {
			msg.sprintf(_T("[%s]��������܂���"), kwd.c_str());
		}
		StatusBar->Panels->Items[3]->Text = msg;
	}

	CloseIME();
}
//---------------------------------------------------------------------
//�I�𒆓��̑O�̒f�Ђ𒆉���
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditPrevSelExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	if (fsp->SelList->Count>0 && fsp->notice_pos>0) {
		Fragment *fp = fsp->SelList->Items[--fsp->notice_pos];
		if (fp) fsp->to_center(fp, true);
		FragGrid->FrgLstGrid->Repaint();
	}
}
//---------------------------------------------------------------------
//�I�𒆓��̎��̒f�Ђ𒆉���
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditNextSelExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	int scnt = fsp->SelList->Count;
	if (scnt>0 && fsp->notice_pos<scnt-1) {
		Fragment *fp = fsp->SelList->Items[++fsp->notice_pos];
		if (fp) fsp->to_center(fp, true);
		FragGrid->FrgLstGrid->Repaint();
	}
}

//===========================================================================
//�y�W�����v���j���[�z
//===========================================================================
//�W�����v
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GoJumpExecute(TObject *Sender)
{
	if (CurFS) CurFS->jump_sel_link();
}
//---------------------------------------------------------------------
//�߂�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GoBackExecute(TObject *Sender)
{
	if (EV->HistoryPtr>0) {
		TStringList *lp = EV->JumpHistoryList;
		if (CurFS) {
			UnicodeString hnam = CurFS->file_name;
			if (!CurFS->LastAnc.IsEmpty()) hnam += ("#" + CurFS->LastAnc);
			if (EV->HistoryPtr==lp->Count)
				lp->Add(hnam);
			else
				lp->Strings[EV->HistoryPtr] = hnam;
		}
		CreateMDIChild(lp->Strings[--(EV->HistoryPtr)]);
	}
}
//---------------------------------------------------------------------
//�i��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GoForwardExecute(TObject *Sender)
{
	TStringList *lp = EV->JumpHistoryList;
	if ((EV->HistoryPtr + 1) < lp->Count)
		CreateMDIChild(lp->Strings[++(EV->HistoryPtr)]);
}
//---------------------------------------------------------------------
//�z�[��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GoHomeExecute(TObject *Sender)
{
	if (CurFS) {
		TStringList *lp = EV->JumpHistoryList;
		if (EV->HomeWindow != CurFS->file_name) {
			UnicodeString hnam = CurFS->file_name;
			if (!CurFS->LastAnc.IsEmpty()) hnam += ("#" + CurFS->LastAnc);
			if (EV->HistoryPtr==lp->Count)
				lp->Add(hnam);
			else
				lp->Strings[EV->HistoryPtr] = hnam;
			EV->HistoryPtr++;
		}
	}
	TMDIChild *cp = CreateMDIChild(EV->HomeWindow);
	if (cp && EV->HomWinMax) cp->WindowState = wsMaximized;
}
//---------------------------------------------------------------------
//�W�����v���X�g
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::JumpListItemClick(TObject *Sender)
{
	int n = ((TComponent*)Sender)->Tag;
	TStringList *lp = EV->JumpHistoryList;
	if (n < lp->Count) {
		EV->HistoryPtr = n;
		CreateMDIChild(lp->Strings[n]);
	}
}

//---------------------------------------------------------------------
//�O���[�v���X�g�𕂓���
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::FloatGrpLst()
{
	if (!GroupListBox->Floating) {
		DockPanel1->Constraints->MinWidth  = 0;		DockPanel1->Width  = 0;
		DockPanel2->Constraints->MinHeight = 0;		DockPanel2->Height = 0;
		DockPanel3->Constraints->MinWidth  = 0;		DockPanel3->Width  = 0;
		DockPanel4->Constraints->MinHeight = 0;		DockPanel4->Height = 0;
		DockPanel1->Visible = true;
		DockPanel2->Visible = true;
		DockPanel3->Visible = true;
		DockPanel4->Visible = true;
		Splitter1->Visible	= false;

		UnicodeString sct = "GroupList";
		TRect rc = Rect(IniFile->ReadInteger(sct, "FLeft",		0),
				 		IniFile->ReadInteger(sct, "FTop",		0),
				 		IniFile->ReadInteger(sct, "FRight",		200),
				 		IniFile->ReadInteger(sct, "FBottom",	200));
		GroupListBox->ManualFloat(rc);
		TCustomDockForm *dp = (TCustomDockForm*)GroupListBox->HostDockSite;
		if (dp) {
			dp->Caption = "�O���[�v���X�g";
			dp->BoundsRect = rc;
		}
	}
}
//---------------------------------------------------------------------
//�O���[�v���X�g���w��T�C�g�փh�b�L���O
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::SetGrpLstPos(TPanel *pSite)
{
	GroupListBox->Visible = false;
	SaveGrpLstPos();

	switch (pSite->Align) {
	case alLeft: case alRight:
		DockPanel2->Visible = false;
		DockPanel4->Visible = false;
		DockPanel1->Visible = true;
		DockPanel3->Visible = true;
		pSite->Constraints->MinWidth = 20;
		pSite->Width = IniFile->ReadInteger("GroupList", "Width", 120);
		if (pSite->Width>(ClientWidth - 120)) pSite->Width = ClientWidth - 120;
		break;
	case alTop: case alBottom:
		DockPanel1->Visible = false;
		DockPanel3->Visible = false;
		DockPanel2->Visible = true;
		DockPanel4->Visible = true;
		pSite->Constraints->MinHeight = 20;
		pSite->Height = IniFile->ReadInteger("GroupList", "Height", 120);
		if (pSite->Height>(ClientHeight - 120)) pSite->Height = ClientHeight - 120;
		if (pSite->Align==alBottom) {
			//�X�e�[�^�X�o�[�Ƃ̓���ւ��h�~
			pSite->Align = alTop;
			pSite->Align = alBottom;
		}
		break;
	default:
		;
	}

	if (pSite->DockClientCount==0) GroupListBox->ManualDock(pSite, NULL, alClient);
	GroupListBox->Visible = true;
	switch (pSite->Align) {
	case alLeft:	Splitter1->Align = alRight;  break;
	case alRight:	Splitter1->Align = alLeft;	 break;
	case alTop:		Splitter1->Align = alBottom; break;
	case alBottom:	Splitter1->Align = alTop;	 break;
	default:		;
	}
	Splitter1->Visible = true;
	Splitter1->Align   = pSite->Align;

	EV->glstAlign = pSite->Align;

	UpdateGrpLst();
}
//---------------------------------------------------------------------
//�O���[�v���X�g��f�Ѓ��X�g�փh�b�L���O
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::DockGrpFrgList()
{
	TPanel *pSite = FragGrid->DockPanel;
	pSite->Constraints->MinWidth = 20;
	pSite->Width = IniFile->ReadInteger("GroupList", "Width", 120);
	if (pSite->DockClientCount==0) GroupListBox->ManualDock(pSite, NULL, alClient);
}

//===========================================================================
//�y�\�����j���[�z
//===========================================================================
void __fastcall TIdeaFragMainForm::Show1Click(TObject *Sender)
{
	ShowGrpLstItem->Checked    = GroupListBox->Visible;
	ShowTool1Item->Checked	   = ToolBar1->Visible;
	ShowTool2Item->Checked	   = ToolBar2->Visible;
	ShowTool3Item->Checked	   = ToolBar3->Visible;
	ShowStatusBarItem->Checked = StatusBar->Visible;
	ShowMapViewerItem->Checked = MapViewer->Visible;
	ShowFrgGridItem->Checked   = FragGrid->Visible;
	ShowNamePlateItem->Checked = EV->show_NamePlate;
	ShowLinkMarkItem->Checked  = EV->show_LinkMark;
	ShowGFrameItem->Checked    = EV->show_GroupFrame;
	ShowGLineItem->Checked	   = EV->show_GroupLine;
	ShowFLineItem->Checked	   = EV->show_FragLine;
	ShowDivLineItem->Checked   = EV->show_DivLine;
	ShowPosGridItem->Checked   = EV->show_PosGrid;
	FragSet *fsp = CurFS;

	ShowDivLineItem->Enabled = fsp? (fsp->div_x>1 || fsp->div_y>1) : false;
}
//---------------------------------------------------------------------
//�O���[�v���X�g
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowGrpLstExecute(TObject *Sender)
{
	//�\������\��
	if (GroupListBox->Visible) {
		SaveGrpLstPos();
		GroupListBox->Visible = false;
 		SetFocus();
	}
	//��\�����\��
	else {
		GroupListBox->Visible = true;
		UpdateGrpLst();
		if (!GroupListBox->Floating) {
			//�h�b�L���O
			if (EV->LastDockSite == FragGrid->DockPanel) {
				//�f�Ѓ��X�g
				DockGrpFrgList();
				if (FragGrid->Visible) GroupListBox->SetFocus();
			}
			else {
				//���C��
				switch (EV->glstAlign) {
				case alLeft:   SetGrpLstPos(DockPanel1); break;
				case alTop:    SetGrpLstPos(DockPanel2); break;
				case alRight:  SetGrpLstPos(DockPanel3); break;
				case alBottom: SetGrpLstPos(DockPanel4); break;
				default:	   ;
				}
				GroupListBox->SetFocus();
			}
		}
		else {
			GroupListBox->SetFocus();
		}
	}
}
//---------------------------------------------------------------------
//�c�[���o�[
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowToolItemClick(TObject *Sender)
{
	switch (((TComponent*)Sender)->Tag) {
	case 0: ToolBar1->Visible = !ToolBar1->Visible; break;
	case 1: ToolBar2->Visible = !ToolBar2->Visible; break;
	case 2: ToolBar3->Visible = !ToolBar3->Visible; break;
	}
}
//---------------------------------------------------------------------
//�X�e�[�^�X�o�[
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowStatusBarExecute(TObject *Sender)
{
	StatusBar->Visible = !StatusBar->Visible;
}
//---------------------------------------------------------------------
//�S�̐}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowMapViewerExecute(TObject *Sender)
{
	if (!MapViewer->Visible) MapViewer->Show(); else MapViewer->Close();
}
//---------------------------------------------------------------------
//�f�Јꗗ
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowFrgGridExecute(TObject *Sender)
{
	if (!FragGrid->Visible) FragGrid->Show(); else FragGrid->Close();
}
//---------------------------------------------------------------------
//�\�D
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowNamePlateExecute(TObject *Sender)
{
	EV->show_NamePlate = !EV->show_NamePlate;
	if (CurFS) CurFS->GInfs->repaint_frame(true);
	UpdateActiveChild();
}
//---------------------------------------------------------------------
//�����N�}�[�N
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowLinkMarkExecute(TObject *Sender)
{
	EV->show_LinkMark = !EV->show_LinkMark;
	for (int i=MDIChildCount-1; i>=0; i--) {
		TMDIChild *cp = (TMDIChild*)MDIChildren[i];
		if (cp->FS) {
			for (int j=0; j<cp->FS->FrgList->Count; j++)
				cp->FS->FrgList->Items[j]->Changed = true;
			cp->FS->all_update();
		}
	}
}
//---------------------------------------------------------------------
//�O���[�v�g
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowGFrameExecute(TObject *Sender)
{
	EV->show_GroupFrame = !EV->show_GroupFrame;
	if (CurFS) CurFS->GInfs->repaint_frame(true);
	UpdateActiveChild();
}
//---------------------------------------------------------------------
//�O���[�v�֌W��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowGLineExecute(TObject *Sender)
{
	EV->show_GroupLine = !EV->show_GroupLine;
	UpdateActiveChild();
}
//---------------------------------------------------------------------
//�f�Њ֌W��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowFLineExecute(TObject *Sender)
{
	EV->show_FragLine = !EV->show_FragLine;
	UpdateActiveChild();
}
//---------------------------------------------------------------------
//��ʕ�����
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowDivLineExecute(TObject *Sender)
{
	EV->show_DivLine = !EV->show_DivLine;
	if (CurFS) CurFS->frg_owner->Invalidate();
}
//---------------------------------------------------------------------
//�z�u�O���b�h
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ShowPosGridExecute(TObject *Sender)
{
	EV->show_PosGrid = !EV->show_PosGrid;
	if (CurFS) CurFS->frg_owner->Invalidate();
}

//===========================================================================
//�y�E�B���h�E���j���[�z
//===========================================================================
void __fastcall TIdeaFragMainForm::WindowCascadeExecute(TObject *Sender)
{
	Cascade();
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::WindowTileHExecute(TObject *Sender)
{
	TileMode = tbHorizontal;
	Tile();
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::WindowTileVExecute(TObject *Sender)
{
	TileMode = tbVertical;
	Tile();
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::WindowArrangeExecute(TObject *Sender)
{
	ArrangeIcons();
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::WindowActMaxExecute(TObject *Sender)
{
	TMDIChild *cp = (TMDIChild*)ActiveMDIChild;
	if (cp) cp->WindowState = wsMaximized;
}
//---------------------------------------------------------------------
//�E�B���h�E�̑I��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::SelWinItemClick(TObject *Sender)
{
	int n = ((TComponent*)Sender)->Tag;
	EV->TimerLockCnt++;
	if (n>=0 && n<MDIChildCount) {
		MDIChildren[n]->Show();
		if (MDIChildren[n]->WindowState==wsMinimized)
			MDIChildren[n]->WindowState = wsNormal;
		UpdateActiveChild();
	}
	EV->TimerLockCnt--;
}


//===========================================================================
//�y�I�v�V�������j���[�z
//===========================================================================
//�ݒ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::OptionSetupExecute(TObject *Sender)
{
	if (FindExForm->Visible) FindExForm->Close();

	if (OptionDlg->ShowModal()==mrOk) {
		FragSet *fsp = CurFS;
		//�f�ЃE�B���h�E�̍X�V
		for (int i=MDIChildCount-1; i>=0; i--) {
			TMDIChild *cp = (TMDIChild*)MDIChildren[i];
			FragSet *cfsp = cp->FS;	if (!cfsp) continue;
			if (cfsp->col_backgr==clNone) cp->Color = EV->col_backgr;
			if (cfsp->ParentFrgFont) cfsp->frgFont->Assign(EV->frgFont);
			if (cfsp->ParentNamFont) cfsp->namFont->Assign(EV->namFont);
			if (cfsp->ParentSymFont) cfsp->symFont->Assign(EV->symFont);
			if (cfsp->ParentLblFont) cfsp->lblFont->Assign(EV->lblFont);
			if (cfsp->ParentTitFont) cfsp->titFont->Assign(EV->titFont);
			for (int j=0; j<cfsp->FrgList->Count; j++) {
				Fragment *fp = cfsp->FrgList->Items[j];
				fp->Changed = true;
			}
			cfsp->update_grp_empty();
			cfsp->all_update(true);
			if (cfsp == fsp) fsp->frg_owner->Invalidate();
		}
		//�O���[�v���X�g�̐ݒ�ύX
		GroupListBox->Font->Assign(EV->glstFont);
		GroupListBox->Font->Color = EV->col_glstFG;
		GroupListBox->Color		  = EV->col_glstBG;
		GroupListBox->Style 	  = lbOwnerDrawFixed;	//Style���ω����Ȃ���OnMeasureItem���Ă΂�Ȃ�(��)
		GroupListBox->Style 	  = lbOwnerDrawVariable;
		GroupListBox->Repaint();
		//�f�Ѓ��X�g�̐ݒ�ύX
		FragGrid->FrgLstGrid->Font->Assign(EV->flstFont);
		FragGrid->FrgLstGrid->DefaultRowHeight = get_FontHeightP(EV->flstFont, EV->FLstInterLine + 12);
		FragGrid->FrgLstGrid->Repaint();
		//�X�e�[�^�X�o�[�̐ݒ�ύX
		StatusBar->Font->Assign(EV->stbarFont);
		StatusBar->Canvas->Font->Assign(EV->stbarFont);
		StatusBar->ClientHeight			   = StatusBar->Canvas->TextHeight("�f��") + 4;
		StatusBar->Panels->Items[0]->Width = StatusBar->Canvas->TextWidth("00/00 ");
		StatusBar->Panels->Items[1]->Width = StatusBar->Canvas->TextWidth("�f�А�=1000  /�I��=1000 ");
		//�S�̐}�̓����x
		MapViewer->AlphaBlendValue = EV->MapAlpha;
		//�S�̐}�̃T�C�Y�t�B�b�g
		if (EV->FitMapView && MapViewer->Visible) MapViewer->AdjustWinHeight();
	}

	Application->DefaultFont->Assign(EV->dialogFont);
}
//---------------------------------------------------------------------
//�ʐݒ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::OptionIndividualExecute(TObject *Sender)
{
	if (CurFS) {
		OptIndDlg->FS = CurFS;
		if (OptIndDlg->ShowModal()==mrOk) {
			CurFS->modify = true;
			for (int j=0; j<CurFS->FrgList->Count; j++)
				CurFS->FrgList->Items[j]->Changed = true;
			CurFS->all_update();
			CurFS->frg_owner->Invalidate();
		}
	}
}

//===========================================================================
//�y�w���v���j���[�z
//===========================================================================
void __fastcall TIdeaFragMainForm::HelpContentsExecute(TObject *Sender)
{
	HtmlHelpContext(1);
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::HelpHomePageExecute(TObject *Sender)
{
	::ShellExecute(NULL, _T("open"), SUPPORT_URL, NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::HelpAboutExecute(TObject *Sender)
{
	AboutBox->ShowModal();
}


//---------------------------------------------------------------------------
//�O���[�v���X�g
//---------------------------------------------------------------------------
//���ڂ̍�����ݒ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxMeasureItem(
	TWinControl *Control, int Index, int &Height)
{
	FragSet *fsp = CurFS;
	if (fsp) {
		int gn = Index + 1;
		if (gn>MAX_GROUP) gn = 0;
		grp_inf *gp = &fsp->GInfs->Items[gn];
		if (EV->HideBlkGrpItem && gn!=0 && gp->TotalSize==0)
			Height = 0;	//��\��
		else 
			Height = get_FontHeightP(EV->glstFont, EV->GLstInterLine);
	}
	else {
		Height = 0;	//��\��
	}
}
//---------------------------------------------------------------------
//�O���[�v���X�g�̕`��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxDrawItem(TWinControl *Control,
	int Index, TRect &Rect, TOwnerDrawState State)
{
	TListBox *lst = (TListBox*)Control;
	TCanvas  *cv  = lst->Canvas;
	cv->Brush->Color = EV->col_glstBG;
	cv->FrameRect(Rect);

	FragSet *fsp  = CurFS;  if (!fsp) return;

	int gn = Index + 1;
	if (gn>MAX_GROUP) gn = 0;
	grp_inf *gp = &fsp->GInfs->Items[gn];

	if (!gp || (EV->HideBlkGrpItem && gn!=0 && gp->TotalSize==0)) return;

	//�`�F�b�N�{�b�N�X
	TRect ckrc = Rect;
	int fh = abs(cv->Font->Height);
	int w = (fh<14)? 12 : 14;
	ckrc.Left += 2; ckrc.Right = ckrc.Left + w;
	InflateRect(&ckrc, 0, (w - (ckrc.Bottom - ckrc.Top))/2);
	if (gp->Visible) {
		::DrawFrameControl(cv->Handle, &ckrc, DFC_BUTTON, DFCS_BUTTONCHECK|DFCS_CHECKED);
		cv->Brush->Color = clBtnShadow;
		cv->FrameRect(ckrc);
	}
	else {
		cv->Pen->Color	 = clBtnShadow;
		cv->Brush->Color = EV->col_glstBG;
		cv->Rectangle(ckrc.Left, ckrc.Top, ckrc.Right, ckrc.Bottom);
	}

	//�e�L�X�g
	if (State.Contains(odSelected)) {
		cv->Brush->Color = clHighlight;
		cv->Font->Color  = clHighlightText;
	}
	else {
		cv->Brush->Color = EV->col_glstBG;
		cv->Font->Color  = gp->Visible? EV->col_glstFG : EV->col_glstOff;
	}
	TRect rc = Rect;
	rc.Left += (w + 4);
	cv->FillRect(rc);
	cv->TextOut(rc.Left + 14, rc.Top + ((rc.Bottom - rc.Top) - fh)/2, lst->Items->Strings[Index]);

	//�O���[�v�F��\��
	if (gn>0 && gp->Visible) {
		//�O���[�v�g
		TRect grc = rc;
		grc.Right = grc.Left + 6;
		grc.Top++; grc.Bottom--;
		cv->Brush->Color = gp->gfp->Pen->Color;
		cv->FillRect(grc);
		//�\�D
		Fragment *np = fsp->GInfs->get_name_plate(gn, NULL);
		if (np) {
			grc.Left += 6;
			grc.Right = grc.Left + 6;
			cv->Brush->Color = np->colBG;
			cv->FillRect(grc);
		}
	}

	//�t�H�[�J�X�`��̂��߂ɔw�i��߂��Ă���
	::SetBkColor(cv->Handle, ColorToRGB(EV->col_glstBG));
}

//---------------------------------------------------------------------
//���X�g�̍X�V
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::UpdateGrpLst()
{
	if (EV->TimerLockCnt>0) return;
	if (!GroupListBox->Visible) return;

	UnicodeString NamePlate, tmpstr;
	int gn, idx;
	FragSet *fsp = CurFS;
	if (fsp) {
		for (gn=0; gn<=MAX_GROUP; gn++) {
			if (gn==0) {
				idx = MAX_GROUP;
				NamePlate = "������";
			}
			else {
				idx = gn - 1;
				fsp->GInfs->get_name_plate(gn, &NamePlate);
			}
			tmpstr = EmptyStr;
			if (!fsp->GInfs->Items[gn].Empty)
				tmpstr.sprintf(_T("%3u: "), fsp->GInfs->Items[gn].TotalSize);
			else {
				if (EV->HideBlkGrpItem && gn!=0)
					tmpstr = EmptyStr;
				else
					tmpstr = "___: ";
			}
			if (!tmpstr.IsEmpty()) tmpstr += NamePlate;
			if (GroupListBox->Items->Strings[idx]!=tmpstr)
				GroupListBox->Items->Strings[idx] = tmpstr;
		}
		GroupListBox->ItemIndex = (fsp->GInfs->cur_group>0)?  (fsp->GInfs->cur_group - 1) : MAX_GROUP;
	}
	else {
		for (gn=0; gn<=MAX_GROUP; gn++) {
			idx = (gn>0)? (gn - 1) : MAX_GROUP;
			GroupListBox->Items->Strings[idx] = EmptyStr;
		}
		GroupListBox->ItemIndex	= 0;
	}
}

//---------------------------------------------------------------------------
//�O���[�v���X�g�̃T�C�Y�E�ʒu����ۑ�
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::SaveGrpLstPos()
{
	if (!GroupListBox->Visible) return;
	TControl *st = GroupListBox->HostDockSite;
	UnicodeString sct = "GroupList";
	//����
	if (GroupListBox->Floating) {
		IniFile->WriteInteger(sct, "FLeft",  st->BoundsRect.Left);
		IniFile->WriteInteger(sct, "FTop",   st->BoundsRect.Top);
		IniFile->WriteInteger(sct, "FRight", st->BoundsRect.Right);
		IniFile->WriteInteger(sct, "FBottom",st->BoundsRect.Bottom);
	}
	//�h�b�L���O
	else {
		if (st==FragGrid->DockPanel) {
			IniFile->WriteString(sct, "DockSite",	"FragList");
			IniFile->WriteString(sct, "Width",		 FragGrid->DockPanel->Width);
		}
		else {
			IniFile->WriteString(sct, "DockSite", "Main");
			switch (EV->glstAlign) {
			case alLeft:   IniFile->WriteInteger(sct, "Width", DockPanel1->Width);  break;
			case alTop:    IniFile->WriteInteger(sct, "Height",DockPanel2->Height); break;
			case alRight:  IniFile->WriteInteger(sct, "Width", DockPanel3->Width);  break;
			case alBottom: IniFile->WriteInteger(sct, "Height",DockPanel4->Height); break;
			default:	   ;
			}
		}
	}
	IniFile->WriteInteger(sct, "UnDockWidth", GroupListBox->UndockWidth);
	IniFile->WriteInteger(sct, "UnDockHeight",GroupListBox->UndockHeight);
}

//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxStartDock(TObject *Sender,
	TDragDockObject *&DragObject)
{
	DockPanel1->Visible = true;
	DockPanel2->Visible = true;
	DockPanel3->Visible = true;
	DockPanel4->Visible = true;
	SaveGrpLstPos();

	TControl *cp = (TControl*)Sender;
	DragObject = new TToolDockObject(cp);

	TControl *st = cp->HostDockSite;
	if (st) {
		UnicodeString sct = "GroupList";
		TRect rc = Rect(IniFile->ReadInteger(sct, "FLeft",		0),
				 		IniFile->ReadInteger(sct, "FTop",		0),
				 		IniFile->ReadInteger(sct, "FRight",		200),
				 		IniFile->ReadInteger(sct, "FBottom",	200));
		st->Width  = rc.Width();
		st->Height = rc.Height();

		TCustomDockForm *dp = dynamic_cast<TCustomDockForm *>(st);
		if (dp) dp->Caption = "�O���[�v���X�g";
	}
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxEndDock(TObject *Sender,
	TObject *Target, int X, int Y)
{
	GroupListBox->SetFocus();
}
//---------------------------------------------------------------------------
//�O���[�v���X�g���h�b�L���O����
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::DockPanelDockDrop(TObject *Sender,
	TDragDockObject *Source, int X, int Y)
{
	TPanel *pSite = (TPanel*)Sender;
	SetGrpLstPos(pSite);
	EV->LastDockSite = pSite;
}
//---------------------------------------------------------------------------
//�O���[�v���X�g���A���h�b�N���ꂽ
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::DockPanelUnDock(TObject *Sender,
	TControl *Client, TWinControl *NewTarget, bool &Allow)
{
	TPanel *pSite = (TPanel*)Sender;

	switch (pSite->Align) {
	case alLeft: case alRight:
		pSite->Constraints->MinWidth = 0;
		pSite->Width  = 0;
		break;
	case alTop: case alBottom:
		pSite->Constraints->MinHeight = 0;
		pSite->Height = 0;
		break;
	default:
		;
	}
	Splitter1->Visible = false;
}
//---------------------------------------------------------------------------
//�h�b�L���O����
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ToolPanelDockOver(TObject *Sender,
	TDragDockObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	if (Source->Control==GroupListBox)
		Accept = false;
	else
		Source->Control->Height = 28;
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::DockPanelGetSiteInfo(TObject *Sender,
	TControl *DockClient, TRect &InfluenceRect, TPoint &MousePos, bool &CanDock)
{
	if (DockClient==ToolBar1 || DockClient==ToolBar2 || DockClient==ToolBar3)
		CanDock = false;
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::DockPanelDockOver(TObject *Sender,
	TDragDockObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	if (Source->Control==ToolBar1 || Source->Control==ToolBar2 || Source->Control==ToolBar3) {
		Accept = false;
	}
	else if (Source->Control==GroupListBox) {
		TControl *cp = (TControl*)Sender;
		if (State==dsDragEnter) {
			if (cp->Align==alLeft || cp->Align==alRight)
				cp->Width  = IniFile->ReadInteger("GroupList", "Width",		120);
			else
				cp->Height = IniFile->ReadInteger("GroupList", "Height",	120);
		}
		else if (State==dsDragLeave) {
			if (cp->Align==alLeft || cp->Align==alRight)
				cp->Width  = 0;
			else
				cp->Height = 0;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ToolBarsStartDock(TObject *Sender, TDragDockObject *&DragObject)
{
	DragObject = new TToolDockObject((TControl*)Sender);
}

//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditPopupMenuPopup(TObject *Sender)
{
	Edit1Click(NULL);
	UpdateMenuGrpLst(Sender);

	//�E�N���b�N�ʒu���擾(�u�}�E�X�ʒu�Ɂ`�v�ɗ��p)
	TPoint p = Point(-1, -1);
	TMDIChild *cp = (TMDIChild*)ActiveMDIChild;
	if (cp) {
		p = cp->ScreenToClient(Mouse->CursorPos);
		if (p.x<0) p.x = 0;
		if (p.y<0) p.y = 0;
		//�ʒu�}�[�N��\��
		TShape *sp = cp->PopPosShape;
		sp->Left = p.x - sp->Width/2;
		sp->Top  = p.y - sp->Height/2;
		sp->Visible = true;
	}
	PopupPos = p;
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GlistPopupMenuPopup(TObject *Sender)
{
	GListFloatItem->Checked  = GroupListBox->Floating;
	if (!GroupListBox->Floating) {
		bool flag = (EV->LastDockSite == FragGrid->DockPanel);
		GListLeftItem->Checked	 = (EV->glstAlign==alLeft	&& !flag);
		GListRightItem->Checked  = (EV->glstAlign==alRight  && !flag);
		GListTopItem->Checked	 = (EV->glstAlign==alTop	&& !flag);
		GListBottomItem->Checked = (EV->glstAlign==alBottom && !flag);
		GListDockFlist->Checked  = flag;
	}
	GListNotBlankItem->Visible = !EV->HideBlkGrpItem;
	GListHideBlkItem->Checked  = EV->HideBlkGrpItem;
}
//---------------------------------------------------------------------
//��O���[�v���ڂ͔�\��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GListHideBlkItemClick(TObject *Sender)
{
	EV->HideBlkGrpItem = !EV->HideBlkGrpItem;
	UpdateGrpLst();
	GroupListBox->Style = lbOwnerDrawFixed;	//Style���ω����Ȃ���OnMeasureItem���Ă΂�Ȃ�(��)
	GroupListBox->Style = lbOwnerDrawVariable;
	GroupListBox->Repaint();

	FragSet *fsp = CurFS;	if (!fsp) return;
	if (EV->HideBlkGrpItem) {
		for (int idx=GroupListBox->ItemIndex; idx<GroupListBox->Items->Count; idx++) {
			if (GroupListBox->Items->Strings[idx].IsEmpty()) continue;
			GroupListBox->ItemIndex	= idx;
			fsp->GInfs->cur_group = (idx==MAX_GROUP)? 0 : (idx + 1);
			break;
		}
	}
}

//---------------------------------------------------------------------
//�O���[�v���X�g�̃h�b�L���O�ʒu�ύX
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GListPosItemClick(TObject *Sender)
{
	switch (((TComponent*)Sender)->Tag) {
	case  1: SetGrpLstPos(DockPanel1); break;	//��
	case  2: SetGrpLstPos(DockPanel2); break;	//��
	case  3: SetGrpLstPos(DockPanel3); break;	//�E
	case  4: SetGrpLstPos(DockPanel4); break;	//��
	case 10: DockGrpFrgList();		   break;	//�f�Ѓ��X�g
	}
}
//---------------------------------------------------------------------
//������
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GListFloatItemClick(TObject *Sender)
{
	if (!GroupListBox->Floating) {
		SaveGrpLstPos();
		FloatGrpLst();
	}
}
//---------------------------------------------------------------------
//�B��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GListHideItemClick(TObject *Sender)
{
	GroupListBox->Visible = false;
	SetFocus();
}
//---------------------------------------------------------------------
//�O���[�v�I��/����
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxDblClick(TObject *Sender)
{
	if (!GlDblSkip) {
		FragSet *fsp = CurFS;
		if (fsp) {
			int idx = GroupListBox->ItemIndex;
			int gn	= (idx==MAX_GROUP)? 0 : (idx + 1);
			if (idx!=-1) {
				//�����̒f�ЗL��
				if (!fsp->GInfs->Items[gn].Empty) {
					if (fsp->get_sel_group()!=gn) {
						fsp->group_select(gn, true);
						FragGrid->SetGroupTop(gn);
					}
					else {
						fsp->all_select(false);
					}
				}
				//��O���[�v
				else {
					fsp->all_select(false);
					if (EV->NewBlkDblClick) {
						//�V�K�f�Ѝ쐬
						FrgPropDlg->prpFS	   = fsp;
						FrgPropDlg->swNewFrag  = true;
						FrgPropDlg->swNewGroup = true;
						FrgPropDlg->newGrp	   = gn;
						for (int i=0; i<MAX_IN_GRP; i++) FrgPropDlg->newGstk.stk[i] = (i==0)? gn : -1;
						FrgPropDlg->ShowModal();
						CloseIME();
					}
				}
			}
		}
	}
	GlDblSkip = false;
}
//---------------------------------------------------------------------
//�I���O���[�v�̕\�D���g�b�v��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxClick(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	int idx = GroupListBox->ItemIndex;
	fsp->GInfs->cur_group = (idx==MAX_GROUP)? 0 : (idx + 1);
	Fragment *np = fsp->GInfs->get_name_plate(fsp->GInfs->cur_group, NULL);
	if (np) {
		fsp->bring_to_front(np); np->show_frag();
	}
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxMouseMove(TObject *Sender,
	TShiftState Shift, int X, int Y)
{
	GlDblSkip = false;
}

//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	GroupListBox->SetFocus();
	int idx = GroupListBox->ItemAtPos(Point(X, Y), true);
	if (idx!=-1) {
		if (X<16) {
			//�O���[�v�\����Ԑ؂�ւ�(�`�F�b�N�{�b�N�X����)
			FragSet *fsp = CurFS;
			if (fsp) {
				fsp->push_all();
				idx = (idx==MAX_GROUP)? 0 : (idx + 1);
				fsp->GInfs->Items[idx].Visible = !fsp->GInfs->Items[idx].Visible;
				if (Shift.Contains(ssShift)) {	//�\��
					for (int i=0; i<=MAX_GROUP; i++) if (i!=idx) fsp->GInfs->Items[i].Visible = false;
				}
				fsp->update_group_frame();
				fsp->modify = true;
				GroupListBox->Repaint();
				fsp->frg_owner->Invalidate();
			}
			GlDblSkip = true;
		}
		GroupListBox->DragKind = dkDrag;
	}
	LastItemRect = Rect(0, 0, 0, 0);
}
//---------------------------------------------------------------------
//�O���[�v����ւ���̕\��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxDragOver(TObject *Sender,
	TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	FragSet *fsp = CurFS;
	if (!fsp) {
		Accept = false;
	}
	else {
		int idx = GroupListBox->ItemAtPos(Point(X, Y), true);
		if (!fsp->read_only && idx!=-1 && fsp->GInfs->cur_group>0) {
			TRect rc = GroupListBox->ItemRect(idx);
			if (rc != LastItemRect) {
				GroupListBox->Canvas->DrawFocusRect(LastItemRect);
				GroupListBox->Canvas->DrawFocusRect(rc);
				LastItemRect = rc;
			}
		}
		else {
			Accept = false;
		}
	}
}
//---------------------------------------------------------------------
//�O���[�v�̒��g���h���b�O&�h���b�v�œ���ւ�
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxDragDrop(TObject *Sender,
	TObject *Source, int X, int Y)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	int idx = GroupListBox->ItemAtPos(Point(X, Y), true);
	if (!fsp->read_only && idx!=-1 && fsp->GInfs->cur_group>0){
		idx++;
		fsp->exchange_group(fsp->GInfs->cur_group, idx);
		fsp->GInfs->cur_group = idx;
		UpdateActiveChild();
	}
	GroupListBox->DragKind = dkDock;
}
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxEndDrag(TObject *Sender,
	TObject *Target, int X, int Y)
{
	GroupListBox->DragKind = dkDock;
}
//---------------------------------------------------------------------
//�O���[�v���ЂƂ��
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GListUpItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	if (ActiveControl!=GroupListBox) return;
	if (fsp->read_only) return;
	if (fsp->GInfs->cur_group>1) {
		int gn2;
		if (EV->HideBlkGrpItem) {
			gn2 = -1;
			for (int gn=fsp->GInfs->cur_group-1; gn>0; gn--) {
				if (fsp->GInfs->Items[gn].TotalSize>0) {
					gn2 = gn; break;
				}
			}
		}
		else {
			gn2 = fsp->GInfs->cur_group - 1;
		}

		if (gn2!=-1) {
			fsp->exchange_group(fsp->GInfs->cur_group, gn2);
			fsp->GInfs->cur_group = gn2;
			UpdateActiveChild();
		}
	}
}
//---------------------------------------------------------------------
//�O���[�v���ЂƂ���
//---------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GListDownItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	if (ActiveControl!=GroupListBox) return;
	if (fsp->read_only) return;

	if (fsp->GInfs->cur_group>0 && fsp->GInfs->cur_group<MAX_GROUP) {
		int gn2;
		if (EV->HideBlkGrpItem) {
			gn2 = -1;
			for (int gn=fsp->GInfs->cur_group+1; gn<MAX_GROUP; gn++) {
				if (fsp->GInfs->Items[gn].TotalSize>0) {
					gn2 = gn; break;
				}
			}
		}
		else {
			gn2 = fsp->GInfs->cur_group + 1;
		}

		if (gn2!=-1) {
			fsp->exchange_group(fsp->GInfs->cur_group, gn2);
			fsp->GInfs->cur_group = gn2;
			UpdateActiveChild();
		}
	}
}
//---------------------------------------------------------------------------
//�O���[�v�̕\����ԕύX
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GListDispItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	fsp->push_all();

	for (int gn=0; gn<=MAX_GROUP; gn++) {
		grp_inf *gp = &fsp->GInfs->Items[gn];
		switch (((TComponent*)Sender)->Tag) {
		case 0:	//�S�ẴO���[�v��\��
			gp->Visible = true; 		break;
		case 1:	//��łȂ��O���[�v�݂̂�\��
			gp->Visible = !gp->Empty;	break;
		case 2:	//�S�ẴO���[�v���B��
			gp->Visible = false;		break;
		}
	}
	fsp->modify = true;
	UpdateActiveChild();
	GroupListBox->Repaint();
}

//---------------------------------------------------------------------------
//�O���[�v���X�g�ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
	FragSet *fsp = CurFS;
	int idx;

	switch (Key) {
	case VK_ESCAPE:	//case VK_CONTROL:
		//�O���[�v���X�g����t�H�[�J�X��D��
		ActiveControl = NULL;
		SetFocus();
		break;

	case VK_RETURN:
		if (!fsp) break;
		//�O���[�v�̕\���^��W���؂芷��
		if (Shift.Contains(ssCtrl) || Shift.Contains(ssShift)) {
			fsp->push_all();
			idx = GroupListBox->ItemIndex;  if (idx==-1) break;
			idx = (idx==MAX_GROUP)? 0 : (idx + 1);
			fsp->GInfs->Items[idx].Visible = !fsp->GInfs->Items[idx].Visible;
			if (Shift.Contains(ssShift)) {	//�\��
				for (int i=0; i<=MAX_GROUP; i++)
					if (i!=idx) fsp->GInfs->Items[i].Visible = false;
			}
			GroupListBox->Repaint();
			fsp->update_group_frame();
			fsp->modify = true;
			fsp->frg_owner->Invalidate();
		}
		//�O���[�v�I��/����
		else {
			GroupListBoxDblClick(Sender);
		}
		break;

	case VK_LEFT:
		Key = 0;
		break;

	case VK_RIGHT:
		Key = 0;
		if (!GroupListBox->Focused()) break;
		if (GroupListBox->Floating) break;
		if (GroupListBox->HostDockSite!=FragGrid->DockPanel) break;
		FragGrid->FrgLstGrid->SetFocus();
		break;

	case VK_DOWN:
		//�J�[�\���_�E������
		if (!fsp) break;
		if (Shift.Contains(ssAlt)) break;
		if (!EV->HideBlkGrpItem) break;
		for (idx=GroupListBox->ItemIndex+1; idx<GroupListBox->Items->Count; idx++) {
			if (!GroupListBox->Items->Strings[idx].IsEmpty()) {
				GroupListBox->ItemIndex	= idx;
				fsp->GInfs->cur_group = (idx==MAX_GROUP)? 0 : (idx + 1);
				break;
			}
		}
		Key = 0;
		break;
	case VK_UP:
		//�J�[�\���A�b�v����
		if (!fsp) break;
		if (Shift.Contains(ssAlt)) break;
		if (!EV->HideBlkGrpItem) break;
		for (idx=GroupListBox->ItemIndex-1; idx>=0; idx--) {
			if (!GroupListBox->Items->Strings[idx].IsEmpty()) {
				GroupListBox->ItemIndex	= idx;
				fsp->GInfs->cur_group = (idx==MAX_GROUP)? 0 : (idx + 1);
				break;
			}
		}
		Key = 0;
		break;
	}
}

//---------------------------------------------------------------------------
//�֌W���̃|�b�v�A�b�v���j���[
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GLinePopupMenuPopup(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	int lt, lw, at, as;
	bool op;

	//�����̊֌W���ɑ΂���|�b�v�A�b�v
	if (fsp->CurLp) {
		GLine_s3->Visible	  = true;
		GLineLblItem->Visible = true;
		GLine_s4->Visible	  = true;
		GLineDelItem->Visible = true;
		lt = fsp->CurLp->lType;
		lw = fsp->CurLp->lWidth;
		at = fsp->CurLp->aType;
		as = fsp->CurLp->aSize;
		op = fsp->CurLp->tOpposite;
	}
	//�����������O�̃|�b�v�A�b�v
	else {
		GLine_s3->Visible	  = false;
		GLineLblItem->Visible = false;
		GLine_s4->Visible	  = false;
		GLineDelItem->Visible = false;
		lt = EV->CurGLineType;
		lw = EV->CurGLineWidth;
		at = EV->CurGArrowType;
		as = EV->CurGArrowSize;
		op = EV->CurTxOpposite;
	}

	GLineType0Item->Checked = (lt==LTYPE_LINE);
	GLineType1Item->Checked = (lt==LTYPE_S_ARROW);
	GLineType2Item->Checked = (lt==LTYPE_D_ARROW);
	GLineType3Item->Checked = (lt==LTYPE_CD_ARROW);
	GLineType4Item->Checked = (lt==LTYPE_D_LINE);
	GLineType5Item->Checked = (lt==LTYPE_C_ARROW);
	GLineDotItem->Checked	= (lw == -1);
	GLineWd0Item->Checked	= (lw == 1);
	GLineWd1Item->Checked	= (lw == 3);
	GLineWd2Item->Checked	= (lw == 5);
	GLineWd3Item->Checked	= (lw == 9);
	GLineAtSldItem->Checked = (at == ATYPE_SOLID);
	GLineAtLinItem->Checked = (at == ATYPE_LINE);
	GLineAsSItem->Checked	= (as == ASIZE_SMALL);
	GLineAsMItem->Checked	= (as == ASIZE_MEDIUM);
	GLineAsLItem->Checked	= (as == ASIZE_LARGE);
	GLineOpsItem->Checked   = op;
}
//---------------------------------------------------------------------------
//�֌W���̃^�C�v�ݒ�
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GLineTypeItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	int n = ((TComponent*)Sender)->Tag;

	//�����̊֌W���̃^�C�v�ύX
	if (fsp->CurLp) {
		fsp->push_all();
		ArrowLine *ap = fsp->CurLp;
		ap->erase(fsp->frg_owner);
		switch (n) {
		case   0: ap->lType = LTYPE_LINE;	  break;
		case   1: ap->lType = LTYPE_S_ARROW;  break;
		case   2: ap->lType = LTYPE_D_ARROW;  break;
		case   3: ap->lType = LTYPE_CD_ARROW; break;
		case   4: ap->lType = LTYPE_D_LINE;   break;
		case   5: ap->lType = LTYPE_C_ARROW;  break;
		case -10: ap->lWidth = -1;			  break;
		case  10: ap->lWidth = 1;			  break;
		case  11: ap->lWidth = 3;			  break;
		case  12: ap->lWidth = 5;			  break;
		case  13: ap->lWidth = 9;			  break;
		case  20: ap->aType = ATYPE_SOLID;    break;
		case  21: ap->aType = ATYPE_LINE;	  break;
		case  30: ap->aSize = ASIZE_SMALL;    break;
		case  31: ap->aSize = ASIZE_MEDIUM;   break;
		case  32: ap->aSize = ASIZE_LARGE;    break;
		}
		UpdateActiveChild();
		fsp->modify = true;

	}
	//�f�t�H���g�̕ύX(�����������O�̃|�b�v�A�b�v)
	else {
		int lt, lw, at, as;
		switch (n) {
		case   0: lt = LTYPE_LINE;	   break;
		case   1: lt = LTYPE_S_ARROW;  break;
		case   2: lt = LTYPE_D_ARROW;  break;
		case   3: lt = LTYPE_CD_ARROW; break;
		case   4: lt = LTYPE_D_LINE;   break;
		case -10: lw = -1;			   break;
		case  10: lw = 1;			   break;
		case  11: lw = 3;			   break;
		case  12: lw = 5;			   break;
		case  13: lw = 9;			   break;
		case  20: at = ATYPE_SOLID;    break;
		case  21: at = ATYPE_LINE;	   break;
		case  30: as = ASIZE_SMALL;    break;
		case  31: as = ASIZE_MEDIUM;   break;
		case  32: as = ASIZE_LARGE;    break;
		}
		switch (n) {
		case   0: case   1: case   2: case   3: case   4:
			EV->CurGLineType  = EV->CurFLineType  = lt; break;
		case -10: case  10: case  11: case  12: case  13:
			EV->CurGLineWidth = EV->CurFLineWidth = lw; break;
		case  20: case  21:
			EV->CurGArrowType = EV->CurFArrowType = at; break;
		case  30: case  31: case  32:
			EV->CurGArrowSize = EV->CurFArrowSize = as; break;
		}
		if (fsp->CLineMode==1) fsp->frg_owner->Cursor = crLineTool;
	}
}
//---------------------------------------------------------------------------
//���x������
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GLineLblItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	if (fsp->CurLp) {
		if (EV->ime_on) {
			//IME�I��
			HIMC hIMC = ::ImmGetContext(Handle);
			::ImmSetOpenStatus(hIMC, TRUE);
			::ImmReleaseContext(Handle, hIMC);
		}
		UnicodeString inpstr = fsp->CurLp->TextStr;
		if (InputQuery("�֌W�����x��", EmptyStr, inpstr)) {
			CloseIME();
			fsp->push_all();
			fsp->CurLp->erase(fsp->frg_owner);
			fsp->CurLp->TextStr = inpstr;
			UpdateActiveChild();
			fsp->modify = true;
		}
	}
}
//---------------------------------------------------------------------------
//���x���𔽑Α���
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GLineOpsItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	if (fsp->CurLp) {
		fsp->push_all();
		fsp->CurLp->erase(fsp->frg_owner);
		fsp->CurLp->tOpposite = !fsp->CurLp->tOpposite;
		UpdateActiveChild();
		fsp->modify = true;
	}
	else {
		EV->CurTxOpposite = !EV->CurTxOpposite;
	}
}

//---------------------------------------------------------------------------
//�֌W���̌ʐF�ύX
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GLineColItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;
	if (fsp->CurLp) {
	    ColorDialog1->Color = EV->col_Brush;
		EV->set_custom_col(ColorDialog1->CustomColors);
	    if (ColorDialog1->Execute()) {
		    TColor lc = ColorDialog1->Color;
			EV->get_custom_col(ColorDialog1->CustomColors);
			fsp->push_all();
			fsp->CurLp->lColor = lc;
			fsp->CurLp = NULL;
			fsp->update_frg_line();
			fsp->update_fg_line();
			fsp->update_group_frame();
			fsp->modify = true;
		}
	}
}

//---------------------------------------------------------------------------
//���_��}��
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GLineJncItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;
	if (fsp) {
		fsp->push_all();
		fsp->add_junction();
		UpdateActiveChild();
	}
}
//---------------------------------------------------------------------------
//�������]
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GLineRevItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;		if (!fsp) return;
	if (fsp->CurLp) {
		fsp->push_all();
		fsp->CurLp->erase(fsp->frg_owner);
		fsp->CurLp->reverse();
		UpdateActiveChild();
		fsp->modify = true;
	}
}
//---------------------------------------------------------------------------
//�֌W���̍폜
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GLineDelItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;		if (!fsp) return;
	if (fsp->CurLp) {
		fsp->push_all();
		fsp->CurLp->erase(fsp->frg_owner);
		int idx = fsp->CLineList->IndexOf(fsp->CurLp);
		delete fsp->CurLp; 	fsp->CurLp = NULL;
		if (idx!=-1) fsp->CLineList->Delete(idx);
		fsp->clr_iso_junction();
		UpdateActiveChild();
		fsp->modify = true;
	}
}

//---------------------------------------------------------------------------
//�O���[�v�g�̃|�b�v�A�b�v���j���[
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GFrmPopupMenuPopup(TObject *Sender)
{
	FragSet *fsp = CurFS;
	bool n_flag = false;
	bool e_flag = false;
	do {
		if (!fsp) break;
		if (fsp->read_only) break;
		if (fsp->GInfs->cur_gfrm<=0) break;
		e_flag = true;
		if (fsp->GInfs->get_name_plate(fsp->GInfs->cur_gfrm, NULL)) break;
		if (!EV->PlateOnGfrm) break;
		n_flag = true;
	} while (0);
	GFrmSelItem->Visible	= e_flag;
	GFrmRelItem->Visible	= e_flag;
	GFrmNameItem->Visible	= n_flag;
	GFrmLwidthItem->Visible = e_flag;
}
//---------------------------------------------------------------------------
//�O���[�v�I��
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GFrmSelItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;		if (!fsp) return;
	if (fsp->GInfs->cur_gfrm>0) {
		CurFS->group_select(fsp->GInfs->cur_gfrm, true);
		fsp->GInfs->Items[fsp->GInfs->cur_gfrm].gfp->Repaint();
		fsp->GInfs->cur_gfrm = 0;
	}
}
//---------------------------------------------------------------------------
//�O���[�v����
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GFrmRelItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;		if (!fsp) return;
	if (fsp->GInfs->cur_gfrm>0) {
		fsp->rel_group(fsp->GInfs->cur_gfrm);
		fsp->GInfs->cur_gfrm = 0;
		UpdateActiveChild();
		fsp->modify = true;
	}
}
//---------------------------------------------------------------------------
//�\�D��t����
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GFrmNameItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;		if (!fsp) return;
	int gn = fsp->GInfs->cur_gfrm;
	if (gn>0) {
		grp_inf *gp = &fsp->GInfs->Items[gn];
		FrgPropDlg->prpFS		= fsp;
		FrgPropDlg->swNewFrag	= true;
		FrgPropDlg->swNewCurPos = true;
		FrgPropDlg->newPosX 	= gp->cRC.Left + 30 - fsp->HscrBar->Position;
		FrgPropDlg->newPosY 	= gp->cRC.Top  + 30 - fsp->VscrBar->Position;
		FrgPropDlg->swNewGroup	= true;
		FrgPropDlg->newGrp		= gn;
		fsp->GInfs->gstk_from_pos(FrgPropDlg->newPosX, FrgPropDlg->newPosY, &FrgPropDlg->newGstk);
		FrgPropDlg->swNewPlate	= true;
		FrgPropDlg->ShowModal();
		CloseIME();
	}
}
//---------------------------------------------------------------------------
//�g�����̕ύX
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GFrmSetLwItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;	if (!fsp) return;

	int gn = fsp->GInfs->cur_gfrm;
	if (gn>0) {
		fsp->push_all();
		grp_inf *gp = &fsp->GInfs->Items[gn];
		gp->LnWidth = ((TComponent*)Sender)->Tag;
		fsp->GInfs->update_frame(false);
		gp->gfp->Repaint();
		fsp->modify = true;
	}
}
//---------------------------------------------------------------------------
//�g���̌ʐF�ύX
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GFrmLcolorItemClick(TObject *Sender)
{
	FragSet *fsp = CurFS;		if (!fsp) return;
	int gn = fsp->GInfs->cur_gfrm;
	if (gn>0) {
	    ColorDialog1->Color = EV->col_Brush;
		EV->set_custom_col(ColorDialog1->CustomColors);
	    if (ColorDialog1->Execute()) {
		    TColor fc = ColorDialog1->Color;
			EV->get_custom_col(ColorDialog1->CustomColors);

			fsp->push_all();
			fsp->GInfs->cur_gfrm = 0;
			grp_inf *gp = &fsp->GInfs->Items[gn];
			gp->gfp->Pen->Color = fc;
			if (fc==fsp->col_GrpFrm || fc==EV->col_GrpFrm) fc = clNone;
			gp->col_GrpFrm = fc;
			gp->RepaintFrm = true;
			gp->gfp->Repaint();
			GroupListBox->Repaint();
			fsp->GInfs->update_frame(false);
			gp->gfp->Repaint();
			fsp->modify = true;
		}
	}
}

//---------------------------------------------------------------------------
//�ʏ�ҏW���[�h��
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ToolNormalExecute(TObject *Sender)
{
	EditGroupBtn->Down	= false;
	EditGLineBtn->Down	= false;
	EditBrushBtn->Down	= false;
	EditNormalBtn->Down = true;
	FragSet *fsp = CurFS;	if (!fsp) return;
	fsp->frg_owner->Cursor = crDefault;
	fsp->CLineMode = CLMODE_NON;
}
//---------------------------------------------------------------------------
//�O���[�v�����[�h��
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ToolGroupExecute(TObject *Sender)
{
	EditNormalBtn->Down = false;
	EditGLineBtn->Down	= false;
	EditBrushBtn->Down	= false;
	EditGroupBtn->Down	= true;
	FragSet *fsp = CurFS;	if (!fsp) return;
	fsp->frg_owner->Cursor = crGroupTool;
	fsp->CLineMode = CLMODE_NON;
}

//---------------------------------------------------------------------------
//�֌W�����[�h��
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ToolGLineExecute(TObject *Sender)
{
	EditGroupBtn->Down	= false;
	EditNormalBtn->Down = false;
	EditBrushBtn->Down	= false;
	EditGLineBtn->Down	= true;
	FragSet *fsp = CurFS;	if (!fsp) return;
	fsp->all_select(false);
	fsp->frg_owner->Cursor = crLineTool;
	fsp->CLineMode = CLMODE_READY;
}
//---------------------------------------------------------------------------
//�w�i�u���V���[�h��
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ToolBrushExecute(TObject *Sender)
{
	EditGroupBtn->Down	= false;
	EditNormalBtn->Down = false;
	EditGLineBtn->Down	= false;
	EditBrushBtn->Down	= true;
	FragSet *fsp = CurFS;	if (!fsp) return;
	fsp->frg_owner->Cursor = crBrushTool;
	fsp->CLineMode = CLMODE_NON;
}
//---------------------------------------------------------------------------
//�g������
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ToolFindExExecute(TObject *Sender)
{
	if (!FindExForm->Visible) FindExForm->Show(); else FindExForm->Close();
}

//---------------------------------------------------------------------------
//�w�i�F���Q��
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::BrushPanelClick(TObject *Sender)
{
    ColorDialog1->Color = EV->col_Brush;
	EV->set_custom_col(ColorDialog1->CustomColors);
    if (ColorDialog1->Execute()) {
	    EV->col_Brush	  = ColorDialog1->Color;
		BrushPanel->Color = EV->col_Brush;
		EV->get_custom_col(ColorDialog1->CustomColors);
	}
}
//---------------------------------------------------------------------------
//�����̔w�i�F����h��F��I��
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::BrushChsBgColItemClick(TObject *Sender)
{
	((TComponent*)Sender)->Tag = 1;
	EditSelBgColExecute(Sender);
}

//---------------------------------------------------------------------------
//��ʈړ�
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::ExMoveExecute(TObject *Sender)
{
	TMDIChild *cp = (TMDIChild*)ActiveMDIChild;		if (!cp) return;

	switch (((TComponent*)Sender)->Tag) {
	case 0:	//��[
		cp->VertScrollBar->Position = 0;	break;
	case 1:	//���[
		if (cp->VertScrollBar->Range > cp->ClientHeight)
			cp->VertScrollBar->Position = cp->VertScrollBar->Range - cp->ClientHeight;
		break;
	case 2:	//���[
		cp->HorzScrollBar->Position = 0;	break;
	case 3:	//�E
		if (cp->HorzScrollBar->Range > cp->ClientWidth)
			cp->HorzScrollBar->Position = cp->HorzScrollBar->Range - cp->ClientWidth;
		break;
	case 4:	//����
		if (cp->HorzScrollBar->Range > cp->ClientWidth)
			cp->HorzScrollBar->Position = (cp->HorzScrollBar->Range - cp->ClientWidth)/2;
		if (cp->VertScrollBar->Range > cp->ClientHeight)
			cp->VertScrollBar->Position = (cp->VertScrollBar->Range - cp->ClientHeight)/2;
		break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxEnter(TObject *Sender)
{
	if (GroupListBox->Floating) return;
	if (GroupListBox->HostDockSite!=FragGrid->DockPanel) return;
	FragGrid->DockPanel->Color = clHighlight;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::GroupListBoxExit(TObject *Sender)
{
	if (GroupListBox->Floating) return;
	if (GroupListBox->HostDockSite!=FragGrid->DockPanel) return;
	FragGrid->DockPanel->Color = FragGrid->Color;
}
//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::EditTrimExecute(TObject *Sender)
{
	FragSet *fsp = CurFS;		if (!fsp) return;
	fsp->trim_select();
	if (EV->TrimSaveAs) FileSaveAs->Execute();
}

//---------------------------------------------------------------------------
void __fastcall TIdeaFragMainForm::RubberLayExecute(TObject *Sender)
{
	RubLayDlg->Show();
}
//---------------------------------------------------------------------------
