#include <windows.h>
#include <stdio.h>

#define help_msg 1
#define exit_win 2
#define open_f 3
#define new_txt 4
#define save_f 5

HWND edit;
HMENU hMenu;
HINSTANCE hInst;
int h, w;

//This function adds menus to the window
void AddMenus(HWND hwnd) {
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hHelpMenu = CreateMenu();

	AppendMenu(hFileMenu, MF_STRING, new_txt, "New");
	AppendMenu(hFileMenu, MF_STRING, save_f, "Save");
	AppendMenu(hFileMenu, MF_STRING, open_f, "Open");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, exit_win, "Exit");

	AppendMenu(hHelpMenu, MF_STRING, help_msg, "About this Text Editor");

	AppendMenu(hMenu, MF_POPUP, (UINT)hFileMenu, "File");
	AppendMenu(hMenu, MF_POPUP, (UINT)hHelpMenu, "Help");
	SetMenu(hwnd, hMenu);
}

void save_file(char* path) {
    FILE *file;
    file = fopen(path, "w");

    int fsize = GetWindowTextLength(edit);
    char *text = new char[fsize+1];
    GetWindowText(edit, text, fsize+1);

    fwrite(text, fsize, 1, file);
    fclose(file);
}

void save_choose(HWND hwnd) {
    int n = MessageBox(hwnd, "Note that while you save this file, any other file with the same name and extension as this file in the same directory will be replaced.", "Alert", MB_OKCANCEL | MB_ICONEXCLAMATION);
    if(n == IDOK) {
        OPENFILENAME sv;
        char filename[50];
        ZeroMemory(&sv, sizeof(OPENFILENAME));

        sv.lStructSize = sizeof(OPENFILENAME);
        sv.hwndOwner = hwnd;
        sv.lpstrFile = filename;
        sv.lpstrFile[0] = '\0';
        sv.nMaxFile = 50;
        sv.lpstrFilter = "All Files\0*.*\0Text Files\0*.txt\0";
        sv.nFilterIndex = 1;

        GetSaveFileName(&sv);
        save_file(sv.lpstrFile);
    }
}

void open_file(char *path) {
    FILE *file;
    file = fopen(path, "rb");
    if(file) {
        fseek(file, 0, SEEK_END);
        int fsize = ftell(file);
        rewind(file);
        char *text = new char[fsize+1];
        fread(text, fsize, 1, file);
        text[fsize] = '\0';
        SetWindowText(edit, text);
    }
    fclose(file);
}

void open_choose(HWND hwnd) {
    OPENFILENAME op;
    char fileName[50];
    ZeroMemory(&op, sizeof(OPENFILENAME));
    op.lStructSize = sizeof(OPENFILENAME);
    op.hwndOwner = hwnd;
    op.lpstrFile = fileName;
    op.lpstrFile[0] = '\0';
    op.nMaxFile = 50;
    op.lpstrFilter = "All Files\0*.*\0Text Files\0*.txt\0";
    op.nFilterIndex = 1;

    GetOpenFileName(&op);

    open_file(op.lpstrFile);
}

void AddControls(HWND hwnd) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    h = rect.bottom-rect.top;
    w = rect.right-rect.left;
    edit = CreateWindowW(L"Edit", L"", WS_VISIBLE | /*WS_BORDER |*/ WS_CHILD | ES_MULTILINE | WS_VSCROLL, 0, 0, w-20, h-50, hwnd, NULL, NULL, NULL) ;
}

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_COMMAND:
			//if(wParam == beep_sound) MessageBeep(MB_OK);
			if(wParam == exit_win) {
			    int n = MessageBox(hwnd, "Do you want to exit this application?", "Exit Dialog Box", MB_YESNO | MB_ICONQUESTION);
			    if(n == IDYES)
                    DestroyWindow(hwnd);
			}
			if(wParam == save_f) save_choose(hwnd);
			if(wParam == open_f) open_choose(hwnd);
			if(wParam == new_txt) SetWindowText(edit, "");
			if(wParam == help_msg) MessageBox(hwnd, "This is a basic text editor for Windows, which you can use to edit plain text, save text files and open text files", "About this Text Editor", MB_OK);
			break;
		//Upon window creation
		case WM_CREATE:
			AddMenus(hwnd);
			AddControls(hwnd);

		case WM_SIZE:
		//To resize child window when parent window is resized
		    RECT rect;
            GetWindowRect(hwnd, &rect);
            h = rect.bottom-rect.top;
            w = rect.right-rect.left;
            SetWindowPos(edit, hwnd, 0, 0, w-20, h-50, SWP_NOZORDER);
            break;

        case WM_CLOSE: {
            int n = MessageBox(hwnd, "Do you want to exit this application?", "Exit Dialog Box", MB_YESNO | MB_ICONQUESTION);
            if(n == IDYES)
                DestroyWindow(hwnd);
            break;
        }

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
	wc.hInstance	 = hInst = hInstance;
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

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Notepad",WS_VISIBLE | WS_OVERLAPPEDWINDOW,
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
