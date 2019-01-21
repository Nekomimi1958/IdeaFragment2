//----------------------------------------------------------------------//
// INIファイル処理クラス(UTF8)											//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrIniFileH
#define UsrIniFileH

class UsrIniFile {
private:
	TStringList *AddSection(UnicodeString sct);
	bool Modified;

public:
	UnicodeString FileName;
	TStringList *SectionList;

	UsrIniFile(UnicodeString fnam);
	~UsrIniFile();

	void Clear();
	void LoadValues();
	void DeleteKey(UnicodeString sct, UnicodeString key);
	void ReadSection(UnicodeString sct, TStringList *lst);
	void AssignSection(UnicodeString sct, TStringList *lst);
	UnicodeString ReadString(UnicodeString sct, UnicodeString key, UnicodeString def = EmptyStr);
	int  ReadInteger(UnicodeString sct, UnicodeString key, int def = 0);
	bool ReadBool(UnicodeString sct, UnicodeString key, bool def = false);
	TColor ReadColor(UnicodeString sct, UnicodeString key, TColor def = clBlack);
	TFont* ReadFontInf(UnicodeString sct, TFont *def = NULL);
	void WriteString(UnicodeString sct, UnicodeString key, UnicodeString v = EmptyStr);
	void WriteInteger(UnicodeString sct, UnicodeString key, int v);
	void WriteBool(UnicodeString sct, UnicodeString key, bool v);
	void WriteFontInf(UnicodeString sct, TFont *f);
	bool UpdateFile();

};
//----------------------------------------------------------------------
extern UsrIniFile  *IniFile;

//----------------------------------------------------------------------
#endif
