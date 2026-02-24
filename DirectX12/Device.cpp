#include"Device.h"
#include<cassert>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

//---  デバイス作成関数  ---
[[nodiscard]] bool Device::Create()noexcept {

    //------  DXGI作成  ------
#if _DEBUG
    ID3D12Debug* debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
        debug->EnableDebugLayer();
    }
#endif
    //DXGIファクトリー作成
    {
        UINT createFactoryFlags = 0;
#if _DEBUG
        createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
        const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&Factory_));
        if (FAILED(hr)) {
            assert(false && "DXGIFactory Creation Failed.");
            return false;
        }
    }

    //------  DXGIアダプター作成  ------
    {
        auto           select = 0;
        IDXGIAdapter1* dxgiAdapter{};
        //アダプタ-列挙
        while (Factory_->EnumAdapters1(select, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {

            DXGI_ADAPTER_DESC1 desc{};
            dxgiAdapter->GetDesc1(&desc);

            select++;

            // ソフトウェアアダプタは除外
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                dxgiAdapter->Release();
                continue;
            }

            // Direct3D12 が動かない場合も除外
            if (FAILED(D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                dxgiAdapter->Release();
                continue;
            }

            Adapter_ = dxgiAdapter;
            break;
        }

        assert(Adapter_ && "Adapter Getting Failed.");
    }

    //------  デバイス作成  ------
    {
        // デバイス作成
        const auto hr = D3D12CreateDevice(Adapter_.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&Device_));
        if (FAILED(hr)) {
            assert(false && "Device Creation Failed.");
            return false;
        }
    }
    return true;
}

//---  デバイス取得関数  ---
[[nodiscard]] ID3D12Device* Device::GetDevice()const noexcept {
    assert(Device_ && "Device not Created.(call GetDevice())");
    return Device_.Get();
}

//---  DXGIファクトリー取得関数  ---
[[nodiscard]] IDXGIFactory4* Device::GetFactory()const noexcept {
    assert(Factory_ && "Factory_ not Created.(call GetFactory())");
    return Factory_.Get();
}

//---  DXGIアダプター取得関数  ---
[[nodiscard]] IDXGIAdapter1* Device::GetAdapter()const noexcept {
    assert(Factory_ && "Adapter not Created.(call GetAdapter())");
    return Adapter_.Get();
}