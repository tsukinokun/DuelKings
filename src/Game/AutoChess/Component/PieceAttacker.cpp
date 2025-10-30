//---------------------------------------------------------------------------
//!	@file	PieceAttacker.cpp
//! @brief	ピースの攻撃機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceAttacker.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void PieceAttacker::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // 攻撃処理
    //---------------------------------------------------------
    auto attack_proc = [this]() {
        //---------------------------------------------------------
        //最も近い敵の駒を攻撃する(通常攻撃)
        //---------------------------------------------------------
        auto this_piece = dynamic_pointer_cast<Piece>(GetOwnerPtr());    //ピースであることが前提

        //---------------------------------------------------------
        //最も近い敵の駒に攻撃を仕掛ける
        //---------------------------------------------------------
        if(auto sensor = this_piece->GetComponent<PieceSensor>()) {
            //敵が存在していたら
            if(auto nearest_enemy = sensor->GetNearestEnemy()) {    //最も近い敵を取得
                //自分の位置を取得
                float3 translate = this_piece->GetTranslate();
                //敵の位置を取得
                float3 nearest_enemy_position = nearest_enemy->GetTranslate();
                //攻撃方向を計算
                float3 direction = nearest_enemy_position - translate;
                //ベクトルの大きさが射程距離以下なら攻撃
                float distance = length(direction);
                if(distance <= this_piece->GetAttackRange()) {
                    //攻撃力を取得
                    int attack_power = this_piece->GetAttackPower();
                    //敵のHPを減少させる
                    nearest_enemy->TakeDamage(attack_power);
                }
            }
        }
    };
    SetProc("attack_proc", attack_proc, ProcTiming::Update, ProcPriority::NORMAL);
}
