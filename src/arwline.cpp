//----------------------------------------------------------------------//
//	矢印線のクラス														//
//																		//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "EnvData.h"
#include "arwline.h"

//---------------------------------------------------------------------------
ArrowLine::ArrowLine()
{
	tType	  = TDIR_NONE;
	fromTag   = toTag = 0;
	lType	  = LTYPE_LINE;
	lWidth	  = 1;
	lColor	  = lCurCol = clNone;
	aType	  = ATYPE_SOLID;
	aSize	  = ASIZE_MEDIUM;
	TextStr   = EmptyStr;
	tOpposite = false;
	x0 = y0 = x1 = y1 = 0;
	vx0 = vy0 = vx1 = vy1 = 0;
	tmpFlag0  = false;
	tmpFlag1  = false;
}

//---------------------------------------------------------------------------
void ArrowLine::assign(ArrowLine* ap)
{
	tType	  = ap->tType;
	fromTag   = ap->fromTag;
	toTag	  = ap->toTag;
	lType	  = ap->lType;
	lWidth	  = ap->lWidth;
	lColor	  = ap->lColor;
	aType	  = ap->aType;
	aSize	  = ap->aSize;
	TextStr   = ap->TextStr;
	tOpposite = ap->tOpposite;
	x0 = y0 = x1 = y1 = 0;
	vx0 = vy0 = vx1 = vy1 = 0;
}

//---------------------------------------------------------------------------
// 点がライン近傍にあるか ?
//---------------------------------------------------------------------------
bool ArrowLine::pt_in_line(int X, int Y)
{
	int d = 5 + abs(lWidth);
	if (lType==LTYPE_D_LINE) d += ((DLINE_WIDTH + 1)/2);

	if (vx0<=vx1) {
		if (X<vx0-d || X>vx1+d) return false;
	}
	else {
		if (X>vx0+d || X<vx1-d) return false;
	}
	if (vy0<=vy1) {
		if (Y<vy0-d || Y>vy1+d) return false;
	}
	else {
		if (Y>vy0+d || Y<vy1-d) return false;
	}

	if (abs(vy0 - vy1) < d) {
		if(abs(vy0-Y)<d)  return true;
	}
	else if (abs(vx0 - vx1) < d) {
		if (abs(vx0 - X) < d) return true;
	}
	else {
		double a = 1.0*(vy1 - vy0)/(vx1 - vx0);
		double b = 1.0*(vx1 - vx0)/(vy1 - vy0);
		if		(abs(a*(X - vx0) - (Y - vy0)) < d) return true;
		else if (abs(b*(Y - vy0) - (X - vx0)) < d) return true;
	}
	return false;
}

//---------------------------------------------------------------------------
// 点がライン始点近傍にあるか ?
//---------------------------------------------------------------------------
bool ArrowLine::pt_in_frm(int X, int Y)
{
	int ds = 6 + abs(lWidth);
	if (lType==LTYPE_D_LINE) ds += ((DLINE_WIDTH + 1)/2);
	int dx = abs(X - vx0);
	int dy = abs(Y - vy0);
	return ((dx*dx + dy*dy) < ds*ds);
}

//---------------------------------------------------------------------------
// 点がライン終点近傍にあるか ?
//---------------------------------------------------------------------------
bool ArrowLine::pt_in_to(int X, int Y)
{
	int ds = 6 + abs(lWidth);
	if (lType==LTYPE_D_LINE) ds += ((DLINE_WIDTH + 1)/2);
	int dx = abs(X - vx1);
	int dy = abs(Y - vy1);
	return ((dx*dx + dy*dy) < ds*ds);
}

//---------------------------------------------------------------------------
// 消去
//---------------------------------------------------------------------------
void ArrowLine::erase(TForm *frm)
{
	if (!frm) return;

	if (Visible) {
		draw(frm->Canvas, 0, frm->Color, frm->Color, false, true);
		FVisible = false;
	}
}

