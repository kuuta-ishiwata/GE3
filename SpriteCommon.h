#pragma once
#include <wrl.h>
#include <string>
#include <dxcapi.h>
#include <d3dx12.h>

#include "DirectXCommon.h"

//‹¤—L
class SpriteCommon
{

private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	void Initialize(DirectXCommon* dxCommon);

	//Getter
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }



private:


	static IDxcBlob* CompileShader(
		const std::wstring& filePath, 
		const wchar_t* profile,
		IDxcUtils* dxcUties,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);
private:

	DirectXCommon* dxCommon_ = nullptr;
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12PipelineState> pipelineState;

};

