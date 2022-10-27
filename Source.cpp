#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string>
#include <ctime>
using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Main window function
void getMyChar(int i); // The function that determines the pressed button

HINSTANCE hInst; 						// Application instance descriptor
char ClassName[] = "MainWindows"; 		
char AppTitle[] = "OVERLAPPEDWINDOW"; 	

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS MainWindows; 			
	HWND hWndMain; 					
	MSG msg; 						

	hInst = hInstance;	

	if ((hWndMain = FindWindow(ClassName, NULL)) != NULL)
	{
		if (IsIconic(hWndMain))
			ShowWindow(hWndMain, SW_RESTORE);
		SetForegroundWindow(hWndMain);
		return FALSE;
	}

	memset(&MainWindows, 0, sizeof(MainWindows));
	MainWindows.lpszClassName = ClassName;								
	MainWindows.lpfnWndProc = (WNDPROC)WndProc;							
	MainWindows.style = CS_HREDRAW | CS_VREDRAW;						
	MainWindows.hInstance = hInstance;									
	MainWindows.hIcon = LoadIcon(NULL, IDI_APPLICATION);				
	MainWindows.hCursor = LoadCursor(NULL, IDC_ARROW);					
	MainWindows.hbrBackground = CreateSolidBrush(RGB (150, 220, 200));	
	MainWindows.lpszMenuName = NULL;									
	MainWindows.cbClsExtra = 0;											
	MainWindows.cbWndExtra = 0;											
	RegisterClass(&MainWindows);										

	hWndMain = CreateWindow(ClassName, AppTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	if (!hWndMain)
	{
		MessageBox(NULL, "Create: error", AppTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain); 

	// Message queue processing cycle
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

string left = "", right = ""; // left - text to the left of the cursor, right - text to the right of the cursor

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_CREATE: { } break;

	case WM_MOUSEMOVE:
	{
		
		UINT fwKeys = wParam;			
		int xPos = LOWORD(lParam);		
		int yPos = HIWORD(lParam);		

		if (fwKeys & MK_LBUTTON) 
		{
			HDC hDC = GetDC(hWnd); 
			SetPixel(hDC, xPos, yPos, 0l); 
			ReleaseDC(hWnd, hDC);
		}
	}; break;

	case WM_LBUTTONDOWN: { } break;

	case WM_RBUTTONDOWN: {
		RECT r; GetClientRect(hWnd, &r);
		InvalidateRect(hWnd, &r, true);
	} break;

	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_LEFT:
		{	
			// LEFT ARROW
			if (left.size() > 0)
			{
				char tmp = left[left.size() - 1];
				right = tmp + right;
				string t = "";
				for (int i = 0; i < left.size() - 1; i++)
					t += left[i];
				left = t;
			}
		} break;

		case VK_RIGHT:
		{	
			// RIGHT ARROW
			if (right.size() > 0)
			{
				left += right[0];
				string t = "";
				for (int i = 1; i < right.size(); i++)
					t += right[i];
				right = t;
			}

		} break;

		case VK_HOME:
		{	// HOME
			right = left + right;
			left = "";
		} break;

		case VK_END:
		{	// END
			left = left + right;
			right = "";
		} break;

		case VK_DELETE:
		{	// DEL 
			if (right.size() > 0)
			{
				string tmp = "";
				for (int i = 1; i < right.size(); i++)
					tmp += right[i];
				right = tmp;
			}
		} break;
		} 

		RECT r; GetClientRect(hWnd, &r);
		InvalidateRect(hWnd, &r, true);
	} break;
	
	case WM_CHAR: {
		getMyChar(wParam);
		RECT r; GetClientRect(hWnd, &r);
		InvalidateRect(hWnd, &r, true);
	} break;

	case WM_COMMAND: {
	} break;

	case WM_PAINT:	
	{
		HDC hDC;
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		
		string myText = left + right;
		
		RECT windowSize;
		GetClientRect(hWnd, &windowSize);

		windowSize.left = windowSize.left + 10;
		windowSize.top = windowSize.top + 10;
		SetBkColor(hDC, RGB(150, 220, 200));
		DrawText(hDC, myText.c_str(), myText.size(), &windowSize, DT_LEFT | DT_CALCRECT);
		DrawText(hDC, myText.c_str(), myText.size(), &windowSize, DT_LEFT);
		EndPaint(hWnd, &ps);	
	}; break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}; break;

	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0l;
}
void getMyChar(int i)
{
	switch (i)
	{
	case 0x08:
	{	// BACKSPACE
		if (left.size() > 0)
		{
			string tmp = "";
			for (int i = 0; i < left.size() - 1; i++)
				tmp += left[i];
			left = tmp;
		}
	} break;

	case 0x0A:
	{
		left += "\n";
	} break;

	case 0x09:
	{
		left += "\t";
	} break;

	case 0x0D:
	{
		left += "\r";
	} break;
	case 0x1B: // Esc
	{
		left = "";
		right = "";
	} break;

	default:
	{ 
		left += char(i);
	} break;
	}
}