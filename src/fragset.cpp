//----------------------------------------------------------------------//
//	断片セットのクラス													//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <math.h>
#include <Vcl.Clipbrd.hpp>
#include <Vcl.Printers.hpp>
#include <RegularExpressions.hpp>
#include "usr_wic.h"
#include "UserFunc.h"
#include "EnvData.h"
#include "fragset.h"
#include "Main.h"
#include "PropDlg.h"
#include "MapView.h"
#include "FindEx.h"
#include "FrgGrid.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//断片セットのクラス
//---------------------------------------------------------------------------
FragSet::FragSet(TForm* AOwner)
{
	frg_owner = AOwner;
	file_name = data_title = EmptyStr;
	title_pos = 0;

	scr_wd	  = EV->DefScrWd;
	scr_hi	  = EV->DefScrHi;

	div_mod  = 0;
	div_lw	 = 1;
	div_x = div_y = 0;

	col_backgr  = Graphics::clNone;
	col_DivLine = Graphics::clNone;
	col_GrpFrm	= Graphics::clNone;
	col_GrpLine = Graphics::clNone;
	col_FrgLine = Graphics::clNone;
	col_LinLbl  = Graphics::clNone;
	col_Title	= Graphics::clNone;

	frgFont = new TFont(); frgFont->Assign(EV->frgFont);
	namFont = new TFont(); namFont->Assign(EV->namFont);
	symFont = new TFont(); symFont->Assign(EV->symFont);
	lblFont = new TFont(); lblFont->Assign(EV->lblFont);
	titFont = new TFont(); titFont->Assign(EV->titFont);

	ParentFrgFont = ParentNamFont = ParentSymFont = true;
	ParentLblFont = ParentTitFont = true;

	read_only = false;
	modify	  = false;
	forcesave = false;
	movskip   = moved = moving = movgroup = pickuped = floating = false;
	jump_fp   = find_fp = catched_fp = NULL;

	FrgList = new TFragList();
	SelList = new TFragList();
	MaxZ  = -1;
	MaxID = -1;

	//関係線の初期化
	CLineList = new TArrowList(AOwner);
	CurLp	  = NULL;
	CLineMode = CLMODE_NON;

	//グループ枠の作成・初期化
	GInfs = new GrpInfs(AOwner, FrgList);
	GInfs->col_GrpFrm = col_GrpFrm;
	GInfs->namFont	  = namFont;

	//アンドゥ・バッファの初期化
	for (int i=0; i<MAX_UNDO; i++) UndoBuf[i] = new FragBuffer();
	UndoN = RedoN = 0;
	UndoPtr = -1;

	HscrBar = frg_owner->HorzScrollBar;
	VscrBar = frg_owner->VertScrollBar;
	VscrBar->Range = 0;
	HscrBar->Range = 0;

	inh_Grid = false;
}
//---------------------------------------------------------------------------
FragSet::~FragSet()
{
	//断片を削除
	for (int i=0; i<FrgList->Count; i++) delete FrgList->Items[i];
	delete FrgList;
	delete SelList;

	//関係線を削除
	for (int i=0; i<CLineList->Count; i++) delete CLineList->Items[i];
	delete CLineList;

	delete GInfs;
	delete frgFont;
	delete namFont;
	delete symFont;
	delete lblFont;
	delete titFont;

	for (int i=0; i<MAX_UNDO; i++) delete UndoBuf[i];
}

//---------------------------------------------------------------------
//断片を生成
//---------------------------------------------------------------------
Fragment *FragSet::new_frag(bool id_sw)
{
	Fragment *fp = new Fragment(frg_owner);
	fp->Z		 = ++MaxZ;
	fp->ID		 = id_sw? ++MaxID : -1;
	fp->Visible  = true;
	fp->frgFont  = frgFont;
	fp->namFont  = namFont;
	fp->symFont  = symFont;
	FrgList->Add(fp);
	return fp;
}
//---------------------------------------------------------------------
//断片を破棄
//---------------------------------------------------------------------
void FragSet::del_frag(Fragment *fp)
{
	if (fp==find_fp) find_fp = NULL;

	//グループ枠の表札情報を削除
	if (fp->style==fgsPlate) {
		for (int gn=1; gn<=MAX_GROUP; gn++) {
			grp_inf *gp = &GInfs->Items[gn];
			if (gp->Plate==fp) {
				gp->Plate = NULL;
				gp->gfp->Plate	   = NULL;
				gp->gfp->PlateRect = Rect(0, 0, 0, 0);
			}
		}
	}

	//断片に引かれている関係線を削除
	int i = 0;
	while (i<CLineList->Count) {
		ArrowLine *ap = CLineList->Items[i];
		bool dflag = false;
		if (ap->tType==TDIR_F2F) {
			if		(FrgList->id_to_fp(ap->fromTag)==fp) dflag = true;
			else if (FrgList->id_to_fp(ap->toTag)==fp)   dflag = true;
		}
		else if (ap->tType==TDIR_F2G || ap->tType==TDIR_G2F) {
			if (ap->tType==TDIR_F2G) {
				if (FrgList->id_to_fp(ap->fromTag)==fp) dflag = true;
			}
			else {
				if (FrgList->id_to_fp(ap->toTag)==fp)   dflag = true;
			}
		}

		if (dflag) {;
			ap->erase(frg_owner);
			delete ap; CLineList->Delete(i);
		}
		else i++;
	}

	delete fp;
}

//---------------------------------------------------------------------
//グループ空状態フラグを更新
//---------------------------------------------------------------------
void FragSet::update_grp_empty()
{
	GInfs->update_empty();

	//引き上げを考慮したグループ枠内の断片範囲を取得
	TRect trc[MAX_GROUP + 1];
	for (int gn=1; gn<=MAX_GROUP; gn++) {
		trc[gn] = Rect(0, 0, 0, 0);
		for (int i=0; i<FrgList->Count; i++) {
			Fragment *fp = FrgList->Items[i];
			if (pickuped && fp->Selected) continue;
			if (fp->in_group(gn)==-1) continue;
			if (!fp->Visible) continue;
			if (fp->style==fgsPlate && fp->OnGfrm) continue;
			::UnionRect(&trc[gn], &trc[gn], &fp->RC);
		}
	}

	int i = 0;
	while (i<CLineList->Count) {
		ArrowLine *ap = CLineList->Items[i];
		//空グループへのグループ間関係線を削除
		if (ap->tType==TDIR_G2G) {
			if (GInfs->Items[ap->fromTag].Empty || GInfs->Items[ap->toTag].Empty) {
				ap->erase(frg_owner);
				delete ap;
				CLineList->Delete(i);
			}
			else {
				if (trc[ap->fromTag].IsEmpty() || trc[ap->toTag].IsEmpty()) ap->erase(frg_owner);
				i++;
			}
		}
		//空グループにつながる断片・グループ間関係線を削除
		else if (ap->tType==TDIR_F2G || ap->tType==TDIR_G2F) {
			int gn = (ap->tType==TDIR_F2G)? ap->toTag : ap->fromTag;
			if (GInfs->Items[gn].Empty) {
				ap->erase(frg_owner);
				delete ap;
				CLineList->Delete(i);
			}
			else {
				if (trc[gn].IsEmpty()) ap->erase(frg_owner);
				i++;
			}
		}
		else i++;
	}

	CurLp = NULL;
	EV->reqFrgLst = true;
}

//---------------------------------------------------------------------
//選択中の断片を削除
//---------------------------------------------------------------------
void FragSet::del_sel_frag()
{
	push_all();

	int i = 0;
	while (i<FrgList->Count) {
		Fragment *fp = FrgList->Items[i];
		if (fp->Selected) {
			del_frag(fp);
			FrgList->Delete(i);
		}
		else i++;
	}
	SelList->Clear();
	update_grp_empty();
	modify		  = true;
	EV->reqUpdate = true;
}

//---------------------------------------------------------------------
//グループ枠と関係線の表示を更新
//---------------------------------------------------------------------
void FragSet::update_group_frame()
{
	GInfs->update_frame(pickuped);

	//グループ間関係線更新・描画
	frg_owner->Canvas->Font->Assign(lblFont);
	for (int i=0; i<CLineList->Count; i++) {
		ArrowLine *ap = CLineList->Items[i];
		if (ap->tType!=TDIR_G2G) continue;
		grp_inf *gp0 = &GInfs->Items[ap->fromTag];
		grp_inf *gp1 = &GInfs->Items[ap->toTag];
		if (gp0->cRC.IsEmpty() || gp1->cRC.IsEmpty()) continue;
		octrect xrc0 = gp0->gfp->FrmOctRect;
		octrect xrc1 = gp1->gfp->FrmOctRect;
		if (!ap->set_pos(xrc0, xrc1, HscrBar->Position, VscrBar->Position)) continue;
		if (!EV->show_GroupLine) continue;
		Fragment *np = gp0->Plate;
		bool nflag = np? np->Visible : false;
		if (!nflag && !gp0->Visible) continue;
		np = gp1->Plate;
		nflag = np? np->Visible : false;
		if (!nflag && !gp1->Visible) continue;
		ap->draw(frg_owner->Canvas, 0, GET_indcol(col_GrpLine), GET_indcol(col_LinLbl), (CurLp==ap));
	}

	for (int gn=1; gn<MAX_GROUP; gn++) GInfs->Items[gn].RepaintLn = false;
}

//---------------------------------------------------------------------
//断片関係線の表示を更新
//---------------------------------------------------------------------
void FragSet::update_frg_line()
{
	frg_owner->Canvas->Font->Assign(lblFont);
	int i = 0;
	while (i<CLineList->Count) {
		ArrowLine *ap = CLineList->Items[i];
		bool ssw = (CurLp==ap);
		if (ap->tType==TDIR_F2F) {
			int idx0 = FrgList->id_to_idx(ap->fromTag);
			int idx1 = FrgList->id_to_idx(ap->toTag);
			if (idx0==-1 || idx1==-1) {
				//断片がすでに無ければ破棄
				delete ap; CLineList->Delete(i);
				if (ssw) CurLp = NULL;
				continue;
			}
			i++;
			Fragment *fp0 = FrgList->Items[idx0];
			Fragment *fp1 = FrgList->Items[idx1];
			if (fp0->style==fgsPlate && fp0->OnGfrm) continue;
			if (fp1->style==fgsPlate && fp1->OnGfrm) continue;
			octrect xrc0, xrc1;
			set_octrect(&fp0->vRC, &xrc0);
			set_octrect(&fp1->vRC, &xrc1);
			if (!ap->set_pos(xrc0, xrc1, HscrBar->Position, VscrBar->Position,
				(fp0->style==fgsJunction), (fp1->style==fgsJunction))) continue;
			if (!EV->show_FragLine) continue;
			if (!fp0->Visible || !fp1->Visible) continue;
			ap->draw(frg_owner->Canvas, 0, GET_indcol(col_FrgLine), GET_indcol(col_LinLbl), ssw);
		}
		else i++;
	}
}
//---------------------------------------------------------------------
//断片・グループ間関係線の表示を更新
//---------------------------------------------------------------------
void FragSet::update_fg_line()
{
	Fragment *fp0, *fp1;
	frg_owner->Canvas->Font->Assign(lblFont);
	int i = 0;
	while (i<CLineList->Count) {
		ArrowLine *ap = CLineList->Items[i];
		bool ssw = (CurLp==ap);

		if (ap->tType==TDIR_F2G) {
			fp0 = FrgList->id_to_fp(ap->fromTag);
			if (!fp0) {;
				//断片がすでに無ければ破棄
				delete ap; CLineList->Delete(i);
				if (ssw) CurLp = NULL;
				continue;
			}
		}
		else if (ap->tType==TDIR_G2F) {
			fp1 = FrgList->id_to_fp(ap->toTag);
			if (!fp1) {;
				//断片がすでに無ければ破棄
				delete ap; CLineList->Delete(i);
				if (ssw) CurLp = NULL;
				continue;
			}
		}
		else {
			i++; continue;
		}
		i++;

		octrect xrc0, xrc1;
		grp_inf *gp;
		//断片→グループ
		if (ap->tType==TDIR_F2G) {
			if (fp0->style==fgsPlate && fp0->OnGfrm) continue;
			if (!fp0->Visible) continue;
			set_octrect(&fp0->vRC, &xrc0);
			gp	 = &GInfs->Items[ap->toTag];
			xrc1 = gp->gfp->FrmOctRect;
		}
		//グループ→断片
		else {
			if (fp1->style==fgsPlate && fp1->OnGfrm) continue;
			if (!fp1->Visible) continue;
			set_octrect(&fp1->vRC, &xrc1);
			gp	 = &GInfs->Items[ap->fromTag];
			xrc0 = gp->gfp->FrmOctRect;
		}
		if (gp->cRC.IsEmpty()) continue;
		if (ap->tType==TDIR_F2G) {
			if (!ap->set_pos(xrc0, xrc1, HscrBar->Position, VscrBar->Position,
				(fp0->style==fgsJunction),false)) continue;
		}
		else {
			if (!ap->set_pos(xrc0, xrc1, HscrBar->Position, VscrBar->Position,
				false, (fp1->style==fgsJunction))) continue;
		}
		if (!EV->show_FragLine) continue;
		ap->draw(frg_owner->Canvas, 0, GET_indcol(col_FrgLine), GET_indcol(col_LinLbl), ssw);
	}
}

//---------------------------------------------------------------------
//注目の解除
//---------------------------------------------------------------------
void FragSet::reset_notice()
{
	if (find_fp) {
		find_fp->Noticed = false;
		find_fp->show_frag();
		find_fp = NULL;
	}
	notice_pos = -1;
}
//---------------------------------------------------------------------
//指定した断片を画面の中央へ
//---------------------------------------------------------------------
void FragSet::to_center(
	Fragment *fp,
	bool sw,		//true = fpを注目
	int x, int y)	//ともに >=0 なら位置指定 (default = -1)
{
	if (fp) {
		TPoint cp;
		//グループ枠上の表札
		if (fp->style==fgsPlate && fp->OnGfrm) {
			TGrpFrame *sp = GInfs->Items[fp->group.stk[0]].gfp;
			TRect prc = sp->PlateRect;
			prc.Offset(sp->Left, sp->Top);
			cp = prc.CenterPoint();
		}
		//通常断片
		else {
			cp = fp->vRC.CenterPoint();
		}

		if (x==-1) x = frg_owner->ClientWidth/2;
		if (y==-1) y = frg_owner->ClientHeight/2;

		HscrBar->Position += (cp.x - x);
		VscrBar->Position += (cp.y - y);

		if (find_fp) {
			find_fp->Noticed = false;
			find_fp->Changed = true;
			find_fp->show_frag();
		}

		if (sw) {
			find_fp	= fp;
			find_fp->Noticed = true;
			find_fp->Changed = true;
			find_fp->show_frag();
			bring_to_front(fp);
		}
		else
			reset_notice();
	}
}

//---------------------------------------------------------------------
//指定アンカーに該当する画面の中央へ
//---------------------------------------------------------------------
void FragSet::to_anchor(
	UnicodeString anam)		//"#ID" または "#文字列" (\t移行は無視)
{
	if (!anam.IsEmpty()) {
		if (anam[1]=='#') anam.Delete(1, 1);
		anam = get_tkn(anam, "\t");
		Fragment *fp = NULL;
		if (!anam.IsEmpty()) {
			int idn = anam.ToIntDef(-1);
			fp = (idn!=-1)? FrgList->id_to_fp(idn) : FrgList->str_to_fp(anam);
		}
		if (fp) {
			if (EV->SelJumpFrg)
				one_select_center(fp);
			else
				to_center(fp, false);
			LastAnc = anam;
		}
		else
			LastAnc = EmptyStr;
	}
}

//---------------------------------------------------------------------
//断片を新規作成位置に
//---------------------------------------------------------------------
void FragSet::set_new_pos(
	Fragment *fp,
	int idx,
	int x, int y)	//ともに >=0 なら位置指定 (default = -1)
{
	if (!fp) return;

	int w = fp->wd;
	int h = fp->hi;

	int xp, yp;
	int fntsiz = abs(EV->frgFont->Height);

	if (x>=0 && y>=0) {
		xp = x;
		yp = y;
	}
	else {
		switch (EV->new_frg_pos) {
		case 1: case 4: case 7:	//left
			xp = 4; break;
		case 2: case 5: case 8:	//center
			xp = (frg_owner->ClientWidth - (fntsiz * w + 4))/2; break;
		case 3: case 6: case 9:	//right
			xp = frg_owner->ClientWidth - (fntsiz * w + 6); break;
		default:
			xp = 4;
		}
		switch (EV->new_frg_pos) {
		case 1: case 2: case 3:	//top
			yp = 4; break;
		case 4: case 5: case 6:	//center
			yp = (frg_owner->ClientHeight - (fntsiz * h + h*2 + 8))/2; break;
		case 7: case 8: case 9:	//bottom
			yp = frg_owner->ClientHeight - (fntsiz * h + h*2 + 8); break;
		default:
			yp = 4;
		}
	}

	xp += ((idx % 100) +idx/100*(fntsiz*EV->def_wd + 8));
	yp += (idx % 100);

	fp->RC.Left = xp + HscrBar->Position;
	fp->RC.Top	= yp + VscrBar->Position;
}

//---------------------------------------------------------------------
//単独選択の断片
//---------------------------------------------------------------------
Fragment *FragSet::get_sel_single()
{
	return ((SelList->Count==1)? SelList->Items[0] : NULL);
}

//---------------------------------------------------------------------
//選択断片リストを作成
//---------------------------------------------------------------------
int FragSet::mak_sel_list(
	bool show)		//断片の表示を更新
{
	reset_notice();

	int last_cnt = SelList->Count;

	SelList->Clear();
	for (int gn=0; gn<=MAX_GROUP; gn++) {
		grp_inf *gp = &GInfs->Items[gn];
		int n = 0;
		if (!gp->Empty) {
			for (int i=0; i<FrgList->Count; i++) {
				Fragment *fp = FrgList->Items[i];
				if (fp->in_group(gn)==-1) continue;
				if (fp->Selected) {
					if (fp->group.stk[0] == gn) SelList->Add(fp);
					n++;
				}
			}
		}
		gp->Selected	= (n>0);
		gp->AllSelected = (n==gp->TotalSize);
	}

	//検索語の強調表示をリセット
	if (SelList->Count==0 && last_cnt!=SelList->Count) {
		for (int i=0; i<FrgList->Count; i++) FrgList->Items[i]->Matched  = false;
		EV->FindWord = EmptyStr;
	}

	SelList->Sort(FrgCmp_Pos);
	EV->reqFrgLst = true;

	if (show) SelList->AllShow();

	return SelList->Count;
}

//---------------------------------------------------------------------
//選択中の重心に一番近い断片を取得
//---------------------------------------------------------------------
Fragment *FragSet::get_sel_center()
{
	if (SelList->Count==0) return NULL;

	//重心を求める
	int cx=0, cy=0;
	for (int i=0; i<SelList->Count; i++) {
		TPoint cp = SelList->Items[i]->RC.CenterPoint();
		cx += cp.x;
		cy += cp.y;
	}
	cx /= SelList->Count;
	cy /= SelList->Count;

	//重心に一番近い断片を探す
	Fragment *cfp = NULL;
	int smin = 999999999;
	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		TPoint cp = fp->RC.CenterPoint();
		int dx = abs((int)(cx - cp.x));
		int dy = abs((int)(cy - cp.y));
		int s  = dx*dx + dy*dy;
		if (s<smin) {
			smin = s;
			cfp  = fp;
		}
	}

	return cfp;
}

//---------------------------------------------------------------------
//選択中断片の統合四角形を取得
//---------------------------------------------------------------------
TRect FragSet::get_sel_union(
	bool tmp_sw)	//tmpRC を対象
{
	TRect rc = Rect(0, 0, 0, 0);
	for (int i=0; i<SelList->Count; i++) {
		::UnionRect(&rc, &rc, (tmp_sw? &SelList->Items[i]->tmpRC : &SelList->Items[i]->RC));
	}
	return rc;
}

//---------------------------------------------------------------------
//選択中断片の指定原点を取得
//---------------------------------------------------------------------
TPoint FragSet::get_sel_org_pos(
	int mode,		//0=左上/ 1=中心/ 2=右下
	bool tmp_sw)	//tmpRC を対象
{
	if (SelList->Count<2) return Point(0, 0);

	TRect rc = get_sel_union(tmp_sw);

	switch (mode) {
	case 1:  return rc.CenterPoint();			//中心
	case 2:  return Point(rc.Right, rc.Bottom);	//右下
	default: return Point(rc.Left,  rc.Top);	//左上
	}
}

//---------------------------------------------------------------------
//選択中グループを取得 (混在していたら -1)
//---------------------------------------------------------------------
int FragSet::get_sel_group()
{
	int gn = -1;
	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		int gnx = fp->get_top_group();
		if (gnx==gn) continue;
		if (gn==-1)
			gn = gnx;
		else {
			gn = -1; break;
		}
	}

	return gn;
}

