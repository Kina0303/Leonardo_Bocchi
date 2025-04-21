#pragma once
#include "../CharaBase.h"

class Enemy :
    public CharaBase
{
private:

public:
    Enemy();
    ~Enemy();

    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate) const override;
    //終了時処理
    void Finalize()override;

public:
    
    //プレイヤーの動き
    void Movement();
    //アニメーション管理
    void AnimationControl();
    //当たった時の挙動
    void OnHitCollision(GameObject* hit_object)override;
};

