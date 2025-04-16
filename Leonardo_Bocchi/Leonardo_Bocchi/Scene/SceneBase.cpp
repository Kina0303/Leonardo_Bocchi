#include "SceneBase.h"
#include "../common.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	this->Finalize();
}

void SceneBase::Initialize()
{
}

eSceneType SceneBase::Update()
{
	for (GameObject* obj : objects)
	{
		//if (camera_location.x + obj->GetBoxSize().x && camera_location.x <= 800) {
		obj->Update();
		//}
	}

	// ��d���[�v�ŏՓ˔���
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = i + 1; j < objects.size(); j++)
		{
			if (objects[i]->CheckBoxCollision(objects[j]))
			{
				objects[i]->OnHitCollision(objects[j]);
				objects[j]->OnHitCollision(objects[i]);
			}
		}
	}

	return GetNowSceneType();
}

void SceneBase::Draw() const
{
	for (GameObject* obj : objects)
	{
		Vector2D draw_location = obj->GetLocation() - camera_location;
		obj->Draw(draw_location, 1.0);
	}
}

void SceneBase::Finalize()
{
	//�I�u�W�F�N�g����̏ꍇ�͏������X�L�b�v
	if (objects.empty())
	{
		return;
	}

	for (GameObject* obj : objects)
	{
		obj->Finalize();

		delete obj;
	}

	objects.clear();
}

void SceneBase::DeleteObject(GameObject* obj)
{
	if (obj == nullptr) return;

	auto it = std::find(objects.begin(), objects.end(), obj);

	if (it != objects.end()) {
		//���������ꍇ�A�I�u�W�F�N�g���폜
		(*it)->Finalize(); //�I������
		delete* it;         //���������
		objects.erase(it);  //���X�g����폜
	}
}

