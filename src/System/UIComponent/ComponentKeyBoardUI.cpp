//---------------------------------------------------------------------------
//!	@file	ComponentKeyBoardUI.cpp
//! @brief	キーボードUIコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentKeyBoardUI.h"
#include <System/UIComponent/ComponentTransformUI.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentKeyBoardUI::Init()
{    // 初期化処理
    __super::Init();
    //---------------------------------------------------------------------------
    //  UI描画を登録
    //---------------------------------------------------------------------------
    auto draw_ui = [this]() {
        frame_count++;                                   //フレームのカウント
        auto   owner      = GetOwner();                  //オーナーを取得
        float3 adjustment = float3(0.0f, 0.0f, 0.0f);    // 調整値(Alignmentに合わせて)
        if(auto comp_transform = owner->GetComponent<ComponentTransformUI>()) {
            ComponentTransformUI::Alignment alignment = comp_transform->GetAlignment();
            //配置位置(縦)
            switch(static_cast<int>(alignment) / 3) {
            case 0:
                adjustment.y = (-size_.y * 0.5f);
                break;    // 上寄せ
            case 1:
                adjustment.y = 0.0f;
                break;    // 中央寄せ
            case 2:
                adjustment.y = (size_.y * 0.5f);
                break;    // 下寄せ
            }
            //配置位置(横)
            switch(static_cast<int>(alignment) % 3) {
            case 0:
                adjustment.x = (-size_.x * 0.5f);
                break;    // 左寄せ
            case 1:
                adjustment.x = 0.0f;
                break;    // 中央寄せ
            case 2:
                adjustment.x = (size_.x * 0.5f);
                break;    // 右寄せ
            }
        }
        float3 pos   = float3(0.0f, 0.0f, 0.0f);
        pos          = owner->GetTranslate() + adjustment;
        float3 scale = owner->GetScaleAxisXYZ();
        //---------------------------------------------------------------------------
        // 土台
        //---------------------------------------------------------------------------
        {
            int x1 = static_cast<int>(foundation_offset.x + pos.x - size_.x * 0.5f);
            int y1 = static_cast<int>(foundation_offset.y + pos.y - size_.y * 0.5f);
            int x2 = static_cast<int>(foundation_offset.x + pos.x + size_.x * 0.5f);
            int y2 = static_cast<int>(foundation_offset.y + pos.y + size_.y * 0.5f);
            DrawFillBox(x1, y1, x2, y2, GetColor(0, 0, 0));
        }
        //---------------------------------------------------------------------------
        // メインのキーボード
        //---------------------------------------------------------------------------
        {
            float2 offset = float2(0.0f, 0.0f);
            //20フレームごとにオフセットを変え、押し込みを表現
            if((frame_count + frame_offset) % 40 >= 20) {
                offset = foundation_offset;
            }
            int x1 = static_cast<int>(offset.x + pos.x - size_.x * 0.5f);
            int y1 = static_cast<int>(offset.y + pos.y - size_.y * 0.5f);
            int x2 = static_cast<int>(offset.x + pos.x + size_.x * 0.5f);
            int y2 = static_cast<int>(offset.y + pos.y + size_.y * 0.5f);
            DrawFillBox(x1, y1, x2, y2, GetColor(230, 230, 230));

            //---------------------------------------------------------------------------
            // テキスト
            //---------------------------------------------------------------------------
            float  hight = static_cast<float>(GetFontSize());                                     //フォントサイズを取得(=高さ)
            float  width = static_cast<float>(GetDrawStringWidth(text_.data(), text_.size()));    // 文字列の幅を取得
            float2 text_pos;
            text_pos.x = offset.x + pos.x - (width * 0.5f);
            text_pos.y = offset.y + pos.y - (hight * 0.5f);
            DrawString(text_pos.x, text_pos.y, text_.data(), GetColor(0, 0, 0), GetColor(0, 0, 0));
        }
    };
    SetProc("UIDraw", draw_ui, ProcTiming::UI, static_cast<ProcPriority>(NORMAL));
}
//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentKeyBoardUI::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"キーボードUIコンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}
//---------------------------------------------------------------------------
//! @brief	サイズの設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentKeyBoardUI> ComponentKeyBoardUI::SetSize(const float2& size)
{
    size_ = size;    // サイズの設定
    return dynamic_pointer_cast<ComponentKeyBoardUI>(shared_from_this());
}
//---------------------------------------------------------------------------
//! @brief	テキストの設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentKeyBoardUI> ComponentKeyBoardUI::SetText(const std::string& text)
{
    text_ = text;
    return dynamic_pointer_cast<ComponentKeyBoardUI>(shared_from_this());
}
//---------------------------------------------------------------------------
//! @brief	フレームのずらしの設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentKeyBoardUI> ComponentKeyBoardUI::SetFrameOffset(int offset)
{
    frame_offset = offset;
    return dynamic_pointer_cast<ComponentKeyBoardUI>(shared_from_this());
}
