//----------------------------------------------------------------------//
//	バージョン情報フォーム												//
//																		//
//----------------------------------------------------------------------//
#ifndef AboutH
#define AboutH
//----------------------------------------------------------------------------
#include <Winapi.Windows.hpp>
#include <System.hpp>
#include <System.Classes.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Graphics.hpp>

//----------------------------------------------------------------------------
class TAboutBox : public TForm
{
__published:
	TButton *OKButton;
	TImage *Image1;
	TLabel *Comments;
	TLabel *ProductName;
	TLabel *Version;
	TPanel *Panel1;
    void __fastcall FormShow(TObject *Sender);
private:

public:
	virtual __fastcall TAboutBox(TComponent *Owner);
};
//----------------------------------------------------------------------------
extern TAboutBox *AboutBox;
//----------------------------------------------------------------------------
#endif
