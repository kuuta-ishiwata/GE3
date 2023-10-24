#pragma once

#include <Windows.h>

class WinApp
{

public:

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	void Initialize();
	void Update();

	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetInstance() const { return w.hInstance; }

public:

	// ウィンドウサイズ
	static const int window_width = 1280;  // 横幅
	static const int window_height = 720;  // 縦幅

private:


	HWND hwnd;
	WNDCLASSEX w{};

};

