//---------------------------------------------------------------------------------
//! @file   DXLibUtils.cpp
//! @brief  DXLibの拡張ライブラリ
//! @author 山﨑愛
//---------------------------------------------------------------------------------
#include "DXLibUtils.h"
#include "HlslppUseful.h"
//---------------------------------------------------------------------------------
//! @brief 3Dのベジェ曲線を描画する関数
//---------------------------------------------------------------------------------
void DrawBezierCurve3D(const std::vector<float3>& control_points, int num_segments, int color)
{
    float t_per_segment = 1.0f / num_segments;

    std::vector<float3> bezier_points;
    bezier_points.reserve(num_segments + 1);

    for(int i = 0; i <= num_segments; ++i) {
        float t = t_per_segment * i;
        bezier_points.push_back(Bezier3D(control_points, t));
    }

    DxLib::SetUseLighting(FALSE);
    for(int i = 0; i < num_segments; ++i) {
        DrawLine3D(cast(bezier_points[i]), cast(bezier_points[i + 1]), color);
    }
    DxLib::SetUseLighting(TRUE);
}

//---------------------------------------------------------------------------------
//! @brief 矢印を描画する関数
//---------------------------------------------------------------------------------
void drawArrow(const float3& position, const float3& direction, int color, float arrow_scale)
{
    float3 s = position;
    float3 e = position + direction;

    DxLib::DrawLine3D(cast(s), cast(e), color);    // 矢印の本体

    if(arrow_scale <= 0.0f) {
        return;    // スケールが0以下の場合は描画しない
    }

    // 矢印の先端部分を描画
    float3 arrow_tail = lerp(s, e, 0.8f);

    //drawSphere(arrow_tail, 0.1f, color);

    float3 dir_x = cross(direction, float3(0.0f, 1.0f, 0.0f));    // 矢印の方向に垂直なベクトル

    if(dot(dir_x, dir_x) < float1(0.00001f)) {    // directionが真上か真下の場合
        dir_x = cross(direction, float3(1.0f, 0.0f, 0.0f));
    }

    float3 dir_y = cross(direction, dir_x);

    // 長さをarrow_scaleに正規化
    arrow_scale *= 0.05f;    // 矢印の先端部分の長さを調整

    dir_x = normalize(dir_x) * arrow_scale;    // 矢印の先端の横幅
    dir_y = normalize(dir_y) * arrow_scale;    // 矢印の先端の高さ

    // 4点の座標を計算
    float3 p1 = arrow_tail - dir_x + dir_y;    // 左上  p1+------+p2
    float3 p2 = arrow_tail + dir_x + dir_y;    // 右上    |      |
    float3 p3 = arrow_tail + dir_x - dir_y;    // 右下    |      |
    float3 p4 = arrow_tail - dir_x - dir_y;    // 左下  p4+------+p3

    DxLib::SetLightEnable(FALSE);
    // 矢印の先端部分を描画
    DxLib::DrawLine3D(cast(p1), cast(p2), color);    // 上辺
    DxLib::DrawLine3D(cast(p2), cast(p3), color);    // 右辺
    DxLib::DrawLine3D(cast(p3), cast(p4), color);    // 下辺
    DxLib::DrawLine3D(cast(p4), cast(p1), color);    // 左辺

    DxLib::DrawLine3D(cast(p1), cast(e), color);    // 左上から矢印の先端へ
    DxLib::DrawLine3D(cast(p2), cast(e), color);    // 右上から矢印の先端へ
    DxLib::DrawLine3D(cast(p3), cast(e), color);    // 右下から矢印の先端へ
    DxLib::DrawLine3D(cast(p4), cast(e), color);    // 左下から矢印の先端へ
    DxLib::SetLightEnable(TRUE);
}

//--------------------------------------------------------------------------------
//! @brief 3Dのベジェ曲線矢印を描画する関数
//--------------------------------------------------------------------------------
void DrawBezierArrow3D(const std::vector<float3>& control_points, int num_segments, int color)
{
    float t_per_segment = 1.0f / num_segments;

    std::vector<float3> bezier_points;
    bezier_points.reserve(num_segments + 1);

    for(int i = 0; i <= num_segments; ++i) {
        float t = t_per_segment * i;
        bezier_points.push_back(Bezier3D(control_points, t));
    }

    DxLib::SetUseLighting(FALSE);
    for(int i = 0; i < num_segments; ++i) {
        if(i < num_segments - 1) {
            //最後のセグメントでないなら
            DrawLine3D(cast(bezier_points[i]), cast(bezier_points[i + 1]), color);
        }
        else {
            //最後のセグメントなら矢印を描画
            drawArrow(bezier_points[i], bezier_points[i + 1] - bezier_points[i], color, 5.0f);
        }
    }
    DxLib::SetUseLighting(TRUE);
}
