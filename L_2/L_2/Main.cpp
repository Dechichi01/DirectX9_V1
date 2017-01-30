// Cap_1.cpp : Defines the entry point for the application.
//
#include "Main.h"
#include "GameApp.h"

//Global variables
CGameApp g_App;

//Application entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	int retCode;

	// Initialise the engine.
	if (!g_App.InitInstance(hInstance, (LPTSTR)lpCmdLine, iCmdShow)) return 0;

	// Begin the gameplay process. Will return when app due to exit.
	retCode = g_App.BeginGame();

	// Shut down the engine, just to be polite, before exiting.
	if (!g_App.ShutDown())  MessageBox(0, _T("Failed to shut system down correctly, please check file named 'debug.txt'.\r\n\r\nIf the problem persists, please contact technical support."), _T("Non-Fatal Error"), MB_OK | MB_ICONEXCLAMATION);

	// Return the correct exit code.
	return retCode;

}

