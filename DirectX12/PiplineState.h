#pragma once
#include"LibrarySummary.h"

// 型エイリアス
using VertexLayout = std::pair<D3D12_INPUT_ELEMENT_DESC*, size_t>;

class PiplineState;

class PSOManager final
{
public:
	//---  パイプラインステートマネージャークラスインスタンス取得関数  ---
	static PSOManager& Instance()noexcept {
		static PSOManager instance_;
		return instance_;
	}

	//---  パイプラインステートディスク引数作成関数  ---
	[[nodiscard]] bool CreateTemplate( const std::string tag, const std::string VS, const std::string PS, ID3D12RootSignature* root, VertexLayout Layout, const int blendType, const bool depth) noexcept;

	//---  パイプラインステート作成指示関数  ---
	[[nodiscard]] bool Create(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& Desc,const std::string tag)noexcept;

	//---  パイプラインステート取得関数  ---
	[[nodiscard]] ID3D12PipelineState* GetCall(const std::string tag)const noexcept;
private:
	PSOManager();
	~PSOManager();

	std::unordered_map<std::string, std::unique_ptr<PiplineState>> piplineMap_{};
};