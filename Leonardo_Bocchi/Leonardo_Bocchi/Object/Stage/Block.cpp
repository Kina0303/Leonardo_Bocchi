#include "Block.h"
#include <DxLib.h>

void Block::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = BLOCK;
}

void Block::Update()
{
}

void Block::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 1.0);
	//DrawBoxAA(location.x, location.y, location.x + box_size.x, location.y + box_size.y, GetColor(0, 0, 0), TRUE);
	//DrawBoxAA(location.x, location.y, location.x + box_size.x, location.y + box_size.y, GetColor(255, 255, 255), FALSE);
}

void Block::Finalize()
{
}
