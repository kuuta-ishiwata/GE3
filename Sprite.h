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
	void Initialize(SpriteCommon* common, std::wstring textureFilePath);
	void Update();
	void Draw();


	//Getter/Setter
	DirectX::XMFLOAT2 GetPosition() { return position; }
	float GetRotation() { return rotation; }
	DirectX::XMFLOAT4 GetColor() { return color_; }
	DirectX::XMFLOAT2 Getize() { return size; }

	//アンカーポイント
	DirectX::XMFLOAT2 GetAnchorPoint() { return anchorPoint; }
	//左右反転
	bool GetFlipX() { return isFlipX; }
	//上下反転
	bool GetFlipY() { return isFlipY; }

	//切り抜き関連
	DirectX::XMFLOAT2 GetTExtureLeftTop() { return textureLeftTop; }
	DirectX::XMFLOAT2 GetTextureSize() { return textureSIze; }



	void SetPosition(DirectX::XMFLOAT2 pos) { position = pos; }
	void SetRotation(float rot) { rotation = rot; }
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }

	//アンカーポイント
	void SetAnchorPoint(DirectX::XMFLOAT2 anchor) { anchorPoint = anchor; }

	//左右反転
	void SetFlipX(bool isFlag) { isFlipX = isFlag; }
	//上下反転
	void SetFlipY(bool isFlag) { isFlipY = isFlag; }

	//切り抜き関連
	void SetTextureLeftTop(DirectX::XMFLOAT2 value) { textureLeftTop = value; }
	void SetTextureSize(DirectX::XMFLOAT2 size) { textureSIze = size; }

	void SetTexture(std::wstring textureFilePath);

private:

	//頂点情報
	void CreateVertex();

	//インデックス情報作成
	void CreateIndex();
	//マテリル
	void CreateMaterial();
	//行列情報
	void CreateWVP();

	//本来の画像サイズで描画する(textureSize)
	void AdujustTextureSize();

private:

	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* common_ = nullptr;

	//D3D12_GPU_DESCRIPTOR_HANDLE textureHandleGPU;

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
	DirectX::XMFLOAT2 size = { 512,512 };

	//アンカーポイント
	DirectX::XMFLOAT2 anchorPoint = { 0.5f,0.5f };
	//左右反転
	bool isFlipY = false;
	//上下反転
	bool isFlipX = false;

	//切り抜き
	//切り抜きたい画像内の座標
	DirectX::XMFLOAT2 textureLeftTop = { 0,0 };
	//切り抜きたい画像内のサイズ
	DirectX::XMFLOAT2 textureSIze = { 414,410 };

	//画像の保存されている場所
	uint32_t textureIndex_ = 0;


	//カメラ
	Transform cameraTransform = { {1,1,1},{0,0,0},{0,0,-5} };



};

