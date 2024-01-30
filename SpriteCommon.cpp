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

  
  //RootSignature
  //�쐬
  D3D12_ROOT_SIGNATURE_DESC descriptorRootSignature{};
  descriptorRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


  //�摜�悤�̃f�B�X�N���v�^�͈�
  D3D12_DESCRIPTOR_RANGE descriptorRange[1]{};
  descriptorRange[0].BaseShaderRegister = 0;
  descriptorRange[0].NumDescriptors = 1;
  descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


  //RootParameter�̍쐬
  D3D12_ROOT_PARAMETER rootParameters[3]{};
  //�F
  rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
  rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
  rootParameters[0].Descriptor.ShaderRegister = 0;

   //�s��	
  rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
  rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
  rootParameters[1].Descriptor.ShaderRegister = 0;
  //�摜
  rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
  rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
  rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
  rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

  descriptorRootSignature.pParameters = rootParameters;
  descriptorRootSignature.NumParameters = _countof(rootParameters);

  //Sample�̐ݒ�
  D3D12_STATIC_SAMPLER_DESC staticSamples[1] = {};
  staticSamples[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
  staticSamples[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  staticSamples[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  staticSamples[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;;
  staticSamples[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
  staticSamples[0].ShaderRegister = 0;
  staticSamples[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

  descriptorRootSignature.pStaticSamplers = staticSamples;
  descriptorRootSignature.NumStaticSamplers = _countof(staticSamples);

  //�V���A���C�Y�Ƃ��ăo�C�i������
  ComPtr<ID3D10Blob>signatureBlob;
  ComPtr<ID3D10Blob>errorBlob;
  result = D3D12SerializeRootSignature(&descriptorRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
  if (FAILED(result))
  {
	  assert(false);
  }

  //�o�C�i�������Ƃ�
  
  result = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
	  signatureBlob->GetBufferSize(),IID_PPV_ARGS(&rootSignature));

  assert(SUCCEEDED(result));

  //InPutLayout
  D3D12_INPUT_ELEMENT_DESC inputElementDesc[2] = { };
  inputElementDesc[0].SemanticName = "POSITION";
  inputElementDesc[0].SemanticIndex = 0;
  inputElementDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  inputElementDesc[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

  inputElementDesc[1].SemanticName = "TEXCOORD";
  inputElementDesc[1].SemanticIndex = 0;
  inputElementDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
  inputElementDesc[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;


  D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
  inputLayoutDesc.pInputElementDescs = inputElementDesc;
  inputLayoutDesc.NumElements = _countof(inputElementDesc);

  //BlendState
  D3D12_BLEND_DESC blendDesc{ };

  blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


  //Rasterizer
  D3D12_RASTERIZER_DESC rasterizerDesc{};
  //���ʂ�`�悵�Ȃ�
  rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
  //�h��Ԃ�
  rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

  //�ǂݍ��ݏ���
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

DirectX::ScratchImage SpriteCommon::LoadTexture(const std::wstring& filePath)
{

	
	DirectX::ScratchImage imgae{};
	HRESULT result = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_DEFAULT_SRGB, nullptr, imgae);

	DirectX::ScratchImage mipImges{};
	result = DirectX::GenerateMipMaps(
		imgae.GetImages(), imgae.GetImageCount(), imgae.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0, mipImges);
	assert(SUCCEEDED(result));

	return imgae;


}

void SpriteCommon::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	//Meta�����擾
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	//�SMipMap
	for(size_t mipLevel = 0; mipLevel < metaData.mipLevels; ++mipLevel)
		{
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Texture�ɓ]��
		HRESULT result = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(result));

		}
}

IDxcBlob* SpriteCommon::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUties, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
	//hlsl
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT result = dxcUties->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(result));

	//�ǂݍ��񂾃t�@�C��
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