//---------------------------------------------------------------------
//選択中のグループを取得 (1～ )
//  なければグループリストで選択されているグループ
//  なければ表示状態の最初のグループ
//  なければ 0
//---------------------------------------------------------------------
int FragSet::get_sel_group_ex()
{
	int gn = get_sel_group();
	if (gn==-1) {
		//グループリストで選択されているグループ
		if (IdeaFragMainForm->GroupListBox->Visible) {
			gn = IdeaFragMainForm->GroupListBox->ItemIndex; if (gn!=-1) gn++;
		}
	}
	if (gn==-1) {
		//表示状態の最初のグループ
		gn = 1;
		for (int i=0; i<=MAX_GROUP; i++) {
			if (!GInfs->Items[i].Visible) continue;
			gn = i + 1; break;
		}
	}
	if (gn==-1) gn = 0;
	return gn;
}

//---------------------------------------------------------------------
//指定ファイルにリンクしている断片を取得
//---------------------------------------------------------------------
Fragment *FragSet::get_linked_frag(UnicodeString fnam)
{
	if (fnam.IsEmpty()) return NULL;
	Fragment *rfp = NULL;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (fp->LinkName.IsEmpty()) continue;
		if (SameText(fnam, fp->LinkName)) {
			rfp = fp; break;
		}
	}
	return rfp;
}

//---------------------------------------------------------------------
//交叉している非選択断片を取得
//---------------------------------------------------------------------
int FragSet::get_intr_flist(Fragment *fp, TFragList *lst)
{
	if (!fp || !lst) return 0;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *dp = FrgList->Items[i];	if (dp==fp) continue;
		if (dp->Selected || !dp->Visible) continue;
		if (!fp->RC.IntersectsWith(dp->RC)) continue;
		if (lst->IndexOf(dp)==-1) lst->Add(dp);
	}
	return lst->Count;
}

//---------------------------------------------------------------------
//関係線でつながっている断片を取得
//---------------------------------------------------------------------
int FragSet::get_con_flist(Fragment *fp, TFragList *lst, bool sel_sw)
{
	if (!fp || !lst) return 0;
	if (fp->Visible && (!sel_sw || fp->Selected)) {
		lst->Add(fp);
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ap = CLineList->Items[i];
			if (ap->tType==TDIR_F2F) {
				Fragment *lfp = NULL;
				if		(ap->fromTag==fp->ID) lfp = FrgList->id_to_fp(ap->toTag);
				else if (ap->toTag==fp->ID)   lfp = FrgList->id_to_fp(ap->fromTag);
				if (lfp) {
					if (lst->IndexOf(lfp)==-1) get_con_flist(lfp, lst);
				}
			}
		}
	}

	return lst->Count;
}

//---------------------------------------------------------------------
//関係線でつながっている断片を選択
//---------------------------------------------------------------------
void FragSet::sel_connected(Fragment *fp)
{
	if (!fp) return;

	std::unique_ptr<TFragList> tmp_list(new TFragList());
	if (get_con_flist(fp, tmp_list.get())>0) {
		for (int i=0; i<tmp_list->Count; i++) tmp_list->Items[i]->Selected = true;
	}
	mak_sel_list(true);
}

//---------------------------------------------------------------------
//位置からトップの断片を取得
//---------------------------------------------------------------------
Fragment *FragSet::frag_from_pos(int X, int Y)
{
	TPoint p = Point(X, Y);
	int z = -1;
	Fragment *dp = NULL;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (!fp->Visible) continue;
		if (fp->style==fgsPlate && fp->OnGfrm) continue;
		if (!fp->vRC.PtInRect(p)) continue;
		if (fp->Z>z) dp = fp;
	}
	if (dp) return dp;

	//グループ枠上の表札か調べる
	if (EV->PlateOnGfrm) {
		for (int gn=1; gn<=MAX_GROUP; gn++) {
			TGrpFrame *sp = GInfs->Items[gn].gfp;
			if (!sp->Visible) continue;
			TRect prc = sp->PlateRect;
			prc.Offset(sp->Left, sp->Top);
			if (prc.PtInRect(p)) {
				dp = GInfs->Items[gn].Plate; break;
			}
		}
	}
	return dp;
}

//---------------------------------------------------------------------
//グループ関係線を設定
//---------------------------------------------------------------------
ArrowLine *FragSet::set_group_line(int gn0, int gn1)
{
	if (gn0<1 || gn0>MAX_GROUP) return NULL;
	if (gn1<1 || gn1>MAX_GROUP) return NULL;
	if (gn0==gn1) return NULL;

	ArrowLine *ap;
	bool gflag = false;
	for (int i=0; i<CLineList->Count; i++) {
		ap = CLineList->Items[i];
		if (ap->tType!=TDIR_G2G) continue;
		if ((ap->fromTag==gn0 && ap->toTag==gn1) || (ap->fromTag==gn1 && ap->toTag==gn0)) {
			//すでに関係線がある
			ap->erase(frg_owner);
			gflag = true;
			break;
		}
	}

	if (!gflag) ap = new ArrowLine();
	ap->tType	  = TDIR_G2G;
	ap->fromTag   = gn0;
	ap->toTag	  = gn1;
	ap->lType	  = EV->CurGLineType;
	ap->lWidth	  = EV->CurGLineWidth;
	ap->aType	  = EV->CurGArrowType;
	ap->aSize	  = EV->CurGArrowSize;
	ap->tOpposite = EV->CurTxOpposite;
	if (!gflag) CLineList->Add(ap);

	modify = true;
	return ap;
}

//---------------------------------------------------------------------
//断片関係線を設定
//---------------------------------------------------------------------
ArrowLine *FragSet::set_frag_line(int id0, int id1)
{
	int idx0 = FrgList->id_to_idx(id0);
	int idx1 = FrgList->id_to_idx(id1);
	if (idx0==-1 || idx1==-1 || idx0==idx1) return NULL;

	ArrowLine *ap;
	bool fflag = false;
	for (int i=0; i<CLineList->Count; i++) {
		ap = CLineList->Items[i];
		if (ap->tType!=TDIR_F2F) continue;
		if ((ap->fromTag==id0 && ap->toTag==id1) || (ap->fromTag==id1 && ap->toTag==id0)) {
			//すでに関係線がある
			ap->erase(frg_owner);
			fflag = true;
			break;
		}
	}

	if (!fflag) ap = new ArrowLine();
	ap->tType	  = TDIR_F2F;
	ap->fromTag   = id0;
	ap->toTag	  = id1;
	ap->lType	  = EV->CurFLineType;
	ap->lWidth	  = EV->CurFLineWidth;
	ap->aType	  = EV->CurFArrowType;
	ap->aSize	  = EV->CurFArrowSize;
	ap->tOpposite = EV->CurTxOpposite;
	if (!fflag)	CLineList->Add(ap);

	modify = true;
	return ap;
}
//---------------------------------------------------------------------
//断片・グループ間関係線を設定
//---------------------------------------------------------------------
ArrowLine *FragSet::set_fg_line(int tp, int id0, int id1)
{
	ArrowLine *ap;
	bool fflag = false;

	if (tp==TDIR_F2G) {
		if (FrgList->id_to_idx(id0)==-1) return NULL;
		if (id1<1 || id1>MAX_GROUP)		 return NULL;
	}
	else if (tp==TDIR_G2F) {
		if (id0<1 || id0>MAX_GROUP)		 return NULL;
		if (FrgList->id_to_idx(id1)==-1) return NULL;
	}
	else return NULL;

	//重複チェック
	for (int i=0; i<CLineList->Count; i++) {
		ap = CLineList->Items[i];
		if (ap->tType==TDIR_F2G || ap->tType==TDIR_G2F) {
			bool xflag;
			if (tp==ap->tType)
				xflag = (ap->fromTag==id0 && ap->toTag==id1);
			else
				xflag = (ap->fromTag==id1 && ap->toTag==id0);

			if (xflag) {
				//すでに関係線がある
				ap->erase(frg_owner);
				fflag = true;
				break;
			}
		}
	}

	if (!fflag) ap = new ArrowLine();
	ap->tType	  = tp;
	ap->fromTag   = id0;
	ap->toTag	  = id1;
	ap->lType	  = EV->CurFLineType;
	ap->lWidth	  = EV->CurFLineWidth;
	ap->aType	  = EV->CurFArrowType;
	ap->aSize	  = EV->CurFArrowSize;
	ap->tOpposite = EV->CurTxOpposite;
	if (!fflag) CLineList->Add(ap);

	modify = true;
	return ap;
}

//---------------------------------------------------------------------
//断片につながってる関係線の数を取得
//---------------------------------------------------------------------
int FragSet::has_lines(Fragment *fp)
{
	if (!fp) return 0;

	int n = 0;
	for (int i=0; i<CLineList->Count; i++) {
		ArrowLine *ap = CLineList->Items[i];
		bool cflag = false;
		if (ap->tType==TDIR_F2F) {
			if		(FrgList->id_to_fp(ap->fromTag)==fp) cflag = true;
			else if (FrgList->id_to_fp(ap->toTag)==fp)   cflag = true;
		}
		else if (ap->tType==TDIR_F2G || ap->tType==TDIR_G2F) {
			if (ap->tType==TDIR_F2G) {
				if (FrgList->id_to_fp(ap->fromTag)==fp) cflag = true;
			}
			else {
				if (FrgList->id_to_fp(ap->toTag)==fp)   cflag = true;
			}
		}
		if (cflag) n++;
	}
	return n;
}

//---------------------------------------------------------------------
//端点を作成
//---------------------------------------------------------------------
Fragment *FragSet::put_junction(int x, int y)
{
	Fragment *fp = new_frag();
	fp->style	 = fgsJunction;
	fp->wd		 = 8;
	fp->hi		 = 8;
	fp->RC.Left  = x;
	fp->RC.Top	 = y;
	fp->Selected = true;
	fp->Visible  = true;
	GInfs->gstk_from_pos(fp->RC.Left - HscrBar->Position, fp->RC.Top - VscrBar->Position, &fp->group);
	GInfs->Items[fp->group.stk[0]].Visible = true;
	return fp;
}

//---------------------------------------------------------------------
//選択中関係線に分割点を挿入
//---------------------------------------------------------------------
bool FragSet::add_junction()
{
	if (!CurLp) return false;

	//分割点を作成
	Fragment *fp = put_junction(
					CurLp->x0 + (CurLp->x1 - CurLp->x0)/2 - 4,
					CurLp->y0 + (CurLp->y1 - CurLp->y0)/2 - 4);
	if (!fp) return false;

	//分割線に新たな関係線を接続
	ArrowLine *ap0 = NULL;
	ArrowLine *ap1 = NULL;
	switch (CurLp->tType) {
	case TDIR_F2F:
		ap0 = set_frag_line(CurLp->fromTag, fp->ID);
		ap1 = set_frag_line(fp->ID, CurLp->toTag);
		break;
	case TDIR_G2G:
		ap0 = set_fg_line(TDIR_G2F, CurLp->fromTag, fp->ID);
		ap1 = set_fg_line(TDIR_F2G, fp->ID, CurLp->toTag);
		break;
	case TDIR_F2G:
		ap0 = set_frag_line(CurLp->fromTag, fp->ID);
		ap1 = set_fg_line(TDIR_F2G, fp->ID, CurLp->toTag);
		break;
	case TDIR_G2F:
		ap0 = set_fg_line(TDIR_G2F, CurLp->fromTag, fp->ID);
		ap1 = set_frag_line(fp->ID, CurLp->toTag);
		break;
	}

	//関係線の属性を設定
	if (ap0 && ap1) {
		switch (CurLp->lType) {
		case LTYPE_LINE: case LTYPE_D_LINE:
			ap0->lType = ap1->lType = CurLp->lType;
			break;
		case LTYPE_S_ARROW: case LTYPE_C_ARROW:
			ap0->lType = LTYPE_LINE;
			ap1->lType = CurLp->lType;
			break;
		case LTYPE_D_ARROW:
			ap0->lType = ap1->lType = LTYPE_S_ARROW;
			ap0->reverse();
			break;
		case LTYPE_CD_ARROW:
			ap0->lType = ap1->lType = LTYPE_C_ARROW;
			ap0->reverse();
			break;
		}
		ap0->lWidth = ap1->lWidth = CurLp->lWidth;
		ap0->aType = ap1->aType = CurLp->aType;
		ap0->aSize = ap1->aSize = CurLp->aSize;
		ap0->lColor = ap1->lColor = CurLp->lColor;
		ap1->TextStr   = CurLp->TextStr;
		ap1->tOpposite = CurLp->tOpposite;
	}

	//以前の関係線を削除
	CurLp->erase(frg_owner);
	int idx = CLineList->IndexOf(CurLp);
	delete CurLp; 	CurLp = NULL;
	if (idx!=-1) CLineList->Delete(idx);

	mak_sel_list();
	update_grp_empty();
	modify = true;
	return true;
}

//---------------------------------------------------------------------
//孤立した分割点を削除
//---------------------------------------------------------------------
void FragSet::clr_iso_junction()
{
	int i = 0;
	int n = 0;
	while (i<FrgList->Count) {
		Fragment *fp = FrgList->Items[i];
		if (fp->style==fgsJunction) {
			if (has_lines(fp)==0) {
				del_frag(fp);
				FrgList->Delete(i);
				n++;
			}
			else i++;
		}
		else i++;
	}

	if (n>0) {
		mak_sel_list();
		update_grp_empty();
		modify		  = true;
		EV->reqUpdate = true;
	}
}

//---------------------------------------------------------------------
//グループ交換
//---------------------------------------------------------------------
void FragSet::exchange_group(int gn0, int gn1)
{
	if (gn0<0 || gn0>MAX_GROUP) return;
	if (gn1<0 || gn1>MAX_GROUP) return;

	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		for (int j=0; j<MAX_IN_GRP; j++) {
			if		(fp->group.stk[j]==gn0) fp->group.stk[j] = gn1;
			else if (fp->group.stk[j]==gn1) fp->group.stk[j] = gn0;
		}
	}

	//グループ関係線、断片・グループ間関係線を更新
	for (int i=0; i<CLineList->Count; i++) {
		ArrowLine *ap = CLineList->Items[i];
		if (ap->tType==TDIR_G2G) {
			if		(ap->fromTag==gn0) ap->fromTag  = gn1;
			else if (ap->fromTag==gn1) ap->fromTag  = gn0;
			if		(ap->toTag==gn0)   ap->toTag    = gn1;
			else if (ap->toTag==gn1)   ap->toTag    = gn0;
		}
		else if (ap->tType==TDIR_F2G) {
			if		(ap->toTag==gn0)   ap->toTag   = gn1;
			else if (ap->toTag==gn1)   ap->toTag   = gn0;
		}
		else if (ap->tType==TDIR_G2F) {
			if		(ap->fromTag==gn0) ap->fromTag = gn1;
			else if (ap->fromTag==gn1) ap->fromTag = gn0;
		}
	}

	grp_inf tmpinf = GInfs->Items[gn0];
	GInfs->Items[gn0]    = GInfs->Items[gn1];
	GInfs->Items[gn1]    = tmpinf;
	update_grp_empty();
	modify = true;
}
//---------------------------------------------------------------------
//選択中の断片を他グループへ移動
//---------------------------------------------------------------------
void FragSet::mov_sel_group(
	int gn,
	bool add_sw)	//多重追加
{
	if (gn<0 || gn>MAX_GROUP) return;

	push_all();

	Fragment *np = GInfs->get_name_plate(gn, NULL);

	//移動前のグループ毎の断片数を数える
	int sgcnt[MAX_GROUP + 1];
	for (int i=0; i<=MAX_GROUP; i++) sgcnt[i] = 0;
	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		for (int j=0; j<MAX_IN_GRP; j++) {
			if (fp->group.stk[j]==-1) break;
			sgcnt[fp->group.stk[j]]++;
		}
	}
	//移動
	bool t_flag = false;
	int sgn = get_sel_group();
	if (sgn!=-1) {
		t_flag = (SelList->Count < GInfs->Items[sgn].TotalSize);
	}
	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		if (!fp->set_group(gn, add_sw, t_flag)) continue;
		if (fp->style==fgsPlate) {
			if (gn==0 || np) {
				fp->era_frag();
				fp->style = fgsNormal; //表札解除
			}
		}
		fp->Changed = true;
	}
	update_grp_empty();

	if (add_sw) {
		//多重化の場合、面積順で所属グループを並び替え
		GInfs->calc_frame(gn);
		for (int i=0; i<SelList->Count; i++) {
			Fragment *fp = SelList->Items[i];
			grp_stk *sp = &fp->group;
			for (int j=0; j<MAX_IN_GRP-1; j++) {
				if (sp->stk[j]==-1) break;
				for (int k=j+1; k<MAX_IN_GRP; k++) {
					if (sp->stk[k]==-1) break;
					if (GInfs->Items[sp->stk[k]].Area < GInfs->Items[sp->stk[j]].Area) {
						 std::swap(sp->stk[k], sp->stk[j]);
					}
				}
			}
		}
		GInfs->update_empty();
		frg_owner->Invalidate();
	}

	//移動して空グループになったら非表示に
	for (int i=0; i<=MAX_GROUP; i++) {
		grp_inf *gp = &GInfs->Items[i];
		if (sgcnt[i]>0 && gp->TotalSize==0) gp->Visible = false;
	}

	if (EV->ShowDstGrp) GInfs->Items[gn].Visible = true;

	modify		  = true;
	EV->reqUpdate = true;
}

//---------------------------------------------------------------------
//指定グループを解除
//---------------------------------------------------------------------
void FragSet::rel_group(int gn)
{
	push_all();
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		int g0 = fp->group.stk[0];
		int n = fp->rel_group(gn);
		if (n==-1) continue;
		if (fp->style==fgsPlate && gn==g0) {
		//そのグループの表札は解除
			fp->style	= fgsNormal;
			fp->Changed = true;
		}
	}
	update_grp_empty();
	modify = true;
}

