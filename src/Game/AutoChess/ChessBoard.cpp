//---------------------------------------------------------------------------
//!	@file	ChessBoard.cpp
//! @brief	インゲームシーンのチェスボード
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChessBoard.h"
#include "Square.h"
#include <Game/AutoChess/system/GameConst.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessBoard::Init()
{
    __super::Init();
    SetName("ChessBoard");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void ChessBoard::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void ChessBoard::Draw()
{
    __super::Draw();
    for(int f = 0; f < FILE_MAX_; f++) {
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
    for(int f = 0; f < FILE_MAX_; f++) {
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
std::array<std::array<std::weak_ptr<Square>, 8>, 8> ChessBoard::GetSquarePtrArray()
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
