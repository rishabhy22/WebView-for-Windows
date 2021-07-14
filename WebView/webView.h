
#pragma comment(lib, "dwmapi.lib")


#include <stdlib.h>
#include <string.h>
#include <string>
#include <tchar.h>

#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"
#include "dwmapi.h"
#include "windowsx.h"


#define LEFTEXTENDWIDTH 1
#define RIGHTEXTENDWIDTH 1
#define BOTTOMEXTENDWIDTH 1
#define TOPEXTENDWIDTH 60
#define RESIZE_BUFFER 8

#define DEF_ROOTWND_WIDTH 1068
#define DEF_ROOTWND_HEIGHT 600



#define WEB_VIEW_BUTTONS_CLASS _T("WebViewButtonClass")
#define ROOT_WND_CLASS _T("RootWindow")




class WebView {

private:
    static HINSTANCE hInstance;
    static HWND rootHWND;
    static HWND homeButtonHWND;
    static HWND backButtonHWND;
    static HWND goButtonHWND;
    static HWND txtFieldHWND;
    static HWND closeWndHWND;
    static HWND minWndHWND;
    static wil::com_ptr<ICoreWebView2Controller> webViewController;
    static wil::com_ptr<ICoreWebView2> webViewWindow;

    static int pxHeight;
    static int pxWidth;
    static int rootWndWidth;
    static int rootWndHeight;
    static int frameTopHeight;
    

protected:

    int registerWindowClasses();
    void createAndShowRootWindow();
    static void createRootLayout(HWND);
    static void CustomPaint(HWND);
    static void MoveAllControls();
    static HRESULT createAndShowWebView();

    static LRESULT CALLBACK RootWndProc(HWND const window,
        UINT const message,
        WPARAM const wParam,
        LPARAM const lParam);
    static LRESULT CALLBACK ButtonWndProc(HWND const window,
        UINT const message,
        WPARAM const wParam,
        LPARAM const lParam);


public:

    WebView();

    ~WebView();

    WebView(HINSTANCE,int,int);



};