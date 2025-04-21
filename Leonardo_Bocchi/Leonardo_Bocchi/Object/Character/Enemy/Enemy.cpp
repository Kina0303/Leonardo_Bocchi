#include "Enemy.h"
#include <DxLib.h>
#include <vector>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = ENEMY;
	velocity = { 0.0f };
	g_velocity = 0.0f;

	animation_count = 0;
}

void Enemy::Update()
{


	__super::Update();
	//移動処理
	//Movement();
	//アニメーション管理
	//AnimationControl();
}

void Enemy::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 1.0);
	DrawFormatString(offset.x, offset.y, GetColor(255, 0, 0), "Enemy");
	DrawFormatString(10, 60, GetColor(255, 0, 0), "%f",location.x);
}

void Enemy::Finalize()
{
}

void Enemy::Movement()
{
}

void Enemy::AnimationControl()
{
}

void Enemy::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);
}


