//---------------------------------------------------------------------------
//!	@file	PieceMaxText.cpp
//! @brief	ピースの最大所持数表示テキスト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include "PieceMaxText.h"
#include <System/Component/Component.h>
#include <Game/AutoChess/Player.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool PieceMaxText::Init()
{
    __super::Init();
    SetName("PieceMaxText");                           //名前設定
    SetFontSize(80);                                   //フォントサイズ設定
    SetColor(GetColor(0, 0, 0), GetColor(0, 0, 0));    //文字色設定
    //---------------------------------------------------------------------------------
    //プレイヤーからレベルを取得して表示する処理を入れ込む
    //---------------------------------------------------------------------------------

    auto set_text_proc = [this]() {
        auto player = Scene::Object::Get<Player>();
        int  level  = player->GetAgentLevel();
        SetText(std::to_string(level));
    };
    SetProc("set_level", set_text_proc, ProcTiming::Update, ProcPriority::NONE);
    return true;
}
