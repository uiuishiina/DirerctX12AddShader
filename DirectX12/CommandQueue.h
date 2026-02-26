#pragma once
#include"LibrarySummary.h"

class CommandQueue final
{
public:
	CommandQueue() = default;
	~CommandQueue() = default;

	//---  コマンドキュー作成関数  ---
	[[nodiscard]] bool Create()noexcept;

	//---  コマンドキュー取得関数  ---
	[[nodiscard]] ID3D12CommandQueue* Get()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> Queue_{};
};