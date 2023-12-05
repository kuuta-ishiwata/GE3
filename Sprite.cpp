#include "Sprite.h"

#include <DirectXMath.h>
#include "BufferResource.h"


using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common)
{

	dxCommon_ = dxCommon;
	common_ = common;
	
	//頂点情報
	CreateVertex();
	//色
	CreateMaterial();
	//行列
	CreateWVP();
}

void Sprite::Draw()
{
	//Y軸回転
	transform.rotate.y += 0.05f;
	//ワールド
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.transform));
	
	//回転行列とスケール行列
	XMMATRIX rotationAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);
	
	//最終的な行列変換
	XMMATRIX worldMatrix = XMMatrixMultiply(rotationAndScaleMatrix, translationMatrix);
	
	//カメラ
	XMMATRIX cameraScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&cameraTransform.scale));
	XMMATRIX cameraRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraTransform.rotate));
	XMMATRIX cameraTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&cameraTransform.transform));
	
	//回転とスケールの掛け算
	XMMATRIX cameraRotateAndScameMatrix = XMMatrixMultiply(cameraRotateMatrix, cameraTranslationMatrix);
	
	//最終的な行列
	XMMATRIX cameraMatrix = XMMatrixMultiply(cameraRotateAndScameMatrix, cameraTranslationMatrix);

	//View
	XMMATRIX view = XMMatrixInverse(nullptr, cameraMatrix);
	
	//Proj
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.f),
		(float)WinApp::window_width / (float)WinApp::window_height,
		0.1f, 100.f);
	
	//WVP
	XMMATRIX worldViewProjectionMatrix = worldMatrix * (view * proj);
	//行列の代入
	*wvpData = worldViewProjectionMatrix;

	//行列の代入
	*wvpData = worldMatrix;


	dxCommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(common_->GetPipelineState());
	//頂点情報
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//色情報
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0,materialResource->GetGPUVirtualAddress());
	//行列
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

}

void Sprite::CreateVertex()
{

	//VertexResource
	vertexResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMFLOAT4) * 3);

	//VertexResource
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(DirectX::XMFLOAT4) * 3;
	vertexBufferView.StrideInBytes = sizeof(DirectX::XMFLOAT4);
	
	//頂点情報
	XMFLOAT4* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	vertexData[0] = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[1] = { 0.0f,+0.5f,0.0f,1.0f };
	vertexData[2] = { 0.5f,-0.5f,0.0f,1.0f };


}

void Sprite::CreateMaterial()
{

	materialResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMFLOAT4));

	XMFLOAT4* materialData = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	*materialData = color_;



}

void Sprite::CreateWVP()
{

	wvpResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMMATRIX));
	
	wvpResource ->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	*wvpData = XMMatrixIdentity();


}
