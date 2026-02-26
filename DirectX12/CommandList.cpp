#include"CommandList.h"
#include"Device.h"
#include<cassert>

//---  コマンドリスト作成関数  ---
[[nodiscard]] bool CommandList::Create(ID3D12CommandAllocator* Allocator, const D3D12_COMMAND_LIST_TYPE type)noexcept {

    // コマンドリスト作成
    const auto hr = Device::Instance().GetDevice()->CreateCommandList(0, type, Allocator, nullptr, IID_PPV_ARGS(&List_));
    if (FAILED(hr)) {
        assert(false && "CommandList Creation Failed.");
        return false;
    }

    //コマンドリスト初期化
    List_->Close();
    return true;
}

//---  コマンドリスト取得関数  ---
[[nodiscard]] ID3D12GraphicsCommandList* CommandList::Get()const noexcept {
    assert(List_ && "List_ not Created.(call Get())");
    return List_.Get();
}

//---  コマンドリストリセット関数  ---
void CommandList::Reset(ID3D12CommandAllocator* Allocator, ID3D12PipelineState* pInitialState)noexcept {
    assert(List_ && "List_ not Created.(call Reset())");

    List_->Reset(Allocator, pInitialState);
}