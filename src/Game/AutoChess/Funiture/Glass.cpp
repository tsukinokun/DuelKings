//---------------------------------------------------------------------------
//!	@file	Glass.cpp
//! @brief	インゲームシーンのカメラ
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Glass.h"
#include <System/Graphics/Model.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Glass::Init()
{
    __super::Init();
    model_ = std::make_unique<Model>("data/AutoChess/Model/Stage/table.mv1");
    SetScaleAxisXYZ(0.2f);
    SetTranslate(float3(0.0f, -10.0f, 0.0f));
    auto render_proc = [this]() {
        matrix mat = GetMatrix();
        model_->setWorldMatrix(mat);
        model_->render();
    };
    SetProc("render_proc", render_proc, ProcTiming::HDR, ProcPriority::NORMAL);
    SetName("Glass");
    return true;
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void Glass::GUI()
{
    __super::GUI();
}
