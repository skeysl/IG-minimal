#ifdef WIN32
#include "window.h"

#include "../../ig/log.h"


namespace igui::win32
{
	window::window(int x, int y, int width, int height, const char * title,
		std::optional<int> monitor)// bool debug = false, int swap_group = -1, int swap_barrier = -1)
	{
		HINSTANCE instance = GetModuleHandle(NULL);

		if (monitor)                             // Attempt Fullscreen Mode?
		{
			int monitor_index = *monitor;

			DISPLAY_DEVICE device;
			device.cb = sizeof(DISPLAY_DEVICE);

			unsigned int i = 0;
			while (EnumDisplayDevices(NULL, i++, &device, 0))
			{
				if ((device.StateFlags & DISPLAY_DEVICE_ACTIVE) && (monitor_index-- == 0))
					break;
			}

			if (monitor_index != -1)
				throw std::runtime_error("EnumDisplayDevices failed!");

			DEVMODE mode;						// Device Mode
			memset(&mode, 0, sizeof(DEVMODE));       // Makes Sure Memory's Cleared
			mode.dmSize = sizeof(DEVMODE);       // Size Of The Devmode Structure
			mode.dmPelsWidth = width;            // Selected Screen Width
			mode.dmPelsHeight = height;           // Selected Screen Height
			mode.dmBitsPerPel = 32;             // Selected Bits Per Pixel
			mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (ChangeDisplaySettingsEx(device.DeviceName, &mode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
				throw std::runtime_error("ChangeDisplaySettingsEx failed!");

			DEVMODE current;
			memset(&current, 0, sizeof(DEVMODE));
			current.dmSize = sizeof(DEVMODE);

			if (!EnumDisplaySettingsEx(device.DeviceName, ENUM_CURRENT_SETTINGS, &current, EDS_ROTATEDMODE))
				throw std::runtime_error("EnumDisplaySettingsEx failed!");

			x = current.dmPosition.x;
			y = current.dmPosition.y;

			ShowCursor(FALSE);
		}

		//https://blogs.msdn.microsoft.com/oldnewthing/20050505-04/?p=35703/
		wnd = CreateWindow(
			"igclass",							// Class Name
			title,								// Window Title
			WS_POPUP | WS_VISIBLE,				// Required Window Style
			x, y, width, height,				// Calculate Window Height
			NULL,								// No Parent Window
			NULL,								// No Menu
			instance,							// Instance
			this);
		if (!wnd)
			throw std::runtime_error("CreateWindow failed!");

		dc = GetDC(wnd);

		SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR)this);

		ShowWindow(wnd, monitor ? SW_MAXIMIZE : SW_SHOW);
		UpdateWindow(wnd);
	}


	window::window(int width, int height, const char * title,
		std::optional<int> monitor) :
		window(CW_USEDEFAULT, CW_USEDEFAULT, width, height, title, monitor)
	{
	}


