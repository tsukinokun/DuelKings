//---------------------------------------------------------------------------
//!	@file	PieceMover.cpp
//! @brief	ピースの移動機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Component/PieceMover.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/Component/MoveStrategy/MoveToNearestEnemyStrategy.h>
#include <Game/AutoChess/Component/StatusEffect/StunStatus.h>
//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void PieceMover::Init()
{
    __super::Init();
    move_strategy_ = std::make_unique<MoveToNearestEnemyStrategy>();
    //---------------------------------------------------------
    // 更新処理の登録
    //---------------------------------------------------------
    auto update_proc = [this]() {
        auto this_piece = dynamic_pointer_cast<Piece>(GetOwnerPtr());    //オーナーはピースであることが前提
        //---------------------------------------------------------
        // スタンチェックをして、スタン中なら処理を抜ける
        //---------------------------------------------------------
        if(this_piece->GetComponent<StunStatus>()) {
            return;
        }
        //---------------------------------------------------------
        // ピースの移動処理
        //---------------------------------------------------------
        if(move_strategy_ && this_piece) {
            move_strategy_->Move(this_piece.get(), update_delta_time_);
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NONE);
}

//---------------------------------------------------------------------------
//! @brief ピースの移動処理を設定する関数
//---------------------------------------------------------------------------
void PieceMover::SetMoveStrategy(std::unique_ptr<IMoveStrategy> strategy)
{
    move_strategy_ = std::move(strategy);
}
