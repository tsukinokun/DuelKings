//---------------------------------------------------------------------------
//!	@file	JapaneseChessKing.cpp
//! @brief	将棋の王将クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessKing.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessKing::Init()
{
    __super::Init();
    SetName("JapaneseChessKing");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/King.mv1");
    model->SetScaleAxisXYZ(0.005f);
    model->SetRotationAxisXYZ(float3(0.0f, 90.0f, 0.0f));

    return true;
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void JapaneseChessKing::Draw()
{
    __super::Draw();
    float3 pos = GetTranslate();
    //DrawSphere3D(cast(pos), RADIUS_, 16, GetColor(255, 0, 255), GetColor(255, 0, 255), TRUE);
    //選択中なら
    if(is_selected_) {
        //位置を取得
        pos       = GetTranslate();
        pos.y     = 0.0f;    //y座標を0にする
        float3 p1 = float3(-SQUARE_HALF, -0.1f, -SQUARE_HALF) + pos;
        float3 p2 = float3(SQUARE_HALF, 0.1f, SQUARE_HALF) + pos;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
        DrawCube3D(cast(p1), cast(p2), GetColor(255, 0, 255), GetColor(255, 0, 255), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    }
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void JapaneseChessKing::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void JapaneseChessKing::GUI()
{
    __super::GUI();
}
