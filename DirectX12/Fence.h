#pragma once
#include"LibrarySummary.h"

class Fence final
{
public:
	Fence() = default;
	~Fence() = default;

	//---  フェンス作成関数  ---
	[[nodiscard]] bool Create()noexcept;

	//---  フェンス取得関数  ---
	[[nodiscard]] ID3D12Fence* Get()const noexcept;

	//---  GPU待機用イベント関数  ---
	void WaitGPU(UINT64 value)const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12Fence> Fence_{};
	HANDLE                              WaitGpuEvent_{};
};