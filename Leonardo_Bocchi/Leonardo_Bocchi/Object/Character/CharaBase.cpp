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
	//�d�͉����x
	g_velocity += GRAVITY / 444.0f;
	velocity.y += g_velocity;// �d�͂����Z
	location.y += velocity.y;

}

void CharaBase::Draw(Vector2D offset, double rate) const
{
	//�e�N���X�ɏ����ꂽ�`�揈���̓��e�����s����
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

	if (obj_size.x > target_pos.x && obj_pos.x < target_size.x &&
		obj_size.y > target_pos.y && obj_pos.y < target_size.y)
	{
		// �߂荞�ݗ�
		float depth_x = Max<float>(0.0f, Min<float>(obj_size.x - target_pos.x, target_size.x - obj_pos.x));
		float depth_y = Max<float>(0.0f, Min<float>(obj_size.y - target_pos.y, target_size.y - obj_pos.y));

		// ���S���W�̍���p���āA�ǂ����������Փ˂��Ă邩�𔻒f
		Vector2D self_center = obj_pos + hit_box / 2.0f;
		Vector2D target_center = target_pos + hit_object->GetBoxSize() / 2.0f;
		Vector2D diff = self_center - target_center;

		bool prioritize_y = fabs(diff.y) > fabs(diff.x);

		if (prioritize_y)
		{
			// Y�����̉����o��
			if (obj_pos.y < target_pos.y)
			{
				// �ォ�璅�n
				location.y -= depth_y;
				if (velocity.y >= 0.0f)
				{
					is_jump = false;
					velocity.y = 0.0f;
					g_velocity = 0.0f;
				}
			}
			else
			{
				// ������Փ�
				location.y += depth_y;
				if (velocity.y < 0.0f) velocity.y = 0.0f;
			}
		}
		else
		{
			// X�����̉����o��
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

}






