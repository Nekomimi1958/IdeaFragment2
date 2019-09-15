//----------------------------------------------------------------------//
//	�g������															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <RegularExpressions.hpp>
#include "UserFunc.h"
#include "EnvData.h"
#include "Main.h"
#include "FindEx.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFindExForm *FindExForm;

//===========================================================================
//�������ʃ��X�g (TList ����p��)
//===========================================================================
__fastcall TQFragList::TQFragList(): TList() {
}
//---------------------------------------------------------------------------
q_fragment * __fastcall TQFragList::Get(int Index) {
 	return (q_fragment*)(TList::Get(Index));
}
//---------------------------------------------------------------------------
void __fastcall TQFragList::Put(int Index, q_fragment *Item) {
	TList::Put(Index, Item);
}

//---------------------------------------------------------------------------
//�ꗗ�\�[�g�p��r�֐�
//---------------------------------------------------------------------------
int __fastcall QFrgCmp(void * Item1, void * Item2)
{
	q_fragment *qp1 = (q_fragment*)Item1;
	q_fragment *qp2 = (q_fragment*)Item2;
	int ret = 0;

	if (qp1->FileName==qp2->FileName) {
		for (int i=0; i<MAX_IN_GRP; i++) {
			if (qp1->group.stk[i] == qp2->group.stk[i]) continue;
			if 		(qp1->group.stk[i] == 0) ret =  1;
			else if (qp2->group.stk[i] == 0) ret = -1;
			else ret = (qp1->group.stk[i] - qp2->group.stk[i]);
			break;
		}

		if (ret == 0) {
			if (qp1->style == qp2->style) {
				if (qp1->TextStr == qp2->TextStr) {
					if (qp1->CreTime == qp2->CreTime) {
						if (qp1->ModTime == qp2->ModTime) {
							if (qp1->LinkName == qp2->LinkName) {
								ret = (qp1->ID - qp2->ID);
							}
							else {
								if		(qp1->LinkName.IsEmpty()) ret =  1;
								else if (qp2->LinkName.IsEmpty()) ret = -1;
								else ret = CompareStr(qp1->LinkName, qp2->LinkName);
							}
						}
						else {
							ret = -1 * (int)(qp1->ModTime - qp2->ModTime);
						}
					}
					else {
						ret = -1 * (int)(qp1->CreTime - qp2->CreTime);
					}
				}
				else {
					if		(qp1->TextStr.IsEmpty()) ret =  1;
					else if (qp2->TextStr.IsEmpty()) ret = -1;
					else ret = CompareStr(qp1->TextStr, qp2->TextStr);
				}

			}
			else {
				if		(qp1->style == fgsPlate)  ret = -1;
				else if (qp2->style == fgsPlate)  ret =  1;
				else if (qp1->style == fgsNormal) ret = -1;
				else if (qp2->style == fgsNormal) ret =  1;
				else if (qp1->style == fgsImage)  ret = -1;
				else if (qp2->style == fgsImage)  ret =  1;
				else							  ret =  0;
			}
		}
	}
	else {
		ret = CompareText(qp1->FileName, qp2->FileName);
	}

	return ret ;
}

