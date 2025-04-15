#include "Player.h"
#include "../../Utility/InputControl.h"
#include "../../Utility/UserTenplate.h"
#include "DxLib.h"

Player::Player() :velocity(0.0f)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	//色を設定
	color = (255, 255, 255);

	//サイズを設定
	box_size = Vector2D(32.0f);

	//初期位置を設定
	location = Vector2D(320.0f, 240.0f);
}

void Player::Update()
{
	//移動処理
	Movement();
}

void Player::Draw() const
{
	__super::Draw();
}

void Player::Finalize()
{
}

void Player::Movement()
{
	float direction = 0.0f;
	//左キーが押されている場合、左に移動
	if (InputControl::GetKey(KEY_INPUT_LEFT))
	{
		direction = -1.0f;
	}
	//右キーが押されている場合、右に移動
	else if (InputControl::GetKey(KEY_INPUT_RIGHT))
	{
		direction = 1.0f;
	}

	//方向が指定されている場合の処理
	if (direction != 0.0f)
	{
		//最大速度を計算
		float max_speed = Abs<float>((5.0f * 0.5 * direction));
		//速度を加算
		velocity.x += 0.5 * direction;
		//速度を最大値を最小値の範囲に収める
		velocity.x = Min<float>(Max<float>(velocity.x, -max_speed), max_speed);
	}
	//方向が指定されていない場合
	else
	{
		if (velocity.x < -1e-6f)
		{
			float calc_speed = velocity.x + 0.1;
			velocity.x = Min<float>(calc_speed, 0.0f);
		}
		else if (1e-6f < velocity.x)
		{
			float calc_speed = velocity.x - 0.1f;
			velocity.x = Max<float>(calc_speed, 0.0f);
		}
	}
	//画面左端に到達した場合
	if (location.x < (box_size.x / 2.0f))
	{
		velocity.x = 0.0f;
		location.x = box_size.x / 2.0f;
	}
	//画面右端に到達した場合
	else if ((640.0f - (box_size.x / 2.0f)) < location.x)
	{
		velocity.x = 0.0f;
		location.x = 640.0f - (box_size.x / 2.0f);
	}

	//位置に速度を加算して更新
	location += velocity;
}
