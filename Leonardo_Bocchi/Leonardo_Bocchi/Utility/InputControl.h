#pragma once

#define D_KEYCODE_MAX (256)		//キーコードの最大数
class InputControl
{
private:
	//今回の入力キー
	static char now_key[D_KEYCODE_MAX];
	//前回の入力キー
	static char old_key[D_KEYCODE_MAX];

public:
	//キー入力の状態を更新
	static void Update();

	//指定されたキーが押されているか
	static bool GetKey(int key_code);
	//指定されたキーが新たに押されたか
	static bool GetKeyDown(int key_code);
	//指定されたキーが離されたか
	static bool GetKeyUp(int key_code);

private:
	//キーコードが有効な範囲にあるかを確認
	static bool CheckKeyCodeRange(int key_code);
};

