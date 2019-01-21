//----------------------------------------------------------------------//
//	�����̃N���X														//
//																		//
//----------------------------------------------------------------------//
#ifndef arwlineH
#define arwlineH

//---------------------------------------------------------------------------
#include "UserFunc.h"

//---------------------------------------------------------------------------
class ArrowLine {
private:
	bool FVisible;

public:
	int  tType;
	int  fromTag, toTag;

	int  lType;					//���^�C�v
	int  lWidth;				//���̑���	(-1:�_��, 1,3,5:����)
	TColor lColor;				//���̌ʐF (clNone�̏ꍇ����)
	TColor lCurCol;
	int  aType;					//��̃^�C�v
	int	 aSize;					//��̃T�C�Y
	int  x0, y0, x1, y1;		//���[��Έʒu
	int  vx0, vy0, vx1, vy1;	//��ʏ�ł̗��[���Έʒu
	UnicodeString TextStr;		//�e�L�X�g���e
	bool tOpposite;				//�e�L�X�g�ʒu�����Α�

	bool tmpFlag0, tmpFlag1;

	ArrowLine();
	void assign(ArrowLine* ap);
	bool pt_in_line(int X, int Y);
	bool pt_in_frm(int X, int Y);
	bool pt_in_to(int X, int Y);

	void draw(TCanvas *cv, double ratio, TColor lcol, TColor tcol, bool sel_sw = false, bool era_sw = false);
	void erase(TForm *frm);
	void view_line(TCanvas *v_cv, double ratio, TColor col);
	bool set_pos(octrect xrc0, octrect xrc1, int ofsx, int ofsy, bool j0sw = false, bool j1sw = false);
	void reverse();

	__property bool Visible = {read = FVisible}; //, write = FVisible};
};

//(tType);
#define TDIR_NONE		0	//����
#define TDIR_G2G		1	//�O���[�v --> �O���[�v
#define TDIR_F2F		2	//�f��     --> �f��
#define TDIR_F2G		3	//�f��     --> �O���[�v
#define TDIR_G2F		4	//�O���[�v --> �f��

//���^�C�v (lType)
#define LTYPE_LINE		1	//��		----
#define LTYPE_S_ARROW	2	//���		--->
#define LTYPE_D_ARROW	3	//�����	<-->
#define LTYPE_CD_ARROW	4	//�t�����	>--<
#define LTYPE_D_LINE	5	//��d��	====
#define LTYPE_C_ARROW	6	//�t���	---<

//��̌` (aType);
#define ATYPE_LINE		0	//��
#define ATYPE_SOLID		1	//�h��Ԃ�

//��̑傫��(aSize)
#define ASIZE_SMALL		 8	//��
#define ASIZE_MEDIUM	12	//��
#define ASIZE_LARGE		16	//��

#define DLINE_WIDTH		 3	//��d���̊Ԋu


//---------------------------------------------------------------------------
//�������X�g (TList ����p��)
//---------------------------------------------------------------------------
class TArrowList : public TList {
private:
	TForm* frg_owner;

	ArrowLine * __fastcall Get(int Index);
	void __fastcall Put(int Index, ArrowLine *Item);

public:
	__fastcall TArrowList(Classes::TComponent* AOwner);
	ArrowLine *	__fastcall lp_from_pos(int X, int Y);
	ArrowLine *	__fastcall lp_from_id(int tp, int id0, int id1);
	void __fastcall erase_line(int tp = TDIR_NONE);
	void __fastcall del_all();
	void __fastcall new_copy(TArrowList *lst);
	void __fastcall clr_tmp_flag();
	void __fastcall change_frg_id(int id0, int id1);

	__property ArrowLine * Items[int Index] = {read=Get, write=Put};
};

//---------------------------------------------------------------------------
#endif