	window::window(int x, int y, int width, int height, const char * title,
		std::initializer_list<int> pf_attribs,
		std::initializer_list<int> context_attribs,
		std::optional<int> monitor, std::optional<int> affinity)// bool debug = false, int swap_group = -1, int swap_barrier = -1)
	{
		HINSTANCE instance = GetModuleHandle(NULL);

		if (monitor)                             // Attempt Fullscreen Mode?
		{
			int monitor_index = *monitor;

			DISPLAY_DEVICE device;
			device.cb = sizeof(DISPLAY_DEVICE);

			unsigned int i = 0;
			while (EnumDisplayDevices(NULL, i++, &device, 0))
			{
				if ((device.StateFlags & DISPLAY_DEVICE_ACTIVE) && (monitor_index-- == 0))
					break;
			}

			if (monitor_index != -1)
				throw std::runtime_error("EnumDisplayDevices failed!");

			DEVMODE mode;						// Device Mode
			memset(&mode, 0, sizeof(DEVMODE));       // Makes Sure Memory's Cleared
			mode.dmSize = sizeof(DEVMODE);       // Size Of The Devmode Structure
			mode.dmPelsWidth = width;            // Selected Screen Width
			mode.dmPelsHeight = height;           // Selected Screen Height
			mode.dmBitsPerPel = 32;             // Selected Bits Per Pixel
			mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (ChangeDisplaySettingsEx(device.DeviceName, &mode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
				throw std::runtime_error("ChangeDisplaySettingsEx failed!");

			DEVMODE current;
			memset(&current, 0, sizeof(DEVMODE));
			current.dmSize = sizeof(DEVMODE);

			if (!EnumDisplaySettingsEx(device.DeviceName, ENUM_CURRENT_SETTINGS, &current, EDS_ROTATEDMODE))
				throw std::runtime_error("EnumDisplaySettingsEx failed!");

			x = current.dmPosition.x;
			y = current.dmPosition.y;

			ShowCursor(FALSE);
		}


		//https://blogs.msdn.microsoft.com/oldnewthing/20050505-04/?p=35703/
		wnd = CreateWindow(
			"igclass",							// Class Name
			title,								// Window Title
			WS_POPUP | WS_VISIBLE,				// Required Window Style
			x, y, width, height,				// Calculate Window Height
			NULL,								// No Parent Window
			NULL,								// No Menu
			instance,							// Instance
			this);
		if (!wnd)
			throw std::runtime_error("CreateWindow failed!");

		SetWindowLongPtr(wnd, GWLP_USERDATA, NULL);// (LONG_PTR)this);

		dc = GetDC(wnd);

		// https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
			32,                        //Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			0,                        //Number of bits for the depthbuffer
			0,                        //Number of bits for the stencilbuffer
			0,                        //Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		int pf = ChoosePixelFormat(dc, &pfd);
		if (pf == 0)
			throw std::runtime_error("ChoosePixelFormat failed!");

		if (!SetPixelFormat(dc, pf, &pfd))
			throw std::runtime_error("SetPixelFormat");

		HGLRC dummy_rc = wglCreateContext(dc);
		if (!dummy_rc)
			throw std::runtime_error("wglCreateContext failed");

		if (!wglMakeCurrent(dc, dummy_rc))
			throw std::runtime_error("wglMakeCurrent failed");


		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (!wglCreateContextAttribs)
			throw std::runtime_error("wglCreateContextAttribsARB not supported");

		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
		if (!wglChoosePixelFormat)
			throw std::runtime_error("wglChoosePixelFormatARB not supported");

		PFNWGLENUMGPUSNVPROC wglEnumGpus = (PFNWGLENUMGPUSNVPROC)wglGetProcAddress("wglEnumGpusNV");
		PFNWGLCREATEAFFINITYDCNVPROC wglCreateAffinityDC = (PFNWGLCREATEAFFINITYDCNVPROC)wglGetProcAddress("wglCreateAffinityDCNV");
		PFNWGLDELETEDCNVPROC wglDeleteDC = (PFNWGLDELETEDCNVPROC)wglGetProcAddress("wglDeleteDCNV");
		PFNWGLENUMGPUSFROMAFFINITYDCNVPROC wglEnumGpusFromAffinityDC = (PFNWGLENUMGPUSFROMAFFINITYDCNVPROC)wglGetProcAddress("wglEnumGpusFromAffinityDCNV");

		wglMakeCurrent(dc, NULL);
		wglDeleteContext(dummy_rc);

		ReleaseDC(wnd, dc);
		DestroyWindow(wnd);

		//https://blogs.msdn.microsoft.com/oldnewthing/20050505-04/?p=35703/
		wnd = CreateWindow(
			"igclass",							// Class Name
			title,								// Window Title
			WS_POPUP | WS_VISIBLE,				// Required Window Style
			x, y, width, height,				// Calculate Window Height
			NULL,								// No Parent Window
			NULL,								// No Menu
			instance,							// Instance
			this);
		if (!wnd)
			throw std::runtime_error("CreateWindow failed!");

		SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR)this);

		dc = GetDC(wnd);

		int pixelFormat;
		UINT numFormats;

		// Query for a pixel format that fits the attributes we want.
		if (!wglChoosePixelFormat(dc, pf_attribs.begin(), NULL, 1, &pixelFormat, &numFormats))
			throw std::runtime_error("wglChoosePixelFormatARB failed");

		// If the video card/display can handle our desired pixel format then we set it as the current one.
		if (!SetPixelFormat(dc, pixelFormat, &pfd))
			throw std::runtime_error("SetPixelFormat failed");


		if (affinity)
		{
			if ((!wglEnumGpus) || (!wglCreateAffinityDC))
				throw std::runtime_error("WGL_NV_gpu_affinity extension is not supported.");

			HGPUNV gpu;
			if (!wglEnumGpus(*affinity, &gpu))
				throw std::runtime_error("wglEnumGpus failed!");

			std::cout << title << ": creating affinity dc for gpu " << *affinity << " " << gpu << std::endl;

			HGPUNV gpu_list[] = { gpu, nullptr };
			HDC affinity_dc = wglCreateAffinityDC(gpu_list);
			if (!affinity_dc)
				throw std::runtime_error("wglCreateAffinityDCNV failed!");

			int pixelFormat;
			UINT numFormats;

			// Query for a pixel format that fits the attributes we want.
			if (!wglChoosePixelFormat(affinity_dc, pf_attribs.begin(), NULL, 1, &pixelFormat, &numFormats))
				throw std::runtime_error("wglChoosePixelFormatARB failed");
						
			// If the video card/display can handle our desired pixel format then we set it as the current one.
			if (!SetPixelFormat(affinity_dc, pixelFormat, &pfd))
				throw std::runtime_error("SetPixelFormat failed");

			rc = wglCreateContextAttribs(affinity_dc, NULL, context_attribs.begin());
			if (!rc)
				throw std::runtime_error("wglCreateContextAttribs failed!");

			wglDeleteDC(affinity_dc); 
		}
		else
		{
			rc = wglCreateContextAttribs(dc, NULL, context_attribs.begin());
			if (!rc)
				throw std::runtime_error("wglCreateContextAttribs failed!");
		}

		ShowWindow(wnd, (monitor != -1) ? SW_MAXIMIZE : SW_SHOW);
		UpdateWindow(wnd);
	}


