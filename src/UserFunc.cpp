//----------------------------------------------------------------------//
//	汎用共通関数														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <htmlhelp.h>
#include <System.IOUtils.hpp>
#include <System.StrUtils.hpp>
#include <RegularExpressions.hpp>
#include "UserFunc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// 長方形から8点長方形への変換
//---------------------------------------------------------------------------
void set_octrect(TRect *rc, octrect *xrc)
{
	int cx = rc->Left + rc->Width()/2;
	int cy = rc->Top  + rc->Height()/2;
	xrc->p[0].x = rc->Left;
	xrc->p[0].y = rc->Top;
	xrc->p[1].x = cx;
	xrc->p[1].y = rc->Top;
	xrc->p[2].x = rc->Right;
	xrc->p[2].y = rc->Top;
	xrc->p[3].x = rc->Right;
	xrc->p[3].y = cy;
	xrc->p[4].x = rc->Right;
	xrc->p[4].y = rc->Bottom;
	xrc->p[5].x = cx;
	xrc->p[5].y = rc->Bottom;
	xrc->p[6].x = rc->Left;
	xrc->p[6].y = rc->Bottom;
	xrc->p[7].x = rc->Left;
	xrc->p[7].y = cy;
}
//---------------------------------------------------------------------------
// 8点長方形が空か?
//---------------------------------------------------------------------------
bool is_octrect_empty(octrect *xrc)
{
	return (xrc->lt.x==xrc->rb.x || xrc->lt.y==xrc->rb.y);
}

//---------------------------------------------------------------------------
// 8点長方形の幅・高さを増減
//---------------------------------------------------------------------------
void inflate_octrect(octrect *xrc, int mgx, int mgy)
{
	xrc->lt.x -= mgx;
	xrc->lt.y -= mgy;
	xrc->ct.y -= mgy;
	xrc->rt.x += mgx;
	xrc->rt.y -= mgy;
	xrc->rc.x += mgx;
	xrc->rb.x += mgx;
	xrc->rb.y += mgy;
	xrc->cb.y += mgy;
	xrc->lb.x -= mgx;
	xrc->lb.y += mgy;
	xrc->lc.x -= mgx;
}

//---------------------------------------------------------------------------
//点(x,y) が長方形境界の±w の領域にあるか？
//---------------------------------------------------------------------------
bool pt_on_border(int x, int y, TRect rc, int w)
{
	TPoint p = Point(x, y);
	rc.Inflate(w, w);
	if (rc.PtInRect(p)) return false;
	rc.Inflate(-2*w, -2*w);
	return rc.PtInRect(p);
}

//---------------------------------------------------------------------------
//点(x,y) が長方形境界角の ±w の領域にあるか？
//---------------------------------------------------------------------------
bool pt_on_corner(int x, int y, TRect rc, int w)
{
	if (!pt_on_border(x, y, rc, w)) return false;
	int dx, dy;
	dx = (rc.Left - x);
	dy = (rc.Top - y);
	if ((dx*dx + dy*dy)<(w*w)) return true;
	dx = (rc.Right - x);
	dy = (rc.Top - y);
	if ((dx*dx + dy*dy)<(w*w)) return true;
	dx = (rc.Left - x);
	dy = (rc.Bottom - y);
	if ((dx*dx + dy*dy)<(w*w)) return true;
	dx = (rc.Right - x);
	dy = (rc.Bottom - y);
	if ((dx*dx + dy*dy)<(w*w)) return true;
	return false;
}

//---------------------------------------------------------------------------
//長方形を指定倍率に変換
//---------------------------------------------------------------------------
void scale_rect(TRect *rc, double ratio)
{
	rc->left   = round_i(rc->left	* ratio);
	rc->top    = round_i(rc->top	* ratio);
	rc->right  = round_i(rc->right  * ratio);
	rc->bottom = round_i(rc->bottom * ratio);
}

//---------------------------------------------------------------------------
// 各座標の大小関係を考慮して長方形を設定
//---------------------------------------------------------------------------
void set_rect_ex(TRect *rc, int x0, int y0, int x1, int y1)
{
	if (x1<x0) {
		rc->Left  = x1;
		rc->Right = x0;
	}
	else {
		rc->Left  = x0;
		rc->Right = x1;
	}
	if (y1<y0) {
		rc->Top	   = y1;
		rc->Bottom = y0;
	}
	else {
		rc->Top	   = y0;
		rc->Bottom = y1;
	}
}

