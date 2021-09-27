//----------------------------------------------------------------------//
//	ƒo[ƒWƒ‡ƒ“î•ñ														//
//																		//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "EnvData.h"
#include "about.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;

//---------------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent *Owner) : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormShow(TObject *Sender)
{
	Version->Caption  = "Version "+ get_VersionStr(Application->ExeName);
	Comments->Caption = "2003-2021 by Nekomimi";
}
//---------------------------------------------------------------------------

