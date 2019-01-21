//----------------------------------------------------------------------//
// INI�t�@�C�������N���X(UTF8)											//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "UIniFile.h"

//---------------------------------------------------------------------------
UsrIniFile *IniFile = NULL;

//---------------------------------------------------------------------------
UsrIniFile::UsrIniFile(UnicodeString fnam)
{
	FileName = fnam;
	SectionList = new TStringList();
	LoadValues();
}
//---------------------------------------------------------------------------
UsrIniFile::~UsrIniFile()
{
	Clear();
	delete SectionList;
}

//---------------------------------------------------------------------------
TStringList * UsrIniFile::AddSection(UnicodeString sct)
{
	TStringList *slst = new TStringList();
	SectionList->AddObject(sct, (TObject*)slst);
	return slst;
}
//---------------------------------------------------------------------------
void UsrIniFile::Clear()
{
	for (int i=0; i<SectionList->Count; i++) {
		TStringList *klist = (TStringList*)SectionList->Objects[i];
		delete klist;
	}
	SectionList->Clear();
}

//---------------------------------------------------------------------------
// �S�����ꊇ�œǂݍ���
//---------------------------------------------------------------------------
void UsrIniFile::LoadValues()
{
	if (FileExists(FileName)) {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->LoadFromFile(FileName);
		TStringList *klist = NULL;
		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString s = fbuf->Strings[i];
			if (s.IsEmpty() || s[1]==';') continue;
			int l = s.Length();
			if ((s[1] == '[') && (s[l] == ']'))
				klist = AddSection(s.SubString(2, l - 2));
			else
				if (klist) klist->Add(s);
		}
	}
	else
		Clear();
	Modified = false;
}

//---------------------------------------------------------------------------
//�L�[���폜
//---------------------------------------------------------------------------
void UsrIniFile::DeleteKey(UnicodeString sct, UnicodeString key)
{
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)(SectionList->Objects[idx]);
		idx = klist->IndexOfName(key);
		if (idx>=0) klist->Delete(idx);
	}
}

//---------------------------------------------------------------------------
//�Z�N�V������ǂݍ���
//---------------------------------------------------------------------------
void UsrIniFile::ReadSection(UnicodeString sct, TStringList *lst)
{
	if (!lst) return;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) lst->Assign((TStringList*)SectionList->Objects[idx]);
}
//---------------------------------------------------------------------------
//�Z�N�V�����֕���
//---------------------------------------------------------------------------
void UsrIniFile::AssignSection(UnicodeString sct, TStringList *lst)
{
	if (!lst) return;
	TStringList *klist;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0)
		klist = (TStringList*)SectionList->Objects[idx];
	else
		klist = AddSection(sct);
	klist->Assign(lst);
	Modified = true;
}

//---------------------------------------------------------------------------
UnicodeString UsrIniFile::ReadString(UnicodeString sct, UnicodeString key,
	UnicodeString def)	//�f�t�H���g�l	(default = EmptyStr)
{
	UnicodeString ret;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList * klist = (TStringList*)(SectionList->Objects[idx]);
		ret = klist->Values[key];
		//�_�u���N�H�[�e�[�V�����ň͂܂�Ă�����O��
		int l = ret.Length();
		if (l>1) {
			if ((ret[1]=='\"') && (ret[l]=='\"')) ret = ret.SubString(2, l - 2);
		}
	}
	if (ret.IsEmpty()) ret = def;
	return ret;
}
//---------------------------------------------------------------------------
int UsrIniFile::ReadInteger(UnicodeString sct, UnicodeString key,
	int def)	//�f�t�H���g�l	(default = 0)
{
	return StrToIntDef(ReadString(sct, key, EmptyStr), def);
}
//---------------------------------------------------------------------------
bool UsrIniFile::ReadBool(UnicodeString sct, UnicodeString key,
	bool def)	//�f�t�H���g�l	(default = false)
{
	return (ReadInteger(sct, key, (int)def) != 0);
}
//---------------------------------------------------------------------------
TColor UsrIniFile::ReadColor(UnicodeString sct, UnicodeString key,
	TColor def)	//�f�t�H���g�l	(default = clBlack)
{
	return (TColor)StrToIntDef(ReadString(sct, key, EmptyStr), (int)def);
}

//---------------------------------------------------------------------------
//�t�H���g���쐬���A����ǂݍ���Őݒ�
// �쐬���ꂽ�t�H���g�́A�Ō�ɔj�����邱��
//---------------------------------------------------------------------------
TFont* UsrIniFile::ReadFontInf(UnicodeString sct,
	TFont *def)	//�f�t�H���g	(default = NULL)
{
	TFont *f = new TFont();
	f->Charset = (TFontCharset)ReadInteger(sct, "FontCharset", (int)SHIFTJIS_CHARSET);
	f->Name    = ReadString( sct, "FontName", (def? def->Name : EmptyStr));
	f->Size    = ReadInteger(sct, "FontSize", (def? def->Size : 11));
	TFontStyles fstyle = TFontStyles();
	if (ReadBool(sct, "FontBold", (def? def->Style.Contains(fsBold) : false)))
		fstyle << fsBold;
	if (ReadBool(sct, "FontItalic", (def? def->Style.Contains(fsItalic) : false)))
		fstyle << fsItalic;
	f->Style = fstyle;
	return f;
}

//---------------------------------------------------------------------------
void UsrIniFile::WriteString(UnicodeString sct, UnicodeString key, UnicodeString v)
{
	TStringList *klist;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0)
		klist = (TStringList*)(SectionList->Objects[idx]);
	else
		klist = AddSection(sct);

	UnicodeString s = key + '=' + v;
	idx = klist->IndexOfName(key);
	if (idx>=0) {
		if (CompareStr(klist->Strings[idx], s)!=0) {
			klist->Strings[idx] = s;
			Modified = true;
		}
	}
	else {
		klist->Add(s);
		Modified = true;
	}
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteInteger(UnicodeString sct, UnicodeString key, int v)
{
	WriteString(sct, key, IntToStr(v));
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteBool(UnicodeString sct, UnicodeString key, bool v)
{
	WriteString(sct, key, (v!=0)? "1" : "0");
}

//---------------------------------------------------------------------------
void UsrIniFile::WriteFontInf(UnicodeString sct, TFont *f)
{
	WriteString( sct, "FontName",	f->Name);
	WriteInteger(sct, "FontSize",	f->Size);
	WriteBool(   sct, "FontBold",	f->Style.Contains(fsBold));
	WriteBool(   sct, "FontItalic",	f->Style.Contains(fsItalic));
}


//---------------------------------------------------------------------------
// INI�t�@�C���̍X�V (Write�`�ŕύX����Ă�����ۑ�)
//---------------------------------------------------------------------------
bool UsrIniFile::UpdateFile()
{
	try {
		if (Modified) {
			std::unique_ptr<TStringList> fbuf(new TStringList());
			for (int i=0; i<SectionList->Count; i++) {
				fbuf->Add("[" + SectionList->Strings[i] + "]");
				TStringList *klist = (TStringList*)SectionList->Objects[i];
				for (int j=0; j<klist->Count; j++) fbuf->Add(klist->Strings[j]);
				fbuf->Add("");
			}
			fbuf->SaveToFile(FileName, TEncoding::UTF8);
			Modified = false;
		}
		return true;
	}
	catch (...) {
		return false;
	}
}
//---------------------------------------------------------------------------