//===========================================================================
// TFindExForm �N���X
//===========================================================================
__fastcall TFindExForm::TFindExForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::FormCreate(TObject *Sender)
{
	QList = new TQFragList();
	DataFileList = new TStringList();

	ProgressBar1 = new TProgressBar(StatusBar1);
	ProgressBar1->Parent = StatusBar1;

	FindModeComboBox->Items->Add("�����ꂩ���܂�");
	FindModeComboBox->Items->Add("�S�Ă��܂�");
	FindModeComboBox->Items->Add("�����ꂩ���܂܂Ȃ�");
	FindModeComboBox->Items->Add("�S�Ă��܂܂Ȃ�");
	FindModeComboBox->Items->Add("���K�\��");

	ObjModeComboBox->Items->Add("�J���Ă���t�@�C��");
	ObjModeComboBox->Items->Add("�t�H���_���̃t�@�C��");
	ObjModeComboBox->Items->Add("�t�H���_���̃t�@�C��");

	ExeModeComboBox->Items->Add("�V�K����");
	ExeModeComboBox->Items->Add("�ǉ�����");
	ExeModeComboBox->Items->Add("�i�荞��");
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::FormShow(TObject *Sender)
{
	UnicodeString sct = "FindEx";

	//�ʒu�E�T�C�Y�𕜌�
	EV->load_pos_info(sct, (TForm*)this,
		(Screen->Width - 600)/2, (Screen->Height - 400)/2, 600, 400);

	StatusBar1->Font->Assign(EV->stbarFont);
	StatusBar1->Canvas->Font->Assign(EV->stbarFont);
	StatusBar1->ClientHeight = StatusBar1->Canvas->TextHeight(" ") + 4;

	ProgressBar1->Top	  = 2;
	ProgressBar1->Left	  = StatusBar1->Panels->Items[0]->Width + 2;
	ProgressBar1->Height  = StatusBar1->ClientHeight -2;
	ProgressBar1->Width   = StatusBar1->Panels->Items[1]->Width -2;
	ProgressBar1->Min	  = 0;
	ProgressBar1->Max	  = 100;
	ProgressBar1->Visible = false;

	TStringList *lp = EV->FindHistoryList;
	if (lp->Count>0) {
		FindComboBox->Items->Assign(lp);
		FindComboBox->Text = lp->Strings[0];
	}
	else {
		FindComboBox->Text = EmptyStr;
	}
	FindComboBox->SetFocus();

	//�������[�h��ݒ�
	FindModeComboBox->ItemIndex = IniFile->ReadInteger(sct, "FindMode", 0);

	switch (FindModeComboBox->ItemIndex) {
	case 0:	EV->FindAndSw = false; EV->FindNotSw = false; EV->FindRegSw = false; break;
	case 1:	EV->FindAndSw = true;  EV->FindNotSw = false; EV->FindRegSw = false; break;
	case 2:	EV->FindAndSw = false; EV->FindNotSw = true;  EV->FindRegSw = false; break;
	case 3:	EV->FindAndSw = true;  EV->FindNotSw = true;  EV->FindRegSw = false; break;
	case 4: EV->FindAndSw = false; EV->FindNotSw = false; EV->FindRegSw = true;  break;
	}

	CaseChkBox->Checked = IniFile->ReadBool(sct, "CaseSw", false);
	EV->FindCaseSw = CaseChkBox->Checked;

	//���s���[�h(=�V�K����)
	ObjModeComboBox->ItemIndex	= IniFile->ReadInteger(sct, "ObjMode", 0);
	ExeModeComboBox->ItemIndex	= 0;

	//�t�H���_������ݒ�
	ObjDirComboBox->Clear();
	for (int i=0; i<MAX_FILEHISTORY; i++) {
		UnicodeString tmpstr = IniFile->ReadString(sct, "ObjDir" + IntToStr(i));
		if (tmpstr.IsEmpty()) break;
		ObjDirComboBox->Items->Add(tmpstr);
	}
	if (ObjDirComboBox->Items->Count>0) ObjDirComboBox->ItemIndex = 0;

	//�ꗗ�̃J��������ݒ�
	TStringGrid *gp = ResultGrid;
	gp->Color = EV->col_xlstBG;
	gp->Font->Assign(EV->xlstFont);
	gp->Font->Color = EV->col_xlstFG;
	gp->DefaultRowHeight = get_FontHeightP(EV->xlstFont, EV->XLstInterLine + 12);

	gp->ColCount = 8;
	gp->RowCount = 2;
	str_to_GridHeadder(gp, "�t�@�C��|�O���[�v|���|�e�L�X�g|�쐬����|�X�V����|�����N��|ID");
	UnicodeString key = "ColWidth";
	gp->ColWidths[0] = IniFile->ReadInteger(sct, key + "0", 100);
	gp->ColWidths[1] = IniFile->ReadInteger(sct, key + "1",  70);
	gp->ColWidths[2] = IniFile->ReadInteger(sct, key + "2",  50);
	gp->ColWidths[3] = IniFile->ReadInteger(sct, key + "3", 200);
	gp->ColWidths[4] = IniFile->ReadInteger(sct, key + "4", 100);
	gp->ColWidths[5] = IniFile->ReadInteger(sct, key + "5", 100);
	gp->ColWidths[6] = IniFile->ReadInteger(sct, key + "6", 120);
	gp->ColWidths[7] = IniFile->ReadInteger(sct, key + "7",  50);

	for (int i=0; i<gp->ColCount; i++) gp->Cells[i][1] = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	UnicodeString sct = "FindEx";
	//�ʒu�E�T�C�Y��ۑ�
	EV->save_pos_info(sct, (TForm*)this);
	//�ꗗ�̃J��������ۑ�
	for (int i=0; i<ResultGrid->ColCount; i++)
		IniFile->WriteInteger(sct, "ColWidth" + IntToStr(i), ResultGrid->ColWidths[i]);
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::FormDestroy(TObject *Sender)
{
	for (int i=0; i<QList->Count; i++) delete QList->Items[i];
	delete QList;

	delete DataFileList;
}

//---------------------------------------------------------------------------
void __fastcall TFindExForm::HideBtnClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
//�t�H���_�̎Q��
//---------------------------------------------------------------------------
void __fastcall TFindExForm::RefDirBtnClick(TObject *Sender)
{
	UnicodeString dirstr = ObjDirComboBox->Text;
	if (SelectDirectory("�����Ώۂ̃t�H���_", EmptyStr, dirstr, TSelectDirExtOpts() << sdNewUI, this))
		ObjDirComboBox->Text = dirstr;
}

//---------------------------------------------------------------------------
//���ʃ��X�g����T�[�`
//---------------------------------------------------------------------------
int __fastcall TFindExForm::QIndexOf(int id, UnicodeString fnam)
{
	int idx = -1;
	for (int i=0; i<QList->Count; i++) {
		q_fragment *qp = QList->Items[i];
		if (id != qp->ID) continue;
		if (!SameText(fnam, qp->FileName)) continue;
		idx = i; break;
	}
	return idx;
}

//---------------------------------------------------------------------------
//�I�����ڂ��J�����g�E�B���h�E�̑I����Ԃƈ�v���Ă��邩
//---------------------------------------------------------------------------
bool __fastcall TFindExForm::CheckListAndWnd()
{
	bool ret = false;
	do {
		int idx = ResultGrid->Row;	if (idx<1) break;
		idx--;  if (idx>=QList->Count) break;
		q_fragment *qp = QList->Items[idx];
		FragSet *fsp = IdeaFragMainForm->CurFS;  if (!fsp) break;
		if (!SameText(fsp->file_name, qp->FileName)) break;
		Fragment *fp = fsp->FrgList->id_to_fp(qp->ID);
		if (!fp) break;
		if (fp!=fsp->get_sel_single()) break;
		ret = true;
	} while (0);
	return ret;
}

//---------------------------------------------------------------------------
//���X�g�̍s���e���X�V
//---------------------------------------------------------------------------
void __fastcall TFindExForm::UpdateListRow(int ridx)
{
	if (ridx>=QList->Count) return;

	q_fragment *qp = QList->Items[ridx];
	int obj_mod = ObjModeComboBox->ItemIndex;
	UnicodeString odir = ObjDirComboBox->Text;

	UnicodeString tmpstr;
	if (!qp->DataTitle.IsEmpty()) {
		tmpstr.sprintf(_T("[%s]"), qp->DataTitle.c_str());
	}
	else {
		tmpstr = qp->FileName;
		tmpstr = ChangeFileExt(tmpstr, EmptyStr);
		if (obj_mod==1 || obj_mod==2) {
			if (StartsText(odir, tmpstr)) {
				tmpstr.Delete(1, odir.Length());
				if (tmpstr[1]=='\\') tmpstr.Delete(1, 1);
			}
		}
		tmpstr = ReplaceStr(tmpstr, "\\", "/");
	}

	TStringGrid *gp = ResultGrid;
	int rn = ridx + 1;
	gp->Cells[0][rn] = tmpstr;

	tmpstr = EmptyStr;
	for (int j=0; j<MAX_IN_GRP; j++) {
		if (qp->group.stk[j]==-1) break;
		if (j>0) tmpstr.UCAT_TSTR(",");
		if (qp->group.stk[j] == 0)
			tmpstr.UCAT_TSTR("������");
		else
			tmpstr.cat_sprintf(_T("%d"), qp->group.stk[j]);
	}
	gp->Cells[1][rn] = tmpstr;

	switch (qp->style) {
	case fgsPlate:	tmpstr = "�\�D"; break;
	case fgsSymbol:	tmpstr = "�W��"; break;
	case fgsImage:	tmpstr = "�摜"; break;
	default:		tmpstr = "�f��"; if (!qp->LinkName.IsEmpty()) tmpstr.UCAT_TSTR(" ��");
	}
	gp->Cells[2][rn] = tmpstr;

	gp->Cells[3][rn] = qp->TextStr;

	DateTimeToString(tmpstr, "yy/mm/dd hh:nn", qp->CreTime);
	gp->Cells[4][rn] = tmpstr;
	DateTimeToString(tmpstr, "yy/mm/dd hh:nn", qp->ModTime);
	gp->Cells[5][rn] = tmpstr;

	gp->Cells[6][rn] = qp->LinkName;
	gp->Cells[7][rn] = IntToStr(qp->ID);
}

//---------------------------------------------------------------------------
//�����J�n
//---------------------------------------------------------------------------
void __fastcall TFindExForm::StartActionExecute(TObject *Sender)
{
	Screen->Cursor = crHourGlass;
	ProgressBar1->Position = 0;
	ProgressBar1->Visible  = true;

	//���s���[�h
	int exe_mod = ExeModeComboBox->ItemIndex;
	if (exe_mod==0) ClrListAction->Execute();

	//������
	UnicodeString wd = trim_ex(FindComboBox->Text);
	TStringList *lp = EV->FindHistoryList;
	int idx = lp->IndexOf(wd);
	if (idx!=0) {
		if (idx>0) lp->Delete(idx);
		lp->Insert(0, wd);	//�����̐擪�ɒǉ�
		FindComboBox->Items->Assign(lp);
	}
	FindComboBox->Text = wd;
	EV->FindWord = wd;

	//�������[�h
	UnicodeString sct = "FindEx";
	IniFile->WriteInteger(sct, "FindMode",	FindModeComboBox->ItemIndex);
	switch (FindModeComboBox->ItemIndex) {
	case 0:	EV->FindAndSw = false; EV->FindNotSw = false; EV->FindRegSw = false; break;
	case 1:	EV->FindAndSw = true;  EV->FindNotSw = false; EV->FindRegSw = false; break;
	case 2:	EV->FindAndSw = false; EV->FindNotSw = true;  EV->FindRegSw = false; break;
	case 3:	EV->FindAndSw = true;  EV->FindNotSw = true;  EV->FindRegSw = false; break;
	case 4: EV->FindAndSw = false; EV->FindNotSw = false; EV->FindRegSw = true;  break;
	}

	EV->FindCaseSw = CaseChkBox->Checked;
	IniFile->WriteBool(sct, "CaseSw", CaseChkBox->Checked);

	//�����Ώ�
	int obj_mod = ObjModeComboBox->ItemIndex;
	IniFile->WriteInteger(sct, "ObjMode", obj_mod);
	UnicodeString odir = ObjDirComboBox->Text;

	//�i�荞��
	if (exe_mod==2) {
		bool found;
		int i = 0;
		while (i<QList->Count) {
			q_fragment *qp = QList->Items[i];
			if (EV->FindRegSw) {
				TRegExOptions opt;
				if (!EV->FindCaseSw) opt << roIgnoreCase;
				found = TRegEx::IsMatch(qp->TextStr, wd, opt);
			}
			else {
				found = find_mlt(wd, qp->TextStr, EV->FindAndSw, EV->FindNotSw, EV->FindCaseSw);
			}

			if (!found) {
				delete qp;
				QList->Delete(i);
			}
			else {
				i++;
			}

			if (QList->Count>0) ProgressBar1->Position = (i + 1)*ProgressBar1->Max/QList->Count;
		}
		StatusBar1->Panels->Items[0]->Text = IntToStr(QList->Count) + "����";
	}
	//�J���Ă���S�ẴE�B���h�E������
	else if (obj_mod==0) {
		for (int cn=0; cn<IdeaFragMainForm->MDIChildCount; cn++) {
			TMDIChild *cp = (TMDIChild*)IdeaFragMainForm->MDIChildren[cn];
			TFragList *flp = cp->FS->FrgList;
			for (int i=0; i<flp->Count; i++) {
				Fragment *fp = flp->Items[i];
				if (!fp->Visible) continue;
				if (fp->style==fgsJunction) continue;
				if (EV->FindRegSw) {
					TRegExOptions opt;
					if (!EV->FindCaseSw) opt << roIgnoreCase;
					if (!TRegEx::IsMatch(fp->TextStr, wd, opt)) continue;
				}
				else {
					if (!find_mlt(wd, fp->TextStr, EV->FindAndSw, EV->FindNotSw, EV->FindCaseSw)) continue;
				}

				if (exe_mod==1) {
					//�ǉ������̏ꍇ�A�d����������
					if (QIndexOf(fp->ID, cp->FS->file_name)!=-1) continue;
				}
				q_fragment *qp = new q_fragment;
				qp->FileName   = cp->FS->file_name;
				qp->DataTitle  = cp->FS->data_title;
				qp->ID		   = fp->ID;
				qp->group	   = fp->group;
				qp->style	   = fp->style;
				qp->wd		   = fp->wd;
				qp->hi		   = fp->hi;
				qp->colFG	   = fp->colFG;
				qp->colBG	   = fp->colBG;
				qp->TextStr    = fp->TextStr;
				qp->LinkName   = fp->LinkName;
				qp->CreTime    = fp->CreTime;
				qp->ModTime    = fp->ModTime;
				QList->Add(qp);
			}
			ProgressBar1->Position = (cn + 1)*ProgressBar1->Max/IdeaFragMainForm->MDIChildCount;
			StatusBar1->Panels->Items[0]->Text = IntToStr(QList->Count) + "����";
			StatusBar1->Repaint();
		}

	}
	//�t�H���_��/���̃f�[�^�t�@�C��������
	else {
		DataFileList->Clear();
		get_files(odir, "*.IFD", DataFileList, (obj_mod==2));
		//��������
		if (DataFileList->Count>0) {
			for (int i=0; i<DataFileList->Count; i++) {
				UnicodeString fnam = DataFileList->Strings[i];
				search_file(fnam, wd);
				ProgressBar1->Position = (i + 1)*ProgressBar1->Max/DataFileList->Count;
				StatusBar1->Panels->Items[0]->Text = IntToStr(QList->Count) + "����";
				StatusBar1->Panels->Items[2]->Text = fnam;
				StatusBar1->Repaint();
			}
			StatusBar1->Panels->Items[2]->Text = EmptyStr;
			//�t�H���_�������X�V
			idx = ObjDirComboBox->Items->IndexOf(odir);
			if (idx!=0) {
				if (idx>0) ObjDirComboBox->Items->Delete(idx);
				ObjDirComboBox->Items->Insert(0, odir);
				ObjDirComboBox->Text = odir;
			}
		}
		//�����ɂ�������폜
		else {
			idx = ObjDirComboBox->Items->IndexOf(odir);
			if (idx!=0) ObjDirComboBox->Items->Delete(idx);
			ObjDirComboBox->Text = EmptyStr;
		}
		//�t�H���_������ۑ�
		for (int i=0; i<MAX_FILEHISTORY; i++) {
			UnicodeString tmpstr = (i<ObjDirComboBox->Items->Count)?
				ObjDirComboBox->Items->Strings[i] : EmptyStr;
			IniFile->WriteString(sct, "ObjDir" + IntToStr(i), tmpstr);
		}
	}

	ProgressBar1->Position = ProgressBar1->Max;
	Sleep(100);	ProgressBar1->Visible = false;

	//�ꗗ���X�V
	if (QList->Count>0) {
		QList->Sort(QFrgCmp);
		ResultGrid->RowCount = QList->Count + 1;
		for (int i=0; i<QList->Count; i++) UpdateListRow(i);
		ResultGridClick(NULL);
		StatusBar1->Panels->Items[1]->Text = "�������������܂���";
	}
	else {
		StatusBar1->Panels->Items[1]->Text =
			((obj_mod==1 || obj_mod==2) && DataFileList->Count==0)?
				"�f�[�^�t�@�C��������܂���" : "�Y������f�Ђ͌�����܂���";
	}
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::StartActionUpdate(TObject *Sender)
{
	bool en_dir = (ExeModeComboBox->ItemIndex!=2)
					&& (ObjModeComboBox->ItemIndex==1 || ObjModeComboBox->ItemIndex==2);
	bool reg_sw = (FindModeComboBox->ItemIndex==4);
	bool flag = false, reg_ng = false;
	do {
		FindComboBox->Color = clWindow;
		if (FindComboBox->Text.IsEmpty())		break;
		if (en_dir) {
			if (ObjDirComboBox->Text.IsEmpty())	break;
		}
		reg_ng = reg_sw && !chk_RegExPtn(FindComboBox->Text);
		if (ExeModeComboBox->ItemIndex==2 && QList->Count==0) break;
		if (ObjModeComboBox->ItemIndex==0 && IdeaFragMainForm->MDIChildCount==0) break;
		flag = !reg_ng;
	} while (0);

	((TAction*)Sender)->Enabled = flag;
	FindComboBox->Font->Color = reg_ng? clRed : clWindowText;

	ObjDirComboBox->Enabled = en_dir;
	RefDirBtn->Enabled = en_dir;

	SrcCutItem->Enabled   = !FindComboBox->SelText.IsEmpty();
	SrcCopyItem->Enabled  = !FindComboBox->SelText.IsEmpty();
	SrcPasteItem->Enabled = Clipboard()->HasFormat(CF_TEXT);
	RegExpItem->Visible	  = reg_sw;
}

//---------------------------------------------------------------------------
//�t�@�C���ɑ΂��錟��
//---------------------------------------------------------------------------
int __fastcall TFindExForm::search_file(UnicodeString fnam, UnicodeString wd)
{
	std::unique_ptr<TStringList> fbuf(new TStringList());
	std::unique_ptr<TQFragList>  tmpQList(new TQFragList());

	q_fragment *qp;
	int MaxID = -1;
	int cnt   = 0;

	UnicodeString kstr, vstr;
	UnicodeString dt_title;

	fbuf->LoadFromFile(fnam);
	int dmod = DSMODE_NON;
	for (int i=0; i<fbuf->Count; i++) {
		UnicodeString s = fbuf->Strings[i];  if (s.IsEmpty()) continue;
		int l = s.Length();
		//�Z�N�V����
		if (s[1] == '[' && s[l] == ']') {
			UnicodeString tmpstr = s.SubString(2, l - 2);
			dmod = SameText(tmpstr, "General") ? DSMODE_GEN :
				   SameText(tmpstr, "Fragment")? DSMODE_FRG :
				   SameText(tmpstr, "Option")  ? DSMODE_OPT : DSMODE_NON;
		}
		//�f�[�^
		else {
			if (dmod==DSMODE_FRG) {
				if (s[1]=='>') {
					qp = new q_fragment;
					qp->FileName = fnam;
					s.Delete(1, 1);
					qp->style = SameStr(s, "PLT")? fgsPlate  :
								SameStr(s, "SYM")? fgsSymbol :
								SameStr(s, "IMG")? fgsImage  :
								SameStr(s, "JNC")? fgsJunction : fgsNormal;
					tmpQList->Add(qp);
					continue;
				}
				else if (s[1]=='<') {
					continue;
				}
			}

			int p = s.Pos("="); if (p==0) continue;
			kstr = s.SubString(1, p - 1); s.Delete(1, p); vstr = s;
			l = vstr.Length();
			if (l>1) {
				if (vstr[1]=='\"' && vstr[l]=='\"') vstr = vstr.SubString(2, l - 2);
			}

			switch (dmod) {
			case DSMODE_GEN:
				if (SameText(kstr, "MaxID")) MaxID = vstr.ToIntDef(EV->DefScrWd);
				break;

			case DSMODE_FRG:	//�f��
				if (SameText(kstr, "ID")) {
					qp->ID = vstr.ToIntDef(-1);
					if (qp->ID>MaxID) MaxID = qp->ID;
				}
				else if (SameText(kstr, "TX")) {
					qp->TextStr = ReplaceStr(vstr, "\"\"", "\"");
				}
				else if (SameText(kstr, "GP")) {
					TStringDynArray ibuf = get_csv_array(vstr, MAX_IN_GRP);
					for (int j=0; j<MAX_IN_GRP; j++)
						qp->group.stk[j] = (j<ibuf.Length)? ibuf[j].ToIntDef(0) : -1;
				}
				else if (SameText(kstr, "WD")) {
					int w = vstr.ToIntDef(EV->def_wd);
					if (w<1 || w>MAX_FRG_WD) w = EV->def_wd;
					qp->wd = w;
				}
				else if (SameText(kstr, "HI")) {
					int h = vstr.ToIntDef(EV->def_hi);
					if (h<1 || h>MAX_FRG_HI) h = EV->def_hi;
					qp->hi = h;
				}
				else if (SameText(kstr, "FG")) {
					TColor c = (TColor)(vstr.ToIntDef(-1));
					switch (qp->style) {
					case fgsPlate:
						qp->colFG = (c==-1)? EV->col_namFG : c; break;
					default:
						qp->colFG = (c==-1)? EV->col_defFG : c;
					}
				}
				else if (SameText(kstr, "BG")) {
					TColor c = (TColor)(vstr.ToIntDef(-1));
					switch (qp->style) {
					case fgsPlate:
						qp->colBG = (c==-1)? EV->col_namBG : c; break;
					default:
						qp->colBG = (c==-1)? EV->col_defBG : c;
					}
				}
				else if (SameText(kstr, "LK")) {
					qp->LinkName = vstr;
				}
				else if (SameText(kstr, "CT")) {
					qp->CreTime = StrToDateTime(vstr);
				}
				else if (SameText(kstr, "MT")) {
					qp->ModTime = StrToDateTime(vstr);
				}
				break;

			case DSMODE_OPT:	//�I�v�V����
				if (SameText(kstr, "DataTitle"))  dt_title = vstr;
				break;
			}
		}
	}

	for (int i=0; i<tmpQList->Count; i++) {
		q_fragment *xqp = tmpQList->Items[i];
		if (xqp->ID==-1) xqp->ID = ++MaxID;
	}

	//����
	int i = 0;
	while (i<tmpQList->Count) {
		q_fragment *xqp = tmpQList->Items[i];
		bool found;
		if (xqp->style==fgsJunction) {
			found = false;
		}
		else if (EV->FindRegSw) {
			TRegExOptions opt;
			if (!EV->FindCaseSw) opt << roIgnoreCase;
			found = TRegEx::IsMatch(xqp->TextStr, wd, opt);
		}
		else {
			found = find_mlt(wd, xqp->TextStr, EV->FindAndSw, EV->FindNotSw, EV->FindCaseSw);
		}

		if (found) {
			if (ExeModeComboBox->ItemIndex==1) {
				//�ǉ������̏ꍇ�A�d����������
				if (QIndexOf(xqp->ID, fnam)!=-1) {
					delete xqp;
					tmpQList->Delete(i);
					continue;
				}
			}
			xqp->DataTitle = dt_title;
			QList->Add(xqp);
			cnt++; i++;
		}
		else {
			delete xqp;
			tmpQList->Delete(i);
		}
	}

	return cnt;
}

//---------------------------------------------------------------------------
//�Z���̕`��
//---------------------------------------------------------------------------
void __fastcall TFindExForm::ResultGridDrawCell(TObject *Sender, int ACol,
	int ARow, TRect &Rect, TGridDrawState State)
{
	TCanvas *cv = ResultGrid->Canvas;
	cv->Font->Assign(EV->xlstFont);
	UnicodeString cellstr = ResultGrid->Cells[ACol][ARow];
	int x = Rect.Left + 4;
	int y = Rect.Top + (Rect.Height() - get_FontHeight(cv))/2;

	//�w�b�_
	if (ARow==0) {
		cv->Brush->Color = clBtnFace;
		cv->Font->Color  = clBtnText;
		cv->FillRect(Rect);
		cv->TextRect(Rect, x, y, cellstr);
	}
	//�Z��
	else {
		if (State.Contains(gdSelected)) {
			cv->Brush->Color = clHighlight;
			cv->Font->Color  = clHighlightText;
		}
		else {
			cv->Brush->Color = EV->col_xlstBG;
			cv->Font->Color  = EV->col_xlstFG;
		}
		cv->FillRect(Rect);

		//�e�L�X�g
		if (ACol==3) {
			int txt_len = cellstr.Length();
			std::unique_ptr<TColor[]> FgCol(new TColor[txt_len + 2]);
			std::unique_ptr<TColor[]> BgCol(new TColor[txt_len + 2]);
			for (int i=0; i<=txt_len; i++) {
				FgCol[i] = cv->Font->Color;
				BgCol[i] = cv->Brush->Color;
			}

			if (!cellstr.IsEmpty()) {
				int p1 = -1, p2 = -1;	//�ŏ��̃}�b�`�ʒu, + ��̒���

				//���K�\��
				if (EV->FindRegSw) {
					TRegExOptions opt;
					if (!EV->FindCaseSw) opt << roIgnoreCase;
					TMatchCollection mts = TRegEx::Matches(cellstr, EV->FindWord, opt);
					for (int i=0; i<mts.Count; i++) {
						int p_i = mts.Item[i].Index;
						int l_i = mts.Item[i].Length;
						for (int j=0,p=p_i; j<l_i; j++,p++) {
							FgCol[p] = cv->Brush->Color;
							BgCol[p] = cv->Font->Color;
						}

						p1 = (p1==-1)? p_i : std::min(p_i, p1);
						if (p1==p_i) p2 = p1 + l_i;
					}
				}
				//�ʏ팟��
				else if (!EV->FindNotSw) {
					std::unique_ptr<TStringList> wlst(new TStringList());
					get_find_wd_list(EV->FindWord, wlst.get());
					int ofs = 1;
					for (;;) {
						int p_e = 0;	//����������̖����ʒu + 1 �̍Ō��
						for (int i=0; i<wlst->Count; i++) {
							UnicodeString kwd = wlst->Strings[i];
							int p = EV->FindCaseSw? PosEx(kwd, cellstr, ofs) : PosEx(kwd.UpperCase(), cellstr.UpperCase(), ofs);
							if (p>0) {
								int len = kwd.Length();
								for (int j=0,k=p; j<len; j++,k++) {
									FgCol[k] = EV->col_selBG;
									BgCol[k] = EV->col_selFG;
								}
								p_e = std::max(p + len, p_e);

								p1 = (p1==-1)? p : std::min(p, p1);
								if (p1==p) p2 = p1 + len;
							}
						}
						if (p_e==0) break;
						ofs = p_e;
					}
				}

				//�}�b�`�ꂪ������悤�ɑO�������ȗ�
				int wd = Rect.Right - x;
				int p_top = 1;
				if (p2>0 && (cv->TextWidth(cellstr.SubString(1, p2 + 1)) > wd)) {
					int w1 = wd / 2;
					int w_r = cv->TextWidth(cellstr.SubString(p1, cellstr.Length() - p1 + 1));
					if (w_r<w1) w1 += (w1 - w_r);
					w1 -= cv->TextWidth("�c ");
					int w = 0;
					for (int i=p1; i>4; i--) {
						w += cv->TextWidth(cellstr.SubString(i, 1));
						if (w<w1) continue;
						cellstr.Delete(1, i);
						cellstr.Insert("�c", 1);
						p_top = i;
						break;
					}
				}

				//�Z���Ɏ��܂�悤�ɖ������ȗ�
				if (cv->TextWidth(cellstr)>wd) {
					cellstr += "�c";
					int p = cellstr.Length() - 1;
					while (p>0) {
						cellstr.Delete(p--, 1);
						if (cv->TextWidth(cellstr)<=wd) break;
					}
				}

				cv->Font->Color  = FgCol[p_top];
				cv->Brush->Color = BgCol[p_top];
				color_textout(cv, x, y, cellstr, &FgCol[p_top], &BgCol[p_top]);
			}
		}
		//���̑�
		else {
			cv->TextRect(Rect, x, y, cellstr);
		}
	}
}

//---------------------------------------------------------------------------
//�N���b�N���ꂽ���ڂ̒f�Ђ��J���Ē�����
//---------------------------------------------------------------------------
void __fastcall TFindExForm::ResultGridClick(TObject *Sender)
{
	int idx = ResultGrid->Row;	if (idx<1) return;
	idx--;
	if (idx<QList->Count) {
		q_fragment *qp = QList->Items[idx];
		StatusBar1->Panels->Items[2]->Text
			= ReplaceStr(qp->FileName, "\\", "/") + "#" + IntToStr(qp->ID);

		TMDIChild *cp = IdeaFragMainForm->CreateMDIChild(qp->FileName);
		if (cp) {
			int fidx = cp->FS->FrgList->id_to_idx(qp->ID);
			if (fidx!=-1) {
				Fragment *fp = cp->FS->FrgList->Items[fidx];
				fp->Matched = true;
				cp->FS->one_select_center(fp);
				SetFocus();
			}
		}
	}
}
//---------------------------------------------------------------------------
//�_�u���N���b�N���ꂽ���ڂ̒f�Ђɑ΂��鏈��
//---------------------------------------------------------------------------
void __fastcall TFindExForm::ResultGridDblClick(TObject *Sender)
{
	int idx = ResultGrid->Row;	if (idx<1) return;
	idx--;
	if (idx<QList->Count) {
		FragSet *fsp = IdeaFragMainForm->CurFS;
		if (fsp && CheckListAndWnd()) {
			//�W�����v
			if (is_KeyDown(VK_SHIFT))
				fsp->jump_link(fsp->get_sel_single());
			//�v���p�e�B
			else
				PropertyAction->Execute();
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFindExForm::ListActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (QList->Count>0);
}

//---------------------------------------------------------------------------
//�I�����ڂ��J�����g�f�[�^�ƈ�v���Ă��邩
//---------------------------------------------------------------------------
void __fastcall TFindExForm::ListSelOneUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = CheckListAndWnd();
}

//---------------------------------------------------------------------------
//���X�g�̒f�Ђ��R�s�[
//�@���\�D�͖����A�O���[�v�͖�������
//---------------------------------------------------------------------------
void __fastcall TFindExForm::CopyListActionExecute(TObject *Sender)
{
	if (QList->Count==0) return;

	UnicodeString tmpstr;
	FSbuf->del_all();
	for (int i=0; i<QList->Count; i++) {
		q_fragment *qp = QList->Items[i];
		if (qp->style==fgsPlate) continue;
		Fragment *dp = FSbuf->new_frag(false);
		dp->style	 = qp->style;
		dp->wd		 = qp->wd;
		dp->hi		 = qp->hi;
		dp->colFG	 = qp->colFG;
		dp->colBG	 = qp->colBG;
		dp->TextStr  = qp->TextStr;
		dp->LinkName = qp->LinkName;
		dp->CreTime  = qp->CreTime;
		dp->ModTime  = qp->ModTime;
		dp->RC		 = Rect(-1, -1, -1, -1);
		dp->Visible  = true;
		if (dp->style==fgsNormal) tmpstr += (dp->TextStr + "\n");
	}

	if (!tmpstr.IsEmpty()) Clipboard()->AsText = tmpstr;
}
//---------------------------------------------------------------------------
//���X�g���N���A
//---------------------------------------------------------------------------
void __fastcall TFindExForm::ClrListActionExecute(TObject *Sender)
{
	for (int i=0; i<QList->Count; i++) delete QList->Items[i];
	QList->Clear();
	ResultGrid->RowCount = 2;
	for (int i=0; i<ResultGrid->ColCount; i++) ResultGrid->Cells[i][1] = EmptyStr;
	ResultGrid->Repaint();
	ClearStatusMsg(NULL);
}

//---------------------------------------------------------------------------
//�t�@�C����#ID���R�s�[
//---------------------------------------------------------------------------
void __fastcall TFindExForm::CopyAnchorActionExecute(TObject *Sender)
{
	int idx = ResultGrid->Row;	if (idx<1) return;
	idx--;
	if (idx<QList->Count) {
		q_fragment *qp = QList->Items[idx];
		Clipboard()->AsText = qp->FileName + "#" + IntToStr(qp->ID);
	}
}
//---------------------------------------------------------------------------
//�v���p�e�B
//---------------------------------------------------------------------------
void __fastcall TFindExForm::PropertyActionExecute(TObject *Sender)
{
	IdeaFragMainForm->EditProperty->Execute();

	do {
		int idx = ResultGrid->Row;	if (idx<1) break;
		idx--;  if (idx>=QList->Count) break;
		q_fragment *qp = QList->Items[idx];
		FragSet *fsp = IdeaFragMainForm->CurFS;  if (!fsp) break;
		if (!SameText(fsp->file_name, qp->FileName)) break;
		Fragment *fp = fsp->FrgList->id_to_fp(qp->ID);
		if (!fp) break;
		qp->group	 = fp->group;
		qp->style	 = fp->style;
		qp->wd		 = fp->wd;
		qp->hi		 = fp->hi;
		qp->colFG	 = fp->colFG;
		qp->colBG	 = fp->colBG;
		qp->TextStr  = fp->TextStr;
		qp->LinkName = fp->LinkName;
		qp->CreTime  = fp->CreTime;
		qp->ModTime  = fp->ModTime;
		UpdateListRow(idx);
	} while (0);
}
//---------------------------------------------------------------------------
//�W�����v
//---------------------------------------------------------------------------
void __fastcall TFindExForm::JumpActionExecute(TObject *Sender)
{
	FragSet *fsp = IdeaFragMainForm->CurFS;
	if (fsp && CheckListAndWnd()) fsp->jump_sel_link();
}

//---------------------------------------------------------------------------
void __fastcall TFindExForm::ClearStatusMsg(TObject *Sender)
{
	StatusBar1->Panels->Items[0]->Text = EmptyStr;
	StatusBar1->Panels->Items[1]->Text = EmptyStr;
	StatusBar1->Panels->Items[2]->Text = EmptyStr;
}

//---------------------------------------------------------------------------
void __fastcall TFindExForm::ResultGridKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	FragSet *fsp = IdeaFragMainForm->CurFS;
	if (fsp && CheckListAndWnd()) {
		switch (Key) {
		case VK_RETURN:
			//�W�����v
			if (is_KeyDown(VK_SHIFT))
				fsp->jump_link(fsp->get_sel_single());
			//�v���p�e�B
			else if (is_KeyDown(VK_CONTROL))
				PropertyAction->Execute();
			break;
		}
	}
}

//---------------------------------------------------------------------------
//�����ꗓ�̕ҏW
//---------------------------------------------------------------------------
void __fastcall TFindExForm::SrcCutItemClick(TObject *Sender)
{
	FindComboBox->Perform(WM_CUT, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::SrcCopyItemClick(TObject *Sender)
{
	FindComboBox->Perform(WM_COPY, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::SrcPasteItemClick(TObject *Sender)
{
	FindComboBox->Perform(WM_PASTE, 0, (NativeInt)0);
}

//---------------------------------------------------------------------------
//�����������\�[�g
//---------------------------------------------------------------------------
void __fastcall TFindExForm::SortFindHistActionExecute(TObject *Sender)
{
	EV->FindHistoryList->Sort();
	FindComboBox->Items->Assign(EV->FindHistoryList);
	FindComboBox->Text = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::SortFindHistActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (FindComboBox->Items->Count>2);
}
//---------------------------------------------------------------------------
//������𗚗�����폜
//---------------------------------------------------------------------------
void __fastcall TFindExForm::DelFromHistActionExecute(TObject *Sender)
{
	UnicodeString swd = FindComboBox->Text.Trim();
	if (!swd.IsEmpty()) {
		int idx = EV->FindHistoryList->IndexOf(swd);
		if (idx!=-1) EV->FindHistoryList->Delete(idx);
		FindComboBox->Items->Assign(EV->FindHistoryList);
		FindComboBox->Text = EmptyStr;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::DelFromHistActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !FindComboBox->Text.Trim().IsEmpty();
}
//---------------------------------------------------------------------------
//�������������ׂč폜
//---------------------------------------------------------------------------
void __fastcall TFindExForm::ClrFindHistActionExecute(TObject *Sender)
{
	if (msgbox_Y_N("�������������ׂč폜���Ă������ł����H")) {
		EV->FindHistoryList->Clear();
		FindComboBox->Clear();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindExForm::ClrFindHistActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (FindComboBox->Items->Count>0);
}

//---------------------------------------------------------------------------
//���K�\���̎Q��
//---------------------------------------------------------------------------
void __fastcall TFindExForm::RefRegItemClick(TObject *Sender)
{
	Clipboard()->AsText = get_tkn(((TMenuItem*)Sender)->Caption, " ");
	FindComboBox->Perform(WM_PASTE, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------

