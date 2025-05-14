#pragma once
#include "../GameObject.h"
#include "../../Utility/UserTemplate.h"
#include <vector>


struct MoveRecord {
	Vector2D position;
	bool is_jumping;
};

class CharaBase :
    public GameObject
{
protected:
	int hp = 0;
	float g_velocity = 0.0f;       //�d�͉����x

	bool on_ground = false;
	int jump_count = 0;

	bool damage_flg = false;	//�_���[�W���󂯂����ǂ���

	int animation_count = 0;    //�A�j���[�V�����J�E���g

	std::vector<GameObject*> hit_objects;
public:
	//����������
	virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
	//�X�V����
	virtual void Update()override;
	//�`�揈��
	virtual void Draw(Vector2D offset, double rate) const override;
	//�I��������
	virtual void Finalize()override;

	virtual void OnHitCollision(GameObject* hit_object)override;

	bool IsOnGround() const { return on_ground; }
};

