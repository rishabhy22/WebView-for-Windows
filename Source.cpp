
#include <windows.h>

#include "WebView/webView.h"





int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    
    int pxWidth = GetSystemMetrics(SM_CXSCREEN);
    int pxHeight = GetSystemMetrics(SM_CYSCREEN);



   
    WebView webView(hInstance,pxWidth,pxHeight);
   
    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}
