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

	if (current_frame < replay_history.size())
	{
		const auto& record = replay_history[current_frame];
		SetLocation(record.position);

		++current_frame;
	}

}

void Enemy::Draw(Vector2D offset, double rate) const
{
	//__super::Draw(offset, 1.0);
	DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(0, 255, 0), TRUE);
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

void Enemy::SetReplayHistory(const std::vector<MoveRecord>& history)
{
	replay_history = history;
	current_frame = 0;
	//replay_history.clear();
}


