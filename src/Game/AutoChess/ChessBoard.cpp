//---------------------------------------------------------------------------
//!	@file	ChessBoard.cpp
//! @brief	インゲームシーンのチェスボード
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChessBoard.h"
#include "Square.h"
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Player.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentCollisionModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessBoard::Init()
{
    __super::Init();
    SetName("ChessBoard");
    CreateSquare();
    //---------------------------------------------------------------------------------
    // 更新処理を登録
    //---------------------------------------------------------------------------------
    auto update = [this]() {
        //マスが変更されたかを取得
        for(int f = 0; f < FILE_HALF_; f++) {
            for(int r = 0; r < RANK_MAX_; r++) {
                if(auto square = squares_[f][r].lock()) {
                    if(square->IsChanged()) {
                        if(auto player = Scene::Object::Get<Player>()) {
                            //マスに中身があれば
                            auto piece_wp = square->GetPutPiece();
                            if(auto piece = piece_wp.lock()) {
                                PieceInfo piece_info;    //空のピース情報
                                piece_info.SetTypeName(piece->GetNameDefault().data());
                                piece_info.SetOwner(player);
                                player->SetBoardInfo(f, r, piece_info);
                            }
                            else {
                                //なければ消す
                                player->RemoveBoardInfo(f, r);
                            }
                        }
                    }
                }
            }
        }
    };
    SetProc("Update", update, ProcTiming::Update, ProcPriority::NORMAL);
    return true;
}
//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void ChessBoard::Draw()
{
    DxLib::SetUseLighting(FALSE);
    __super::Draw();
    for(int f = 0; f < FILE_HALF_; f++) {
        for(int r = 0; r < RANK_MAX_; r++) {
            int color = GetColor(0, 0, 0);
            //ファイルとランクの合計値が偶数なら白に
            if(((f + r) % 2) == 0) {
                color = GetColor(255, 255, 255);
            }
            //レイに当たっていたら緑にしておく
            if(auto square = squares_[f][r].lock()) {
                if(square->IsRayHit()) {
                    color = GetColor(0, 255, 0);
                }
            }
            float  x  = (r * SQUARE_SIZE) - RANK_HALF_ * (SQUARE_SIZE);
            float  z  = (f * SQUARE_SIZE) - (FILE_HALF_ * SQUARE_SIZE);
            float3 p1 = float3(x + -SQUARE_HALF, -0.1f, z + -SQUARE_HALF);
            float3 p2 = float3(x + SQUARE_HALF, 0.1f, z + SQUARE_HALF);
            DrawCube3D(cast(p1), cast(p2), color, color, TRUE);
        }
    }
    DxLib::SetUseLighting(TRUE);
}

//---------------------------------------------------------------------------------
//!	オーナーを設定
//---------------------------------------------------------------------------------
void ChessBoard::SetOwner(std::weak_ptr<Object> owner)
{
    owner_ = owner;
}

//---------------------------------------------------------------------------------
//!	マスの生成
//---------------------------------------------------------------------------------
void ChessBoard::CreateSquare()
{
    for(int f = 0; f < FILE_HALF_; f++) {
        for(int r = 0; r < RANK_MAX_; r++) {
            auto square = Scene::Object::Create<Square>();
            square->SetOwner(owner_);
            float  x = (r * SQUARE_SIZE) - RANK_HALF_ * (SQUARE_SIZE);
            float  z = (f * SQUARE_SIZE) - (FILE_HALF_ * SQUARE_SIZE);
            float3 p = float3(x, -0.1f, z);
            square->SetTranslate(p);
            squares_[f][r] = square;
        }
    }
}

//---------------------------------------------------------------------------------
//!	マスのウィークポインタを取得
//---------------------------------------------------------------------------------
std::array<std::array<std::weak_ptr<Square>, 8>, 4> ChessBoard::GetSquarePtrArray()
{
    return squares_;
}
//---------------------------------------------------------------------------------
//!	置かれている駒の数を取得
//---------------------------------------------------------------------------------
int ChessBoard::GetPieceNumOnSquares() const
{
    int num = 0;
    for(int f = 0; f < FILE_MAX_; f++) {
        for(int r = 0; r < RANK_MAX_; r++) {
            if(auto square = squares_[f][r].lock()) {
                //あるならカウント
                if(!square->GetPutPiece().expired()) {
                    num++;
                }
            }
        }
    }
    return num;
}
//-----------------------------------------------------------
//! ボード、マス、駒の描画と更新処理を行うかを設定する関数
//-----------------------------------------------------------
void ChessBoard::SetBoardProcessEnable(bool enable)
{
    bool not_enable = !enable;
    //-----------------------------------------------------------
    // マスが所持している駒とマスの描画と更新処理
    //-----------------------------------------------------------
    for(int f = 0; f < FILE_HALF_; f++) {
        for(int r = 0; r < RANK_MAX_; r++) {
            if(auto square = squares_[f][r].lock()) {
                auto piece_wp = square->GetPutPiece();
                if(auto piece = piece_wp.lock()) {
                    piece->SetStatus(Object::StatusBit::NoDraw, not_enable);
                    piece->SetStatus(Object::StatusBit::NoUpdate, not_enable);
                }
                square->SetStatus(Object::StatusBit::NoDraw, not_enable);
                square->SetStatus(Object::StatusBit::NoUpdate, not_enable);
                //マスのコリジョンモデルの有効無効を切り替え
                if(not_enable) {
                    //コリジョンモデルがあれば削除
                    if(square->GetComponent<ComponentCollisionModel>()) {
                        square->RemoveComponent<ComponentCollisionModel>();
                    }
                }
                else {
                    if(!square->GetComponent<ComponentCollisionModel>()) {
                        //コリジョンモデルがなければ追加
                        square->AddComponent<ComponentCollisionModel>()->AttachToModel();
                    }
                }
            }
        }
    }
    //-----------------------------------------------------------
    //チェスボード自身の描画と更新
    //-----------------------------------------------------------
    SetStatus(Object::StatusBit::NoDraw, not_enable);
    SetStatus(Object::StatusBit::NoUpdate, not_enable);
}
