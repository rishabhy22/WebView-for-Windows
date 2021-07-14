
#include "webView.h"

#define HOTKEY_OVERLAY 1001

HINSTANCE WebView::hInstance = nullptr;
HWND WebView::rootHWND = nullptr;
HWND WebView::homeButtonHWND = nullptr;
HWND WebView::backButtonHWND = nullptr;
HWND WebView::txtFieldHWND = nullptr;
HWND WebView::goButtonHWND = nullptr;
HWND WebView::closeWndHWND = nullptr;
HWND WebView::minWndHWND = nullptr;
wil::com_ptr<ICoreWebView2Controller> WebView::webViewController = nullptr;
wil::com_ptr<ICoreWebView2> WebView::webViewWindow = nullptr;

int WebView::pxWidth = 0;
int WebView::pxHeight = 0;
int WebView::rootWndWidth = 0;
int WebView::rootWndHeight = 0;
int WebView::frameTopHeight = 0;

WebView::WebView() {

}

WebView::~WebView() {
	rootHWND = nullptr;

	UnregisterClass(ROOT_WND_CLASS, hInstance);
	UnregisterClass(WEB_VIEW_BUTTONS_CLASS, NULL);
	hInstance = nullptr;
}

WebView::WebView(HINSTANCE hInstance, int pxWidth, int pxHeight) {

	this->hInstance = hInstance;
	this->pxWidth = pxWidth;
	this->pxHeight = pxHeight;
	rootWndWidth = (int)(pxWidth * ((double)5 / 7));
	rootWndHeight = (int)(pxHeight * ((double)5 / 7));
	frameTopHeight = rootWndHeight / 12;
	registerWindowClasses();
	createAndShowRootWindow();
	createAndShowWebView();
}

int WebView::registerWindowClasses() {

	// Root Window Class
	WNDCLASS rootWndClass = { 0 };


	rootWndClass.style = CS_HREDRAW | CS_VREDRAW;
	rootWndClass.lpfnWndProc = RootWndProc;
	rootWndClass.cbClsExtra = 0;
	rootWndClass.cbWndExtra = 0;
	rootWndClass.hInstance = hInstance;
	rootWndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	rootWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	rootWndClass.hbrBackground = (HBRUSH)(BLACK_BRUSH + 1);
	rootWndClass.lpszMenuName = NULL;
	rootWndClass.lpszClassName = ROOT_WND_CLASS;



	// Button Classes
	WNDCLASS buttonClass = { 0 };

	buttonClass.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	buttonClass.lpfnWndProc = ButtonWndProc;
	buttonClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	buttonClass.hbrBackground = (HBRUSH)(BLACK_BRUSH + 1);
	buttonClass.lpszClassName = WEB_VIEW_BUTTONS_CLASS;


	if (!RegisterClass(&rootWndClass))
	{

		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("WebView"),
			NULL);

		return 1;
	}
	else
	{
		RegisterClass(&buttonClass);
		return 0;
	}
}

void WebView::createAndShowRootWindow() {
	rootHWND = CreateWindow(
		ROOT_WND_CLASS,
		L"WebView",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(int)pxWidth / 7, (int)pxHeight / 7,
		rootWndWidth, rootWndHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!rootHWND)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("WebView"),
			NULL);

	}
	RegisterHotKey(rootHWND, HOTKEY_OVERLAY, MOD_SHIFT, VK_TAB);

}


HBITMAP hBitmaps[5];


