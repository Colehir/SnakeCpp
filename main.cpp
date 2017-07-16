#include <windows.h>
#include <iostream>
#include <time.h>
#include <thread>
#include <vector>
#include <unordered_set>

#define pixel 20
#define hig 600
#define wid 600
#define fastCheck


//http://www.winprog.org/tutorial/simple_window.html


const wchar_t g_szClassName[] = L"myWindowClass";

struct point
{
	int x;
	int y;
};

struct food
{
	int x;
	int y;
	bool ava;
};

struct snake
{
	int x, y;
	int direction; //1 = up, 2 = right, 3 = down, 4 = left
	std::vector<point> vPoints;
	std::unordered_set<int> sPoints;
};

bool run = true,gameOver=false,start=false,end=false,pause=false;
snake s1;
int score;
food f;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hGreenPen;
	HPEN hWhitePen;
	HPEN hRedPen;
	HBRUSH bKBrush;
	HBRUSH hGreenBrush;
	HBRUSH hRedBrush;
	int red, green, blue;
	HFONT hFont;

	switch (msg)
	{
	case WM_PAINT:
	{
			hdc = BeginPaint(hwnd, &ps);
			red = rand() % 250+5;
				green = rand() % 250+5;
				blue = rand() % 250+5;

			bKBrush = CreateSolidBrush(RGB(0, 0, 0));
			hGreenPen = CreatePen(PS_SOLID, 1, RGB(red, green, blue));
			hGreenBrush = CreateSolidBrush(RGB(red, green, blue));
			hWhitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
			hRedPen = CreatePen(PS_SOLID, 2, RGB(200, 0, 0));
			hRedBrush = CreateSolidBrush(RGB(200, 0, 0));

			SelectObject(hdc, bKBrush);

			Rectangle(hdc, -1, -1, 100000, 100000);
			DeleteObject(bKBrush);

			SelectObject(hdc, hWhitePen);
			Rectangle(hdc, 50, 50, wid-50, hig-50);

			SelectObject(hdc, hGreenPen);
			SelectObject(hdc, hGreenBrush);

			wchar_t szSize[100];
			wsprintf(szSize, L"Score: %d", score);
			hFont = CreateFont(20, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Arial");
			SelectObject(hdc, hFont);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 255, 255));
			RECT rect;
			rect.left = 50;
			rect.top = 0;
			rect.right = 560;
			rect.bottom = 50;
			DrawText(hdc, szSize, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			if (!gameOver)
			{
				Ellipse(hdc, (pixel*s1.x) + 50, (pixel*s1.y) + 50, pixel*(s1.x + 1) + 50, pixel*(s1.y + 1) + 50);
				SelectObject(hdc, hRedPen);
				SelectObject(hdc, hRedBrush);
				Ellipse(hdc, pixel*f.x + 50, pixel*f.y + 50, pixel*(f.x + 1) + 50, pixel*(f.y + 1) + 50);
				DeleteObject(hGreenPen);
				DeleteObject(hGreenBrush);
			}
			else
			{
				hFont = CreateFont(60, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Arial");
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(255, 255, 255));
				SelectObject(hdc, hFont);
				rect.left = 0;
				rect.top = 0;
				rect.bottom = hig;
				rect.right = wid;

				DrawText(hdc, L"GAME OVER!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			DeleteObject(hRedPen);
			DeleteObject(hRedBrush);

	}
		break;
	case WM_KEYDOWN:
		if (!pause)
		{
			switch (wParam)
			{
			case VK_UP:
				if (s1.direction != 3)s1.direction = 1;
				break;
			case VK_RIGHT:
				if (s1.direction != 4)s1.direction = 2;
				break;
			case VK_DOWN:
				if (s1.direction != 1)s1.direction = 3;
				break;
			case VK_LEFT:
				if (s1.direction != 2)s1.direction = 4;
				break;
			case VK_ESCAPE:
				pause = true;
				break;
			};
		}
		if (wParam == VK_RETURN)
		{
			start = true;
			pause = false;
		}
		break;
	case WM_SIZING:
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		end = true;
		start = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		end = true;
		run = false;
		start = true;
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void GameOver(HWND hwnd) {
	gameOver = true;
	run = false;
	MessageBox(NULL, L"You Died!", L"Game Over!", MB_ICONEXCLAMATION | MB_OK);
	InvalidateRect(hwnd, NULL, false);
}

void getFood()
{
	f.x = ((rand() % ((wid-100-pixel) / pixel)));
	f.y = ((rand() % ((hig-100-pixel) / pixel)));
#ifdef fastCheck
 // !fastCheck
	if (s1.sPoints.find(f.x * 100 + f.y) != s1.sPoints.end())
	{
		getFood();
		return;
	}
#else
	for each(point po in s1.vPoints)
	{
		if (po.x == f.x&&po.y == f.y)
		{
			getFood();
			return;
		}
	}
#endif
	f.ava = true;
}

void calculateNext(HWND hwnd)
{
	point p;
	p.x = s1.x;
	p.y = s1.y;
	#ifdef fastCheck
	if (s1.sPoints.find(p.x * 100 + p.y) != s1.sPoints.end())
	{
		GameOver(hwnd);
		return;
	}
	#else
	for each(point po in s1.vPoints)
	{
		if (po.x == p.x&&po.y == p.y)
		{
			GameOver(hwnd);
			return;
		}
	}
	#endif
	s1.vPoints.push_back(p);
	s1.sPoints.insert(p.x * 100 + p.y);
	switch (s1.direction)
	{
	case 1:
		s1.y -= 1;
		if (s1.y < 0)
		{
			GameOver(hwnd);
			return;
		}
		break;
	case 2:
		s1.x += 1;
		if (s1.x > (double)((wid-100) / pixel)-1)
		{
			GameOver(hwnd);
			return;
		}
		break;
	case 3:
		s1.y += 1;
		if (s1.y > (double)((hig-100) / pixel)-1)
		{
			GameOver(hwnd);
			return;
		}
		break;
	case 4:
		s1.x -= 1;
		if (s1.x < 0)
		{
			GameOver(hwnd);
			return;
		}
		break;
	};
	RECT rect;
	if (f.x == s1.x&&f.y == s1.y)
	{
		f.ava = false;
		score += 1;
	}
	if (f.ava == false)
	{
		getFood();
		rect.left = f.x*pixel+50;
		rect.top = f.y*pixel+50;
		rect.right = (f.x + 1)*pixel+50;
		rect.bottom = (f.y + 1)*pixel+50;
		InvalidateRect(hwnd, &rect, true);
		rect.left = 10;
		rect.top = 0;
		rect.right = 600;
		rect.bottom = 50;
		InvalidateRect(hwnd, &rect, true);
	}
	else
	{
		rect.left = (s1.vPoints[0].x*pixel)+50;
		rect.top = (s1.vPoints[0].y*pixel)+50;
		rect.right = ((s1.vPoints[0].x + 1)*pixel)+50;
		rect.bottom = ((s1.vPoints[0].y + 1)*pixel)+50;
		InvalidateRect(hwnd, &rect, true);
		s1.sPoints.erase(s1.vPoints[0].x*100+ s1.vPoints[0].y);
		s1.vPoints.erase(s1.vPoints.begin());
	}
	rect.left = (s1.x*pixel)+50;
	rect.top = (s1.y*pixel)+50;
	rect.right = ((s1.x + 1)*pixel)+50;
	rect.bottom = ((s1.y + 1)*pixel)+50;
	InvalidateRect(hwnd, &rect, true);
}

void timer(HWND hwnd)
{
	s1.direction = (rand() % 4) + 1;
	s1.x = 5;
	s1.y = 5;
	point p;
	p.x = 4;
	p.y = 4;
	score = 0;
	s1.vPoints.push_back(p);
	s1.sPoints.insert(p.x * 100 + p.y);
	getFood();
	f.ava = true;
	InvalidateRect(hwnd, NULL, true);
	while (run)
	{
		calculateNext(hwnd);
		do
		{
			Sleep(90);
		} while (pause);
	}
	

}

void gameController(HWND hwnd)
{
	while (!end)
	{
		while (!start) //wait for start command
		{
			Sleep(90);
		}
		
		Sleep(100);
		timer(hwnd);//start game
		start = false;
		while (!start) //wait for start command
		{
			Sleep(90);
		}
		run = true;
		gameOver = false;
		s1.vPoints.clear();
		s1.sPoints.clear();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(NULL));
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"SNAKE",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wid, hig,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	std::thread snake(gameController, hwnd);

	// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	snake.join();

	return Msg.wParam;
}
