#pragma once
#include"../DirectX12/LibrarySummary.h"

using VertexLayout = std::pair<D3D12_INPUT_ELEMENT_DESC*, size_t>;

class RenderData
{
public:
	//---  描画データ作成関数  ---
	[[nodiscard]] bool Create()noexcept;

	//---  頂点データ取得関数  ---
	[[nodiscard]] ID3D12Resource* GetVertex()const noexcept;

	//---  インデックスデータ取得関数  ---
	[[nodiscard]] ID3D12Resource* GetIndex()const noexcept;

	//---  トポロジー設定取得関数  ---
	[[nodiscard]] const D3D_PRIMITIVE_TOPOLOGY GetToporogy()const noexcept;

	//---  インデックス数取得関数  ---
	[[nodiscard]] const UINT GetIndexCount()const noexcept;

	//---  バッファサイズ取得関数  ---
	[[nodiscard]] std::pair<unsigned long long, unsigned long long> GetBuffersize()const noexcept {
		return { VertexBuffersize_,IndexBuffersize_ };
	}

	struct VertexLayOut
	{
		const D3D12_INPUT_ELEMENT_DESC layout[2] = {
		   {"POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		   {   "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};
	};

	//---  頂点レイアウト取得関数  ---
	[[nodiscard]] VertexLayout GetLayout()const noexcept {
		static VertexLayOut out;
		return { const_cast<D3D12_INPUT_ELEMENT_DESC*>(out.layout),_countof(out.layout) };
	}

	void Draw(ID3D12GraphicsCommandList* List)const noexcept {
		// 頂点バッファの設定
		List->IASetVertexBuffers(0, 1, &VertexBufferView_);
		// インデックスバッファの設定
		List->IASetIndexBuffer(&IndexBufferView_);
		// プリミティブ形状の設定（三角形）
		List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 描画コマンド
		List->DrawIndexedInstanced(3, 1, 0, 0, 0);
	}
	
private:
	Microsoft::WRL::ComPtr<ID3D12Resource>	VertexBuffer_{};      /// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>	IndexBuffer_{};       /// インデックスバッファ
	D3D12_VERTEX_BUFFER_VIEW				VertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW					IndexBufferView_{};
	D3D_PRIMITIVE_TOPOLOGY					Topology_{};          /// プリミティブトポロジー
	UINT									IndexCount_{};        /// インデックス数
	unsigned long long						VertexBuffersize_{};
	unsigned long long						IndexBuffersize_{};
};