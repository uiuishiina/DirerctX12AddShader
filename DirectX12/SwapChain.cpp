#include"SwapChain.h"
#include"../Window/Window.h"
#include"Device.h"
#include"CommandQueue.h"
#include<cassert>

//---  スワップチェーン作成関数  ---
[[nodiscard]] bool SwapChain::Create(ID3D12CommandQueue* commandQueue, const UINT index)noexcept {

    //---  スワップチェーン作成  ---
    //ウィンドウサイズ取得
    const auto [w, h] = Window::Instance().GetSize();

    Desc_.BufferCount = index;                                 // バックバッファの数（ダブルバッファ）
    Desc_.Width = w;                                           // バックバッファの横幅
    Desc_.Height = h;                                          // バックバッファの縦幅
    Desc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                 // バックバッファのフォーマット
    Desc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;       // レンダーターゲットとして使用
    Desc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;          // 毎フレーム画面更新するので描画が終わったらバッファを破棄
    Desc_.SampleDesc.Count = 1;                                // マルチサンプリングなし


    Microsoft::WRL::ComPtr<IDXGISwapChain1> tempSwapChain{};
    {
        const auto hr = Device::Instance().GetFactory()->CreateSwapChainForHwnd(
            commandQueue, Window::Instance().GetHandle(),
            &Desc_, nullptr, nullptr, tempSwapChain.GetAddressOf());
        if (FAILED(hr)) {
            assert(false && "SwapChain Creation Failed.");
            return false;
        }
    }

    //---  スワップチェーンアップグレード  ---
    {
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&SwapChain_));
        if (FAILED(hr)) {
            assert(false && "SwapChain Upgrade Failed.");
            return false;
        }
    }

    return true;
}

//---  スワップチェーン取得関数  ---
[[nodiscard]] IDXGISwapChain3* SwapChain::Get()const noexcept {
    assert(SwapChain_ && "SwapChain not Created.(call Get())");
    return SwapChain_.Get();
}

//---  スワップチェーンハンドル取得関数  ---
[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& SwapChain::GetDesc()const noexcept {
    assert(SwapChain_ && "SwapChain not Created.(call GetDesc())");
    return Desc_;
}