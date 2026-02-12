//---------------------------------------------------------------------------
//!	@file	ComponentAnimUI.cpp
//! @brief	アニメーションUIコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentAnimUI.h"
#include <System/UIComponent/ComponentTransformUI.h>

//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentAnimUI::Init()
{    // 初期化処理
    __super::Init();
    //---------------------------------------------------------------------------
    //  更新処理を登録
    //---------------------------------------------------------------------------
    auto update = [this]() {
        if(update_frame_ <= 0)
            return;                 //0以下が指定されていたら、アニメーションを行わない
        ++frame_;                   //フレームをカウント
        frame_ %= update_frame_;    //もしフレームカウントがupdate_frameと同じなら0になる。
        if(frame_ != 0)
            return;                    //0でない時ならここから先の更新は行わない
        src_x_ += (int)div_size_.x;    //spriteを一枚分ずらす
        if(src_x_ >= size_.x) {
            Reset();    //もしずらした先が画像サイズをはみ出ていたら1毎目に戻す
        }
    };
    SetProc("Update", update, ProcTiming::UI, static_cast<ProcPriority>(NORMAL));
    //---------------------------------------------------------------------------
    //  UI描画を登録
    //---------------------------------------------------------------------------
    auto draw_ui = [this]() {
        auto   owner     = GetOwner();
        float3 translate = owner->GetTranslate();
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);    // 透明度設定
        DrawRectRotaGraphF(translate.x,
                           translate.y,    // 描画位置
                           src_x_,
                           0,    // 画像切り抜き位置
                           (int)div_size_.x,
                           (int)div_size_.y,    // 画像ひとつのサイズ
                           ex_rate_,
                           0,
                           img_,
                           TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);    // ブレンドモードを元に戻す
    };
    SetProc("UIDraw", draw_ui, ProcTiming::UI, static_cast<ProcPriority>(NORMAL));
}
//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentAnimUI::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"アニメーションUIコンポーネント")) {
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}
//---------------------------------------------------------------------------
//! @brief	アニメーションステータスの設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentAnimUI> ComponentAnimUI::SetAnimStatus(int gh, int div_num, float ex_rate, int update_frame)
{
    img_          = gh;
    ex_rate_      = ex_rate;
    update_frame_ = update_frame;
    //---------------------------------------------------------------------------
    // サイズの取得
    //---------------------------------------------------------------------------
    float size_x = 0.0f;
    float size_y = 0.0f;
    GetGraphSizeF(img_, &size_x, &size_y);
    size_.x     = size_x;
    size_.y     = size_y;
    div_size_.x = size_.x / div_num;
    div_size_.y = size_.y;
    return dynamic_pointer_cast<ComponentAnimUI>(shared_from_this());
}

//--------------------------------------------------------------------
//! @brief 透明度の設定
//--------------------------------------------------------------------
std::shared_ptr<ComponentAnimUI> ComponentAnimUI::SetAlpha(int alpha)
{
    alpha_ = alpha;
    return dynamic_pointer_cast<ComponentAnimUI>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	フレームとsrc_xのリセット、実質のアニメーションリセット
//---------------------------------------------------------------------------
void ComponentAnimUI::Reset()
{
    frame_ = 0;
    src_x_ = 0;
}
