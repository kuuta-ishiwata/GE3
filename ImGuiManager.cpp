#include "ImGuiManager.h"
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_dx12.h"
#include "External/imgui/imgui_impl_win32.h"


void ImGuiManager::Initialize(HWND hwnd, DirectXCommon* dxCommon)
{

	//ImGui�̏�����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(
		dxCommon->GetDevice(),
		dxCommon->GetswapChainDesc().BufferCount,
		dxCommon->GetRtvDesc().Format,
		dxCommon->GetSrvDescriptorHeaop(),
		dxCommon->GetSrvDescriptorHeaop()->GetCPUDescriptorHandleForHeapStart(),
		dxCommon->GetSrvDescriptorHeaop()->GetGPUDescriptorHandleForHeapStart()
	
	);


}

void ImGuiManager::NewFrame()
{
	//�X�V�����J�n��
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void ImGuiManager::CreateCommand()
{
	//�X�V�����I����
	ImGui::Render();

}

void ImGuiManager::CommandsExcute(ID3D12GraphicsCommandList* commandList)
{
	//ImGui�`��
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

}

ImGuiManager* ImGuiManager::Create()
{
	ImGuiManager* instance = new ImGuiManager();

	return nullptr;
}

ImGuiManager::~ImGuiManager()
{
	//�f�Xtp��N�^(delete)
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

void ImGuiManager::ShowDemo()
{
	ImGui::ShowDemoWindow();

}
