//---------------------------------------------------------------------------
//!	@file	PieceTargetLocker.cpp
//! @brief	ピースのターゲット固定機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceTargetLocker.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------
//! @brief  初期化
//---------------------------------------------------------
void PieceTargetLocker::Init()
{
    __super::Init();
    auto update_proc = [this]() {
        // ターゲットロックが有効な時間を減少させる
        if(active_time_ > 0.0f) {
            active_time_ -= GetDeltaTime();
            if(active_time_ <= 0.0f) {
                // ターゲットロックが無効になったら、ロックしているターゲットを解除する
                locked_target_ = nullptr;
            }
        }
    };
}

//---------------------------------------------------------
//! @brief  ターゲットをロックする関数
//---------------------------------------------------------
void PieceTargetLocker::LockTarget(Piece* target, float duration)
{
    locked_target_ = target;
    active_time_   = duration;
}
