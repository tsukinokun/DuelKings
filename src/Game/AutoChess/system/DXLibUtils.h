//---------------------------------------------------------------------------------
//! @file DXLibUtils.h
//! @brief DXLibの拡張ライブラリ
//! @author 山﨑愛
//---------------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
//---------------------------------------------------------------------------------
// 3Dのベジェ曲線を描画する関数
//! @param control_points 制御点のリスト
//! @param num_segments 曲線を分割するセグメント数
//! @param thickness 曲線の太さ
//! @param color 曲線の色（ARGB形式）
//--------------------------------------------------------------------------------
void DrawBezierCurve3D(const std::vector<float3>& control_points, int num_segments = 100, int color = 0xFFFFFFFF);

//--------------------------------------------------------------------------------
// 矢印を描画する関数
//! @param  [in]    position       矢印の位置
//! @param  [in]    direction      矢印の方向
//! @param  [in]    color          矢印の色
//! @param  [in]    arrow_scale    矢印のスケール
//--------------------------------------------------------------------------------
void drawArrow(const float3& position, const float3& direction, int color, float arrow_scale = 1.0f);

//--------------------------------------------------------------------------------
// 3Dのベジェ曲線矢印を描画する関数
//! @param control_points 制御点のリスト
//! @param num_segments 曲線を分割するセグメント数
//! @param thickness 曲線の太さ
//! @param color 曲線の色（ARGB形式）
//--------------------------------------------------------------------------------
void DrawBezierArrow3D(const std::vector<float3>& control_points, int num_segments = 100, int color = 0xFFFFFFFF);
