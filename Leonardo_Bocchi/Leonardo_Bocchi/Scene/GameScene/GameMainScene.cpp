#include "GameMainScene.h"
#include "DxLib.h"


GameMainScene::GameMainScene()
{
}

GameMainScene::~GameMainScene()
{
}

void GameMainScene::Initialize()
{
}

eSceneType GameMainScene::Update()
{
	return __super::Update();
}

void GameMainScene::Draw() const
{
	__super::Draw();
	DrawFormatString(10, 10, GetColor(255, 255, 255), "ƒƒCƒ“‰æ–Ê");
}

void GameMainScene::Finalize()
{
}

eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}
