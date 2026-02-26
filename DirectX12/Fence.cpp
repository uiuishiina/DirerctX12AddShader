#include"Fence.h"
#include"Device.h"
#include<cassert>

//---  フェンス作成関数  ---
[[nodiscard]] bool Fence::Create()noexcept {

	//---  フェンス作成  ---
	const auto hr = Device::Instance().GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence_));
	if (FAILED(hr)) {
		assert(false && "Fence Creation Failed.");
		return false;
	}
	//---  GPU同期用イベントハンドル作成  ---
	WaitGpuEvent_ = CreateEvent(nullptr, false, false, "WAIT_GPU");
	assert(WaitGpuEvent_ && "GPUWaitEvent Creation Failed.");

	return true;
}

//---  フェンス取得関数  ---
[[nodiscard]] ID3D12Fence* Fence::Get()const noexcept {
	assert(Fence_ && "Fence not Created.(call Get())");
	return Fence_.Get();
}

//---  GPU待機用イベント関数  ---
void Fence::WaitGPU(UINT64 value)const noexcept {
	assert(Fence_ && "Fence not Created.(call WaitGPU())");

	// フェンスの値が指定された値に達するまで待機
	if (Fence_->GetCompletedValue() < value) {
		// GPU がフェンス値に到達するまで待つ
		Fence_->SetEventOnCompletion(value, WaitGpuEvent_);
		WaitForSingleObject(WaitGpuEvent_, INFINITE);
	}
}