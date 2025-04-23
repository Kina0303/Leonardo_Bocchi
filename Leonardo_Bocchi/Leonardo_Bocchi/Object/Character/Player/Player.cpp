#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"

Player::Player() : player_state(PlayerState::eIDLE), animation_data()
{
}

Player::~Player()
{
}

void Player::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = PLAYER;
	hp = 5;
	velocity = { 0.0f };
	g_velocity = 0.350f;

	//image = animation_data[0];
	//image = NULL;

	animation_count = 0;
}

void Player::Update()
{
	__super::Update();

	//移動処理
	Movement();


	SaveMoveHistory();

	//アニメーション管理
	//AnimationControl();
}

void Player::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 1.1);
	//DrawBoxAA(location.x, location.y, location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 0), FALSE);
}
void Player::Finalize()
{
	animation_data.clear();
}

void Player::Movement()
{
	//入力情報の取得
	InputControl* input = InputControl::GetInstance();

	switch (player_state)
	{
		//何も動いていない状態（待機）
	case PlayerState::eIDLE:

		// 待機状態（キーが押されていないときの減速処理）
		if (velocity.x < -1e-6f) // 左向きの速度を減らす
		{
			velocity.x = Min<float>(velocity.x + 0.2f, 0.0f); // 徐々に0に近づける
		}
		else if (velocity.x > 1e-6f) // 右向きの速度を減らす
		{
			velocity.x = Max<float>(velocity.x - 0.2f, 0.0f); // 徐々に0に近づける
		}

		//左右移動
		if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT))player_state = PlayerState::eLEFT;
		else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))player_state = PlayerState::eRIGHT;

		//ジャンプ
		if (!is_jump) {
			if (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))
			{
				player_state = PlayerState::eJUMP;
			}
		}
		break;
		//左矢印キーを押したら
	case PlayerState::eLEFT:
		velocity.x -= 1.5;
		flip_flg = TRUE;      // 左向きフラグをセット

		//左キーが離されたら
		if (!input->GetButton(XINPUT_BUTTON_DPAD_LEFT))player_state = PlayerState::eIDLE;

		//ジャンプ
		if (!is_jump) {
			if (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))
			{
				//is_jump = true;
				//is_double_jump = true;
				player_state = PlayerState::eJUMP;
			}
		}
		break;
		//右矢印キーを押したら
	case PlayerState::eRIGHT:
		velocity.x += 1.5;
		flip_flg = FALSE;      // 右向きフラグをセット

		//左キーが離されたら
		if (!input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))player_state = PlayerState::eIDLE;

		//ジャンプ
		if (!is_jump) {
			if (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))
			{
				player_state = PlayerState::eJUMP;
			}
		}
		break;
		//ジャンプキー押したら
	case PlayerState::eJUMP:      
		velocity.y -= 6.0f;
		is_jump = true;
		//is_double_jump = false; // ダブルジャンプフラグをリセット
		//ジャンプキーが離されたら
		if (!input->GetButtonDown(XINPUT_BUTTON_A))player_state = PlayerState::eIDLE;
		break;
	case PlayerState::eDAMAGE:
		break;
	case PlayerState::eDEAD:
		break;
	default:
		break;
	}

	//最大速度を制限
	float max_speed = 7.5f;  // 最大速度
	velocity.x = Min<float>(Max<float>(velocity.x, -max_speed), max_speed);

	//位置を更新
	location += velocity;
}


void Player::AnimationControl()
{
	//カウントの更新
	animation_count++;

	//
	if (animation_count >= 10)
	{
		//カウントを0クリアする
		animation_count = 0;
		//画像の切替を行う
		if (image == animation_data[0])
		{
			image = animation_data[1];
		}
		else
		{
			image = animation_data[0];
		}
	}
}

void Player::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);

	if (hit_object->GetObjectType() == BLOCK)
	{
		// ブロックの位置とサイズを取得
		Vector2D block_pos = hit_object->GetLocation();
		Vector2D block_size = hit_object->GetBoxSize();

		// プレイヤーの下端とブロックの上端を比較
		float player_bottom = location.y + box_size.y;
		float block_top = block_pos.y;

		// 前フレームから下方向に移動している（落下中） && ブロックの上に乗った
		if (velocity.y >= 0.0f && player_bottom <= block_top + 10.0f) // +10.0fは誤差の吸収用
		{
			// ブロックの上に乗ってると判断
			location.y = block_top - box_size.y; // プレイヤーをブロックの上に配置
			velocity.y = 0.0f;
			is_jump = false;
			is_double_jump = false;
		}
	}
}

void Player::SaveMoveHistory()
{
	MoveRecord record;
	record.position = this->location;
	record.is_jumping = this->is_jump;

	move_history.push_back(record);
}
