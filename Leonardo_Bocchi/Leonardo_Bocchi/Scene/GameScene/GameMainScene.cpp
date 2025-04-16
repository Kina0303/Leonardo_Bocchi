#include "GameMainScene.h"
#include "DxLib.h"
#include "../../Object/Character/Player/Player.h"
#include "../../Object/Stage/Block.h"

#include <fstream>
#include <sstream>
#include <iostream>


GameMainScene::GameMainScene() :stage_width_num(0), stage_height_num(0), stage_data{ 0 }
{
}

GameMainScene::~GameMainScene()
{
}

void GameMainScene::Initialize()
{
	LoadStage();
}

eSceneType GameMainScene::Update()
{
	UpdateCamera();

	return __super::Update();
}

void GameMainScene::Draw() const
{
	//�w�i
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(150, 150, 150), TRUE); // �w�i�����œh��Ԃ�
	__super::Draw();
	DrawFormatString(10, 10, GetColor(255, 255, 255), "���C�����");
	DrawFormatString(10, 40, GetColor(255, 255, 255), "�X�e�[�W�T�C�Y�F�� = %d      ���� = %d\n", stage_width_num, stage_height_num);

}

void GameMainScene::Finalize()
{
}

eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}

void GameMainScene::LoadStage()
{
	std::ifstream file("Resource/file/stage.csv");

	if (!file) {
		std::cerr << "�t�@�C�����J���܂���ł���: " << std::endl;
		return;
	}
	// 1�s�ڂ�ǂݍ���ŃX�e�[�W���ƍ������擾
	std::string line;

	//file����1�s�ǂݍ���ŁAline �Ɋi�[�B
	if (std::getline(file, line)) {
		//���������͂��邽�߂̃X�g���[�����쐬�B
		std::stringstream ss(line);
		std::string width, height;

		// �J���}�ŕ������ĕ��ƍ������擾
		std::getline(ss, width, ',');
		std::getline(ss, height, ',');

		//������𐮐��ɕϊ�
		stage_width_num = std::stoi(width);   // �X�e�[�W��
		stage_height_num = std::stoi(height); // �X�e�[�W����
	}

	//�X�e�[�W�f�[�^�̓ǂݍ��݁iCSV��2�s�ڈȍ~�j
	for (int i = 0; i < stage_height_num; i++) {
		//1�s���ǂݍ���
		if (std::getline(file, line)) {
			std::stringstream ss(line);
			for (int j = 0; j < stage_width_num; j++) {
				//�J���}��؂�Ńf�[�^���擾
				std::string value;
				if (std::getline(ss, value, ',')) {
					//������𐮐��ɕϊ����ăX�e�[�W�f�[�^�Ɋi�[
					stage_data[i][j] = std::stoi(value);
				}
			}
		}
	}

	file.close();

	SetStage();
}

void GameMainScene::SetStage()
{
	//�X�e�[�W�f�[�^�����ɃI�u�W�F�N�g�𐶐�
	for (int i = 0; i < stage_height_num; i++) {
		for (int j = 0; j < stage_width_num; j++) {
			//�u���b�N��Y���W���v�Z	
			int y = 720 - ((stage_height_num - 1 - i) * BOX_SIZE);
			switch (stage_data[i][j])
			{
			case 0:
				break;
			case BLOCK:
				CreateObject<Block>(Vector2D(j * BOX_SIZE, y), Vector2D((float)BOX_SIZE));
				break;
			case PLAYER:
				CreateObject<Player>(Vector2D(j * BOX_SIZE, y), Vector2D(64.0f,96.0f));
				break;
			default:
				break;
			}
		}
	}
}

void GameMainScene::UpdateCamera()
{
	//�v���C���[�̎擾
	GameObject* player = nullptr;
	for (auto obj : objects)
	{
		if (obj->GetObjectType() == PLAYER)
		{
			player = obj;
			break;
		}
	}

	//�v���C���[�����݂���Ȃ�J������Ǐ]������
	if (player)
	{
		float screen_half_width = SCREEN_WIDTH / 2;				//��ʂ̔����̕�
		float stage_limit_left = 0.0f;							//�X�e�[�W�̍��[
		float stage_limit_right = stage_width_num * BOX_SIZE - SCREEN_WIDTH; //�X�e�[�W�̉E�[ 

		//�J�����ʒu �� �v���C���[�̈ʒu - ��ʂ̔����̕� 
		camera_location.x = player->GetLocation().x - screen_half_width;

		//��ʒ[�ł̓X�N���[�����Ȃ��悤����
		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}
