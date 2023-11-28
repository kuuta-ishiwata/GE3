#include "SpriteCommon.h"
#include <cassert>


#pragma comment(lib, "dxcompiler.lib")

using namespace Microsoft::WRL;

void SpriteCommon::Initialize(DirectXCommon* dxCommon)
{


  HRESULT result{};
  dxCommon_ = dxCommon;
  ComPtr<IDxcUtils>dxcUils;
  ComPtr<IDxcCompiler3> dxcCompiler ;
  ComPtr<IDxcIncludeHandler> includeHandler;
  result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUils));
  assert(SUCCEEDED(result));
  result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
  assert(SUCCEEDED(result));
 
  result = dxcUils->CreateDefaultIncludeHandler(&includeHandler);
  
  assert(SUCCEEDED(result));

  
  //rootSignature
  //作成
  D3D12_ROOT_SIGNATURE_DESC descriptorRootSignature{};
  descriptorRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

  //シリアライズとしてバイナリする
  ComPtr<ID3D10Blob>signatureBlob;
  ComPtr<ID3D10Blob>errorBlob;
  result = D3D12SerializeRootSignature(&descriptorRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
  if (FAILED(result))
  {
	  assert(false);
  }

  //バイナリをもとに
  
  result = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
	  signatureBlob->GetBufferSize(),IID_PPV_ARGS(&rootSignature));

  assert(SUCCEEDED(result));

  //InPutLayout
  D3D12_INPUT_ELEMENT_DESC inputElementDesc[1] = { };
  inputElementDesc[0].SemanticName = "POSITION";
  inputElementDesc[0].SemanticIndex = 0;
  inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  inputElementDesc[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
  D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
  inputLayoutDesc.pInputElementDescs = inputElementDesc;
  inputLayoutDesc.NumElements = _countof(inputElementDesc);

  //BlendState
  D3D12_BLEND_DESC blendDesc{ };

  blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


  //Rasterizer
  D3D12_RASTERIZER_DESC rasterizerDesc{};
  //裏面を描画しない
  rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
  //塗りつぶし
  rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

  //読み込み処理
  ComPtr<IDxcBlob> vertexShaderBlob = CompileShader(L"Resources/shaders/SpriteVS.hlsl", L"vs_6_0", dxcUils.Get(), dxcCompiler.Get(), includeHandler.Get());
  assert(SUCCEEDED(result));
  ComPtr<IDxcBlob> pixelShaderBlob = CompileShader(L"Resources/shaders/SpritePS.hlsl", L"ps_6_0", dxcUils.Get(), dxcCompiler.Get(), includeHandler.Get());

  assert(SUCCEEDED(result));

  //PipelineState
  D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateeDesc{};
  graphicsPipelineStateeDesc.pRootSignature = rootSignature.Get();
  graphicsPipelineStateeDesc.InputLayout = inputLayoutDesc;
  graphicsPipelineStateeDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
  graphicsPipelineStateeDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize()};
  graphicsPipelineStateeDesc.BlendState = blendDesc;
  graphicsPipelineStateeDesc.RasterizerState = rasterizerDesc;

  graphicsPipelineStateeDesc.NumRenderTargets = 1;
  graphicsPipelineStateeDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

  graphicsPipelineStateeDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

  graphicsPipelineStateeDesc.SampleDesc.Count = 1;
  graphicsPipelineStateeDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;


  
  result = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateeDesc, IID_PPV_ARGS(&pipelineState));
  assert(SUCCEEDED(result));


}

IDxcBlob* SpriteCommon::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUties, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
	//hlsl
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT result = dxcUties->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(result));

	//読み込んだファイル
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] =
	{
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	IDxcResult* shaderResult = nullptr;
	result = dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult)

	);
	assert(SUCCEEDED(result));
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		assert(false);
	}


	IDxcBlob* shaderBlob = nullptr;
	result = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(result));
	shaderSource->Release();
	shaderResult->Release();



	return shaderBlob;
}