//---------------------------------------------------------------------------
//フォントの描画高を取得
//---------------------------------------------------------------------------
int get_FontHeight(TCanvas *cv, int mgn)
{
	int fh = cv->TextHeight("Q");
	if (mgn>0) {
		//内部レディングと相殺
		int ld = fh - abs(cv->Font->Height);
		if (ld>0) mgn -= ld;
		if (mgn<0) mgn = 0;
		fh += mgn;
	}
	return fh;
}
//---------------------------------------------------------------------------
int get_FontHeight(TFont *font, int mgn)
{
	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	TCanvas *cv = bmp->Canvas;
	cv->Font->Assign(font);
	int fh = cv->TextHeight("Q");
	if (mgn>0) {
		//内部レディングと相殺
		int ld = fh - abs(cv->Font->Height);
		if (ld>0) mgn -= ld;
		if (mgn<0) mgn = 0;
		fh += mgn;
	}
	return fh;
}
//---------------------------------------------------------------------------
int get_FontHeightP(TFont *font, int per)
{
	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	TCanvas *cv = bmp->Canvas;
	cv->Font->Assign(font);
	int fh = cv->TextHeight("Q");
	if (per>0) {
		int mgn = fh * per / 100;
		//内部レディングと相殺
		int ld = fh - abs(cv->Font->Height);
		if (ld>0) mgn -= ld;
		if (mgn<0) mgn = 0;
		fh += mgn;
	}
	return fh;
}
//---------------------------------------------------------------------------
//全角フォントの描画幅を取得
//---------------------------------------------------------------------------
int get_FontWidthF(TCanvas *cv)
{
	return cv->TextWidth("─");
}

//---------------------------------------------------------------------------
//四捨五入
//---------------------------------------------------------------------------
int round_i(double v)
{
	return (int)(v + 0.5);
}

//---------------------------------------------------------------------------
// 指定セパレータの前の文字列を取得
//---------------------------------------------------------------------------
UnicodeString get_tkn(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? s : s.SubString(1, p - 1);
}

//---------------------------------------------------------------------------
// 指定セパレータの後の文字列を取得
//---------------------------------------------------------------------------
UnicodeString get_tkn_r(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? EmptyStr : s.SubString(p + sp.Length(), s.Length() - (p - sp.Length() + 1));
}

//---------------------------------------------------------------------------
// 指定セパレータ前の文字列を分離して取得
// ※セパレータが見つからない場合は、文字列をそのまま返し、元文字列は空に
//---------------------------------------------------------------------------
UnicodeString split_tkn(UnicodeString &s, UnicodeString sp)
{
	UnicodeString r;
	int p = s.Pos(sp);
	if (p==0) {
		r = s;
		s = EmptyStr;
	}
	else {
		r = s.SubString(1, p - 1);
		s = s.SubString(p + sp.Length(), s.Length() - (p - sp.Length() + 1));
	}
	return r;
}

//---------------------------------------------------------------------------
// 文字列から指定語を削除
// 戻り値: ture=削除した/ false=語が含まれていない
//---------------------------------------------------------------------------
bool remove_text(UnicodeString &s, UnicodeString w)
{
	if (ContainsText(s, w)) {
		s = ReplaceText(s, w, EmptyStr);
		return true;
	}
	else
		return false;
}

//---------------------------------------------------------------------------
//全角スペースも考慮したトリミング
//---------------------------------------------------------------------------
UnicodeString trim_ex(UnicodeString src)
{
	for (;;) {
		if (src.IsEmpty()) break;
		if (StartsText("　", src)) {
			src.Delete(1, 1); continue;
		}
		if (src[1]==' ' || src[1]=='\t') {
			src.Delete(1, 1); continue;
		}
		break;
	}
	for (;;) {
		if (src.IsEmpty()) break;
		int n = src.Length();
		if (n>1) {
			if (src.SubString(n - 1, 1)=="　") {
				src.Delete(n - 1, 1); continue;
			}
		}
		if (src[n]==' ' || src[n]=='\t') {
			src.Delete(n, 1); continue;
		}
		break;
	}
	return src;
}

//---------------------------------------------------------------------------
//大小文字を区別せずに文字列を検索
//---------------------------------------------------------------------------
int pos_i(
	UnicodeString wd,	//検索語
	UnicodeString s)	//対象文字列
{
	if (wd.IsEmpty()) return 0;
	s  = s.UpperCase();
	wd = wd.UpperCase();
	return s.Pos(wd);
}

//---------------------------------------------------------------------------
// 文字列が最後に現れる位置を取得
//---------------------------------------------------------------------------
int pos_r(
	UnicodeString wd,	//検索語
	UnicodeString s)	//対象文字列
{
	if (wd.IsEmpty()) return 0;
	UnicodeString ss = s;
	int p = 0;
	for (;;) {
		int p1 = ss.Pos(wd);
		if (p1==0) break;
		ss.Delete(1, p1);
		p += p1;
	}
	return p;
}

//---------------------------------------------------------------------------
//引用符を外す
//---------------------------------------------------------------------------
UnicodeString exclude_quot(UnicodeString s)
{
	if (s.Length()>=2 && ((StartsStr("\"", s) && EndsStr("\"", s)) || (StartsStr("'", s) && EndsStr("'", s))))
		s = s.SubString(2, s.Length() - 2);
	return s;
}
//---------------------------------------------------------------------------
//空白を含んでいる場合、引用符で囲む
//---------------------------------------------------------------------------
UnicodeString add_quot_if_spc(UnicodeString s)
{
	if (ContainsStr(s, " ") || ContainsStr(s, "　")) s = "\"" + s + "\"";
	return s;
}

