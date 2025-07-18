//---------------------------------------------------------------------------
//!	@file	Player.cpp
//! @brief	インゲームシーンのプレイヤークラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Agent.h"
#include "Player.h"
#include "Piece.h"
#include "Square.h"
#include "PieceStand.h"
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Player::Init()
{
    __super::Init();
    SetName("Player");
    auto stand = Scene::Object::Create<PieceStand>();    // ピーススタンドを作成
    stand->SetOwner(shared_from_this());
    stand->CreateSquare();
    stand_ = stand;
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void Player::Update()
{
    __super::Update();
    if(IsKeyDown(KEY_INPUT_SPACE)) {
        if(auto stand = stand_.lock()) {
            stand->PieceInit();    // ピースの初期化
        }
    }

    should_select_piece_ = false;    //選択するかをリセット
    //左クリックで選択
    if(IsMouseDown(MOUSE_INPUT_LEFT)) {
        if(auto stand = stand_.lock()) {
            auto stand_squares_ = stand->GetSquarePtrArray();
            for(int i = 0; i < stand_squares_.size(); i++) {
                if(auto square = stand_squares_[i].lock()) {
                    if(auto piece = square->GetPutPiece().lock()) {
                        piece->SetSelect(false);
                    }
                }
            }
        }
        should_select_piece_ = true;
    }

    //ドロップ操作
    should_drop_piece_ = false;    //ドロップするかをリセット
    if(IsMouseUp(MOUSE_INPUT_LEFT)) {
        if(auto stand = stand_.lock()) {
            auto stand_squares_ = stand->GetSquarePtrArray();
            for(int i = 0; i < stand_squares_.size(); i++) {
                if(auto square = stand_squares_[i].lock()) {
                    if(auto piece = square->GetPutPiece().lock()) {
                        if(piece->IsSelect()) {
                            should_drop_piece_ = true;    //ドロップする
                        }
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void Player::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void Player::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void Player::GUI()
{
    __super::GUI();
}
