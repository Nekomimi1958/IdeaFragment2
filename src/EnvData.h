//----------------------------------------------------------------------//
//	���ݒ�Ǘ��̃N���X												//
//																		//
//----------------------------------------------------------------------//
#ifndef EnvDataH
#define EnvDataH

//---------------------------------------------------------------------------
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "UIniFile.h"
#include "spiunit.h"

//---------------------------------------------------------------------------
#define SUPPORT_URL		_T("http://nekomimi.la.coocan.jp/")

//---------------------------------------------------------------------------
#define MAX_FRGWIN 40			//�ő�q�E�B���h�E��

#define MAX_FILEHISTORY  20		//�ő�t�@�C������
#define MAX_FINDHISTORY  40		//�ő匟������
#define MAX_BGCOL_ITM    20		//�w�i�F�I���̍ő區�ڐ�
#define MAX_JUMPLIST     20		//�W�����v�������j���[�̍ő區�ڐ�

#define DEF_TRIM_WD		800
#define DEF_TRIM_HI		600
#define DEF_TRIM_MGN	 50

#define FEXT_WICSTD		".bmp.jpg.jpeg.jpe.png.gif.tif.wdp.hdp"		//WIC���W���ŕ\���\

//�ʐݒ�F�̎擾
#define GET_indcol(id)  ((id==Graphics::clNone)? EV->id : id)

int  ScaledInt(int n);
void ResizeImageList(TImageList *i_list);
void InitUpDown(TUpDown *udp, int n);

//---------------------------------------------------------------------------
class EnvData {
public:
	TForm *env_owner;
	bool IsAdmin;				//�Ǘ��Ҍ����������Ă��邩

	//------------------------------
	// �I�v�V����
	//------------------------------
	int def_wd, def_hi;			//�f�Ѓf�t�H���g�T�C�Y
	int nam_wd, nam_hi;			//�\�D�f�t�H���g�T�C�Y
	int img_r1, img_r2;			//�摜�f�t�H���g�{��
	int frg_mgn;				//�f�Ђ̗]��
	int frg_itl;				//�f�Ђ̍s��
	int plt_mgn;				//�g��\�D�̗]��

	int WicScaleOpt;			//�摜�̏k���A���S���Y��

	TColor col_defFG, col_defBG;	//�f�t�H���g
	TColor col_Border;
	TColor col_selFG, col_selBG;	//�I��
	TColor col_namFG, col_namBG;	//�\�D
	TColor col_symFG;				//�W��
	TColor col_LinLbl;				//���x��
	TColor col_Title;				//�^�C�g��
	TColor col_backgr, col_selRct;
	TColor col_glstFG, col_glstBG, col_glstOff;
	TColor col_xlstFG, col_xlstBG;
	TColor col_AreaFrm, col_MapBG;
	TColor col_GrpFrm;
	TColor col_GrpLine, col_FrgLine;
	TColor col_DivLine;
	TColor col_Brush;
	TColor col_PosGrid;
	TColor col_OptFind;			//�ݒ荀�ڌ����̋����\��

	int    col_Custom[16];

	TFont *frgFont; 			//�f�Ђ̃t�H���g
	TFont *namFont; 			//�\�D�̃t�H���g
	TFont *symFont; 			//�W���̃t�H���g
	TFont *lblFont; 			//�֌W�����x���̃t�H���g
	TFont *titFont;				//�^�C�g���̃t�H���g
	TFont *glstFont;			//�O���[�v���X�g�̃t�H���g
	TFont *flstFont;			//�f�Ѓ��X�g�̃t�H���g
	TFont *xlstFont;			//�g���������X�g�̃t�H���g
	TFont *stbarFont;			//�X�e�[�^�X�o�[�̃t�H���g
	TFont *dialogFont;			//�_�C�A���O�̃t�H���g

	int  GLstInterLine;			//�O���[�v���X�g�̍s��(��)
	int  FLstInterLine;			//�f�Ѓ��X�g�̍s��(��)
	int  XLstInterLine;			//�g���������X�g�̍s��(��)
	int  gfrmLineWidth;			//�O���[�v�g�̐���
	int  gfrmMaxRnd;

	int  new_frg_pos;			//�V�K�쐬�ʒu (1�`9)
	bool new_no_group;			//�V�K�쐬���ɖ�����

	TCursor crFragMove; 		//�f�Јړ����̃}�E�X�J�[�\��

	bool show_GroupFrame;		//�O���[�v�g��\��
	bool show_GroupLine;		//�O���[�v�֌W����\��
	bool show_FragLine;			//�f�Њ֌W����\��
	bool show_NamePlate;		//�\�D��\��
	bool show_LinkMark; 		//�����N�}�[�N��\��
	bool show_DivLine;			//��ʕ�������\��
	bool show_PosGrid;			//�z�u�O���b�h��\��

