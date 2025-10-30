//---------------------------------------------------------------------------
//!	@file	PieceMover.cpp
//! @brief	ピースの移動機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Component/PieceMover.h>
#include <Game/AutoChess/Component/PieceSensor.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void PieceMover::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // 移動処理
    //---------------------------------------------------------
    auto move_proc = [this]() {
        //オーナーはピースであることが前提
        auto this_piece = dynamic_pointer_cast<Piece>(GetOwnerPtr());
        //センサーを取得
        if(auto sensor = this_piece->GetComponent<PieceSensor>()) {
            //敵が存在していたら
            //---------------------------------------------------------
            //敵の駒に向かって移動
            //---------------------------------------------------------
            if(auto nearest_enemy = sensor->GetNearestEnemy()) {    //最も近い敵を取得
                //自分の位置を取得
                float3 translate = this_piece->GetTranslate();
                //敵の位置を取得
                float3 nearest_enemy_position = nearest_enemy->GetTranslate();
                //移動方向を計算
                float3 direction = nearest_enemy_position - translate;
                //ベクトルの大きさが射程距離以下なら移動しない
                float distance = length(direction);
                if(distance <= this_piece->GetAttackRange()) {
                    return;
                }
                //方向を正規化
                direction = normalize(direction);
                //移動速度を取得
                float move_speed = this_piece->GetMoveSpeed();
                //移動量を計算
                float3 move_amount = direction * move_speed * update_delta_time_;
                //移動
                this_piece->AddTranslate(move_amount, false, true);
            }
        }
    };
    SetProc("piece_move_proc", move_proc, ProcTiming::Update, ProcPriority::NONE);
}
