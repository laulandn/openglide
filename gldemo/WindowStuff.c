

#ifdef WIN32
//Window handler
long WINAPI WindowMessageHandler (HWND hWnd, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
  //Close window
  if (wMessage == WM_CLOSE) Done = -1;
  //Other messages
  return DefWindowProc (hWnd, wMessage, wParam, lParam);
}
#endif

/*
  Registers a new window class and creates an empty window.
*/
void InitWindow ()
{
#ifdef WIN32
  //Window class
  WNDCLASS  wndclass;

  //Register window class, as standard as can be
  wndclass.style = 0;
  wndclass.lpfnWndProc = (WNDPROC)WindowMessageHandler;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = InstanceHandle;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = GetStockObject (LTGRAY_BRUSH);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = (LPSTR)"GlideDemo";
  RegisterClass (&wndclass);

  //Create empty window
  WindowHandle = 
  CreateWindow ((LPSTR)"GlideDemo",           //Class name
                (LPSTR)"Glide demonstration", //Window name
                WS_OVERLAPPED,                //Window type
                CW_USEDEFAULT, 0, 0, 0,       //Don't specify size and location
                NULL,                         //No parent window
                NULL,                         //No menu
                InstanceHandle,               //It belongs to this program
                NULL);                        //No other data

  //Display the window
  ShowWindow (WindowHandle, SW_SHOWMAXIMIZED);
#endif
}

