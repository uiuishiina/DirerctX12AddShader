#pragma once
#include"LibrarySummary.h"

struct RSPreparation {
	RSPreparation() {
        //---  CBV設定  ---
        DRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        DRange_[0].NumDescriptors = 1;
        DRange_[0].BaseShaderRegister = 0;
        DRange_[0].RegisterSpace = 0;
        DRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        DRange_[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        DRange_[1].NumDescriptors = 1;
        DRange_[1].BaseShaderRegister = 1;
        DRange_[1].RegisterSpace = 0;
        DRange_[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        //---  ルートパラメータ設定  ---
        RParameter_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        RParameter_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        RParameter_[0].DescriptorTable.NumDescriptorRanges = 1;
        RParameter_[0].DescriptorTable.pDescriptorRanges = &DRange_[0];
        RParameter_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        RParameter_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        RParameter_[1].DescriptorTable.NumDescriptorRanges = 1;
        RParameter_[1].DescriptorTable.pDescriptorRanges = &DRange_[1];

        //---  ルートシグネチャ設定  ---
        RSDesc_.pParameters = RParameter_;
        RSDesc_.NumParameters = 2;
        RSDesc_.pStaticSamplers = nullptr;
        RSDesc_.NumStaticSamplers = 0;
        RSDesc_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    }

    D3D12_DESCRIPTOR_RANGE    DRange_[2]{};
    D3D12_ROOT_PARAMETER      RParameter_[2]{};
    D3D12_ROOT_SIGNATURE_DESC RSDesc_{};
};

class RootSignature final
{
public:
	RootSignature() = default;
	~RootSignature() = default;

    //---  ルートシグネチャーテンプレート作成関数  ---
    template <class T>
    [[nodiscard]] bool CreateTemplate() noexcept{
        T instance;
        return Create(instance.RSDesc_);
    }

	//---  ルートシグネチャ作成関数  ---
	[[nodiscard]] bool Create(const D3D12_ROOT_SIGNATURE_DESC& Desc)noexcept;

	//---  ルートシグネチャ取得関数  ---
	[[nodiscard]] ID3D12RootSignature* Get()const noexcept;

	//---  ルートシグネチャー
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature_{};
};