#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"

Player::Player() : player_state(PlayerState::eIDLE), animation_data()
{
}

Player::~Player()
{
}

void Player::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = PLAYER;
	hp = 5;
	velocity = { 0.0f };
	g_velocity = 0.350f;

	//image = animation_data[0];
	//image = NULL;

	animation_count = 0;
}

void Player::Update()
{
	__super::Update();

	//�ړ�����
	Movement();


	SaveMoveHistory();

	//�A�j���[�V�����Ǘ�
	//AnimationControl();
}

void Player::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 1.1);
	//DrawBoxAA(location.x, location.y, location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 0), FALSE);
}
void Player::Finalize()
{
	animation_data.clear();
}

void Player::Movement()
{
	//���͏��̎擾
	InputControl* input = InputControl::GetInstance();

	switch (player_state)
	{
		//���������Ă��Ȃ���ԁi�ҋ@�j
	case PlayerState::eIDLE:

		// �ҋ@��ԁi�L�[��������Ă��Ȃ��Ƃ��̌��������j
		if (velocity.x < -1e-6f) // �������̑��x�����炷
		{
			velocity.x = Min<float>(velocity.x + 0.2f, 0.0f); // ���X��0�ɋ߂Â���
		}
		else if (velocity.x > 1e-6f) // �E�����̑��x�����炷
		{
			velocity.x = Max<float>(velocity.x - 0.2f, 0.0f); // ���X��0�ɋ߂Â���
		}

		//���E�ړ�
		if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT))player_state = PlayerState::eLEFT;
		else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))player_state = PlayerState::eRIGHT;

		//�W�����v
		if (!is_jump) {
			if (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))
			{
				player_state = PlayerState::eJUMP;
			}
		}
		break;
		//�����L�[����������
	case PlayerState::eLEFT:
		velocity.x -= 1.5;
		flip_flg = TRUE;      // �������t���O���Z�b�g

		//���L�[�������ꂽ��
		if (!input->GetButton(XINPUT_BUTTON_DPAD_LEFT))player_state = PlayerState::eIDLE;

		//�W�����v
		if (!is_jump) {
			if (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))
			{
				//is_jump = true;
				//is_double_jump = true;
				player_state = PlayerState::eJUMP;
			}
		}
		break;
		//�E���L�[����������
	case PlayerState::eRIGHT:
		velocity.x += 1.5;
		flip_flg = FALSE;      // �E�����t���O���Z�b�g

		//���L�[�������ꂽ��
		if (!input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))player_state = PlayerState::eIDLE;

		//�W�����v
		if (!is_jump) {
			if (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))
			{
				player_state = PlayerState::eJUMP;
			}
		}
		break;
		//�W�����v�L�[��������
	case PlayerState::eJUMP:      
		velocity.y -= 6.0f;
		is_jump = true;
		//is_double_jump = false; // �_�u���W�����v�t���O�����Z�b�g
		//�W�����v�L�[�������ꂽ��
		if (!input->GetButtonDown(XINPUT_BUTTON_A))player_state = PlayerState::eIDLE;
		break;
	case PlayerState::eDAMAGE:
		break;
	case PlayerState::eDEAD:
		break;
	default:
		break;
	}

	//�ő呬�x�𐧌�
	float max_speed = 7.5f;  // �ő呬�x
	velocity.x = Min<float>(Max<float>(velocity.x, -max_speed), max_speed);

	//�ʒu���X�V
	location += velocity;
}


void Player::AnimationControl()
{
	//�J�E���g�̍X�V
	animation_count++;

	//
	if (animation_count >= 10)
	{
		//�J�E���g��0�N���A����
		animation_count = 0;
		//�摜�̐ؑւ��s��
		if (image == animation_data[0])
		{
			image = animation_data[1];
		}
		else
		{
			image = animation_data[0];
		}
	}
}

void Player::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);

	if (hit_object->GetObjectType() == BLOCK)
	{
		// �u���b�N�̈ʒu�ƃT�C�Y���擾
		Vector2D block_pos = hit_object->GetLocation();
		Vector2D block_size = hit_object->GetBoxSize();

		// �v���C���[�̉��[�ƃu���b�N�̏�[���r
		float player_bottom = location.y + box_size.y;
		float block_top = block_pos.y;

		// �O�t���[�����牺�����Ɉړ����Ă���i�������j && �u���b�N�̏�ɏ����
		if (velocity.y >= 0.0f && player_bottom <= block_top + 10.0f) // +10.0f�͌덷�̋z���p
		{
			// �u���b�N�̏�ɏ���Ă�Ɣ��f
			location.y = block_top - box_size.y; // �v���C���[���u���b�N�̏�ɔz�u
			velocity.y = 0.0f;
			is_jump = false;
			is_double_jump = false;
		}
	}
}

void Player::SaveMoveHistory()
{
	MoveRecord record;
	record.position = this->location;
	record.is_jumping = this->is_jump;

	move_history.push_back(record);
}
