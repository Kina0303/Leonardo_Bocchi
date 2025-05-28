#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"

Player::Player() : animation_data(),damage_timer(),is_invincible(false),invincible_timer(0)
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
	g_velocity = 0.35f;
	max_fall_speed = 10.0f;
	on_ground = false;

	damage_timer = 0;

	image = LoadGraph("Resource/Images/Character/Player/player.png");

	//image = animation_data[0];
	//image = NULL;

	animation_count = 0;
	jump_count = 1;
}

void Player::Update()
{
	__super::Update();

	//移動処理
	HandleInput();

	//プレイヤーの動きを保存
	SaveMoveHistory();

	//ダメージを受けてからの無敵時間
	if (damage_flg)
	{
		damage_timer++;
		if (damage_timer >= 180)
		{
			damage_timer = 0;
			damage_flg = false;
		}
	}

	if (is_invincible)
	{
		invincible_timer++;
		if (invincible_timer >= 240)
		{
			invincible_timer = 0;
			is_invincible = false;
		}
	}
	// 最大速度の制限
	ConstrainVelocity();

	//位置を更新
	location += velocity;

	//アニメーション管理
	//AnimationControl();
}

void Player::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 1.5);
	DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), FALSE);

#ifdef _DEBUG
	DrawFormatString(10, 120, GetColor(255, 255, 255), "HP × %d", hp);
	DrawFormatString(10, 100, GetColor(255, 255, 255), "%f     %f", velocity.x, velocity.y);
	DrawFormatString(10, 80, GetColor(255, 255, 255), "%d", jump_time);
	DrawFormatString(10, 60, GetColor(255, 255, 255), "invicible :%d", is_invincible);
	DrawFormatString(10, 40, GetColor(255, 255, 255), "invicible_timer :%d", invincible_timer);
	switch (action_state)
	{
	case Player::ActionState::IDLE:
		DrawFormatString(10, 140, GetColor(255, 255, 255), "State IDLE");
		break;
	case Player::ActionState::JUMP:
		DrawFormatString(10, 160, GetColor(255, 255, 255), "State JUMP");
		break;
	case Player::ActionState::DAMAGE:
		DrawFormatString(10, 180, GetColor(255, 255, 255), "State DAMEGE");
		break;
	default:
		break;
	}
#endif // DEBUG

	
}
void Player::Finalize()
{
	animation_data.clear();
}

void Player::ApplyDeceleration()
{
	if (velocity.x < -1e-6f) {
		velocity.x = Min<float>(velocity.x + 0.5f, 0.0f);
	}
	else if (velocity.x > 1e-6f) {
		velocity.x = Max<float>(velocity.x - 0.5f, 0.0f);
	}
}

void Player::ConstrainVelocity()
{
	// 最大速度の制限
	const float max_speed = 7.0f;
	velocity.x = Min<float>(Max<float>(velocity.x, -max_speed), max_speed);
}

void Player::HandleInput()
{
	InputControl* input = InputControl::GetInstance();

	// 移動入力
	if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT)) {
		move = MoveDirection::LEFT;
		velocity.x -= 3.0f;
		flip_flg = true;
	}
	else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		move = MoveDirection::RIGHT;
		velocity.x += 0.5f;
		//velocity.x += 3.0f;
		flip_flg = false;
	}
	else
	{
		move = MoveDirection::NONE;
		ApplyDeceleration();
	}

	//ジャンプ入力
	if (input->GetButtonDown(XINPUT_BUTTON_A) && jump_count < 2)
	{
		jump_time = 0;
		jump_count++;
		on_ground = false;
		action_state = ActionState::JUMP;
	}

	if (input->GetButton(XINPUT_BUTTON_A) && jump_count < 1 && jump_time <= 20)
	{
		jump_time++;
		if (jump_count == 0) {
			velocity.y = Max<float>(-9.0f, -4.0f - (float)jump_time * 0.08f);

		}
	}

	// 状態更新
	if (action_state != ActionState::DAMAGE) {
		if ((velocity.x == 0.0f || velocity.y == 0.0f) && on_ground)
			action_state = ActionState::IDLE;
	}
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

	//エネミーヒット時 OR トラップヒット時
	if (hit_object->GetObjectType() == ENEMY || hit_object->GetObjectType() == TRAP)
	{
		if (!damage_flg && !is_invincible)
		{
			//ダメージを受ける
			ApplyDamage();
		}
	}

	//回復アイテムヒット時
	if (hit_object->GetObjectType() == HEAL)
	{
		hp += 1;
	}

	//無敵アイテムヒット時
	if (hit_object->GetObjectType() == INVINCIBLE)
	{
		if (!is_invincible)
		{
			is_invincible = true;
		}
	}

}


void Player::SaveMoveHistory()
{
	MoveRecord record;
	record.position = this->location;
	//record.is_jumping = this->is_jump;
	record.flip = this->flip_flg;

	move_history.push_back(record);
}

void Player::ApplyDamage()
{
	damage_flg = true;
	hp--;
	
}

