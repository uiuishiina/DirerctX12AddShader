#include"PiplineState.h"
#include"Device.h"
#include"CompileShader.h"
#include<cassert>

class PiplineState final
{
public:
	PiplineState() = default;
	~PiplineState() = default;

	//---  パイプラインステート作成関数  ---
	[[nodiscard]] bool Create(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& Desc)noexcept {
        const auto hr = Device::Instance().GetDevice()->CreateGraphicsPipelineState(&Desc, IID_PPV_ARGS(&PiplineState_));
        if (FAILED(hr)) {
            assert(false && "piplineState Creation Failed.");
            return false;
        }
        return true;
	}

	//---  パイプラインステート取得関数  ---
	[[nodiscard]] ID3D12PipelineState* Get()const noexcept {
		assert(PiplineState_ && "PiPlineState not Created.");
		return PiplineState_.Get();
	}
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> PiplineState_{};
};

//
//
//
//---  パイプラインステートマネージャー  ---
//
//
//

PSOManager::PSOManager() = default;
PSOManager::~PSOManager() = default;

//---  パイプラインステートディスク引数作成関数  ---
[[nodiscard]] bool PSOManager::CreateTemplate( const std::string tag, const std::string VS, const std::string PS, ID3D12RootSignature* root, VertexLayout Layout, const int blendType, const bool depth) noexcept {

    // デプスステートの設定
    D3D12_DEPTH_STENCIL_DESC depthStateDesc{};
    depthStateDesc.DepthEnable = depth;
    depthStateDesc.StencilEnable = false;
    depthStateDesc.DepthWriteMask = depth ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
    depthStateDesc.DepthFunc = depth ? D3D12_COMPARISON_FUNC_LESS : D3D12_COMPARISON_FUNC_NONE;

    // ブレンドステート
    D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc{};
    renderTargetBlendDesc.BlendEnable = true;
    renderTargetBlendDesc.SrcBlend = blendType == 2 ? D3D12_BLEND_SRC_ALPHA : D3D12_BLEND_ONE;
    renderTargetBlendDesc.DestBlend = blendType == 2 ? D3D12_BLEND_INV_SRC_ALPHA : blendType == 1 ? D3D12_BLEND_ONE : D3D12_BLEND_ZERO;
    renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
    renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    D3D12_BLEND_DESC blendDesc{};
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
        blendDesc.RenderTarget[i] = renderTargetBlendDesc;
    }

    //ラスタライザステート
    D3D12_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.ForcedSampleCount = 0;
    rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


    //パイプラインステート
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
    psoDesc.InputLayout = { Layout.first, static_cast<UINT>(Layout.second)};
    psoDesc.pRootSignature = root;
    psoDesc.VS = { ShaderManager::Instance().GetCall(VS)->GetBufferPointer(), ShaderManager::Instance().GetCall(VS)->GetBufferSize()};
    psoDesc.PS = { ShaderManager::Instance().GetCall(PS)->GetBufferPointer(), ShaderManager::Instance().GetCall(PS)->GetBufferSize() };
    psoDesc.RasterizerState = rasterizerDesc;
    psoDesc.BlendState = blendDesc;
    psoDesc.DepthStencilState = depthStateDesc;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    //---  パイプラインステート作成指示  ---
    return Create(psoDesc, tag);
}

//---  パイプラインステート作成指示関数  ---
[[nodiscard]] bool PSOManager::Create(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& Desc, const std::string tag)noexcept {

    //作成済みか確認
    if (piplineMap_.find(tag) != piplineMap_.end()) {
        return false;
    }
    //---  パイプラインステート作成指示  ---
    auto p = std::make_unique<PiplineState>();
    assert(p->Create(Desc) && "piplineState CreationCall Failed.");

    //---  パイプラインステートマップ登録  ---
    piplineMap_.emplace(tag, std::move(p));
    return true;
}

//---  パイプラインステート取得関数  ---
[[nodiscard]] ID3D12PipelineState* PSOManager::GetCall(const std::string tag)const noexcept {

    //作成済みか確認
    assert(piplineMap_.find(tag) != piplineMap_.end() && "piplineState not Created.");

    return piplineMap_.find(tag)->second->Get();
}