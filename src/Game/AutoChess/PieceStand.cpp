//---------------------------------------------------------------------------
//!	@file	PieceStand.cpp
//! @brief	インゲームシーンのピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceStand.h"
#include <Game/AutoChess/Piece/Piece.h>
#include "Square.h"
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Player.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool PieceStand::Init()
{
    __super::Init();
    SetTranslate(float3(0.0f, 0.0f, -6.0f));
    SetName("PieceStand");
    CreateSquare();
    //---------------------------------------------------------------------------------
    //  更新処理を追加
    //---------------------------------------------------------------------------------
    auto update = [this]() {
        //ピースが変更されたかを確認
        for(int i = 0; i < STAND_SQUARE_MAX_; i++) {
            if(auto square = squares_[i].lock()) {
                if(square->IsChanged()) {
                    if(auto player = Scene::Object::Get<Player>()) {
                        PieceInfo piece_info;    //空のピース情報
                        //マスに中身があれば
                        auto piece_wp = square->GetPutPiece();
                        if(auto piece = piece_wp.lock()) {
                            piece_info.SetTypeName(piece->GetNameDefault().data());
                            piece_info.SetOwner(player);
                        }
                        player->SetPieceStandInfo(i, piece_info);
                    }
                }
            }
        }
    };
    SetProc("Update", update, ProcTiming::Update, ProcPriority::NONE);
    return true;
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void PieceStand::Draw()
{
    __super::Draw();
    for(int s = 0; s < STAND_SQUARE_MAX_; s++) {
        int color = GetColor(0, 255, 0);
        //ファイルとランクの合計値が偶数なら白に
        if((s % 2) == 0) {
            color = GetColor(0, 255, 255);
        }
        float3 curr_translate = GetTranslate();    //現在のポジションを取得
        float  x              = (s * SQUARE_SIZE) - STAND_SQUARE_HALF_ * (SQUARE_SIZE);
        float3 p1             = float3(x + -SQUARE_HALF, -0.1f, -SQUARE_HALF) + curr_translate;
        float3 p2             = float3(x + SQUARE_HALF, 0.1f, SQUARE_HALF) + curr_translate;
        DrawCube3D(cast(p1), cast(p2), color, color, TRUE);
    }
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void PieceStand::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void PieceStand::GUI()
{
    __super::GUI();
}

//---------------------------------------------------------------------------------
//!	ピースの初期化
//---------------------------------------------------------------------------------
void PieceStand::AddPiece(std::shared_ptr<Piece> piece)
{
    //バッファを確認
    for(int i = 0; i < squares_.size(); i++) {
        //ヌルポインタなら
        if(auto square = squares_[i].lock()) {
            if(square->GetPutPiece().expired()) {
                piece->SetTranslate(float3((i * SQUARE_SIZE) - STAND_SQUARE_HALF_ * (SQUARE_SIZE), 0.5f, 0.0f) + GetTranslate());    //位置を設定
                square->SetPutPiece(piece);    //バッファにポインタを登録
                return;
            }    //一度生成したらリターンする
        }
    }
}

//---------------------------------------------------------------------------------
//!	オーナーを設定
//---------------------------------------------------------------------------------
void PieceStand::SetOwner(std::weak_ptr<Object> owner)
{
    owner_ = owner;
}

//---------------------------------------------------------------------------------
//!	マスの生成
//---------------------------------------------------------------------------------
void PieceStand::CreateSquare()
{
    for(int i = 0; i < STAND_SQUARE_MAX_; i++) {
        auto square = Scene::Object::Create<Square>();
        square->SetOwner(owner_);
        square->SetTranslate(float3((i * SQUARE_SIZE) - STAND_SQUARE_HALF_ * (SQUARE_SIZE), 0.0f, 0.0f) + GetTranslate());
        squares_[i] = square;
    }
}

//---------------------------------------------------------------------------------
//!	マスのウィークポインタを取得
//---------------------------------------------------------------------------------
std::array<std::weak_ptr<Square>, 8> PieceStand::GetSquarePtrArray()
{
    return squares_;
}
//---------------------------------------------------------------------------
// ピースのスタンドが満タンかどうかを取得する関数
//! @retval 満タンの場合 true、そうでない場合 false
//---------------------------------------------------------------------------
bool PieceStand::IsFull() const
{
    for(int i = 0; i < squares_.size(); i++) {
        if(auto square = squares_[i].lock()) {
            if(square->GetPutPiece().expired()) {
                return false;    //空きマスがあったら false を返す
            }
        }
    }
    return true;    //全てのマスが埋まっていたら true を返す
}
