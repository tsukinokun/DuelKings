//---------------------------------------------------------------------------
//!	@file	PieceStand.cpp
//! @brief	インゲームシーンのピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceStand.h"
#include "Piece.h"
#include <Game/AutoChess/system/GameConst.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool PieceStand::Init()
{
    __super::Init();
    SetTranslate(float3(5.0f, 0.0f, 0.0f));
    SetName("PieceStand");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void PieceStand::Update()
{
    __super::Update();
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
        float  z              = (s * SQUARE_SIZE) - STAND_SQUARE_HALF_ * (SQUARE_SIZE);
        float3 p1             = float3(-SQUARE_HALF, -0.1f, z + -SQUARE_HALF) + curr_translate;
        float3 p2             = float3(SQUARE_HALF, 0.1f, z + SQUARE_HALF) + curr_translate;
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
void PieceStand::PieceInit()
{
    //バッファを確認
    for(int i = 0; i < pieces_.size(); i++) {
        //ヌルポインタなら
        if(pieces_[i].expired()) {
            auto piece = Scene::Object::Create<Piece>();                                                                         //ピースを生成
            piece->SetTranslate(float3(0.0f, 0.5f, (i * SQUARE_SIZE) - STAND_SQUARE_HALF_ * (SQUARE_SIZE)) + GetTranslate());    //位置を設定
            pieces_[i] = piece;                                                                                                  //バッファにポインタを登録
            return;                                                                                                              //一度生成したらリターンする
        }
    }
}
