#include "Window.h"
#include <string.h>
namespace gameA
{
	LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{

		switch (uMessage) {
		case WM_CREATE:
		{
			return 0;
		}
		case WM_PAINT:
		{
			ValidateRect(hWnd, NULL);//basically saying - yeah we took care of any paint msg without any overhead
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
		}
		}
	}

	Window::Window(HINSTANCE * hInstance)
	{
		strAppName = (char*)malloc(sizeof(char) * 20);
		strcpy_s(strAppName,10, "Game");
		this->phInstance = hInstance;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpfnWndProc = (WNDPROC)Window::WndProc;
		wc.hInstance = *phInstance;
		wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_HAND);
		wc.hCursor = LoadCursor(NULL, IDC_CROSS);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = strAppName;
	}

	Window::~Window()
	{
		free(strAppName);
	}
	void Window::registerWindow()
	{
		RegisterClassEx(&(this->wc));
	}

	void Window::createWindow()
	{
		registerWindow();

		this->hwnd = CreateWindowEx(NULL,
			strAppName,
			strAppName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			512, 512,
			NULL,
			NULL,
			*phInstance,
			NULL);
	}

	HWND* Window::getHWnd()
	{
		return &hwnd;
	}

	void Window::show(int icmdShow)
	{
		ShowWindow(hwnd, icmdShow);
		UpdateWindow(hwnd);
	}

}