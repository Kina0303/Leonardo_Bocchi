#pragma once
#pragma once
#include "../Utility/Vector2D.h"
enum eObjectType
{
	BLOCK = 1,
	PLAYER,
	ENEMY,
};

class GameObject
{
protected:
	Vector2D location;	//�I�u�W�F�N�g�̈ʒu
	Vector2D box_size;	//�I�u�W�F�N�g�̃T�C�Y
	Vector2D velocity = { 0.0f };
	int image;			//�`�悷��摜�f�[�^
	int flip_flg;		//�`�悷�锽�]�t���O

	bool stage_hit_flg[2][4];		//object�ƃX�e�[�W�̓����蔻��
	float move[4] = { 0,0,0,0 };	//object�������蔻��ɔ��Ȃ��悤�ɂ��鎞�̊e�����̈ړ���

	eObjectType object_type;		//�I�u�W�F�N�g�̎��


public:
	GameObject();
	~GameObject();

	//����������
	virtual void Initialize(Vector2D _location, Vector2D _box_size);
	//�X�V����
	virtual void Update();
	//�`�揈��
	virtual void Draw(Vector2D offset, double rate) const;
	//�I��������
	virtual void Finalize();

public:
	//���W��ݒ�
	void SetLocation(Vector2D _location);

	//���W���擾
	Vector2D GetLocation()const;

	//�{�b�N�X�T�C�Y���擾
	Vector2D GetBoxSize()const;

	//���x�擾
	Vector2D GetVelocity()const;

	//�I�u�W�F�N�g�^�C�v�擾	
	eObjectType GetObjectType();

	virtual void OnHitCollision(GameObject* hit_object);

	//�����蔻��
	bool CheckBoxCollision(GameObject* obj);

};


