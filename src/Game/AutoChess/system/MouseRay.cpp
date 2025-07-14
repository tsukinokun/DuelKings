//---------------------------------------------------------------------------
//!	@file	MouseRay.cpp
//! @brief	インゲームシーンのマウス光線
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MouseRay.h"
#include "HlslppUseful.h"
#include <System/Component/ComponentCollisionLine.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool MouseRay::Init()
{
    __super::Init();
    auto   col               = AddComponent<ComponentCollisionLine>();
    float2 mouse             = GetMouseFloat2();    //マウスのスクリーン座標を取得
    VECTOR mouse_world_front = ConvScreenPosToWorldPos(cast(float3(mouse, 0.0f)));
    VECTOR mouse_world_back  = ConvScreenPosToWorldPos(cast(float3(mouse, 1.0f)));
    if(auto col = GetComponent<ComponentCollisionLine>()) {
        col->SetLine(float3(mouse_world_front.x, mouse_world_front.y, mouse_world_front.z), float3(mouse_world_back.x, mouse_world_back.y, mouse_world_back.z));
    }
    SetName("MouseRay");
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void MouseRay::Update()
{
    __super::Update();
    float2 mouse             = GetMouseFloat2();    //マウスのスクリーン座標を取得
    VECTOR mouse_world_front = ConvScreenPosToWorldPos(cast(float3(mouse, 0.0f)));
    VECTOR mouse_world_back  = ConvScreenPosToWorldPos(cast(float3(mouse, 1.0f)));
    if(auto col = GetComponent<ComponentCollisionLine>()) {
        col->SetLine(float3(mouse_world_front.x, mouse_world_front.y, mouse_world_front.z), float3(mouse_world_back.x, mouse_world_back.y, mouse_world_back.z));
    }
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void MouseRay::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void MouseRay::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void MouseRay::GUI()
{
    __super::GUI();
}
