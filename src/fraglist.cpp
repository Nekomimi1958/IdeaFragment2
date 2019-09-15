//----------------------------------------------------------------------//
//	�f�Ѓ��X�g�̃N���X													//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include <algorithm>

#include "EnvData.h"
#include "UserFunc.h"
#include "fraglist.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//===========================================================================
//�f�Ѓ��X�g (TList ����p��)
//===========================================================================
__fastcall TFragList::TFragList(): TList()
{
}
//---------------------------------------------------------------------------
void __fastcall TFragList::AllShow()
{
	for (int i=0; i<Count; i++) Items[i]->show_frag();
}

//---------------------------------------------------------------------------
//�S�Ĕj��
//---------------------------------------------------------------------------
void __fastcall TFragList::del_all()
{
	for (int i=0; i<Count; i++) delete Items[i];
	Clear();
}


//---------------------------------------------------------------------------
// ID����f�Ђ̃��X�g�C���f�b�N�X���擾
//---------------------------------------------------------------------------
int __fastcall TFragList::id_to_idx(int id)
{
	int idx = -1;
	for (int i=0; i<Count; i++) {
		if (Items[i]->ID==id) {
			idx = i; break;
		}
	}
	return idx;
}
//---------------------------------------------------------------------------
// ID����f�Ђւ̃|�C���^���擾
//---------------------------------------------------------------------------
Fragment * __fastcall TFragList::id_to_fp(int id)
{
	Fragment *fp = NULL;
	for (int i=0; i<Count; i++) {
		if (Items[i]->ID==id) {
			fp = Items[i]; break;
		}
	}
	return fp;
}
//---------------------------------------------------------------------------
// �����񂪑O����v����f�Ђւ̃|�C���^���擾
//---------------------------------------------------------------------------
Fragment * __fastcall TFragList::str_to_fp(UnicodeString s)
{
	Fragment *fp = NULL;
	if (!s.IsEmpty()) {
		for (int i=0; i<Count; i++) {
			if (ContainsText(Items[i]->TextStr, s)) {
				fp = Items[i]; break;
			}
		}
	}
	return fp;
}

//---------------------------------------------------------------------------
//�����f�Ђ̍���ʒu���擾
//---------------------------------------------------------------------------
TPoint __fastcall TFragList::get_min_pos()
{
	int min_x = 999999;
	int min_y = 999999;
	for (int i=0; i<Count; i++) {
		TRect *rp = &Items[i]->RC;
		min_x = std::min<int>(rp->Left, min_x);
		min_y = std::min<int>(rp->Top,  min_y);
	}
	return (Point(min_x, min_y));
}


