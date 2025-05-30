#include "TitleScene.h"
#include "DxLib.h"
#include "../../Utility/InputControl.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
}

eSceneType TitleScene::Update()
{
	InputControl* input = InputControl::GetInstance();

	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		// Aボタンが押されたらゲームメインシーンに移行
		return eSceneType::GAME_MAIN;
	}
	return __super::Update();
}

void TitleScene::Draw() const
{
	DrawFormatString(10, 10, GetColor(255, 255, 255), "Title Scene");
}

void TitleScene::Finalize()
{
}

eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::TITLE;
}
