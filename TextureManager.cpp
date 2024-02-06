#include "TextureManager.h"
#include "BufferResource.h"


TextureManager* TextureManager::instance = nullptr;
uint32_t TextureManager::kSRVIndexTop = 1;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureManager();

	}

	return instance;
}

void TextureManager::Finalize()
{

	delete instance;
	instance = nullptr;

}

void TextureManager::Initialize(DirectXCommon* dxCommon)
{


	dxCommon_ = dxCommon;
	textureDatas.reserve(DirectXCommon::kMaxSRVCount);

}

void TextureManager::LoadTexture(const std::wstring& filePath)
{

	//�ǂݍ��ݍς݂��`�F�b�N����
	auto it = std::find_if(
	textureDatas.begin(),
	textureDatas.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; }
	);

	if (it != textureDatas.end())
	{
		return;
	}
	assert(textureDatas.size() + kSRVIndexTop < DirectXCommon::kMaxSRVCount);

	//�e�N�X�`����ǂݍ���Ńv���O�����ň�����悤�ɂ���
	DirectX::ScratchImage imgae{};
	HRESULT result = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_DEFAULT_SRGB, nullptr, imgae);
	assert(SUCCEEDED(result));

	DirectX::ScratchImage mipImges{};
	result = DirectX::GenerateMipMaps(
		imgae.GetImages(), imgae.GetImageCount(), imgae.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0, mipImges);
	assert(SUCCEEDED(result));

	//�摜�f�[�^���X�C�J
	textureDatas.resize(textureDatas.size() + 1);

	//�ǉ������f�[�^�̕ҏW
	TextureData& data = textureDatas.back();

	data.filePath = filePath;
	data.metaData = mipImges.GetMetadata();
	data.resource = CreateTextureResource(dxCommon_->GetDevice(), data.metaData);
	UploadTextureData(data.resource.Get(), mipImges);

	//�摜���ۑ�����Ă��郁����
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = dxCommon_->GetSrvDescriptorHeaop()->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = dxCommon_->GetSrvDescriptorHeaop()->GetGPUDescriptorHandleForHeapStart();
	handleCPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * srvIndex;
	handleGPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * srvIndex;
	data.sevHandleCPU = handleCPU;
	data.srvHandleGPU = handleGPU;

	//shaderview�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format =data. metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = UINT(data.metaData.mipLevels);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;


	//�ǂݍ��񂾏���SrvDesc(�g)��handle(�ʒu)���g���ĕۑ�����
	dxCommon_->GetDevice()->CreateShaderResourceView(data.resource.Get(), &srvDesc, data.sevHandleCPU);

}

uint32_t TextureManager::GetTextureIndexFilePatj(const std::wstring& filePath)
{

	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; }
	);
	
	if (it != textureDatas.end())
	{
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}

	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t textureIndex)
{
	//�Ώۗv�f�ԍ���7�������͈̔͊O��I�����Ă��Ȃ����m�F
	assert(textureIndex < DirectXCommon::kMaxSRVCount);

	//�v�f�ԍ���TextureData������
	TextureData& data = textureDatas[textureIndex];


	return data.srvHandleGPU;
}

void TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{

	//Meta�����擾
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	//�SMipMap
	for (size_t mipLevel = 0; mipLevel < metaData.mipLevels; ++mipLevel)
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
