#pragma once
#include"LibrarySummary.h"

class CommandAllocator final
{
public:
	CommandAllocator() = default;
	~CommandAllocator() = default;

	//---  コマンドアロケーター作成関数  ---
	[[nodiscard]] bool Create(UINT index, const D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT)noexcept;

	//---  コマンドアロケーター取得関数  ---
	[[nodiscard]] ID3D12CommandAllocator* Get(UINT index)const noexcept;

	//---  コマンドリストタイプ取得関数  ---
	[[nodiscard]] const D3D12_COMMAND_LIST_TYPE GetType()const noexcept;

	//---  コマンドアロケーターリセット関数  ---
	void Reset(UINT index)noexcept;
private:
	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> Allocator_{};
	D3D12_COMMAND_LIST_TYPE Type_{};
};