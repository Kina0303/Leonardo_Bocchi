#pragma once
#include "../CharaBase.h"

class Enemy :
    public CharaBase
{
private:

public:
    Enemy();
    ~Enemy();

    //����������
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //�X�V����
    void Update()override;
    //�`�揈��
    void Draw(Vector2D offset, double rate) const override;
    //�I��������
    void Finalize()override;

public:
    
    //�v���C���[�̓���
    void Movement();
    //�A�j���[�V�����Ǘ�
    void AnimationControl();
    //�����������̋���
    void OnHitCollision(GameObject* hit_object)override;
};

