#pragma once
#include"LibrarySummary.h"

class DescriptorHeap;

//---  CPU用ディスクリプタヒープマネージャー  ---
class CPUDescriptorManager final
{
public:
	//---  CPU用ディスクリプタヒープマネージャークラス取得関数  ---
	static CPUDescriptorManager& Instance()noexcept {
		static CPUDescriptorManager instance_;
		return instance_;
	}
	//---  CPU用ディスクリプタヒープ作成指示関数  ---
	[[nodiscard]] bool CreateCall(const D3D12_DESCRIPTOR_HEAP_TYPE type, UINT index)noexcept;

	//---  CPU用ディスクリプタヒープインデックス確保指示関数  ---
	[[nodiscard]] std::optional<UINT> AllocateHeapCall(const D3D12_DESCRIPTOR_HEAP_TYPE type)noexcept;

	//---  CPU用ディスクリプタヒープ先頭ハンドル取得関数  ---
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetFristHandle(const D3D12_DESCRIPTOR_HEAP_TYPE type)noexcept;

	//---  CPU用ディスクリプタヒープ返還登録関数  ---
	void DescriptorReturnIndex(const D3D12_DESCRIPTOR_HEAP_TYPE type, const UINT index)noexcept;

	//---  CPU用ディスクリプタヒープ返還関数  ---
	void DescriptorReturn()noexcept;
private:
	CPUDescriptorManager();
	~CPUDescriptorManager();

	std::unordered_map<D3D12_DESCRIPTOR_HEAP_TYPE, std::unique_ptr<DescriptorHeap>> DescriptorMap_{};
	std::multimap<D3D12_DESCRIPTOR_HEAP_TYPE, UINT> ReturnMap_{};
};

//---  GPU用ディスクリプタヒープマネージャー  ---
class GPUDescriptorManager final
{
public:
	//---  GPU用ディスクリプタヒープマネージャークラス取得関数  ---
	static GPUDescriptorManager& Instance()noexcept {
		static GPUDescriptorManager instance_;
		return instance_;
	}
	//---  GPU用ディスクリプタヒープ作成指示関数  ---
	[[nodiscard]] bool CreateCall(const UINT index,const UINT buffercount)noexcept;

	//---  GPU用ディスクリプタヒープインデックス確保指示関数  ---
	[[nodiscard]] std::optional<UINT> AllocateHeapCall(const UINT buffercount)noexcept;

	//---  GPU用ディスクリプタヒープ先頭ハンドル取得関数  ---
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetFristHandle(const UINT buffercount)noexcept;

	//---  GPU用ディスクリプタヒープオフセットリセット関数  ---
	void OffsetReset(const UINT buffercount)noexcept;
private:
	GPUDescriptorManager();
	~GPUDescriptorManager();

	std::vector<std::unique_ptr<DescriptorHeap>> DescriptorVec_{};
	std::vector<UINT> OffsetVec_{};
	UINT MaxIndex_{};
};