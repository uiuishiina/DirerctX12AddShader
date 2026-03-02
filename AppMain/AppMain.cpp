#include"../DirectX12/LibrarySummary.h"
#include"ReferenceSummary.h"
#include<cassert>

namespace AppInfo {
	//バッファカウント
	const UINT64 BufferCount_ = 2;
}

class Application final
{
public:
	Application() = default;
	~Application() = default;
	//---  アプリケーション作成  ---
	[[nodiscard]] bool Create(HINSTANCE hInstance)noexcept {

		//ウィンドウ作成
		assert(Window::Instance().Create(hInstance, 1280, 720, "DirectX12Addshader") && "ウィンドウ作成失敗");
		//デバイス作成
		assert(Device::Instance().Create() && "デバイス作成失敗");
		//コマンドキュー作成
		assert(Queue_.Create() && "コマンドキュー作成失敗");
		//コマンドアロケーター作成
		assert(Allocator_.Create(AppInfo::BufferCount_) && "コマンドアロケーター作成失敗");
		//コマンドリスト作成
		assert(List_.Create(Allocator_.Get(0),Allocator_.GetType()) && "コマンドリスト作成失敗");
		//スワップチェーン作成
		assert(SwapChain_.Create(Queue_.Get(), AppInfo::BufferCount_) && "スワップチェーン作成失敗");

		//CPU用ディスクリプタヒープ作成
		assert(CPUDescriptorManager::Instance().CreateCall(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, AppInfo::BufferCount_) && "RTVディスクリプタヒープ作成失敗");
		//GPU用ディスクリプタヒープ作成
		assert(GPUDescriptorManager::Instance().CreateCall(2, AppInfo::BufferCount_) && "GPU用ディスクリプタヒープ作成失敗");

		//レンダーターゲット作成
		assert(Target_.Create(SwapChain_.Get(), AppInfo::BufferCount_) && "レンダーターゲット作成失敗");
		//フェンス作成
		assert(Fence_.Create() && "フェンス作成失敗");

		//ルートシグネチャー作成
		assert(Root_.CreateTemplate<RSPreparation>() && "ルートシグネチャー作成失敗");

		//頂点シェーダー作成
		assert(ShaderManager::Instance().CreateCall("../ShaderFile/VertexShader.hlsl", "VS", "vs_5_0", "Vertex") && "頂点シェーダー作成失敗");
		//ピクセルシェーダー作成
		assert(ShaderManager::Instance().CreateCall("../ShaderFile/PixelShader.hlsl", "PS", "ps_5_0", "Pixel") && "ピクセルシェーダー作成失敗");

		assert(Data_.Create() && "");

		//3D描画用パイプラインステート作成
		assert(PSOManager::Instance().CreateTemplate("3D","Vertex","Pixel",Root_.Get(), Data_.GetLayout(), 0, false) && "");

		return true;
	}

	void AppLoop()noexcept {
		while (Window::Instance().MessageLoop())
		{
			const auto backBufferIndex = SwapChain_.Get()->GetCurrentBackBufferIndex();
			// 以前のフレームの GPU の処理が完了しているか確認して待機する
			if (FrameFenceValue_[backBufferIndex] != 0) {
				Fence_.WaitGPU(FrameFenceValue_[backBufferIndex]);
			}

			// コマンドアロケータリセット
			Allocator_.Reset(backBufferIndex);
			// コマンドリストリセット
			List_.Reset(Allocator_.Get(backBufferIndex));

			// リソースバリアでレンダーターゲットを Present から RenderTarget へ変更
			auto pToRT = Device::Instance().ResourceBarrier(Target_.Get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			List_.Get()->ResourceBarrier(1, &pToRT);

			// レンダーターゲットの設定
			D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { Target_.GetHandle(backBufferIndex)};
			List_.Get()->OMSetRenderTargets(1, handles, false, nullptr);

			// レンダーターゲットのクリア
			const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			List_.Get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

			{
				// パイプラインステートの設定
				List_.Get()->SetPipelineState(PSOManager::Instance().GetCall("3D"));
				// ルートシグネチャの設定
				List_.Get()->SetGraphicsRootSignature(Root_.Get());

				// ビューポートの設定
				const auto [w, h] = Window::Instance().GetSize();
				D3D12_VIEWPORT viewport{};
				viewport.TopLeftX = 0.0f;
				viewport.TopLeftY = 0.0f;
				viewport.Width = static_cast<float>(w);
				viewport.Height = static_cast<float>(h);
				viewport.MinDepth = 0.0f;
				viewport.MaxDepth = 1.0f;
				List_.Get()->RSSetViewports(1, &viewport);

				// シザー矩形の設定
				D3D12_RECT scissorRect{};
				scissorRect.left = 0;
				scissorRect.top = 0;
				scissorRect.right = w;
				scissorRect.bottom = h;
				List_.Get()->RSSetScissorRects(1, &scissorRect);


				Data_.Draw(List_.Get());
			}
			
			// リソースバリアでレンダーターゲットを RenderTarget から Present へ変更
			auto rtToP = Device::Instance().ResourceBarrier(Target_.Get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			List_.Get()->ResourceBarrier(1, &rtToP);

			// コマンドリストをクローズ
			List_.Get()->Close();

			// コマンドキューにコマンドリストを送信
			ID3D12CommandList* ppCommandLists[] = { List_.Get() };
			Queue_.Get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			// プレゼント
			SwapChain_.Get()->Present(1, 0);

			// フェンスにフェンス値を設定
			Queue_.Get()->Signal(Fence_.Get(), NextFenceValue_);
			FrameFenceValue_[backBufferIndex] = NextFenceValue_;
			NextFenceValue_++;

		}
	}
private:
	CommandQueue		Queue_{};			//コマンドキュークラス
	CommandAllocator	Allocator_{};		//コマンドアロケータークラス
	CommandList			List_{};			//コマンドリストクラス

	SwapChain			SwapChain_{};		//スワップチェーンクラス
	RenderTarget		Target_{};			//レンダーターゲットクラス
	RootSignature		Root_{};			//ルートシグネチャークラス
	
	Fence				Fence_{};			//フェンスクラス
	UINT64				FrameFenceValue_[AppInfo::BufferCount_]{};  /// 現在のフレームのフェンス
	UINT64				NextFenceValue_ = 1;

	RenderData			Data_{};


};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Application App;
	//アプリ作成
	assert(App.Create(hInstance) && "アプリケーション作成失敗");
	//ループ開始
	App.AppLoop();

	return 0;
}