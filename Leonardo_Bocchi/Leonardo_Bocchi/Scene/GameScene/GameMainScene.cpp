#include "GameMainScene.h"
#include "DxLib.h"
#include "../../Object/Character/Player/Player.h"
#include "../../Object/Stage/Block.h"

#include <fstream>
#include <sstream>
#include <iostream>


GameMainScene::GameMainScene() :stage_width_num(0), stage_height_num(0), stage_data{ 0 }
{
}

GameMainScene::~GameMainScene()
{
}

void GameMainScene::Initialize()
{
	LoadStage();
}

eSceneType GameMainScene::Update()
{
	UpdateCamera();

	return __super::Update();
}

void GameMainScene::Draw() const
{
	//背景
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(150, 150, 150), TRUE); // 背景を黒で塗りつぶす
	__super::Draw();
	DrawFormatString(10, 10, GetColor(255, 255, 255), "メイン画面");
	DrawFormatString(10, 40, GetColor(255, 255, 255), "ステージサイズ：幅 = %d      高さ = %d\n", stage_width_num, stage_height_num);

}

void GameMainScene::Finalize()
{
}

eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}

void GameMainScene::LoadStage()
{
	std::ifstream file("Resource/file/stage.csv");

	if (!file) {
		std::cerr << "ファイルを開けませんでした: " << std::endl;
		return;
	}
	// 1行目を読み込んでステージ幅と高さを取得
	std::string line;

	//fileから1行読み込んで、line に格納。
	if (std::getline(file, line)) {
		//文字列を解析するためのストリームを作成。
		std::stringstream ss(line);
		std::string width, height;

		// カンマで分割して幅と高さを取得
		std::getline(ss, width, ',');
		std::getline(ss, height, ',');

		//文字列を整数に変換
		stage_width_num = std::stoi(width);   // ステージ幅
		stage_height_num = std::stoi(height); // ステージ高さ
	}

	//ステージデータの読み込み（CSVの2行目以降）
	for (int i = 0; i < stage_height_num; i++) {
		//1行ずつ読み込む
		if (std::getline(file, line)) {
			std::stringstream ss(line);
			for (int j = 0; j < stage_width_num; j++) {
				//カンマ区切りでデータを取得
				std::string value;
				if (std::getline(ss, value, ',')) {
					//文字列を整数に変換してステージデータに格納
					stage_data[i][j] = std::stoi(value);
				}
			}
		}
	}

	file.close();

	SetStage();
}

void GameMainScene::SetStage()
{
	//ステージデータを元にオブジェクトを生成
	for (int i = 0; i < stage_height_num; i++) {
		for (int j = 0; j < stage_width_num; j++) {
			//ブロックのY座標を計算	
			int y = 720 - ((stage_height_num - 1 - i) * BOX_SIZE);
			switch (stage_data[i][j])
			{
			case 0:
				break;
			case BLOCK:
				CreateObject<Block>(Vector2D(j * BOX_SIZE, y), Vector2D((float)BOX_SIZE));
				break;
			case PLAYER:
				CreateObject<Player>(Vector2D(j * BOX_SIZE, y), Vector2D(64.0f,96.0f));
				break;
			default:
				break;
			}
		}
	}
}

void GameMainScene::UpdateCamera()
{
	//プレイヤーの取得
	GameObject* player = nullptr;
	for (auto obj : objects)
	{
		if (obj->GetObjectType() == PLAYER)
		{
			player = obj;
			break;
		}
	}

	//プレイヤーが存在するならカメラを追従させる
	if (player)
	{
		float screen_half_width = SCREEN_WIDTH / 2;				//画面の半分の幅
		float stage_limit_left = 0.0f;							//ステージの左端
		float stage_limit_right = stage_width_num * BOX_SIZE - SCREEN_WIDTH; //ステージの右端 

		//カメラ位置 ＝ プレイヤーの位置 - 画面の半分の幅 
		camera_location.x = player->GetLocation().x - screen_half_width;

		//画面端ではスクロールしないよう制限
		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}