//---------------------------------------------------------------------
//画面分割線を描画
//---------------------------------------------------------------------
void FragSet::draw_divln(
	TCanvas *cv,
	int w, int h,	//実サイズ
	double ratio)	//出力倍率 default = 1.0
{
	if (!EV->show_DivLine || div_mod==0) return;

	cv->Pen->Color
		= (cv==Printer()->Canvas && EV->PrintMono)? clBlack : GET_indcol(col_DivLine);
	cv->Pen->Style = psSolid;

	if (cv==Printer()->Canvas && EV->FixPrnLWidth>0)
		cv->Pen->Width = (div_lw>1)? (std::max(EV->FixPrnLWidth, (int)(div_lw * ratio))) : EV->FixPrnLWidth;
	else
		cv->Pen->Width = std::max(1, (int)(div_lw * ratio));

	double fw = w * ratio;
	double fh = h * ratio;
	int iw = (int)fw;
	int ih = (int)fh;

	int xp, yp, dx, dy, xn, yn, xn2, yn2;
	double fx, fy;
	switch (div_mod) {
	case DVMODE_DVN:	//縦横(分割)
		//縦線
		if (div_x>1) {
			dx = iw/div_x;
			xn = div_x;
			if (cv==frg_owner->Canvas) {
				xp = dx - HscrBar->Position;
				for (int i=1; i<xn; i++, xp += dx) {
					cv->MoveTo(xp, 0);  cv->LineTo(xp, ih);
				}
			}
			else if (dx>2) {
				for (int i=1; i<xn; i++) {
					xp = iw*i/xn;
					cv->MoveTo(xp, 0);  cv->LineTo(xp, ih);
				}
			}
		}
		//横線
		if (div_y>1) {
			dy = ih/div_y;
			yn = div_y;
			if (cv==frg_owner->Canvas) {
				yp = dy - VscrBar->Position;
				for (int i=1; i<yn; i++, yp += dy) {
					cv->MoveTo(0, yp);  cv->LineTo(iw, yp);
				}
			}
			else if (dy>2) {
				for (int i=1; i<yn; i++) {
					yp = ih*i/yn;
					cv->MoveTo(0, yp);  cv->LineTo(iw, yp);
				}
			}
		}
		break;

	case DVMODE_PXN:	//縦横(ピクセル)
		//縦線
		if (div_x>1) {
			dx = (int)(div_x*ratio);
			if (cv==frg_owner->Canvas) {
				xp = dx - HscrBar->Position;
				for (int i=1; true; i++, xp+=dx) {
					if (xp>iw) break;
					cv->MoveTo(xp, 0);  cv->LineTo(xp, ih);
				}
			}
			else if (dx>2) {
				for (int i=1; true; i++) {
					xp = (int)(i*div_x*ratio); if (xp>iw) break;
					cv->MoveTo(xp, 0);  cv->LineTo(xp, ih);
				}
			}
		}
		//横線
		if (div_y>1) {
			dy = (int)(div_y*ratio);
			if (cv==frg_owner->Canvas) {
				yp = dy - VscrBar->Position;
				for (int i=1; true; i++, yp+=dy) {
					if (yp>ih) break;
					cv->MoveTo(0, yp);  cv->LineTo(iw, yp);
				}
			}
			else if (dy>2) {
				for (int i=1; true; i++) {
					yp = (int)(i*div_y*ratio);  if (yp>ih) break;
					cv->MoveTo(0, yp);  cv->LineTo(iw, yp);
				}
			}
		}
		break;

	//三角形タイル
	case DVMODE_TRH: case DVMODE_TRV:
		if (div_x>1) {
			int pw0 = div_x;
			int pw1 = pw0/2;
			int pw2 = (int)(0.86602540*div_x);
			if (div_mod==DVMODE_TRH) {
				yn = h/pw2;  xn = w/pw0;
			}
			else {
				xn = w/pw2;  yn = h/pw0;
			}
			TPoint p[4];
			if (cv==frg_owner->Canvas) {
				//水平方向
				if (div_mod==DVMODE_TRH) {
					for (int i=0; i<yn; i++) {
						int y0 = i*pw2;
						int ry0 = y0-VscrBar->Position;
						if (ry0 < -pw2) continue;
						if (ry0 > frg_owner->ClientHeight) break;
						for (int j=0; j<xn; j++) {
							int x0 = j*pw0;
							int rx0 = x0-HscrBar->Position;
							if (rx0< -pw0) continue;
							if ((rx0-pw1) > frg_owner->ClientWidth) break;
							if (i%2==0) {
								p[0] = Point(rx0 + pw1,	ry0);
								p[1] = Point(rx0,		ry0 + pw2);
								p[2] = Point(rx0 + pw0,	ry0 + pw2);
								p[3] = p[0];
								cv->Polyline(p, 3);
								if (i==0 && j>0) {
									cv->MoveTo(p[0].x,		 p[0].y);
									cv->LineTo(p[0].x - pw0, p[0].y);
								}
								if (j==xn-1 && (x0 + pw0 + pw1)<iw) {
									cv->MoveTo(p[0].x,		 p[0].y);
									cv->LineTo(p[0].x + pw0, p[0].y);
									cv->LineTo(p[2].x,		 p[2].y);
								}
							}
							else {
								p[0] = Point(rx0,		ry0);
								p[1] = Point(rx0 + pw1,	ry0 + pw2);
								p[2] = Point(rx0 + pw0,	ry0);
								p[3] = p[0];
								cv->Polyline(p, 2);
								if (j>0) {
									cv->MoveTo(p[1].x,		 p[1].y);
									cv->LineTo(p[1].x - pw0, p[1].y);
								}
								if (j==xn-1 && (x0 + pw0 + pw1)<w) {
									cv->MoveTo(p[2].x, 		 p[2].y);
									cv->LineTo(p[1].x + pw0, p[1].y);
									if (i==yn-1) cv->LineTo(p[1].x, p[1].y);
								}
							}
						}
					}

				}
				//垂直方向
				else {
					for (int i=0; i<xn; i++) {
						int x0 = i*pw2;
						int rx0 = x0-HscrBar->Position;
						if (rx0 < -pw2) continue;
						if (rx0 > frg_owner->ClientWidth) break;
						for (int j=0; j<yn; j++) {
							int y0 = j*pw0;
							int ry0 = y0-VscrBar->Position;
							if (ry0< -pw0) continue;
							if ((ry0-pw1) > frg_owner->ClientHeight) break;
							if (i%2==0) {
								p[0] = Point(rx0,		ry0 + pw1);
								p[1] = Point(rx0 + pw2,	ry0);
								p[2] = Point(rx0 + pw2,	ry0 + pw0);
								p[3] = p[0];
								cv->Polyline(p, 3);
								if (i==0 && j>0) {
									cv->MoveTo(p[0].x,	p[0].y);
									cv->LineTo(p[0].x,	p[0].y - pw0);
								}
								if (j==yn-1 && (y0 + pw0 + pw1)<h) {
									cv->MoveTo(p[0].x,	p[0].y);
									cv->LineTo(p[0].x,	p[0].y + pw0);
									cv->LineTo(p[2].x,	p[2].y);
								}
							}
							else {
								p[0] = Point(rx0,		ry0);
								p[1] = Point(rx0 + pw2,	ry0 + pw1);
								p[2] = Point(rx0,		ry0 + pw0);
								p[3] = p[0];
								cv->Polyline(p, 2);
								if (j>0) {
									cv->MoveTo(p[1].x, p[1].y);
									cv->LineTo(p[1].x, p[1].y - pw0);
								}
								if (j==yn-1 && (y0 + pw0 + pw1)<h) {
									cv->MoveTo(p[2].x, p[2].y);
									cv->LineTo(p[1].x, p[1].y + pw0);
									if (i==xn-1) cv->LineTo(p[1].x, p[1].y);
								}
							}
						}
					}
				}
			}
			else {
				int rpw0 = (int)(pw0*ratio);
				int	rpw1 = (int)(pw1*ratio);
				int	rpw2 = (int)(pw2*ratio);
				int rpw3 = (int)((pw0 + pw1)*ratio);
				//水平方向
				if (div_mod==DVMODE_TRH) {
					for (int i=0; i<yn; i++) {
						int ry0 = (int)(i*pw2*ratio);
						for (int j=0; j<xn; j++) {
							int x0 = j*pw0;
							int rx0 = (int)(x0*ratio);
							if (i%2==0) {
								p[0] = Point(rx0 + rpw1, ry0);
								p[1] = Point(rx0,		 ry0 + rpw2);
								p[2] = Point(rx0 + rpw0, ry0 + rpw2);
								p[3] = p[0];
								cv->Polyline(p, 3);
								if (i==0 && j>0) {
									cv->MoveTo(p[0].x,		  p[0].y);
									cv->LineTo(p[0].x - rpw0, p[0].y);
								}
								if (j==xn-1 && (x0 + pw0 + pw1)<w) {
									cv->MoveTo(p[0].x,		  p[0].y);
									cv->LineTo(p[0].x + rpw0, p[0].y);
									cv->LineTo(p[2].x,		  p[2].y);
								}
							}
							else {
								p[0] = Point(rx0,		 ry0);
								p[1] = Point(rx0 + rpw1, ry0 + rpw2);
								p[2] = Point(rx0  +rpw0, ry0);
								p[3] = p[0];
								cv->Polyline(p, 2);
								if (j>0) {
									cv->MoveTo(p[1].x, 		  p[1].y);
									cv->LineTo(p[1].x - rpw0, p[1].y);
								}
								if (j==xn-1 && (x0 + pw0 + pw1)<w) {
									cv->MoveTo(p[2].x, 		  p[2].y);
									cv->LineTo(p[1].x + rpw0, p[1].y);
									if (i==yn-1) cv->LineTo(p[1].x, p[1].y);
								}
							}
						}
					}

				}
				//垂直方向
				else {
					for (int i=0; i<xn; i++) {
						int rx0 = (int)(i*pw2*ratio);
						for (int j=0; j<yn; j++) {
							int y0  = j*pw0;
							int ry0 = (int)(y0*ratio);
							if (i%2==0) {
								p[0] = Point(rx0,		 ry0 + rpw1);
								p[1] = Point(rx0 + rpw2, ry0);
								p[2] = Point(rx0 + rpw2, ry0 + rpw0);
								p[3] = p[0];
								cv->Polyline(p, 3);
								if (i==0 && j>0) {
									cv->MoveTo(p[0].x,	p[0].y);
									cv->LineTo(p[0].x,	p[0].y - rpw0);
								}
								if (j==yn-1 && (ry0 + rpw3)<ih) {
									cv->MoveTo(p[0].x,	p[0].y);
									cv->LineTo(p[0].x,	p[0].y + rpw0);
									cv->LineTo(p[2].x,	p[2].y);
								}
							}
							else {
								p[0] = Point(rx0,		 ry0);
								p[1] = Point(rx0 + rpw2, ry0 + rpw1);
								p[2] = Point(rx0,		 ry0 + rpw0);
								p[3] = p[0];
								cv->Polyline(p, 2);
								if (j>0) {
									cv->MoveTo(p[1].x, p[1].y);
									cv->LineTo(p[1].x, p[1].y - rpw0);
								}
								if (j==yn-1 && (y0 + pw0 + pw1)<h) {
									cv->MoveTo(p[2].x, p[2].y);
									cv->LineTo(p[1].x, p[1].y + rpw0);
									if (i==xn-1) cv->LineTo(p[1].x, p[1].y);
								}
							}
						}
					}
				}
			}
		}
		break;

	//六角形タイル
	case DVMODE_HXH: case DVMODE_HXV:
		if (div_x>1) {
			int pw0 = div_x;
			int pw1 = pw0/2;
			int pw2 = (int)(0.86602540*div_x);
			int pw3 = pw2*2;
			int pw4 = pw1 + pw0;
			int pw5 = pw0*2;
			if (div_mod==DVMODE_HXH) {
				yn = h/pw3;
				xn = (w - pw1)/pw4;
			}
			else {
				xn = w/pw3;
				yn = (h - pw1)/pw4;
			}
			TPoint p[8];
			if (cv==frg_owner->Canvas) {
				//水平方向
				if (div_mod==DVMODE_HXH) {
					for (int j=0; j<xn; j++) {
						int x0	= j*pw4;
						int rx0 = x0 - HscrBar->Position;
						if (rx0 < -pw4) continue;
						if (rx0 > frg_owner->ClientWidth) break;
						for (int i=0; i<yn; i++) {
							int y0	= (j%2)*pw2 + i*pw3;
							int ry0 = y0 - VscrBar->Position;
							if (ry0 < -pw3) continue;
							if (ry0 > frg_owner->ClientHeight) break;
							if (y0+pw3 > h) break;
							p[0] = Point(rx0 + pw5,		  ry0 + pw2);
							p[1] = Point(rx0 + pw1 + pw0, ry0);
							p[2] = Point(rx0 + pw1,		  ry0);
							p[3] = Point(rx0,			  ry0 + pw2);
							p[4] = Point(rx0 + pw1,		  ry0 + pw3);
							p[5] = Point(rx0 + pw1 + pw0, ry0 + pw3);
							p[6] = p[0];
							p[7] = p[1];
							int pn;
							TPoint *pp;
							if (j==xn-1 && i==0) {
								pn = 6;  pp = &p[0];
							}
							else if (j==xn-1 || (i==0 && j%2==0)) {
								pn = 5;  pp = (i==0)? &p[0] : &p[2];
							}
							else if (i==0 || i==yn-1) {
								pn = 4;  pp = (i==yn-1)? &p[2] : &p[1];
							}
							else {
								pn = 3;  pp = &p[2];
							}
							cv->Polyline(pp, pn);
						}
					}
				}
				//垂直方向
				else {
					for (int j=0; j<yn; j++) {
						int y0	= j*pw4;
						int ry0 = y0-VscrBar->Position;
						if (ry0 < -pw4) continue;
						if (ry0 > frg_owner->ClientHeight) break;
						for (int i=0; i<xn; i++) {
							int x0	= (j%2)*pw2 + i*pw3;
							int rx0 = x0-HscrBar->Position;
							if (rx0 < -pw3) continue;
							if (rx0 > frg_owner->ClientWidth) break;
							if (x0+pw3 > w) break;
							p[0] = Point(rx0 + pw2,	ry0 + pw5);
							p[1] = Point(rx0,		ry0 + pw1 + pw0);
							p[2] = Point(rx0, 		ry0 + pw1);
							p[3] = Point(rx0 + pw2,	ry0);
							p[4] = Point(rx0 + pw3,	ry0 + pw1);
							p[5] = Point(rx0 + pw3,	ry0 + pw1 + pw0);
							p[6] = p[0];
							p[7] = p[1];
							int pn;
							TPoint *pp;
							if (j==yn-1 && i==0) {
								pn = 6;  pp = &p[0];
							}
							else if (j==yn-1 || (i==0 && j%2==0)) {
								pn = 5;  pp = (i==0)? &p[0] : &p[2];
							}
							else if (i==0 || i==xn-1) {
								pn = 4;  pp = (i==xn-1)? &p[2] : &p[1];
							}
							else {
								pn = 3;  pp = &p[2];
							}
							cv->Polyline(pp, pn);
						}
					}
				}
			}
			else {
				int rpw0 = (int)(pw0*ratio);
				int	rpw1 = (int)(pw1*ratio);
				int	rpw2 = (int)(pw2*ratio);
				int	rpw3 = (int)(pw3*ratio);
				int	rpw5 = (int)(pw5*ratio);
				//水平方向
				if (div_mod==DVMODE_HXH) {
					for (int j=0; j<xn; j++) {
						int rx0 = j * (int)(pw4*ratio);
						for (int i=0; i<yn; i++) {
							int y0	= (j%2)*pw2 + i*pw3;
							int ry0 = (int)(y0*ratio);
							if (y0+pw3 > h) break;
							p[0] = Point(rx0 + rpw5,		ry0 + rpw2);
							p[1] = Point(rx0 + rpw1 + rpw0,	ry0);
							p[2] = Point(rx0 + rpw1,		ry0);
							p[3] = Point(rx0,				ry0 + rpw2);
							p[4] = Point(rx0 + rpw1,		ry0 + rpw3);
							p[5] = Point(rx0 + rpw1 + rpw0,	ry0 + rpw3);
							p[6] = p[0];
							p[7] = p[1];
							int pn;
							TPoint *pp;
							if (j==xn-1 && i==0) {
								pn = 6;  pp = &p[0];
							}
							else if (j==xn-1 || (i==0 && j%2==0)) {
								pn = 5;  pp = (i==0)? &p[0] : &p[2];
							}
							else if (i==0 || i==yn-1) {
								pn = 4;  pp = (i==yn-1)? &p[2] : &p[1];
							}
							else {
								pn = 3;  pp = &p[2];
							}
							cv->Polyline(pp, pn);
						}
					}
				}
				//垂直方向
				else {
					for (int j=0; j<yn; j++) {
						int ry0 = j * (int)(pw4*ratio);
						for (int i=0; i<xn; i++) {
							int x0	= ((j%2)*pw2 + i*pw3);
							int rx0 = (int)(x0 * ratio);
							if (x0+pw3 > w) break;
							p[0] = Point(rx0 + rpw2, ry0 + rpw5);
							p[1] = Point(rx0,		 ry0 + rpw1 + rpw0);
							p[2] = Point(rx0,		 ry0 + rpw1);
							p[3] = Point(rx0 + rpw2, ry0);
							p[4] = Point(rx0 + rpw3, ry0 + rpw1);
							p[5] = Point(rx0 + rpw3, ry0 + rpw1 + rpw0);
							p[6] = p[0];
							p[7] = p[1];
							int pn;
							TPoint *pp;
							if (j==yn-1 && i==0) {
								pn = 6;  pp = &p[0];
							}
							else if (j==yn-1 || (i==0 && j%2==0)) {
								pn = 5;  pp = (i==0)? &p[0] : &p[2];
							}
							else if (i==0 || i==xn-1) {
								pn = 4;  pp = (i==xn-1)? &p[2] : &p[1];
							}
							else {
								pn = 3;  pp = &p[2];
							}
							cv->Polyline(pp, pn);
						}
					}
				}
			}
		}
		break;
	}
}

//---------------------------------------------------------------------
//配置グリッドを描画
//---------------------------------------------------------------------
void FragSet::draw_grid()
{
	if (inh_Grid) {	//描画の抑止
		inh_Grid = false;
		return;
	}

	if (!EV->show_PosGrid) return;

	int gw = EV->GridSize;
	int x0 = gw - HscrBar->Position % gw;
	int y0 = gw - VscrBar->Position % gw;
	int x_sz = frg_owner->ClientWidth;
	int y_sz = frg_owner->ClientHeight;
	TColor col = EV->col_PosGrid;

	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	bmp->Width	= frg_owner->ClientWidth;
	bmp->Height = frg_owner->ClientHeight;
	bmp->Canvas->CopyRect(frg_owner->ClientRect, frg_owner->Canvas, frg_owner->ClientRect);

	for (int xp=x0; xp<x_sz; xp+=gw)
		for (int yp=y0; yp<y_sz; yp+=gw)
			bmp->Canvas->Pixels[xp][yp] = col;

	frg_owner->Canvas->CopyRect(frg_owner->ClientRect, bmp->Canvas, frg_owner->ClientRect);
}

//---------------------------------------------------------------------
//タイトルを描画
//---------------------------------------------------------------------
void FragSet::draw_title(
	TCanvas *cv,
	double ratio,	//倍率 =0: 描画(画面相対)/  ratio>0: 印刷／画像出力(絶対)
	int w, int h)
{
	if (title_pos!=0 && !data_title.IsEmpty()) {
		cv->Font->Assign(titFont);
		bool vflag = (titFont->Name[1]=='@');

		if (ratio!=0) cv->Font->Height = (int)(ratio * titFont->Height);

		int tw = cv->TextWidth(data_title);
		int th = cv->TextHeight(" ");
		int mg = th/2;
		int xp = mg;
		int yp = mg;
		if (vflag) std::swap(th, tw);

		switch (title_pos) {
		case 1: case 4: case 7: xp = mg;		  break;
		case 2: case 5: case 8: xp = (w - tw)/2;  break;
		case 3: case 6: case 9: xp = w - tw - mg; break;
		}
		switch (title_pos) {
		case 1: case 2: case 3: yp = mg;		  break;
		case 4: case 5: case 6: yp = (h - th)/2;  break;
		case 7: case 8: case 9: yp = h - th - mg; break;
		}

		if (ratio==0) {
			xp -= HscrBar->Position;
			yp -= VscrBar->Position;
		}

		cv->Font->Color
			= (cv==Printer()->Canvas && EV->PrintMono)? clBlack : GET_indcol(col_Title);
		cv->Brush->Style = bsClear;

		if (vflag)
			vert_textout(cv, xp + tw, yp, data_title);
		else
			cv->TextOut(xp, yp, data_title);
	}
}

//---------------------------------------------------------------------
//非選択標識・分割点の輪郭を消す
//---------------------------------------------------------------------
bool FragSet::erase_sym_frm()
{
	bool ret = false;
	TCanvas *cv = frg_owner->Canvas;
	cv->Brush->Color = frg_owner->Color;

	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (!fp->Visible) continue;
		if (fp->style!=fgsSymbol && fp->style!=fgsJunction) continue;
		if (fp->Selected) continue;
		TRect s_rc = fp->vRC;
		InflateRect(&s_rc, -1, -1);
		cv->FrameRect(fp->vRC);
		if (fp->style!=fgsJunction) cv->FrameRect(s_rc);
		ret = true;
	}

	return ret;
}

//---------------------------------------------------------------------
//表示中の断片のみ全て更新
//---------------------------------------------------------------------
void FragSet::visible_update()
{
	CLineList->erase_line();

	//画面分割線を描画
	draw_divln(frg_owner->Canvas, scr_wd, scr_hi);

	//グリッドを描画
	draw_grid();

	//タイトルを表示
	draw_title(frg_owner->Canvas, 0, scr_wd, scr_hi);

	//断片を更新
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (!fp->Selected || (EV->ImageIsBack && fp->style==fgsImage)) fp->show_frag();
	}

	//選択断片を更新
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (fp->Selected && (!EV->ImageIsBack || fp->style!=fgsImage))
			fp->show_frag();
	}

	update_frg_line();
	update_group_frame();
	update_fg_line();

	//全体図更新
	MapViewer->MapUpdate(NULL);
	//断片リスト更新
	FragGrid->GridUpdate();
}

//---------------------------------------------------------------------
//全て断片を更新
//---------------------------------------------------------------------
void FragSet::all_update(bool upd_img)
{
	Fragment *fp;

	//表札をトップに
	for (int i=0; i<MAX_GROUP; i++) bring_to_front(GInfs->get_name_plate(i + 1, NULL));
	FrgList->Sort(FrgCmp_Z);

	//表示状態更新
	for (int i=0; i<FrgList->Count; i++) {
		fp = FrgList->Items[i];
		if (upd_img) fp->upd_link();

		bool flag = false;
		for (int j=0; j<MAX_IN_GRP; j++) {
			if (fp->group.stk[j]==-1) break;
			if (GInfs->Items[fp->group.stk[j]].Visible) { flag = true; break; }
		}
		bool lastV = fp->Visible;
		//表札
		if (fp->style==fgsPlate) {
			if (EV->show_NamePlate)
				fp->Visible = EV->AlwaysShowNp? true : flag;
			else
				fp->Visible = false;

		}
		//断片
		else
			fp->Visible = flag;
		if (lastV != fp->Visible) fp->Changed = true;
	}

	//消す
	for (int i=0; i<FrgList->Count; i++) {
		fp = FrgList->Items[i];	if (fp->Visible) continue;
		fp->era_frag();
		fp->Selected = false;
		int idx = SelList->IndexOf(fp);
		if (idx>=0) SelList->Delete(idx);
	}

	//表示
	erase_sym_frm();
	visible_update();
}

