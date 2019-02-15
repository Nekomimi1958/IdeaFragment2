//----------------------------------------------------------------------//
//	断片リストのクラス													//
//																		//
//----------------------------------------------------------------------//
#ifndef FragListH
#define FragListH

//---------------------------------------------------------------------------
#include "fragment.h"

//---------------------------------------------------------------------------
//断片リスト (TList から継承)
//---------------------------------------------------------------------------
class TFragList : public TList {
private:
	Fragment * __fastcall Get(int Index)
	{
	 	return (Fragment*)(TList::Get(Index));
	}

	void __fastcall Put(int Index, Fragment *Item)
	{
		TList::Put(Index, Item);
	}

public:
	__fastcall TFragList();
	__property Fragment * Items[int Index] = {read=Get, write=Put};

	int  __fastcall id_to_idx(int id);
	Fragment * __fastcall id_to_fp(int id);
	Fragment * __fastcall str_to_fp(UnicodeString s);
	TPoint __fastcall get_min_pos();
	void __fastcall AllShow();
	void __fastcall del_all();
};

//---------------------------------------------------------------------------
int __fastcall FrgCmp_Z(void * Item1, void * Item2);
int __fastcall FrgCmp_Pos(void * Item1, void * Item2);
int __fastcall FrgCmp_PosX(void * Item1, void * Item2);
int __fastcall FrgCmp_PosY(void * Item1, void * Item2);

//---------------------------------------------------------------------------
int __fastcall FrgCmp_GroupEx(void * Item1, void * Item2);
int __fastcall FrgCmp_Style(void * Item1, void * Item2);
int __fastcall FrgCmp_Text(void * Item1, void * Item2);
int __fastcall FrgCmp_CreTime(void * Item1, void * Item2);
int __fastcall FrgCmp_ModTime(void * Item1, void * Item2);
int __fastcall FrgCmp_Link(void * Item1, void * Item2);
int __fastcall FrgCmp_ID(void * Item1, void * Item2);
int __fastcall FrgCmp_Select(void * Item1, void * Item2);
//---------------------------------------------------------------------------
#endif
