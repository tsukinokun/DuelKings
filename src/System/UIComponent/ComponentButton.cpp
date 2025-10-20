//---------------------------------------------------------------------------
//!	@file	ComponentButton.cpp
//! @brief	ボタンコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentButton.h"
#include "ComponentTransformUI.h"
#include "ComponentImage.h"
#include <Game/AutoChess/system/HlslppUseful.h>
#include <Game/AutoChess/UIObject/UIAnimation.h>
#include <Game/AutoChess/system/ImageBuffer.h>
//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentButton::Init()
{    // 初期化処理
    __super::Init();
    //---------------------------------------------------------------------------------
    //  更新処理を登録
    //--------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------
    //マウスが重なっている際の処理
    //---------------------------------------------------------------------------------
    SetProc("Update", mouse_over_func_, ProcTiming::Update, static_cast<ProcPriority>(NONE));
    //---------------------------------------------------------------------------------
    //クリック時の処理
    //---------------------------------------------------------------------------------
    auto click_proc = [this]() {
        if(IsClick()) {
            click_func_();    //クリック時の関数を実行
        }
    };
    SetProc("ClickProc", click_proc, ProcTiming::Update, static_cast<ProcPriority>(NONE));
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
//---------------------------------------------------------------------------
//  マウスがボタンに触れている時に表示させる情報の設定
//! @return 自身のポインタ
//---------------------------------------------------------------------------
std::shared_ptr<ComponentButton> ComponentButton::SetOverInformation(OverInformation info)
{
    switch(info) {
    case OverInformation::NONE:
        //空の関数
        mouse_over_func_ = []() {};
        break;
    case OverInformation::LEFT_CLICK:
        //左クリックを促す
        mouse_over_func_ = [this]() {
            auto        owner         = GetOwner();
            std::string click_ui_name = std::string(owner->GetName()) + "left_click_anim";
            //マウスが触れていたら
            if(IsMouseOver()) {
                //左マウスクリックを促す
                if(auto click_ui = Scene::Object::Get<UIAnimation>(click_ui_name)) {
                    //マウス座標を取得
                    float2 mouse_pos = GetMouseFloat2();
                    click_ui->SetTranslate(float3(mouse_pos.x, mouse_pos.y, 0.0f));    //位置をマウス座標に設定
                }
                else {
                    click_ui = Scene::Object::Create<UIAnimation>();    //左クリックアニメーションオブジェクトを生成
                    //マウス座標を取得
                    float2 mouse_pos = GetMouseFloat2();
                    click_ui->SetTranslate(float3(mouse_pos.x, mouse_pos.y, 0.0f));                          //位置をマウス座標に設定
                    click_ui->SetAnimStatus(ImageBuffer::GetImageHandle("left_click_anim"), 2, 0.2f, 30);    //アニメーションステータス設定
                    click_ui->SetName(click_ui_name);                                                        //名前設定
                }
            }
            else {
                //非表示
                if(auto click_ui = Scene::Object::Get<UIAnimation>(click_ui_name)) {
                    Scene::Object::Release(click_ui);
                }
            }
        };
        break;
    }
    return dynamic_pointer_cast<ComponentButton>(shared_from_this());
}

//---------------------------------------------------------------------------
//!  マウスをクリックした時に行う処理の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentButton> ComponentButton::SetClickFunc(const std::function<void()>& click_func)
{
    click_func_ = click_func;
    return dynamic_pointer_cast<ComponentButton>(shared_from_this());
}
CEREAL_REGISTER_TYPE(ComponentButton)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentButton)