//===========================================================================
// �\�[�g�֐�
//===========================================================================
//�O���[�v�ɂ��g����r (1,2..,0)
//---------------------------------------------------------------------------
int __fastcall FrgCmp_GroupEx(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;

	int ret = 0;
	for (int i=0; i<MAX_IN_GRP; i++) {
		if (fp1->group.stk[i]==fp2->group.stk[i]) continue;
		if		(fp1->group.stk[i]==0) ret =  1;
		else if (fp2->group.stk[i]==0) ret = -1;
		else ret = (fp1->group.stk[i] - fp2->group.stk[i]);
		break;
	}

	if (ret==0) {
		if (fp1->style==fp2->style) {
			if (fp1->TextStr==fp2->TextStr) {
				if (fp1->colBG==fp2->colBG) {
					if (fp1->CreTime==fp2->CreTime) {
						if (fp1->ModTime==fp2->ModTime) {
							if (fp1->LinkName==fp2->LinkName) {
								ret = (fp1->ID - fp2->ID);
							}
							else {
								if		(fp1->LinkName.IsEmpty()) ret =  1;
								else if (fp2->LinkName.IsEmpty()) ret = -1;
								else ret = CompareStr(fp1->LinkName, fp2->LinkName);
							}
						}
						else {
							ret = -1 * (int)(fp1->ModTime - fp2->ModTime);
						}
					}
					else {
						ret = -1 * (int)(fp1->CreTime - fp2->CreTime);
					}
				}
				else {
					ret = -1 * (int)(fp1->colBG - fp2->colBG);
				}
			}
			else {
				if		(fp1->TextStr.IsEmpty()) ret =  1;
				else if (fp2->TextStr.IsEmpty()) ret = -1;
				else ret = CompareStr(fp1->TextStr, fp2->TextStr);
			}
		}
		else {
			if		(fp1->style==fgsPlate)  ret = -1;
			else if (fp2->style==fgsPlate)  ret =  1;
			else if (fp1->style==fgsNormal) ret = -1;
			else if (fp2->style==fgsNormal) ret =  1;
			else if (fp1->style==fgsImage)  ret = -1;
			else if (fp2->style==fgsImage)  ret =  1;
			else							ret =  0;
		}
	}
	else {
		ret *= EV->FLstSortDir;
	}
	return ret ;
}
//---------------------------------------------------------------------------
//��ނɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_Style(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	int ret;
	if (fp1->style==fp2->style) {
		ret = FrgCmp_GroupEx(Item1, Item2);
	}
	else {
		if		(fp1->style==fgsPlate)  ret = -1;
		else if (fp2->style==fgsPlate)  ret =  1;
		else if (fp1->style==fgsNormal) ret = -1;
		else if (fp2->style==fgsNormal) ret =  1;
		else if (fp1->style==fgsImage)  ret = -1;
		else if (fp2->style==fgsImage)  ret =  1;
		else 							ret =  0;
		ret *= EV->FLstSortDir;
	}
	return ret;
}
//---------------------------------------------------------------------------
//�e�L�X�g�ɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_Text(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	int ret;
	if (fp1->TextStr==fp2->TextStr) {
		ret = FrgCmp_GroupEx(Item1, Item2);
	}
	else {
		if		(fp1->TextStr.IsEmpty()) ret =  1;
		else if (fp2->TextStr.IsEmpty()) ret = -1;
		else ret = CompareStr(fp1->TextStr, fp2->TextStr);
		ret *= EV->FLstSortDir;
	}
	return ret;
}
//---------------------------------------------------------------------------
//�쐬�����ɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_CreTime(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	int ret;
	if (fp1->CreTime==fp2->CreTime)
		ret = FrgCmp_GroupEx(Item1, Item2);
	else
		ret = ((fp1->CreTime > fp2->CreTime)? -1 : 1) * EV->FLstSortDir;
	return ret;
}
//---------------------------------------------------------------------------
//�X�V�����ɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_ModTime(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	int ret;
	if (fp1->ModTime==fp2->ModTime)
		ret = FrgCmp_GroupEx(Item1, Item2);
	else
		ret = ((fp1->ModTime > fp2->ModTime)? -1 : 1) * EV->FLstSortDir;
	return ret;
}
//---------------------------------------------------------------------------
//�����N��ɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_Link(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	int ret;
	if (fp1->LinkName==fp2->LinkName) {
		ret = FrgCmp_GroupEx(Item1, Item2);
	}
	else {
		if		(fp1->LinkName.IsEmpty()) ret =  1;
		else if (fp2->LinkName.IsEmpty()) ret = -1;
		else ret = CompareStr(fp1->LinkName, fp2->LinkName);
		ret *= EV->FLstSortDir;
	}
	return ret;
}
//---------------------------------------------------------------------------
//ID �ɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_ID(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	return ((fp1->ID - fp2->ID) * EV->FLstSortDir);
}

//---------------------------------------------------------------------------
//�I����Ԃɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_Select(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	int ret;
	if (fp1->Selected==fp2->Selected)
		ret = FrgCmp_GroupEx(Item1, Item2);
	else
		ret = fp1->Selected? -1 : 1;
	return ret;
}

//---------------------------------------------------------------------------
//Z�I�[�_�[ �ɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_Z(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	if (EV->ImageIsBack) {
		if (fp1->style==fp2->style)	   return (fp1->Z - fp2->Z);
		else if (fp1->style==fgsImage) return -1;
		else if (fp2->style==fgsImage) return 1;
		else						   return (fp1->Z - fp2->Z);
	}
	else {
		return (fp1->Z - fp2->Z);
	}
}
//---------------------------------------------------------------------------
//�ʒu�ɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_Pos(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;

	TRect rc1 = fp1->RC;
	TRect rc2 = fp2->RC;
	rc1.top = rc2.top = 0;
	rc1.bottom = rc2.bottom = 1;
	bool flag  = false;

	TRect rc = rc1;  rc.Intersect(rc2);
	if (!rc.IsEmpty()) flag = (rc.Width() > std::min(rc1.Width(), rc2.Width())/2);
	//���̔����ȏ�d�Ȃ��Ă�����A�c���тƂ݂Ȃ��ォ�牺��
	if (flag)
		return (fp1->RC.Top - fp2->RC.Top);
	//����ȊO�͍�����E��
	else
		return (fp1->RC.Left - fp2->RC.Left);
}
//---------------------------------------------------------------------------
//X�ʒu�ɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_PosX(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	if (fp1->RC.Left==fp2->RC.Left)
		return (fp1->RC.Top - fp2->RC.Top);
	else
		return (fp1->RC.Left - fp2->RC.Left);
}
//---------------------------------------------------------------------------
//Y�ʒu�ɂ���r
//---------------------------------------------------------------------------
int __fastcall FrgCmp_PosY(void * Item1, void * Item2)
{
	Fragment *fp1 = (Fragment*)Item1;
	Fragment *fp2 = (Fragment*)Item2;
	if (fp1->RC.Top==fp2->RC.Top)
		return (fp1->RC.Left - fp2->RC.Left);
	else
		return (fp1->RC.Top - fp2->RC.Top);
}
//---------------------------------------------------------------------------
