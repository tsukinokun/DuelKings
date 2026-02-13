//---------------------------------------------------------------------------
//!	@file	UIAnimation.cpp
//! @brief	UIのアニメーションオブジェクト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include "UIAnimation.h"
#include <System/UIComponent/ComponentAnimUI.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool UIAnimation::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    //	文字列機能コンポーネントの追加
    //---------------------------------------------------------------------------------
    auto img_comp   = AddComponent<ComponentAnimUI>();    // アニメーション機能コンポーネントを追加
    anim_component_ = img_comp;                           // weak_ptrとして保持
    return true;
}

//---------------------------------------------------------------------------
//! @brief	アニメーションステータスの設定
//---------------------------------------------------------------------------
std::shared_ptr<UIAnimation> UIAnimation::SetAnimStatus(int gh, int div_num, float ex_rate, int update_frame)
{
    if(auto anim_comp = anim_component_.lock()) {
        anim_comp->SetAnimStatus(gh, div_num, ex_rate, update_frame);    // ステータス設定
    }
    return dynamic_pointer_cast<UIAnimation>(shared_from_this());
}

//--------------------------------------------------------------------
//! @brief 透明度の設定
//--------------------------------------------------------------------
std::shared_ptr<UIAnimation> UIAnimation::SetAlpha(int alpha)
{
    if(auto anim_comp = anim_component_.lock()) {
        anim_comp->SetAlpha(alpha);    // 透明度設定
    }
    return dynamic_pointer_cast<UIAnimation>(shared_from_this());
}
