#pragma once
#include "../Utility/Vector2D.h"

class GameObject
{
protected:
	unsigned int color;	//オブジェクトの色
	Vector2D location;	//オブジェクトの位置
	Vector2D box_size;	//オブジェクトのサイズ

public:
	//コンストラクタ
	GameObject();
	//デストラクタ
	virtual ~GameObject();

	//初期化
	virtual void Initialize();
	//更新
	virtual void Update();
	//描画
	virtual void Draw()const;
	//終了処理
	virtual void Finalize();

public:
	//座標を設定
	void SetLocation(Vector2D location);
	//座標を取得
	Vector2D GetLocation()const;
	//ボックスサイズを取得
	Vector2D GetBoxSize()const;

};

