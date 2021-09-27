//----------------------------------------------------------------------//
// MP3 ID3�^�O���̏���												//
//																		//
//----------------------------------------------------------------------//
#include "usr_wic.h"
#include "usr_id3.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
unsigned int get_SynchsafeInt(BYTE *p)
{
	return (unsigned int)(((p[0]&0x7f) << 21) + ((p[1]&0x7f) << 14) + ((p[2]&0x7f) << 7) + (p[3] & 0x7f));
}
//---------------------------------------------------------------------------
unsigned int get_32bitInt(BYTE *p)
{
	return (unsigned int)((p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]);
}
//---------------------------------------------------------------------------
unsigned int get_24bitInt(BYTE *p)
{
	return (unsigned int)((p[0] << 16) + (p[1] << 8) + p[2]);
}

//---------------------------------------------------------------------------
UnicodeString get_str_from_buf(char *c, int size)
{
	std::unique_ptr<char []> ts(new char[size + 1]);
	char *p = ts.get();
	::CopyMemory(p, c, size);
	p += size;
	*p = '\0';
	UnicodeString ret_str = ts.get();
	return ret_str;
}

//---------------------------------------------------------------------------
//�o�[�W�������擾���A�擪�t���[���w�b�_��
//�߂�l 1: v1?  3:v2.3  4:v2.4
//---------------------------------------------------------------------------
bool ID3_getVer_toFrame(TFileStream *fs, unsigned int *last, unsigned int *majv)
{
	try {
		id3_hdr idhdr;
		if (fs->Read(&idhdr, 10)<10) Abort();
		if (idhdr.hdr_id[0]!='I' || idhdr.hdr_id[1]!='D' || idhdr.hdr_id[2]!='3') Abort();
		unsigned int size = get_SynchsafeInt(&idhdr.size[0]);
		*last = size + 10;
		*majv = idhdr.maj_ver;
		if (idhdr.maj_ver!=2 && idhdr.maj_ver!=3 && idhdr.maj_ver!=4) Abort();
		if (idhdr.maj_ver>=3 && (idhdr.flag & 0x40)!=0) {
			//�g���w�b�_������Γǂݔ�΂�
			BYTE exsz[4];
			id3_ex_hdr exhdr;
			if (fs->Read(&exsz[0], 4)<4) Abort();
			int ex_size = (idhdr.maj_ver==4)? get_SynchsafeInt(&exsz[0]) : get_32bitInt(&exsz[0]);
			if (idhdr.maj_ver==4) ex_size-=4,
			fs->Seek(ex_size, soFromCurrent);
		}
		return true;
	}
	catch (...) {
		return false;
	}
}


//---------------------------------------------------------------------------
//ID3 Tag v1 �����擾
//  ���ʂ� lst �� ����=���e �`���Ŋi�[
//	���ڂ͈ȉ��̒ʂ�
//	  TrackName  :�g���b�N��
//	  ArtistName :�A�[�e�B�X�g��
//	  AlbumName  :�A���o����
//	  Year		 :�����[�X�N��
//	  Comment	 :�R�����g
//---------------------------------------------------------------------------
bool ID3_GetV1Inf(
	UnicodeString fnam,		//�t�@�C����
	TStringList* lst)		//[o] ��񃊃X�g
{
	lst->Clear();
	if (!FileExists(fnam)) return false;

	bool ret = false;
	char fbuf[128];
	int hFile = FileOpen(fnam, fmOpenRead);
	if (hFile!=-1) {
		FileSeek(hFile, -128, 2);
		int n = FileRead(hFile, fbuf, sizeof(fbuf));
		FileClose(hFile);
		if (n==128) {
			char sbuf[32];
			UnicodeString tmpstr;
			char *s = &fbuf[0]; memcpy(sbuf, s, 3); sbuf[3] = '\0'; tmpstr = sbuf;
			if (SameText(tmpstr, "TAG")) {
				s = &fbuf[ 3]; memcpy(sbuf, s, 30); sbuf[30] = '\0'; tmpstr = sbuf;
				lst->Add("TrackName=" + tmpstr.Trim());
				s = &fbuf[33]; memcpy(sbuf, s, 30); sbuf[30] = '\0'; tmpstr = sbuf;
				lst->Add("ArtistName=" + tmpstr.Trim());
				s = &fbuf[63]; memcpy(sbuf, s, 30); sbuf[30] = '\0'; tmpstr = sbuf;
				lst->Add("AlbumName=" + tmpstr.Trim());
				s = &fbuf[93]; memcpy(sbuf, s,  4); sbuf[ 4] = '\0'; tmpstr = sbuf;
				lst->Add("Year=" + tmpstr.Trim());
				s = &fbuf[97]; memcpy(sbuf, s, 30); sbuf[30] = '\0'; tmpstr = sbuf;
				lst->Add("Comment=" + tmpstr.Trim());
				ret = true;
			}
		}
	}
	return ret;
}

