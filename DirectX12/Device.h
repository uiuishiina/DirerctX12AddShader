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

	//---  リソースバリア変更関数  ---
	D3D12_RESOURCE_BARRIER ResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = resource;
		barrier.Transition.StateBefore = from;
		barrier.Transition.StateAfter = to;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		return barrier;
	}
private:
	Device() = default;
	~Device() = default;

	Microsoft::WRL::ComPtr<ID3D12Device>	Device_{};
	Microsoft::WRL::ComPtr<IDXGIFactory4>	Factory_{};
	Microsoft::WRL::ComPtr<IDXGIAdapter1>	Adapter_{};
};