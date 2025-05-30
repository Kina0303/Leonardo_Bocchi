#include "GameMainScene.h"
#include "DxLib.h"
#include "../../Object/ObjectList.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

GameMainScene::GameMainScene() :stage_width_num(0), stage_height_num(0), stage_data{ 0 }, player(nullptr), back_ground_image(0),clone_spawn_timer(0.0f),is_create(false)
{
}

GameMainScene::~GameMainScene()
{
}

void GameMainScene::Initialize()
{
	//ステージを読み込む
	LoadStage();

	//カメラの初期位置を設定
	camera_location = Vector2D(0.0f, 0.0f); //カメラの初期位置を設定

	back_ground_image = LoadGraph("Resource/Images/back_ground.png"); // 背景画像を読み込む

	//back_ground_img[0] = LoadGraph("Resource/Images/back_ground/Night/1.png"); // 背景画像を読み込む
	//back_ground_img[1] = LoadGraph("Resource/Images/back_ground/Night/2.png"); // 背景画像を読み込む
	//back_ground_img[2] = LoadGraph("Resource/Images/back_ground/Night/3.png"); // 背景画像を読み込む
	//back_ground_img[3] = LoadGraph("Resource/Images/back_ground/Night/4.png"); // 背景画像を読み込む
	//back_ground_img[4] = LoadGraph("Resource/Images/back_ground/Night/5.png"); // 背景画像を読み込む

}

eSceneType GameMainScene::Update()
{
	//ステージリロード	
	if (IsStageReload())
	{
		//ステージクリア時の処理
		StageClear();

		//ステージの再読み込み
		ReLoadStage();
	}
	//カメラ更新
	UpdateCamera();

	// プレイヤーがいない場合のみ探す
	if (!player)
	{
		FindPlayer();
	}


	if (!is_create)
	{
		clone_spawn_timer++;
		if (clone_spawn_timer >= 45)
		{
			CreateClone();
			clone_spawn_timer = 0;
			is_create = true;
		}
	}




	if (player->GetHp() <= 0 || player->GetLocation().y > 850.0f)
	{
		player->SetDelete();
		return eSceneType::RESULT;
	}

	return __super::Update();
}

void GameMainScene::Draw() const
{
	//背景
	//DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(150, 150, 150), TRUE); // 背景を黒で塗りつぶす
	DrawGraph(0, 0, back_ground_image, TRUE); // 背景画像を読み込む
	//DrawFormatString(10, 10, GetColor(255, 255, 255), "メイン画面");
	//DrawFormatString(10, 40, GetColor(255, 255, 255), "ステージサイズ：幅 = %d      高さ = %d\n", stage_width_num, stage_height_num);

	//for (int i = 0; i < 5; i++)
	//{
	//	DrawGraph(0, 0, back_ground_img[i], TRUE); // 背景画像を読み込む
	//}

	DrawFormatString(10, 70, GetColor(255, 255, 255), "LOOP : %d\n", clear_count);

	__super::Draw();

}

void GameMainScene::Finalize()
{
	DeleteGraph(back_ground_image);
	back_ground_image = 0;
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
	for (int i = 0; i < stage_height_num; ++i) {
		for (int j = 0; j < stage_width_num; ++j) {
			int x = j * BOX_SIZE;
			int y = 720 - ((stage_height_num - i) * BOX_SIZE);
			Vector2D pos(x, y);

			switch (stage_data[i][j]) {
			case EMPTY: break;
			case BLOCK: CreateObject<Block>(pos, Vector2D((float)BOX_SIZE)); break;
			case PLAYER: CreateObject<Player>(pos, Vector2D(48.0f, 96.0f)); break;
			case MOVE_BLOCK: CreateObject<MoveBlock>(pos, Vector2D((float)BOX_SIZE, 24.0f)); break;
			case GOAL: CreateObject<GoalPoint>(pos, Vector2D((float)BOX_SIZE * 2)); break;
			default: break;
			}
		}
	}

	CreateItem();
	CreateGimmick();


	//CreateClone();
}