//---------------------------------------------------------------------------
//MP3�t�@�C�����疄�ߍ��݉摜���r�b�g�}�b�v�Ŏ擾 or �t�@�C���ɏo��
//---------------------------------------------------------------------------
bool ID3_GetImage(
	UnicodeString fnam,			//�t�@�C����
	Graphics::TBitmap *bmpimg)	//�o�̓r�b�g�}�b�v
{
	bool ok = false;
	try {
		if (!FileExists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyWrite));

		//ID3v2�^�O�w�b�_
		unsigned int maj_ver = 0, last_p = 0;
		if (!ID3_getVer_toFrame(fs.get(), &last_p, &maj_ver)) Abort();

		//�t���[���w�b�_
		for (;;) {
			id3_frm_hdr   frmhdr;
			id3_frm_hdr_2 frmhdr2;
			UnicodeString id_str;
			if (maj_ver==2) {
				if (fs->Read(&frmhdr2, 6) < 6) Abort();
				id_str = get_str_from_buf(frmhdr2.frame_id, 3);
			}
			else {
				if (fs->Read(&frmhdr, 10) < 10) Abort();
				id_str = get_str_from_buf(frmhdr.frame_id, 4);
			}
			if (id_str.IsEmpty()) break;

			int size;
			switch (maj_ver) {
			case 2: size = get_24bitInt(&frmhdr2.size[0]);		break;
			case 3: size = get_32bitInt(&frmhdr.size[0]);		break;
			case 4: size = get_SynchsafeInt(&frmhdr.size[0]);	break;
			}

			if (SameText(id_str, "APIC") || SameText(id_str, "PIC")) {
				//�e�L�X�g�G���R�[�f�B���O
				BYTE code;
				if (fs->Read(&code, 1) < 1) Abort();
				size--;
				bool is_16 = (code==1 || code==2);

				UnicodeString img_typ;
				char tbuf[32];
				//�摜�`��
				if (maj_ver==2) {
					if (fs->Read(&tbuf[0], 3) < 3) Abort();
					tbuf[3] = '\0';
					size -= 3;
				}
				//MIMI�^�C�v
				else {
					for (unsigned int i=0; i<sizeof(tbuf); i++) {
						if (fs->Read(&tbuf[i], 1) < 1) Abort();
						size--;
						if (tbuf[i]=='\0') break;
					}
				}
				img_typ = tbuf;
				//�摜�^�C�v
				BYTE pic_typ;
				if (fs->Read(&pic_typ, 1) < 1) Abort();
				size--;
				//Description �͓ǂݔ�΂�
				int nul_cnt = 0;
				for (;;) {
					char d;
					if (fs->Read(&d, 1) < 1) Abort();
					size--;
					if (d!='\0') continue;
					nul_cnt++;
					if (!is_16 && nul_cnt==1) break;
					if ( is_16 && nul_cnt==2) break;
				}
				//�C���[�W
				std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
				if (ms->CopyFrom(fs.get(), size)<size) Abort();
				ms->Seek(0, soFromBeginning);
				if (SameStr(img_typ, "image/jpeg") || SameStr(img_typ, "jpg")) {
					if (!WIC_get_from_memory(ms.get(), bmpimg, GUID_ContainerFormatJpeg)) Abort();
					ok = !bmpimg->Empty;
				}
				else if (SameStr(img_typ, "image/png") || SameStr(img_typ, "png")) {
					if (!WIC_get_from_memory(ms.get(), bmpimg, GUID_ContainerFormatPng)) Abort();
					ok = !bmpimg->Empty;
				}
				break;
			}
			else {
				fs->Seek(size, soFromCurrent);
			}
			if (fs->Position>=last_p) break;
		}
	}
	catch (...) {
		ok = false;
	}

	return ok;
}
//---------------------------------------------------------------------------
