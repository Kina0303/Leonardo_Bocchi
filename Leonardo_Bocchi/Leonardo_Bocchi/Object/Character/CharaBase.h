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
	float g_velocity = 0.0f;       //重力加速度

	bool on_ground = false;
	int jump_count = 0;

	bool damage_flg = false;	//ダメージを受けたかどうか

	int animation_count = 0;    //アニメーションカウント

	std::vector<GameObject*> hit_objects;
public:
	//初期化処理
	virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
	//更新処理
	virtual void Update()override;
	//描画処理
	virtual void Draw(Vector2D offset, double rate) const override;
	//終了時処理
	virtual void Finalize()override;

	virtual void OnHitCollision(GameObject* hit_object)override;

	bool IsOnGround() const { return on_ground; }
};

