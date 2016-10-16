//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
GeneralConfiguration general_configuration;
Engine *g_engine = NULL;

//-----------------------------------------------------------------------------
// WindowProc for handling Windows messages.
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_ACTIVATEAPP:
		g_engine->SetDeactiveFlag(!wparam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(wnd, msg, wparam, lparam);
	}
}

//-----------------------------------------------------------------------------
// Engine
//-----------------------------------------------------------------------------
Engine::Engine(const EngineSetup *setup) {
	// Indicate that the engine is not yet loaded.
	m_loaded = false;

	// If no setup structure was passed in, then create a default one.
	// Otehrwise, make a copy of the passed in structure.
	m_setup = (setup) ? *setup : EngineSetup();
	
	// Store a pointer to the engine in a global variable for easy access.
	g_engine = this;

	// Prepare and register the window class.
	//-----------------------------------------------------------------------------
	// Structure ontaining window class information. 
	WNDCLASSEX wcex;
	// The size, in bytes, of this structure.
	wcex.cbSize				= sizeof(WNDCLASSEX);
	// The class style(s)
	// CS_CLASSDC:	Allocates one device context to be shared by all windows in the class. 
	//				Because window classes are process specific, it is possible for multiple 
	//				threads of an application to create a window of the same class. 
	//				It is also possible for the threads to attempt to use the device context simultaneously. 
	//				When this happens, the system allows only one thread to successfully finish its drawing operation.
	wcex.style				= CS_CLASSDC;
	// A pointer to the window procedure.
	wcex.lpfnWndProc		= WindowProc;
	// The number of extra bytes to allocate following the window-class structure.
	wcex.cbClsExtra			= 0;
	// The number of extra bytes to allocate following the window instance.
	wcex.cbWndExtra			= 0;
	//A handle to the instance that contains the window procedure for the class.
	wcex.hInstance			= m_setup.m_instance;
	// A handle to the class icon. This member must be a handle to an icon resource.
	wcex.hIcon				= LoadIcon(NULL, IDI_APPLICATION);
	// A handle to the class cursor. This member must be a handle to a cursor resource.
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	// A handle to the class background brush. This member can be a handle to
	// the brush to be used for painting the background, or it can be a color value.
	wcex.hbrBackground		= NULL;
	// Pointer to a null-terminated character string that specifies the resource name 
	// of the class menu, as the name appears in the resource file. 
	// If this member is NULL, windows belonging to this class have no default menu.
	wcex.lpszMenuName		= NULL;
	// A pointer to a null-terminated string or is an atom.
	// If lpszClassName is a string, it specifies the window class name.
	wcex.lpszClassName		= L"WindowClass";
	// A handle to a small icon that is associated with the window class.
	wcex.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	// Registers a window class
	RegisterClassEx(&wcex);
	//-----------------------------------------------------------------------------

	// Initializes the COM library for use by the calling thread 
	// and sets the thread's concurrency model to multithreaded concurrency.
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// Creates the window and retrieve a handle to it.
	// Note: Later the window will be created using a windowed/fullscreen flag.
	m_window = CreateWindow(L"WindowClass", m_setup.m_name.c_str(), WS_OVERLAPPED, 0, 0, 800, 600, NULL, NULL, m_setup.m_instance, NULL);

	// Seed the random number generator with the current time.
	srand(timeGetTime());

	// The engine is fully loaded and ready to go.
	m_loaded = true;
}

Engine::~Engine() {
	// Ensure the engine is loaded.
	if (m_loaded) {
		
	}

	// Uninitialise the COM.
	CoUninitialize();

	// Unregister the window class.
	UnregisterClass(L"WindowClass", m_setup.m_instance);
}

// Enters the engine into the main processing loop.
void Engine::Run() {
	// Ensure the engine is loaded.
	if (m_loaded) {
		// Activates the window and displays it in its current size and position.
		ShowWindow(m_window, SW_NORMAL);

		Timer timer;
		timer.Start();

		// Enter the message loop.
		MSG msg;
		SecureZeroMemory(&msg, sizeof(MSG));
		while (msg.message != WM_QUIT) {
			// Retrieves messages for any window that belongs to the current thread
			// without performing range filtering. Furthermore messages are removed
			// after processing.
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				// Translates virtual-key messages into character messages.
				TranslateMessage(&msg);
				// Dispatches a message to a window procedure.
				DispatchMessage(&msg);
			}
			else if (!m_deactive) {
				const double elapsed = timer.Time();
				timer.Reset();
				timer.Start();
			}
		}
	}

	// Destroy the engine.
	delete g_engine;
}

//-----------------------------------------------------------------------------
// Entry point for the application.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int cmdShow) {
	// Create the engine setup structure.
	EngineSetup setup;
	setup.m_instance = instance;
	setup.m_name = L"Framework Test";

	// Create the engine, then run it.
	new Engine(&setup);
	g_engine->Run();

	return true;
}