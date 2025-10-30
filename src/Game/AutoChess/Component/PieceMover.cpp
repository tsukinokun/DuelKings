//---------------------------------------------------------------------------
//!	@file	PieceMover.cpp
//! @brief	ピースの移動機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceMover.h>
#include <Game/AutoChess/Piece/Piece.h>
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
        //---------------------------------------------------------
        //最も近い敵の駒の場所を調べる
        //---------------------------------------------------------
        auto this_piece       = dynamic_pointer_cast<Piece>(GetOwnerPtr());    //ピースであることが前提
        auto this_piece_owner = this_piece->GetOwner();                        //自身のピースのエージェントを取得
        //所有されていなければ処理を抜ける
        if(!this_piece_owner) {
            return;
        }
        //最も近かった敵の駒との距離を保存する変数
        float min_distance = FLT_MAX;
        // 最も近い敵の駒の位置を保存する変数
        float3 nearest_enemy_position = float3(0.0f, 0.0f, 0.0f);
        // 自分の位置を取得
        float3 translate = this_piece->GetTranslate();
        //最も近い敵の駒を探す
        for(auto other_piece : Scene::Object::GetArray<Piece>()) {
            //そのピースが自分と同じチームならスルー
            if(auto other_piece_owner = other_piece->GetOwner()) {
                //同じインスタンスを指しているならスルーする。
                if(other_piece_owner == this_piece_owner)
                    continue;
            }
            else {
                //所有者がいないピースはスルー
                continue;
            }
            //そのピースがNoUpdate状態ならスルー
            if(other_piece->GetStatus(Object::StatusBit::NoUpdate)) {
                continue;
            }
            //敵の駒との距離を計算
            float distance = length(other_piece->GetTranslate() - translate);
            //最も近い敵の駒の位置を更新
            if(distance < min_distance) {
                min_distance           = distance;
                nearest_enemy_position = other_piece->GetTranslate();
            }
        }
        //---------------------------------------------------------
        //敵の駒に向かって移動
        //---------------------------------------------------------
        //敵の駒が見つかった場合(FLT_MAX以外の値の場合は敵の駒が見つかっている)
        if(min_distance != FLT_MAX) {
            //移動方向を計算
            float3 direction = normalize(nearest_enemy_position - translate);
            //移動速度を取得
            float move_speed = this_piece->GetMoveSpeed();
            //移動量を計算
            float3 move_amount = direction * move_speed * update_delta_time_;
            //移動
            this_piece->AddTranslate(move_amount, false, true);
        }
    };
    SetProc("PieceMoveProc", move_proc, ProcTiming::Update, ProcPriority::NONE);
}