	window::window(int width, int height, const char * title,
		std::initializer_list<int> pf_attribs,
		std::initializer_list<int> context_attribs,
		std::optional<int> monitor, std::optional<int> affinity) : 
		window(CW_USEDEFAULT, CW_USEDEFAULT, width, height, title, pf_attribs, context_attribs, monitor, affinity)
	{
	}


	window::window(window && other) noexcept :
		wnd(other.wnd),
		dc(other.dc),
		rc(other.rc)
	{
		if (wnd)
			SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR)this);

		other.wnd = NULL;
		other.dc = NULL;
		other.rc = NULL;
	}

	window& window::operator = (window&& other) noexcept
	{
		std::swap(wnd, other.wnd);
		std::swap(dc, other.dc);
		std::swap(rc, other.rc);

		if (wnd)
			SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR)this);
		if (other.wnd)
			SetWindowLongPtr(other.wnd, GWLP_USERDATA, (LONG_PTR)&other);

		return *this;
	}


	window::~window()
	{
		if (wnd)
		{
			if (rc)
			{
				wglMakeCurrent(dc, NULL);
				wglDeleteContext(rc);
			}

			if (dc)
			{
				ReleaseDC(wnd, dc);
			}

			DestroyWindow(wnd);
		}
	}


	void window::swap_buffers()
	{
		SwapBuffers(dc);
	}


	std::pair<int, int> window::size() const 
	{ 
		RECT rect;

		GetClientRect(wnd, &rect);
		int _width = rect.right - rect.left;
		int _height = rect.bottom - rect.top;

		return std::make_pair(_width, _height); 
	}
}

#endif
