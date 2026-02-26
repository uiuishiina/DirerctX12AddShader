#pragma once
#include"LibrarySummary.h"

class RenderTarget final
{
public:
	RenderTarget() = default;
	~RenderTarget() = default;

	//---  レンダーターゲット作成関数  ---
	[[nodiscard]] bool Create(IDXGISwapChain3* Swapchain, const UINT index)noexcept;

	//---  レンダーターゲットハンドル取得関数(描画データ用)  ---
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(const UINT index)noexcept;

	//---  レンダーターゲット取得関数(リソースバリア用)  ---
	[[nodiscard]] ID3D12Resource* Get(const UINT index)const noexcept;
private:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> RenderTarget_{};
	D3D12_CPU_DESCRIPTOR_HANDLE Handle_{};
};