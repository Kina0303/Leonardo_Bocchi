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
	g_velocity = 0.0f;

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
			velocity.x = Min<float>(velocity.x + 0.15f, 0.0f); // 徐々に0に近づける
		}
		else if (velocity.x > 1e-6f) // 右向きの速度を減らす
		{
			velocity.x = Max<float>(velocity.x - 0.15f, 0.0f); // 徐々に0に近づける
		}

		//左右移動
		if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT))player_state = PlayerState::eLEFT;
		else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))player_state = PlayerState::eRIGHT;

		//ジャンプ
		if (!jump_flag) {
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
		if (!jump_flag) {
			if (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))
			{
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
		if (!jump_flag) {
			if (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))
			{
				player_state = PlayerState::eJUMP;
			}
		}
		break;
		//ジャンプキー押したら
	case PlayerState::eJUMP:
		jump_flag = true;
		velocity.y -= 4.0f;
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
	float max_speed = 5.0f;  // 最大速度
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
}

void Player::SaveMoveHistory()
{
	MoveRecord record;
	record.position = this->location;
	record.is_jumping = this->jump_flag;

	move_history.push_back(record);
}
