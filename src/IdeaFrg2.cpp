//----------------------------------------------------------------------//
//	IdeaFragment2														//
//																		//
//----------------------------------------------------------------------//
USEFORM("OptDlg.cpp", OptionDlg);
USEFORM("OptIdDlg.cpp", OptIndDlg);
USEFORM("PropDlg.cpp", FrgPropDlg);
USEFORM("Main.cpp", IdeaFragMainForm);
USEFORM("MapView.cpp", MapViewer);
USEFORM("RubrLay.cpp", RubLayDlg);
USEFORM("Stretch.cpp", StretchDlg);
USEFORM("FindEx.cpp", FindExForm);
USEFORM("FindDlg.cpp", FindSetDlg);
USEFORM("about.cpp", AboutBox);
USEFORM("ChildWin.cpp", MDIChild);
USEFORM("FrgGrid.cpp", FragGrid);

//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR lpCmdLine, int)
{
	//ミューテックスにより二重起動を判定
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("IdeaFragment2_is_running"));
	if (::GetLastError()==ERROR_ALREADY_EXISTS) {
		HWND hPrevWnd = ::FindWindow(_T("TIdeaFragMainForm"), NULL);
		if (hPrevWnd) {
			//既存の IdeaFragment2 をアクティブに
			HWND hApp	  = ::GetWindow(hPrevWnd, GW_OWNER);
			HWND hPrevPop = ::GetLastActivePopup(hApp);
			if (::IsIconic(hApp)) ::SendMessage(hApp, WM_SYSCOMMAND, SC_RESTORE, 0);
			::SetForegroundWindow(hPrevPop);
			//コマンドラインの内容を送る
			COPYDATASTRUCT cd;
			cd.dwData = 1;
			cd.cbData = sizeof(_TCHAR) * (lstrlen(lpCmdLine) + 1);
			cd.lpData = lpCmdLine;
			::SendMessage(hPrevWnd, WM_COPYDATA, 0, (LPARAM)&cd);
		}
		::CloseHandle(hMutex);
		return -1;
	}

	::CoInitialize(NULL);

	try {
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "IdeaFragment2";
		Application->CreateForm(__classid(TIdeaFragMainForm), &IdeaFragMainForm);
		Application->CreateForm(__classid(TMapViewer), &MapViewer);
		Application->CreateForm(__classid(TFrgPropDlg), &FrgPropDlg);
		Application->CreateForm(__classid(TFindSetDlg), &FindSetDlg);
		Application->CreateForm(__classid(TStretchDlg), &StretchDlg);
		Application->CreateForm(__classid(TOptionDlg), &OptionDlg);
		Application->CreateForm(__classid(TOptIndDlg), &OptIndDlg);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TFragGrid), &FragGrid);
		Application->CreateForm(__classid(TFindExForm), &FindExForm);
		Application->CreateForm(__classid(TRubLayDlg), &RubLayDlg);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TFindSetDlg), &FindSetDlg);
		Application->CreateForm(__classid(TFindExForm), &FindExForm);
		Application->CreateForm(__classid(TFragGrid), &FragGrid);
		Application->CreateForm(__classid(TMapViewer), &MapViewer);
		Application->CreateForm(__classid(TOptionDlg), &OptionDlg);
		Application->CreateForm(__classid(TOptIndDlg), &OptIndDlg);
		Application->CreateForm(__classid(TFrgPropDlg), &FrgPropDlg);
		Application->CreateForm(__classid(TRubLayDlg), &RubLayDlg);
		Application->CreateForm(__classid(TStretchDlg), &StretchDlg);
		Application->Run();
	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (...) {
		try {
			throw Exception("");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}

	::CoUninitialize();
	::CloseHandle(hMutex);
	return 0;
}
//---------------------------------------------------------------------
