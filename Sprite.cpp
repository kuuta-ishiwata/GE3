#include "Sprite.h"

#include <DirectXMath.h>
#include "BufferResource.h"

#include "External/imgui/imgui.h"

#include "TextureManager.h"

using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize(SpriteCommon* common, std::wstring textureFilePath)
{

	
	common_ = common;
	dxCommon_ = common_->GetDirectXCommon();

	/*
	//�摜�̓ǂݍ���
	DirectX::ScratchImage mipImages = common->LoadTexture(L"Resources/mario.jpg");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(dxCommon_->GetDevice(),  metaData);
	common_->UploadTextureData(textureResource, mipImages);

	//shaderview�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = UINT(metaData.mipLevels);
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	//�ۑ��������̏ꏊ���w��
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvhandleCPU = dxCommon_->GetSrvDescriptorHeaop()->GetCPUDescriptorHandleForHeapStart();
	 textureHandleGPU = dxCommon_->GetSrvDescriptorHeaop()->GetGPUDescriptorHandleForHeapStart();

	textureSrvhandleCPU.ptr+= dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureHandleGPU.ptr+= dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	//�ǂݍ��񂾏���SrvDesc(�g)��handle(�ʒu)���g���ĕۑ�����
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvhandleCPU);
	*/
	

	textureIndex_ = TextureManager::GetInstance()->GetTextureIndexFilePatj(textureFilePath);


	//���_���
	CreateVertex();
	//�C���f�b�N�X���
	CreateIndex();
	//�F
	CreateMaterial();
	//�s��
	CreateWVP();

	//�摜�T�C�Y�𐮗�����
	AdujustTextureSize();
}

void Sprite::Update()
{
	position.y = 200.0f;
	//rotation += 0.01f;
	//�X�V����
	transform.translate = {position.x,position.y,0};
	transform.rotate = { 0,0,rotation };
	materialData->color = color_;
	transform.scale = { size.x, size.y,1.0f };
	
	//�A���J�[�|�C���g�X�V
	float left   = 0.0f - anchorPoint.x;
	float right  = 1.0f - anchorPoint.x;
	float top    = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;


	//�t���b�v
	if (isFlipX == true)
	{
		//���E���]
		left = -left;
		right = -right;

	}
	if (isFlipY == true)
	{
		//�㉺���]
		top = -top;
		bottom = -bottom;
	}

	//���_���
	vertexData[0].position = { left,bottom,0.0f,1.0f };
	vertexData[1].position = { left,top,0.0f,1.0f };
	vertexData[2].position = { right, bottom,0.0f,1.0f };
	vertexData[3].position = { right,top,0.0f,1.0f };

	const DirectX::TexMetadata& metaData = TextureManager::GetInstance()->GetMetaData(textureIndex_);
	float tex_left    = textureLeftTop.x / metaData.width ;
	float tex_right   =  (textureLeftTop.x + textureSIze.x) /metaData.width;
	float tex_top     =  textureLeftTop.x / metaData.height;
	float tex_bottom  =  (textureLeftTop.y + textureSIze.y) / metaData.height;

	//UV���W
	vertexData[0].texcoord = { tex_left,tex_bottom };
	vertexData[1].texcoord = { tex_left,tex_top };
	vertexData[2].texcoord = { tex_right,tex_bottom };
	vertexData[3].texcoord = { tex_right,tex_top };


	ImGui::Begin("Texture");
	ImGui::DragFloat3("Pos", &transform.translate.x, 0.1f);

	ImGui::DragFloat3("UV-Pos", &uvTransform.translate.x, 0.1f);
	ImGui::DragFloat3("UV-Rot", &uvTransform.translate.x, 0.01f, -10.f, 10.f);
	ImGui::SliderAngle("UV-Scale", &uvTransform.scale.x, 0.01f, 10.f);
	ImGui::End();
}

