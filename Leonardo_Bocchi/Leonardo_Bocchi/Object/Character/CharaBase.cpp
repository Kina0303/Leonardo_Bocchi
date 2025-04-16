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
	//重力加速度
	g_velocity += GRAVITY / 444.0f;
	velocity.y += g_velocity;// 重力を加算
	location.y += velocity.y;

}

void CharaBase::Draw(Vector2D offset, double rate) const
{
	//親クラスに書かれた描画処理の内容を実行する
	__super::Draw(offset, rate);

}

void CharaBase::Finalize()
{
}

void CharaBase::OnHitCollision(GameObject* hit_object)
{

	//当たったオブジェクトがブロックだった場合の処理
	if (hit_object->GetObjectType() == BLOCK)
	{
		//プレイヤーとオブジェクトの情報を取得
		Vector2D obj_location = location;
		Vector2D obj_size = location + box_size;

		Vector2D target_location = hit_object->GetLocation();
		Vector2D target_size = target_location + hit_object->GetBoxSize();


		//AABB衝突判定
		if (obj_size.x > target_location.x && obj_location.x < target_size.x &&
			obj_size.y > target_location.y && obj_location.y < target_size.y)
		{
			//押し戻し用のベクトル
			Vector2D push{ 0.0f, 0.0f };

			//めり込みの計算
			float depth_x = Min<float>(obj_size.x - target_location.x, target_size.x - obj_location.x);
			float depth_y = Min<float>(obj_size.y - target_location.y, target_size.y - obj_location.y);

			//衝突の深さ（めり込み量）を計算
			if (depth_x < depth_y)
			{
				// 横方向の衝突処理
				if (obj_location.x < target_location.x)
				{
					push.x = -depth_x;	//左側から衝突
				}
				else
				{
					push.x = depth_x;	//右側から衝突
				}
				velocity.x = 0.0f;
			}
			else
			{
				//縦方向の衝突処理
				if (obj_location.y < target_location.y)
				{
					push.y = -depth_y; //上から衝突
					jump_flag = false;	  //ジャンプフラグをfalseにする
				}
				else
				{
					push.y = depth_y;	//下から衝突
				}
				velocity.y = 0.0f;
				g_velocity = 0.0f;
			}
			// 位置を修正
			location += push;
		}

	}
}