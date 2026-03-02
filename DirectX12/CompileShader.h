#pragma once
#include"LibrarySummary.h"

class CompileShader;

class ShaderManager final
{
public:
	//---  シェーダーマネージャークラスインスタンス取得関数  ---
	static ShaderManager& Instance()noexcept {
		static ShaderManager instance_;
		return instance_;
	}

	//---  シェーダー作成指示関数  ---
	[[nodiscard]] bool CreateCall(const std::string path,const std::string entryPoint,const std::string version,const std::string tag)noexcept;

	//---  シェーダー取得指示関数  ---
	[[nodiscard]] ID3DBlob* GetCall(const std::string tag) const noexcept;
private:
	ShaderManager();
	~ShaderManager();

	std::unordered_map<std::string, std::unique_ptr<CompileShader>> ShaderMap_{};
};