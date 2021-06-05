#include <windows.h>

#define beep 1
#define exit_win 2

HMENU hMenu;

//This function adds menus to the window
void AddMenus(HWND hwnd) {
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hHelpMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hSubMenu, MF_STRING, beep, "Text File");
	//AppendMenu(hSubMenu, MF_STRING, 1, "C File");

	AppendMenu(hFileMenu, MF_POPUP, (UINT)hSubMenu, "New");
	AppendMenu(hFileMenu, MF_STRING, beep, "Open");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, exit_win, "Exit");

	AppendMenu(hHelpMenu, MF_STRING, beep, "About Notepad");

	AppendMenu(hMenu, MF_POPUP, (UINT)hFileMenu, "File");
	AppendMenu(hMenu, MF_POPUP, (UINT)hHelpMenu, "Help");
	SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd) {
    //CreateWindowW(L"Static", L"Enter the text here:-", WS_VISIBLE | WS_CHILD, 0, 10, 100, 15, hwnd, NULL, NULL, NULL);
    //CreateWindowW(L"Edit", L".,.", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 1020, 682, hwnd, NULL, NULL, NULL) ;
    CreateWindowW(L"Edit", L"Enter your text here", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 620, 420, hwnd, NULL, NULL, NULL) ;
}

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_COMMAND:
			if(wParam == 1) MessageBeep(MB_OK);
			if(wParam == 2) DestroyWindow(hwnd);
			break;
		//Upon window creation
		case WM_CREATE:
			AddMenus(hwnd);
			AddControls(hwnd);
			break;
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}

		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Sample Project",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}

