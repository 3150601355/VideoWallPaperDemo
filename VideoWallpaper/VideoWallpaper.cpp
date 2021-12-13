#include <windows.h>
#include <stdio.h>

/*
	这是在Win10中实现动态壁纸的简单demo，旨在用简洁的代码演示其原理
	本例子有一定门槛，不可以直接运行，其中有些代码要根据实际情况修改
	一定要正确安装开源软件ffplay

	请保留如下消息：
	作者：偶尔有点小迷糊 (https://space.bilibili.com/39665558)
	粉丝：23万

	讲解视频：https://www.bilibili.com/video/BV1HZ4y1978a
*/

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		// 找它的下一个窗口，类名为WorkerW，隐藏它
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, SW_HIDE);

		return FALSE;
	}
	return TRUE;
}

int main(int argc, char *argv[])
{
	// 视频路径、1920和1080，要根据实际情况改。建议使用GetSystemMetrics函数获取分辨率属性
	LPCWSTR lpParameter = L" F:\\桌面\\动态壁纸Demo\\视频.mp4  -noborder -x 1920 -y 1080  -loop 0";
	STARTUPINFO si{ 0 };
	PROCESS_INFORMATION pi{ 0 };

	// 下面是我电脑上ffplay的路径，要根据实际情况改
	if (CreateProcess(L"F:\\免安装程序\\ffmpeg-20200523\\bin\\ffplay.exe", (LPWSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		Sleep(200);												// 等待视频播放器启动完成。可用循环获取窗口尺寸来代替Sleep()

		HWND hProgman = FindWindow(L"Progman", 0);				// 找到PM窗口
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// 给它发特殊消息
		HWND hFfplay = FindWindow(L"SDL_app", 0);				// 找到视频窗口
		SetParent(hFfplay, hProgman);							// 将视频窗口设置为PM的子窗口
		EnumWindows(EnumWindowsProc, 0);						// 找到第二个WorkerW窗口并隐藏它
	}

	return 0;
}
