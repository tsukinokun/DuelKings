//---------------------------------------------------------------------------
//!	@file	ComponentButton.cpp
//! @brief	ボタンコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentButton.h"
#include "ComponentTransformUI.h"
#include "ComponentImage.h"
#include <Game/AutoChess/system/HlslppUseful.h>
//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentButton::Init()
{    // 初期化処理
    __super::Init();
}
//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentButton::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"ボタンコンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}
//---------------------------------------------------------------------------
//! @brief	クリックされているかを返す関数
//---------------------------------------------------------------------------
bool ComponentButton::IsClick() const
{
    //左クリックされていれば
    if(IsMouseDown(MOUSE_INPUT_LEFT)) {
        return IsMouseOver();    //マウスがボタンに触れているかを返す
    }
    return false;
}
//---------------------------------------------------------------------------
//  マウスがボタンに触れているかを返す関数
//! @return マウスがボタンに触れているか
//---------------------------------------------------------------------------
bool ComponentButton::IsMouseOver() const
{
    // とりあえずオーナーを取得
    auto owner = GetOwner();
    //ComponentImageがあることを確認
    if(auto image_comp = owner->GetComponent<ComponentImage>()) {
        //マウス座標を取得
        float2 mouse_pos = GetMouseFloat2();
        //オーナー(UI)座標を取得
        float3 translate = owner->GetTranslate() + image_comp->GetAdjustment();
        float2 ui_pos    = float2(translate.x, translate.y);
        //UIサイズを取得
        float2 ui_size = float2(0.0f, 0.0f);                  //とりあえず宣言
        ui_size        = image_comp->GetScreenImageSize();    //サイズ取得
        if(CheckBoxPointHit(ui_pos, ui_size, mouse_pos)) {
            return true;
        }
    }
    return false;
}
CEREAL_REGISTER_TYPE(ComponentButton)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentButton)
