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

	//���������I�u�W�F�N�g���u���b�N�������ꍇ�̏���
	if (hit_object->GetObjectType() == BLOCK)
	{
		//�v���C���[�ƃI�u�W�F�N�g�̏����擾
		Vector2D obj_location = location;
		Vector2D obj_size = location + box_size;

		Vector2D target_location = hit_object->GetLocation();
		Vector2D target_size = target_location + hit_object->GetBoxSize();


		//AABB�Փ˔���
		if (obj_size.x > target_location.x && obj_location.x < target_size.x &&
			obj_size.y > target_location.y && obj_location.y < target_size.y)
		{
			//�����߂��p�̃x�N�g��
			Vector2D push{ 0.0f, 0.0f };

			//�߂荞�݂̌v�Z
			float depth_x = Min<float>(obj_size.x - target_location.x, target_size.x - obj_location.x);
			float depth_y = Min<float>(obj_size.y - target_location.y, target_size.y - obj_location.y);

			//�Փ˂̐[���i�߂荞�ݗʁj���v�Z
			if (depth_x < depth_y)
			{
				// �������̏Փˏ���
				if (obj_location.x < target_location.x)
				{
					push.x = -depth_x;	//��������Փ�
				}
				else
				{
					push.x = depth_x;	//�E������Փ�
				}
				velocity.x = 0.0f;
			}
			else
			{
				//�c�����̏Փˏ���
				if (obj_location.y < target_location.y)
				{
					push.y = -depth_y; //�ォ��Փ�
					jump_flag = false;	  //�W�����v�t���O��false�ɂ���
				}
				else
				{
					push.y = depth_y;	//������Փ�
				}
				velocity.y = 0.0f;
				g_velocity = 0.0f;
			}
			// �ʒu���C��
			location += push;
		}

	}
}