//---------------------------------------------------------------------------
//!	@file	HlslppUseful.cpp
//! @brief	Hlslppの拡張
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "HlslppUseful.h"
//---------------------------------------------------------------------------------
//!	マウス座標を float2 で取得
//---------------------------------------------------------------------------------
float2 GetMouseFloat2()
{
    float2 pos;
    pos.x = (float)GetMouseX();
    pos.y = (float)GetMouseY();
    return pos;
}
//---------------------------------------------------------------------------
//	2Dの四角同士の当たり判定（座標と大きさに Float2 を渡すバージョン）
//---------------------------------------------------------------------------
bool CheckBoxCenterHit(const float2& box_pos1, const float2& box_size1, const float2& box_pos2, const float2& box_size2)
{
    if(box_pos1.x + box_size1.x / 2 >= box_pos2.x - box_size2.x / 2 && box_pos1.x - box_size1.x / 2 <= box_pos2.x + box_size2.x / 2) {
        if(box_pos1.y + box_size1.y / 2 >= box_pos2.y - box_size2.y / 2 && box_pos1.y - box_size1.y / 2 <= box_pos2.y + box_size2.y / 2) {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
//	2Dの四角と点の当たり判定（座標と大きさに Float2 を渡すバージョン）
//---------------------------------------------------------------------------
bool CheckBoxPointHit(const float2& box_pos, const float2& box_size, const float2& point)
{
    if(box_pos.x + box_size.x / 2 >= point.x && box_pos.x - box_size.x / 2 <= point.x) {
        if(box_pos.y + box_size.y / 2 >= point.y && box_pos.y - box_size.y / 2 <= point.y) {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
// ワールド座標をスクリーン座標に変換する関数
//---------------------------------------------------------------------------
float2 WorldPositionToScreenPosition(const float3& world_position)
{
    float2 pixel_position = float2(0.0f, 0.0f);
    if(auto camera = Scene::GetCurrentCamera().lock()) {
        matrix view_matrix      = camera->GetViewMatrix();          //ビュー行列
        matrix proj_matrix      = camera->GetProjectionMatrix();    //投影行列
        matrix view_proj_matrix = mul(view_matrix, proj_matrix);
        float4 screen_position  = mul(float4(world_position, 1.0f), view_proj_matrix);
        screen_position.xyz     = screen_position.xyz / screen_position.w;
        // スクリーン座標(-1～+1)→UV座標(0～1)
        float2 uv      = screen_position.xy * float2(0.5f, -0.5f) + 0.5f;
        pixel_position = uv * float2(WINDOW_W, WINDOW_H);
    }
    return pixel_position;
}
