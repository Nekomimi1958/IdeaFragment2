//----------------------------------------------------------------------//
// INIファイル処理クラス(UTF8)											//
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
// 全情報を一括で読み込む
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
//キーを削除
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
//セクションを読み込む
//---------------------------------------------------------------------------
void UsrIniFile::ReadSection(UnicodeString sct, TStringList *lst)
{
	if (!lst) return;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) lst->Assign((TStringList*)SectionList->Objects[idx]);
}
//---------------------------------------------------------------------------
//セクションへ複写
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
	UnicodeString def)	//デフォルト値	(default = EmptyStr)
{
	UnicodeString ret;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList * klist = (TStringList*)(SectionList->Objects[idx]);
		ret = klist->Values[key];
		//ダブルクォーテーションで囲まれていたら外す
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
	int def)	//デフォルト値	(default = 0)
{
	return StrToIntDef(ReadString(sct, key, EmptyStr), def);
}
//---------------------------------------------------------------------------
bool UsrIniFile::ReadBool(UnicodeString sct, UnicodeString key,
	bool def)	//デフォルト値	(default = false)
{
	return (ReadInteger(sct, key, (int)def) != 0);
}
//---------------------------------------------------------------------------
TColor UsrIniFile::ReadColor(UnicodeString sct, UnicodeString key,
	TColor def)	//デフォルト値	(default = clBlack)
{
	return (TColor)StrToIntDef(ReadString(sct, key, EmptyStr), (int)def);
}

//---------------------------------------------------------------------------
//フォントを作成し、情報を読み込んで設定
// 作成されたフォントは、最後に破棄すること
//---------------------------------------------------------------------------
TFont* UsrIniFile::ReadFontInf(UnicodeString sct,
	TFont *def)	//デフォルト	(default = NULL)
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
// INIファイルの更新 (Write～で変更されていたら保存)
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
