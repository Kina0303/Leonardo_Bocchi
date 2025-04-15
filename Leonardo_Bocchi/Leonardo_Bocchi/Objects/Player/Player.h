#pragma once
#include "../GameObject.h"

class Player : public GameObject
{
private:
	Vector2D velocity;

public:
	//コンストラクタ
	Player();
	//デストラクタ
	virtual ~Player();

	//初期化
	virtual void Initialize();
	//更新
	virtual void Update();
	//描画
	virtual void Draw()const;
	//終了処理
	virtual void Finalize();

private:
	//移動
	void Movement();
};

