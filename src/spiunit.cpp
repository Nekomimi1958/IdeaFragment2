//----------------------------------------------------------------------//
// Susie 32bit Plug-in 操作クラス										//
//																		//
//----------------------------------------------------------------------//
#include "spiunit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TSpiList::TSpiList(): TList()
{
}
//---------------------------------------------------------------------------
spi_info * __fastcall TSpiList::Get(int Index)
{
	return (spi_info*)(TList::Get(Index));
}
//---------------------------------------------------------------------------
void __fastcall TSpiList::Put(int Index, spi_info *Item)
{
	TList::Put(Index, Item);
}

//---------------------------------------------------------------------------
SpiUnit::SpiUnit(UnicodeString spidir)
{
	PlgList = new TSpiList();
	SpiDir  = EmptyStr;

	try {
		if (spidir.IsEmpty()) Abort();
		SpiDir = IncludeTrailingPathDelimiter(spidir);
		TSearchOption opt = TSearchOption::soTopDirectoryOnly;
		TStringDynArray f_list = TDirectory::GetFiles(SpiDir, "*.SPI", opt);

		for (int i=0; i<f_list.Length; i++) {
			UnicodeString dllnam = f_list[i];
			HMODULE handle = ::LoadLibrary(dllnam.c_str());	if (!handle) continue;
			spi_info *sp = new spi_info;
			sp->hDLL		   = handle;
			sp->GetPluginInfo  = (SPI_GetPluginInfo) ::GetProcAddress(handle, "GetPluginInfo");
			sp->IsSupported    = (SPI_IsSupported)   ::GetProcAddress(handle, "IsSupported");
			sp->GetPictureInfo = (SPI_GetPictureInfo)::GetProcAddress(handle, "GetPictureInfo");
			sp->GetPicture	   = (SPI_GetPicture)    ::GetProcAddress(handle, "GetPicture");
			sp->ConfigurationDlg = (SPI_ConfigurationDlg) ::GetProcAddress(handle, "ConfigurationDlg");
			bool flag = false;
			if (sp->GetPluginInfo && sp->IsSupported && sp->GetPictureInfo && sp->GetPicture) {
				char ibuf[256];
				if ((*sp->GetPluginInfo)(0, ibuf, 5)>=4) {
					//Import Normal 以外は使用しない
					if (ibuf[2]=='I' && ibuf[3]=='N') {
						sp->FileName = ExtractFileName(dllnam);
						if ((*sp->GetPluginInfo)(2, ibuf, 256)>0)
							sp->FileExt = ibuf;
						else
							sp->FileExt = EmptyStr;
						if ((*sp->GetPluginInfo)(3, ibuf, 256)>0)
							sp->FileType = ibuf;
						else
							sp->FileType = EmptyStr;
						PlgList->Add(sp);
						flag = true;
					}
				}
			}
			if (!flag) {
				::FreeLibrary(handle);
				delete sp;
			}
		}
	}
	catch(...) {
		return;
	}
}

//---------------------------------------------------------------------------
SpiUnit::~SpiUnit()
{
	for (int i=0; i<PlgList->Count; i++) {
		::FreeLibrary(PlgList->Items[i]->hDLL);
		delete PlgList->Items[i];
	}
	delete PlgList;
}

//---------------------------------------------------------------------------
//対応拡張子かチェック
//---------------------------------------------------------------------------
bool SpiUnit::TestFExt(UnicodeString fext)
{
	if (fext.IsEmpty() || fext==".") return false;
	if (!StartsStr(".", fext)) fext.Insert(".", 1);

	bool found = false;
	for (int i=0; i<PlgList->Count; i++) {
		if (ContainsText(PlgList->Items[i]->FileExt, fext)) {
			found = true; break;
		}
	}
	return found;
}

//---------------------------------------------------------------------------
//画像のサイズを取得
//---------------------------------------------------------------------------
bool SpiUnit::GetImgSize(UnicodeString fnam, unsigned int *w, unsigned int *h)
{
	if (!FileExists(fnam) || !w || !h) return false;

	try {
		spi_info *sp = FindPlugin(fnam);
		if (!sp) Abort();

		char fstr[MAX_PATH];
		strcpy(fstr, AnsiString(fnam).c_str());
		pic_info pinf;
		if (sp->GetPictureInfo((LPSTR)fstr, 0, 0, &pinf)!=0) Abort();
		*w = pinf.width;
		*h = pinf.height;
		return true;
	}
	catch(...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//対応するプラグインを探す
//---------------------------------------------------------------------------
spi_info* SpiUnit::FindPlugin(UnicodeString fnam)
{
	if (!FileExists(fnam)) return NULL;
	if (PlgList->Count==0) return NULL;

	try {
		char fstr[MAX_PATH];
		strcpy(fstr, AnsiString(fnam).c_str());

		//ファイルの先頭2KBを読み込む
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyWrite));
		std::unique_ptr<char[]> fbuf(new char[SPI_CHECKSIZE]);
		::ZeroMemory(fbuf.get(), SPI_CHECKSIZE);
		DWORD dw = (DWORD)(fbuf.get());
		spi_info *res = NULL;
		if (fs->Read(fbuf.get(), SPI_CHECKSIZE)>0) {
			//プラグインを探す
			for (int i=0; i<PlgList->Count; i++) {
				spi_info *sp = PlgList->Items[i];
				if ((*sp->IsSupported)((LPSTR)fstr, dw) != 0) {
					res = sp; break;
				}
			}
		}
		return res;
	}
	catch(...) {
		return NULL;
	}
}

//---------------------------------------------------------------------------
//画像をビットマップに読み込む
//---------------------------------------------------------------------------
bool SpiUnit::LoadImage(UnicodeString fnam, Graphics::TBitmap *bmp, spi_info *sp)
{
	if (PlgList->Count==0) return false;

	try {
		//対応しているプラグインを探す
		if (!sp) sp = FindPlugin(fnam);
		if (!sp) Abort();

		char fstr[MAX_PATH];
		strcpy(fstr, AnsiString(fnam).c_str());

		HANDLE bmp_inf, bmp_dat;
		if ((*sp->GetPicture)((LPSTR)fstr, 0, 0, &bmp_inf, &bmp_dat, NULL, 0)!=0) Abort();

		BITMAPINFO *p_inf = (BITMAPINFO*)::LocalLock(bmp_inf);
		VOID       *p_dat = ::LocalLock(bmp_dat);
		bmp->SetSize(p_inf->bmiHeader.biWidth, p_inf->bmiHeader.biHeight);
		int res = ::SetDIBits(bmp->Canvas->Handle, bmp->Handle,
					0, p_inf->bmiHeader.biHeight, p_dat, p_inf, DIB_RGB_COLORS);

		::LocalUnlock(bmp_inf);
		::LocalUnlock(bmp_dat);
		::LocalFree(bmp_inf);
		::LocalFree(bmp_dat);
		return (res>0);
	}
	catch(...) {
		return false;
	}
}
//---------------------------------------------------------------------------