//---------------------------------------------------------------------------
//正規表現パターンのチェック
//---------------------------------------------------------------------------
bool chk_RegExPtn(UnicodeString ptn)
{
	if (ptn.IsEmpty()) return false;

	try {
		TRegEx x(ptn, TRegExOptions() << roCompiled);
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//検索語リスト作成
//---------------------------------------------------------------------------
void get_find_wd_list(UnicodeString wd, TStringList *lst)
{
	lst->Clear();

	for (;;) {
		if (wd.IsEmpty()) break;
		int ph = wd.Pos(" ");
		int pw = wd.Pos("　");
		int p = (ph && !pw)? ph : (!ph && pw)? pw : (ph && pw)? std::min(ph, pw) : 0;
		if (p==1)
			wd.Delete(1, 1);
		else if (p>1) {
			lst->Add(wd.SubString(1, p - 1));
			wd.Delete(1, p);
		}
		else {
			lst->Add(wd); break;
		}
	}

	//空白やタブを変換
	for (int i=0; i<lst->Count; i++) {
		UnicodeString tmpstr = lst->Strings[i];
		tmpstr = ReplaceStr(tmpstr, "\\s", " ");
		tmpstr = ReplaceStr(tmpstr, "\\t", "\t");
		lst->Strings[i] = tmpstr;
	}
}

//---------------------------------------------------------------------------
//複数の文字列を OR/AND 検索
//---------------------------------------------------------------------------
bool find_mlt(
	UnicodeString wd,	//検索語 (複数の場合は半角または全角空白で区切る）
	UnicodeString s,	//対象文字列
	bool and_sw,		//AND検索
	bool not_sw,		//NOT検索
	bool case_sw)		//大小文字を区別 (default = false)
{
	if (wd.IsEmpty()) return 1;

	//検索語リスト作成
	std::unique_ptr<TStringList> wlst(new TStringList());
	get_find_wd_list(wd, wlst.get());

	//検索
	bool ret = and_sw;
	for (int i=0; i<wlst->Count; i++) {
		int p = case_sw? s.Pos(wlst->Strings[i]) : pos_i(wlst->Strings[i], s);
		if (!not_sw) {
			//含む
			if (p>0) {
				//いずれかを
				if (!and_sw) { ret = true; break; }
			}
			else {
				//全てを
				if (and_sw) { ret = false; break; }
			}
		}
		else {
			//含まない
			if (p==0) {
				//いずれかを
				if (!and_sw) { ret = true; break; }
			}
			else {
				//全てを
				if (and_sw) { ret = false; break; }
			}
		}
	}

	return ret;
}

//---------------------------------------------------------------------------
//色指定して文字列描画
//---------------------------------------------------------------------------
void color_textout(TCanvas *cv, int xp, int yp, UnicodeString s, TColor *fg, TColor *bg)
{
	int len = s.Length();
	int yh  = yp + cv->TextHeight(s);
	for (int i=1; i<=len; i++,fg++,bg++) {
		cv->Font->Color  = *fg;
		cv->Brush->Color = *bg;
		UnicodeString tmp = s.SubString(i, 1);
		int w = cv->TextWidth(tmp);
		cv->TextRect(Rect(xp, yp, xp + w, yh), xp, yp, tmp);
		xp += w;
	}
}
//---------------------------------------------------------------------------
void color_textout_v(TCanvas *cv, int xp, int yp, UnicodeString s, TColor *fg, TColor *bg)
{
	int len = s.Length();
	for (int i=1; i<=len; i++,fg++,bg++) {
		cv->Font->Color  = *fg;
		cv->Brush->Color = *bg;
		UnicodeString tmp = s.SubString(i, 1);
		vert_textout(cv, xp, yp, tmp);
		yp += cv->TextWidth(tmp);
	}
}

//---------------------------------------------------------------------------
//縦書き文字列描画
//---------------------------------------------------------------------------
void vert_textout(TCanvas *cv, int xp, int yp, UnicodeString s)
{
	LOGFONT lf;
	::GetObject(cv->Font->Handle, sizeof(LOGFONT), &lf);
	lf.lfEscapement = lf.lfOrientation = -900;
	HFONT hTmpFont = ::CreateFontIndirect(&lf);
	HFONT hOrgFont = (HFONT)::SelectObject(cv->Handle, hTmpFont);
	cv->TextOut(xp, yp, s);
	::SelectObject(cv->Handle, hOrgFont);
	::DeleteObject(hTmpFont);
}

//---------------------------------------------------------------------------
//要素から指定した属性の値を取得
//---------------------------------------------------------------------------
UnicodeString get_tag_atr(
	UnicodeString s,	//要素文字列
	UnicodeString t,	//タグ名
	UnicodeString a)	//属性名
{
	UnicodeString retstr;
	if (s.IsEmpty()) return retstr;

	int flag = 0;
	UnicodeString tag;
	UnicodeString atr;
	UnicodeString vstr;
	bool qflag;

	for (int i=1; i<=s.Length(); i++) {
		UnicodeString c = s.SubString(i, 1);
		switch (flag) {
		case 0:
			if (c=="<") {
				tag  = EmptyStr;
				flag = 1;
			}
			break;
		case 1:
			if (c==">")
				flag = 0;
			else if (c==" ") {	//タグ確定
				tag = tag.Trim();
				atr  = EmptyStr;
				flag = 2;
			}
			else
				tag += c;
			break;
		case 2:
			if (c=="=") {	//属性名確定
				atr = atr.Trim();
				flag = 3;
			}
			else if (c==">")
				flag = 0;
			else
				atr += c;
			break;
		case 3:
			if (c==">")
				flag = 0;
			else if (c!=" ") {	//属性値開始
				qflag = (c=="\"");
				vstr  = qflag? EmptyStr : c;
				flag  = 4;
			}
			break;
		case 4:
			if ((qflag && c=="\"") || (!qflag && (c==" " || c==">"))) {
				if (tag.CompareIC(t)==0 && atr.CompareIC(a)==0) {
					retstr = vstr;
					flag   = -1;	//属性値確定!
				}
				else {
					atr  = EmptyStr;
					flag = (c==">")? 0 : 2;
				}
			}
			else
				vstr += c;
			break;
		}

		if (flag==-1) break;
	}

	return retstr;
}

//---------------------------------------------------------------------------
//HTML文書のヘッダ情報(charset, TITLE, META)を取得
//---------------------------------------------------------------------------
bool get_html_inf(
	UnicodeString fnam,		//HTMLファイル名
	TStringList *lst)		//[o] 結果リスト(名前=値 形式で格納)
{
	lst->Clear();
	if (!FileExists(fnam)) return false;

	//ヘッダ部分を取得　
	std::unique_ptr<TStringList> fbuf(new TStringList());
	fbuf->LoadFromFile(fnam);

	UnicodeString headstr;
	int p = pos_i("</HEAD>", fbuf->Text);
	if (p>0) headstr = fbuf->Text.SubString(1, p + 6);

	if (!headstr.IsEmpty()) {
		//charset をチェック
		int p = pos_i("charset", headstr);
		UnicodeString charset;
		if (p>0) {
			UnicodeString tmpstr = headstr.SubString(p, headstr.Length() - p + 1);
			tmpstr = get_tkn(get_tkn_r(tmpstr, "="), ">");
			if (!tmpstr.IsEmpty()) {
				p = tmpstr.Pos("\"");
				if (p>0) charset = tmpstr.SubString(1, p - 1);
				lst->Add("charset=" + charset);
			}
		}

		//エンコーディングを指定して再読み込み
		if (!charset.IsEmpty()) {
			int code_page = 0;
			if		(charset.CompareIC("EUC-JP")==0)	  code_page = 20932;
			else if (charset.CompareIC("ISO-2022-JP")==0) code_page = 50220;
			else if (charset.CompareIC("UTF-8")==0)		  code_page = CP_UTF8;
			if (code_page!=0) {
				std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
				std::unique_ptr<TStringList> fbuf(new TStringList());
				fbuf->LoadFromFile(fnam, enc.get());
				headstr = EmptyStr;
				int p = pos_i("</HEAD>", fbuf->Text);
				if (p>0) headstr = fbuf->Text.SubString(1, p + 6);
			}
		}

		//コメント部分を削除
		int p0, p1;
		p0 = headstr.Pos("<!--");
		while (p0>0) {
			p1 = headstr.Pos("-->");
			if (p1>p0) {
				headstr.Delete(p0, p1 - p0 + 3);
				p0 = headstr.Pos("<!--");
			}
			else {
				headstr = headstr.SubString(1, p1 - 1);
				break;
			}
		}

		//タイトルを取得
		p0 = pos_i("<TITLE>",  headstr);
		p1 = pos_i("</TITLE>", headstr);
		if (p0>0 && p1>p0) {
			lst->Add("title=" + headstr.SubString(p0 + 7, p1 - p0 - 7));
		}

		//メタ情報を取得
		UnicodeString tmpstr = headstr;
		while (!tmpstr.IsEmpty()) {
			int p = tmpstr.Pos("<");  if (p==0) break;
			tmpstr.Delete(1, p - 1);
			UnicodeString tstr = tmpstr;
			p = tstr.Pos(">");  if (p==0) break;
			tstr = tstr.SubString(1, p);
			UnicodeString nstr = get_tag_atr(tstr, "META", "NAME");
			if (!nstr.IsEmpty()) {
				UnicodeString cstr = get_tag_atr(tstr, "META", "CONTENT");
				if (!cstr.IsEmpty()) lst->Add(nstr.LowerCase() + "=" + cstr);
			}
			tmpstr.Delete(1, tstr.Length());
		}
		return true;
	}
	else
		return false;
}

//---------------------------------------------------------------------------
//ヘルプファイル(.HLP)のタイトルを取得 (対応する .CNT から取得)
//---------------------------------------------------------------------------
UnicodeString get_hlp_title(
	UnicodeString fnam)		//ヘルプファイル名( .HLP)
{
	UnicodeString tstr;
	fnam = ChangeFileExt(fnam, ".CNT");
	if (FileExists(fnam)) {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->LoadFromFile(fnam);
		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString tmpstr = fbuf->Strings[i];
			if (tmpstr.IsEmpty()) continue;
			if (tmpstr.SubString(1, 7).CompareIC(":Title ")==0) {
				tstr = tmpstr.SubString(8, tmpstr.Length() - 7);
				break;
			}
		}
	}
	return tstr;
}

//---------------------------------------------------------------------------
//ファイルはテキストか?
//---------------------------------------------------------------------------
bool is_text_file(UnicodeString fnam)
{
	if (!FileExists(fnam)) return false;

	unsigned char fbuf[512];
	::ZeroMemory(fbuf, sizeof(fbuf));

	bool ret = true;
	int hFile = FileOpen(fnam, fmOpenRead);
	if (hFile!=-1) {
		FileSeek(hFile, 0, 0);
		int n = FileRead(hFile, fbuf, sizeof(fbuf));
		FileClose(hFile);
		//チェック
		for (int i=0; (i<n) && ret; i++) {
			if ((fbuf[i]<=0x08) || (fbuf[i]>=0x0b && fbuf[i]<=0x0c) ||
				(fbuf[i]>=0x0e && fbuf[i]<=0x1f) || (fbuf[i]>=0xfd))
			{
				ret = false; break;
			}
		}
	}
	else
		ret = false;

	return ret;
}

//---------------------------------------------------------------------------
//テキストファイルから有効そうな最初の行を取得
// 空行はスキップ
// 行に含まれる文字が4種類以下で最大文字数が20を超える場合もスキップ
//---------------------------------------------------------------------------
UnicodeString get_top_textline(UnicodeString fnam)
{
	UnicodeString retstr;
	if (!FileExists(fnam)) return retstr;

	int hFile = FileOpen(fnam, fmOpenRead);
	if (hFile!=-1) {
		char sbuf[2048];
		FileSeek(hFile, 0, 0);
		for (;;) {
			int n = FileRead(hFile, sbuf, 2000);  if (n==0) break;
			char *c;
			c = strchr(sbuf,'\r'); if (c) *c='\0';
			c = strchr(sbuf,'\n'); if (c) *c='\0';
			retstr = sbuf; retstr = retstr.Trim();
			if (retstr.IsEmpty()) continue;

			int  sn[4];
			char sc[4];
			for (int i=0; i<4; i++) sn[i] = sc[i] = 0;
			int chrn = 0;
			int maxn = 0;

			for (int i=1; i<=retstr.Length(); i++) {
				char c = retstr[i];
				bool flag = false;
				for (int j=0; j<4; j++) {
					if (sc[j]!=c) continue;
					sn[j]++;
					if (maxn<sn[j]) maxn++;
					flag = true;
					break;
				}
				if (flag) continue;
				chrn++; if (chrn>4) break;
				for (int j=0; j<4; j++) {
					if (sc[j]=='\0') {
						sc[j] = c; sn[j]++; break;
					}
				}
			}
			//文字が4種類以下で、最大文字数が20を超える場合は無用行と判定
			if (chrn<=4 && maxn>20) continue;
			if (!retstr.IsEmpty()) break;
		}
		FileClose(hFile);
	}
	return retstr;
}

//---------------------------------------------------------------------------
//CSV項目内容取得
//---------------------------------------------------------------------------
TStringDynArray get_csv_array(UnicodeString src, int size)
{
	TStringDynArray ret_array;
	ret_array.Length = size;
	std::unique_ptr<TStringList> rec(new TStringList());
 	rec->Delimiter		 = ',';
 	rec->QuoteChar		 = '\"';
    rec->StrictDelimiter = true;
	rec->DelimitedText	 = src;
	for (int i=0; i<rec->Count && i<size; i++) ret_array[i] = rec->Strings[i];
	ret_array.Length = rec->Count;
	return ret_array;
}

//---------------------------------------------------------------------------
//パラメータ配列の取得
//---------------------------------------------------------------------------
TStringDynArray get_prm_array(UnicodeString src)
{
	UnicodeString itm_buf, ary_buf;
	while (!src.IsEmpty()) {
		src = Trim(src);
		//先頭が "
		if (StartsStr("\"", src)) {
			src.Delete(1, 1);
			itm_buf = split_tkn(src, "\"");
		}
		//先頭が " ではない
		else {
			itm_buf = split_tkn(src, " ");
		}
		if (!itm_buf.IsEmpty()) {
			if (!ary_buf.IsEmpty()) ary_buf += "\t";
			ary_buf += itm_buf;
		}
	}
	return SplitString(ary_buf, "\t");
}

//---------------------------------------------------------------------------
//文字列からフォント情報を取得
//---------------------------------------------------------------------------
void get_font_from_str(TFont *f, UnicodeString s)
{
	if (!f) return;

	TStringDynArray ibuf = get_csv_array(s, 4);
	ibuf.set_length(4);
	if (!ibuf[0].IsEmpty()) f->Name = ibuf[0];
	int n = ibuf[1].ToIntDef(f->Size);
	if (n!=0) f->Size = n;
	TFontStyles fstyle = TFontStyles();
	if (ibuf[2]=="1") fstyle << fsBold;
	if (ibuf[3]=="1") fstyle << fsItalic;
	f->Style = fstyle;
}

//---------------------------------------------------------------------------
//２つのフォントが等しいか?
//---------------------------------------------------------------------------
bool equal_font(TFont *f0, TFont *f1)
{
	if (f0->Name != f1->Name) return false;
	if (f0->Size != f1->Size) return false;
	if (f0->Style != f1->Style) return false;
	return true;
}

//---------------------------------------------------------------------------
//拡張子のチェック
//---------------------------------------------------------------------------
bool test_FileExt(UnicodeString fext, UnicodeString list)
{
	if (list.IsEmpty() || fext.IsEmpty() || fext==".") return false;
	if (list=="*" || list==".*") return true;
	if (!StartsStr(".", fext)) fext.Insert(".", 1);
	if (!EndsStr(".", fext))   fext.UCAT_TSTR(".");
	if (!StartsStr(".", list)) list.Insert(".", 1);
	if (!EndsStr(".", list))   list.UCAT_TSTR(".");
	return ContainsText(list, fext);
}

//---------------------------------------------------------------------------
//ディレクトリ下のマスクに該当する全ファイルを取得
//---------------------------------------------------------------------------
void get_files(
	UnicodeString pnam,		//ディレクトリ名
	UnicodeString mask,		//マスク
	TStrings *lst,			//[o] 結果格納リスト
	bool subsw)				//サブディレクトリも検索 (default = false);
{
	try {
		TSearchOption opt = subsw? TSearchOption::soAllDirectories : TSearchOption::soTopDirectoryOnly;
		TStringDynArray f_list = TDirectory::GetFiles(pnam, mask, opt);
		for (int i=0; i<f_list.Length; ++i) lst->Add(f_list[i]);
	} catch (...) {
		return;
	}
}

//---------------------------------------------------------------------------
//断片ファイルか？
//---------------------------------------------------------------------------
bool test_ideafrag2(UnicodeString fnam)
{
	int p = pos_r(".IFD#", fnam.UpperCase());
	if (p>0) fnam = fnam.SubString(1, p + 3);
	UnicodeString xnam = ExtractFileExt(fnam);
	if (!FileExists(fnam)) return false;
	return SameText(xnam, ".IFD");
}

//---------------------------------------------------------------------------
//相対パスから絶対パスへ
//---------------------------------------------------------------------------
UnicodeString rel_to_absdir(
	UnicodeString fnam,		//対象ファイル名
	UnicodeString rnam)		//基準ファイル名
{
	if (fnam.IsEmpty()) return EmptyStr;
	if (!ExtractFileDrive(fnam).IsEmpty()) return fnam;
	if (StartsText("http://", fnam) || StartsText("https://", fnam) || StartsText("mailto:", fnam)) return fnam;
	if (StartsText("onenote:///", fnam)) return fnam;
	if (StartsText("#", fnam)) return fnam;

	UnicodeString dnam = ExtractFileDir(rnam);
	if (dnam.IsEmpty()) return fnam;

	for (;;) {
		if (fnam.SubString(1, 3)!="..\\") break;
		int p = pos_r("\\", dnam); if (p==0) break;
		dnam = dnam.SubString(1, p - 1);
		fnam.Delete(1, 3);
	}
	return dnam+"\\"+fnam;
}

//---------------------------------------------------------------------------
//絶対パスから相対パスへ
//---------------------------------------------------------------------------
UnicodeString abs_to_reldir(
	UnicodeString fnam,		//対象ファイル名
	UnicodeString rnam)		//基準ファイル名
{
	if (ExtractFileDrive(fnam).IsEmpty()) return fnam;
	if (StartsText("http://", fnam) || StartsText("https://", fnam) || StartsText("mailto:", fnam)) return fnam;
	UnicodeString dnam = ExtractFilePath(rnam);
	if (dnam.IsEmpty()) return fnam;

	if (ExtractFileDrive(fnam) != ExtractFileDrive(dnam)) return fnam;

	for (;;) {
		int pf = fnam.Pos("\\");
		int pd = dnam.Pos("\\");
		if (pf>0 && pd>0) {
			if (SameText(fnam.SubString(1, pf), dnam.SubString(1, pd))) {
				fnam.Delete(1, pf);
				dnam.Delete(1, pd);
			}
			else {
				dnam.Delete(1, pd);
				fnam.Insert("..\\", 1);
				for (;;) {
					pd = dnam.Pos("\\");  if (pd==0) break;
					dnam.Delete(1, pd);
					fnam.Insert("..\\", 1);
				}
				break;
			}
		}
		else if (pf==0 && pd>0) {
			dnam.Delete(1, pd);
			fnam.Insert("..\\", 1);
			for (;;) {
				pd = dnam.Pos("\\");  if (pd==0) break;
				dnam.Delete(1, pd);
				fnam.Insert("..\\", 1);
			}
			break;
		}
		else break;
	}
	return fnam;
}

//---------------------------------------------------------------------------
//yyyy/mm/dd hh:nn:ss 形式の文字列を TDateTime に変換
//---------------------------------------------------------------------------
TDateTime str_to_date_time(UnicodeString str)
{
	TDateTime t = Now();
	do {
		if (str.Length()<19) break;
		unsigned short y = str.SubString( 1, 4).ToIntDef(0); if (y==0) break;
		unsigned short m = str.SubString( 6, 2).ToIntDef(0); if (m==0) break;
		unsigned short d = str.SubString( 9, 2).ToIntDef(0); if (d==0) break;
		unsigned short h = str.SubString(12, 2).ToIntDef(0); if (h==0) break;
		unsigned short n = str.SubString(15, 2).ToIntDef(0); if (n==0) break;
		unsigned short s = str.SubString(18, 2).ToIntDef(0); if (s==0) break;
		try {
			t = EncodeDate(y, m, d) + EncodeTime(h, n, s, 0);
		}
		catch(EConvertError &e) {
			t = Now();
		}
	} while(0);
	return t;
}

//---------------------------------------------------------------------------
// TUpDown に関連づけられている編集欄の値をチェック
//---------------------------------------------------------------------------
bool check_TUpDown(TUpDown *udp)
{
	bool ret = true;
	if (udp->Associate) {
		TCustomEdit *ep = dynamic_cast<TCustomEdit *>(udp->Associate);
		if (ep) {
			int n = ep->Text.ToIntDef(udp->Min - 1);
			if (n<udp->Min || n>udp->Max) ret = false;
		}
	}
	return ret;
}

//---------------------------------------------------------------------------
//文字列から ComboBox に項目を追加
//---------------------------------------------------------------------------
void str_to_ComboBox(TComboBox *cp, UnicodeString s,
	UnicodeString sp)	//区切り default = "|":
{
	for (;;) {
		UnicodeString itm = get_tkn(s, sp);
		if (itm.IsEmpty()) break;
		cp->Items->Add(itm);
		s = get_tkn_r(s, sp);
	}
}

//---------------------------------------------------------------------------
//グリッドの見出し設定
//---------------------------------------------------------------------------
void str_to_GridHeadder(TStringGrid *gp, UnicodeString s,
	UnicodeString sp)	//区切り default = "|":
{
	for (int i=0; i<gp->ColCount; i++) {
		UnicodeString itm = get_tkn(s, sp);
		if (itm.IsEmpty()) break;
		gp->Cells[i][0] = itm;
		s = get_tkn_r(s, sp);
	}
}

//---------------------------------------------------------------------------
//キーリストの作成
//---------------------------------------------------------------------------
void make_key_list(TStrings *lst)
{
	lst->Clear();
	UnicodeString tmpstr;

	for (int i=0; i<26; i++) lst->Add(tmpstr.sprintf(_T("%c"), 'A' + i));	//英字
	for (int i=0; i<10; i++) lst->Add(tmpstr.sprintf(_T("%c"), '0' + i));	//数字
	for (int i=0; i<12; i++) lst->Add(tmpstr.sprintf(_T("F%u"), i  + 1));	//Fキー

	lst->Add("Del");
	lst->Add("Ins");
	lst->Add("BkSp");
	lst->Add("PgUp");
	lst->Add("PgDn");
	lst->Add("Home");
	lst->Add("End");
	lst->Add("Left");
	lst->Add("Up");
	lst->Add("Right");
	lst->Add("Down");
	lst->Add("Tab");
	lst->Add("Esc");
	lst->Add("Enter");
	lst->Add("Space");
}

//---------------------------------------------------------------------------
//キーが押されているか?
//---------------------------------------------------------------------------
bool is_KeyDown(WORD key)
{
	return (HIBYTE(::GetAsyncKeyState(key)) != 0);
}

//---------------------------------------------------------------------------
//アプリケーションの製品バージョン取得
//---------------------------------------------------------------------------
UnicodeString get_VersionStr(
	UnicodeString fnam,		//ファイル名
	bool is_num)			//書式 true = "nnn"/ false = "n.n.n.0" (default = false)
{
	UnicodeString verstr;
	unsigned mj, mi, bl;
	if (GetProductVersion(fnam, mj, mi, bl)) {
		verstr.sprintf(is_num? _T("%u%u%u") : _T("%u.%u.%u.0"), mj, mi, bl);
	}
	return verstr;
}

//---------------------------------------------------------------------------
//「ポインターを自動的に規定のボタン上に移動」が有効化?
//---------------------------------------------------------------------------
bool is_SnapToDefBtn()
{
	BOOL sw;
	return (::SystemParametersInfo(SPI_GETSNAPTODEFBUTTON, 0, &sw, 0) && sw);
}

//---------------------------------------------------------------------------
//エラーメッセージ
//---------------------------------------------------------------------------
void msgbox_ERR(UnicodeString msg, UnicodeString tit)
{
	if (msg.IsEmpty()) return;
	Application->MessageBox(msg.c_str(), tit.c_str(), MB_OK|MB_ICONERROR);
}
//---------------------------------------------------------------------------
//警告メッセージ
//---------------------------------------------------------------------------
void msgbox_WARN(UnicodeString msg, UnicodeString tit)
{
	if (msg.IsEmpty()) return;
	Application->MessageBox(msg.c_str(), tit.c_str(), MB_OK|MB_ICONWARNING);
}
//---------------------------------------------------------------------------
//メッセージ
//---------------------------------------------------------------------------
void msgbox_OK(UnicodeString msg, UnicodeString tit)
{
	Application->MessageBox(msg.c_str(), tit.c_str(), MB_OK);
}
//---------------------------------------------------------------------------
//確認メッセージ
//---------------------------------------------------------------------------
bool msgbox_Y_N(UnicodeString msg, UnicodeString tit)
{
	int res = Application->MessageBox(msg.c_str(), tit.c_str(), MB_YESNO|MB_ICONQUESTION);
	return (res==IDYES);
}
//---------------------------------------------------------------------------
int msgbox_Y_N_C(UnicodeString msg, UnicodeString tit)
{
	return Application->MessageBox(msg.c_str(), tit.c_str(), MB_YESNOCANCEL|MB_ICONQUESTION);
}

//---------------------------------------------------------------------------
////HTMLヘルプ
//---------------------------------------------------------------------------
FUNC_HtmlHelp lpfHtmlHelp = NULL;
DWORD ProcessId = 0;				//プロセスID

//---------------------------------------------------------------------------
//ヘルプウィンドウを取得
//---------------------------------------------------------------------------
BOOL CALLBACK EnumHelpWndProc(HWND hWnd, LPARAM hHelp)
{
	DWORD pid;
	::GetWindowThreadProcessId(hWnd, &pid);
	if (ProcessId!=pid) return TRUE;

	_TCHAR cbuf[256];
	if (::GetClassName(hWnd, cbuf, 255)==0) return TRUE;
	if (_tcsicmp(cbuf, _T("HH Parent"))!=0) return TRUE;

	*((HWND*)hHelp) = hWnd;
	return FALSE;
}
//---------------------------------------------------------------------------
HWND get_HelpWnd()
{
	HWND hlp_wnd = NULL;
	::EnumWindows(EnumHelpWndProc, (LPARAM)&hlp_wnd);
	return hlp_wnd;
}

//---------------------------------------------------------------------------
//ヘルプを閉じる
//---------------------------------------------------------------------------
void HtmlHelpClose(
	bool only_fg)	//フォアグランドの場合にのみ閉じる (default = false)
{
	if (lpfHtmlHelp) {
		HWND hWnd = get_HelpWnd();
		if (only_fg && ::GetForegroundWindow()!=hWnd) hWnd = NULL;
		if (hWnd) lpfHtmlHelp(NULL, NULL, HH_CLOSE_ALL, 0);
	}
}
//---------------------------------------------------------------------------
//指定コンテキスト番号のトピックを表示
//---------------------------------------------------------------------------
void HtmlHelpContext(
	int idx,	//コンテキスト番号
	HWND hWnd)	//default = NULL
{
	if (!lpfHtmlHelp) return;

	for (int i=0; !hWnd && i<Screen->FormCount; i++) {
		TForm *fp = Screen->Forms[i];
		if (!fp->Active || fp->FormStyle==fsMDIChild || fp==Application->MainForm) continue;
		hWnd = fp->Handle;
	}
	if (!hWnd) hWnd = Application->MainForm->Handle;

	lpfHtmlHelp(hWnd, Application->HelpFile.c_str(), HH_HELP_CONTEXT, idx);
}
//---------------------------------------------------------------------------
