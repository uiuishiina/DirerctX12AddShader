#include"CompileShader.h"
#include"Device.h"
#include<cassert>

#include <D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

class CompileShader final
{
public:
	CompileShader() = default;
	~CompileShader() = default;

	//---  シェーダー作成関数  ---
	[[nodiscard]] bool Create(const std::string path, const std::string entryPoint, const std::string version)noexcept {

		//---  シェーダーパス取得  ---
		const std::wstring temp = std::wstring(path.begin(), path.end());

		Microsoft::WRL::ComPtr<ID3DBlob> error{};
		auto res = D3DCompileFromFile(temp.data(), nullptr, nullptr, entryPoint.c_str(), version.c_str(), D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &Shader_, &error);
		if (FAILED(res)) {
			char* p = static_cast<char*>(error->GetBufferPointer());
			assert(false && "Shader Compile Failed.");
		}
		return true;
	}

	//---  シェーダー取得関数  ---
	[[nodiscard]] ID3DBlob* Get()const noexcept {
		assert(Shader_ && "Shader not Created.");
		return Shader_.Get();
	}
private:
	Microsoft::WRL::ComPtr<ID3DBlob> Shader_{};
};

//
//
//
//---  シェーダーマネージャー  ---
//
//
//

ShaderManager::ShaderManager() = default;
ShaderManager::~ShaderManager() = default;

//---  シェーダー作成指示関数  ---
[[nodiscard]] bool ShaderManager::CreateCall(const std::string path, const std::string entryPoint, const std::string version, const std::string tag)noexcept {

	//作成済みか確認
	if (ShaderMap_.find(tag) != ShaderMap_.end()) {
		return false;
	}

	//---  シェーダー作成指示  ---
	auto p = std::make_unique<CompileShader>();
	assert(p->Create(path,entryPoint,version) && "ShaderManager CreationCall Failed.");

	//---  シェーダーマップ登録  ---
	ShaderMap_.emplace(tag, std::move(p));
	return true;
}

//---  シェーダー取得指示関数  ---
[[nodiscard]] ID3DBlob* ShaderManager::GetCall(const std::string tag) const noexcept {

	//作成済みか確認
	assert(ShaderMap_.find(tag) != ShaderMap_.end() && "Shader not Created.(call GetCall())");

	return ShaderMap_.find(tag)->second->Get();
}