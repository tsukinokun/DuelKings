//---------------------------------------------------------------------------
//!	@file	UISynergy.cpp
//! @brief	UIのシナジーオブジェクト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include "UISynergy.h"
#include <System/UIComponent/ComponentSynergyUI.h>
#include <Game/AutoChess/system/ImageBuffer.h>
//---------------------------------------------------------------------------------
//!	@brief 初期化
//---------------------------------------------------------------------------------
bool UISynergy::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    //	シナジー機能コンポーネントの追加
    //---------------------------------------------------------------------------------
    auto synergy_comp = AddComponent<ComponentSynergyUI>();    // 画像機能コンポーネントを追加
    //デフォルト画像を設定
    synergy_comp->SetSynergyImage(ImageBuffer::GetImageHandle("deff"));    // 画像コンポーネントに画像を設定
    synergy_component_ = synergy_comp;                                     // weak_ptrとして保持
    return true;
}

//-------------------------------------------------
//! @brief シナジー画像の設定関数
//-------------------------------------------------
std::shared_ptr<ComponentSynergyUI> UISynergy::SetSynergyImage(int image_handle)
{
    if(auto synergy_comp = synergy_component_.lock()) {
        synergy_comp->SetSynergyImage(image_handle);    // 画像コンポーネントに画像を設定
    }
    return dynamic_pointer_cast<ComponentSynergyUI>(shared_from_this());
}

//-------------------------------------------------
//! @brief 次のレベルまでの必要数を設定する関数
//-------------------------------------------------
std::shared_ptr<ComponentSynergyUI> UISynergy::SetNextCount(int next_count)
{
    if(auto synergy_comp = synergy_component_.lock()) {
        synergy_comp->SetNextCount(next_count);    // 次のレベルまでの必要数を設定
    }
    return dynamic_pointer_cast<ComponentSynergyUI>(shared_from_this());
}

//-------------------------------------------------
//! @brief シナジーの数を設定する関数
//-------------------------------------------------
std::shared_ptr<ComponentSynergyUI> UISynergy::SetSynergyCount(int synergy_count)
{
    if(auto synergy_comp = synergy_component_.lock()) {
        synergy_comp->SetSynergyCount(synergy_count);    // シナジーの数を設定
    }
    return dynamic_pointer_cast<ComponentSynergyUI>(shared_from_this());
}
