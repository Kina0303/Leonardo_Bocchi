#pragma once
#include "../SceneBase.h"
#include "../../common.h"

class GameMainScene :
    public SceneBase
{
private:
public:
	GameMainScene();
	~GameMainScene();

	void Initialize() override;		//‰Šú‰»ˆ—
	eSceneType Update() override;	//XVˆ—
	void Draw() const override;		//•`‰æˆ—
	void Finalize() override;		//I—¹ˆ—

	eSceneType GetNowSceneType()const override;
};

