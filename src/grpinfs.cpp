//----------------------------------------------------------------------//
//	グループ情報のクラス												//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include "UserFunc.h"
#include "EnvData.h"
#include "grpinfs.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall GrpInfs::GrpInfs(TForm* AOwner, TFragList *lst)
{
	FrgList = lst;
	gcv = AOwner->Canvas;

	col_GrpFrm = EV->col_GrpFrm;
	namFont    = EV->namFont;

	for (int i=0; i<=MAX_GROUP; i++) {
		TGrpFrame *sp  = new TGrpFrame(AOwner);
		sp->Parent	   = (TForm*)AOwner;
		sp->Pen->Color = col_GrpFrm;
		sp->Pen->Width = EV->gfrmLineWidth;
		sp->Enabled    = false;
		sp->Visible    = false;
		grp_inf *gp = &Items[i];
		gp->gfp 		= sp;
		gp->RC			= Rect(0, 0, 0, 0);
		gp->cRC 		= Rect(0, 0, 0, 0);
		gp->Area		= 0;
		gp->col_GrpFrm	= clNone;
		gp->Visible 	= true;
		gp->Selected	= false;
		gp->AllSelected = false;
		gp->RepaintFrm	= false;
		gp->RepaintLn	= false;
		gp->Empty		= true;
		gp->TotalSize	= 0;
		gp->Plate		= NULL;
		gp->IncludeGrp	= 0;
		gp->LnWidth 	= 0;
	}

	cur_group = 1;
	cur_gfrm  = 0;
	exist_img = false;
}
//---------------------------------------------------------------------------
__fastcall GrpInfs::~GrpInfs()
{
}

//---------------------------------------------------------------------------
//グループ空状態フラグを更新
//---------------------------------------------------------------------------
void __fastcall GrpInfs::update_empty()
{
	grp_inf *gp;
	for (int gn=0; gn<=MAX_GROUP; gn++)  {
		gp = &Items[gn];
		gp->TotalSize  = 0;
		gp->Plate	   = NULL;
		gp->IncludeGrp = 0;
	}
	exist_img = false;

	if (FrgList) {
		for (int i=0; i<FrgList->Count; i++) {
			Fragment *fp = FrgList->Items[i];
			for (int j=0; j<MAX_IN_GRP; j++) {
				if (fp->group.stk[j]==-1) break;
				gp = &Items[fp->group.stk[j]];
				gp->TotalSize++;
				if (fp->style==fgsPlate && j==0) gp->Plate = fp;
				if (j>gp->IncludeGrp) gp->IncludeGrp = j;
			}
			if (fp->style==fgsImage) exist_img = true;
		}
	}
	for (int gn=0; gn<=MAX_GROUP; gn++)  {
		gp = &Items[gn];
		gp->Empty = (gp->TotalSize == 0);
		Fragment *np = gp->Plate;	if (!np) continue;
		if (EV->show_GroupFrame && gp->Visible)
			np->OnGfrm = (EV->PlateOnGfrm && (EV->OnlyNpHideGfrm? (gp->TotalSize>1) : true));
		else
			np->OnGfrm = false;
	}
}
//---------------------------------------------------------------------------
//最初の空グループのインデックスを取得
//---------------------------------------------------------------------------
int __fastcall GrpInfs::get_empty_idx()
{
	int gn = -1;
	for (int i=1; i<=MAX_GROUP; i++)
		if (Items[i].Empty) { gn = i; break; }
	if (gn==-1) msgbox_WARN("これ以上グループ化できません");
	return gn;
}

//---------------------------------------------------------------------------
//全体が選択されているグループの最大のものを取得(なければ -1)
//---------------------------------------------------------------------------
int __fastcall GrpInfs::get_sel_max()
{
	int gn = -1;
	int sgcnt[MAX_GROUP + 1];
	for (int i=0; i<=MAX_GROUP; i++) sgcnt[i] = 0;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (!fp->Selected) continue;
		for (int j=0; j<MAX_IN_GRP; j++) {
			if (fp->group.stk[j]==-1) break;
			sgcnt[fp->group.stk[j]]++;
		}
	}
	int maxn = 0;
	for (int i=0; i<=MAX_GROUP; i++) {
		int n = sgcnt[i];	if (n==0) continue;
		if (n<Items[i].TotalSize) continue;
		if (maxn<n) {
			gn	 = i;
			maxn = n;
		}
	}
	return gn;
}

