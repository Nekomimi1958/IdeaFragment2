//----------------------------------------------------------------------//
//	�f�ЃZ�b�g�̃N���X													//
//																		//
//----------------------------------------------------------------------//
#ifndef FragSetH
#define FragSetH

//---------------------------------------------------------------------------
#include "arwline.h"
#include "fragment.h"
#include "fraglist.h"
#include "grpframe.h"
#include "grpinfs.h"
#include "fragbuf.h"

//---------------------------------------------------------------------------
#define MAX_GRP_ITM	  40		//�O���[�v�֌W�̍ő僁�j���[���ڐ�
#define MAX_TEXTLEN	2000		//�ő�e�L�X�g��
#define MAX_UNDO	 100		//�ő�A���h�D��

#define DSMODE_NON	0
#define DSMODE_GEN	1
#define DSMODE_FRG	2
#define DSMODE_CON	3
#define DSMODE_GRP	4
#define DSMODE_OPT	5

//��ʕ������[�h
#define DVMODE_NON	0	//����
#define DVMODE_DVN	1	//�c������(�������w��)
#define DVMODE_PXN	2	//�c������(�s�N�Z���w��)
#define DVMODE_TRH	3	//�O�p�`�^�C��(����)
#define DVMODE_TRV	4	//�O�p�`�^�C��(����)
#define DVMODE_HXH  5	//�Z�p�`�^�C��(����)
#define DVMODE_HXV  6	//�Z�p�`�^�C��(����)

//�֌W�����[�h (CLineMode)
#define CLMODE_NON		0	//����
#define CLMODE_READY	1	//�ҋ@
#define CLMODE_CONCT	2	//�ݒ蒆
#define CLMODE_RECON	3	//�ڑ��ύX��


//---------------------------------------------------------------------------
//�f�ЃZ�b�g�̃N���X
//---------------------------------------------------------------------------
class FragSet {
private:
	int  MaxZ;						//(�ő�)�őO��Z
	int  MaxID;						//�ő�ID

	Fragment *jump_fp;				//�W�����v���f��
	Fragment *find_fp;				//�������Œ��ڒ��̒f��
	Fragment *catched_fp;			//�ړ����ɂ���ł���f��

	bool movskip;					//DblClick��̈ړ������

	FragBuffer *UndoBuf[MAX_UNDO];

	void del_frag(Fragment *fp);
	void reset_notice();
	int  import_core(TStringList *TextBuff, int gn);
	bool erase_sym_frm();
	void visible_update();
	void draw_prn_img(TCanvas *cv, int wd, int hi, int foot_hi);

public:
	TForm* frg_owner;				//�f�ЃZ�b�g�̃I�[�i�[
	UnicodeString file_name;		//�f�Ѓt�@�C����
	UnicodeString data_title;		//�^�C�g��

	TFragList *FrgList; 			//�f�Ђ̃��X�g
	TFragList *SelList; 			//�I�𒆒f�Ђ̃��X�g

	GrpInfs   *GInfs;				//�O���[�v���

	TArrowList *CLineList;			//�֌W�����X�g
	int  CLineMode;					//�֌W�����[�h
	ArrowLine *CurLp;				//�I�𒆊֌W��

	int  scr_wd, scr_hi;			//��ʂ̑S�̃T�C�Y

	int  div_x, div_y;				//��ʕ�����
	int  div_mod;					//��ʕ������[�h
	int  div_lw;					//��ʕ�������

	int  title_pos;					//�^�C�g���̕\���ʒu (1�`9, 0:��\��)

	bool inh_Grid;					//�O���b�h�`��̗}�~

	TControlScrollBar *HscrBar;		//�����X�N���[���o�[
	TControlScrollBar *VscrBar;		//�����X�N���[���o�[

	TColor col_backgr;				//�w�i�F(��)
	TColor col_DivLine;				//�������̐F(��)
	TColor col_GrpFrm;				//�O���[�v�g�̐F(��)
	TColor col_GrpLine;				//�֌W���̐F(��)
	TColor col_FrgLine;
	TColor col_LinLbl;				//�֌W�����x���̐F(��)
	TColor col_Title;				//�^�C�g���F(��)

	TFont *frgFont; 				//�f�Ђ̃t�H���g(��)
	TFont *namFont; 				//�\�D�̃t�H���g(��)
	TFont *symFont; 				//�W���̃t�H���g(��)
	TFont *lblFont; 				//���x���̃t�H���g(��)
	TFont *titFont;					//�^�C�g���̃t�H���g(��)