//---------------------------------------------------------------------------
// 描画／印刷／画像出力
//	※フォントは予め倍率を掛け cv->Font に設定しておくこと
//---------------------------------------------------------------------------
void ArrowLine::draw(
	TCanvas *cv,
	double ratio,	//倍率 =0: 描画(画面相対)/ >0: 印刷／画像出力(絶対)
	TColor lcol,	//関係線の色
	TColor tcol,	//ラベルの文字色
	bool sel_sw,	//選択中
	bool era_sw)	//消去
{
	if (!cv) return;

	bool is_prn = (cv==Printer()->Canvas);

	int px0, py0, px1, py1, pwd;
	int lwd = abs(lWidth);

	//ラベル
	int tw, th, tmg;
	if (!TextStr.IsEmpty()) {
		tw = cv->TextWidth(TextStr);
		th = cv->TextHeight(" ");
		tmg = th/2;
	}
	else {
		tw = th = tmg = 0;
	}

	int lmg = 0;

	//絶対
	if (ratio>0) {
		px0 = round_i(x0 * ratio);
		py0 = round_i(y0 * ratio);
		px1 = round_i(x1 * ratio);
		py1 = round_i(y1 * ratio);

		if (is_prn && EV->FixPrnLWidth>0)
			pwd = (lwd>1)? (std::max(EV->FixPrnLWidth, (int)(lwd * ratio))) : EV->FixPrnLWidth;
		else
			pwd = std::max(1, (int)(lwd * ratio));
	}
	//画面相対
	else {
		px0 = vx0; py0 = vy0;
		px1 = vx1; py1 = vy1;
		pwd = lwd;
	}

	if (era_sw)
		lCurCol = lcol;
	else
		lCurCol = (!is_prn && lColor!=clNone)? lColor : lcol;
	cv->Pen->Color = sel_sw? EV->col_selRct : lCurCol;

	//ライン
	if (lType==LTYPE_LINE) {
		cv->Pen->Style	 = ((lWidth==-1)? psDot : psSolid);
		cv->Pen->Width	 = (cv->Pen->Style!=psDot)? pwd : 1;
		cv->Brush->Style = bsClear;
		cv->MoveTo(px0, py0);
		cv->LineTo(px1, py1);
	}
	//二重線
	else if (lType==LTYPE_D_LINE) {
		cv->Pen->Style	 = ((lWidth==-1)? psDot : psSolid);
		cv->Pen->Width	 = (cv->Pen->Style!=psDot)? pwd : 1;
		cv->Brush->Style = bsClear;
		int dw = DLINE_WIDTH + pwd/2;
		double ldx, ldy;
		if (px1==px0) {
			ldx = dw; ldy = 0;
		}
		else if (py1==py0) {
			ldy = dw; ldx = 0;
		}
		else {
			double a = atan(1.0*(px1-px0)/(py1-py0));
			ldy = dw * sin(a);
			ldx = dw * cos(a);
		}
		cv->MoveTo(px0 + ldx, py0 - ldy);
		cv->LineTo(px1 + ldx, py1 - ldy);
		cv->MoveTo(px0 - ldx, py0 + ldy);
		cv->LineTo(px1 - ldx, py1 + ldy);
		lmg += dw;
	}
	//矢
	else {
		int asz = aSize + lwd;
		if (ratio>0) asz = round_i(asz * ratio);
		double aw = M_PI/12 + (M_PI/12 * (lwd - 1)/4);
		if (aType==ATYPE_LINE) {
			if ((lwd>1) && (aType==ATYPE_LINE)) {
				cv->Pen->Width = ((ratio>0)? ((1 + lwd/2) * ratio) : (1 + lwd/2));
			}
			else {
				if (is_prn && (EV->FixPrnLWidth>0))
					cv->Pen->Width = EV->FixPrnLWidth;
				else
					cv->Pen->Width = std::max(1, (int)ratio);
			}
		}
		else {
			cv->Pen->Width = 1;
		}

		double a;
		if (px1==px0)
			a = ((py0>py1)? M_PI/2 : -M_PI/2);
		else
			a = atan(1.0*(py1-py0)/(px1-px0));

		if (px1-px0>0) a += M_PI;
		double a0  = a + aw;
		double a1  = a - aw;
		double ax0 = asz * cos(a0);
		double ay0 = asz * sin(a0);
		double ax1 = asz * cos(a1);
		double ay1 = asz * sin(a1);
		int sdx = pwd * cos(a);
		int sdy = pwd * sin(a);
		double axc = (ax0 + ax1)/2;
		double ayc = (ay0 + ay1)/2;
		if (lType==LTYPE_CD_ARROW) {
			if (px0!=px1) { px0 -= axc; px1 += axc; }
			if (py0!=py1) { py0 -= ayc; py1 += ayc; }
		}
		else if (lType==LTYPE_C_ARROW) {
			if (px0!=px1) px1 += axc;
			if (py0!=py1) py1 += ayc;
		}

		TPoint arp[3];
		if (lType==LTYPE_CD_ARROW || lType==LTYPE_C_ARROW) {
			axc *= 2; ayc *= 2;
			arp[0] = Point(px1 + ax0 - axc, py1 + ay0 - ayc);
			arp[1] = (aType==ATYPE_SOLID)? Point(px1 + sdx, py1 + sdy) : Point(px1, py1);
			arp[2] = Point(px1 + ax1 - axc, py1 + ay1 - ayc);
		}
		else {
			arp[0] = Point(px1 + ax0, py1 + ay0);
			arp[1] = Point(px1 - sdx, py1 - sdy);
			arp[2] = Point(px1 + ax1, py1 + ay1);
		}

		cv->Pen->Style   = psSolid;
		cv->Brush->Style = bsSolid;
		cv->Brush->Color = sel_sw? EV->col_selRct : lCurCol;
		if (aType==ATYPE_SOLID) cv->Polygon(arp, 2); else cv->Polyline(arp, 2);

		if (lType==LTYPE_D_ARROW || lType==LTYPE_CD_ARROW) {
			if (lType==LTYPE_CD_ARROW) {
	 			arp[0] = Point(px0 - ax0 + axc, py0 - ay0 + ayc);
				arp[1] = (aType==ATYPE_SOLID)? Point(px0 - sdx, py0 - sdy) : Point(px0, py0);
	 			arp[2] = Point(px0 - ax1 + axc, py0 - ay1 + ayc);
			}
			else {
				arp[0] = Point(px0 - ax0, py0 - ay0);
				arp[1] = Point(px0 + sdx, py0 + sdy);
				arp[2] = Point(px0 - ax1, py0 - ay1);
			}
			if (aType==ATYPE_SOLID) cv->Polygon(arp, 2); else cv->Polyline(arp, 2);
		}

		//ライン
		cv->Pen->Style	 = ((lWidth==-1)? psDot : psSolid);
		cv->Pen->Width	 = (cv->Pen->Style!=psDot)? pwd : 1;
		cv->Brush->Style = bsClear;
		cv->MoveTo(px0, py0);
		cv->LineTo(px1, py1);
	}

	//ラベル
	if (tw>0) {
		cv->Font->Color  = tcol;
		cv->Brush->Style = bsClear;
		int lx0, lx1, ly0, ly1, lxc, lyc, dtx = 0, dty = 0;
		//縦書き
		if (cv->Font->Name[1]=='@') {
			if (py0>py1) {
				lx0 = px1; lx1 = px0;
				ly0 = py1; ly1 = py0;
			}
			else {
				lx0 = px0; lx1 = px1;
				ly0 = py0; ly1 = py1;
			}
			lxc = lx0 + (lx1 - lx0)/2;
			lyc = ly0 + (ly1 - ly0)/2;
			if (abs(lx1 - lx0) < abs(ly1 - ly0)) {
				if (lx1>lx0) dtx -= (tmg + lmg); else dtx += (th + tmg + lmg);
				if (abs(lx1-lx0)<th) dty -= tw/2;
			}
			else {
				dtx += th/2;
				dty += lmg;
				if (lx1!=lx0) dty += (int)((fabs(0.5*(ly1 - ly0)/(lx1 - lx0)) + 0.5) * th);
			}
			if (tOpposite) {
				dtx = -dtx + th;
				dty = -dty - tw;
			}
			vert_textout(cv, lxc + dtx, lyc + dty, TextStr);

		}
		//横書き
		else {
			if (px0>px1) {
				lx0 = px1; lx1 = px0;
				ly0 = py1; ly1 = py0;
			}
			else {
				lx0 = px0; lx1 = px1;
				ly0 = py0; ly1 = py1;
			}
			lxc = lx0 + (lx1 - lx0)/2;
			lyc = ly0 + (ly1 - ly0)/2;
			if (abs(ly1 - ly0) < abs(lx1 - lx0)) {
				if (ly1>=ly0) dty -= (th + tmg + lmg); else dty += (tmg + lmg);
				if (abs(ly1 - ly0)<th) dtx -= tw/2;
			}
			else {
				dtx += lmg;
				if (ly1!=ly0) dtx += (int)((fabs(0.5*(lx1 - lx0)/(ly1 - ly0)) + 0.5) * th);
				dty -= th/2;
			}
			if (tOpposite) {
				dtx = -dtx - tw;
				dty = -dty - th;
			}
			cv->TextOut(lxc + dtx, lyc + dty, TextStr);
		}
	}

	FVisible = true;
}

