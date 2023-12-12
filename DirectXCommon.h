#pragma once
#include <wrl.h>
#include <dxgi1_6.h>
#include <d3d12.h>


#include <vector>
#include<chrono>
#include "WinApp.h"

class DirectXCommon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public:

	void Initialize(WinApp* winApp);

	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();

	//Getter
	ID3D12Device* GetDevice() const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
    
	//�X���b�v�`�F�[��
	DXGI_SWAP_CHAIN_DESC1 GetswapChainDesc() { return swapChainDesc; };

	//RTV�f�B�X�N
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() { return rtvDesc; }

	//SRV
	ID3D12DescriptorHeap* GetSrvDescriptorHeaop() { return srvDescriptorHeap.Get(); }

private:
	//�f�o�C�X
	void DeviceInitialize();
	//�R�}���h
	void CommandInitialize();
	//�X���b�v�`�F�[��
	void SwapChainInitialize();
	//�����_�[�^�[�Q�b�g
	void RenderTargetInitialize();

	void InitializeDepthBuffer();
	//�t�F���Y
	void FenceInitialize();

	//FPS�Œ菉��������
	void InitializeFixFPS();
	//FPS�Œ�X�V����
	void UpdateFixFPS();


	//�f�B�X�N���v�^�[�q�[�v
	ID3D12DescriptorHeap* CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescripots, bool shaderVisible);

	
private:
	WinApp* winApp = nullptr;

	ComPtr<ID3D12Device> device;

	ComPtr<IDXGIFactory7> dxgiFactory;
	
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	ComPtr<IDXGISwapChain4> swapChain;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};


	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;


	ComPtr<ID3D12Resource> depthBuff;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};
	
	//�L�^�p�̎��Ԍv��
	std::chrono::steady_clock::time_point reference_;

	//�f�B�X�N���v�^�[�q�[�v
	ComPtr<ID3D12DescriptorHeap>rtvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>srvDescriptorHeap;


};

