#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"

Player::Player() : player_state(PlayerState::eIDLE), animation_data(),damage_time()
{
}

Player::~Player()
{
}

void Player::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = PLAYER;
	hp = 2;
	velocity = { 0.0f };
	g_velocity = 0.350f;

	damage_time = 0;

	//image = animation_data[0];
	//image = NULL;

	animation_count = 0;
}

void Player::Update()
{
	__super::Update();

	//�ړ�����
	Movement();

	//�v���C���[�̓�����ۑ�
	SaveMoveHistory();

	if (damage_flg)
	{
		damage_time++;
		if (damage_time >= 180)
		{
			damage_time = 0;
			damage_flg = false;
		}
	}


	//�A�j���[�V�����Ǘ�
	//AnimationControl();
}

void Player::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 1.1);
	//DrawBoxAA(location.x, location.y, location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 0), FALSE);

	DrawFormatString(10, 120, GetColor(255, 255, 255), "HP �~ %d", hp);
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
		// �ҋ@��Ԃ̏���
		IdleState(input);
		break;
		//�����L�[����������
	case PlayerState::eLEFT:
		LeftState(input);
		break;
		//�E���L�[����������
	case PlayerState::eRIGHT:
		RightState(input);
		break;
		//�W�����v�L�[��������
	case PlayerState::eJUMP:      
		JumpState(input);
		break;
	case PlayerState::eDAMAGE:
	case PlayerState::eDEAD:
	default:
		break;
	}

	// �ő呬�x�̐���
	ConstrainVelocity();

	//�ʒu���X�V
	location += velocity;
}

void Player::IdleState(InputControl* input)
{
	// �ҋ@��ԁi�{�^����������Ă��Ȃ��Ƃ��̌��������j
	ApplyDeceleration();

	// ���E�ړ�
	if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT) || input->GetKey(KEY_INPUT_A))
	{
		player_state = PlayerState::eLEFT;
	}
	else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT) || input->GetKey(KEY_INPUT_D))
	{
		player_state = PlayerState::eRIGHT;
	}

	// �W�����v
	if (!is_jump && (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE))) 
	{
		player_state = PlayerState::eJUMP;
	}
}

void Player::LeftState(InputControl* input)
{
	velocity.x -= 0.5;
	flip_flg = TRUE;  // �������t���O

	// ���L�[�������ꂽ��ҋ@���
	if (!input->GetButton(XINPUT_BUTTON_DPAD_LEFT) || input->GetKeyDown(KEY_INPUT_A)) {
		player_state = PlayerState::eIDLE;
	}

	// �W�����v
	if (!is_jump && (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE)))
	{
		player_state = PlayerState::eJUMP;
	}
}

void Player::RightState(InputControl* input)
{
	velocity.x += 0.5;
	flip_flg = FALSE;  // �E�����t���O

	// �E�L�[�������ꂽ��ҋ@���
	if (!input->GetButton(XINPUT_BUTTON_DPAD_RIGHT) || input->GetKeyDown(KEY_INPUT_D)) {
		player_state = PlayerState::eIDLE;
	}

	// �W�����v
	if (!is_jump && (input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyDown(KEY_INPUT_SPACE)))
	{
		player_state = PlayerState::eJUMP;
	}
}

void Player::JumpState(InputControl* input)
{
	velocity.y -= 6.0f;
	is_jump = true;

	// �W�����v�L�[�������ꂽ��ҋ@��Ԃɖ߂�
	if (!input->GetButtonDown(XINPUT_BUTTON_A) || input->GetKeyUp(KEY_INPUT_SPACE)) {
		player_state = PlayerState::eIDLE;
	}
}

void Player::DamageState(InputControl* input)
{
}

void Player::DeadState(InputControl* input)
{
}

void Player::ApplyDeceleration()
{
	if (velocity.x < -1e-6f) {
		velocity.x = Min<float>(velocity.x + 0.2f, 0.0f);
	}
	else if (velocity.x > 1e-6f) {
		velocity.x = Max<float>(velocity.x - 0.2f, 0.0f);
	}
}

void Player::ConstrainVelocity()
{
	// �ő呬�x�̐���
	const float max_speed = 7.5f;
	velocity.x = Min<float>(Max<float>(velocity.x, -max_speed), max_speed);
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

	if (hit_object->GetObjectType() == ENEMY)
	{
		if (!damage_flg)
		{
			//�_���[�W���󂯂�
			ApplyDamage();
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

void Player::ApplyDamage()
{
	damage_flg = true;
	hp--;
}
