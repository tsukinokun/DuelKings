//---------------------------------------------------------------------------
//!	@file	MoveTrackingStrategy.cpp
//! @brief	ピースの移動ストラテジークラス(突進)
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MoveTrackingStrategy.h"
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------------------------
//! @brief デフォルトコンストラクタ
//---------------------------------------------------------------------------
MoveTrackingStrategy::MoveTrackingStrategy()
{
    //こちらでは特に処理なし、引数なしでも生成できるように作っておく
}

//---------------------------------------------------------------------------
//! @brief 引数付きコンストラクタ
//---------------------------------------------------------------------------
MoveTrackingStrategy::MoveTrackingStrategy(std::weak_ptr<Piece> target_piece, const float3& offset, float move_speed_rate)
    : target_piece_(target_piece)
    , offset_(offset)
    , move_speed_rate_(move_speed_rate)
{
}

//---------------------------------------------------------------------------
//! @brief ピースの移動処理
//---------------------------------------------------------------------------
void MoveTrackingStrategy::Move(Piece* piece, float delta_time)
{
    //突進速度を計算
    float charge_speed = piece->GetMoveSpeed() * move_speed_rate_;
    //追尾対象ピースの共有ポインタを取得
    std::shared_ptr<Piece> target_piece = target_piece_.lock();
    if(target_piece) {
        //追尾対象ピースの位置を取得
        float3 target_position = target_piece->GetTranslate() + offset_;
        //現在のピースの位置を取得
        float3 current_position = piece->GetTranslate();
        //目標位置への方向ベクトルを計算
        float3 direction = target_position - current_position;
        //方向ベクトルを正規化
        direction = normalize(direction);
        //移動量を計算
        float3 move_vector = direction * charge_speed * delta_time;
        //ピースの位置を更新
        piece->AddTranslate(move_vector, false, true);
    }
}
