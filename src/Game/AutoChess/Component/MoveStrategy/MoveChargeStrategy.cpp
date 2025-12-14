//---------------------------------------------------------------------------
//!	@file	MoveChargeStrategy.cpp
//! @brief	ピースの移動ストラテジークラス(突進)
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MoveChargeStrategy.h"
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------------------------
//! @brief デフォルトコンストラクタ
//---------------------------------------------------------------------------
MoveChargeStrategy::MoveChargeStrategy()
{
    //こちらでは特に処理なし、引数なしでも生成できるように作っておく
}

//---------------------------------------------------------------------------
//! @brief 引数付きコンストラクタ
//---------------------------------------------------------------------------
MoveChargeStrategy::MoveChargeStrategy(const float3& direction)
{
    SetChargeDirection(direction);
}

//---------------------------------------------------------------------------
//! @brief ピースの移動処理
//---------------------------------------------------------------------------
void MoveChargeStrategy::Move(Piece* piece, float delta_time)
{
    //突進速度を計算
    float charge_speed = piece->GetMoveSpeed() * charge_speed_multiplier_;
    //移動方向に突進速度を掛けたベクトルを取得
    float3 move_vector = charge_direction_ * charge_speed * delta_time;
    //ピースの位置を更新
    piece->AddTranslate(move_vector, false, true);
}

//---------------------------------------------------------------------------
//! @brief 突進方向の設定
//---------------------------------------------------------------------------
void MoveChargeStrategy::SetChargeDirection(const float3& direction)
{
    charge_direction_ = direction;                       // 突進方向を設定
    charge_direction_ = normalize(charge_direction_);    // 正規化
}
