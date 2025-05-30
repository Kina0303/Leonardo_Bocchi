#pragma once
#include "../SceneBase.h"
class TitleScene :
    public SceneBase
{
private:
	enum MenuItem
	{
		MENU_START,
		MENU_HELP,
		MENU_RANKING,
		MENU_END
	};

	int select_index;

	int menu_font;
	int title_font;
	int small_font;

public:
	TitleScene();
	~TitleScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() const override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;
};

