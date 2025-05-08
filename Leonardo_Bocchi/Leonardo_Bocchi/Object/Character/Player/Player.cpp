#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"

Player::Player() : player_state(PlayerState::eIDLE), animation_data(),damage_time()
{
}

Player::~Player()
{
}

void Player::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = PLAYER;
	hp = 2;
	velocity = { 0.0f };
	g_velocity = 0.350f;

	damage_time = 0;

	//image = animation_data[0];
	//image = NULL;

	animation_count = 0;
}

void Player::Update()
{
	__super::Update();

	//移動処理
	Movement();

	//プレイヤーの動きを保存
	SaveMoveHistory();

	if (damage_flg)
	{
		damage_time++;
		if (damage_time >= 180)
		{
			damage_time = 0;
			damage_flg = false;
		}
	}


	//アニメーション管理
	//AnimationControl();
}

void Player::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 1.1);
	//DrawBoxAA(location.x, location.y, location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 0), FALSE);

	DrawFormatString(10, 120, GetColor(255, 255, 255), "HP × %d", hp);
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
		// 待機状態の処理
		IdleState(input);
		break;
		//左矢印キーを押したら
	case PlayerState::eLEFT:
		LeftState(input);
		break;
		//右矢印キーを押したら
	case PlayerState::eRIGHT:
		RightState(input);
		break;
		//ジャンプキー押したら
	case PlayerState::eJUMP:      
		JumpState(input);
		break;
	case PlayerState::eDAMAGE:
	case PlayerState::eDEAD:
	default:
		break;
	}

	// 最大速度の制限
	ConstrainVelocity();

	//位置を更新
	location += velocity;
}

void Player::IdleState(InputControl* input)
{
	// 待機状態（ボタンが押されていないときの減速処理）
	ApplyDeceleration();

	// 左右移動
	if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT) || input->GetKey(KEY_INPUT_A))
	{
		player_state = PlayerState::eLEFT;
	}
	else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT) || input->GetKey(KEY_INPUT_D))
	{
		player_state = PlayerState::eRIGHT;
	}

	// ジャンプ
	if (!is_jump && (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))) 
	{
		player_state = PlayerState::eJUMP;
	}
}

void Player::LeftState(InputControl* input)
{
	velocity.x -= 0.5;
	flip_flg = TRUE;  // 左向きフラグ

	// 左キーが離されたら待機状態
	if (!input->GetButton(XINPUT_BUTTON_DPAD_LEFT) || input->GetKeyDown(KEY_INPUT_A)) {
		player_state = PlayerState::eIDLE;
	}

	// ジャンプ
	if (!is_jump && (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE)))
	{
		player_state = PlayerState::eJUMP;
	}
}

void Player::RightState(InputControl* input)
{
	velocity.x += 0.5;
	flip_flg = FALSE;  // 右向きフラグ

	// 右キーが離されたら待機状態
	if (!input->GetButton(XINPUT_BUTTON_DPAD_RIGHT) || input->GetKeyDown(KEY_INPUT_D)) {
		player_state = PlayerState::eIDLE;
	}

	// ジャンプ
	if (!is_jump && (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE)))
	{
		player_state = PlayerState::eJUMP;
	}
}

void Player::JumpState(InputControl* input)
{
	velocity.y -= 6.0f;
	is_jump = true;

	// ジャンプキーが離されたら待機状態に戻す
	if (!input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyUp(KEY_INPUT_SPACE)) {
		player_state = PlayerState::eIDLE;
	}
}

void Player::DamageState(InputControl* input)
{
}

void Player::DeadState(InputControl* input)
{
}

void Player::ApplyDeceleration()
{
	if (velocity.x < -1e-6f) {
		velocity.x = Min<float>(velocity.x + 0.2f, 0.0f);
	}
	else if (velocity.x > 1e-6f) {
		velocity.x = Max<float>(velocity.x - 0.2f, 0.0f);
	}
}

void Player::ConstrainVelocity()
{
	// 最大速度の制限
	const float max_speed = 7.5f;
	velocity.x = Min<float>(Max<float>(velocity.x, -max_speed), max_speed);
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

	if (hit_object->GetObjectType() == ENEMY)
	{
		if (!damage_flg)
		{
			//ダメージを受ける
			ApplyDamage();
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

void Player::ApplyDamage()
{
	damage_flg = true;
	hp--;
}
