//----------------------------------------------------------------------//
// Shell �֘A															//
// �Œ���̃h���b�v�����̂�												//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "usr_shell.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
HWND TargetHandle = NULL;
int  DroppedMode  = 0;				//�h���b�v�̃��[�h

TStringList *DroppedList   = NULL;	//�h���b�v���ꂽ�t�@�C�����X�g

//---------------------------------------------------------------------------
//�h���b�v�̃��[�h���擾 (�R�s�[�̂�)
//---------------------------------------------------------------------------
DWORD get_DropMode(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt)
{
	DWORD d_mode = DROPEFFECT_NONE;
	const POINT p = {pt.x, pt.y};
	HWND hWnd = ::WindowFromPoint(p);

	_TCHAR cbuf[256];
	UnicodeString cnam = (::GetClassName(hWnd, cbuf, 255)>0)? UnicodeString(cbuf) : EmptyStr;

	if (SameText(cnam, "TMDIChild") || SameText(cnam, "TIdeaFragMainForm")) {
		DroppedMode = d_mode = DROPEFFECT_COPY;
	}
	return d_mode;
}

//---------------------------------------------------------------------------
// TDropSource �N���X
//---------------------------------------------------------------------------
class TDropSource : public IDropSource
{
public:
	TDropSource() : FRefCount(0)
	{
	}

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		IUnknown *punk = NULL;
		if		(riid == IID_IUnknown)		punk = static_cast<IUnknown*>(this);
		else if (riid == IID_IDropSource)	punk = static_cast<IDropSource*>(this);

		*ppv = punk;
		if (punk) {
			punk->AddRef();
			return S_OK;
		}
		else
			return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		InterlockedIncrement(&FRefCount);
		return (ULONG)FRefCount;
	}

	ULONG __stdcall Release()
	{
		ULONG ret = (ULONG)InterlockedDecrement(&FRefCount);
		if (ret==0) delete this;
		return ret;
	}

	HRESULT __stdcall QueryContinueDrag(BOOL fEsc, DWORD grfKeyState)
	{
		//ESC�A�܂��͗����̃{�^���������ꂽ�璆�~
		if (fEsc || (grfKeyState & (MK_LBUTTON|MK_RBUTTON))==(MK_LBUTTON|MK_RBUTTON))
			return DRAGDROP_S_CANCEL;
		//�}�E�X�{�^���������ꂽ�ꍇ�̓h���b�v������
		if ((grfKeyState & (MK_LBUTTON|MK_RBUTTON))==0)
			return DRAGDROP_S_DROP;
		return S_OK;
	}

	HRESULT __stdcall GiveFeedback(DWORD dwEffect)
	{
		return DRAGDROP_S_USEDEFAULTCURSORS;
	}

private:
	LONG FRefCount;
};

//---------------------------------------------------------------------------
// TDropTarget �N���X
//---------------------------------------------------------------------------
class TDropTargetBase : public IDropTarget
{
protected:
	unsigned int refCount;

	TDropTargetBase()
	{
		refCount = 0;
	}

	~TDropTargetBase() { }

	virtual HRESULT __stdcall QueryInterface(const IID& iid, void **ppv)
	{
		if (IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IDropTarget))
			*ppv = static_cast<IDropTarget *>(this);
		else {
			*ppv = NULL;
			return E_NOINTERFACE;
		}
		AddRef();
		return S_OK;
	}

	virtual ULONG __stdcall AddRef()
	{
		refCount++;
		return refCount;
	}

	virtual ULONG __stdcall Release()
	{
		refCount--;
		if (refCount==0) {
			delete this;
			return 0;
		}
		else
			return refCount;
	}
};
//---------------------------------------------------------------------------
class TDropTarget : public TDropTargetBase
{
public:
	static void CreateInstance(IDropTarget **pp)
	{
		if (pp) {
			TDropTarget *p = new TDropTarget();
			p->QueryInterface(IID_IDropTarget, (void **)pp);
		}
	}

private:
	virtual HRESULT __stdcall DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	virtual HRESULT __stdcall DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	virtual HRESULT __stdcall DragLeave();
	virtual HRESULT __stdcall Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
};

