#pragma once
#include"LibrarySummary.h"

class SwapChain final
{
public:
	SwapChain() = default;
	~SwapChain() = default;

	//---  スワップチェーン作成関数  ---
	[[nodiscard]] bool Create(ID3D12CommandQueue* commandQueue, const UINT index = 2)noexcept;

	//---  スワップチェーン取得関数  ---
	[[nodiscard]] IDXGISwapChain3* Get()const noexcept;

	//---  スワップチェーンハンドル取得関数  ---
	[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& GetDesc()const noexcept;
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain3>	SwapChain_{};
	DXGI_SWAP_CHAIN_DESC1					Desc_{};
};