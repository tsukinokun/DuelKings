//---------------------------------------------------------------------------------
//! @file   DXLibUtils.cpp
//! @brief  DXLibの拡張ライブラリ
//! @author 山﨑愛
//---------------------------------------------------------------------------------
#include "DXLibUtils.h"
#include "HlslppUseful.h"
void DrawBezierCurve3D(const std::vector<float3>& control_points, int num_segments, int color)
{
    //補間係数の上がり幅を計算
    float t_per_segment = 1.0f / num_segments;
    //ベジェ曲線のそれぞれの点を計算
    std::vector<float3> bezier_points;
    for(int i = 0; i < num_segments; ++i) {
        bezier_points.push_back(Bezier3D(control_points, i * t_per_segment));
    }
    //ベジェ曲線の点を描画
    for(int i = 0; i < bezier_points.size() - 1; ++i) {
        DrawLine3D(cast(bezier_points[i]), cast(bezier_points[i + 1]), color);
    }
}