//---------------------------------------------------------------------
//全情報を待避
//---------------------------------------------------------------------
void FragSet::push_all()
{
	UndoPtr = (UndoPtr + 1) % MAX_UNDO;
	FragBuffer *bp = UndoBuf[UndoPtr];

	//断片
	bp->FrgList->del_all();
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *dp = bp->new_frag(false);
		Fragment *fp = FrgList->Items[i];
		fp->tmpRC = fp->RC;
		dp->assign(fp, true, true);
	}
	//関係線
	bp->CLineList->new_copy(CLineList);

	//画面サイズ
	bp->scr_wd   = scr_wd;
	bp->scr_hi   = scr_hi;
	bp->LastHPos = HscrBar->Position;
	bp->LastVPos = VscrBar->Position;

	//グループ情報
	for (int i=0; i<=MAX_GROUP; i++) bp->GinfItem[i] = GInfs->Items[i];

	if (UndoN<MAX_UNDO) UndoN++;
	RedoN = 0;
}
//---------------------------------------------------------------------
//全情報を復元 (現在の状態と交換)
//---------------------------------------------------------------------
void FragSet::pop_all(int idx)
{
	FragBuffer *undo_bp = UndoBuf[idx];

	//画面サイズ
	std::swap(scr_wd, undo_bp->scr_wd);
	std::swap(scr_hi, undo_bp->scr_hi);
	VscrBar->Range = scr_hi + 20;
	HscrBar->Range = scr_wd + 20;
	std::swap(LastHPos, undo_bp->LastHPos);
	std::swap(LastVPos, undo_bp->LastVPos);
	HscrBar->Position = LastHPos;
	VscrBar->Position = LastVPos;

	//断片
	std::unique_ptr<FragBuffer> tmp_bp(new FragBuffer());
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *dp = tmp_bp->new_frag(false);
		dp->assign(FrgList->Items[i], true, true);
	}
	FrgList->del_all();
	for (int i=0; i<undo_bp->FrgList->Count; i++) {
		Fragment *fp = new_frag(false);
		fp->assign(undo_bp->FrgList->Items[i], true, true);
		fp->upd_link();
	}
	undo_bp->FrgList->del_all();
	for (int i=0; i<tmp_bp->FrgList->Count; i++) {
		Fragment *dp = undo_bp->new_frag(false);
		dp->assign(tmp_bp->FrgList->Items[i], true, true);
	}

	//関係線
	tmp_bp->CLineList->new_copy(CLineList);
	CLineList->new_copy(undo_bp->CLineList);
	undo_bp->CLineList->new_copy(tmp_bp->CLineList);

	//グループ情報
	for (int i=0; i<=MAX_GROUP; i++) std::swap(undo_bp->GinfItem[i], GInfs->Items[i]);

	update_grp_empty();
	mak_sel_list();

	frg_owner->Invalidate();
	IdeaFragMainForm->GroupListBox->Repaint();
}

//---------------------------------------------------------------------
//断片位置・グループのアンドゥ(一回のみ削除のアンドゥ)
//---------------------------------------------------------------------
void FragSet::undo()
{
	if (UndoN==0) return;

	pop_all(UndoPtr);

	if (UndoPtr>0) UndoPtr--; else UndoPtr = MAX_UNDO - 1;
	UndoN--;
	if (RedoN<MAX_UNDO) RedoN++;
}
//---------------------------------------------------------------------
//断片位置のリドゥ
//---------------------------------------------------------------------
void FragSet::redo()
{
	if (RedoN==0) return;
	UndoPtr = (UndoPtr + 1) % MAX_UNDO;

	pop_all(UndoPtr);

	if (UndoN<MAX_UNDO) UndoN++;
	RedoN--;
}

//---------------------------------------------------------------------------
//タイトルバーの設定
//---------------------------------------------------------------------
void FragSet::set_titlebar()
{
	UnicodeString tmpstr =  !data_title.IsEmpty()? data_title : ReplaceStr(file_name, "\\", "/");
	int fatr = FileGetAttr(file_name);
	if (fatr!=-1) read_only = ((fatr & faReadOnly) != 0);
	if (read_only) tmpstr.UCAT_TSTR(" : 編集禁止");
	frg_owner->Caption = tmpstr;
}


//---------------------------------------------------------------------------
//ファイル処理
//---------------------------------------------------------------------------
//---------------------------------------------------------------------
//読み込み/マージ
//  戻り値: 断片数
//---------------------------------------------------------------------
int FragSet::load_file(
	UnicodeString fnam,		//ファイル名
	bool mg_sw)				//false = 読み込み/ true = マージ
{
	std::unique_ptr<TStringList> fbuf(new TStringList());
	UnicodeString tmpstr, kstr, vstr;

	Fragment *fp;
	TColor cl_lc;
	UnicodeString cl_str;
	bool cl_op;
	int  gn = -1;
	int  vn;
	TColor vc;

	scr_wd	 = EV->DefScrWd;
	scr_hi	 = EV->DefScrHi;
	LastHPos = LastVPos = 0;
	LastAnc  = EmptyStr;

	try {
		if (test_ideafrag2(fnam)) fbuf->LoadFromFile(fnam);

		int dmod = DSMODE_NON;
		if (fbuf->Count>0 && !mg_sw) {
			for (int i=0; i<CLineList->Count; i++) delete CLineList->Items[i];
			CLineList->Clear();
		}

		int cl_md, cl_to, cl_fr, cl_tp, cl_lw, cl_at, cl_as;

		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString s = fbuf->Strings[i];  if (s.IsEmpty()) continue;
			int l = s.Length();
			//セクション
			if (s[1] == '[' && s[l] == ']') {
				tmpstr = s.SubString(2, l - 2);
				if (SameText(tmpstr, "General"))
					dmod = DSMODE_GEN;
				else if (SameText(tmpstr, "Fragment"))
					dmod = DSMODE_FRG;
				else if (SameText(tmpstr, "Connection") && !mg_sw) {
					dmod  = DSMODE_CON;
					cl_md = TDIR_NONE;
				}
				else if (SameText(tmpstr, "Group") && !mg_sw)
					dmod = DSMODE_GRP;
				else if (SameText(tmpstr, "Option"))
					dmod = DSMODE_OPT;
				else
					dmod = DSMODE_NON;
			}
			//データ
			else {
				if (dmod==DSMODE_FRG) {
					if (s[1]=='>') {
						fp = new_frag(false);
						s.Delete(1, 1);
						if		(SameStr(s, "PLT")) fp->style = fgsPlate;
						else if (SameStr(s, "SYM")) fp->style = fgsSymbol;
						else if (SameStr(s, "IMG")) fp->style = fgsImage;
						else if (SameStr(s, "JNC")) fp->style = fgsJunction;
						else			   			fp->style = fgsNormal;
						continue;
					}
					else if (s[1]=='<') continue;
				}
				else if (dmod==DSMODE_CON) {
					if (s[1]=='>') {	//関係線データの開始
						s.Delete(1, 1);
						if		(SameStr(s, "GCL")) cl_md = TDIR_G2G;
						else if (SameStr(s, "FCL")) cl_md = TDIR_F2F;
						else if (SameStr(s, "FGL")) cl_md = TDIR_F2G;
						else if (SameStr(s, "GFL")) cl_md = TDIR_G2F;
						cl_tp  = LTYPE_LINE; cl_fr = cl_to = -1;
						cl_lw  = 1; cl_at = 1; cl_as = ASIZE_MEDIUM;
						cl_lc  = Graphics::clNone;
						cl_str = EmptyStr;
						cl_op  = false;
						continue;
					}
					else if (s[1]=='<') {	//関係線データの終了
						if (cl_fr!=-1 && cl_to!=-1) {
							ArrowLine *ap;
							//重複チェック
							bool flag = false;
							for (int j=0; j<CLineList->Count; j++) {
								ap = CLineList->Items[j];
								if (ap->tType!=cl_md) continue;
								if ((cl_md==TDIR_F2G) || (cl_md==TDIR_G2F)) {
									if (ap->fromTag==cl_fr && ap->toTag==cl_to) {
										flag = true; break;
									}
								}
								else {
									if ((ap->fromTag==cl_fr && ap->toTag==cl_to) ||
										(ap->fromTag==cl_to && ap->toTag==cl_fr))
									{
										flag = true; break;
									}
								}
							}

							if (!flag) {
								//追加
								if (cl_lw<-1 || cl_lw==0) cl_lw = 1;
								if (cl_lw>9) cl_lw = 9;
								ap = new ArrowLine();
								ap->tType	  = cl_md;
								ap->fromTag   = cl_fr;
								ap->toTag	  = cl_to;
								ap->lType	  = cl_tp;
								ap->lWidth	  = cl_lw;
								ap->lColor	  = cl_lc;
								ap->aType	  = cl_at;
								ap->aSize	  = cl_as;
								ap->TextStr   = cl_str;
								ap->tOpposite = cl_op;
								CLineList->Add(ap);
							}
						}
						cl_md = TDIR_NONE;
						continue;
					}
				}
				else if (dmod==DSMODE_GRP) {
					if (SameStr(s, ">GRP") || s[1]=='<') gn = -1;
				}

				int p = s.Pos("="); if (p==0) continue;
				kstr = s.SubString(1, p - 1); s.Delete(1, p); vstr = s;
				l = vstr.Length();
				if (l>1 && vstr[1]=='\"' && vstr[l]=='\"') vstr = vstr.SubString(2, l - 2);

				switch (dmod) {
				case DSMODE_GEN:
					if (SameStr(kstr, "MaxID")) MaxID = vstr.ToIntDef(0);
					break;

				case DSMODE_FRG:	//断片
					if (SameStr(kstr, "ID")) {
						if (!mg_sw) {
							fp->ID = vstr.ToIntDef(-1);
							if (fp->ID>MaxID) MaxID = fp->ID;
						}
					}
					else if (SameStr(kstr, "TX"))
						fp->TextStr = ReplaceStr(vstr, "\"\"", "\"");
					else if (SameStr(kstr, "GP")) {
						TStringDynArray ibuf = get_csv_array(vstr, MAX_IN_GRP);
						for (int j=0; j<MAX_IN_GRP; j++)
							fp->group.stk[j] = (j<ibuf.Length)? ibuf[j].ToIntDef(0) : -1;
					}
					else if (SameStr(kstr, "XP"))
						fp->RC.Left = vstr.ToIntDef(2);
					else if (SameStr(kstr, "YP"))
						fp->RC.Top  = vstr.ToIntDef(2);
					else if (SameStr(kstr, "WD")) {
						int w = vstr.ToIntDef(EV->def_wd);
						if (w<1 || w>MAX_FRG_WD) w = EV->def_wd;
						fp->wd = w;
					}
					else if (SameStr(kstr, "HI")) {
						int h = vstr.ToIntDef(EV->def_hi);
						if (h<1 || h>MAX_FRG_HI) h = EV->def_hi;
						fp->hi = h;
					}
					else if (SameStr(kstr, "FG")) {
						TColor c = (TColor)(vstr.ToIntDef(-1));
						switch (fp->style) {
						case fgsPlate:
							fp->colFG = (c==-1)? EV->col_namFG : c; break;
						default:
							fp->colFG = (c==-1)? EV->col_defFG : c;
						}
					}
					else if (SameStr(kstr, "BG")) {
						TColor c = (TColor)(vstr.ToIntDef(-1));
						switch (fp->style) {
						case fgsPlate:
							fp->colBG = (c==-1)? EV->col_namBG : c; break;
						default:
							fp->colBG = (c==-1)? EV->col_defBG : c;
						}
					}
					else if (SameStr(kstr, "LK")) {
						fp->LinkName = rel_to_absdir(vstr, fnam);
						fp->upd_link();
					}
					else if (SameStr(kstr, "CT"))
						fp->CreTime = str_to_date_time(vstr);
					else if (SameStr(kstr, "MT"))
						fp->ModTime = str_to_date_time(vstr);
					break;

				case DSMODE_CON:	//関係線
					if (cl_md!=TDIR_NONE) {
						if		(SameStr(kstr, "FR")) cl_fr  = vstr.ToIntDef(-1);
						else if (SameStr(kstr, "TO")) cl_to  = vstr.ToIntDef(-1);
						else if (SameStr(kstr, "LT")) cl_tp  = vstr.ToIntDef(LTYPE_LINE);
						else if (SameStr(kstr, "LW")) cl_lw  = vstr.ToIntDef(1);
						else if (SameStr(kstr, "LC")) cl_lc  = (TColor)(vstr.ToIntDef(Graphics::clNone));
						else if (SameStr(kstr, "AT")) cl_at  = vstr.ToIntDef(ATYPE_LINE);
						else if (SameStr(kstr, "AS")) cl_as  = vstr.ToIntDef(ASIZE_MEDIUM);
						else if (SameStr(kstr, "TX")) cl_str = vstr;
						else if (SameStr(kstr, "OP")) cl_op  = (vstr.ToIntDef(0)==1);
					}
					break;

				case DSMODE_GRP:	//グループ
					if (gn==-1) {
						if (SameStr(kstr, "ID")) {
							gn = vstr.ToIntDef(-1);
							if (gn>MAX_GROUP) gn = 0;
						}
					}
					else {
						if (SameStr(kstr, "VS"))
							GInfs->Items[gn].Visible = ((vstr.ToIntDef(1)==1));
						else if (SameStr(kstr, "FC")) {
							TColor c = (TColor)(vstr.ToIntDef(-1));
							GInfs->Items[gn].col_GrpFrm = (c==-1)? Graphics::clNone : c;
						}
						else if (SameStr(kstr, "LW"))
							GInfs->Items[gn].LnWidth = vstr.ToIntDef(0);
					}
					break;

				case DSMODE_OPT:	//オプション
					if (StartsText("Col", kstr)) vc = (TColor)vstr.ToIntDef((int)Graphics::clNone);

					if		(SameText(kstr, "DataTitle"))	data_title = vstr;
					else if (SameText(kstr, "TitlePos"))	title_pos  = vstr.ToIntDef(0);
					else if (SameText(kstr, "ScrWidth"))	scr_wd	   = vstr.ToIntDef(EV->DefScrWd);
					else if (SameText(kstr, "ScrHeight"))	scr_hi	   = vstr.ToIntDef(EV->DefScrHi);
					else if (SameText(kstr, "ScrDivMode")) {
						if		(SameStr(vstr, "DVN")) div_mod = DVMODE_DVN;
						else if (SameStr(vstr, "PXN")) div_mod = DVMODE_PXN;
						else if (SameStr(vstr, "TRH")) div_mod = DVMODE_TRH;
						else if (SameStr(vstr, "TRV")) div_mod = DVMODE_TRV;
						else if (SameStr(vstr, "HXH")) div_mod = DVMODE_HXH;
						else if (SameStr(vstr, "HXV")) div_mod = DVMODE_HXV;
						else 						   div_mod = DVMODE_NON;
					}
					else if (SameText(kstr, "ScrDivX"))			div_x	 = vstr.ToIntDef(0);
					else if (SameText(kstr, "ScrDivY"))			div_y	 = vstr.ToIntDef(0);
					else if (SameText(kstr, "ScrDivLw"))		div_lw	 = vstr.ToIntDef(1);
					else if (SameText(kstr, "LastHPos"))		LastHPos = vstr.ToIntDef(0);
					else if (SameText(kstr, "LastVPos"))		LastVPos = vstr.ToIntDef(0);
					else if (SameText(kstr, "CurGroup"))		GInfs->cur_group = vstr.ToIntDef(1);
					else if (SameText(kstr, "ColBackground"))	col_backgr	= vc;
					else if (SameText(kstr, "ColDivLine"))		col_DivLine = vc;
					else if (SameText(kstr, "ColGrpFrame"))		col_GrpFrm	= vc;
					else if (SameText(kstr, "ColGrpLine"))		col_GrpLine = vc;
					else if (SameText(kstr, "ColFrgLine"))		col_FrgLine = vc;
					else if (SameText(kstr, "ColLinLbl"))		col_LinLbl	= vc;
					else if (SameText(kstr, "ColTitle"))		col_Title	= vc;
					else if (SameText(kstr, "FontFrg")) {
						get_font_from_str(frgFont, vstr);	ParentFrgFont = false;
					}
					else if (SameText(kstr, "FontNam")) {
						get_font_from_str(namFont, vstr);	ParentNamFont = false;
					}
					else if (SameText(kstr, "FontSym")) {
						get_font_from_str(symFont, vstr);	ParentSymFont = false;
					}
					else if (SameText(kstr, "FontLbl")) {
						get_font_from_str(lblFont, vstr);	ParentLblFont = false;
					}
					else if (SameText(kstr, "FontTit")) {
						get_font_from_str(titFont, vstr);	ParentTitFont = false;
					}
					break;
				}
			}
		}
	}
	__finally
	{
		//読み込み
		if (!mg_sw) {
			if (file_name.IsEmpty()) file_name = fnam;
		}
		//マージ
		else
			modify = (fbuf->Count>1);
	}

	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (fp->ID==-1) fp->ID = ++MaxID;
	}

	frg_owner->Color = GET_indcol(col_backgr);

	update_grp_empty();
	for (gn=1; gn<=MAX_GROUP; gn++) GInfs->calc_frame(gn);

	VscrBar->Range = scr_hi + 20;
	HscrBar->Range = scr_wd + 20;

	if ((div_x>1 || div_y>1) && div_mod==DVMODE_NON) div_mod = DVMODE_DVN;

	set_titlebar();
	EV->reqUpdate = true;
	return FrgList->Count;
}

