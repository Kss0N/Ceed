#include "framework.h"
#include "resource.h"
#include "utils.h"
#include "CommandLine.h"
#include "shader.h"


LRESULT WINAPI wndProc(HWND, UINT, WPARAM, LPARAM);


unsigned int width = 800;
unsigned int height = 640;


#define MAX_STRING 32
#define INITIAL_BUFFER_SIZE 1024




int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPWSTR cmdLine, 
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	WCHAR title[MAX_STRING];
	WCHAR winClassName[MAX_STRING];
	
	LoadString(hInstance, (IDS_Title), title,	MAX_STRING);
	LoadString(hInstance, (IDS_mainWinClass), winClassName, MAX_STRING);



	/*Register window class*/
	UINT wndClassStyle = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_PARENTDC;
	WNDCLASSEX wc = {
		.cbSize = sizeof(WNDCLASSEX),
	/* Win 3.x */
		.style = wndClassStyle,
		.lpfnWndProc = wndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,

		.hInstance = hInstance,
		.hIcon		= LoadIcon(NULL, (IDI_APPLICATION)),
		.hCursor	= LoadCursor(NULL, (IDC_ARROW)),
		.hbrBackground = (HBRUSH)COLOR_WINDOW + 1,
		.lpszMenuName = NULL,
		.lpszClassName = winClassName,
	/* Win 4.0 */
		.hIconSm = LoadIcon(NULL, (IDI_APPLICATION))
	};
	cdAssert(RegisterClassEx(&wc), "Failed to register class.");



	/*Create Window*/
	DWORD exStyle = 0L;
	DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	HWND hWindow = CreateWindowEx(exStyle, winClassName, title, style,
		/*Begin Position*/
		CW_USEDEFAULT, CW_USEDEFAULT,
		/*Begin Size*/
		width, height,
		NULL, NULL,			//No Parent, Menu
		hInstance, NULL);	//No extra params
	cdAssert(hWindow != NULL, "Failed to create window.");



	/*Init OpenGL*/
	HDC hDC= GetDC(hWindow);
	DWORD pfdFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	PIXELFORMATDESCRIPTOR pfd = {
		.nSize = sizeof(PIXELFORMATDESCRIPTOR),
		.nVersion = 1,
		.dwFlags = pfdFlags,
		.iPixelType = PFD_TYPE_RGBA,
		.cColorBits = 32,

		.cRedBits	= 0, .cRedShift		= 0,
		.cGreenBits = 0, .cGreenShift	= 0,
		.cBlueBits  = 0, .cBlueShift	= 0,
		.cAlphaBits = 0, .cAlphaShift	= 0,

		.cAccumBits = 0,
		.cAccumRedBits = 0, .cAccumGreenBits = 0, .cAccumBlueBits = 0, 
		.cAccumAlphaBits = 0,

		.cDepthBits = 24,
		.cStencilBits = 8,
		.cAuxBuffers = 0,

		.iLayerType = 0, //Ignored
		.bReserved = 0,
		.dwLayerMask = 0, //Ignored
		.dwVisibleMask = 0,
		.dwDamageMask = 0 //Ignored
	};
	int format = ChoosePixelFormat(hDC, &pfd);
	cdAssert(format != 0, "Failed to Choose pixelformat");
	SetPixelFormat(hDC, format, &pfd);
	HGLRC renderingContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, renderingContext);
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);


	/*init utils*/
	buffer buffer = cdCreateBuffer(INITIAL_BUFFER_SIZE);


	/*init main OpenGL objects*/
	GLuint Shader = cdCreateShader("default.vert", NULL, "default.frag", &buffer);


	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



	/*Create Command bar*/
	HWND hCommandWindow = createCommandLine(hInstance, hWindow);
	
	
	ShowWindow(hWindow, nCmdShow);
	ShowWindow(hCommandWindow, nCmdShow);
	UpdateWindow(hWindow);
	UpdateWindow(hCommandWindow);



	MSG msg;
	while (1) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		glUseProgram(Shader);



		

		SwapBuffers(hDC);
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
			BOOL success = GetMessage(&msg, NULL, 0, 0);
			if (success  == FALSE) 
				goto SUCCESSFUL_EXIT;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
SUCCESSFUL_EXIT:

	cdRemoveAllResources();
	glDeleteShader(Shader);
	cdFreeBuffer(&buffer);

	return 0;
}



LRESULT WINAPI wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{







	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

