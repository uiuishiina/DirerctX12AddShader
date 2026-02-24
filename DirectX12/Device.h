#pragma once
#include"LibrarySummary.h"

class Device final
{
public:
	//---  デバイスクラス取得関数  ---
	static Device& Instance()noexcept {
		static Device instance_;
		return instance_;
	}
	//---  デバイス作成関数  ---
	[[nodiscard]] bool Create()noexcept;
	//---  デバイス取得関数  ---
	[[nodiscard]] ID3D12Device* GetDevice()const noexcept;
	//---  DXGIファクトリー取得関数  ---
	[[nodiscard]] IDXGIFactory4* GetFactory()const noexcept;
	//---  DXGIアダプター取得関数  ---
	[[nodiscard]] IDXGIAdapter1* GetAdapter()const noexcept;
private:
	Device() = default;
	~Device() = default;

	Microsoft::WRL::ComPtr<ID3D12Device>	Device_{};
	Microsoft::WRL::ComPtr<IDXGIFactory4>	Factory_{};
	Microsoft::WRL::ComPtr<IDXGIAdapter1>	Adapter_{};
};