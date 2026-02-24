#include"../DirectX12/LibrarySummary.h"
#include"ReferenceSummary.h"
#include<cassert>

namespace AppInfo {

	//ウィンドウサイズ

}

class Application final
{
public:
	Application() = default;
	~Application() = default;
	//---  アプリケーション作成  ---
	[[nodiscard]] bool Create(HINSTANCE hInstance)noexcept {

		//ウィンドウ作成
		assert(Window::Instance().Create(hInstance, 1280, 720, "DirectX12Addshader") && "ウィンドウ作成失敗");
		//デバイス作成
		assert(Device::Instance().Create() && "デバイス作成失敗");

		return true;
	}

	void AppLoop()noexcept {
		while (Window::Instance().MessageLoop())
		{

		}
	}
private:
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Application App;
	//アプリ作成
	assert(App.Create(hInstance) && "アプリケーション作成失敗");
	//ループ開始
	App.AppLoop();

	return 0;
}