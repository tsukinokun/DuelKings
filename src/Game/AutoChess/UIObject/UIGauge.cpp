//---------------------------------------------------------------------------
//!	@file	UIGauge.cpp
//! @brief	UIのゲージオブジェクト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include "UIGauge.h"
#include <System/UIComponent/ComponentGauge.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool UIGauge::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    //	ゲージ機能コンポーネントの追加
    //---------------------------------------------------------------------------------
    auto gauge_comp  = AddComponent<ComponentGauge>();    // ゲージ機能コンポーネントを追加
    gauge_component_ = gauge_comp;                        // weak_ptrとして保持
    return true;
}

//---------------------------------------------------------------------------
//! ゲージの割合を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<UIGauge> UIGauge::SetGaugeSize(const int2& size)
{
    if(auto gauge_comp = gauge_component_.lock()) {
        gauge_comp->SetGaugeSize(size);    // ゲージコンポーネントにサイズを設定
    }
    return dynamic_pointer_cast<UIGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! 滑らかに変化する時間を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<UIGauge> UIGauge::SetDuration(float duration)
{
    if(auto gauge_comp = gauge_component_.lock()) {
        gauge_comp->SetDuration(duration);    // ゲージコンポーネントに滑らかに変化する時間を設定
    }
    return dynamic_pointer_cast<UIGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! ゲージの割合を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<UIGauge> UIGauge::SetGaugeRate(float rate)
{
    if(auto gauge_comp = gauge_component_.lock()) {
        gauge_comp->SetGaugeRate(rate);    // ゲージコンポーネントに割合を設定
    }
    return dynamic_pointer_cast<UIGauge>(shared_from_this());
}

//---------------------------------------------------------------------------
//! ゲージの色を設定する関数
//---------------------------------------------------------------------------
std::shared_ptr<UIGauge> UIGauge::SetGaugeColor(int color, int smooth_color)
{
    if(auto gauge_comp = gauge_component_.lock()) {
        gauge_comp->SetGaugeColor(color, smooth_color);    // ゲージコンポーネントに色を設定
    }
    return dynamic_pointer_cast<UIGauge>(shared_from_this());
}
