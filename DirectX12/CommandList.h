#pragma once
#include"LibrarySummary.h"

class CommandList final
{
public:
	CommandList() = default;
	~CommandList() = default;

	//---  コマンドリスト作成関数  ---
	[[nodiscard]] bool Create(ID3D12CommandAllocator* Allocator, const D3D12_COMMAND_LIST_TYPE type)noexcept;

	//---  コマンドリスト取得関数  ---
	[[nodiscard]] ID3D12GraphicsCommandList* Get()const noexcept;

	//---  コマンドリストリセット関数  ---
	void Reset(ID3D12CommandAllocator* Allocator, ID3D12PipelineState* pInitialState = nullptr)noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> List_{};
};