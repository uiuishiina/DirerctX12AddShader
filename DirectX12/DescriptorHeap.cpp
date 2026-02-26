#include"DescriptorHeap.h"
#include"Device.h"
#include<cassert>

//---  ディスクリプタヒープ管理クラス  ---
class DescriptorHeap final
{
public:
	DescriptorHeap() = default;
	~DescriptorHeap() = default;
	//---  ディスクリプタヒープ作成関数  ---
	[[nodiscard]] bool Create(const D3D12_DESCRIPTOR_HEAP_TYPE type, UINT index, bool useshader)noexcept {

		//---  ディスクリプタヒープ作成  ---
		//ディスクリプタヒープ設定
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Type = type;
		heapDesc.NumDescriptors = index;
		heapDesc.Flags = useshader ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		Type_ = type;

		//ディスクリプタヒープ作成
		HRESULT hr = Device::Instance().GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&Heap_));
		if (FAILED(hr)) {
			assert(false && "DescriptorHeap Creation Failed.");
			return false;
		}
		//---  未使用インデックス登録  ---
		for (UINT i = 0; i < index; ++i) {
			FreeIndex_.push_back(i);
		}
		return true;
	}
	//---  ディスクリプタヒープインデックス確保関数  ---
	[[nodiscard]] std::optional<UINT> AllocateHeap()noexcept {

		//---  空き確認  ---
		assert(!FreeIndex_.empty() && "FreeIndex_ didn't vacancy");

		//---  確保  ---
		const auto index = FreeIndex_.back();
		FreeIndex_.pop_back();
		return index;
	}
	//---  ディスクリプタヒープ先頭CPUハンドル取得関数  ---
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle()noexcept {
		assert(Heap_ && "Heap_ not Created.");
		return Heap_->GetCPUDescriptorHandleForHeapStart();
	}
	//---  ディスクリプタヒープ先頭GPUハンドル取得関数  ---
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle()noexcept {
		assert(Heap_ && "Heap_ not Created.");
		return Heap_->GetGPUDescriptorHandleForHeapStart();
	}
	//---  ディスクリプタヒープタイプ取得関数  ---
	[[nodiscard]] const D3D12_DESCRIPTOR_HEAP_TYPE GetType()const noexcept {
		assert(Heap_ && "Heap_ not Created.");
		return Type_;
	}
	//---  //---  CPU用ディスクリプタヒープ返還関数  ---
	void DescriptorReturn(UINT index)noexcept {
		assert(Heap_ && "Heap_ not Created.");
		FreeIndex_.push_back(index);
	}
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Heap_{};
	D3D12_DESCRIPTOR_HEAP_TYPE Type_{};
	std::vector<UINT> FreeIndex_{};
};

//
// 
// 
//------  CPU用ディスクリプタヒープマネージャー  ------
//
// 
// 
CPUDescriptorManager::CPUDescriptorManager() = default;
CPUDescriptorManager::~CPUDescriptorManager() = default;

//---  CPU用ディスクリプタヒープ作成指示関数  ---
[[nodiscard]] bool CPUDescriptorManager::CreateCall(const D3D12_DESCRIPTOR_HEAP_TYPE type, UINT index)noexcept {

	//作成済みか確認
	if (DescriptorMap_.find(type) != DescriptorMap_.end()) {
		return false;
	}
	//---  ディスクリプタヒープ作成指示  ---
	auto p = std::make_unique<DescriptorHeap>();
	assert(p->Create(type,index,false) && "DescriptorHeap CreationCall Failed()");
	
	//---  ディスクリプタヒープマップ登録  ---
	DescriptorMap_.emplace(type, std::move(p));

	return true;
}

//---  CPU用ディスクリプタヒープインデックス確保指示関数  ---
[[nodiscard]] std::optional<UINT> CPUDescriptorManager::AllocateHeapCall(const D3D12_DESCRIPTOR_HEAP_TYPE type)noexcept {

	//作成済みか確認
	assert(DescriptorMap_.find(type) != DescriptorMap_.end() && "DescriptorHeap not Created.(call AllocateHeapCall())");

	//インデックス確保
	return DescriptorMap_.find(type)->second->AllocateHeap();
}

//---  CPU用ディスクリプタヒープ先頭ハンドル取得関数  ---
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorManager::GetFristHandle(const D3D12_DESCRIPTOR_HEAP_TYPE type)noexcept {

	//作成済みか確認
	assert(DescriptorMap_.find(type) != DescriptorMap_.end() && "DescriptorHeap not Created.(call GetFristHandle())");

	//ハンドル確保
	return DescriptorMap_.find(type)->second->GetCPUHandle();
}

//---  CPU用ディスクリプタヒープ返還登録関数  ---
void CPUDescriptorManager::DescriptorReturnIndex(const D3D12_DESCRIPTOR_HEAP_TYPE type, const UINT index)noexcept {

	//作成済みか確認
	assert(DescriptorMap_.find(type) != DescriptorMap_.end() && "DescriptorHeap not Created.(call DescriptorReturnIndex())");

	//返還指示登録
	ReturnMap_.emplace(type, index);
}

//---  CPU用ディスクリプタヒープ返還関数  ---
void CPUDescriptorManager::DescriptorReturn()noexcept {

	for (auto& p : ReturnMap_) {
		DescriptorMap_.find(p.first)->second->DescriptorReturn(p.second);
	}
}

//
// 
// 
//------  GPU用ディスクリプタヒープマネージャー  ------
//
// 
// 
GPUDescriptorManager::GPUDescriptorManager() = default;
GPUDescriptorManager::~GPUDescriptorManager() = default;

//---  GPU用ディスクリプタヒープ作成指示関数  ---
[[nodiscard]] bool GPUDescriptorManager::CreateCall(const UINT index, const UINT buffercount)noexcept {

	//作成済みか確認
	assert(DescriptorVec_.empty() && "DescriptorHeap already created.");

	//初期化
	OffsetVec_.clear();

	//---  ディスクリプタヒープ作成指示  ---
	for (UINT i = 0; i < buffercount; i++) {
		auto p = std::make_unique<DescriptorHeap>();
		assert(p->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, index, false) && "DescriptorHeap CreationCall Failed()");
		DescriptorVec_.push_back(std::move(p));

		OffsetVec_.push_back(0);
	}
	MaxIndex_ = index;

	return true;
}

//---  GPU用ディスクリプタヒープインデックス確保指示関数  ---
[[nodiscard]] std::optional<UINT> GPUDescriptorManager::AllocateHeapCall(const UINT buffercount)noexcept {

	//作成済みか確認
	assert(DescriptorVec_.size() > buffercount && "DescriptorVec_ out of length.");
	assert(OffsetVec_[buffercount] > MaxIndex_ && "DescriptorVec_ out of MaxIndex_.");

	//オフセット適用
	return OffsetVec_[buffercount]++;
}

//---  GPU用ディスクリプタヒープ先頭ハンドル取得関数  ---
[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GPUDescriptorManager::GetFristHandle(const UINT buffercount)noexcept {

	//作成済みか確認
	assert(!DescriptorVec_.empty() && "DescriptorVec_ not Created.");

	return DescriptorVec_[buffercount]->GetGPUHandle();
}

//---  GPU用ディスクリプタヒープオフセットリセット関数  ---
void GPUDescriptorManager::OffsetReset(const UINT buffercount)noexcept {

	//作成済みか確認
	assert(DescriptorVec_.size() > buffercount && "DescriptorVec_ out of length.");
	OffsetVec_[buffercount] = 0;
}