//---------------------------------------------------------------------
//保存 (UTF-8)
//---------------------------------------------------------------------
bool FragSet::save_file(
	UnicodeString fnam,
	bool ts_sw)			//true = タイムスタンプ無変更 (default = false);
{
	std::unique_ptr<TStringList> fbuf(new TStringList());
	UnicodeString tmpstr;

	//----------------
	//一般情報
	//----------------
	fbuf->Add("[General]");
	fbuf->Add(tmpstr.sprintf(_T("MaxID=%ld"), MaxID));
	fbuf->Add(EmptyStr);

	//----------------
	//断片
	//----------------
	fbuf->Add("[Fragment]");
	int cf, cb;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		switch (fp->style) {
		case fgsPlate:	  fbuf->Add(">PLT"); break;
		case fgsSymbol:   fbuf->Add(">SYM"); break;
		case fgsImage:	  fbuf->Add(">IMG"); break;
		case fgsJunction: fbuf->Add(">JNC"); break;
		default:		  fbuf->Add(">TXT");
		}
		fbuf->Add(tmpstr.sprintf(_T("ID=%d"), fp->ID));

		tmpstr = fp->TextStr;
		tmpstr = ReplaceStr(tmpstr, "\"", "\"\"");
		fbuf->Add("TX=\"" + tmpstr + "\"");

		tmpstr = IntToStr(fp->group.stk[0]);
		for (int j=1; j<MAX_IN_GRP; j++) {
			if (fp->group.stk[j]==-1) break;
			tmpstr += ("," + IntToStr(fp->group.stk[j]));
		}
		fbuf->Add("GP=" + tmpstr);

		fbuf->Add(tmpstr.sprintf(_T("XP=%d"),	fp->RC.Left));
		fbuf->Add(tmpstr.sprintf(_T("YP=%d"),	fp->RC.Top));
		fbuf->Add(tmpstr.sprintf(_T("WD=%d"),	fp->wd));
		fbuf->Add(tmpstr.sprintf(_T("HI=%d"),	fp->hi));

		switch (fp->style) {
		case fgsPlate:
			cf = (int)fp->colFG; cb = (int)fp->colBG; break;
		default:
			cf = (int)fp->colFG; cb = (int)fp->colBG;
		}
		fbuf->Add(tmpstr.sprintf(_T("FG=0x%08x"),	cf));
		fbuf->Add(tmpstr.sprintf(_T("BG=0x%08x"),	cb));

		tmpstr = fp->LinkName;
		if (EV->LinkRelPath) tmpstr = abs_to_reldir(tmpstr, fnam);
		fbuf->Add("LK=" + tmpstr);

		DateTimeToString(tmpstr, "yyyy/mm/dd hh:nn:ss", fp->CreTime);
		fbuf->Add("CT=" + tmpstr);
		DateTimeToString(tmpstr, "yyyy/mm/dd hh:nn:ss", fp->ModTime);
		fbuf->Add("MT=" + tmpstr);
		fbuf->Add("<");
		fbuf->Add(EmptyStr);
	}
	fbuf->Add(EmptyStr);

	//----------------
	//関係線
	//----------------
	//グループ関係線
	fbuf->Add("[Connection]");
	for (int i=0; i<CLineList->Count; i++) {
		ArrowLine *ap = CLineList->Items[i];
		if		(ap->tType==TDIR_G2G) fbuf->Add(">GCL");
		else if (ap->tType==TDIR_F2F) fbuf->Add(">FCL");
		else if (ap->tType==TDIR_F2G) fbuf->Add(">FGL");
		else if (ap->tType==TDIR_G2F) fbuf->Add(">GFL");
		else continue;

		fbuf->Add(tmpstr.sprintf(_T("FR=%d"),	ap->fromTag));
		fbuf->Add(tmpstr.sprintf(_T("TO=%d"),	ap->toTag));
		fbuf->Add(tmpstr.sprintf(_T("LT=%d"),	ap->lType));
		fbuf->Add(tmpstr.sprintf(_T("LW=%d"),	ap->lWidth));
		if (ap->lColor!=Graphics::clNone)
			fbuf->Add(tmpstr.sprintf(_T("LC=%d"), (int)ap->lColor));
		fbuf->Add(tmpstr.sprintf(_T("AT=%d"),	ap->aType));
		fbuf->Add(tmpstr.sprintf(_T("AS=%d"),	ap->aSize));
		if (!ap->TextStr.IsEmpty()) fbuf->Add("TX=\""+ ap->TextStr + "\"");
		if (ap->tOpposite) fbuf->Add("OP=1");
		fbuf->Add("<");
		fbuf->Add(EmptyStr);
	}
	fbuf->Add(EmptyStr);

	//----------------
	//グループ情報
	//----------------
	fbuf->Add("[Group]");
	for (int i=0; i<=MAX_GROUP; i++) {
		grp_inf *gp = &GInfs->Items[i];
		fbuf->Add(">GRP");
		fbuf->Add(tmpstr.sprintf(_T("ID=%d"),		i));
		fbuf->Add(tmpstr.sprintf(_T("VS=%d"),		gp->Visible));
		fbuf->Add(tmpstr.sprintf(_T("FC=0x%08x"),	(int)gp->col_GrpFrm));
		fbuf->Add(tmpstr.sprintf(_T("LW=%d"),		gp->LnWidth));
		fbuf->Add("<");
		fbuf->Add(EmptyStr);
	}
	fbuf->Add(EmptyStr);

	//----------------
	//オプション
	//----------------
	fbuf->Add("[Option]");
	//タイトル
	fbuf->Add("DataTitle=\"" + data_title + "\"");
	fbuf->Add(tmpstr.sprintf(_T("TitlePos=%d"),	title_pos));

	//画面サイズ
	fbuf->Add(tmpstr.sprintf(_T("ScrWidth=%d"),	scr_wd));
	fbuf->Add(tmpstr.sprintf(_T("ScrHeight=%d"),	scr_hi));

	//画面分割線
	switch (div_mod) {
	case DVMODE_DVN: fbuf->Add("ScrDivMode=DVN"); break;
	case DVMODE_PXN: fbuf->Add("ScrDivMode=PXN"); break;
	case DVMODE_TRH: fbuf->Add("ScrDivMode=TRH"); break;
	case DVMODE_TRV: fbuf->Add("ScrDivMode=TRV"); break;
	case DVMODE_HXH: fbuf->Add("ScrDivMode=HXH"); break;
	case DVMODE_HXV: fbuf->Add("ScrDivMode=HXV"); break;
	default:		 fbuf->Add("ScrDivMode=NON");
	}
	fbuf->Add(tmpstr.sprintf(_T("ScrDivX=%d"),  div_x));
	fbuf->Add(tmpstr.sprintf(_T("ScrDivY=%d"),  div_y));
	fbuf->Add(tmpstr.sprintf(_T("ScrDivLw=%d"), div_lw));

	//個別配色
	fbuf->Add(tmpstr.sprintf(_T("ColBackground=0x%08x"),	(int)col_backgr));
	fbuf->Add(tmpstr.sprintf(_T("ColDivLine=0x%08x"),		(int)col_DivLine));
	fbuf->Add(tmpstr.sprintf(_T("ColGrpFrame=0x%08x"),		(int)col_GrpFrm));
	fbuf->Add(tmpstr.sprintf(_T("ColGrpLine=0x%08x"),		(int)col_GrpLine));
	fbuf->Add(tmpstr.sprintf(_T("ColFrgLine=0x%08x"),		(int)col_FrgLine));
	fbuf->Add(tmpstr.sprintf(_T("ColLinLbl=0x%08x"),		(int)col_LinLbl));
	fbuf->Add(tmpstr.sprintf(_T("ColTitle=0x%08x"),		(int)col_Title));

	//個別フォント
	TFont *f = frgFont;
	if (!ParentFrgFont) {
		fbuf->Add(tmpstr.sprintf(_T("FontFrg=%s,%d,%d,%d"),
			f->Name.c_str(), f->Size,
			f->Style.Contains(fsBold), f->Style.Contains(fsItalic)));
	}
	f = namFont;
	if (!ParentNamFont) {
		fbuf->Add(tmpstr.sprintf(_T("FontNam=%s,%d,%d,%d"),
			f->Name.c_str(), f->Size, f->Style.Contains(fsBold), f->Style.Contains(fsItalic)));
	}
	f = symFont;
	if (!ParentSymFont) {
		fbuf->Add(tmpstr.sprintf(_T("FontSym=%s,%d,%d,%d"),
			f->Name.c_str(), f->Size, f->Style.Contains(fsBold), f->Style.Contains(fsItalic)));
	}
	f = lblFont;
	if (!ParentLblFont) {
		fbuf->Add(tmpstr.sprintf(_T("FontLbl=%s,%d,%d,%d"),
			f->Name.c_str(), f->Size, f->Style.Contains(fsBold), f->Style.Contains(fsItalic)));
	}
	f = titFont;
	if (!ParentTitFont) {
		fbuf->Add(tmpstr.sprintf(_T("FontTit=%s,%d,%d,%d"),
			f->Name.c_str(), f->Size, f->Style.Contains(fsBold), f->Style.Contains(fsItalic)));
	}

	//前回の位置
	LastHPos = HscrBar->Position;
	LastVPos = VscrBar->Position;
	fbuf->Add(tmpstr.sprintf(_T("LastHPos=%d"),	LastHPos));
	fbuf->Add(tmpstr.sprintf(_T("LastVPos=%d"),	LastVPos));
	fbuf->Add(tmpstr.sprintf(_T("CurGroup=%d"),	GInfs->cur_group));

	try {
		int last_ft = -1;
		TDateTime td;
		if (FileAge(fnam, td)) last_ft = DateTimeToFileDate(td);
		fbuf->SaveToFile(fnam, TEncoding::UTF8);
		if (ts_sw && last_ft!=-1) FileSetDate(fnam, last_ft);
		file_name = fnam;
		modify	  = false;
		read_only = false;

	}
	catch (...) {
		msgbox_ERR(tmpstr.sprintf(_T(" [%s] の保存に失敗しました"), ExtractFileName(fnam).c_str()));
		return false;
	}

	return true;
}

//---------------------------------------------------------------------
//インポート
//---------------------------------------------------------------------
int FragSet::import_core(TStringList *TextBuff, int gn)
{
	int  n = 0;
	bool np_flag = false;
	UnicodeString tmpstr;

	for (int i=0; i<TextBuff->Count; i++) {
		tmpstr = TextBuff->Strings[i];
		if (tmpstr.IsEmpty()) continue;

		if (SameStr(tmpstr, "-")) {
			//グループの区切り
			gn++; if (gn>MAX_GROUP) gn = MAX_GROUP;
			np_flag = false;
			continue;
		}
		else if (StartsStr(";グループ", tmpstr)) { //グループか？
			int tmpn = tmpstr.SubString(6, 3).ToIntDef(0);
			if (tmpn>0 && tmpn<=MAX_GROUP) {
				gn = tmpn; continue;
			}
		}
		else if (SameStr(tmpstr, ";無所属")) { //無所属か？
			gn = 0; continue;
		}

		Fragment *fp = new_frag();
		//表札
		if (tmpstr[1]==';' && !np_flag) {
			fp->style	= fgsPlate;
			fp->TextStr	= tmpstr.Delete(1, 1);
			fp->colFG	= EV->col_namFG;
			fp->colBG	= EV->col_namBG;
			fp->wd		= EV->nam_wd;
			fp->hi		= EV->nam_hi;
			np_flag		= true;
		}
		//断片
		else {
			fp->style	= fgsNormal;
			fp->TextStr	= tmpstr;
			fp->colFG	= EV->col_defFG;
			fp->colBG	= EV->col_defBG;
			fp->wd		= EV->def_wd;
			fp->hi		= EV->def_hi;
			//自動的に行数増
			if (EV->auto_exp) {
				TCanvas *cv = frg_owner->Canvas;
				cv->Font->Assign(frgFont);
				int l = get_FragFitHi(cv, fp->TextStr, fp->wd);
				if (l>=EV->def_hi) fp->hi = l;
			}
		}
		fp->group.stk[0] = gn;
		GInfs->Items[gn].Visible = true;
		set_new_pos(fp, i);
		fp->LinkName = EmptyStr;
		n++;
	}

	update_grp_empty();
	modify		  = true;
	EV->reqUpdate = true;
	return n;
}

//---------------------------------------------------------------------
//テキストファイルの読み込み
//---------------------------------------------------------------------
int FragSet::import_text(UnicodeString  fnam)
{
	std::unique_ptr<TStringList> fbuf(new TStringList());
	fbuf->LoadFromFile(fnam);
	int gn = EV->new_no_group? 0 : get_sel_group_ex();
	import_core(fbuf.get(), gn);
	return FrgList->Count;
}
//---------------------------------------------------------------------
//テキストファイルへ書き出し
//---------------------------------------------------------------------
int FragSet::export_text(UnicodeString fnam)
{
	std::unique_ptr<TStringList> fbuf(new TStringList());
	UnicodeString tmpstr;

	if (EV->ClipPosSort)
		FrgList->Sort(FrgCmp_Pos);
	else
		FrgList->Sort(FrgCmp_Z);

	//データをテキスト化
	for (int gn=0; gn<=MAX_GROUP; gn++) {
		if (GInfs->Items[gn].Empty) continue;
		if (gn==0)
			fbuf->Add(";無所属");
		else {
			UnicodeString np_str;
			Fragment *np = GInfs->get_name_plate(gn, &np_str);
			if (np)
				fbuf->Add(tmpstr.sprintf(_T(";%s"), np_str.c_str())); //表札はコメント化
			else
				fbuf->Add(tmpstr.sprintf(_T(";グループ%d"), gn));
		}
		for (int i=0; i<FrgList->Count; i++) {
			Fragment *fp = FrgList->Items[i];
			if (fp->group.stk[0]==gn && fp->style==fgsNormal) fbuf->Add(fp->TextStr);
		}
		fbuf->Add("-");
	}

	//書き込み
	int n = fbuf->Count;
	try {
		fbuf->SaveToFile(fnam, EV->ExportUTF8? TEncoding::UTF8 : TEncoding::Default);
	}
	catch (...) {
		msgbox_ERR(tmpstr.sprintf(_T(" [%s] の保存に失敗しました"), ExtractFileName(fnam).c_str()));
		n = 0;
	}
	return n;
}
//---------------------------------------------------------------------------
//断片をエクスポート
//---------------------------------------------------------------------
int FragSet::export_file(
	UnicodeString fnam,			//ファイル名
	UnicodeString separator)	//区切り文字
{
	if (separator.IsEmpty()) separator = ",";

	if (EV->ClipPosSort)
		FrgList->Sort(FrgCmp_Pos);
	else
		FrgList->Sort(FrgCmp_Z);

	std::unique_ptr<TStringList> fbuf(new TStringList());

	UnicodeString lbuf = EV->CsvFormat;
	lbuf = ReplaceStr(lbuf, "GP", "グループ");
	lbuf = ReplaceStr(lbuf, "TP", "種類");
	lbuf = ReplaceStr(lbuf, "TX", "テキスト");
	lbuf = ReplaceStr(lbuf, "FG", "文字色");
	lbuf = ReplaceStr(lbuf, "BG", "背景色");
	lbuf = ReplaceStr(lbuf, "LK", "リンク先");
	lbuf = ReplaceStr(lbuf, "CT", "作成日時");
	lbuf = ReplaceStr(lbuf, "MT", "更新日時");
	lbuf = ReplaceStr(lbuf, ",",  separator);
	fbuf->Add(lbuf);	//見出し行

	TStringDynArray ibuf = get_csv_array(EV->CsvFormat, 20);
	for (int gn=0; gn<=MAX_GROUP; gn++) {
		for (int i=0; i<FrgList->Count; i++) {
			Fragment *fp = FrgList->Items[i];
			if (fp->group.stk[0]!=gn) continue;
			if (fp->style==fgsJunction) continue;
			lbuf = EmptyStr;
			for (int j=0; j<ibuf.Length; j++) {
				UnicodeString key = ibuf[j];
				if (key.IsEmpty()) break;
				UnicodeString temp;
				if (SameStr(key, "TP")) {
					switch (fp->style) {
					case fgsPlate:	temp = "表札"; break;
					case fgsSymbol:	temp = "標識"; break;
					case fgsImage:	temp = "画像"; break;
					default:		temp = "断片";
					}
				}
				else if (SameStr(key, "GP")) temp.sprintf(_T("%d"),		fp->group.stk[0]);
				else if (SameStr(key, "TX")) temp.sprintf(_T("\"%s\""),	fp->TextStr.c_str());
				else if (SameStr(key, "FG")) temp.sprintf(_T("0x%08x"),	fp->colFG);
				else if (SameStr(key, "BG")) temp.sprintf(_T("0x%08x"),	fp->colBG);
				else if (SameStr(key, "LK")) temp.sprintf(_T("\"%s\""),	fp->LinkName.c_str());
				else if (SameStr(key, "CT"))
					temp.sprintf(_T("\"%s\""), fp->CreTime.FormatString("yyyy/mm/dd hh:nn:ss").c_str());
				else if (SameStr(key, "MT"))
					temp.sprintf(_T("\"%s\""), fp->ModTime.FormatString("yyyy/mm/dd hh:nn:ss").c_str());

				if (j>0) lbuf += separator;
				lbuf += temp;
			}
			fbuf->Add(lbuf);
		}
	}

	//書き込み
	int n = fbuf->Count;
	try {
		fbuf->SaveToFile(fnam, EV->ExportUTF8? TEncoding::UTF8 : TEncoding::Default);
	}
	catch (...) {
		msgbox_ERR("エクスポートに失敗しました");
		n = 0;
	}
	return n;
}

//---------------------------------------------------------------------------
// カットバッファ処理
//---------------------------------------------------------------------------
//選択中断片の文字列を取得
void FragSet::get_sel_str(UnicodeString *cpybuf)
{
	*cpybuf = EmptyStr;

	if (SelList->Count>0) {
		SelList->Sort(EV->ClipPosSort? FrgCmp_Pos : FrgCmp_Z);

		for (int gn=0; gn<=MAX_GROUP; gn++) {
			if (GInfs->Items[gn].Empty) continue;
			UnicodeString s;
			for (int i=0; i<SelList->Count; i++) {
				Fragment *fp = SelList->Items[i];
				if (fp->group.stk[0]==gn && fp->style==fgsNormal) s.cat_sprintf(_T("%s\n"), fp->TextStr.c_str());
			}
			if (s.IsEmpty()) continue;
			if (gn==0)
				(*cpybuf).UCAT_TSTR(";無所属\n");
			else {
				UnicodeString np_str;
				Fragment *np = GInfs->get_name_plate(gn, &np_str);
				if (np) {
					if (np->Selected) (*cpybuf).cat_sprintf(_T(";%s\n"), np_str.c_str());
				}
			}
			*cpybuf += s;
		}
	}
}

//---------------------------------------------------------------------
//コピー/ 切り取り
//---------------------------------------------------------------------
void FragSet::copy_buf(bool cutsw)
{
	if (SelList->Count==0) return;

	if (cutsw) push_all();

	FSbuf->del_all();
	FSbuf->CopySource = frg_owner;

	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		Fragment *dp = FSbuf->new_frag(false);
		dp->assign(fp, true);	//IDごとコピー
		//全体選択されていないグループは解除
		int j = 0;
		while (j<MAX_IN_GRP) {
			int gn = dp->group.stk[j]; if (gn<=0) break;
			if (!GInfs->Items[gn].AllSelected) dp->rel_group(gn); else j++;
		}
	}

	//関係線を抽出
	if (EV->CopyCLine) {
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ip = CLineList->Items[i];
			int cn = 0;
			for (int j=0; j<FSbuf->FrgList->Count && cn<2; j++) {
				Fragment *fp = FSbuf->FrgList->Items[j];
				switch (ip->tType) {
				case TDIR_F2F:
					if (ip->fromTag==fp->ID || ip->toTag==fp->ID) cn++;
					break;
				case TDIR_G2G:
					if (fp->in_group(ip->fromTag)!=-1 || fp->in_group(ip->toTag)!=-1) cn++;
					break;
				case TDIR_F2G:
					if (ip->fromTag==fp->ID || fp->in_group(ip->toTag)!=-1) cn++;
					break;
				case TDIR_G2F:
					if (fp->in_group(ip->fromTag)!=-1 || ip->toTag==fp->ID) cn++;
					break;
				}
			}
			if (cn==2) {
				ArrowLine *ap = new ArrowLine();
				ap->assign(ip);
				FSbuf->CLineList->Add(ap);
			}
		}
	}

	//切り取り
	if (cutsw) {
		int i = 0;
		while (i<FrgList->Count) {
			Fragment *fp = FrgList->Items[i];
			if (fp->Selected) {
				del_frag(fp);
				FrgList->Delete(i);
			}
			else i++;
		}
		FSbuf->IsCopy = false;
		SelList->Clear();
		update_grp_empty();
		modify		  = true;
		EV->reqUpdate = true;
	}
	//コピー
	else {
		FSbuf->IsCopy = true;
		all_select(false);
	}
}

//---------------------------------------------------------------------
//貼り付け
//---------------------------------------------------------------------
void FragSet::paste_buf(
	int x, int y)	//位置指定 (ともに >=0 の場合に採用)
{
	if (FSbuf->FrgList->Count==0) return;

	push_all();

	Fragment *fp,*dp;

	//貼り付け位置チェック
	bool flag = true;
	for (int i=0; i<FSbuf->FrgList->Count; i++) {
		dp = FSbuf->FrgList->Items[i];
		if (dp->RC.Left!=-1 || dp->RC.Top!=-1) {
			flag = false; break;
		}
	}

	//グループコピー用のテーブルを用意
	int cvtbl[MAX_GROUP + 1];
	for (int i=0; i<=MAX_GROUP; i++) cvtbl[i] = -1;

	//関係線のID誤変更防止フラグをクリア(change_frg_idで使用)
	FSbuf->CLineList->clr_tmp_flag();

	//指定位置に貼り付け
	if (x>=0 && y>=0) {
		if (flag) {
			for (int i=0; i<FSbuf->FrgList->Count; i++)
				set_new_pos(FSbuf->FrgList->Items[i], i);
		}
		//最小のx,y位置をもとめる
		TPoint p = FSbuf->FrgList->get_min_pos();
		//指定位置のグループスタックを取得
		grp_stk tmp_stk;
		GInfs->gstk_from_pos(x, y, &tmp_stk);
		//貼り付け位置までの差をもとめる
		int dx = x - p.x + HscrBar->Position;
		int dy = y - p.y + VscrBar->Position;

		//断片を作成して貼り付け
		for (int i=0; i<FSbuf->FrgList->Count; i++) {
			fp = new_frag();
		 	dp = FSbuf->FrgList->Items[i];
			fp->assign(dp);
			fp->RC.Left += dx;
			fp->RC.Top	+= dy;
			fp->Selected = true;
			fp->Visible  = true;
			//コピーまたは他ウィンドウのものならグループのコピーを作る
			if (FSbuf->IsCopy || FSbuf->CopySource!=frg_owner) {
				for (int j=0; j<MAX_IN_GRP; j++) {
					int gn0 = fp->group.stk[j];  if (gn0<=0) break;
					if (cvtbl[gn0]==-1) {
						int gn1 = GInfs->get_empty_idx();
						if (gn1==-1) gn1 = 0; else GInfs->Items[gn1].Empty = false;
						cvtbl[gn0] = gn1;
					}
					dp->group.stk[j] = fp->group.stk[j] = cvtbl[gn0];
				}
			}
			//指定位置のグループ構造を追加
			for (int j=0; j<MAX_IN_GRP; j++) {
				int gn = tmp_stk.stk[j];  if (gn<=0) break;
				fp->set_group(gn, true);
			}
			//すでに表札があれば解除
			Fragment *np = GInfs->get_name_plate(fp->group.stk[0], NULL);
			if (np && np!=fp) fp->style = fgsNormal;
			fp->upd_link();

			//断片IDを修正
			if (EV->CopyCLine) {
				FSbuf->CLineList->change_frg_id(dp->ID, fp->ID);
				dp->ID = fp->ID;
			}
		}

		for (int i=0; i<=MAX_GROUP; i++) if (cvtbl[i]==-1) cvtbl[i] = i;

		//関係線のグループ情報を修正
		for (int i=0; i<FSbuf->CLineList->Count; i++) {
			ArrowLine *ap = FSbuf->CLineList->Items[i];
			if (ap->tType==TDIR_F2G) {
				if (ap->toTag!=-1) ap->toTag = cvtbl[ap->toTag];
			}
			else if (ap->tType==TDIR_G2F) {
				if (ap->fromTag!=-1) ap->fromTag = cvtbl[ap->fromTag];
			}
			else if (ap->tType==TDIR_G2G) {
				if (ap->toTag!=-1) ap->toTag = cvtbl[ap->toTag];
				if (ap->fromTag!=-1) ap->fromTag = cvtbl[ap->fromTag];
			}
		}

	}
	//普通の貼り付け
	else {
		//断片を作成して貼り付け
		for (int i=0; i<FSbuf->FrgList->Count; i++) {
			fp = new_frag();
			dp = FSbuf->FrgList->Items[i];
			fp->assign(dp);
			if (flag)
				//新規作成位置に貼り付けに
				set_new_pos(fp, i);
			else {
				//元位置から4ドットずらして貼り付け
				fp->RC.Left += 4;
				fp->RC.Top	+= 4;
			}
			fp->Selected = true;
			fp->Visible  = true;
			//すでに表札があれば解除
			Fragment *np = GInfs->get_name_plate(fp->group.stk[0], NULL);
			if (np && np!=fp) fp->style = fgsNormal;
			fp->upd_link();

			//断片IDを修正
			if (EV->CopyCLine) {
				FSbuf->CLineList->change_frg_id(dp->ID, fp->ID);
				dp->ID = fp->ID;
			}
		}
	}

	//関係線を貼り付け
	if (EV->CopyCLine) {
		for (int i=0; i<FSbuf->CLineList->Count; i++) {
			ArrowLine *ip = FSbuf->CLineList->Items[i];
			if (!CLineList->lp_from_id(ip->tType, ip->fromTag, ip->toTag)) {
				ArrowLine *ap = new ArrowLine();
				ap->assign(ip);
				CLineList->Add(ap);
			}
		}
	}

	FrgList->Sort(FrgCmp_Z);
	update_grp_empty();
	mak_sel_list();
	modify		  = true;
	EV->reqUpdate = true;
	if (!FSbuf->IsCopy) FSbuf->IsCopy = true;
}

