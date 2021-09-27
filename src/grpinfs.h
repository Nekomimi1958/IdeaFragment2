//----------------------------------------------------------------------//
// �O���[�v���̃N���X													//
//																		//
//----------------------------------------------------------------------//
#ifndef GrpInfsH
#define GrpInfsH
//---------------------------------------------------------------------------
#include "fragment.h"
#include "fraglist.h"
#include "grpframe.h"

//---------------------------------------------------------------------------
#define MAX_GROUP	 100		//�ő�O���[�v��

struct grp_inf {
	TGrpFrame *gfp;		//�O���[�v�g
	TRect RC;			//�O���[�v�g�̐�Έʒu
	TRect cRC;			//�O���[�v�g���̒f�Д͈�
	int  Area;			//�O���[�v�g�̖ʐ�
	TColor col_GrpFrm;	//�O���[�v�g�̌ʐF
	int  LnWidth;		//�O���[�v�g�̌ʐ���
	bool Visible;
	bool Selected;		//�O���[�v���̂ǂꂩ���I������Ă���
	bool AllSelected;	//�O���[�v�S�̂��I������Ă���
	bool RepaintFrm;	//�O���[�v�g�\���X�V�v��
	bool RepaintLn;		//�֌W���\���X�V�v��
	bool Empty;
	int  IncludeGrp;
	int  TotalSize;		//�S�f�А�
	Fragment *Plate;	//�\�D
};

//---------------------------------------------------------------------------
// �O���[�v���
//---------------------------------------------------------------------------
class GrpInfs {
private:
	TCanvas *gcv;

public:
	grp_inf Items[MAX_GROUP + 1];	//�O���[�v��� 0:������/ 1�`:�O���[�v

	int  cur_group; 			//�J�����g�O���[�v
	int  cur_gfrm;

	bool exist_img;				//�摜�L��

	TColor col_GrpFrm;
	TFont *namFont;

	TFragList *FrgList;

	GrpInfs(TForm* AOwner, TFragList *lst);
	~GrpInfs();

	void __fastcall update_empty();
	int  __fastcall get_empty_idx();
	int  __fastcall get_sel_max();
	Fragment * __fastcall get_name_plate(int gn, UnicodeString *name);
	void __fastcall calc_frame(int gn, bool pickuped = false, TRect *crc = NULL);
	void __fastcall update_frame(bool pickuped);
	void __fastcall repaint_frame(bool sw = false);
	int  __fastcall group_from_pos(int X, int Y);
	int  __fastcall gstk_from_pos(int X, int Y, grp_stk *sp);
	int  __fastcall gfrm_from_pos(int X, int Y);
	int  __fastcall compare_group(int gn0, int gn1);
	void __fastcall stretch(double ratio, bool sw);
};
//---------------------------------------------------------------------------
#endif