//---------------------------------------------------------------------------
//表札文字列を取得
//---------------------------------------------------------------------------
Fragment * __fastcall GrpInfs::get_name_plate(
	int gn,					//グループ番号
	UnicodeString *name)	//[o] 表札 (無い場合は "グループ n")
{
	if (name) *name = EmptyStr;
	Fragment *np = NULL;
	if (gn>=1 && gn<=MAX_GROUP) {
		if (!Items[gn].Empty) {
			np = Items[gn].Plate;
			if (np && name) *name = np->TextStr;
		}
		if (!np) {
			if (name) name->sprintf(_T("グループ %2u"), gn);
		}
	}
	else if (gn==0) {
		if (name) *name = "無所属";
	}
	return np;
}

//---------------------------------------------------------------------------
//グループ枠および表札サイズの算出
//---------------------------------------------------------------------------
void __fastcall GrpInfs::calc_frame(
	int gn,			//グループ番号
	bool pickuped,	//ピックアップ中	(default = false)
	TRect *crc)		//[o]				(default = NULL)
{
	if (gn<1 || gn>MAX_GROUP) return;

	//グループ枠のサイズ設定
	grp_inf *gp = &Items[gn];
	TRect rc  = Rect(0, 0, 0, 0);
	TRect vrc = Rect(0, 0, 0, 0);
	if (crc) *crc = Rect(0, 0, 0, 0);
	if (gp->Empty) return;

	Fragment *np = gp->Plate;
	bool vflag = (namFont->Name[1]=='@');
	int lw = (gp->LnWidth==0)? EV->gfrmLineWidth : gp->LnWidth;
	int gx0 = 999999;
	int gx1 = -99999;
	int gy0 = 999999;
	int gy1 = -99999;
	for (int i=0; i<FrgList->Count; i++) {
		Fragment *fp = FrgList->Items[i];
		if (!gp->AllSelected && pickuped && fp->Selected) continue;
		if (!fp->Visible) continue;
		int gl = fp->in_group(gn);   if (gl==-1) continue;
		if (fp->style==fgsPlate && fp->OnGfrm) continue;

		//有効範囲を求める
		::UnionRect(&rc, &rc,  &fp->RC);
		::UnionRect(&vrc,&vrc, &fp->vRC);
		//余白を考慮した最大範囲を求める
		TRect xrc = fp->RC;
		int xmg = 16 + gl * (8 + lw);
		InflateRect(&xrc, xmg, xmg);
		if (EV->PlateOnGfrm) {
			//枠上表札の重なりを避ける
			int pmg = 0;
			int ph = abs((int)namFont->Height) + EV->plt_mgn*2 - 4;
			bool lst_p = false;
			for (int j=0; j<=gl; j++) {
				if (Items[fp->group.stk[j]].Plate) {
					pmg += lst_p? ph : ph/2;
					lst_p = true;
				}
				else {
					pmg += lst_p? ph/2 : 0;
					lst_p = false;
				}
			}
			if (vflag) xrc.Right += pmg; else xrc.Top -= pmg;
		}
		if (xrc.Left  <gx0) gx0 = xrc.Left;
		if (xrc.Right >gx1) gx1 = xrc.Right;
		if (xrc.Top   <gy0) gy0 = xrc.Top;
		if (xrc.Bottom>gy1) gy1 = xrc.Bottom;
	}

	//表札幅が範囲より大きい場合の補正
	if (np) {
		if (np->Visible && np->OnGfrm) {
			int pw = 0;
			if (EV->PlateOnGfrm) {
				gcv->Font->Assign(namFont);
				pw = gcv->TextWidth(np->TextStr) + abs((int)namFont->Height) + EV->plt_mgn*2;
				int rh;
				if (EV->gfrmMaxRnd>1) {
					int r = std::min(pw/4, EV->gfrmMaxRnd);
					if (r%2==0) r++;
					rh = r/2 + 1;
				}
				else
					rh = 8;
				pw += rh;
				pw += 16;
			}
			if (!rc.IsEmpty()) {
				//縦
				if (vflag) {
					int h = rc.Height();
					if (h<pw) {
						int d = (pw - h)/2;
						rc.Inflate(0, d);
						vrc.Inflate(0, d);
					}
				}
				//横
				else {
					int w = rc.Width();
					if (w<pw) {
						int d = (pw - w)/2;
						rc.Inflate(d, 0);
						vrc.Inflate(d, 0);
					}
				}
			}
		}
	}

	if (crc) *crc = rc;

	//余白の調整
	int xmg_0 = 0, xmg_1 = 0, ymg_0 = 0, ymg_1 = 0;
	if (!rc.IsEmpty()) {
		if (rc.Left>gx0)   {xmg_0 = (rc.Left - gx0);   rc.Left   -= xmg_0; }
		if (rc.Right<gx1)  {xmg_1 = (gx1 - rc.Right);  rc.Right  += xmg_1; }
		if (rc.Top>gy0)    {ymg_0 = (rc.Top - gy0);    rc.Top    -= ymg_0; }
		if (rc.Bottom<gy1) {ymg_1 = (gy1 - rc.Bottom); rc.Bottom += ymg_1; }
		vrc.Left   -= xmg_0;
		vrc.Right  += xmg_1;
		vrc.Top    -= ymg_0;
		vrc.Bottom += ymg_1;
	}

	//表札による調整
	TGrpFrame *sp = gp->gfp;
	sp->PlateRect = Rect(0, 0, 0, 0);
	if (np) {
		int fh = get_FontHeight(namFont, EV->plt_mgn*2);
		if (np->Visible) {
			if (np->OnGfrm) {
				sp->Font	   = namFont;
				sp->PlateAlign = EV->PlateAlign;
				int dh = fh/2;
				if (vflag) {
					if (dh > (xmg_1 - 4)) dh += (dh - (xmg_1 - 4));
					rc.right  += dh;
					vrc.right += dh;
				}
				else {
					if (dh > (ymg_0 - 4)) dh += (dh - (ymg_0 - 4));
					rc.top  -= dh;
					vrc.top -= dh;
				}
			}
			else {
				if (!vrc.IntersectsWith(np->vRC)) np->era_frag();
				if (!gp->Visible) {
					rc  = np->RC;
					vrc = np->vRC;
				}
			}
		}
		int frm_w = rc.Width();
		int frm_h = rc.Height();

		//表札長方形を設定
		if (EV->PlateOnGfrm && np) {
			gcv->Font->Assign(namFont);
			//縦書き
			if (vflag) {
				int y0 = sp->Pen->Width/2;
				int x0 = y0 + frm_w - fh/2 -1;
				int w  = frm_w - sp->Pen->Width + 1;
				int h  = frm_h - sp->Pen->Width + 1;
				if (sp->Pen->Width==0) { --w; --h; }
				int rh;
				if (EV->gfrmMaxRnd>1) {
					int r = ((w<h)? w : h)/4;
					r = std::min(r, EV->gfrmMaxRnd);
					if (r%2==0) r++;
					rh = r/2 +1;
				}
				else
					rh = 8;
				if (np->Visible) {
					int tw = gcv->TextWidth(np->TextStr) + fh;
					int yt0;
					switch (sp->PlateAlign) {
					case 1:	 yt0 = y0 + (frm_h - tw)/2; 		break;	//中央
					case 2:  yt0 = y0 + (frm_h - tw -rh - 4); 	break;	//下
					default: yt0 = y0 + rh +4;							//上
					}
					sp->PlateRect = Rect(x0-fh/2-(fh%2), yt0, x0+fh/2, yt0+tw);
				}
			}
			//横書き
			else {
				int x0 = sp->Pen->Width/2;
				int y0 = x0 + fh/2;
				int w  = frm_w - sp->Pen->Width + 1;
				int h  = frm_h - sp->Pen->Width + 1;
				if (sp->Pen->Width==0) { --w; --h; }
				int rh;
				if (EV->gfrmMaxRnd>1) {
					int r = ((w<h)? w : h)/4;
					r = std::min(r, EV->gfrmMaxRnd);
					if (r%2==0) r++;
					rh = r/2 +1;
				}
				else
					rh = 8;
				if (np->Visible) {
					int tw = gcv->TextWidth(np->TextStr) + fh;
					int xt0;
					switch (sp->PlateAlign) {
					case 1:	 xt0 = x0 + (frm_w - tw)/2; 		break;	//中央
					case 2:  xt0 = x0 + (frm_w - tw -rh - 4); 	break;	//右
					default: xt0 = x0 + rh +4;							//左
					}
					sp->PlateRect = Rect(xt0, y0 - fh/2, xt0 + tw, y0 + fh/2 + (fh%2));
				}
			}
		}
	}

	gp->gfp->SetVRect(vrc);
	gp->RC	 = rc;
	gp->Area = rc.Width() * rc.Height();
}