//---------------------------------------------------------------------------
//クリップボードから新規断片
//---------------------------------------------------------------------------
int FragSet::new_from_clipboard()
{
	if (Clipboard()->HasFormat(CF_TEXT)) {
		push_all();
		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->Text = Clipboard()->AsText;
		int gn = EV->new_no_group? 0 : get_sel_group_ex();
		import_core(fbuf.get(), gn);
	}
	return FrgList->Count;
}

//---------------------------------------------------------------------------
//全てを選択／解除
//---------------------------------------------------------------------------
void FragSet::all_select(
	bool sw)	//true = 選択(デフォルト)/ false = 解除
{
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (!fp->Visible) continue;
		if (fp->Selected != sw) {
			fp->Selected = sw;
			fp->show_frag();
			if (fp->style==fgsPlate && fp->OnGfrm)
				GInfs->Items[fp->group.stk[0]].gfp->Repaint();
		}
	}
	mak_sel_list();

	if (!sw) {
		if (erase_sym_frm()) visible_update();
	}
}
//---------------------------------------------------------------------
//枠で選択追加
//---------------------------------------------------------------------
void FragSet::rect_select(TRect rc)
{
	if ((int)(rc.right - rc.left)*(rc.bottom - rc.top)>64L) {
		TRect rc2;
		for (int i=0; i<FrgList->Count; i++) {
			Fragment *fp = FrgList->Items[i];	if (!fp->Visible) continue;
			grp_inf *gp0 = &GInfs->Items[fp->group.stk[0]];
			bool gflag = (fp->style==fgsPlate && fp->OnGfrm);
			if (gflag) {
				//グループ枠上の表札
				TGrpFrame *sp = gp0->gfp;
				rc2 = sp->PlateRect;
				rc2.Offset(sp->Left, sp->Top);
			}
			else
				rc2 = fp->vRC;

			if (EV->FrameAllSel || rc.Contains(rc2)) fp->Selected = true;
			if (gflag)
				gp0->RepaintFrm = true;
			else if (!fp->Changed)
				fp->show_frag();
		}
		mak_sel_list(true);
	}
	else if (EV->ImageIsBack && GInfs->exist_img && SelList->Count==0) {
		FrgList->Sort(FrgCmp_Z);
		erase_sym_frm();
		visible_update();
	}
}
//---------------------------------------------------------------------
//指定グループのみを選択
//---------------------------------------------------------------------
void FragSet::group_select(
	int gn,
	bool sw)	//選択後中央へ
{
	if (gn<0 || gn>MAX_GROUP) return;

	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];	if (!fp->Visible) continue;
		fp->Selected = (fp->in_group(gn)!=-1);
		if (fp->Selected)
			bring_to_front(fp);
		else {
			fp->show_frag();
			if (fp->style==fgsPlate && fp->OnGfrm) GInfs->Items[fp->group.stk[0]].gfp->Repaint();
		}
	}

	//選択された断片の表示を更新
	FrgList->Sort(FrgCmp_Z);
	mak_sel_list(true);

	//表札をトップに
	bring_to_front(GInfs->get_name_plate(gn, NULL));

	//重心断片を中央へ
	if (sw) to_center(get_sel_center(), false);

	GInfs->Items[gn].RepaintFrm = true;
	GInfs->repaint_frame();
}
//---------------------------------------------------------------------
//背景色で選択
//---------------------------------------------------------------------
void FragSet::bgcol_select(TColor col)
{
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];	if (!fp->Visible) continue;
		fp->Selected = (fp->colBG == col);
		if (!fp->Changed) fp->show_frag();
	}
	mak_sel_list(true);

	//重心断片を中央へ
	to_center(get_sel_center(), false);
}

//---------------------------------------------------------------------
//選択中のものだけを残す
//---------------------------------------------------------------------
void FragSet::trim_select()
{
	if (FrgList->Count==0) return;

	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		fp->Selected = fp->Visible? !fp->Selected : true;
	}
	mak_sel_list();
	del_sel_frag();
	update_group_frame();

	//移動幅をもとめる
	all_select(true);
	TPoint p = SelList->get_min_pos();
	p.x = EV->TrimMargin - p.x;
	p.y = EV->TrimMargin - p.y;

	//左上端へ全体移動／ 広がりサイズを求める
	int trm_wd = 1;
	int trm_hi = 1;
	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		if (EV->TrimMovTop) fp->RC.Offset(p.x, p.y);	//移動
		if (fp->RC.Right  >trm_wd) trm_wd = fp->RC.Right;
		if (fp->RC.Bottom >trm_hi) trm_hi = fp->RC.Bottom;
	}
	if (EV->TrimScrSize) {
		//固定サイズ
		if (EV->TrimFixSize) {
			trm_wd = EV->TrimFixWd;
			trm_hi = EV->TrimFixHi;
		}
		//フィット
		else {
			trm_wd += EV->TrimMargin;
			trm_hi += EV->TrimMargin;
		}
		scr_wd = trm_wd;
		scr_hi = trm_hi;
		VscrBar->Range = scr_hi + 20;
		HscrBar->Range = scr_wd + 20;
		VscrBar->Position = 0;
		HscrBar->Position = 0;
	}
	all_select(false);

	update_grp_empty();
	modify = true;
}

//---------------------------------------------------------------------
//指定断片だけを選択して中央へ
//---------------------------------------------------------------------
void FragSet::one_select_center(Fragment *fp)
{
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *dp = FrgList->Items[i];
		dp->Selected = (dp == fp);
		if (dp->Changed) dp->show_frag();
	}

	mak_sel_list();
	bring_to_front(fp);

	//拡張検索 or 断片リスト表示中
	if (FindExForm->Visible || FragGrid->Visible) {
		int x = 0, y = 0;
		TRect rc = FindExForm->Visible? FindExForm->BoundsRect : FragGrid->BoundsRect;
		TPoint lt = frg_owner->ScreenToClient(Point(rc.Left, rc.Top));
		TPoint rb = frg_owner->ScreenToClient(Point(rc.Right, rc.Bottom));
		rc = Rect(lt.x, lt.y, rb.x, rb.y);
		//交差していたら、一番広く空き領域の中央へ
		if (rc.IntersectsWith(frg_owner->ClientRect)) {
			int w_l = rc.Left;
			int w_r = frg_owner->ClientWidth - rc.Right;
			int w_t = rc.Top;
			int w_b = frg_owner->ClientHeight - rc.Bottom;
			int w = 0;
			int idx = -1;
			if (w_l>w) { w = w_l; idx = 0; }
			if (w_r>w) { w = w_r; idx = 1; }
			if (w_t>w) { w = w_t; idx = 2; }
			if (w_b>w) { w = w_b; idx = 3; }
			switch (idx) {
			case 0: x = w/2;				break;
			case 1: x = w/2 + rc.Right;		break;
			case 2: y = w/2;				break;
			case 3: y = w/2 + rc.Bottom;	break;
			}
		}
		to_center(fp, false, x, y);
	}
	else
		to_center(fp, false);
}

//---------------------------------------------------------------------
//選択中断片を集合/片付け
//---------------------------------------------------------------------
#define AWAY_MARGIN 12

void FragSet::gather_sel(
	bool sw,	//0 = 集合/  1 = 片付け
	int pt)		//ずらし幅
{
	if (SelList->Count==0) return;

	push_all();

	//テキスト順にソート
	int sd = EV->FLstSortDir;
	EV->FLstSortDir = 1;
	SelList->Sort(FrgCmp_Text);
	EV->FLstSortDir = sd;

	int xp, yp;
	Fragment *fp,*dp;

	//集合する左上位置を求める
	if (!sw) {
		TPoint p = SelList->get_min_pos();
		xp = p.x;
		yp = p.y;
	}
	//片付け位置
	else {
		switch (EV->away_frg_pos) {
		case 1:	//右上
			xp = scr_wd - AWAY_MARGIN;
			yp = AWAY_MARGIN;
			break;
		case 2:	//左下
			xp = AWAY_MARGIN;
			yp = scr_hi - AWAY_MARGIN;
			break;
		case 3:	//右下
			xp = scr_wd - AWAY_MARGIN;
			yp = scr_hi - AWAY_MARGIN;
			break;
		default: //左上
			xp = AWAY_MARGIN;
			yp = AWAY_MARGIN;
		}
	}

	//断片を移動
	std::unique_ptr<TFragList> tmp_list(new TFragList());
	for (int i=0; i<SelList->Count; i++) {
		fp = SelList->Items[i];
		TRect *rp = &fp->RC;
		get_intr_flist(fp, tmp_list.get());
		fp->era_frag();
		//集合
		if (!sw) {
			if (i>0) {
				xp += pt;
				yp += pt;
			}
			rp->Left = xp;
			rp->Top  = yp;
		}
		//片付け
		else {
			switch (EV->away_frg_pos) {
			case 1:	 //右上
				rp->Left = xp - rp->Width();
				rp->Top  = yp;
				xp -= pt; yp += pt;
				break;
			case 2:	 //左下
				rp->Left = xp;
				rp->Top  = yp - rp->Height();
				xp += pt; yp -= pt;
				break;
			case 3:	 //右下
				rp->Left = xp - rp->Width();
				rp->Top  = yp - rp->Height();
				xp -= pt; yp -= pt;
				break;
			default: //左上
				rp->Left = xp;
				rp->Top  = yp;
				xp += pt; yp += pt;
			}
		}
		fp->Z = ++MaxZ;
	}

	tmp_list->AllShow();	//交叉してた断片を更新

	//同一グループで表札があればトップに
	int n = get_sel_group();
	if (n!=-1) bring_to_front(GInfs->get_name_plate(n, NULL));

	FrgList->Sort(FrgCmp_Z);
	mak_sel_list(true);
	CLineList->erase_line(TDIR_G2G);
	update_group_frame();
	if (!sw) to_center(SelList->Items[0], false);
	MapViewer->MapUpdate(NULL);
	modify = true;
}

//---------------------------------------------------------------------
//選択中断片をひろげて並べる
//---------------------------------------------------------------------
void FragSet::spread_sel()
{
	int scnt = SelList->Count; if (SelList->Count<2) return;

	push_all();

	int coln = (int)(sqrt(1.0 * scnt));

	//左上位置を求める
	TPoint p = SelList->get_min_pos();
	int left_min = p.x;
	int top_min  = p.y;

	if (EV->SpreadShuffle)
		//シャッフル
		for (int i=0; i<scnt/2; i++) SelList->Exchange(random(scnt), random(scnt));
	else {
		//テキスト順にソート
		int sd = EV->FLstSortDir;
		EV->FLstSortDir = 1;
		SelList->Sort(FrgCmp_Text);
		EV->FLstSortDir = sd;
	}

	//展開
	int  cp	  = 0;
	int  xp	  = left_min;
	int  yp	  = top_min;
	int  hmx  = 0;

	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		int w = fp->RC.Width();
		int h = fp->RC.Height(); if (h>hmx) hmx=h;
		fp->era_frag();

		if ((xp + w + w/3)>scr_wd) {
			//横にはみ出したら次行へ
			xp = (EV->SpreadAlign? left_min : (left_min + random(8)));
			yp += (hmx + 8); hmx = 0;
			cp = 0;
		}

		//揃える
		if (EV->SpreadAlign || i==0) {
			fp->RC.Left	= xp;
			fp->RC.Top	= yp;
		}
		//ばらまき
		else {
			fp->RC.Left	= xp + random(8);
			fp->RC.Top	= yp + random(8) + random(hmx - h);
		}
		fp->RC.Right  = fp->RC.Left + w;
		fp->RC.Bottom = fp->RC.Top + h;
		fp->Z = ++MaxZ;

		cp++;
		if (cp==coln) {
			cp = 0;
			xp = (EV->SpreadAlign? left_min : (left_min + random(8)));
			yp += (hmx + 8); hmx = 0;
		}
		else
			xp += (w + 8);
	}

	FrgList->Sort(FrgCmp_Z);
	mak_sel_list();
	CLineList->erase_line(TDIR_G2G);
	update_group_frame();
	modify		  = true;
	EV->reqUpdate = true;
}

//---------------------------------------------------------------------
//選択中断片(全体の中心)を指定位置に移動
//---------------------------------------------------------------------
void FragSet::move_sel(int x, int y)
{
	if (SelList->Count==0) return;

	push_all();

	//移動位置までの差をもとめる
	TRect rc = get_sel_union();
	if (x<0) x = 0;
	if (y<0) y = 0;
	int dx = x - rc.CenterPoint().x + HscrBar->Position;
	int dy = y - rc.CenterPoint().y + VscrBar->Position;;
	rc.Offset(dx, dy);
	if (rc.Left<0) dx += (0 - rc.Left);
	if (rc.Top<0)  dy += (0 - rc.Top);
	if (rc.Right>scr_wd)  dx -= (rc.Right  - scr_wd);
	if (rc.Bottom>scr_hi) dy -= (rc.Bottom - scr_hi);

	//移動
	for (int i=0; i<SelList->Count; i++) SelList->Items[i]->RC.Offset(dx, dy);

	//表示更新
	update_group_frame();
	frg_owner->Invalidate();
	MapViewer->MapUpdate(NULL);

	modify = true;
}

//---------------------------------------------------------------------
//選択中断片の位置を指定比率で伸縮して並べる
// tmpRCが元位置
//---------------------------------------------------------------------
void FragSet::stretch_sel(
	double ratio,	//比率
	int smod)		//伸縮方向 0=同心/ 1=斜め/ 2=左右/ 3=上下/ 4=回転/ 5=縦横
{
	if (SelList->Count<2) return;

	TPoint p_lt = get_sel_org_pos(0, true);	//左上
	TPoint p_c	= get_sel_org_pos(1, true);	//中心

	int cx, cy;
	if (smod==1 || smod==5) {
		cx = p_lt.x;
		cy = p_lt.y;
	}
	else {
		cx = p_c.x;
		cy = p_c.y;
	}

	if (smod==5) {
		if (ratio>=0)
			SelList->Sort(FrgCmp_PosY);
		else
			SelList->Sort(FrgCmp_PosX);
	}

	std::unique_ptr<TFragList> tmp_list(new TFragList());
	Fragment *fp,*dp;
	double cosR, sinR;
	int refx = p_lt.x;
	int refy = p_lt.y;
	for (int i=0; i<SelList->Count; i++) {
		fp = SelList->Items[i];
		TRect *rp = &fp->RC;
		TRect *tp = &fp->tmpRC;
		get_intr_flist(fp, tmp_list.get());
		fp->era_frag();

		//位置変換
		int hx = tp->Width()/2;
		int hy = tp->Height()/2;
		int dx = tp->Left - cx;
		int dy = tp->Top  - cy;
		if (smod!=1 && smod!=5) {
			dx += hx;  dy += hy;
		}

		switch (smod) {
		case 0:	//同心
			rp->Left = (int)(cx + dx*ratio) -hx;
			rp->Top  = (int)(cy + dy*ratio) -hy;
			break;
		case 1:	//斜め
			rp->Left = (int)(cx + dx*ratio);
			rp->Top  = (int)(cy + dy*ratio);
			break;
		case 2:	//左右
			rp->Left = (int)(cx + dx*ratio) -hx;
			rp->Top  = tp->Top;
			break;
		case 3:	//上下
			rp->Left = tp->Left;
			rp->Top  = (int)(cy + dy*ratio) -hy;
			break;
		case 4: //回転
			cosR = cos(ratio * M_PI_2);
			sinR = sin(ratio * M_PI_2);
			rp->Left = (int)(cx + dx*cosR - dy*sinR) - hx;
			rp->Top  = (int)(cy + dx*sinR + dy*cosR) - hy;
			break;

		case 5: //縦横
			//縦一列
			if (ratio>=0) {
				if (ratio < 1.0) {
					rp->Top  = tp->Top + (int)((refy - tp->Top) * ratio);
					rp->Left = tp->Left - (int)(dx * ratio);
				}
				else {
					rp->Top  = refy;
					rp->Left = cx;
				}
			}
			//横一列
			else {
				if (ratio > -1.0) {
					rp->Left = tp->Left - (int)((refx - tp->Left) * ratio);
					rp->Top  = tp->Top + (int)(dy * ratio);
				}
				else {
					rp->Left = refx;
					rp->Top  = cy;
				}
			}
			refy += (tp->Height() + 4);
			if (ratio>=1.0)  refy += (int)(50 * (ratio - 1.0));
			refx += (tp->Width() + 4);
			if (ratio<=-1.0) refx += (int)(50 * (-ratio - 1.0));
			break;
		}
		if (rp->Left < 0) rp->Left = 0;
		if (rp->Top < 0)  rp->Top  = 0;
		if ((rp->Left + hx*2) > scr_wd) rp->Left = scr_wd - hx*2;
		if ((rp->Top + hy*2)  > scr_hi) rp->Top  = scr_hi - hy*2;
	}

	tmp_list->AllShow();	//交叉してた断片を更新

	//表示更新
	SelList->AllShow();
	CLineList->erase_line(TDIR_G2G);
	update_group_frame();
	GInfs->repaint_frame();
	MapViewer->MapUpdate(NULL);
}

//---------------------------------------------------------------------
//グループの位置を指定比率で伸縮
// tmpRCが元位置
//---------------------------------------------------------------------
void FragSet::stretch_grp(
	double ratio,	//比率
	bool sw)		//無所属断片も連動
{
	GInfs->stretch(ratio, sw);
	erase_sym_frm();
	visible_update();
	GInfs->repaint_frame();
}

