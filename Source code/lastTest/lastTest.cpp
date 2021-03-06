// lastTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "lastTest.h"
#include "RibbonFramework.h"
#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")
#include "RibbonIDs.h"
#include <vector>
#include <windowsx.h>
#include <gdiplusinit.h>
#include <commdlg.h> 
#include <process.h>
#include <strsafe.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")


#define MAX_LOADSTRING 100

using namespace std;

// Global Variables:
HINSTANCE g_hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

												//tọa độ điểm bắt đầu
int firstX;
int firstY;
//Tọa độ điểm kết thúc
int lastX;
int lastY;

Graphics *grp;

//vecto chứa các đối tượng đã vẽ
vector<CShape*> Shapes;
CShape* curShape;

HWND g_hwnd;

int selectedShape;// kiểu vẽ đang được lựa chọn
CStyle StyleForSelection; // style của bút brush đang được chọn
bool isDrawing = false; // bắt đầu vẽ 
bool checkSelect = false;
bool checkDrawText = false;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		return FALSE;
	}
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LASTTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LASTTEST));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	CoUninitialize();
	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;// CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LASTTEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LASTTEST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindow(szWindowClass, L"My Ribbon", 
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	g_hwnd = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

bool change = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	
	case WM_CREATE:
		if (!InitializeFramework(hWnd))
		{
			return -1;
		}

		StyleForSelection.color = Color::Black; // màu chổi tô là đen
		StyleForSelection.WidthLine = 2; // độ rộng nét vẽ 2
		StyleForSelection.dStyle = DashStyleCustom; // nét liền

		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		HANDLE Hold;
		PAINTSTRUCT ps;
		 hdc = BeginPaint(g_hwnd, &ps);

		RECT rect;
		GetClientRect(g_hwnd, &rect);
		rect.top += GetRibbonHeight();

		// Tạo MDC tương thích với DC
		HDC hMemDC;
		hMemDC = CreateCompatibleDC(hdc);
		HBITMAP bitmap;
		bitmap = CreateCompatibleBitmap(hdc, rect.right + 100, rect.bottom);
		// Chọn đối tượng bitmap để mở rộng vùng hiển thị cho MemDC
		Hold = SelectObject(hMemDC, bitmap);
		FillRect(hMemDC, &rect, HBRUSH(GetBkColor(hMemDC)));	// Vẽ lại nền

		grp = new Graphics(hMemDC);		// Vẽ trên nền MemDC
		grp->SetSmoothingMode(SmoothingModeAntiAlias);

		for (int i = 0; i < Shapes.size(); i++)
			Shapes[i]->Draw(hMemDC, StyleForSelection);
		if (isDrawing) {
			curShape->Draw(hMemDC, StyleForSelection);
		}

		BitBlt(hdc, 0, 0, rect.right + 100, rect.bottom, hMemDC, 0, 0, SRCCOPY);

		/*SelectObject(hMemDC, Hold);
		DeleteObject(bitmap);*/
		DeleteDC(hMemDC);
		delete grp;
		
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DestroyFramework();
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN: {
		//MessageBox(NULL, L"button down", L"", MB_OK);
		if (!isDrawing) {
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			isDrawing = true;
			firstX = x;
			firstY = y;

			switch (selectedShape) {
			case LINE:
				curShape = new CLine();
				break;
			case RECTANGLE:
				if (wParam & MK_SHIFT) {
					curShape = new CSquare();
				}
				else {
					curShape = new CRectangle();
				}

				break;
			case ELLIPSE:
				if (wParam & MK_SHIFT) {
					curShape = new CCircle();
				}
				else {
					curShape = new CElippse();
				}
				break;
			case SQUARE:
				curShape = new CSquare();
				break;
			case CIRCLE:
				curShape = new CCircle();
				break;

			default:
				isDrawing = false;
			}
		}

		break;
	}
	case WM_LBUTTONUP: {
		//MessageBox(NULL, L"button up", L"", MB_OK);
		if (isDrawing) {
			Shapes.push_back(curShape);
			//WCHAR* name = new WCHAR[20];
			InvalidateRect(hWnd, NULL, FALSE);
			isDrawing = false;
		}
		break;
	}
	case WM_MOUSEMOVE: {

		if (isDrawing) {

			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			lastX = x;
			lastY = y;
			//nếu đang vẽ hình chữ nhật
			if (curShape->GetNameClass() == L"CRectangle") {
				if (wParam & MK_SHIFT) { // có nhấn phím shift 
					change = true;
					curShape = new CSquare(); // chuyển qua hình vuông
					curShape->SetData(firstX, firstY, lastX, lastY);
				}
				else {
					curShape->SetData(firstX, firstY, lastX, lastY);
					change = false;
				}
			}
			//nếu đang vẽ hình vuông
			else if (curShape->GetNameClass() == L"CSquare") {
				if (wParam & MK_SHIFT) {
					curShape = new CSquare();
					curShape->SetData(firstX, firstY, lastX, lastY);
				}
				else if (change == true) { // không nhấn shift mà change == true => vừa chuyển đổi từ rectangle qua
										   //=> chuyển về Rectangle lại ( vì đã bỏ nhấn phím shift)
					curShape = new CRectangle();
					curShape->SetData(firstX, firstY, lastX, lastY);
				}
				else {
					curShape->SetData(firstX, firstY, lastX, lastY);
					change = false;
				}
			}
			//nếu đang vẽ hình elippse mà nhấn shift thì chuyển thành hình tròn
			else if (curShape->GetNameClass() == L"CElippse") {
				if (wParam & MK_SHIFT) {
					change = true;
					curShape = new CCircle();
					curShape->SetData(firstX, firstY, lastX, lastY);
				}
				else {
					curShape = new CElippse();
					change = false;
					curShape->SetData(firstX, firstY, lastX, lastY);
				}
			}
			else if (curShape->GetNameClass() == L"CCircle") {
				if (wParam & MK_SHIFT) {
					curShape = new CCircle();
					curShape->SetData(firstX, firstY, lastX, lastY);
				}
				else if (change == true) { // không nhấn shift mà change == true => vừa chuyển đổi từ Elippse qua
										   //=> chuyển về Elippse lại ( vì đã bỏ nhấn phím shift)
					curShape = new CElippse();
					curShape->SetData(firstX, firstY, lastX, lastY);
				}
				else {
					curShape->SetData(firstX, firstY, lastX, lastY);
					change = false;
				}
			}
			else {
				curShape->SetData(firstX, firstY, lastX, lastY);
				change = false;
			}

			InvalidateRect(hWnd, NULL, FALSE);
		}

		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	//GdiplusShutdown(gdiplusToken);
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
