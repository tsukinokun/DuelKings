//---------------------------------------------------------------------------
//!	@file	ComponentButton.cpp
//! @brief	ボタンコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentButton.h"
#include "ComponentTransformUI.h"

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
        if(ImGui::TreeNode(u8"画像コンポーネント")) {
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
}

CEREAL_REGISTER_TYPE(ComponentButton)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentButton)
