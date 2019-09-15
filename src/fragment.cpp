//----------------------------------------------------------------------//
// �f�Ђ̃N���X															//
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
//�t�B�b�g����s�����擾
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
// �R���X�g���N�^
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

	set_group(0);	//������
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
// �f�X�g���N�^
//---------------------------------------------------------------------------
Fragment::~Fragment()
{
	era_frag();
	delete ImgBuff;
	if (MetaBuff) delete MetaBuff;
}

//---------------------------------------------------------------------------
//�����N���e�̍X�V (�摜�Ǎ�)
//---------------------------------------------------------------------------
void Fragment::upd_link()
{
	img_wd = img_hi = 48;

	if (style==fgsImage) {
		ImgBuff->Handle = NULL;

		if (FileExists(LinkName)) {
			try {
				//���^�t�@�C��
				if (test_FileExt(ExtractFileExt(LinkName), ".wmf.emf")) {
					if (!MetaBuff) MetaBuff = new TMetafile();
					MetaBuff->LoadFromFile(LinkName);
					if (MetaBuff->Empty) Abort();
					img_wd = MetaBuff->Width * wd / hi;
					img_hi = MetaBuff->Height * wd / hi;
					colBG  = Graphics::clNone;
				}
				//�摜�t�@�C��
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
					//�{��
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
//�f�Ђ�����
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

	if (!Parent->ClientRect.IntersectsWith(vRC)) return;	//��ʊO�͕`�揈�����Ȃ�

	cv->Brush->Color = Parent->Color;
	cv->FillRect(vRC);
	Erased = true;
}

//---------------------------------------------------------------------------
//�f�Ђ̃e�L�X�g���o��
//---------------------------------------------------------------------------
void Fragment::out_frag_text(TCanvas *t_cv, TRect tRC, int mgn, int itl)
{
	int cp = 1, n = 1, l = 1;
	int crp = TextStr.Pos(EV->CR_str);
	int lw	= get_FontHeight(t_cv, itl);
	int txt_len = TextStr.Length();

	//�������ʂ������\��
	if (Selected && !EV->FindWord.IsEmpty() && Matched && !EV->FindNotSw) {
		std::unique_ptr<TColor[]> FgCol(new TColor[txt_len + 2]);
		std::unique_ptr<TColor[]> BgCol(new TColor[txt_len + 2]);

		for (int i=0; i<=txt_len; i++) {
			FgCol[i] = EV->col_selFG;
			BgCol[i] = EV->col_selBG;
		}

		//���K�\��
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
		//�ʏ팟��
		else {
			std::unique_ptr<TStringList> wlst(new TStringList());
			get_find_wd_list(EV->FindWord, wlst.get());
			int ofs = 1;
			for (;;) {
				int p1 = 0;	//����������̖����ʒu + 1 �̍Ō��
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
		//�c�����\��
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
		//�������\��
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
		//�c�����\��
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
		//�������\��
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
//�X���[���A�C�R����`��
// �g���q�ˑ��A�C�R���� EV->LinkIconList �ɓo�^���čė��p
//---------------------------------------------------------------------------
bool Fragment::draw_icon(
	UnicodeString fnam,		//�t�@�C����
	TCanvas *cv,
	int x, int y,			//�\���ʒu
	bool act)				//�A�N�e�B�u�\��(�W�����v��)
{
	HICON hIcon = NULL;

	UnicodeString fext;
	if (StartsText("http://", fnam) || StartsText("https:", fnam))
		fext = ".html";
	else if (StartsText("onenote:///", fnam))
		fext = ".one";
	else
		fext = ExtractFileExt(fnam);

	//���t�@�C���ˑ�
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
	//�g���q�ˑ�
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

	//�`��
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
//�f�Ђ�\��
//---------------------------------------------------------------------------
void Fragment::show_frag()
{
	if (!Parent) return;

	int dx = Parent->HorzScrollBar->Position;
	int dy = Parent->VertScrollBar->Position;
	int w, h;

	//�f�Ђ̃T�C�Y�����߂�
	//�摜
	if (style==fgsImage) {
		RC.Right  = RC.Left + img_wd;
		RC.Bottom = RC.Top  + img_hi;
	}
	//���_
	else if (style==fgsJunction) {
		w = h = 8;
		RC.Right  = RC.Left + w;
		RC.Bottom = RC.Top  + h;
	}
	//�f�ЁE�\�D�E�W��
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

	//�f�Ђ̌��ݕ\���ʒu��ݒ�
	vRC.Left   = RC.Left   - dx;
	vRC.Right  = RC.Right  - dx;
	vRC.Top	   = RC.Top    - dy;
	vRC.Bottom = RC.Bottom - dy;
	Initialized = true;

	if (!Visible) return;
	if (style==fgsPlate && OnGfrm) return;

	TRect s_rc = vRC;
	InflateRect(&s_rc, -1, -1);	//��=2�̗֊s��`������

	//�摜
	if (style==fgsImage) {
		if (!Selected) {
			cv->Brush->Color = Parent->Color;
			cv->FrameRect(vRC);
			cv->FrameRect(s_rc);
		}
		if (!ImgBuff->Empty || (MetaBuff && !MetaBuff->Empty)) {
			cv->CopyMode = cmSrcCopy;
			//���^�t�@�C��
			if (MetaBuff)
				cv->StretchDraw(vRC, MetaBuff);
			//�r�b�g�}�b�v
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

		//�����N�}�[�N(MP3)
		if (EV->show_LinkMark && test_FileExt(ExtractFileExt(LinkName), ".mp3")) {
			//�A�C�R��
			if (EV->LinkIsIcon) {
				draw_icon(LinkName, cv, vRC.right - 16, vRC.bottom - 16, Jumping);
			}
			//�}�[�N
			else {
				cv->CopyMode = Jumping? cmNotSrcCopy : cmSrcCopy;
				cv->Draw(vRC.right - 16, vRC.bottom - 16, EV->LinkMark);
			}
		}
	}
	//�f�ЁE�\�D�E�W���E���_
	else {
		//��ʂ̊O�Ȃ�C���[�W��j��
		if (!Parent->ClientRect.IntersectsWith(vRC)) {
			ImgBuff->Handle = NULL;
			return;
		}

		//�f�ЃC���[�W��(��)�\�z
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

			//�w�i
			//�W���E���_
			if (style==fgsSymbol || style==fgsJunction) {
				icv->Font->Color		  = Selected? EV->col_selFG : colFG;
				icv->Brush->Color		  = Parent->Color;
				ImgBuff->TransparentColor = Parent->Color;
				ImgBuff->Transparent	  = true;
			}
			//�f��
			else {
				icv->Font->Color  = Selected? EV->col_selFG : colFG;
				icv->Brush->Color = Selected? EV->col_selBG : colBG;
				ImgBuff->Transparent  = false;
			}
			icv->FillRect(irc);

			//�e�L�X�g
			if (style!=fgsJunction) out_frag_text(icv, irc, EV->frg_mgn, EV->frg_itl);

			//�����N�}�[�N
			if (EV->show_LinkMark && !LinkName.IsEmpty()) {
				UnicodeString fnam = LinkName;
				int p = pos_r(".IFD#", fnam.UpperCase());
				if (p>0) fnam = fnam.SubString(1, p + 3);
				int ixp = (cv->Font->Name[1]=='@')? irc.left : irc.right - 16;
				//�A�C�R��
				if (!fnam.IsEmpty() && EV->LinkIsIcon && !test_FileExt(ExtractFileExt(fnam), ".ifd"))
					draw_icon(fnam, icv, ixp, irc.bottom - 16, Jumping);
				//�}�[�N
				else {
					icv->CopyMode = Jumping? cmNotSrcCopy : cmSrcCopy;
					icv->Draw(ixp, irc.bottom - 16, EV->LinkMark);
				}
			}

			//�W���E���_�ȊO�̗֊s
			if (style!=fgsSymbol && style!=fgsJunction) {
				icv->Brush->Color = (Noticed && Selected)? EV->col_selRct : EV->col_Border;
				icv->FrameRect(irc);
			}
		}

		cv->CopyMode = cmSrcCopy;
		cv->Draw(vRC.Left, vRC.Top, ImgBuff);

		//�I�𒆕W���̗֊s
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
//�f�Ђ����
//---------------------------------------------------------------------------
void Fragment::print_frag(
	TCanvas *p_cv,
	double ratio)	//�{��	=0: �v�����^�ȊO�̃L�����o�X�֏o��
{
	if (!Visible) return;
	if (style==fgsPlate && OnGfrm) return;
	if (style==fgsJunction) return;

	bool out_prn = (ratio!=0);
	if (!out_prn) ratio = 1.0;

	//�摜
	if (style==fgsImage) {
		TRect pRC = Rect(round_i(RC.Left * ratio), round_i(RC.Top * ratio),
						 round_i(RC.Right * ratio), round_i(RC.Bottom * ratio));
		//���^�t�@�C��
		if (MetaBuff && !MetaBuff->Empty) {
			p_cv->StretchDraw(pRC, MetaBuff);
		}
		//�r�b�g�}�b�v
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

		//�w�i
		p_cv->Brush->Color = ((out_prn && EV->PrintMono)? clWhite : colBG);
		if (style!=fgsSymbol && style!=fgsJunction) p_cv->FillRect(pRC);

		//�e�L�X�g
		p_cv->Font->Color  = ((out_prn && EV->PrintMono)? clBlack : colFG);
		p_cv->Brush->Style = bsClear;
		out_frag_text(p_cv, pRC, (int)(ratio*EV->frg_mgn), (int)(ratio*EV->frg_itl));
		p_cv->Brush->Style = bsSolid;

		//�֊s
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
//�f�Ђ��k���\��
//---------------------------------------------------------------------------
void Fragment::view_frag(TCanvas *v_cv, double ratio)
{
	if (!Initialized || !Visible) return;
	if (style==fgsPlate && OnGfrm) return;
	if (style==fgsJunction) return;

	TRect sRC = Rect(
		round_i(RC.Left  * ratio), round_i(RC.Top    * ratio),
		round_i(RC.Right * ratio), round_i(RC.Bottom * ratio));

	//�w�i������������悤�ɍŏ��T�C�Y�␳
	if ((sRC.Bottom - sRC.Top) < 3) sRC.Bottom = sRC.Top  + 3;
	if ((sRC.Right - sRC.Left) < 3) sRC.Right  = sRC.Left + 3;

	//�摜
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
	//�f�Дw�i
	else {
		if (style!=fgsSymbol) {
			v_cv->Brush->Color = Selected? EV->col_selBG : colBG;
			v_cv->FillRect(sRC);
		}
		//�֊s
		v_cv->Brush->Color = EV->col_Border;
		v_cv->FrameRect(sRC);
	}
}

//---------------------------------------------------------------------------
//�f�Г��e�̕���
//---------------------------------------------------------------------------
void Fragment::assign(
	Fragment *fp, 
	bool id_sw,		//ID �𕡎�
	bool sel_sw)	//�I����Ԃ���
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
//�w��O���[�v�Ɋ܂܂�Ă��邩?
//�@�߂�l: �܂܂�Ă���΂��̃X�^�b�N�ʒu�A�����łȂ���� -1
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
//�g�b�v�O���[�v���擾
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
//�����O���[�v��ݒ�
// add_sw �ǉ�
// top_sw �擪�ɑ}��
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
//�����O���[�v������
//  �߂�l: ���������X�^�b�N�ʒu�^ ���X�������Ă��Ȃ��Ȃ� -1
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