//---------------------------------------------------------------------------
// 縮小表示 (ライン部分のみ)
//---------------------------------------------------------------------------
void ArrowLine::view_line(TCanvas *v_cv, double ratio, TColor col)
{
	if (!v_cv) return;

	if (Visible) {
		v_cv->Pen->Color   = col;
		v_cv->Pen->Width   = 1;
		v_cv->Pen->Style   = ((lWidth==-1)? psDot : psSolid);
		v_cv->Brush->Style = bsClear;
		v_cv->MoveTo(round_i(x0 * ratio), round_i(y0 * ratio));
		v_cv->LineTo(round_i(x1 * ratio), round_i(y1 * ratio));
	}
}

//---------------------------------------------------------------------------
//長方形間を結ぶライン位置を設定
//---------------------------------------------------------------------------
bool ArrowLine::set_pos(
	octrect xrc0, octrect xrc1,
	int ofsx, int ofsy,
	bool j0sw,		//xrc0 側が中点
	bool j1sw)		//xrc1 側が中点
{
	if (is_octrect_empty(&xrc0)) return false;
	if (is_octrect_empty(&xrc1)) return false;

	//適切かつ最短な線位置を求める
	int mgn = abs(lWidth) + 3;
	if (!j0sw) inflate_octrect(&xrc0, mgn, mgn);
	if (!j1sw) inflate_octrect(&xrc1, mgn, mgn);

	int min_l  = 0x7fffffff;	//無条件での最短
	int min_ok = 0x7fffffff;	//適切な最短
	int i0, i1, i0_n, i1_n;
	int i0_ok = -1;
	int i1_ok = -1;
	POINT *pi0, *pi1;

	for (i0=0; i0<8; i0++) for(i1=0; i1<8; i1++) {
		if (abs(i0-i1)==2 || abs(i0-i1)==6) continue;
		pi0 = &xrc0.p[i0];
		pi1 = &xrc1.p[i1];
		int dx = pi0->x - pi1->x;
		int dy = pi0->y - pi1->y;
		int l  = dx*dx + dy*dy;
		if (l<min_l) {
			min_l = l;
			i0_n  = i0;
			i1_n  = i1;
		}
		bool flag = true;
		switch (i0) {
		case 0: if (pi0->y<pi1->y || pi0->x<pi1->x) flag = false; break;
		case 2: if (pi0->y<pi1->y || pi0->x>pi1->x) flag = false; break;
		case 4: if (pi0->y>pi1->y || pi0->x>pi1->x) flag = false; break;
		case 6: if (pi0->y>pi1->y || pi0->x<pi1->x) flag = false; break;
		case 1: if (pi0->y<pi1->y) flag = false; break;
		case 3: if (pi0->x>pi1->x) flag = false; break;
		case 5: if (pi0->y>pi1->y) flag = false; break;
		case 7: if (pi0->x<pi1->x) flag = false; break;
		}
		if (!flag) continue;
		switch (i1) {
		case 0: if (pi1->y<pi0->y || pi1->x<pi0->x) flag = false; break;
		case 2: if (pi1->y<pi0->y || pi1->x>pi0->x) flag = false; break;
		case 4: if (pi1->y>pi0->y || pi1->x>pi0->x) flag = false; break;
		case 6: if (pi1->y>pi0->y || pi1->x<pi0->x) flag = false; break;
		case 1: if (pi1->y<pi0->y) flag = false; break;
		case 3: if (pi1->x>pi0->x) flag = false; break;
		case 5: if (pi1->y>pi0->y) flag = false; break;
		case 7: if (pi1->x<pi0->x) flag = false; break;
		}
		if (!flag) continue;
		if (l<min_ok) {
			min_ok = l;
			i0_ok  = i0;
			i1_ok  = i1;
		}
	}

	//両端位置を設定
	if (i0_ok!=-1) {
		i0 = i0_ok;
		i1 = i1_ok;
	}
	else {
		i0 = i0_n;
		i1 = i1_n;	//適切なものがない場合は最短位置
	}
	vx0 = xrc0.p[i0].x;
	vy0 = xrc0.p[i0].y;
	vx1 = xrc1.p[i1].x;
	vy1 = xrc1.p[i1].y;

	if (j0sw) {
		vx0 = xrc0.ct.x;
		vy0 = xrc0.lc.y;
	}
	if (j1sw) {
		vx1 = xrc1.ct.x;
		vy1 = xrc1.lc.y;
	}

	//端の重なりを避ける
	int mg = (lType==LTYPE_CD_ARROW)? (aSize/2+4) : 4;
	if (!j0sw) {
		switch (i0) {
		case 1: case 5: vx0 += ((vx0<vx1)? mg : -mg); break;
		case 3: case 7: vy0 += ((vy0<vy1)? mg : -mg); break;
		}
	}
	mg = (lType==LTYPE_CD_ARROW || lType==LTYPE_C_ARROW)? (aSize/2 + 4) : 4;
	if (!j1sw) {
		switch (i1) {
		case 1: case 5: vx1 += ((vx1<vx0)? mg : -mg); break;
		case 3: case 7: vy1 += ((vy1<vy0)? mg : -mg); break;
		}
	}

	//辺同士の関係線をなるべく垂直に
	if (EV->GlRightAngle && vx0!=vx1 && vy0!=vy1 && (!j0sw || !j1sw)) {
		if ((i0==1 || i0==5) && (i1==1 || i1==5)) {
			double a = atan(1.0*(vx1-vx0)/(vy1-vy0));
			if (!j0sw && !j1sw) {
				if (fabs(a)<M_PI/12)
					vx1 = vx0 = vx0 + (vx1 - vx0)/2;
			}
			else {
				if (fabs(a)<M_PI/24) {
					if (j0sw) vx1 = vx0;
					if (j1sw) vx0 = vx1;
				}
			}
		}
		else if ((i0==3 || i0==7) && (i1==3 || i1==7)) {
			double a = atan(1.0*(vy1-vy0)/(vx1-vx0));
			if (!j0sw && !j1sw) {
				if (fabs(a)<M_PI/12)
					vy1 = vy0 = vy0 + (vy1 - vy0)/2;
			}
			else {
				if (fabs(a)<M_PI/24) {
					if (j0sw) vy1 = vy0;
					if (j1sw) vy0 = vy1;
				}
			}
		}
	}

	//絶対位置を設定
	x0 = vx0 + ofsx;
	y0 = vy0 + ofsy;
	x1 = vx1 + ofsx;
	y1 = vy1 + ofsy;

	return true;
}
//---------------------------------------------------------------------------
//方向を逆転
//---------------------------------------------------------------------------
void ArrowLine::reverse()
{
	if (lType==LTYPE_D_ARROW) {
		lType = LTYPE_CD_ARROW;
	}
	else if (lType==LTYPE_CD_ARROW) {
		lType = LTYPE_D_ARROW;
	}
	else {
		switch (tType) {
		case TDIR_F2G: tType = TDIR_G2F; break;
		case TDIR_G2F: tType = TDIR_F2G; break;
		}
		int tmp = fromTag;
		fromTag = toTag;
		toTag	= tmp;
	}
}

