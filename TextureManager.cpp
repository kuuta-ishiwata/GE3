#include "TextureManager.h"
#include "BufferResource.h"
TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureManager();

	}

	return nullptr;
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
	DirectX::ScratchImage imgae{};
	HRESULT result = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_DEFAULT_SRGB, nullptr, imgae);
	assert(SUCCEEDED(result));

	DirectX::ScratchImage mipImges{};
	result = DirectX::GenerateMipMaps(
		imgae.GetImages(), imgae.GetImageCount(), imgae.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0, mipImges);
	assert(SUCCEEDED(result));

	//画像データをスイカ
	textureDatas.resize(textureDatas.size() + 1);

	//追加したデータの編集
	TextureData& data = textureDatas.back();

	data.filePath = filePath;
	data.metaData = mipImges.GetMetadata();
	data.resource = CreateTextureResource(dxCommon_->GetDevice(), data.metaData);
	UploadTextureData(data.resource.Get(), mipImges);

	//画像が保存されているメモリ
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1);
	


}

void TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{

	//Meta情報を取得
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	//全MipMap
	for (size_t mipLevel = 0; mipLevel < metaData.mipLevels; ++mipLevel)
	{
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Textureに転送
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
