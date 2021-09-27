//----------------------------------------------------------------------//
// �R���g���[���̌������}�[�N											//
//																		//
//----------------------------------------------------------------------//
#ifndef MarkListH
#define MarkListH

//---------------------------------------------------------------------------
//�}�[�N���X�g (TList ����p��)
//---------------------------------------------------------------------------
class MarkList : public TList {
private:
	TShape * __fastcall Get(int Index);
	void __fastcall Put(int Index, TShape *Item);

public:
	__fastcall MarkList(Classes::TComponent* AOwner);
	__fastcall ~MarkList();

	void __fastcall ClearAll();
	int  __fastcall SearchControl(TWinControl *ctrl, UnicodeString wd);
	void __fastcall ShowMark();

	TForm *MarkOwner;
	TColor MarkColor;

	__property TShape * Items[int Index] = {read=Get, write=Put};
};
//---------------------------------------------------------------------------
#endif