//---------------------------------------------------------------------------
//グループ枠と表示を更新
//---------------------------------------------------------------------------
void __fastcall GrpInfs::update_frame(bool pickuped)
{
	for (int gn=1; gn<=MAX_GROUP; gn++) {
		//グループ枠の表示設定
		grp_inf   *gp = &Items[gn];
		TGrpFrame *sp = gp->gfp;
		if (cur_gfrm==gn)
			sp->Pen->Color = EV->col_selRct;
		else
			sp->Pen->Color = (gp->col_GrpFrm==clNone)? GET_indcol(col_GrpFrm) : gp->col_GrpFrm;
		int lw = (gp->LnWidth==0)? EV->gfrmLineWidth : gp->LnWidth;
		sp->Pen->Width = lw;
		Fragment *np = gp->Plate;
		gp->gfp->Plate = np;
		if (EV->show_GroupFrame && gp->Visible) {
			//表札有
			if (np) {
				sp->Visible = EV->OnlyNpHideGfrm? (gp->TotalSize>1) : true;
				np->OnGfrm	= (sp->Visible && EV->PlateOnGfrm);
			}
			//表札無
			else
				sp->Visible = EV->NoNpHideGfrm? false : !gp->Empty;
		}
		else {
			sp->Visible = false;
			if (np) np->OnGfrm = false;
		}

		calc_frame(gn, pickuped, &gp->cRC);
	}
}

