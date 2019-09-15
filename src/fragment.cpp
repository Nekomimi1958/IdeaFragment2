//----------------------------------------------------------------------//
// 断片のクラス															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <tchar.h>
#include <algorithm>
#include <RegularExpressions.hpp>
#include "usr_wic.h"
#include "usr_id3.h"
#include "UserFunc.h"
#include "EnvData.h"
#include "fragment.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//フィットする行数を取得
//---------------------------------------------------------------------------
int get_FragFitHi(TCanvas * cv, UnicodeString s, int wd)
{
	int w = get_FontWidthF(cv) * wd;
	if (w<=0) return EV->def_hi;

	int cp = 1;
	int n  = 1;
	int l  = 1;
	int crp  = s.Pos("\r\n");

	for (;;) {
		if (EV->accept_cr && crp==n) {
			cp += (n + 1); n = 1;
			crp = s.SubString(cp, MAX_FRG_WD*2).Pos("\r\n");
			l++;
		}
		else if (cv->TextWidth(s.SubString(cp, n))>w) {
			if (n>1) n--;
			cp += n;
			if (cp>s.Length()) break;
			n = 1;
			crp = s.SubString(cp, MAX_FRG_WD*2).Pos("\r\n");
			l++;
		}
		else {
			if ((cp + n - 1) >= s.Length()) break;
			n++;
		}
	}
	return l;
}

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
Fragment::Fragment(TForm *pf)
{
	ID			= -1;
	CreTime 	= Now();
	ModTime 	= CreTime;

	Parent		= pf;
	cv			= Parent? Parent->Canvas : NULL;

	wd			= 4;
	hi			= 1;
	RC			= Rect(2, 2, 10, 10);
	vRC 		= RC;
	tmpRC		= RC;
	img_wd		= 0;
	img_hi		= 0;
	Z			= 0;

	sX			= 0;
	sY			= 0;

	frgFont		= EV->frgFont;
	namFont 	= EV->namFont;
	symFont 	= EV->symFont;

	colFG		= EV->col_defFG;
	colBG		= EV->col_defBG;

	set_group(0);	//無所属
	style		= fgsNormal;

	Jumping 	= Visible = Erased = Noticed = Matched = false;
	OnGfrm		= false;

    Selected	= false;
	TextStr 	= EmptyStr;
	LinkName	= EmptyStr;

	ImgBuff 	= new Graphics::TBitmap();
	MetaBuff	= NULL;

	Changed 	= true;
	Initialized = false;
}
//---------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------
Fragment::~Fragment()
{
	era_frag();
	delete ImgBuff;
	if (MetaBuff) delete MetaBuff;
}

//---------------------------------------------------------------------------
//リンク内容の更新 (画像読込)
//---------------------------------------------------------------------------
void Fragment::upd_link()
{
	img_wd = img_hi = 48;

	if (style==fgsImage) {
		ImgBuff->Handle = NULL;

		if (FileExists(LinkName)) {
			try {
				//メタファイル
				if (test_FileExt(ExtractFileExt(LinkName), ".wmf.emf")) {
					if (!MetaBuff) MetaBuff = new TMetafile();
					MetaBuff->LoadFromFile(LinkName);
					if (MetaBuff->Empty) Abort();
					img_wd = MetaBuff->Width * wd / hi;
					img_hi = MetaBuff->Height * wd / hi;
					colBG  = Graphics::clNone;
				}
				//画像ファイル
				else {
					if (MetaBuff) delete MetaBuff;
					bool ok = false;
					if (test_FileExt(ExtractFileExt(LinkName), ".mp3")) {
						ok = ID3_GetImage(LinkName, ImgBuff);
					}
					else {
						if (EV->UseSpiFirst) ok = SPI->LoadImage(LinkName, ImgBuff);
						if (!ok) ok = WIC_load_image(LinkName, ImgBuff);
						if (!ok && !EV->UseSpiFirst) ok = SPI->LoadImage(LinkName, ImgBuff);
					}
					if (!ok || ImgBuff->Empty) Abort();
					img_wd = ImgBuff->Width *wd/hi;
					img_hi = ImgBuff->Height *wd/hi;
					//倍率
					if (wd!=hi) {
						std::unique_ptr<Graphics::TBitmap> tmp_img(new Graphics::TBitmap());
						tmp_img->Assign(ImgBuff);
						if (!WIC_resize_image(tmp_img.get(), ImgBuff, 0.0, img_wd, img_hi, EV->WicScaleOpt))
							Abort();
					}
					ImgBuff->Transparent = (colBG==Graphics::clNone);
				}
			}
			catch (...) {
				ImgBuff->Handle = NULL;
				if (MetaBuff) delete MetaBuff;
				img_wd = img_hi = 48;
			}
		}
	}
}

