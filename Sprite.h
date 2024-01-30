#pragma once
#include "DirectXCommon.h"
#include <d3d12.h>
#include <wrl.h>
#include <DIrectXMath.h>

#include "SpriteCommon.h"

//単体
class Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	struct  Transform
	{
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rotate;
		DirectX::XMFLOAT3  translate;
	};


	//頂点情報
	struct VertexData
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texcoord;

	};
	//マテリアル
	struct  MaterialData
	{

		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX uvTransform;

	};
	


public:

	//初期化
	void Initialize(DirectXCommon* dxCommon, SpriteCommon* common);
	void Update();
	void Draw();


	//Getter/Setter
	DirectX::XMFLOAT2 GetPosition() { return position; }
	float GetRotation() { return rotation; }
	DirectX::XMFLOAT4 GetColor() { return color_; }
	DirectX::XMFLOAT2 Getize() { return size; }

	void SetPosition(DirectX::XMFLOAT2 pos) { position = pos; }
	void SetRotation(float rot) { rotation = rot; }
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }


private:

	//頂点情報
	void CreateVertex();

	//インデックス情報作成
	void CreateIndex();
	//マテリル
	void CreateMaterial();
	//行列情報
	void CreateWVP();


private:

	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* common_ = nullptr;

	D3D12_GPU_DESCRIPTOR_HANDLE textureHandleGPU;

	//頂点情報
	ComPtr<ID3D12Resource>vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData = nullptr;
	//インデックス
	ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	//マテリアル
	ComPtr<ID3D12Resource> materialResource;

	MaterialData* materialData = nullptr;

	//行列情報
	ComPtr<ID3D12Resource> wvpResource;

	DirectX::XMMATRIX* wvpData = nullptr;
	//パラメーター
	DirectX::XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	//UV座標
	Transform uvTransform = { {1,1,1},{0,0,0},{0,0,0} };

	//自機                      scale   Rotate   Transform 
	Transform transform = { {1,1,1},{0,0,0},{0,0,0} };
	DirectX::XMFLOAT2 position = {0,0};
	float rotation = 0;
	DirectX::XMFLOAT2 size = { 1,1 };
	//カメラ
	Transform cameraTransform = { {1,1,1},{0,0,0},{0,0,-5} };



};

