//----------------------------------------------------------------------//
// Susie 32bit Plug-in 操作クラス										//
//																		//
//----------------------------------------------------------------------//
#ifndef SpiUnitH
#define SpiUnitH

//---------------------------------------------------------------------------
#define SPI_CHECKSIZE 2048

//---------------------------------------------------------------------------
#pragma pack(push, 1)

typedef struct pic_info {
	long   left, top;
	long   width;
	long   height;
	WORD   x_density;
	WORD   y_density;
	short  color;
	HLOCAL info;
} pic_info;

#pragma pack(pop)

//---------------------------------------------------------------------------
typedef int (CALLBACK *SPI_PROGRESS)(int, int, long);
typedef int (__stdcall *SPI_GetPluginInfo)(int infono, LPSTR buf, int buflen);
typedef int (__stdcall *SPI_IsSupported)(LPSTR filename, DWORD dw);
typedef int (__stdcall *SPI_GetPictureInfo)(LPSTR buf, long len,
					unsigned int flag, struct pic_info *lpInfo);
typedef int (__stdcall *SPI_GetPicture)(LPSTR buf, long len, unsigned int flag,
					HANDLE *pHBInfo, HANDLE *pHBm,
					SPI_PROGRESS lpPrgressCallback, long lData);

typedef int (__stdcall *SPI_ConfigurationDlg)(HWND hWnd, int function);

//---------------------------------------------------------------------------
struct spi_info {
	HMODULE hDLL;
	SPI_GetPluginInfo		GetPluginInfo;
	SPI_IsSupported			IsSupported;
	SPI_GetPictureInfo		GetPictureInfo;
	SPI_GetPicture	 		GetPicture;
	SPI_ConfigurationDlg	ConfigurationDlg;

	UnicodeString FileName;
	UnicodeString FileExt;
	UnicodeString FileType;
};

//---------------------------------------------------------------------------
//SPIリスト (TList から継承)
//---------------------------------------------------------------------------
class TSpiList : public TList {
private:
	spi_info * __fastcall Get(int Index);
	void __fastcall Put(int Index, spi_info *Item);
public:
	__fastcall TSpiList();
	__property spi_info * Items[int Index] = {read=Get, write=Put};
};

//---------------------------------------------------------------------------
class SpiUnit {
public:
	UnicodeString SpiDir;
	TSpiList *PlgList;

	SpiUnit(UnicodeString spidir);
	~SpiUnit();

	bool TestFExt(UnicodeString fext);
	bool GetImgSize(UnicodeString fnam, unsigned int *w, unsigned int *h);
	spi_info* FindPlugin(UnicodeString fnam);
	bool LoadImage(UnicodeString fnam, Graphics::TBitmap *bmp, spi_info *sp = NULL);
};
//---------------------------------------------------------------------------
#endif