void WebView::createRootLayout(HWND hWnd) {

	std::wstring bitmapPaths[5] = { { L".\\Assets\\ytmusic.bmp" },{ L".\\Assets\\google.bmp" },{ L".\\Assets\\go.bmp" },
		{ L".\\Assets\\min.bmp" },{ L".\\Assets\\close.bmp" } };

	homeButtonHWND = CreateWindow(
		WEB_VIEW_BUTTONS_CLASS,
		L"Home",      // Button text 
		WS_VISIBLE | WS_CHILD,  // Styles 
		rootWndWidth / 100,         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		hInstance,
		NULL);

	backButtonHWND = CreateWindow(
		WEB_VIEW_BUTTONS_CLASS,
		NULL,
		WS_CHILD | WS_VISIBLE,
		rootWndWidth / 50 + rootWndWidth / 25,         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,
		hWnd,
		0,
		hInstance,
		0);

	txtFieldHWND = CreateWindowEx(
		0, L"EDIT",   // predefined class 
		NULL,         // no window title 
		WS_CHILD | WS_VISIBLE |
		ES_LEFT,
		rootWndWidth * ((double)3 / 100) + rootWndWidth * ((double)2 / 25),
		rootWndHeight / 100,
		rootWndWidth / 5,
		frameTopHeight - rootWndHeight / 50,
		hWnd,         // parent window 
		NULL,   // edit control ID 
		hInstance,
		NULL);

	goButtonHWND = CreateWindow(
		WEB_VIEW_BUTTONS_CLASS,
		NULL,
		WS_CHILD | WS_VISIBLE,
		rootWndWidth * ((double)3 / 25) + rootWndWidth / 5,         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,
		hWnd,
		0,
		hInstance,
		0);


	minWndHWND = CreateWindow(
		WEB_VIEW_BUTTONS_CLASS,
		NULL,
		WS_CHILD | WS_VISIBLE,
		rootWndWidth * ((double)90 / 100),         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,
		hWnd,
		0,
		hInstance,
		0);

	closeWndHWND = CreateWindow(
		WEB_VIEW_BUTTONS_CLASS,
		NULL,
		WS_CHILD | WS_VISIBLE,
		rootWndWidth * ((double)95 / 100),         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,
		hWnd,
		0,
		hInstance,
		0);




	for (int i = 0; i < 5; ++i)
	{
		hBitmaps[i] = (HBITMAP)LoadImage(NULL, bitmapPaths[i].c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

}

HRESULT WebView::createAndShowWebView() {


	return CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			[](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

				env->CreateCoreWebView2Controller(rootHWND, Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
					[](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
						if (controller != nullptr) {
							webViewController = controller;
							webViewController->get_CoreWebView2(&webViewWindow);
						}


						ICoreWebView2Settings* Settings;
						webViewWindow->get_Settings(&Settings);
						Settings->put_IsScriptEnabled(TRUE);
						Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
						Settings->put_IsWebMessageEnabled(TRUE);


						RECT bounds;
						GetClientRect(rootHWND, &bounds);
						bounds.top += frameTopHeight;
						bounds.bottom -= (BOTTOMEXTENDWIDTH);
						bounds.left += (LEFTEXTENDWIDTH);
						bounds.right -= (RIGHTEXTENDWIDTH);
						webViewController->put_Bounds(bounds);




						webViewWindow->Navigate(L"https://music.youtube.com/");


						return S_OK;
					}).Get());
				return S_OK;
			}).Get());
}




LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

	POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	RECT rcWindow;
	GetWindowRect(hWnd, &rcWindow);

	// Get the frame rectangle, adjusted for the style without a caption.
	RECT rcFrame = { 0 };

	AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

	// Determine if the hit test is for resizing. Default middle (1,1).
	USHORT uRow = 1;
	USHORT uCol = 1;
	bool fOnResizeBorder = false;

	if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + TOPEXTENDWIDTH)
	{
		fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
		uRow = 0;
	}
	else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - BOTTOMEXTENDWIDTH - RESIZE_BUFFER)
	{
		uRow = 2;
	}

	if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + LEFTEXTENDWIDTH + RESIZE_BUFFER)
	{
		uCol = 0; // left side
	}
	else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - RIGHTEXTENDWIDTH - RESIZE_BUFFER)
	{
		uCol = 2; // right side
	}

	LRESULT hitTests[3][3] =
	{
		{ HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
		{ HTLEFT,       HTNOWHERE,     HTRIGHT },
		{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
	};

	return hitTests[uRow][uCol];
}





void WebView::MoveAllControls()
{
	MoveWindow(homeButtonHWND,
		rootWndWidth / 100,         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,
		TRUE
	);
	MoveWindow(backButtonHWND,
		rootWndWidth / 50 + rootWndWidth / 25,         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,
		TRUE
	);

	MoveWindow(txtFieldHWND,
		rootWndWidth * ((double)3 / 100) + rootWndWidth * ((double)2 / 25),
		rootWndHeight / 100,
		rootWndWidth / 5,
		frameTopHeight - rootWndHeight / 50,
		TRUE
	);
	MoveWindow(goButtonHWND,
		rootWndWidth * ((double)3 / 25) + rootWndWidth / 5,         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,
		TRUE
	);

	MoveWindow(minWndHWND,
		rootWndWidth * ((double)90 / 100),         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,
		TRUE
	);
	MoveWindow(closeWndHWND,
		rootWndWidth * ((double)95 / 100),         // x position 
		rootWndHeight / 100,         // y position 
		rootWndWidth / 25,        // Button width
		frameTopHeight - rootWndHeight / 50,
		TRUE
	);
}


LRESULT CALLBACK WebView::RootWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	RECT bounds;
	HRESULT hr;
	HRGN hrgn;
	MARGINS margins;
	LRESULT lRet = 0;
	HDC hdc;
	HBRUSH hBrush = CreateSolidBrush(RGB(47, 49, 54));

	switch (message)
	{

	case WM_CREATE:


		RECT rcClient;
		GetWindowRect(hWnd, &rcClient);

		SetWindowPos(hWnd,
			NULL,
			rcClient.left, rcClient.top,
			rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
			SWP_FRAMECHANGED);

		createRootLayout(hWnd);
		OutputDebugString(L"WM_CREATE called\n");
		break;

	case WM_NCACTIVATE:
	{
		if ((bool)wParam == FALSE)
			return TRUE;
		break;
	}

	case WM_NCCALCSIZE:
		if ((bool)wParam == true) {
			RECT* rc = (RECT*)lParam;

			rc->left += 0;
			rc->right -= 0;
			rc->top += 0;
			rc->bottom -= 0;
		}

		break;

	case WM_HOTKEY:
	{
		if ((int)wParam == HOTKEY_OVERLAY) {
			WINDOWPLACEMENT place = {0};
			place.length = sizeof(WINDOWPLACEMENT);
			
			GetWindowPlacement(hWnd, &place);
			if (place.showCmd == SW_SHOWMINIMIZED)
				ShowWindow(hWnd, SW_RESTORE);
			else
				ShowWindow(hWnd, SW_SHOWMINIMIZED);
		}
	}

	case WM_NCHITTEST:
		if (lRet == 0)
			return HitTestNCA(hWnd, wParam, lParam);

		break;

	case WM_DESTROY:
		DeleteObject(hBrush);
		for (int i = 0; i < 5; ++i)
		{
			DeleteObject(hBitmaps[i]);
		}
		UnregisterHotKey(hWnd, HOTKEY_OVERLAY);
		PostQuitMessage(0);
		break;


	case WM_SIZING:


		if (webViewController != nullptr)
		{
			GetClientRect(hWnd, &bounds);
			rootWndHeight = bounds.bottom - bounds.top;
			rootWndWidth = bounds.right - bounds.left;
			MoveAllControls();
			bounds.top += frameTopHeight;
			bounds.bottom -= BOTTOMEXTENDWIDTH;
			bounds.left += LEFTEXTENDWIDTH;
			bounds.right -= RIGHTEXTENDWIDTH;
			webViewController->put_Bounds(bounds);
		}

		break;

	case WM_SIZE:

		if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
		{


			if (webViewController != nullptr)
			{
				GetClientRect(hWnd, &bounds);
				rootWndHeight = bounds.bottom - bounds.top;
				rootWndWidth = bounds.right - bounds.left;
				MoveAllControls();
				bounds.top += frameTopHeight;
				bounds.bottom -= BOTTOMEXTENDWIDTH;
				bounds.left += LEFTEXTENDWIDTH;
				bounds.right -= RIGHTEXTENDWIDTH;
				webViewController->put_Bounds(bounds);
			}

		}
		break;


	case WM_PAINT:
		PAINTSTRUCT ps;
		RECT rect;
		{
			GetClientRect(hWnd, &rect);

			hdc = BeginPaint(hWnd, &ps);
			hrgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.top + frameTopHeight);

			FillRgn(hdc, hrgn, hBrush);
			EndPaint(hWnd, &ps);

		}
		break;



	default:

		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void WebView::CustomPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc, hdcMem;
	RECT rect;
	HBITMAP  hOldBitmap;
	BITMAP bm;
	int bitmapIndx = -1;


	if (hwnd == homeButtonHWND) {

		bitmapIndx = 0;
	}
	else if (hwnd == backButtonHWND)
	{

		bitmapIndx = 1;
	}
	else if (hwnd == goButtonHWND)
	{

		bitmapIndx = 2;
	}
	else if (hwnd == minWndHWND)
	{

		bitmapIndx = 3;
	}
	else if (hwnd == closeWndHWND)
	{

		bitmapIndx = 4;
	}

	GetObject(hBitmaps[bitmapIndx], sizeof(BITMAP), &bm);


	hdc = BeginPaint(hwnd, &ps);
	hdcMem = CreateCompatibleDC(hdc);
	hOldBitmap = SelectBitmap(hdcMem, hBitmaps[bitmapIndx]);
	GetClientRect(hwnd, &rect);
	SetStretchBltMode(hdc, STRETCH_HALFTONE);
	StretchBlt(hdc, 0, 0, rect.right, rect.bottom,
		hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	DeleteDC(hdcMem);

	DeleteObject(hOldBitmap);
	EndPaint(hwnd, &ps);
}




LRESULT CALLBACK WebView::ButtonWndProc(HWND const window,
	UINT const message,
	WPARAM const wParam,
	LPARAM const lParam) {


	TCHAR url[1024];


	switch (message) {


	case WM_PAINT:

		CustomPaint(window);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONUP:
		if (webViewWindow != nullptr) {
			if (window == backButtonHWND)
				webViewWindow->Navigate(L"https://www.google.com/");
			if (window == homeButtonHWND)
				webViewWindow->Navigate(L"https://music.youtube.com/");
			if (window == goButtonHWND) {

				GetWindowText(txtFieldHWND, url, 1024);
				std::wstring txtStr(url);
				if (txtStr.substr(0, 5) != L"https")
				{
					txtStr = L"https://" + txtStr;
				}

				webViewWindow->Navigate(txtStr.c_str());

				SetWindowText(txtFieldHWND, NULL);

			}
			if (window == closeWndHWND) {
				DestroyWindow(rootHWND);

			}
			if (window == minWndHWND) {
				CloseWindow(rootHWND);
			}
		}

	default:
		return DefWindowProc(window, message, wParam, lParam);
	}
	return 0;
}

