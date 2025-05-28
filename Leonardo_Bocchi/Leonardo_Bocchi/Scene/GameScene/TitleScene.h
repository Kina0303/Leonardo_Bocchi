#pragma once
#include "../SceneBase.h"
class TitleScene :
    public SceneBase
{
public:
	TitleScene();
	~TitleScene();

	void Initialize() override;		//‰Šú‰»ˆ—
	eSceneType Update() override;	//XVˆ—
	void Draw() const override;		//•`‰æˆ—
	void Finalize() override;		//I—¹ˆ—

	eSceneType GetNowSceneType()const override;
};