//===========================================================================
//矢印線リスト (TList から継承)
//===========================================================================
__fastcall TArrowList::TArrowList(Classes::TComponent* AOwner): TList()
{
	frg_owner = (TForm*)AOwner;
}

//---------------------------------------------------------------------------
ArrowLine * __fastcall TArrowList::Get(int Index)
{
 	return (ArrowLine*)(TList::Get(Index));
}
//---------------------------------------------------------------------------
void __fastcall TArrowList::Put(int Index, ArrowLine *Item)
{
	TList::Put(Index, Item);
}

//---------------------------------------------------------------------------
//位置から線を取得
//---------------------------------------------------------------------------
ArrowLine * __fastcall TArrowList::lp_from_pos(int X, int Y)
{
	ArrowLine *ap = NULL;
	for (int i=0; i<Count; i++) {
		if (Items[i]->pt_in_line(X, Y)) {
			ap = Items[i]; break;
		}
	}
	return ap;
}
//---------------------------------------------------------------------------
//指定したIDの断片間の線を取得
//---------------------------------------------------------------------------
ArrowLine *	__fastcall TArrowList::lp_from_id(int tp, int id0, int id1)
{
	ArrowLine *ap = NULL;
	for (int i=0; i<Count; i++) {
		ArrowLine *ip = Items[i];
		if (ip->tType==tp) {
			if ((ip->fromTag==id0 && ip->toTag==id1) || (ip->fromTag==id1 && ip->toTag==id0)) {
				ap = ip; break;
			}
		}
	}
	return ap;
}