//---------------------------------------------------------------------------
//グループ枠を再描画
//---------------------------------------------------------------------------
void __fastcall GrpInfs::repaint_frame(
	bool sw)	//true = 強制再描画	(default = false)
{
	for (int i=1; i<=MAX_GROUP; i++) {
		grp_inf *gp = &Items[i];
		if (sw || (EV->ImageIsBack && exist_img) || gp->RepaintFrm) {
			if (gp->gfp->Visible) gp->gfp->Repaint();
			gp->RepaintFrm = false;
		}
	}
}

//---------------------------------------------------------------------------
//位置からグループを取得
// 戻り値: 1～/ なければ 0
//---------------------------------------------------------------------------
int __fastcall GrpInfs::group_from_pos(int X, int Y)
{
	TPoint p = Point(X, Y);
	int smin = 0x7fffffff;
	int gn = 0;
	for (int i=1; i<=MAX_GROUP; i++) {
		grp_inf *gp = &Items[i];
		Fragment *np = gp->Plate;
		bool nflag = np? np->Visible : false;
		if ((!gp->Visible && !nflag) || gp->Empty) continue;
		TRect rc = gp->gfp->BoundsRect;
		if (rc.PtInRect(p)) {
			int s = rc.Width() * rc.Height();
			if (s<smin) {
				smin = s; gn = i;
			}
		}
	}
	return gn;
}
//---------------------------------------------------------------------------
//位置からグループスタックを取得
// 戻り値: 1～ (sp->stk[0])/ なければ 0
//---------------------------------------------------------------------------
int __fastcall GrpInfs::gstk_from_pos(
	int X, int Y,
	grp_stk *sp)	//[o] スタック
{
	//位置からグループスタックを仮設定
	for (int i=0; i<MAX_IN_GRP; i++) sp->stk[i] = ((i==0)? 0 : -1);
	TPoint p = Point(X, Y);
	int plvl = 0;
	for (int i=1; i<=MAX_GROUP; i++) {
		grp_inf *gp = &Items[i];
		Fragment *np = gp->Plate;
		bool nflag = np? np->Visible : false;
		if ((!gp->Visible && !nflag) || gp->Empty) continue;
		TRect rc = gp->gfp->BoundsRect;
		if (rc.PtInRect(p)) {
			for (int j=0; j<MAX_IN_GRP; j++) {
				if (sp->stk[j]>0) continue;
				sp->stk[j] = i;
				plvl++;
				break;
			}
		}
	}

	if (plvl>0) {
		//同じグループ構成の断片を見つけてスタック順をコピー
		bool fflag = false;
		for (int i=0; i<FrgList->Count; i++) {
			Fragment *fp = FrgList->Items[i];
			int flvl = 0;	//断片のスタック数
			for (int j=0; j<MAX_IN_GRP; j++)
				if (fp->group.stk[j]>0) flvl++; else break;
			if (flvl!=plvl) continue;
			int fn = 0;	//一致した数
			for (int j=0; j<plvl; j++) {
				for (int k=0; k<flvl; k++) {
					if (sp->stk[j] == fp->group.stk[k]) {
						fn++; break;
					}
				}
			}
			if (flvl==fn) {
				//構成が一致したら順序をコピー
				for (int j=0; j<plvl; j++) sp->stk[j] = fp->group.stk[j];
				fflag = true;
				break;
			}
		}
		if (!fflag) {
			//一致する断片が無ければ面積順で並び替え
			for (int j=0; j<plvl-1; j++) {
				for (int k=j+1; k<plvl; k++) {
					if (Items[sp->stk[k]].Area < Items[sp->stk[j]].Area) {
						int tmp = sp->stk[k];  sp->stk[k] = sp->stk[j];  sp->stk[j] = tmp;
					}
				}
			}
		}
	}
	return sp->stk[0];
}
//---------------------------------------------------------------------------
//位置からグループ枠を取得
// 戻り値: 1～/ なければ 0
//---------------------------------------------------------------------------
int __fastcall GrpInfs::gfrm_from_pos(int X, int Y)
{
	TPoint p = Point(X, Y);
	int gn = 0;
	for (int i=1; i<=MAX_GROUP; i++) {
		grp_inf *gp = &Items[i];
		Fragment *np = gp->Plate;
		bool nflag = np? np->Visible : false;
		if ((!gp->Visible && !nflag) || gp->Empty) continue;
		TRect rc = gp->gfp->FrmRect;
		int mgn = 6 + ((gp->LnWidth==0)? EV->gfrmLineWidth : gp->LnWidth);
		rc.Inflate(mgn, mgn);
		if (!rc.PtInRect(p)) continue;
		rc.Inflate(-mgn, -mgn);
		mgn /= 2;
		if (abs((int)(rc.left - X))<mgn || abs((int)(rc.right - X))<mgn || abs((int)(rc.top - Y))<mgn || abs((int)(rc.bottom - Y))<mgn) {
			gn = i; break;
		}
	}
	return gn;
}
//---------------------------------------------------------------------------
//グループの包含関係を取得
//  戻り値:
//    0: 同一
//   -1: gn0 ⊂ gn1
//    1: gn1 ⊂ gn0
//   -2: その他
//---------------------------------------------------------------------------
int __fastcall GrpInfs::compare_group(int gn0, int gn1)
{
	if (gn0<1 || gn0>MAX_GROUP) return -2;
	if (gn1<1 || gn1>MAX_GROUP) return -2;
	if (gn0==gn1) return 0;

	TRect rc0 = Items[gn0].gfp->FrmRect;
	TRect rc1 = Items[gn1].gfp->FrmRect;
	TRect rc = rc0; rc.Intersect(rc1);
	if (!rc.IsEmpty())
		return (rc==rc0)? -1 : (rc==rc1)? 1 :  -2;
	else
		return -2;
}

