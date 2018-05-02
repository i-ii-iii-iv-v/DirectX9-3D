#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>


namespace gameA
{
	class Window
	{

	public:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		Window::Window() = delete;
		Window::Window(HINSTANCE * hInstance);

		Window::~Window();
		void registerWindow();
		void createWindow();

		void show(int icmdShow);

		HWND* getHWnd();
	private:
		HWND hwnd;
		MSG msg;
		WNDCLASSEX wc;
		HINSTANCE * phInstance;
		char *strAppName;
	};
}