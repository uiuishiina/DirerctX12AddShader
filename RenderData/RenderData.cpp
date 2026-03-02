#include"RenderData.h"
#include"../DirectX12/Device.h"
#include"../DirectX12/DescriptorHeap.h"
#include<cassert>
#include <DirectXMath.h>

struct Vertex {
    DirectX::XMFLOAT3 position;  // 頂点座標（x, y, z）
    DirectX::XMFLOAT4 color;     // 頂点色（r, g, b, a）
};


//---  描画データ作成関数  ---
[[nodiscard]] bool RenderData::Create()noexcept {

    {
        // 今回利用する三角形の頂点データ
        Vertex triangleVertices[] = {
            {  {0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 上頂点（赤色）
            { {0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 右下頂点（緑色）
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}  // 左下頂点（青色）
        };

        // 頂点データのサイズ
        VertexBuffersize_ = sizeof(triangleVertices);

        // ヒープの設定を指定
        // CPU からアクセス可能なメモリを利用する為の設定
        D3D12_HEAP_PROPERTIES heapProperty{};
        heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
        heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProperty.CreationNodeMask = 1;
        heapProperty.VisibleNodeMask = 1;

        // どんなリソースを作成するかの設定
        D3D12_RESOURCE_DESC resourceDesc{};
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Alignment = 0;
        resourceDesc.Width = VertexBuffersize_;
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.SampleDesc.Count = 1;
        resourceDesc.SampleDesc.Quality = 0;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        // 頂点バッファの生成
        auto hr = Device::Instance().GetDevice()->CreateCommittedResource(
            &heapProperty,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&VertexBuffer_));
        if (FAILED(hr)) {
            assert(false && "VertexBuffer Creation Failed.");
            return false;
        }

        // 頂点バッファにデータを転送する
        // CPU からアクセス可能なアドレスを取得
        Vertex* data{};

        // バッファをマップ（CPUからアクセス可能にする）
        // vertexBuffer_ を直接利用するのではなく、data を介して更新するイメージ
        hr = VertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
        if (FAILED(hr)) {
            assert(false && "VertexBuffer Mapping Failed.");
            return false;
        }

        // 頂点データをコピー
        memcpy_s(data, VertexBuffersize_, triangleVertices, VertexBuffersize_);

        // コピーが終わったのでマップ解除（CPUからアクセス不可にする）
        VertexBuffer_->Unmap(0, nullptr);
        // 頂点バッファビューの設定
        VertexBufferView_.BufferLocation = VertexBuffer_->GetGPUVirtualAddress();  // 頂点バッファのアドレス
        VertexBufferView_.SizeInBytes = VertexBuffersize_;                       // 頂点バッファのサイズ
        VertexBufferView_.StrideInBytes = sizeof(Vertex);                         // 1頂点あたりのサイズ


        // トポロジーの設定（三角形）
        Topology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }


    //
    //
    //
    //
    //

    {
        uint16_t triangleIndices[] = {
        0, 1, 2  // 三角形を構成する頂点のインデックス
        };

        // インデックスデータのサイズ
        IndexBuffersize_ = sizeof(triangleIndices);

        // ヒープの設定を指定
        D3D12_HEAP_PROPERTIES heapProperty{};
        heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
        heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProperty.CreationNodeMask = 1;
        heapProperty.VisibleNodeMask = 1;

        // リソースの設定を行う
        D3D12_RESOURCE_DESC resourceDesc{};
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Alignment = 0;
        resourceDesc.Width = IndexBuffersize_;
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.SampleDesc.Count = 1;
        resourceDesc.SampleDesc.Quality = 0;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        // インデックスバッファの生成
        auto res = Device::Instance().GetDevice()->CreateCommittedResource(
            &heapProperty,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&IndexBuffer_));
        if (FAILED(res)) {
            assert(false && "IndexBuffer Creation Failed.");
            return false;
        }

        // インデックスバッファにデータを転送する
        uint16_t* data{};
        res = IndexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
        if (FAILED(res)) {
            assert(false && "IndexBuffer Mapping Failed.");
            return false;
        }

        memcpy_s(data, IndexBuffersize_, triangleIndices, IndexBuffersize_);
        // ここまで来たら GPU が利用するメモリ領域（VRAM）にコピー済みなので、triangleIndices は不要になる
        IndexBuffer_->Unmap(0, nullptr);

        IndexBufferView_.BufferLocation = IndexBuffer_->GetGPUVirtualAddress();
        IndexBufferView_.SizeInBytes = IndexBuffersize_;
        IndexBufferView_.Format = DXGI_FORMAT_R16_UINT;  // triangleIndices の型が 16bit 符号なし整数なので R16_UINT


        IndexCount_ = _countof(triangleIndices);
    }
    return true;
}

//---  頂点データ取得関数  ---
[[nodiscard]] ID3D12Resource* RenderData::GetVertex()const noexcept {
    assert(VertexBuffer_ && "VertexBuffer not Created.");
    return VertexBuffer_.Get();
}

//---  インデックスデータ取得関数  ---
[[nodiscard]] ID3D12Resource* RenderData::GetIndex()const noexcept {
    assert(IndexBuffer_ && "IndexBuffer not Created.");
    return IndexBuffer_.Get();
}

//---  トポロジー設定取得関数  ---
[[nodiscard]] const D3D_PRIMITIVE_TOPOLOGY RenderData::GetToporogy()const noexcept {
    return Topology_;
}

//---  インデックス数取得関数  ---
[[nodiscard]] const UINT RenderData::GetIndexCount()const noexcept {
    return IndexCount_;
}
