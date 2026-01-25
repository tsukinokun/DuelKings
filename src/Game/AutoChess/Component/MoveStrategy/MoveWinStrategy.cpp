//---------------------------------------------------------------------------
//!	@file	MoveWinStrategy.cpp
//! @brief	ピースの移動ストラテジークラス(最も近い敵へ移動)
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MoveWinStrategy.h"
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------------------------
//! @brief ピースの移動処理
//---------------------------------------------------------------------------
void MoveWinStrategy::Move(Piece* piece, float delta_time)
{
    //サインカーブで上下させたい
    sin_curve_rot_  += delta_time * 2.0f;
    float  t         = sinf(sin_curve_rot_) * 0.5f + 0.5f;    // 0~1の範囲に正規化
    float  y_offset  = ((t + sinf(90.0f)) * 0.5f) + 0.5f;     // 上下移動のオフセット量
    float3 pos       = piece->GetTranslate();
    pos.y            = y_offset;
    piece->SetTranslate(pos);

    // ピースをY軸回転させる
    rot_y_ += ROTATION_SPEED_;
    piece->SetRotationAxisXYZ(float3(0.0f, rot_y_, 0.0f));
}
