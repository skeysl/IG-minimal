#ifdef WIN32
#include "application.h"

#include "window.h"

#include <stdexcept>


namespace igui::win32
{
	application::application()
	{
		HINSTANCE instance = GetModuleHandle(NULL);

		WNDCLASS wndclass = { 0 };
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = instance;
		wndclass.hIcon = NULL;
		wndclass.hCursor = NULL;
		wndclass.hbrBackground = NULL;
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = "igclass";

		if (!RegisterClass(&wndclass))
			throw std::runtime_error("RegisterClass failed!");
	}

	//https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-close
	LONG WINAPI application::MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		window* curWindow = (window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if ((!curWindow) || (curWindow->wnd != hWnd))
			return (LONG)DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
			/*case WM_CREATE:
				break;

			case WM_PAINT:
				break;*/

				//case WM_SYSCOMMAND:                     // Intercept System Commands
				//{
				//	switch (wParam)                     // Check System Calls
				//	{
				//	case SC_SCREENSAVE:             // Screensaver Trying To Start?
				//	case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
				//		return 0;                   // Prevent From Happening
				//	}
				//	break;                          // Exit
				//}

				//case WM_SIZE:
				//	break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		return 0;
	}


	void application::run()
	{
		MSG msg{};

		BOOL bRet;

		// NULL hwnd means any window that belongs to current thread
		while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (bRet == -1)
			throw std::runtime_error("GetMessage failed");
	}


	bool application::poll()
	{
		bool wm_quit = false;

		MSG msg{};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				wm_quit = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return !wm_quit;
	}


	void application::minimize_console()
	{
		WINDOWPLACEMENT placemenet;
		::GetWindowPlacement(::GetConsoleWindow(), &placemenet);
		placemenet.showCmd = SW_MINIMIZE;
		::SetWindowPlacement(::GetConsoleWindow(), &placemenet);
	}
}

#endif
