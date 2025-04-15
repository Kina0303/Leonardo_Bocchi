#include "InputControl.h"
#include "DxLib.h"

char InputControl::now_key[D_KEYCODE_MAX] = {};
char InputControl::old_key[D_KEYCODE_MAX] = {};

void InputControl::Update()
{
	//前回のキー状態を現在のキー状態にコピー
	memcpy(old_key, now_key, (sizeof(char) * D_KEYCODE_MAX));

	//現在のキー状態を取得
	GetHitKeyStateAll(now_key);
}

bool InputControl::GetKey(int key_code)
{
	//キーが現在も前回も押されている状態であればtrueを返す
	return CheckKeyCodeRange(key_code) && ((now_key[key_code] == TRUE) && (old_key[key_code] == TRUE));
}

bool InputControl::GetKeyDown(int key_code)
{
	//キーが今回押されたが前回は押されていなかった状態であればtrueを返す
	return CheckKeyCodeRange(key_code) && ((now_key[key_code] == TRUE) && (old_key[key_code] == FALSE));
}

bool InputControl::GetKeyUp(int key_code)
{
	//そのキーが今回離されたが前回は押されていた状態であればtrueを返す
	return CheckKeyCodeRange(key_code) && ((now_key[key_code] == FALSE) && (old_key[key_code] == TRUE));
}

bool InputControl::CheckKeyCodeRange(int key_code)
{
	//key_codeが0以上でD_KEYCODE_MAX未満であればtrueを返す
	return (0 <= key_code && key_code < D_KEYCODE_MAX);
}