//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	*pdwEffect = get_DropMode(pDataObj, grfKeyState, pt);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	*pdwEffect = get_DropMode(NULL, grfKeyState, pt);
	return (*pdwEffect==DROPEFFECT_NONE)? DRAGDROP_S_CANCEL : S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::DragLeave()
{
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	if (!DroppedList) return E_FAIL;
	DroppedList->Clear();

	FORMATETC formatetc;
	formatetc.cfFormat = CF_HDROP;
	formatetc.ptd	   = NULL;
	formatetc.dwAspect = DVASPECT_CONTENT;
	formatetc.lindex   = -1;
	formatetc.tymed    = TYMED_HGLOBAL;
	STGMEDIUM medium;
	HRESULT hr;

	//�h���b�v���ꂽ�t�@�C���̃��X�g���쐬
	if (pDataObj->QueryGetData(&formatetc)==S_OK) {
		hr = pDataObj->GetData(&formatetc, &medium);	if (FAILED(hr)) return hr;
		HDROP dp = (HDROP)::GlobalLock(medium.hGlobal);
		UINT f_cnt = ::DragQueryFile(dp, 0xFFFFFFFF, NULL, 0);
		UnicodeString fnam;
		for (UINT i=0; i<f_cnt; i++) {
			fnam.SetLength(MAX_PATH);
			int len = ::DragQueryFile(dp, i, fnam.c_str(),fnam.Length());
			fnam.SetLength(len);
			if (faDirectory & FileGetAttr(fnam)) fnam += "\\";
			DroppedList->Add(fnam);
		}
		::GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);
	}
	//�����N����
	else {
		//�^�C�g��
		UnicodeString fnam;
		formatetc.cfFormat = ::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW);
		HRESULT hr = pDataObj->QueryGetData(&formatetc);	if (FAILED(hr)) return hr;
		hr = pDataObj->GetData(&formatetc, &medium);		if (FAILED(hr)) return hr;
		FILEGROUPDESCRIPTOR *fg = (FILEGROUPDESCRIPTOR*)::GlobalLock(medium.hGlobal);
		if (fg->cItems>0) fnam = fg->fgd[0].cFileName;
		::GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);
		//URL
		UnicodeString url;
		formatetc.cfFormat = ::RegisterClipboardFormat(CFSTR_INETURL);
		hr = pDataObj->QueryGetData(&formatetc);		if (FAILED(hr)) return hr;
		hr = pDataObj->GetData(&formatetc, &medium);	if (FAILED(hr)) return hr;
		url = (LPTSTR)::GlobalLock(medium.hGlobal);
		::GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);

		if (!fnam.IsEmpty() && !url.IsEmpty()) {
			DroppedList->Add(UnicodeString().sprintf(_T("%s\t%s"), ChangeFileExt(fnam, EmptyStr).c_str(), url.c_str()));
			DroppedMode = DROPEFFECT_LINK;
		}
	}

	//���C���t�H�[���Ƀh���b�v������v��
	if (DroppedList && DroppedList->Count>0) ::PostMessage(TargetHandle, WM_FORM_DROPPED, 0, 0);

	return S_OK;
}

//---------------------------------------------------------------------------
//�h���b�v�����̏�����
//---------------------------------------------------------------------------
void DropInitialize(HWND hWnd)
{
	TargetHandle  = hWnd;

	::OleInitialize(NULL);

	DroppedList = new TStringList();

	//�h���b�v�^�[�Q�b�g��o�^
	IDropTarget *pDropTarget = NULL;
	TDropTarget::CreateInstance(&pDropTarget);
	if (pDropTarget) {
		RegisterDragDrop(TargetHandle, pDropTarget);
		pDropTarget->Release();
	}
}
//---------------------------------------------------------------------------
//�h���b�v�̏I��������
//---------------------------------------------------------------------------
void DropUninitialize()
{
	RevokeDragDrop(TargetHandle);

	delete DroppedList;

	::OleUninitialize();
}
//---------------------------------------------------------------------------
