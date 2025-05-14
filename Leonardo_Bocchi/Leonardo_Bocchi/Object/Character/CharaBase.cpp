#include "CharaBase.h"
#include <iostream>
#include <algorithm>

#define GRAVITY (9.087f)

void CharaBase::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
}

void CharaBase::Update()
{
	on_ground = false;
	//重力加速度
	g_velocity += GRAVITY / 444.0f;
	velocity.y += g_velocity;// 重力を加算
	location.y += velocity.y;

}

void CharaBase::Draw(Vector2D offset, double rate) const
{
	//親クラスに書かれた描画処理の内容を実行する
	__super::Draw(offset, rate);

}

void CharaBase::Finalize()
{
}
void CharaBase::OnHitCollision(GameObject* hit_object)
{
	if (hit_object->GetObjectType() != BLOCK) return;

	Vector2D obj_pos = location;
	Vector2D obj_size = location + hit_box;

	Vector2D target_pos = hit_object->GetLocation();
	Vector2D target_size = target_pos + hit_object->GetBoxSize();

	// 当たっていないなら return
	if (obj_size.x <= target_pos.x || obj_pos.x >= target_size.x ||
		obj_size.y <= target_pos.y || obj_pos.y >= target_size.y)
		return;

	// めり込み量
	float depth_x = Min(obj_size.x - target_pos.x, target_size.x - obj_pos.x);
	float depth_y = Min(obj_size.y - target_pos.y, target_size.y - obj_pos.y);

	// ---------- Y方向の押し出しを先に処理 ----------
	if (depth_y <= depth_x)
	{
		if (obj_pos.y < target_pos.y)
		{
			// 上から接触（床の上に乗る）
			location.y -= depth_y;
			if (velocity.y >= 0.0f)
			{
				velocity.y = 0.0f;
				g_velocity = 0.0f;
				on_ground = true;
				jump_count = 0;
			}
		}
		else
		{
			// 下から接触（天井に当たる）
			location.y += depth_y;
			if (velocity.y < 0.0f) velocity.y = 0.0f;
		}
	}
	else
	{
		// ---------- X方向の押し出し ----------
		if (obj_pos.x < target_pos.x)
		{
			location.x -= depth_x;
		}
		else
		{
			location.x += depth_x;
		}
		velocity.x = 0.0f;
	}
}