//---------------------------------------------------------------------------
//全ての線を消去
//---------------------------------------------------------------------------
void __fastcall TArrowList::erase_line(int tp)
{
	if (frg_owner) {
		for (int i=0; i<Count; i++) {
			if (tp==TDIR_NONE)
				Items[i]->erase(frg_owner);
			else if (tp==Items[i]->tType)
				Items[i]->erase(frg_owner);
		}
	}
}

//---------------------------------------------------------------------------
//全て破棄
//---------------------------------------------------------------------------
void __fastcall TArrowList::del_all()
{
	for (int i=0; i<Count; i++) delete Items[i];
	Clear();
}

//---------------------------------------------------------------------------
//他のリストの内容をコピー
//---------------------------------------------------------------------------
void __fastcall TArrowList::new_copy(TArrowList *lst)
{
	del_all();
	for (int i=0; i<lst->Count; i++) {
		ArrowLine *ap = new ArrowLine();
		ap->assign(lst->Items[i]);
		Add(ap);
	}

}

//---------------------------------------------------------------------------
//tmpFlag をクリア
//---------------------------------------------------------------------------
void __fastcall TArrowList::clr_tmp_flag()
{
	for (int i=0; i<Count; i++) {
		ArrowLine *ip = Items[i];
		ip->tmpFlag0 = ip->tmpFlag1 = false;
	}
}

//---------------------------------------------------------------------------
//断片IDを変更 (tmpFlagにより誤変更防止)
//---------------------------------------------------------------------------
void __fastcall TArrowList::change_frg_id(int id0, int id1)
{
	for (int i=0; i<Count; i++) {
		ArrowLine *ip = Items[i];
		if (ip->tType==TDIR_F2F) {
			if (!ip->tmpFlag0 && ip->fromTag==id0) {
				ip->fromTag  = id1;
				ip->tmpFlag0 = true;
			}
			else if (!ip->tmpFlag1 && ip->toTag==id0) {
				ip->toTag	 = id1;
				ip->tmpFlag1 = true;
			}
		}
		else if (ip->tType==TDIR_F2G) {
			if (!ip->tmpFlag0 && ip->fromTag==id0) {
				ip->fromTag  = id1;
				ip->tmpFlag0 = true;
			}
		}
		else if (ip->tType==TDIR_G2F) {
			if (!ip->tmpFlag1 && ip->toTag==id0) {
				ip->toTag	 = id1;
				ip->tmpFlag1 = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
