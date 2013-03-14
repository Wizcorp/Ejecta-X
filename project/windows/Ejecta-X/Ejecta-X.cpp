// Ejecta-X.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Ejecta-X.h"

#define MAX_LOADSTRING 100
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 960

// 全局变量:
HINSTANCE hInst;								// 当前实例
HWND g_hWnd;
HDC g_hDC;
HGLRC g_hRC;
WNDCLASS g_windClass; 
RECT g_windowRect;
bool g_ContinueRendering;
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void SetupRC()
{
	//glShadeModel(GL_SMOOTH);// Enable Smooth Shading
	//glClearColor(0.0f, 0.0f, 1.0f, 0.5f);// Black Background
	//glClearDepth(1.0f);// Depth Buffer Setup
	//glEnable(GL_DEPTH_TEST);// Enables Depth Testing
	//glDepthFunc(GL_LEQUAL);// The Type Of Depth Testing To Do
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// Really Nice Perspective Calculations

	const char *nativeString = ".";
	EJApp::instance()->init(nativeString, SCREEN_WIDTH, SCREEN_HEIGHT);

}

void RenderScene(void)
{
	if(!g_ContinueRendering)
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear Screen And Depth Buffer
	glClearColor(0.0f, 0.0f, 1.0f, 0.5f);// Black Background

	//glBegin(GL_TRIANGLES);
	//glVertex2f(0.5, 0.5);
	//glVertex2f(0.0, 0.5);
	//glVertex2f(0.5, 0.0);
	//glEnd();

	EJApp::instance()->run();


	// Flush drawing commands
	SwapBuffers(g_hDC);
}

void ChangeSize(int w, int h)
{
	//glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION);// Select The Projection Matrix
	//glLoadIdentity();// Reset The Projection Matrix
	//// Calculate The Aspect Ratio Of The Window
	//glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	//glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	//glLoadIdentity();

	EJApp::instance()->setScreenSize(w, h);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;


	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EJECTAX, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EJECTAX));

	DWORD lastTime = GetTickCount();

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		DWORD curTime = GetTickCount();
		float deltaTime = (float)( curTime - lastTime ) / 1000.0f;
		lastTime = curTime;

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Call update function if registered
		if (g_hRC)
		{
			RenderScene();
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EJECTAX));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= _T("MENU");//MAKEINTRESOURCE(IDC_EJECTAX);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	bool bRetVal = true;
	int nPixelFormat  = -1;
	PIXELFORMATDESCRIPTOR pfd = 	{
		sizeof(PIXELFORMATDESCRIPTOR), // nSize
		1, // nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // dwFlags
		PFD_TYPE_RGBA, // iPixelType
		24, // cColorBits
		0, 0, 0, 0, 0, 0, // Not used
		8, // cAlphaBits
		0, // Not used
		0, // cAccumBits
		0, 0, 0, 0, // Not used
		16, // cDepthBits
		0, // cStencilBits
		0, // cAuxBuffers
		PFD_MAIN_PLANE, // iLayerType
		0, 0, 0, 0, // Not used
	};

   hInst = hInstance; // 将实例句柄存储在全局变量中

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

   // now that we have a window, setup the pixel format descriptor
   g_hDC = GetDC(g_hWnd);

   int nPixCount = 0;

   // Specify the important attributes we care about
   int pixAttribs[] = { 
	   WGL_DRAW_TO_WINDOW_ARB,GL_TRUE, 
	   WGL_SUPPORT_OPENGL_ARB,GL_TRUE, 
	   WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB, 
	   WGL_COLOR_BITS_ARB,16, 
	   WGL_DEPTH_BITS_ARB,16, 
	   WGL_DOUBLE_BUFFER_ARB,GL_TRUE, 
	   WGL_SAMPLE_BUFFERS_ARB,GL_TRUE, 
	   0
   }; // NULL termination

   // Ask OpenGL to find the most relevant format matching our attribs
   // Only get one format back.
   nPixelFormat = ChoosePixelFormat(g_hDC, &pfd);
   //wglChoosePixelFormatARB(g_hDC, &pixAttribs[0], NULL, 1, &nPixelFormat, (UINT*)&nPixCount);

   if(nPixelFormat == -1) 
   {
	   // Couldn't find a format, perhaps no 3D HW or drivers are installed
	   g_hDC = 0;
	   g_hDC = 0;
	   bRetVal = false;
	   printf("!!! An error occurred trying to find a pixel format with the requested attribs.\n");
   }
   else
   {
	   // Got a format, now set it as the current one
	   BOOL bset = SetPixelFormat( g_hDC, nPixelFormat, &pfd );

	   GLint attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,  3,
		   WGL_CONTEXT_MINOR_VERSION_ARB,  1,
		   0 };

	   g_hRC = wglCreateContext(g_hDC);
	   if (g_hRC == NULL)
	   {
		   printf("!!! Could not create an OpenGL 3.1 context.\n");
	   }

	   wglMakeCurrent( g_hDC, g_hRC );
   }

   if (g_hDC == 0 ||
	   g_hDC == 0)
   {
	   bRetVal = false;
	   printf("!!! An error occured creating an OpenGL window.\n");
   }

   // Setup GLEW which loads OGL function pointers
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
	   /* Problem: glewInit failed, something is seriously wrong. */
	   bRetVal = false;
	   printf("Error: %s\n", glewGetErrorString(err));
   }
   const GLubyte *oglVersion = glGetString(GL_VERSION);
   printf("This system supports OpenGL Version %s.\n", oglVersion);

   if( bRetVal )
   {
	   ShowWindow( g_hWnd, nCmdShow );
	   UpdateWindow(g_hWnd);
	   SetForegroundWindow( g_hWnd );
	   SetFocus( g_hWnd );
	   SetupRC();
	   g_ContinueRendering = true;
   }

   return bRetVal;
}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			//Cleanup OGL RC
			
			EJApp::finalize();

			if(g_hRC) 
			{
				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(g_hRC);
				g_hRC = NULL;
			}

			// release the DC
			if(g_hDC)
			{
				ReleaseDC(g_hWnd, g_hDC);
				g_hDC = NULL;
			}

			DestroyWindow(hWnd);

			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		{

			if (g_hRC)
			{
				RenderScene();
			}

			//ValidateRect( _esContext->hWnd, NULL );
		}
		//EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		ChangeSize(LOWORD(lParam),HIWORD(lParam));
		RenderScene();
		break;
	case WM_DESTROY:
		g_ContinueRendering = FALSE;
		PostQuitMessage(0);
		break;

	case WM_CHAR:
		//{
		//	POINT      point;
		//	ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );

		//	GetCursorPos( &point );

		//	if ( esContext && esContext->keyFunc )
		//		esContext->keyFunc ( esContext, (unsigned char) wParam, 
		//		(int) point.x, (int) point.y );
		//}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
