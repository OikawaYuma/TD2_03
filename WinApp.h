#pragma once
#include<Windows.h>
#include<cstdint>

class WinApp
{
public:
	
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	WinApp(const wchar_t* label);
	~WinApp();

	WNDCLASS wc{};

	// クライアント領域のサイズ
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;
	

};




