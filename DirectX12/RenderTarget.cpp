#include"RenderTarget.h"
#include"Device.h"
#include"DescriptorHeap.h"
#include<cassert>

//---  レンダーターゲット作成関数  ---
[[nodiscard]] bool RenderTarget::Create(IDXGISwapChain3* Swapchain, const UINT index)noexcept {

    RenderTarget_.resize(index);

    //ディスクリプターヒープ先頭ハンドル取得 & 保存
    Handle_ = CPUDescriptorManager::Instance().GetFristHandle(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    auto handle = Handle_;
    // バックバッファ生成
    for (uint8_t i = 0; i < index; ++i) {
        const auto hr = Swapchain->GetBuffer(i, IID_PPV_ARGS(&RenderTarget_[i]));
        if (FAILED(hr)) {
            assert(false && "BackBuffer Creation Failed.");
            return false;
        }

        Device::Instance().GetDevice()->CreateRenderTargetView(RenderTarget_[i].Get(), nullptr, handle);

        //次のハンドルへ移動
        handle.ptr += Device::Instance().GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    return true;
}

//---  レンダーターゲットハンドル取得関数(描画データ用)  ---
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetHandle(const UINT index)noexcept {
    assert(index < RenderTarget_.size() && RenderTarget_[index] && "RenderTarget_ not Created.(call GetHandle())");

    auto handle = Handle_;
    handle.ptr += (Device::Instance().GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)) * index;
    return handle;
}

//---  レンダーターゲット取得関数(リソースバリア用)  ---
[[nodiscard]] ID3D12Resource* RenderTarget::Get(const UINT index)const noexcept {
    assert(index < RenderTarget_.size() && RenderTarget_[index] && "RenderTarget_ not Created.(call GetHandle())");
    return RenderTarget_[index].Get();
}