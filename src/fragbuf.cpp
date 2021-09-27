//----------------------------------------------------------------------//
//	断片、関係線、グループ情報のバッファのクラス						//
//																		//
//		カットバッファやアンドゥに使用									//
//----------------------------------------------------------------------//
#include "EnvData.h"
#include "fragbuf.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
FragBuffer::FragBuffer()
{
	FrgList = new TFragList();
	MaxZ  = -1;
	MaxID = -1;

	CLineList  = new TArrowList(NULL);

	scr_wd	 = EV->DefScrWd;
	scr_hi	 = EV->DefScrHi;
	LastHPos = LastVPos = 0;

	IsCopy = false;
	CopySource = NULL;
}
//---------------------------------------------------------------------------
FragBuffer::~FragBuffer()
{
	del_all();
	delete FrgList;
	delete CLineList;
}

//---------------------------------------------------------------------------
//断片を生成
//---------------------------------------------------------------------------
Fragment *FragBuffer::new_frag(bool idsw)
{
	Fragment *fp = new Fragment(NULL);
	fp->Z		 = ++MaxZ;
	fp->ID		 = idsw? ++MaxID : -1;
	fp->Visible  = true;
	FrgList->Add(fp);
	return fp;
}

//---------------------------------------------------------------------------
//全て破棄
//---------------------------------------------------------------------------
void FragBuffer::del_all()
{
	FrgList->del_all();
	CLineList->del_all();
}
//---------------------------------------------------------------------------

