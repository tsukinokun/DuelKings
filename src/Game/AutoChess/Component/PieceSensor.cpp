//---------------------------------------------------------------------------
//!	@file	PieceSensor.cpp
//! @brief	ピースの敵探索機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void PieceSensor::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // 移動処理
    //---------------------------------------------------------
    auto piece_sensor_proc = [this]() {
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
        //最も近い敵の駒を保存する一時変数
        std::shared_ptr<Piece> nearest_enemy = nullptr;
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
                min_distance  = distance;       //最も近い敵との距離を更新
                nearest_enemy = other_piece;    //最も近い敵の駒を保存
            }
        }
        //最も近い敵の駒を保存
        nearest_enemy_ = nearest_enemy;
    };
    SetProc("piece_sensor_proc", piece_sensor_proc, ProcTiming::Update, ProcPriority::HIGH);
}

//----------------------------------------------------------
//! 最も近い敵の駒を取得
//----------------------------------------------------------
std::shared_ptr<Piece> PieceSensor::GetNearestEnemy() const
{
    return nearest_enemy_.lock();    //weak_ptrを共有ポインタに変換
}
