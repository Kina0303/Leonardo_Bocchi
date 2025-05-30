#include "ResultScene.h"
#include <DxLib.h>
#include "../../Utility/InputControl.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

eSceneType ResultScene::Update()
{
	InputControl* input = InputControl::GetInstance();

	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		// Aボタンが押されたらゲームメインシーンに移行
		return eSceneType::TITLE;
	}
	return __super::Update();
}

void ResultScene::Draw() const
{
	DrawFormatString(10, 10, GetColor(255, 255, 255), "Result Scene");
}

void ResultScene::Finalize()
{
}

eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::RESULT;
}

