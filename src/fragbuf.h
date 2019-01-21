//----------------------------------------------------------------------//
//	断片、関係線、グループ情報のバッファのクラス						//
//																		//
//		カットバッファや完全アンドゥに使用								//
//----------------------------------------------------------------------//
#ifndef FragBufH
#define FragBufH

//---------------------------------------------------------------------------
#include "arwline.h"
#include "fragment.h"
#include "fraglist.h"
#include "grpinfs.h"

//---------------------------------------------------------------------------
//断片バッファ
//---------------------------------------------------------------------------
class FragBuffer {
private:
	int  MaxZ;							//(最大)最前面Z
	int  MaxID;							//最大ID

public:
	TFragList  *FrgList; 				//断片のリスト
	TArrowList *CLineList;				//関係線リスト

	grp_inf GinfItem[MAX_GROUP + 1];	//グループ情報

	int  scr_wd, scr_hi;				//画面の全体サイズ
	int  LastHPos, LastVPos;			//前回の位置

	bool IsCopy;

	TForm *CopySource;					//コピー元

	FragBuffer();
	~FragBuffer();

	Fragment *new_frag(bool idsw = true);
	void del_all();
};

//---------------------------------------------------------------------------
#endif