//---------------------------------------------------------------------
//選択中断片の位置を力学的に伸縮
//---------------------------------------------------------------------
void FragSet::rubber_sel(
	int len,	//関係線長
	int spc)	//断片間隔
{
	if (SelList->Count<2) return;

	Fragment *fp;

	//交叉している断片を取得
	std::unique_ptr<TFragList> tmp_list(new TFragList());
	for (int i=0; i<SelList->Count; i++)
		get_intr_flist(SelList->Items[i], tmp_list.get());

	//移動量サムクリア
	for (int i=0; i<SelList->Count; i++) {
		fp = SelList->Items[i];
		fp->sX = fp->sY = 0;
	}

	std::unique_ptr<TFragList> clst(new TFragList());

	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp0 = SelList->Items[i];
		if (fp0->style==fgsPlate && fp0->OnGfrm) continue;
		clst->Clear(); get_con_flist(fp0, clst.get());
		for (int j=i+1; j<SelList->Count; j++) {
			Fragment *fp1 = SelList->Items[j];
			if (fp1->style==fgsPlate && fp1->OnGfrm) continue;
			int sx = fp1->RC.Left - fp0->RC.Left;
			int sy = fp1->RC.Top - fp0->RC.Top;
			double fs = sqrt((float)(sx*sx + sy*sy));
			double ff;
			if (clst->IndexOf(fp1)!=-1) {
				ArrowLine *ap = CLineList->lp_from_id(TDIR_F2F, fp0->ID, fp1->ID);
				//直接連結
				if (ap) {
					if (ap->fromTag==fp0->ID) {
						sx = ap->x1 - ap->x0;
						sy = ap->y1 - ap->y0;
					}
					else {
						sx = ap->x0 - ap->x1;
						sy = ap->y0 - ap->y1;
					}
					fs = sqrt((float)(sx*sx + sy*sy));
					ff = (fs - len)*0.1;
				}
				//間接連結
				else {
					ff = (fs >= (len*2))? 0.0 : (fs - (len*2))*0.05;
				}

			}
			//分離
			else {
				ff = (fs - spc)*0.01;
			}

			double mx, my;
			if (fs==0) {
				switch (random(4)) {
				case 0: mx = ff;   my = 0.0; break;
				case 1: mx = 0.0;  my = ff;  break;
				case 2: mx = -ff;  my = 0.0; break;
				case 3: mx = 0.0;  my = -ff; break;
				}
			}
			else {
				mx = ff*(sx/fs);
				my = ff*(sy/fs);
			}
			fp0->sX += mx;  fp0->sY += my;
			fp1->sX -= mx;  fp1->sY -= my;
		}
	}

	//移動量チェック
	bool m_flag = false;
	for (int i=0; i<SelList->Count; i++) {
		fp = SelList->Items[i];
		if (floor(fp->sX)>0 || floor(fp->sY)>0) {
			m_flag = true; break;
		}
	}
	if (m_flag) {
		//移動
		for (int i=0; i<SelList->Count; i++) {
			fp = SelList->Items[i];  if (fp==catched_fp) continue;
			fp->era_frag();
			double maxd = 50.0;	//制限
			if (fp->sX>maxd) fp->sX = maxd; else if (fp->sX<-maxd) fp->sX = -maxd;
			if (fp->sY>maxd) fp->sY = maxd; else if (fp->sY<-maxd) fp->sY = -maxd;
			fp->RC.Offset(fp->sX, fp->sY);
			if (fp->RC.Left<0) fp->RC.Left = 0;
			if (fp->RC.Top <0) fp->RC.Top  = 0;
			if (fp->RC.Right>scr_wd)  fp->RC.Left = scr_wd - fp->RC.Width();
			if (fp->RC.Bottom>scr_hi) fp->RC.Top  = scr_hi - fp->RC.Height();
		}

		tmp_list->AllShow();	//交叉してた断片を更新

		//表示更新
		SelList->AllShow();
		CLineList->erase_line();
		update_frg_line();
		update_group_frame();
		update_fg_line();
		GInfs->repaint_frame();
		MapViewer->MapUpdate(NULL);
	}
}

//---------------------------------------------------------------------
//選択中断片位置を回転
//---------------------------------------------------------------------
void FragSet::quick_mov_sel(int mode, double angle)
{
	if (SelList->Count<2) return;
	//中心
	TPoint p = get_sel_org_pos(1);
	int cx = p.x;
	int cy = p.y;

	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		fp->era_frag();
		TRect *rp = &fp->RC;
		int hx = rp->Width()/2;
		int hy = rp->Height()/2;
		int dx = rp->Left - cx;
		int dy = rp->Top  - cy;
		double cosR, sinR;
		dx += hx;  dy += hy;
		switch (mode) {
		case 0:	//上下
			rp->Left = rp->Left;
			rp->Top  = (int)(cy + -dy) -hy;
			break;
		case 1:	//左右
			rp->Left = (int)(cx + -dx) -hx;
			rp->Top  = rp->Top;
			break;
		case 2:	//回転
			cosR = cos(angle);
			sinR = sin(angle);
			rp->Left = (int)(cx + dx*cosR - dy*sinR) - hx;
			rp->Top  = (int)(cy + dx*sinR + dy*cosR) - hy;
			break;
		}
		if (rp->Left<0)  rp->Left = 0;
		if (rp->Top <0)  rp->Top  = 0;
	}

	//表示更新
	update_group_frame();
	frg_owner->Invalidate();
	MapViewer->MapUpdate(NULL);
}

//---------------------------------------------------------------------
//検索
//---------------------------------------------------------------------
int FragSet::find_str(
	UnicodeString wd,	//検索語
	bool sel_sw)		//選択範囲を検索
{
	int n = 0;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		fp->Matched  = false;
		if (!fp->Visible) continue;
		if (sel_sw && !fp->Selected) continue;

		bool found;
		if (EV->FindRegSw) {
			TRegExOptions opt;
			if (!EV->FindCaseSw) opt << roIgnoreCase;
			found = TRegEx::IsMatch(fp->TextStr, wd, opt);
		}
		else
			found = find_mlt(wd, fp->TextStr, EV->FindAndSw, EV->FindNotSw, EV->FindCaseSw);

		fp->Matched = found;

		if (sel_sw) {
			if (found) {
				fp->Changed  = true;
				n++;
			}
			else
				fp->Selected = false;
		}
		else {
			if (found) {
				fp->Selected = true;
				n++;
			}
			else
				fp->Selected = false;
		}
	}

	mak_sel_list();
	erase_sym_frm();
	visible_update();

	//見つかった最初の断片を画面中央へ
	if (n>0) {
		notice_pos = 0;
		to_center(SelList->Items[0], true);
	}

	return n;
}

//---------------------------------------------------------------------
//ジャンプ
//---------------------------------------------------------------------
void FragSet::jump_link(Fragment *fp)
{
	if (!fp) return;
	if (!fp->Visible) return;
	fp->Selected = false;
	fp->show_frag();
	int idx = SelList->IndexOf(fp); if (idx>=0) SelList->Delete(idx);
	if (!fp->LinkName.IsEmpty()) {
		//ヒストリー設定
		UnicodeString txhd = fp->TextStr;
		if (txhd.Length()>10) txhd = txhd.SubString(1, 10);
		if (txhd.Length()<fp->TextStr.Length()) txhd.UCAT_TSTR("...");
		UnicodeString hnam;  hnam.sprintf(_T("%s#%u\t%s"),file_name.c_str(), fp->ID, txhd.c_str());
		if (EV->HistoryPtr==EV->JumpHistoryList->Count)
			EV->JumpHistoryList->Add(hnam);
		else
			EV->JumpHistoryList->Strings[EV->HistoryPtr] = hnam;
		EV->HistoryPtr++;
		//ファイル内アンカー
		if (StartsStr("#", fp->LinkName[1])) {
			to_anchor(fp->LinkName);
		}
		//断片ファイル
		else if (test_ideafrag2(fp->LinkName))
			IdeaFragMainForm->CreateMDIChild(fp->LinkName);
		//その他
		else {
			while (is_KeyDown(VK_SHIFT));
			UnicodeString cmd  = EV->get_associate(fp->LinkName);
			UnicodeString fnam = add_quot_if_spc(fp->LinkName);
			//独自の関連付けで開く
			if (!cmd.IsEmpty())
				::ShellExecute(NULL, _T("open"), cmd.c_str(), fnam.c_str(), NULL, SW_SHOWNORMAL);
			//Windowsの関連付けで開く
			else
				::ShellExecute(NULL, _T("open"), fnam.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
	}
}
//---------------------------------------------------------------------
//選択中の断片からジャンプ
//---------------------------------------------------------------------
void FragSet::jump_sel_link()
{
	for (int i=0; i<SelList->Count; i++) jump_link(SelList->Items[i]);
}


//--------------------------------------------------------------------------------
//印刷
//--------------------------------------------------------------------------------
void FragSet::draw_prn_img(TCanvas *cv, int wd, int hi, int foot_hi)
{
	//断片の右下隅を求める
	int v_wd = 1;
	int v_hi = 1;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (!fp->Visible) continue;
		if (fp->style==fgsPlate && fp->OnGfrm) continue;
		if (fp->RC.Right  >v_wd) v_wd = fp->RC.Right;
		if (fp->RC.Bottom >v_hi) v_hi = fp->RC.Bottom;
	}
	for (int i=0; i<=MAX_GROUP; i++)  {
		grp_inf *gp = &GInfs->Items[i];
		if (gp->Empty || !gp->gfp->Visible) continue;
		if (gp->RC.Right  >v_wd) v_wd = gp->RC.Right;
		if (gp->RC.Bottom >v_hi) v_hi = gp->RC.Bottom;
	}

	//背景を印刷
	if (!EV->PrintMono && EV->PrintBackGr) {
		cv->Brush->Color = frg_owner->Color;
		cv->FillRect(Rect(0, 0, wd, hi));
	}

	if (foot_hi>0) hi -= foot_hi;	//フッタの分を確保

	//倍率を決定
	double p_ratio;
	if (EV->PrintScrSize || (EV->show_DivLine && div_mod!=DVMODE_NON)) {
		p_ratio = std::min(1.00*wd/scr_wd, 1.00*hi/scr_hi);
		v_wd = scr_wd;
		v_hi = scr_hi;
		//画面分割線を印刷
		if (EV->show_DivLine && div_mod!=DVMODE_NON)
			draw_divln(cv, scr_wd, scr_hi, p_ratio);
	}
	else
		p_ratio = std::min(0.95*wd/v_wd, 0.95*hi/v_hi);

	//タイトルを印刷
	draw_title(cv, p_ratio, (int)(v_wd * p_ratio), (int)(v_hi * p_ratio));

	//断片を印刷
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (fp->Visible) fp->print_frag(cv, p_ratio);
	}

	//断片関係線を印刷
	if (EV->show_FragLine) {
		cv->Font->Assign(lblFont);
		cv->Font->Height = (int)(p_ratio * lblFont->Height);
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ap = CLineList->Items[i];
			if (ap->tType!=TDIR_F2F) continue;
			if (ap->Visible)	//Visibleで画面と同じ表示を反映できる
				ap->draw(cv, p_ratio,
					(EV->PrintMono? clBlack : GET_indcol(col_FrgLine)),
					(EV->PrintMono? clBlack : GET_indcol(col_LinLbl)) );
		}
	}

	//グループ枠を印刷
	cv->Pen->Style = psSolid;
	cv->Font->Assign(namFont);
	cv->Font->Height = (int)(p_ratio * namFont->Height);
	bool vflag = (namFont->Name[1]=='@');
	for (int gn=1; gn<=MAX_GROUP; gn++) {
		grp_inf *gp = &GInfs->Items[gn];
		if (gp->Empty || !gp->gfp->Visible) continue;
		TRect rc = gp->RC;
		scale_rect(&rc, p_ratio);
		TRect prc = gp->gfp->PlateRect;
		prc.Offset((gp->gfp->Left + HscrBar->Position), (gp->gfp->Top + VscrBar->Position));
		scale_rect(&prc, p_ratio);
		int dh = vflag? prc.Width()/2 : prc.Height()/2;
		if (EV->PlateOnGfrm && gp->Plate) {
			if (vflag) rc.Right -= dh; else rc.Top += dh;
		}
		int r = std::min(rc.Width(), rc.Height()/4);
		r = std::min(r, (int)(EV->gfrmMaxRnd * p_ratio));
		cv->Brush->Style = bsClear;
		if (EV->PrintMono)
			cv->Pen->Color = clBlack;
		else
			cv->Pen->Color = (gp->col_GrpFrm==Graphics::clNone)? GET_indcol(col_GrpFrm) : gp->col_GrpFrm;
		int lw = (gp->LnWidth==0)? EV->gfrmLineWidth : gp->LnWidth;
		if (EV->FixPrnLWidth>0)
			cv->Pen->Width = (lw>1)? (std::max(EV->FixPrnLWidth, (int)(lw * p_ratio))) : EV->FixPrnLWidth;
		else
			cv->Pen->Width = std::max(1, (int)(lw * p_ratio));
		cv->RoundRect(rc.Left, rc.Top, rc.Right, rc.Bottom, r, r);
		if (!EV->PlateOnGfrm || !gp->Plate) continue;
		if (!gp->Plate->Visible) continue;
		//表札
		cv->Pen->Width = (EV->FixPrnLWidth>0)? EV->FixPrnLWidth : std::max(1, (int)p_ratio);

		cv->Pen->Color = EV->PrintMono? clBlack : EV->col_Border;
		cv->Brush->Color = EV->PrintMono? clWhite : gp->Plate->colBG;
		cv->Brush->Style = bsSolid;
		switch (EV->PlateShape) {
		case 1: case 2:
			//角丸長方形
			r = (EV->PlateShape==2)? dh*2 : dh;
			cv->RoundRect(prc.Left, prc.Top, prc.Right, prc.Bottom, r, r);
			break;
		default:
			//長方形
			cv->FillRect(prc);
			cv->MoveTo(prc.Left,	prc.Top);
			cv->LineTo(prc.Right,	prc.Top);
			cv->LineTo(prc.Right,	prc.Bottom);
			cv->LineTo(prc.Left,	prc.Bottom);
			cv->LineTo(prc.Left,	prc.Top);
		}
		//文字
		cv->Brush->Style = bsClear;
		cv->Font->Color  = EV->PrintMono? clBlack : gp->Plate->colFG;
		if (vflag) {
			vert_textout(cv,
				(int)(prc.Right - EV->plt_mgn*p_ratio), (int)(prc.Top + dh),
				gp->Plate->TextStr);
		}
		else {
			cv->TextOut(
				(int)(prc.Left + dh), (int)(prc.Top + EV->plt_mgn*p_ratio),
				gp->Plate->TextStr);
		}
	}

	//グループ関係線を印刷
	if (EV->show_GroupLine) {
		cv->Font->Assign(lblFont);
		cv->Font->Height = (int)(p_ratio * lblFont->Height);
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ap = CLineList->Items[i];
			if (ap->tType!=TDIR_G2G) continue;
			if (ap->Visible)	//Visibleで画面と同じ表示を反映できる
				ap->draw(cv, p_ratio,
					(EV->PrintMono? clBlack : GET_indcol(col_GrpLine)),
					(EV->PrintMono? clBlack : GET_indcol(col_LinLbl)) );
		}
	}
	//断片・グループ間関係線を印刷
	if (EV->show_FragLine) {
		cv->Font->Assign(lblFont);
		cv->Font->Height = (int)(p_ratio * lblFont->Height);
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ap = CLineList->Items[i];
			if (ap->tType!=TDIR_F2G && ap->tType!=TDIR_G2F) continue;
			if (ap->Visible)	//Visibleで画面と同じ表示を反映できる
				ap->draw(cv, p_ratio, (EV->PrintMono? clBlack : GET_indcol(col_FrgLine)), GET_indcol(col_LinLbl));
		}
	}

	//印刷日時を印字
	if (foot_hi>0) {
		cv->Font->Assign(frg_owner->Font);
		cv->Font->Color  = clBlack;
		cv->Font->Height = -foot_hi;
		cv->Brush->Style = bsClear;
		UnicodeString tstr = FormatDateTime("yyyy-mm-dd hh:nn:ss ", Now());
		cv->TextOut(wd - cv->TextWidth(tstr), hi, tstr);
	}
}

//--------------------------------------------------------------------------------
void FragSet::printout()
{
	all_update();

	int p_wd = ::GetDeviceCaps(Printer()->Handle, HORZRES);
	int p_hi = ::GetDeviceCaps(Printer()->Handle, VERTRES);

	//フッタの文字サイズ(9ポイント)
	int f_hi = 0;
	if (EV->PrintTime)
		f_hi = 9.0 * 0.3527 * p_hi / ::GetDeviceCaps(Printer()->Handle, VERTSIZE);

	Printer()->BeginDoc();
	draw_prn_img(Printer()->Canvas, p_wd, p_hi, f_hi);
	Printer()->EndDoc();
}

//--------------------------------------------------------------------------------
// 画像出力
//--------------------------------------------------------------------------------
bool FragSet::imageout(UnicodeString fnam)
{
	all_update();

	std::unique_ptr<Graphics::TBitmap> tmp_bmp(new Graphics::TBitmap());
	TCanvas *cv = tmp_bmp->Canvas;

	//サイズを求める
	int v_wd = 1;
	int v_hi = 1;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (!fp->Visible) continue;
		if (fp->RC.Right  >v_wd) v_wd = fp->RC.Right;
		if (fp->RC.Bottom >v_hi) v_hi = fp->RC.Bottom;
	}
	for (int i=0; i<=MAX_GROUP; i++)  {
		grp_inf *gp = &GInfs->Items[i];
		if (gp->Empty || !gp->gfp->Visible) continue;
		if (gp->RC.Right  >v_wd) v_wd = gp->RC.Right;
		if (gp->RC.Bottom >v_hi) v_hi = gp->RC.Bottom;
	}
	if (EV->show_DivLine && div_mod!=DVMODE_NON)
		tmp_bmp->SetSize(scr_wd, scr_hi);
	else
		tmp_bmp->SetSize((int)(v_wd * 1.05), (int)(v_hi * 1.05));

	//背景
	cv->Brush->Color = frg_owner->Color;
	cv->FillRect(Rect(0, 0, tmp_bmp->Width, tmp_bmp->Height));

	//画面分割線を描画
	draw_divln(cv, tmp_bmp->Width, tmp_bmp->Height);

	//タイトルを描画
	draw_title(cv, 1.0, tmp_bmp->Width, tmp_bmp->Height);

	//断片を描画
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (fp->Visible) fp->print_frag(cv, 0);
	}

	//断片関係線を描画
	if (EV->show_FragLine) {
		cv->Font->Assign(lblFont);
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ap = CLineList->Items[i];
			if (ap->tType!=TDIR_F2F) continue;
			if (ap->Visible)
				ap->draw(cv, 1.0, GET_indcol(col_FrgLine), GET_indcol(col_LinLbl));
		}
	}

	//グループ枠を描画
	cv->Pen->Style = psSolid;
	cv->Font->Assign(namFont);
	bool vflag = (namFont->Name[1]=='@');
	for (int gn=1; gn<=MAX_GROUP; gn++) {
		grp_inf *gp = &GInfs->Items[gn];
		if (gp->Empty || !gp->gfp->Visible) continue;
		TRect rc  = gp->RC;
		TRect prc = gp->gfp->PlateRect;
		prc.Offset((gp->gfp->Left + HscrBar->Position), (gp->gfp->Top + VscrBar->Position));
		int dh = vflag? prc.Width()/2 : prc.Height()/2;
		if (EV->PlateOnGfrm && gp->Plate) {
			if (vflag) rc.Right -= dh; else rc.Top += dh;
		}
		int r  = std::min(rc.Width(), rc.Height())/4;
		r = std::min(r, EV->gfrmMaxRnd);
		cv->Brush->Style = bsClear;
		cv->Pen->Color = (gp->col_GrpFrm==Graphics::clNone)? GET_indcol(col_GrpFrm) : gp->col_GrpFrm;
		cv->Pen->Width = (gp->LnWidth==0)? EV->gfrmLineWidth : gp->LnWidth;
		cv->RoundRect(rc.left, rc.top, rc.right, rc.bottom, r, r);
		if (!EV->PlateOnGfrm || !gp->Plate) continue;
		if (!gp->Plate->Visible) continue;
		//表札
		cv->Brush->Style = bsSolid;
		switch (EV->PlateShape) {
		case 1: case 2:
			//角丸長方形
			r = (EV->PlateShape==2)? dh*2 : dh;
			cv->Brush->Color = gp->Plate->colBG;
			cv->Pen->Width = 1;
			cv->Pen->Color = EV->col_Border;
			cv->RoundRect(prc.Left, prc.Top, prc.Right, prc.Bottom, r, r);
			break;
		default:
			//長方形
			cv->Brush->Color = gp->Plate->colBG;
			cv->FillRect(prc);
			cv->Brush->Color = EV->col_Border;
			cv->FrameRect(prc);
		}
		//文字
		cv->Brush->Style = bsClear;
		cv->Font->Color  = gp->Plate->colFG;
		if (vflag)
			vert_textout(cv, prc.Right - EV->plt_mgn, prc.Top + dh, gp->Plate->TextStr);
		else
			cv->TextOut(prc.Left + dh, prc.Top + EV->plt_mgn, gp->Plate->TextStr);
	}

	//グループ関係線を描画
	if (EV->show_GroupLine) {
		cv->Font->Assign(lblFont);
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ap = CLineList->Items[i];
			if (ap->tType!=TDIR_G2G) continue;
			if (ap->Visible)
				ap->draw(cv, 1.0, GET_indcol(col_GrpLine), GET_indcol(col_LinLbl));
		}
	}
	//断片・グループ間関係線を描画
	if (EV->show_FragLine) {
		cv->Font->Assign(lblFont);
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ap = CLineList->Items[i];
			if (ap->tType!=TDIR_F2G && ap->tType!=TDIR_G2F) continue;
			if (ap->Visible)
				ap->draw(cv, 1.0, GET_indcol(col_FrgLine), GET_indcol(col_LinLbl));
		}
	}

	//ファイルに保存
	return WIC_save_image(fnam, tmp_bmp.get(), EV->JpegQuality);
}

