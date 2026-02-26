#pragma once
#include"../DirectX12/LibrarySummary.h"

class Window final
{
public:
	//---  ウィンドウクラス取得関数  ---
	static Window& Instance()noexcept {
		static Window instance_;
		return instance_;
	}
	//---  ウィンドウ作成関数  ---
	[[nodiscard]] bool Create(HINSTANCE hInstance,const UINT w,const UINT h,const std::string_view& WindowName_)noexcept;

	//---  ウィンドウハンドル作成関数  ---
	[[nodiscard]] HWND GetHandle()const noexcept;

	//---  ウィンドウサイズ取得関数  ---
	[[nodiscard]] const std::pair<UINT, UINT> GetSize()const noexcept;

	//---  メッセージループ継続判定処理関数  ---
	[[nodiscard]] bool MessageLoop()noexcept;
private:
	//コンストラクタ,デストラクタ初期化
	Window() = default;
	~Window() = default;

	//コピーコンストラクタ削除
	Window(const Window& r) = delete;
	Window& operator=(const Window& r) = delete;
	//ムーブコンストラクタ削除
	Window(Window&& r) = delete;
	Window& operator=(Window&& r) = delete;

	//---  変数  ---
	HWND Handle_{};
	std::pair<UINT, UINT>	WindowSize_{};
};