#include"CommandAllocator.h"
#include"Device.h"
#include<cassert>

//---  コマンドアロケーター作成関数  ---
[[nodiscard]] bool CommandAllocator::Create(UINT index,const D3D12_COMMAND_LIST_TYPE type)noexcept {

	Allocator_.resize(index);

	//コマンドアロケーター作成
	for (UINT i = 0; i < index; i++) {
		const auto hr = Device::Instance().GetDevice()->CreateCommandAllocator(type, IID_PPV_ARGS(&Allocator_[i]));
		if (FAILED(hr)) {
			assert(false && "CommandAllocator Creation Failed.");
			return false;
		}
	}
	Type_ = type;
	return true;
}

//---  コマンドアロケーター取得関数  ---
[[nodiscard]] ID3D12CommandAllocator* CommandAllocator::Get(UINT index)const noexcept {
	assert(index < Allocator_.size() && "Allocator_ out of Lange.(call Get())");
	assert(Allocator_[index] && "Allocator_ not Created.(call Get())");
	return Allocator_[index].Get();
}

//---  コマンドリストタイプ取得関数  ---
[[nodiscard]] const D3D12_COMMAND_LIST_TYPE CommandAllocator::GetType()const noexcept {
	assert(Allocator_[0] && "Allocator_");
	return Type_;
}

//---  コマンドアロケーターリセット関数  ---
void CommandAllocator::Reset(UINT index)noexcept {
	assert(index < Allocator_.size() && "Allocator_ out of Lange.(call Reset())");
	assert(Allocator_[index] && "Allocator_ not Created.(call Reset())");
	Allocator_[index]->Reset();
}