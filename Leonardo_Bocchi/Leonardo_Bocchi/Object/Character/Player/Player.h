#pragma once
#include "../CharaBase.h"
#include <vector>

enum class PlayerState
{
    eIDLE,
    eLEFT,
    eRIGHT,
    eJUMP,
    eDAMAGE,
    eDEAD
};

class Player :
    public CharaBase
{
private:
    PlayerState player_state;       //�v���C���[�̏��
    std::vector<int> animation_data;//�A�j���[�V�����f�[�^

	std::vector<MoveRecord> move_history; //�ړ�����

public:
    Player();
    ~Player();

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

	//�ړ�������ۑ�
	void SaveMoveHistory();

	//�ړ��������擾
    const std::vector<MoveRecord>& GetMoveHistory() const {
        return move_history;
    }
};