//--------------------------------------------------------------------------------
//マウス操作
//--------------------------------------------------------------------------------
void __fastcall FragSet::FragMouseDown(Fragment *sp,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (!sp) return;
	if (!sp->Visible) return;

	catched_fp = sp;

	bool jflag = false;

	switch (Button) {
	case mbLeft:
		if (sp->style==fgsImage) FrgList->Sort(FrgCmp_Z);
		lastP = Point(X, Y);

		//リンクマーク上かをチェック
		if (EV->show_LinkMark && !sp->LinkName.IsEmpty()
			&& (sp->style!=fgsImage || test_FileExt(ExtractFileExt(sp->LinkName), ".mp3")))
		{
			bool vflag;
			switch (sp->style) {
			case fgsNormal: vflag = (sp->frgFont->Name[1]=='@'); break;
			case fgsPlate:	vflag = (sp->namFont->Name[1]=='@'); break;
			case fgsSymbol:	vflag = (sp->symFont->Name[1]=='@'); break;
			default:		vflag = false;
			}
			if (Y > (sp->vRC.Bottom - 16)) {
				jflag = vflag? (X < (sp->vRC.Left + 16)) : (X > (sp->vRC.Right - 16));
			}
		}

		if (Shift.Contains(ssShift) && !Shift.Contains(ssCtrl)) {
		//+ Shift キー
			//選択の解除
			if (sp->Selected) {
				sp->Selected = false;
				int idx = SelList->IndexOf(sp);
				if (idx>=0) SelList->Delete(idx);
				if (notice_pos>=0) notice_pos--;
			}
			//選択の追加
			else {
				sp->Selected = true;
			}
			if (sp->style==fgsPlate && sp->OnGfrm)
				GInfs->Items[sp->group.stk[0]].gfp->Repaint();
			if (sp==find_fp) {
				find_fp->Noticed = false;
				find_fp = NULL;
			}
			mak_sel_list();
		}
		//リンクマーク
		else if (jflag) {
			jump_fp			 = sp;
			jump_fp->Jumping = true;
			jump_fp->Changed = true;
		}
		else {
			//+ Shift + Ctrl で関係線接続断片を選択
			if (Shift.Contains(ssShift) && Shift.Contains(ssCtrl)) {
				all_select(false);
				sel_connected(sp);
			}
			//+ Ctrl でグループ選択
			else if (Shift.Contains(ssCtrl)) {
				group_select(sp->group.stk[0], false);
				FragGrid->SetGroupTop(sp->group.stk[0]);
				movgroup = true;
			}
			else if (SelList->Count<=1 || !sp->Selected) {
				all_select(false);
				sp->Selected = true;
				if (sp->style==fgsPlate && sp->OnGfrm)
					GInfs->Items[sp->group.stk[0]].gfp->Repaint();
				FrgList->Sort(FrgCmp_Z);
				FragGrid->SetFrgIndex(sp);
			}
			mak_sel_list();

			if (movskip) {
				moving = movgroup = movskip = false; break;
			}
			if (read_only) break;

			if (sp->style==fgsPlate && sp->OnGfrm) {
				if (!GInfs->Items[sp->group.stk[0]].AllSelected) break;
			}

			//移動開始
			Screen->Cursor = EV->crFragMove;
			moving	= true;
			moved	= false;
		}
		break;

	case mbRight:
		if (SelList->Count<=1 || !sp->Selected) {
			all_select(false);
			sp->Selected = true;
			FrgList->Sort(FrgCmp_Z);
			mak_sel_list();
		}
		break;

	default:
		;
	}

	sp->show_frag();
}

//---------------------------------------------------------------------------
void __fastcall FragSet::FragMouseMove(Fragment *sp, TShiftState Shift, int X, int Y)
{
	if (!moving) return;			//断片の移動中ではない

	TPoint P = Point(X, Y);
	int dx = P.x - lastP.x;
	int dy = P.y - lastP.y;
	int vx = dx;
	int vy = dy;

	if (sp && dx==0 && dy==0) return;	//位置変化無し

	if (!moved) push_all();			//動き始めの時に元の位置を待避

	if (movgroup && !Shift.Contains(ssCtrl)) movgroup = false;

	HRGN hTmpRg;

	//画面端にかかったら移動を中断
	bool over_flag = false;
	if (floating && catched_fp) {
		TRect rc = catched_fp->RC;
		rc.Offset(dx, dy);
		if (rc.Left<0 || rc.Top<0 || rc.Right>scr_wd || rc.Bottom>scr_hi) over_flag = true;
	}
	else {
		for (int i=0; i<SelList->Count; i++) {
			TRect rc  = SelList->Items[i]->RC;
			rc.Offset(dx, dy);
			if (rc.Left<0 || rc.Top<0 || rc.Right>scr_wd || rc.Bottom>scr_hi) {
				over_flag = true; break;
			}
		}
	}
	if (over_flag) {
		Screen->Cursor = crNo;
		return;
	}
	else if (!pickuped)
		Screen->Cursor = EV->crFragMove;

	//画面右・下端でのスクロール処理
	int  x_scr_area = frg_owner->ClientWidth  - 32;
	int  y_scr_area = frg_owner->ClientHeight - 32;
	bool r_scrolled = (P.x>x_scr_area && lastP.x>x_scr_area && dx>0);
	bool b_scrolled = (P.y>y_scr_area && lastP.y>y_scr_area && dy>0);
	if (r_scrolled || b_scrolled) {
		if (r_scrolled) {
			HscrBar->Position += 16;
			dx += 16;
			vx = 0;
		}
		if (b_scrolled) {
			VscrBar->Position += 16;
			dy += 16;
			vy = 0;
		}
		all_update();
	}

	//画面左・上端でのスクロール処理
	bool l_scrolled = (HscrBar->Position>0 && P.x<32 && lastP.x<32 && dx<0);
	bool t_scrolled = (VscrBar->Position>0 && P.y<32 && lastP.y<32 && dy<0);
	if (l_scrolled || t_scrolled) {
		if (l_scrolled) {
			HscrBar->Position -= 16;
			dx -= 16;
			vx = 0;
		}
		if (t_scrolled) {
			VscrBar->Position -= 16;
			dy -= 16;
			vy = 0;
		}
		all_update();
	}

	if (!sp) all_update();

	//移動前のリージョン作成
	HRGN hRg0 = ::CreateRectRgn(0, 0, 0, 0);
	for (int i=0; i<SelList->Count; i++) {
		TRect *vrp = &SelList->Items[i]->vRC;
		hTmpRg = ::CreateRectRgn(vrp->left, vrp->top, vrp->right, vrp->bottom);
		::CombineRgn(hRg0, hRg0, hTmpRg, RGN_OR);
		::DeleteObject(hTmpRg);
	}

	//交叉チェック
	std::unique_ptr<TFragList> tmp_list(new TFragList());
	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		if (fp->style==fgsPlate && fp->OnGfrm) continue;
		fp->era_frag();
		//断片
		get_intr_flist(fp, tmp_list.get());
		//グループ枠
		for (int j=0; j<MAX_IN_GRP; j++) {
			if (fp->group.stk[j]==-1) break;
			grp_inf *gp = &GInfs->Items[fp->group.stk[j]];
			if (!fp->vRC.IntersectsWith(gp->gfp->BoundsRect)) continue;
			gp->RepaintFrm = true;
			inh_Grid = true;
		}
	}

	//移動
	for (int i=0; i<SelList->Count; i++) {
		Fragment *fp = SelList->Items[i];
		TRect *rp  = &fp->RC;
		TRect *vrp = &fp->vRC;
		if (floating) {
			if (catched_fp==fp) rp->Offset(dx, dy);
		}
		else
			rp->Offset(dx, dy);
		if (!r_scrolled && !l_scrolled)	vrp->Left += vx;
		if (!b_scrolled && !t_scrolled)	vrp->Top  += vy;
	}

	//グループ枠の変化チェック
	for (int gn=1; gn<=MAX_GROUP; gn++) {
		grp_inf *gp = &GInfs->Items[gn];	if (!gp->RepaintFrm) continue;
		TRect rc = Rect(0, 0, 0, 0);
		for (int i=0; i<FrgList->Count; i++) {
			Fragment *fp = FrgList->Items[i];
			if (pickuped && fp->Selected) continue;
			if (fp->in_group(gn)==-1) continue;
			if (!fp->Visible) continue;
			if (fp->style==fgsPlate && fp->OnGfrm) continue;
			::UnionRect(&rc, &rc, &fp->RC);
		}
		//変化なし
		if (rc==gp->cRC)
			gp->RepaintFrm = false;
		//変化有り
		else {
			gp->cRC = rc;
			gp->RepaintLn = true;
		}
	}

	//変化しているグループ関係線を一旦消す
	for (int i=0; i<CLineList->Count; i++) {
		ArrowLine *ap = CLineList->Items[i];
		if (ap->tType!=TDIR_G2G) continue;
		if (GInfs->Items[ap->fromTag].RepaintLn || GInfs->Items[ap->toTag].RepaintLn)
			ap->erase(frg_owner);
	}

	//移動後のリージョン作成
	HRGN hRg1 = ::CreateRectRgn(0, 0, 0, 0);
	for (int i=0; i<SelList->Count; i++) {
		TRect *vrp = &SelList->Items[i]->vRC;
		hTmpRg = ::CreateRectRgn(vrp->Left, vrp->Top, vrp->Right, vrp->Bottom);
		::CombineRgn(hRg1, hRg1, hTmpRg, RGN_OR);
		::DeleteObject(hTmpRg);
	}

	//移動中に交叉した断片を更新
	HRGN hOrgRg = (HRGN)::SelectObject(frg_owner->Canvas->Handle, hRg0);
	tmp_list->AllShow();

	//移動後の断片を更新
	::CombineRgn(hRg0, hRg1, NULL, RGN_COPY);
	for (int i=SelList->Count-1; i>=0; i--) {
		Fragment *fp = SelList->Items[i];
		if (fp->style==fgsPlate && fp->OnGfrm) continue;
		fp->show_frag();
		TRect *vrp = &fp->vRC;
		hTmpRg = ::CreateRectRgn(vrp->Left, vrp->Top, vrp->Right, vrp->Bottom);
		::CombineRgn(hTmpRg, hRg0, hTmpRg, RGN_XOR);
		::CombineRgn(hRg1, hRg1, hTmpRg, RGN_AND);
		::DeleteObject(hTmpRg);
		::SelectObject(frg_owner->Canvas->Handle, hRg1);
	}

	hTmpRg = ::CreateRectRgn(0, 0, frg_owner->ClientWidth, frg_owner->ClientHeight);
	::SelectObject(frg_owner->Canvas->Handle, hTmpRg);

	//断片関係線更新
	if (EV->show_FragLine) {
		frg_owner->Canvas->Font->Assign(lblFont);
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ap = CLineList->Items[i];
			if (ap->tType!=TDIR_F2F) continue;
			Fragment *fp0 = FrgList->id_to_fp(ap->fromTag);	if (!fp0) continue;
			Fragment *fp1 = FrgList->id_to_fp(ap->toTag);	if (!fp1) continue;
			if (!fp0->Selected && !fp1->Selected) continue;
			if (fp0->style==fgsPlate && fp0->OnGfrm) continue;
			if (fp1->style==fgsPlate && fp1->OnGfrm) continue;
			ap->erase(frg_owner);
			octrect xrc0, xrc1;
			set_octrect(&fp0->vRC, &xrc0);
			set_octrect(&fp1->vRC, &xrc1);
			if (!ap->set_pos(xrc0, xrc1, HscrBar->Position, VscrBar->Position,
				(fp0->style==fgsJunction), (fp1->style==fgsJunction))) continue;
			ap->draw(frg_owner->Canvas, 0, GET_indcol(col_FrgLine), GET_indcol(col_LinLbl));
		}
	}

	//グループ枠更新
	for (int gn=1; gn<=MAX_GROUP; gn++) {
		grp_inf *gp = &GInfs->Items[gn];  if (!gp->RepaintFrm || gp->Empty) continue;
		GInfs->calc_frame(gn, pickuped);
	}

	//グループ関係線更新
	frg_owner->Canvas->Font->Assign(lblFont);
	for (int i=0; i<CLineList->Count; i++) {
		ArrowLine *ap = CLineList->Items[i];
		if (ap->tType!=TDIR_G2G) continue;
		grp_inf *gp0 = &GInfs->Items[ap->fromTag];
		grp_inf *gp1 = &GInfs->Items[ap->toTag];
		if (gp0->cRC.IsEmpty() || gp1->cRC.IsEmpty()) continue;
		octrect xrc0 = gp0->gfp->FrmOctRect;
		octrect xrc1 = gp1->gfp->FrmOctRect;
		if (!ap->set_pos(xrc0, xrc1, HscrBar->Position, VscrBar->Position)) continue;
		if (!EV->show_GroupLine) continue;
		Fragment *np = gp0->Plate;
		bool nflag = np? np->Visible : false;
		if (!nflag && !gp0->Visible) continue;
		np = gp1->Plate;
		nflag = np? np->Visible : false;
		if (!nflag && !gp1->Visible) continue;
		ap->draw(frg_owner->Canvas, 0, GET_indcol(col_GrpLine), GET_indcol(col_LinLbl));
	}

	//断片・グループ間関係線更新
	if (EV->show_FragLine) {
		frg_owner->Canvas->Font->Assign(lblFont);
		for (int i=0; i<CLineList->Count; i++) {
			ArrowLine *ap = CLineList->Items[i];
			Fragment *fp;
			grp_inf *gp;
			if (ap->tType!=TDIR_F2G && ap->tType!=TDIR_G2F) continue;
			octrect xrc0, xrc1;
			//断片→グループ
			if (ap->tType==TDIR_F2G) {
				fp = FrgList->id_to_fp(ap->fromTag);	if (!fp) continue;
				if (fp->style==fgsPlate && fp->OnGfrm) continue;
				set_octrect(&fp->vRC, &xrc0);
				gp = &GInfs->Items[ap->toTag];
				if (!fp->Selected && !gp->RepaintFrm) continue;
				ap->erase(frg_owner);
				xrc1 = gp->gfp->FrmOctRect;
			}
			//グループ→断片
			else {
				fp = FrgList->id_to_fp(ap->toTag);	if (!fp) continue;
				if (fp->style==fgsPlate && fp->OnGfrm) continue;
				set_octrect(&fp->vRC, &xrc1);
				gp = &GInfs->Items[ap->fromTag];
				if (!fp->Selected && !gp->RepaintFrm) continue;
				ap->erase(frg_owner);
				xrc0 = gp->gfp->FrmOctRect;
			}
			if (::IsRectEmpty(&gp->cRC)) continue;
			if (ap->tType==TDIR_F2G) {
				if (!ap->set_pos(xrc0, xrc1, HscrBar->Position, VscrBar->Position,
					(fp->style==fgsJunction),false)) continue;
			}
			else {
				if (!ap->set_pos(xrc0, xrc1, HscrBar->Position, VscrBar->Position,
					false, (fp->style==fgsJunction))) continue;
			}
			ap->draw(frg_owner->Canvas, 0, GET_indcol(col_FrgLine), GET_indcol(col_LinLbl));
		}
	}

	for (int gn=1; gn<MAX_GROUP; gn++) {
		grp_inf *gp = &GInfs->Items[gn];
		gp->RepaintFrm = gp->RepaintLn = false;
	}

	::SelectObject(frg_owner->Canvas->Handle, hOrgRg);
	::DeleteObject(hRg0);
	::DeleteObject(hRg1);
	::DeleteObject(hTmpRg);

	lastP = P;
	moved = true;
}

//---------------------------------------------------------------------------
void __fastcall FragSet::FragMouseUp(Fragment *sp,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	inh_Grid = false;
	Screen->Cursor = crDefault;

	if (moving) {
		//移動中
		moving = false;
		if (moved) {
			moved = false;
			//グリッドへスナップ
			if (EV->SnapGrid) {
				for (int i=0; i<SelList->Count; i++) {
					Fragment *fp = SelList->Items[i];
					TRect *rp  = &fp->RC;
					TRect *vrp = &fp->vRC;
					int dx = rp->Left % EV->GridSize;
					int dy = rp->Top % EV->GridSize;
					dx = (dx<EV->GridSize/2)? -dx : EV->GridSize - dx;
					dy = (dy<EV->GridSize/2)? -dy : EV->GridSize - dy;
					rp->Offset(dx, dy);
					vrp->Offset(dx, dy);
				}
			}

			if (!movgroup && Shift.Contains(ssCtrl) && !Shift.Contains(ssShift)) {
				// + Ctrl キー でのグループ移動
				grp_stk tmpstk;
				for (int i=0; i<SelList->Count; i++) {
					Fragment *fp = SelList->Items[i];
					//全選択されていないグループスタックは解除
					int j = 0;
					while (j<MAX_IN_GRP) {
						int gx = fp->group.stk[j]; if (gx<=0) break;
						if (!GInfs->Items[gx].AllSelected)
							fp->rel_group(gx);
						else
							j++;
					}

					int gn = fp->group.stk[0];
					if (EV->ShowDstGrp) GInfs->Items[gn].Visible = true;

					//断片の実質位置を取得
					TRect rc1 = fp->vRC;
					if (fp->style==fgsPlate && fp->OnGfrm) {
						//グループ枠上の表札
						TGrpFrame *sfp = GInfs->Items[fp->group.stk[0]].gfp;
						rc1 = sfp->PlateRect;
						rc1.Offset(sfp->Left, sfp->Top);
					}
					//移動先のグループスタックを追加
					GInfs->gstk_from_pos(rc1.left, rc1.top, &tmpstk);
					for (int j=0; j<MAX_IN_GRP; j++) {
						gn = tmpstk.stk[j]; if (gn<1) break;
						grp_inf *gp = &GInfs->Items[gn];
						if (!gp->Visible || gp->Empty) continue;
						if (gp->gfp->BoundsRect.Contains(rc1)) fp->set_group(gn, true);
					}
					//表札のチェック
					if (fp->style==fgsPlate) {
						gn = fp->group.stk[0];
						//無所属なら解除
						if (gn==0)
							fp->style = fgsNormal;
						//すでに表札があれば解除
						else {
							Fragment *np = GInfs->get_name_plate(gn, NULL);
							if (np && np!=fp) {
								fp->era_frag();
								fp->style = fgsNormal; //表札解除
							}
						}
					}

					fp->Changed = true;
					fp->show_frag();
				}
				CLineList->erase_line(TDIR_F2F);
				update_frg_line();
				update_grp_empty();
				IdeaFragMainForm->UpdateGrpLst();
			}
			frg_owner->Invalidate();
			for (int i=0; i<SelList->Count; i++) SelList->Items[i]->ModTime = Now();
			modify = true;
		}
		else if (EV->ImageIsBack && GInfs->exist_img) {
			erase_sym_frm();
			visible_update();
			GInfs->repaint_frame();
		}
		movgroup = pickuped = false;
	}
	else if (jump_fp) {
		//ジャンプ
		jump_fp->Jumping = false;
		if (jump_fp==sp) {
			jump_link(jump_fp);
			jump_fp->Changed = true;
			jump_fp->show_frag();
		}
		else
			all_update();
		jump_fp = NULL;
	}

	catched_fp = NULL;
}

//---------------------------------------------------------------------------
void __fastcall FragSet::FragDblClick(Fragment *sp)
{
	if (!sp) return;

	//ジャンプ
	if (is_KeyDown(VK_SHIFT))
		jump_link(sp);
	//プロパティ
	else if (!read_only) {
		if (SelList->Count==0) return;
		if (sp->style==fgsJunction) return;

		FrgPropDlg->prpFS = this;

		if (SelList->Count>1) {
			for (int i=0; i<FrgList->Count; i++) {
				Fragment *fp = FrgList->Items[i];
				if (fp->style==fgsJunction) fp->Selected = false;
			}
			mak_sel_list();
		}

		FrgPropDlg->prpFrag   = (SelList->Count>1)? NULL : (sp? sp : get_sel_single());
		FrgPropDlg->swNewFrag = false;
		FrgPropDlg->ShowModal();
		if (moving) {
			moving = movgroup = false;
			Screen->Cursor = crDefault;
		}
		IdeaFragMainForm->CloseIME();
		if (SelList->Count>0) movskip = true;	//DblClick後のOnMouseDownによる移動を回避
	}
}
//---------------------------------------------------------------------------