void Sprite::Draw()
{
	//Y����]
	//transform.rotate.y += 0.05f;
	//���[���h
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.translate));
	
	//��]�s��ƃX�P�[���s��
	XMMATRIX rotationAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);
	
	//�ŏI�I�ȍs��ϊ�
	XMMATRIX worldMatrix = XMMatrixMultiply(rotationAndScaleMatrix, translationMatrix);
	
	//�J����
	XMMATRIX cameraScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&cameraTransform.scale));
	XMMATRIX cameraRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraTransform.rotate));
	XMMATRIX cameraTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&cameraTransform.translate));
	
	//��]�ƃX�P�[���̊|���Z
	XMMATRIX cameraRotateAndScameMatrix = XMMatrixMultiply(cameraRotateMatrix, cameraTranslationMatrix);
	
	//�ŏI�I�ȍs��
	XMMATRIX cameraMatrix = XMMatrixMultiply(cameraRotateAndScameMatrix, cameraTranslationMatrix);

	//View
	XMMATRIX view = XMMatrixInverse(nullptr, cameraMatrix);
	
	//Proj
	XMMATRIX proj = XMMatrixOrthographicOffCenterLH(0, WinApp::window_width, WinApp::window_height, 0, 0.1f, 100.f);
	/*
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.f),
		(float)WinApp::window_width / (float)WinApp::window_height,
		0.1f, 100.f);
	*/
	//WVP
	XMMATRIX worldViewProjectionMatrix = worldMatrix * (view * proj);
	//�s��̑��
	*wvpData = worldViewProjectionMatrix;

	//�s��̑��
	//*wvpData = worldMatrix;



	//UV���W
	XMMATRIX uvscaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&uvTransform.scale));
	XMMATRIX uvrotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&uvTransform.rotate));
	XMMATRIX uvtranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&uvTransform.translate));

	//��]�s��ƃX�P�[���s��
	XMMATRIX uvrotationAndScaleMatrix = XMMatrixMultiply(uvrotateMatrix, uvscaleMatrix);

	//�ŏI�I�ȍs��ϊ�
	XMMATRIX uvworldMatrix = XMMatrixMultiply(uvrotationAndScaleMatrix, uvtranslationMatrix);

	materialData->uvTransform = uvworldMatrix;

	//���_���
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(common_->GetPipelineState());

	//���_���
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	//�C���f�b�N�X���
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�F���
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0,materialResource->GetGPUVirtualAddress());
	//�s��
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());


	//�摜
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex_));

	//dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	//�C���f�b�N�X��񂪂���ꍇ�̕`��
	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0,0);


}

void Sprite::SetTexture(std::wstring textureFilePath)
{
	textureIndex_ = TextureManager::GetInstance()->GetTextureIndexFilePatj(textureFilePath);

}

void Sprite::CreateVertex()
{

	//VertexResource
	vertexResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 4);

	//VertexResource
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	
	//���_���
	
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = { 0.0f,1.0f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };

	vertexData[1].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };

	vertexData[2].position = { 1.0f, 1.0f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };

	vertexData[3].position = { 1.0f,0.0f,0.0f,1.0f };
	vertexData[3].texcoord = { 1.0f,0.0f };


}

void Sprite::CreateIndex()
{

	indexResource = CreateBufferResource(dxCommon_->GetDevice(),sizeof(uint32_t)*6);
	
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	//vertexData[0,1,2] �̒��_�ŎO�p�`���ꖇ�쐬
	indexData[0] = 0;  indexData[1] = 1;  indexData[2] = 2;

	//verrexData[1,3,2]�̒��_�ŎO�p�`���ꖇ�쐬
	indexData[3] = 1; indexData[4] = 3;   indexData[5] = 2;



}

void Sprite::CreateMaterial()
{

	materialResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(MaterialData));



	
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	materialData->color =  color_;
	materialData->uvTransform = XMMatrixIdentity();



}

void Sprite::CreateWVP()
{

	wvpResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMMATRIX));
	
	wvpResource ->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	*wvpData = XMMatrixIdentity();


}

void Sprite::AdujustTextureSize()
{
	const DirectX::TexMetadata& metaData = TextureManager::GetInstance()->GetMetaData(textureIndex_);

	textureSIze.x = static_cast<float>(metaData.width);
	textureSIze.y = static_cast<float>(metaData.height);

	size = textureSIze;

}
