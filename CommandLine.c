#include "CommandLine.h" 
#include "utils.h"

static LRESULT WINAPI childWndProc(HWND, UINT, WPARAM, LPARAM);



WCHAR COMMAND_LINE_CLASS_NAME[] = L"CommandParser";


buffer commandBuffer;
char* commandFront;



HWND createCommandLine(HINSTANCE hInstance, HWND hParent) {
	
	DWORD childClassStyle = CS_VREDRAW | CS_HREDRAW;
	WNDCLASSEX childWindClass = {
		.cbSize = sizeof(WNDCLASSEX),
		/* Win 3.x */
		.style = childClassStyle,
		.lpfnWndProc = childWndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = LoadIcon(NULL, IDI_APPLICATION),
		.hCursor = LoadCursor(NULL,IDC_IBEAM),
		.hbrBackground = GetStockObject(WHITE_BRUSH),
		.lpszMenuName = NULL,
		.lpszClassName = COMMAND_LINE_CLASS_NAME,
		/* Win 4.0 */
		.hIconSm = LoadIcon(NULL,IDI_APPLICATION)
	};
	ATOM childClassRegistered = RegisterClassEx(&childWindClass);
	cdAssert(childClassRegistered != FALSE, "Could not register command bar\n");

	RECT parentRectangle;
	GetWindowRect(hParent, &parentRectangle);
	LONG parentWidth  = (LONG)parentRectangle.right - parentRectangle.left;
	LONG parentHeight = (LONG)parentRectangle.top - parentRectangle.bottom;

	DWORD exChildStyles = 0;
	DWORD childStyles = WS_CHILD | WS_BORDER;
	HWND hCommandWindow = CreateWindowExW(exChildStyles, COMMAND_LINE_CLASS_NAME,
		L"CommandParser", childStyles,
		//Position
		0, 550,
		//Size
		parentWidth, 20,
		hParent, NULL, hInstance, NULL);
	cdAssert(hCommandWindow != NULL, "Could not create command bar\n");

	commandBuffer = cdCreateBuffer(256);
	commandFront = commandBuffer;




	return hCommandWindow;
}



LRESULT WINAPI childWndProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_CREATE:

		break;
	default:
		return DefWindowProc(hWindow, uMsg, wParam, lParam);
	}
	return 0;
}