//----------------------------------------------------------------------//
//	�f�ЁA�֌W���A�O���[�v���̃o�b�t�@�̃N���X						//
//																		//
//		�J�b�g�o�b�t�@�⊮�S�A���h�D�Ɏg�p								//
//----------------------------------------------------------------------//
#ifndef FragBufH
#define FragBufH

//---------------------------------------------------------------------------
#include "arwline.h"
#include "fragment.h"
#include "fraglist.h"
#include "grpinfs.h"

//---------------------------------------------------------------------------
//�f�Ѓo�b�t�@
//---------------------------------------------------------------------------
class FragBuffer {
private:
	int  MaxZ;							//(�ő�)�őO��Z
	int  MaxID;							//�ő�ID

public:
	TFragList  *FrgList; 				//�f�Ђ̃��X�g
	TArrowList *CLineList;				//�֌W�����X�g

	grp_inf GinfItem[MAX_GROUP + 1];	//�O���[�v���

	int  scr_wd, scr_hi;				//��ʂ̑S�̃T�C�Y
	int  LastHPos, LastVPos;			//�O��̈ʒu

	bool IsCopy;

	TForm *CopySource;					//�R�s�[��

	FragBuffer();
	~FragBuffer();

	Fragment *new_frag(bool idsw = true);
	void del_all();
};

//---------------------------------------------------------------------------
#endif