	bool ParentFrgFont;
	bool ParentNamFont;
	bool ParentSymFont;
	bool ParentLblFont;
	bool ParentTitFont;

	int  LastHPos, LastVPos;		//�O��̈ʒu
	UnicodeString LastAnc;			//�O��J�����Ƃ��̈ʒu

	bool read_only; 				//�ҏW�֎~
	bool forcesave; 				//�m�F�Ȃ��ŋ����ۑ�
	bool modify;					//�ύX���ꂽ
	bool moving;					//�ړ���
	bool moved; 					//�ړ�����
	bool movgroup;					//+Ctrl�L�[�ŃO���[�v�I����Ɉړ���
	bool pickuped;					//�ړ����ɃO���[�v��������グ
	bool floating;					//���V��
	int  notice_pos; 				//�I��f�Г��̒��ڈʒu(0�`�I��-1)

	TPoint lastP;					//�f�Јړ����̑O��̃}�E�X�ʒu

	int UndoN, RedoN, UndoPtr;

	FragSet(TForm* AOwner);
	~FragSet();

	Fragment *new_frag(bool id_sw = true);
	void update_grp_empty();
	void del_sel_frag();
	Fragment *get_sel_single();
	int  mak_sel_list(bool show = false);
	Fragment *get_sel_center();
	TRect  get_sel_union(bool tmp_sw = false);
	TPoint get_sel_org_pos(int mode, bool tmp_sw = false);
	int  get_sel_group();
	int  get_sel_group_ex();
	Fragment *get_linked_frag(UnicodeString fnam);
	int  get_intr_flist(Fragment *fp, TFragList *lst);
	int  get_con_flist(Fragment *fp, TFragList *lst, bool sel_sw = false);
	void sel_connected(Fragment *fp);
	Fragment *frag_from_pos(int X, int Y);

	ArrowLine *set_group_line(int gn0, int gn1);
	ArrowLine *set_frag_line(int id0, int id1);
	ArrowLine *set_fg_line(int tp, int id0, int id1);
	int  has_lines(Fragment *fp);
	Fragment *put_junction(int x, int y);
	bool add_junction();
	void clr_iso_junction();

	void exchange_group(int gn0, int gn1);
	void mov_sel_group(int gn, bool add_sw = false);
	void rel_group(int gn);

	void bring_to_front(Fragment *fp)
	{
		if (fp) fp->Z = ++MaxZ;
	}

	void update_group_frame();
	void update_frg_line();
	void update_fg_line();
	void to_center(Fragment *fp, bool sw, int x = -1, int y = -1);
	void to_anchor(UnicodeString anam);
	void set_new_pos(Fragment *fp, int idx, int x = -1, int y = -1);

	void push_all();
	void pop_all(int idx);
	void undo();
	void redo();

	void set_titlebar();
	int  load_file(UnicodeString fnam, bool mg_sw);
	bool save_file(UnicodeString fnam, bool ts_sw = false);
	int  import_text(UnicodeString fnam);
	int  export_text(UnicodeString fnam);
	int  export_file(UnicodeString fnam, UnicodeString separator);
	int  new_from_clipboard();

	void get_sel_str(UnicodeString* cpybuf);
	void copy_buf(bool cutsw=false);
	void paste_buf(int x = -1, int y = -1);

	void all_select(bool sw = true);
	void rect_select(TRect rc);
	void group_select(int gn, bool sw);
	void bgcol_select(TColor col);
	void trim_select();
	void one_select_center(Fragment *fp);
	void gather_sel(bool sw, int pt = 1);
	void spread_sel();
	void move_sel(int x, int y);
	void stretch_sel(double ratio, int smod);
	void stretch_grp(double ratio, bool sw);
	void rubber_sel(int len, int spc);
	void quick_mov_sel(int mode, double angle = 0.0);
	void draw_divln(TCanvas *cv, int w, int h, double ratio = 1.0);
	void draw_grid();
	void draw_title(TCanvas *cv, double ratio, int w, int h);
	void all_update(bool upd_img = false);
	int  find_str(UnicodeString wd, bool sel_sw);

	void jump_link(Fragment *fp);
	void jump_sel_link();

	void printout();
	bool imageout(UnicodeString fnam);

	//�}�E�X�C�x���g
	void __fastcall FragMouseDown(Fragment *sp, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void __fastcall FragMouseMove(Fragment *sp, TShiftState Shift, int X, int Y);
	void __fastcall FragMouseUp(Fragment *sp, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void __fastcall FragDblClick(Fragment *sp);
};

//---------------------------------------------------------------------------
#endif
