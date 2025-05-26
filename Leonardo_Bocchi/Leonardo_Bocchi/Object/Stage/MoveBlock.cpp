#include "MoveBlock.h"
#include <DxLib.h>

void MoveBlock::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
	object_type = MOVE_BLOCK;
}

void MoveBlock::Update()
{
}

void MoveBlock::Draw(Vector2D offset, double rate) const
{
	DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 255), TRUE);
}

void MoveBlock::Finalize()
{
}

void MoveBlock::OnHitCollision(GameObject* hit_object)
{
}
