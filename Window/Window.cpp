#include"Window.h"
#include<cassert>

//---  コールバック  ---
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//---  ウィンドウ作成関数  ---
[[nodiscard]] bool Window::Create(HINSTANCE hInstance,const UINT w,const UINT h,const std::string_view& WindowName_)noexcept {

    //---   ウィンドウ作成  ---
    // ウィンドウの定義
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WindowName_.data();
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    // ウィンドウクラスの登録
    RegisterClass(&wc);

    // ウィンドウの作成
    Handle_ = CreateWindow(wc.lpszClassName, wc.lpszClassName,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h,
        nullptr, nullptr, hInstance, nullptr);
    // ウィンドウ確認
    assert(Handle_ && "Window Creation Failed.");

    //---  ウィンドウ作成後処理  ---
    // ウインドウの表示
    ShowWindow(Handle_, SW_SHOW);
    // ウィンドウを更新
    UpdateWindow(Handle_);
    // ウィンドウのサイズを保存
    WindowSize_ = { w,h };

    return true;
}

//---  ウィンドウハンドル作成関数  ---
[[nodiscard]] HWND Window::GetHandle()const noexcept {
    assert(Handle_ && "Window not created.(call GetHandle())");
    return Handle_;
}

//---  ウィンドウサイズ取得関数  ---
[[nodiscard]] const std::pair<UINT, UINT> Window::GetSize()const noexcept {
    assert(Handle_ && "Window not created.(call GetSize())");
    return WindowSize_;
}

//---  メッセージループ継続判定処理関数  ---
[[nodiscard]] bool Window::MessageLoop()noexcept {
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        // メッセージ処理
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}