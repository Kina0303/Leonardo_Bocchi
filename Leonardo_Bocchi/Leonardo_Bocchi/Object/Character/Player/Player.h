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
    PlayerState player_state;       //プレイヤーの状態
    std::vector<int> animation_data;//アニメーションデータ

	std::vector<MoveRecord> move_history; //移動履歴

public:
    Player();
    ~Player();

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

	//移動履歴を保存
	void SaveMoveHistory();

	//移動履歴を取得
    const std::vector<MoveRecord>& GetMoveHistory() const {
        return move_history;
    }
};