void GameMainScene::UpdateCamera()
{
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

void GameMainScene::StageClear()
{
	//プレイヤーを取得
	Player* p = static_cast<Player*>(player);

	if (p){
		// プレイヤーの移動履歴を保存
		stage_clear_history.push_back(p->GetMoveHistory());
	}
}

void GameMainScene::ReLoadStage()
{
	//オブジェクトの削除
	for (auto obj : objects)
	{
		delete obj;
		obj = nullptr;
	}
	player = nullptr;
	objects.clear();
	stage_reload = false;
	is_create = false;
	LoadStage();

}

void GameMainScene::FindPlayer()
{
	//プレイヤーオブジェクトを探して取得
	for (auto obj : objects)
	{
		if (obj->GetObjectType() == PLAYER)
		{
			player = obj;
			break;
		}
	}
}

void GameMainScene::CreateClone()
{
	//ステージクリア時のプレイヤーの履歴を基にエネミーを生成
	for (const auto& history : stage_clear_history)
	{
		//新しいエネミー（過去のプレイヤー）を生成
		Enemy* enemy = CreateObject<Enemy>(Vector2D(0.0f, 0.0f) , Vector2D(48.0f, 96.0f));

		// 履歴をエネミーにセット
		enemy->SetReplayHistory(history);
	}
}

// アイテムを生成する関数
void GameMainScene::CreateItem()
{
	std::vector<Vector2D> item_positions;

	// ステージ上のブロックの上に空間がある場所を探して、アイテムの候補位置を取得
	for (int i = 1; i < stage_height_num; ++i) {
		for (int j = 0; j < stage_width_num; ++j) {
			// ブロック以外はスキップ
			if (stage_data[i][j] != BLOCK) continue;
			// ブロックの下が空白ならスキップ（浮いているブロックはNG）
			if (i + 1 >= stage_height_num || stage_data[i + 1][j] == EMPTY) continue;
			// ブロックの上に空きがない or トラップがあるならスキップ
			if (stage_data[i - 1][j] != EMPTY || stage_data[i][j] == TRAP) continue;

			// 画面上の座標に変換して保存
			int x = j * BOX_SIZE;
			int y = 720 - ((stage_height_num - i) * BOX_SIZE) - BOX_SIZE;
			item_positions.emplace_back(x, y);
		}
	}

	// 候補位置をシャッフル
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(item_positions.begin(), item_positions.end(), gen);

	// 最大3個までアイテムを生成
	int item_count = Min(3, static_cast<int>(item_positions.size()));
	for (int i = 0; i < item_count; ++i) {
		const Vector2D& pos = item_positions[i];

		// ステージのグリッド位置を計算
		int grid_x = pos.x / BOX_SIZE;
		int grid_y = (720 - pos.y - BOX_SIZE) / BOX_SIZE;

		// 偶数番目は無敵アイテム、奇数番目は回復アイテム
		if (i % 2 == 0) {
			CreateObject<InvincibleItem>(pos, Vector2D((float)BOX_SIZE));
			stage_data[grid_y][grid_x] = INVINCIBLE;
		}
		else {
			CreateObject<HealItem>(pos, Vector2D((float)BOX_SIZE));
			stage_data[grid_y][grid_x] = HEAL;
		}
	}
}

// ギミック（トラップ）を生成する関数
void GameMainScene::CreateGimmick()
{
	std::vector<Vector2D> trap_pos;

	// ブロックの上に空きがあり、かつアイテムが置かれていない位置を探す
	for (int i = 1; i < stage_height_num - 1; i++) {
		for (int j = 0; j < stage_width_num; j++) {

			// ブロックであるかをチェック
			if (stage_data[i][j] == BLOCK) {

				// 上のマスが空白、かつその場所にアイテムがないことを確認
				if (stage_data[i - 1][j] == EMPTY &&
					stage_data[i][j] != HEAL &&
					stage_data[i][j] != INVINCIBLE)
				{
					// 上のマスにアイテムがある場合はスキップ
					if (stage_data[i - 1][j] == HEAL || stage_data[i - 1][j] == INVINCIBLE) continue;

					// トラップの描画位置を計算
					int block_y = 720 - ((stage_height_num - i) * BOX_SIZE);
					int y = block_y - BOX_SIZE;

					trap_pos.push_back(Vector2D(j * BOX_SIZE, y));
				}
			}
		}
	}

	// トラップ候補をシャッフル
	std::random_device rand;
	std::mt19937 gen(rand());
	std::shuffle(trap_pos.begin(), trap_pos.end(), gen);

	// 最大5個までランダムにトラップを生成
	const int trap_count = Min(5, (int)trap_pos.size());
	for (int i = 0; i < trap_count; i++) {
		CreateObject<Trap>(trap_pos[i], Vector2D((float)BOX_SIZE));
	}
}