	TAlign glstAlign;			//�O���[�v���X�g�̕\���ʒu

	int  CurGLineType;			//�֌W���̌��݂̃f�t�H���g
	int  CurFLineType;
	int  CurGLineWidth;
	int  CurFLineWidth;
	int  CurGArrowType;
	int  CurFArrowType;
	int  CurGArrowSize;
	int  CurFArrowSize;
	bool CurTxOpposite;

	bool GlRightAngle;			//�ӓ��m�̊֌W�����Ȃ�ׂ�������

	bool auto_exp;				//�f�t�H���g���z�����玩���I�ɍs����
	bool input_wordwrap;		//�e�L�X�g���͂̉E�[�Ő܂�Ԃ�
	bool ime_on;				//�f�Ѓe�L�X�g���͎�IME�I��
	bool accept_cr; 			//�f�Ѓe�L�X�g���͎����s���󂯓����
	bool cr_returnkey;			//���s�̓��^�[���L�[��
	bool ok_ctrl_return;		//Ctrl + ���^�[���L�[�� OK
	UnicodeString CR_str;		//���s�̕ϊ�������
	bool LinkIsIcon;			//�����N���֘A�t���A�C�R���ŕ\��
	bool LinkRelPath;			//�����N�𑊑΃p�X�ŕۑ�

	UnicodeString HomeWindow;	//�z�[���E�B���h�E
	bool HomWinMax; 			//�z�[���E�B���h�E�͍ő剻
	UnicodeString LastFile;		//�O��Ō�ɃA�N�e�B�u�������t�@�C��
	bool OpenLastFile;			//�O��Ō�ɃA�N�e�B�u�������t�@�C�����N�����ɊJ��
	bool ChildMax;				//��ڂ̃E�B���h�E�͍ő剻
	bool HideTitleMenu; 		//�ő剻���Ƀ^�C�g���o�[�ƃ��j���[������
	bool RstScrPos; 			//�X�N���[���o�[��O��̈ʒu�ɕ���
	bool ActivDistWin;			//�f�Ђ̈ړ���E�B���h�E���A�N�e�B�u��
	int	 DefScrWd, DefScrHi;	//�f�t�H���g�̉�ʃT�C�Y

	bool SpreadShuffle; 		//�W�J���ɃV���b�t������
	bool SpreadAlign;			//�W�J���ɑ����ĕ��ׂ�
	int  StretchMode;			//�L�k���[�h(��_)  0:���S 1:����
	bool StretchGrp0;			//�������f�Ђ��A�����ăO���[�v�L�k
	int  away_frg_pos;			//�ЂÂ��ʒu(1�`4)
	int  ArrangePt;				//�ЂÂ��E�W���̂��炵��
	bool AlwaysShowNp;			//�\�D����ɕ\��
	bool NoNpHideGfrm;			//�\�D���Ȃ��ꍇ�̓O���[�v�g��\�����Ȃ�
	bool OnlyNpHideGfrm;		//�\�D�݂̂̏ꍇ�O���[�v�g��\�����Ȃ�
	bool PlateOnGfrm;			//�\�D���O���[�v�g��ɕ\��
	int	 PlateAlign;			//�O���[�v�g��̕\�D�̈ʒu
	int  PlateShape;			//�O���[�v�g��̕\�D�̊O�`	0:�����` 1:�p�ی` 2:�[�ی`
	bool MapViewSelRect;		//�S�̐}�ɑI��g��\��
	bool FitMapView;			//�S�̐}�̃E�B���h�E�T�C�Y���t�B�b�g
	int  MapAlpha;				//�S�̐}�̓����x
	bool CopyCLine;				//�؂���E�R�s�[�Ŋ֌W�����܂߂�
	bool ClipPosSort;			//�N���b�v�{�[�h�R�s�[���Ɉʒu�Ń\�[�g
	bool DropImgFile;			//�摜�t�@�C���h���b�v���ɉ摜�f�Ѝ쐬
	bool NewDropPos;			//�h���b�v�ʒu�ɐV�K�쐬
	bool ImageIsBack;			//�摜�͔w��ɕ\��
	bool ShowDstGrp;			//�f�Јړ���̃O���[�v����\����������\����
	bool FrameAllSel;			//�I��g�ƌ��������f�Ђ͑S�đI������
	bool ChgGrpModeCtrl;		//�O���[�v������Ctrl�L�[�̖��������ւ���
	bool RDragScroll;			//�E�h���b�O�ŉ�ʂ��X�N���[��
	bool SelJumpFrg;			//�W�����v��̒f�Ђ�I����Ԃɂ���
	bool PrintMono;				//����(2�l)���
	int  FixPrnLWidth;			//������̍ŏ������@0:��(�ŏ�1) 1..:�Œ�
	bool PrintTime;				//�����������
	bool PrintBackGr;			//�w�i�F�����
	bool PrintScrSize;			//��ʃT�C�Y�Ɋ�Â��Ĉ��

	bool HideBlkGrpItem;		//��O���[�v�̓O���[�v���X�g�ɕ\�����Ȃ�

	int  GridSize;				//�z�u�O���b�h�̃T�C�Y
	bool SnapGrid;				//�O���b�h�Ɉ�������

	bool TrimMovTop;			//�g���~���O���ɍ���[�Ɉړ�
	bool TrimScrSize;			//�g���~���O���Ƀ��T�C�Y
	bool TrimFixSize;			//�g���~���O���ɕύX�T�C�Y���Œ�
	bool TrimSaveAs;			//�g���~���O��ɖ��O�����ĕۑ�
	int  TrimMargin;			//�g���~���O���̗]��
	int  TrimFixWd;				//�Œ�T�C�Y
	int  TrimFixHi;

	bool NewBlkDblClick;		//�O���[�v���X�g�ŁA��O���[�v���_�u���N���b�N�ŐV�K�f��

	UnicodeString CsvFormat;	//�G�N�X�|�[�g����

	bool ExportUTF8;			//UTF8�ŃG�N�X�|�[�g

	UnicodeString FindWord;		//������
	int  FindMode;				//�������[�h
	bool FindAndSw;				//����: AND
	bool FindNotSw;				//����: NOT
	bool FindRegSw;				//����: ���K�\��
	bool FindCaseSw;			//����: �召�������
	bool FindFupSw;				//����: �f�Ѓ��X�g�Ő擪�ɏW�߂�

	int  ImgFilterIdx;			//�t�B���^�̃C���f�b�N�X(1,2)
	int  JpegQuality;			//Jpeg�摜�ۑ����̕i��

	int  WheelScrQty;			//�z�C�[���̃X�N���[����
	bool WheelShiftHorz;		//Shit + �z�C�[���ŉ��X�N���[��

	bool IndFixRatio;			//�ʐݒ�ŏc������Œ�

	//------------------------------
	int TimerLockCnt;			//�^�C�}�[�����}�~�p�J�E���^
								// >0 �̏ꍇ�^�C�}�[�������s��Ȃ�

	bool reqUpdate;				//�e��\���X�V�v��
	bool reqFrgLst;				//�f�Ѓ��X�g�̍X�V�v��

	TStringList *AssociateList;		//�֘A�Â����X�g

	TStringList *JumpHistoryList;	//�W�����v����
	int HistoryPtr;

	TStringList *FindHistoryList;	//��������
	TStringList *FileHistoryList;	//�t�@�C������

	bool printer_ok;

	Graphics::TBitmap *LinkMark;	//�����N�}�[�N
	TStringList *LinkIconList;		//�����N�A�C�R�����X�g
	TStringList *CachedIcoList;		//�t�@�C���ˑ��̃A�C�R���L���b�V�����X�g

	UnicodeString SpiDir;			//Susie�v���O�C���̃f�B���N�g��
	bool UseSpiFirst;				//Susie�v���O�C����D��I�Ɏg�p

	TControl *LastDockSite;		//�O���[�v���X�g���Ō�Ƀh�b�L���O�����T�C�g

	int FLstSortDir;			//�f�Ѓ��X�g�̃\�[�g���� 1:���� -1:�~��

	//------------------------------
	EnvData(Classes::TComponent* AOwner);
	~EnvData();

	UnicodeString __fastcall read_ini_str(UnicodeString sct, UnicodeString key);
	void __fastcall load_pos_info(UnicodeString sct, TForm *frm,
		int x, int y, int w, int h);
	void __fastcall save_pos_info(UnicodeString sct, TForm *frm);
	void __fastcall load_tool_info(UnicodeString sct, TToolBar *tb);
	void __fastcall save_tool_info(UnicodeString sct, TToolBar *tb);
	void __fastcall add_file_history(UnicodeString fnam);
	void __fastcall set_custom_col(TStrings *cust_col);
	void __fastcall get_custom_col(TStrings *cust_col);
	UnicodeString __fastcall get_associate(UnicodeString fnam);
	bool __fastcall save_ini_file();
};

//---------------------------------------------------------------------------
extern EnvData  *EV;
extern SpiUnit  *SPI;

extern TCursor crHandGrabL, crHandGrabR, crHandPickL, crHandPickR;
extern TCursor crLineTool, crGroupTool, crBrushTool, crSpuitTool;

extern double ScrScale;

//---------------------------------------------------------------------------
#endif
