#include"CommandQueue.h"
#include"Device.h"
#include<cassert>

//---  コマンドキュー作成関数  ---
[[nodiscard]] bool CommandQueue::Create()noexcept {

    // コマンドキュー設定
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    // コマンドキュー作成
    const auto hr = Device::Instance().GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&Queue_));
    if (FAILED(hr)) {
        assert(false && "CommandQueue Creation Failed.");
        return false;
    }
    return true;
}

//---  コマンドキュー取得関数  ---
[[nodiscard]] ID3D12CommandQueue* CommandQueue::Get()const noexcept {
    assert(Queue_ && "Queue_ not Created.");
    return Queue_.Get();
}