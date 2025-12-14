//---------------------------------------------------------------------------
//!	@file	MoveToNearestEnemyStrategy.cpp
//! @brief	ピースの移動ストラテジークラス(最も近い敵へ移動)
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MoveToNearestEnemyStrategy.h"
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Component/PieceSensor.h>
//---------------------------------------------------------------------------
//! @brief ピースの移動処理
//---------------------------------------------------------------------------
void MoveToNearestEnemyStrategy::Move(Piece* piece, float delta_time)
{
    //センサーを取得
    if(auto sensor = piece->GetComponent<PieceSensor>()) {
        //敵が存在していたら
        //---------------------------------------------------------
        //敵の駒に向かって移動
        //---------------------------------------------------------
        if(auto nearest_enemy = sensor->GetNearestEnemy()) {    //最も近い敵を取得
            //自分の位置を取得
            float3 translate = piece->GetTranslate();
            //敵の位置を取得
            float3 nearest_enemy_position = nearest_enemy->GetTranslate();
            //移動方向を計算
            float3 direction = nearest_enemy_position - translate;
            //ベクトルの大きさが射程距離以下なら移動しない
            float distance = length(direction);
            if(distance <= piece->GetAttackRange()) {
                return;
            }
            //方向を正規化
            direction = normalize(direction);
            //移動速度を取得
            float move_speed = piece->GetMoveSpeed();
            //移動量を計算
            float3 move_amount = direction * move_speed * delta_time;
            //移動
            piece->AddTranslate(move_amount, false, true);
        }
    }
}
