#include "DxLib.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)

	{

		return -1;

	}
	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() != -1)

	{
		ClearDrawScreen();

		DrawCircle(320, 240, 15, GetColor(255, 255, 255), TRUE);

		ScreenFlip();

	}

	DxLib_End();

	return 0;

}