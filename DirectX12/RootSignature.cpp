#include"RootSignature.h"
#include"Device.h"
#include<cassert>

//---  ルートシグネチャ作成関数  ---
[[nodiscard]] bool RootSignature::Create(const D3D12_ROOT_SIGNATURE_DESC& Desc)noexcept {

    //---  ルートシグネチャーシリアライズ化  ---
	Microsoft::WRL::ComPtr<ID3DBlob> signature{};
	Microsoft::WRL::ComPtr<ID3DBlob> error{};
    {
        const auto hr = D3D12SerializeRootSignature(&Desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
        if (FAILED(hr)) {
            if (error) {
                const char* msg = static_cast<const char*>(error->GetBufferPointer());
                OutputDebugStringA(msg);
                OutputDebugStringA("\n");
            }
            assert(false && "RootSignature serializable Failed.");
            return false;
        }
    }

    //---  ルートシグネチャ作成  ---
    {
        const auto hr = Device::Instance().GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&RootSignature_));
        if (FAILED(hr)) {
            assert(false && "RootSignature Creation Failed");
            return false;
        }
    }
    return true;
}

//---  ルートシグネチャ取得関数  ---
[[nodiscard]] ID3D12RootSignature* RootSignature::Get()const noexcept {
    assert(RootSignature_ && "RootSignature_ not Created.(call Get())");
    return RootSignature_.Get();
}