//---------------------------------------------------------------------------
//グループ伸縮
//---------------------------------------------------------------------------
void __fastcall GrpInfs::stretch(
	double ratio,	//比率
	bool sw)		//無所属断片も連動
{
	std::unique_ptr<TFragList> TmpList(new TFragList());

	if (sw) {
		//無所属断片を連動して伸縮
		grp_inf *gp = &Items[0];
		if (gp->Visible && !gp->Empty) {
			//無所属断片をリストアップ
			TmpList->Clear();
			for (int i=0; i<FrgList->Count; i++) {
				Fragment *fp = FrgList->Items[i];
				if (fp->in_group(0)!=-1) TmpList->Add(fp);
			}
			//個別に位置変換
			for (int i=0; i<TmpList->Count; i++) {
				Fragment *fp = TmpList->Items[i];
				fp->era_frag();
				fp->RC.Left = fp->tmpRC.Left * ratio;
				fp->RC.Top  = fp->tmpRC.Top  * ratio;
			}
		}
	}

	for (int gn=1; gn<=MAX_GROUP; gn++) {
		grp_inf *gp = &Items[gn];
		if (!gp->Visible || gp->Empty) continue;
		//グループの断片をリストアップ
		TmpList->Clear();
		for (int i=0; i<FrgList->Count; i++) {
			Fragment *fp = FrgList->Items[i];
			if (fp->in_group(gn)!=-1) TmpList->Add(fp);
		}
		if (TmpList->Count==0) continue;
		//グループの左上位置を求める
		int min_x = 999999;
		int min_y = 999999;
		for (int i=0; i<TmpList->Count; i++) {
			Fragment *fp = TmpList->Items[i];
			min_x = std::min<int>(fp->tmpRC.Left, min_x);
			min_y = std::min<int>(fp->tmpRC.Top,  min_y);
		}
		//グループの位置変換
		int dx = (int)(min_x * (ratio - 1.0));
		int dy = (int)(min_y * (ratio - 1.0));
		for (int i=0; i<TmpList->Count; i++) {
			Fragment *fp = TmpList->Items[i];
			fp->era_frag();
			fp->RC.Left = fp->tmpRC.Left + dx;
			fp->RC.Top  = fp->tmpRC.Top  + dy;
		}
	}
}
//---------------------------------------------------------------------------
