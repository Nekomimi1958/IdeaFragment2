//----------------------------------------------------------------------//
//	�f�Ђ̃N���X														//
//																		//
//----------------------------------------------------------------------//
#ifndef FragmentH
#define FragmentH
//---------------------------------------------------------------------------
#define MAX_FRG_WD	 40		//�f�Ђ̍ő啶����
#define MAX_FRG_HI	 50		//�f�Ђ̍s��

#define MAX_IN_GRP	 6		//�����ł���ő�O���[�v��

//---------------------------------------------------------------------------
enum FragStyle {
	fgsNone, fgsNormal, fgsPlate, fgsSymbol, fgsImage, fgsJunction
};

struct grp_stk {
	int stk[MAX_IN_GRP];	//�����O���[�v�ԍ�
	int levle;				//(���g�p)
};

//---------------------------------------------------------------------------
int get_FragFitHi(TCanvas * cv, UnicodeString s, int wd);

//---------------------------------------------------------------------------
class Fragment {
private:
	Graphics::TBitmap *ImgBuff;	//�\���C���[�W�p�o�b�t�@
	TMetafile *MetaBuff;		//���^�t�@�C���p�o�b�t�@

	TCanvas *cv;				//�f�Ђ�`�悷��Canvas
	bool Initialized;			//������
	bool Erased;

	bool FSelected;
	void __fastcall SetSelected(bool Value)
	{
		if (FSelected != Value) {
			FSelected = Value;
			Changed   = true;
		}
	}

	void out_frag_text(TCanvas *t_cv, TRect tRC, int mgn, int itl);
	bool draw_icon(UnicodeString fnam, TCanvas *cv, int x, int y, bool act = false);

public:
	TForm   *Parent;		//�e�E�B���h�E
	int     wd, hi;			//�����E�s��
	TRect   RC;				//�f�Ђ̎l���̈ʒu
	TRect   vRC;			//��ʏ�ł̎l���̕\���ʒu
	TRect	tmpRC;			//�ʒu�Ҕ�(�L�k���Ŏg�p)
	int     Z;				//Z�I�[�_
	int		img_wd, img_hi;
	bool	Changed;		//�\���C���[�W�v�ύX
	bool	OnGfrm;			//�O���[�v�g��ɃA�^�b�`(�\�D�̂�)
	double	sX, sY;			//���V�v�Z�p�T��

	int  	ID;				//ID(�i�v�Œ�)	!NEW!
	TDateTime CreTime;		//�쐬����		!NEW!
	TDateTime ModTime;		//�ŏI�A�N�Z�X	!NEW!

	TFont *frgFont; 		//�f�Ђ̃t�H���g(��)
	TFont *namFont; 		//�\�D�̃t�H���g(��)
	TFont *symFont; 		//�W���̃t�H���g(��)

	TColor colFG, colBG;	//�����E�w�i�F
	UnicodeString TextStr;	//�e�L�X�g���e
	UnicodeString LinkName;	//�����N��(��΃p�X)

	grp_stk group;			//�����O���[�v
	FragStyle style;		//�X�^�C��
	bool Visible;			//�\����
	bool Jumping;			//�W�����v��
	bool Noticed;			//���ڒ�
	bool Matched;			//�����Ń}�b�`

	__property bool Selected = {read = FSelected, write = SetSelected};	//�I��

	Fragment(TForm *pf);
	~Fragment();

	void upd_link();
	void era_frag();
	void show_frag();
	void print_frag(TCanvas *p_cv, double ratio);
	void view_frag(TCanvas *v_cv, double ratio);
	void assign(Fragment *fp,  bool id_sw = false, bool sel_sw = false);
	int  in_group(int gn);
	int  get_top_group();
	bool set_group(int gn, bool add_sw = false, bool top_sw = false);
	int  rel_group(int gn);
};
//---------------------------------------------------------------------------
#endif