//---------------------------------------------------------------------------
//断片を消す
//---------------------------------------------------------------------------
void Fragment::era_frag()
{
	if (!Parent || Erased) return;

	int dx = Parent->HorzScrollBar->Position;
	int dy = Parent->VertScrollBar->Position;
	vRC.Left   = RC.Left   - dx;
	vRC.Right  = RC.Right  - dx;
	vRC.Top	   = RC.Top    - dy;
	vRC.Bottom = RC.Bottom - dy;

	if (!Parent->ClientRect.IntersectsWith(vRC)) return;	//画面外は描画処理しない

	cv->Brush->Color = Parent->Color;
	cv->FillRect(vRC);
	Erased = true;
}

//---------------------------------------------------------------------------
//断片のテキストを出力
//---------------------------------------------------------------------------
void Fragment::out_frag_text(TCanvas *t_cv, TRect tRC, int mgn, int itl)
{
	int cp = 1, n = 1, l = 1;
	int crp = TextStr.Pos(EV->CR_str);
	int lw	= get_FontHeight(t_cv, itl);
	int txt_len = TextStr.Length();

	//検索結果を強調表示
	if (Selected && !EV->FindWord.IsEmpty() && Matched && !EV->FindNotSw) {
		std::unique_ptr<TColor[]> FgCol(new TColor[txt_len + 2]);
		std::unique_ptr<TColor[]> BgCol(new TColor[txt_len + 2]);

		for (int i=0; i<=txt_len; i++) {
			FgCol[i] = EV->col_selFG;
			BgCol[i] = EV->col_selBG;
		}

		//正規表現
		if (EV->FindRegSw) {
			TRegExOptions opt;
			if (!EV->FindCaseSw) opt << roIgnoreCase;
			TMatchCollection mts = TRegEx::Matches(TextStr, EV->FindWord, opt);
			for (int i=0; i<mts.Count; i++) {
				for (int j=0,p=mts.Item[i].Index; j<mts.Item[i].Length; j++,p++) {
					FgCol[p] = EV->col_selBG;
					BgCol[p] = EV->col_selFG;
				}
			}
		}
		//通常検索
		else {
			std::unique_ptr<TStringList> wlst(new TStringList());
			get_find_wd_list(EV->FindWord, wlst.get());
			int ofs = 1;
			for (;;) {
				int p1 = 0;	//見つかった語の末尾位置 + 1 の最後尾
				for (int i=0; i<wlst->Count; i++) {
					UnicodeString kwd = wlst->Strings[i];
					int p = EV->FindCaseSw? PosEx(kwd, TextStr, ofs) : PosEx(kwd.UpperCase(), TextStr.UpperCase(), ofs);
					if (p>0) {
						int len = kwd.Length();
						for (int j=0,k=p; j<len; j++,k++) {
							FgCol[k] = EV->col_selBG;
							BgCol[k] = EV->col_selFG;
						}
						p1 = std::max(p + len, p1);
					}
				}
				if (p1==0) break;
				ofs = p1;
			}
		}

		t_cv->Font->Color  = FgCol[1];
		t_cv->Brush->Color = BgCol[1];

		if (t_cv->Font->Name[1]=='@') {
		//縦書き表示
			int xp = tRC.Right - mgn;
			int yp = tRC.Top + mgn;
			int wd = tRC.Bottom - tRC.Top - mgn*2;

			for (;;) {
				if (EV->accept_cr && crp==n) {
					color_textout_v(t_cv, xp, yp, TextStr.SubString(cp, n - 1), &FgCol[cp], &BgCol[cp]);
					xp -= lw;
					cp += (n + EV->CR_str.Length() - 1);
					n = 1;
					crp = PosEx(EV->CR_str, TextStr, cp);
					if (crp>0) crp -= (cp - 1);
					l++; if (l>hi) break;
				}
				else if (t_cv->TextWidth(TextStr.SubString(cp, n)) > wd) {
					n--; if (n==0) n = 1;
					color_textout_v(t_cv, xp, yp, TextStr.SubString(cp, n), &FgCol[cp], &BgCol[cp]);
					cp += n;
					if (cp > txt_len) break;
					n = 1;
					crp = PosEx(EV->CR_str, TextStr, cp);
					if (crp>0) crp -= (cp - 1);
					xp -= lw;
					l++; if (l>hi) break;
				}
				else {
					if ((cp + n - 1) >= txt_len) {
						color_textout_v(t_cv, xp, yp, TextStr.SubString(cp, n), &FgCol[cp], &BgCol[cp]);
						break;
					}
					n++;
				}
			}
		}
		//横書き表示
		else {
			int y_mgn = mgn;
			if (y_mgn>0) {
				int ld = t_cv->TextHeight("Q") - abs(t_cv->Font->Height);
				if (ld>0) y_mgn -= ld;
				if (y_mgn<0) y_mgn = 0;
			}
			int yp = tRC.Top + y_mgn + 2;
			int xp = tRC.Left + mgn;
			int wd = tRC.Width() - mgn*2;

			for (;;) {
				if (EV->accept_cr && crp==n) {
					color_textout(t_cv, xp, yp, TextStr.SubString(cp, n - 1), &FgCol[cp], &BgCol[cp]);
					yp += lw;
					cp += (n + EV->CR_str.Length() - 1);
					n = 1;
					crp = PosEx(EV->CR_str, TextStr, cp);
					if (crp>0) crp -= (cp - 1);
					l++; if (l>hi) break;
				}
				else if (t_cv->TextWidth(TextStr.SubString(cp, n)) > wd) {
					n--; if (n==0) n = 1;
					color_textout(t_cv, xp, yp, TextStr.SubString(cp, n), &FgCol[cp], &BgCol[cp]);
					cp += n;
					if (cp > txt_len) break;
					n = 1;
					crp = PosEx(EV->CR_str, TextStr, cp);
					if (crp>0) crp -= (cp - 1);
					yp += lw;
					l++; if (l>hi) break;
				}
				else {
					if ((cp + n - 1) >= txt_len) {
						color_textout(t_cv, xp, yp, TextStr.SubString(cp, n), &FgCol[cp], &BgCol[cp]);
						break;
					}
					n++;
				}
			}
		}
	}
	else {
		if (t_cv->Font->Name[1]=='@') {
		//縦書き表示
			LOGFONT lf;
			::GetObject(t_cv->Font->Handle, sizeof(LOGFONT), &lf);
			lf.lfEscapement = lf.lfOrientation = -900;
			HFONT hTmpFont = ::CreateFontIndirect(&lf);
			HFONT hOrgFont = (HFONT)::SelectObject(t_cv->Handle, hTmpFont);
			int xp = tRC.Right - mgn;
			int yp = tRC.Top + mgn;
			int wd = tRC.Bottom - tRC.Top - mgn*2;

			for (;;) {
				if (EV->accept_cr && crp==n) {
					t_cv->TextOut(xp, yp, TextStr.SubString(cp, n - 1));
					xp -= lw;
					cp += (n + EV->CR_str.Length() - 1);
					n = 1;
					crp = PosEx(EV->CR_str, TextStr, cp);
					if (crp>0) crp -= (cp - 1);
					l++; if (l>hi) break;
				}
				else if (t_cv->TextWidth(TextStr.SubString(cp, n)) > wd) {
					n--; if (n==0) n = 1;
					t_cv->TextOut(xp, yp, TextStr.SubString(cp, n));
					cp += n;
					if (cp > txt_len) break;
					n = 1;
					crp = PosEx(EV->CR_str, TextStr, cp);
					if (crp>0) crp -= (cp - 1);
					xp -= lw;
					l++; if (l>hi) break;
				}
				else {
					if ((cp + n - 1) >= txt_len) {
						t_cv->TextOut(xp, yp, TextStr.SubString(cp, n));
						break;
					}
					n++;
				}
			}

			::SelectObject(t_cv->Handle, hOrgFont);
			::DeleteObject(hTmpFont);

		}
		//横書き表示
		else {
			int y_mgn = mgn;
			if (y_mgn>0) {
				int ld = t_cv->TextHeight("Q") - abs(t_cv->Font->Height);
				if (ld>0) y_mgn -= ld;
				if (y_mgn<0) y_mgn = 0;
			}
			int yp = tRC.Top + y_mgn + 2;
			int xp = tRC.Left + mgn;
			int wd = tRC.Width() - mgn*2;

			for (;;) {
				if (EV->accept_cr && crp==n) {
					t_cv->TextOut(xp, yp, TextStr.SubString(cp, n - 1));
					yp += lw;
					cp += (n + EV->CR_str.Length() - 1);
					n = 1;
					crp = PosEx(EV->CR_str, TextStr, cp);
					if (crp>0) crp -= (cp - 1);
					l++; if (l>hi) break;
				}
				else if (t_cv->TextWidth(TextStr.SubString(cp, n)) > wd) {
					n--; if (n==0) n = 1;
					t_cv->TextOut(xp, yp, TextStr.SubString(cp, n));
					cp += n;
					if (cp > txt_len) break;
					n = 1;
					crp = PosEx(EV->CR_str, TextStr, cp);
					if (crp>0) crp -= (cp - 1);
					yp += lw;
					l++; if (l>hi) break;
				}
				else {
					if ((cp + n - 1) >= txt_len) {
						t_cv->TextOut(xp, yp, TextStr.SubString(cp, n));
						break;
					}
					n++;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//スモールアイコンを描画
// 拡張子依存アイコンは EV->LinkIconList に登録して再利用
//---------------------------------------------------------------------------
bool Fragment::draw_icon(
	UnicodeString fnam,		//ファイル名
	TCanvas *cv,
	int x, int y,			//表示位置
	bool act)				//アクティブ表示(ジャンプ時)
{
	HICON hIcon = NULL;

	UnicodeString fext;
	if (StartsText("http://", fnam) || StartsText("https:", fnam))
		fext = ".html";
	else if (StartsText("onenote:///", fnam))
		fext = ".one";
	else
		fext = ExtractFileExt(fnam);

	//実ファイル依存
	if (test_FileExt(fext, ".exe.ico.cur.ani.lnk")) {
		int idx = EV->CachedIcoList->IndexOf(fnam);
		if (idx!=-1) {
			hIcon = ((TIcon*)EV->CachedIcoList->Objects[idx])->Handle;
		}
		else {
			UINT flag = SHGFI_ICON|SHGFI_SMALLICON;
			if (fnam.IsEmpty()) {
				fnam = ".exe";
				flag |= SHGFI_USEFILEATTRIBUTES;
			}
			SHFILEINFO sif;
			if (::SHGetFileInfo(fnam.c_str(), faArchive, &sif, sizeof(SHFILEINFO), flag)) {
				hIcon = sif.hIcon;
				TIcon *icon  = new TIcon();
				icon->Handle = hIcon;
				EV->CachedIcoList->AddObject(fnam, (TObject*)icon);
			}
		}
	}
	//拡張子依存
	else {
		int idx = EV->LinkIconList->IndexOf(fext);
		if (idx==-1) {
			UnicodeString cmd = exclude_quot(EV->get_associate(fnam));
			fnam = (!cmd.IsEmpty())? cmd : fext;

			SHFILEINFO sif;
			if (::SHGetFileInfo(fnam.c_str(), faArchive,
				&sif, sizeof(SHFILEINFO), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_USEFILEATTRIBUTES))
			{
				hIcon = sif.hIcon;
				TIcon *icon = new TIcon();
				icon->Handle = hIcon;
				EV->LinkIconList->AddObject(fext, (TObject*)icon);
			}
		}
		else {
			hIcon = ((TIcon*)EV->LinkIconList->Objects[idx])->Handle;
		}
	}

	//描画
	if (hIcon) {
		if (act) {
			std::unique_ptr<Graphics::TBitmap> mrk(new Graphics::TBitmap());
			mrk->SetSize(16, 16);
			::DrawIconEx(mrk->Canvas->Handle, 0, 0, hIcon, 16, 16, 0, NULL, DI_NORMAL);
			cv->CopyMode = cmNotSrcCopy;
			cv->Draw(x, y, mrk.get());
		}
		else {
			::DrawIconEx(cv->Handle, x, y, hIcon, 16, 16, 0, NULL, DI_NORMAL);
		}
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
//断片を表示
//---------------------------------------------------------------------------
void Fragment::show_frag()
{
	if (!Parent) return;

	int dx = Parent->HorzScrollBar->Position;
	int dy = Parent->VertScrollBar->Position;
	int w, h;

	//断片のサイズを求める
	//画像
	if (style==fgsImage) {
		RC.Right  = RC.Left + img_wd;
		RC.Bottom = RC.Top  + img_hi;
	}
	//中点
	else if (style==fgsJunction) {
		w = h = 8;
		RC.Right  = RC.Left + w;
		RC.Bottom = RC.Top  + h;
	}
	//断片・表札・標識
	else {
		switch (style) {
		case fgsNormal: cv->Font->Assign(frgFont); break;
		case fgsPlate:	cv->Font->Assign(namFont); break;
		case fgsSymbol:	cv->Font->Assign(symFont); break;
		default:		;
		}

		w = (style==fgsSymbol)? cv->TextWidth(TextStr) : get_FontWidthF(cv) * wd;
		h = get_FontHeight(cv, EV->frg_itl) * hi;

		if (cv->Font->Name[1]=='@') std::swap(h, w);
		RC.Right  = RC.Left + w + EV->frg_mgn*2;
		RC.Bottom = RC.Top  + h + EV->frg_mgn*2;
	}

	//断片の現在表示位置を設定
	vRC.Left   = RC.Left   - dx;
	vRC.Right  = RC.Right  - dx;
	vRC.Top	   = RC.Top    - dy;
	vRC.Bottom = RC.Bottom - dy;
	Initialized = true;

	if (!Visible) return;
	if (style==fgsPlate && OnGfrm) return;

	TRect s_rc = vRC;
	InflateRect(&s_rc, -1, -1);	//幅=2の輪郭を描くため

	//画像
	if (style==fgsImage) {
		if (!Selected) {
			cv->Brush->Color = Parent->Color;
			cv->FrameRect(vRC);
			cv->FrameRect(s_rc);
		}
		if (!ImgBuff->Empty || (MetaBuff && !MetaBuff->Empty)) {
			cv->CopyMode = cmSrcCopy;
			//メタファイル
			if (MetaBuff)
				cv->StretchDraw(vRC, MetaBuff);
			//ビットマップ
			else
				cv->Draw(vRC.Left, vRC.Top, ImgBuff);

			if (Selected) {
				cv->Brush->Color = Noticed? EV->col_selRct : EV->col_selBG;
				cv->FrameRect(vRC);
				cv->Brush->Color = EV->col_selBG;
				cv->FrameRect(s_rc);
			}
		}
		else {
			cv->Brush->Color = Selected? EV->col_selBG : Parent->Color;
			cv->FillRect(vRC);
			cv->Brush->Color = EV->col_Border;
			cv->FrameRect(vRC);
		}

		//リンクマーク(MP3)
		if (EV->show_LinkMark && test_FileExt(ExtractFileExt(LinkName), ".mp3")) {
			//アイコン
			if (EV->LinkIsIcon) {
				draw_icon(LinkName, cv, vRC.right - 16, vRC.bottom - 16, Jumping);
			}
			//マーク
			else {
				cv->CopyMode = Jumping? cmNotSrcCopy : cmSrcCopy;
				cv->Draw(vRC.right - 16, vRC.bottom - 16, EV->LinkMark);
			}
		}
	}
	//断片・表札・標識・中点
	else {
		//画面の外ならイメージを破棄
		if (!Parent->ClientRect.IntersectsWith(vRC)) {
			ImgBuff->Handle = NULL;
			return;
		}

		//断片イメージを(再)構築
		if (Changed || ImgBuff->Empty) {
			TRect irc = Rect(0, 0, w + EV->frg_mgn*2, h + EV->frg_mgn*2);
			ImgBuff->SetSize(RC.Width(), RC.Height());

			TCanvas *icv = ImgBuff->Canvas;
			switch (style) {
			case fgsNormal:	icv->Font->Assign(frgFont); break;
			case fgsPlate:	icv->Font->Assign(namFont); break;
			case fgsSymbol:	icv->Font->Assign(symFont); break;
			default:		;
			}

			//背景
			//標識・中点
			if (style==fgsSymbol || style==fgsJunction) {
				icv->Font->Color		  = Selected? EV->col_selFG : colFG;
				icv->Brush->Color		  = Parent->Color;
				ImgBuff->TransparentColor = Parent->Color;
				ImgBuff->Transparent	  = true;
			}
			//断片
			else {
				icv->Font->Color  = Selected? EV->col_selFG : colFG;
				icv->Brush->Color = Selected? EV->col_selBG : colBG;
				ImgBuff->Transparent  = false;
			}
			icv->FillRect(irc);

			//テキスト
			if (style!=fgsJunction) out_frag_text(icv, irc, EV->frg_mgn, EV->frg_itl);

			//リンクマーク
			if (EV->show_LinkMark && !LinkName.IsEmpty()) {
				UnicodeString fnam = LinkName;
				int p = pos_r(".IFD#", fnam.UpperCase());
				if (p>0) fnam = fnam.SubString(1, p + 3);
				int ixp = (cv->Font->Name[1]=='@')? irc.left : irc.right - 16;
				//アイコン
				if (!fnam.IsEmpty() && EV->LinkIsIcon && !test_FileExt(ExtractFileExt(fnam), ".ifd"))
					draw_icon(fnam, icv, ixp, irc.bottom - 16, Jumping);
				//マーク
				else {
					icv->CopyMode = Jumping? cmNotSrcCopy : cmSrcCopy;
					icv->Draw(ixp, irc.bottom - 16, EV->LinkMark);
				}
			}

			//標識・中点以外の輪郭
			if (style!=fgsSymbol && style!=fgsJunction) {
				icv->Brush->Color = (Noticed && Selected)? EV->col_selRct : EV->col_Border;
				icv->FrameRect(irc);
			}
		}

		cv->CopyMode = cmSrcCopy;
		cv->Draw(vRC.Left, vRC.Top, ImgBuff);

		//選択中標識の輪郭
		if ((style==fgsSymbol || style==fgsJunction) && Selected) {
			cv->Brush->Color = Noticed? EV->col_selRct : EV->col_selBG;
			cv->FrameRect(vRC);
			if (style==fgsSymbol) cv->FrameRect(s_rc);
		}
	}

	Changed = false;
	Erased	= false;
}

//---------------------------------------------------------------------------
//断片を印刷
//---------------------------------------------------------------------------
void Fragment::print_frag(
	TCanvas *p_cv,
	double ratio)	//倍率	=0: プリンタ以外のキャンバスへ出力
{
	if (!Visible) return;
	if (style==fgsPlate && OnGfrm) return;
	if (style==fgsJunction) return;

	bool out_prn = (ratio!=0);
	if (!out_prn) ratio = 1.0;

	//画像
	if (style==fgsImage) {
		TRect pRC = Rect(round_i(RC.Left * ratio), round_i(RC.Top * ratio),
						 round_i(RC.Right * ratio), round_i(RC.Bottom * ratio));
		//メタファイル
		if (MetaBuff && !MetaBuff->Empty) {
			p_cv->StretchDraw(pRC, MetaBuff);
		}
		//ビットマップ
		else if (!ImgBuff->Empty) {
			std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
			bmp->SetSize(ImgBuff->Width, ImgBuff->Height);
			bmp->Canvas->Draw(0, 0, ImgBuff);
			p_cv->StretchDraw(pRC, bmp.get());
		}
	}
	else {
		int f_hi;
		switch (style) {
		case fgsNormal:
			p_cv->Font->Assign(frgFont);
			f_hi = frgFont->Height;
			break;
		case fgsPlate:
			p_cv->Font->Assign(namFont);
			f_hi = namFont->Height;
			break;
		case fgsSymbol:
			p_cv->Font->Assign(symFont);
			f_hi = symFont->Height;
			break;
		default:
			;
		}
		p_cv->Font->Height = (int)(ratio*f_hi);

		int p_fh = get_FontHeight(p_cv);
		int mg = round_i(EV->frg_mgn * 2 * ratio);
		int w = (style==fgsSymbol)? p_cv->TextWidth(TextStr) + mg : p_fh*wd;
		int h = p_fh*hi + round_i(ratio * ( hi*2));

		if (p_cv->Font->Name[1]=='@') std::swap(h, w);

		TRect pRC;
		pRC.Left   = (int)(RC.Left * ratio);
		pRC.Top	   = (int)(RC.Top  * ratio);
		pRC.Right  = pRC.Left + w + mg;
		pRC.Bottom = pRC.Top  + h + mg;

		//背景
		p_cv->Brush->Color = ((out_prn && EV->PrintMono)? clWhite : colBG);
		if (style!=fgsSymbol && style!=fgsJunction) p_cv->FillRect(pRC);

		//テキスト
		p_cv->Font->Color  = ((out_prn && EV->PrintMono)? clBlack : colFG);
		p_cv->Brush->Style = bsClear;
		out_frag_text(p_cv, pRC, (int)(ratio*EV->frg_mgn), (int)(ratio*EV->frg_itl));
		p_cv->Brush->Style = bsSolid;

		//輪郭
		if (style!=fgsSymbol && style!=fgsJunction) {
			p_cv->Pen->Color = ((out_prn && EV->PrintMono)? clBlack : EV->col_Border);

			if (out_prn) {
				if (EV->FixPrnLWidth>0)
					p_cv->Pen->Width = EV->FixPrnLWidth;
				else
					p_cv->Pen->Width = (ratio>1.0)? (int)ratio : 1;
			}
			else
				p_cv->Pen->Width = 1;

			p_cv->MoveTo(pRC.Left,	pRC.Top);
			p_cv->LineTo(pRC.Right,	pRC.Top);
			p_cv->LineTo(pRC.Right,	pRC.Bottom);
			p_cv->LineTo(pRC.Left,	pRC.Bottom);
			p_cv->LineTo(pRC.Left,	pRC.Top);
		}
	}
}

//---------------------------------------------------------------------------
//断片を縮小表示
//---------------------------------------------------------------------------
void Fragment::view_frag(TCanvas *v_cv, double ratio)
{
	if (!Initialized || !Visible) return;
	if (style==fgsPlate && OnGfrm) return;
	if (style==fgsJunction) return;

	TRect sRC = Rect(
		round_i(RC.Left  * ratio), round_i(RC.Top    * ratio),
		round_i(RC.Right * ratio), round_i(RC.Bottom * ratio));

	//背景部分が見えるように最小サイズ補正
	if ((sRC.Bottom - sRC.Top) < 3) sRC.Bottom = sRC.Top  + 3;
	if ((sRC.Right - sRC.Left) < 3) sRC.Right  = sRC.Left + 3;

	//画像
	if (style==fgsImage) {
		if (MetaBuff && !MetaBuff->Empty) {
			v_cv->StretchDraw(sRC, MetaBuff);
		}
		else if (!ImgBuff->Empty) {
			v_cv->StretchDraw(sRC, ImgBuff);
		}
		else {
			v_cv->Brush->Color = EV->col_Border;
			v_cv->FrameRect(sRC);
		}
	}
	//断片背景
	else {
		if (style!=fgsSymbol) {
			v_cv->Brush->Color = Selected? EV->col_selBG : colBG;
			v_cv->FillRect(sRC);
		}
		//輪郭
		v_cv->Brush->Color = EV->col_Border;
		v_cv->FrameRect(sRC);
	}
}

//---------------------------------------------------------------------------
//断片内容の複写
//---------------------------------------------------------------------------
void Fragment::assign(
	Fragment *fp, 
	bool id_sw,		//ID を複写
	bool sel_sw)	//選択状態を社
{
	TextStr  = fp->TextStr;
	group	 = fp->group;
	style	 = fp->style;
	RC		 = fp->RC;
	wd		 = fp->wd;
	hi		 = fp->hi;
	colFG	 = fp->colFG;
	colBG	 = fp->colBG;
	LinkName = fp->LinkName;
	Visible  = fp->Visible;
	CreTime  = fp->CreTime;
	ModTime  = fp->ModTime;

	if (id_sw)  ID = fp->ID;
	if (sel_sw) Selected = fp->Selected;
}

//---------------------------------------------------------------------------
//指定グループに含まれているか?
//　戻り値: 含まれていればそのスタック位置、そうでなければ -1
//---------------------------------------------------------------------------
int Fragment::in_group(int gn)
{
	if (gn<0) return -1;

	int ret = -1;
	for (int i=0; i<MAX_IN_GRP; i++) {
		if (group.stk[i]==-1) break;
		if (group.stk[i]==gn) { ret = i; break; }
	}
	return ret;
}
//---------------------------------------------------------------------------
//トップグループを取得
//---------------------------------------------------------------------------
int Fragment::get_top_group()
{
	int gnx = group.stk[0];
	for (int i=1; i<MAX_IN_GRP; i++) {
		if (group.stk[i]==-1) break;
		gnx = group.stk[i];
	}
	return gnx;
}

//---------------------------------------------------------------------------
//所属グループを設定
// add_sw 追加
// top_sw 先頭に挿入
//---------------------------------------------------------------------------
bool Fragment::set_group(int gn, bool add_sw, bool top_sw)
{
	bool ret = false;
	if (add_sw) {
		if (in_group(gn)==-1) {
			if (top_sw) {
				if (group.stk[MAX_IN_GRP - 1] == -1) {
					if (group.stk[0]!=0)
						for (int i=MAX_IN_GRP-1; i>0; i--) group.stk[i] = group.stk[i - 1];
					group.stk[0] = gn;
					ret = true;
				}
			}
			else {
				for (int i=0; i<MAX_IN_GRP; i++) {
					if (group.stk[i]>0) continue;
					group.stk[i] = gn;
					ret = true;
					break;
				}
			}
		}
	}
	else {
		for (int i=0; i<MAX_IN_GRP; i++) group.stk[i] = (i==0)? gn : -1;
		ret = true;
	}
	return ret;
}
//---------------------------------------------------------------------------
//所属グループを解除
//  戻り値: 解除したスタック位置／ 元々所属していないなら -1
//---------------------------------------------------------------------------
int Fragment::rel_group(int gn)
{
	if (gn<=0) return -1;

	int n = -1;
	for (int i=0; i<MAX_IN_GRP; i++) {
		if (group.stk[i]==gn) { n = i; break; }
	}
	if (n==-1)  return -1;

	for (int i=n; i<(MAX_IN_GRP - 1); i++) {
		group.stk[i] = group.stk[i + 1];
	}
	group.stk[MAX_IN_GRP - 1] = -1;
	if (group.stk[0]==-1) group.stk[0] = 0;

	return n;
}
//---------------------------------------------------------------------------

