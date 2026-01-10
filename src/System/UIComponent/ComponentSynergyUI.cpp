//---------------------------------------------------------------------------
//!	@file	ComponentSynergyUI.cpp
//! @brief	シナジーのUIコンポーネント
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ComponentSynergyUI.h"
#include <System/UIComponent/ComponentTransformUI.h>
#include <Game/AutoChess/system/FontBuffer.h>
#include <Game/AutoChess/system/HlslppUseful.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/UIObject/UIAnimation.h>
#include <Game/AutoChess/Synergy/SynergyData.h>
//---------------------------------------------------------------------------
//! @brief	初期化関数
//---------------------------------------------------------------------------
void ComponentSynergyUI::Init()
{    // 初期化処理
    __super::Init();
    //---------------------------------------------------------------------------
    // 更新処理を登録
    //---------------------------------------------------------------------------
    auto update_proc = [this]() {
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
    SetProc("UpdateSynergyUI", update_proc, ProcTiming::Update, ProcPriority::NORMAL);

    //---------------------------------------------------------------------------------
    //クリック時の処理
    //---------------------------------------------------------------------------------
    auto click_proc = [this]() {
        if(IsClick()) {
            click_func_();    //クリック時の関数を実行
        }
    };
    SetProc("ClickProc", click_proc, ProcTiming::Update, static_cast<ProcPriority>(NONE));

    //---------------------------------------------------------------------------
    // 描画処理の追加
    //---------------------------------------------------------------------------
    auto draw_proc = [this]() {
        //オーナーとなるオブジェクトを取得
        auto owner = GetOwner();
        //---------------------------------------------------------------------------
        // 描画位置の計算
        //---------------------------------------------------------------------------
        float3 adjustment = GetAdjustment();    // 調整値
        float3 pos        = float3(0.0f, 0.0f, 0.0f);
        pos               = owner->GetTranslate() + adjustment;
        float3 scale      = owner->GetScaleAxisXYZ();
        //サイズは、Transformの平均
        float size = (scale.x + scale.y + scale.z) / 3.0f;    // 平均値をとる
        //角度はx軸の角度から取る
        float angle = owner->GetRotationAxisXYZ().x;    // X軸の角度を取得0
        //---------------------------------------------------------------------------
        // シナジー画像が有効な場合、描画する
        //---------------------------------------------------------------------------
        if(synergy_image_ != -1) {
            //少しサイズを小さくする
            DrawRotaGraphF(pos.x, pos.y, static_cast<double>(size) * 0.5f, static_cast<double>(angle), synergy_image_, TRUE);
        }
        //---------------------------------------------------------------------------
        // 下に現在の数と次のレベルまでの必要数を表示する
        //---------------------------------------------------------------------------
        int font_handle = FontBuffer::GetFontHandle(font_name_, font_size_, 1, DX_FONTTYPE_ANTIALIASING_EDGE, edge_size_);    //フォントのハンドルを取得
        std::string synergy_text = std::to_string(synergy_count_) + " / " + std::to_string(next_count_);
        //フォントが存在しているかで分岐
        if(font_handle != -1) {
            DrawStringToHandle(pos.x, pos.y, synergy_text.data(), text_color_, font_handle, edge_color_);
        }
        else {
            //存在しない
            DrawString(pos.x, pos.y, synergy_text.data(), text_color_, edge_color_);
        }
    };
    SetProc("DrawSynergyUI", draw_proc, ProcTiming::UI, ProcPriority::NORMAL);
}

//---------------------------------------------------------------------------
//! @brief	クリックされているかを返す関数
//---------------------------------------------------------------------------
bool ComponentSynergyUI::IsClick()
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
bool ComponentSynergyUI::IsMouseOver()
{
    // とりあえずオーナーを取得
    auto owner = GetOwner();
    // 描画しない設定なら触れていない
    if(GetStatus(Component::StatusBit::NoDraw) || owner->GetStatus(Object::StatusBit::NoDraw)) {
        return false;
    }
    //マウス座標を取得
    float2 mouse_pos = GetMouseFloat2();
    //オーナー(UI)座標を取得
    float3 translate = owner->GetTranslate() + GetAdjustment();
    float2 ui_pos    = float2(translate.x, translate.y);
    //UIサイズを取得
    float2 ui_size = float2(0.0f, 0.0f);      //とりあえず宣言
    ui_size        = GetScreenImageSize();    //サイズ取得
    if(CheckBoxPointHit(ui_pos, ui_size, mouse_pos)) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//! @brief	画面にうつる画像のサイズを取得する関数
//---------------------------------------------------------------------------
float2 ComponentSynergyUI::GetScreenImageSize()
{
    float width = 0;                                  // 幅
    float hight = 0;                                  //高さ
    GetGraphSizeF(synergy_image_, &width, &hight);    // 画像のサイズを取得
    auto   owner = GetOwner();                        //オーナーを取得
    float3 scale = owner->GetScaleAxisXYZ();
    //サイズは、Transformの平均
    float size  = (scale.x + scale.y + scale.z) / 3.0f;    // 平均値をとる
    width      *= size;                                    // 幅にサイズをかける
    hight      *= size;                                    // 高さにサイズをかける
    return float2(width, hight);                           // 画像のサイズをfloat2で返す
}

//---------------------------------------------------------------------------
//!  マウスをクリックした時に行う処理の設定
//---------------------------------------------------------------------------
std::shared_ptr<ComponentSynergyUI> ComponentSynergyUI::SetClickFunc(const std::function<void()>& click_func)
{
    click_func_ = click_func;
    return dynamic_pointer_cast<ComponentSynergyUI>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	ImGui
//---------------------------------------------------------------------------
void ComponentSynergyUI::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"シナジーUIコンポーネント")) {
            // GUI上でオーナーから自分(SampleObjectController)を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//--------------------------------------------------------------------
//! @brief シナジー画像の設定関数
//--------------------------------------------------------------------
std::shared_ptr<ComponentSynergyUI> ComponentSynergyUI::SetSynergyImage(int image_handle)
{
    synergy_image_ = image_handle;
    return dynamic_pointer_cast<ComponentSynergyUI>(shared_from_this());
}

//--------------------------------------------------------------------
//! @brief 次のレベルまでの必要数を設定する関数
//--------------------------------------------------------------------
std::shared_ptr<ComponentSynergyUI> ComponentSynergyUI::SetNextCount(int next_count)
{
    next_count_ = next_count;
    return dynamic_pointer_cast<ComponentSynergyUI>(shared_from_this());
}

//--------------------------------------------------------------------
//! @brief シナジーの数を設定する関数
//--------------------------------------------------------------------
std::shared_ptr<ComponentSynergyUI> ComponentSynergyUI::SetSynergyCount(int synergy_count)
{
    synergy_count_ = synergy_count;
    return dynamic_pointer_cast<ComponentSynergyUI>(shared_from_this());
}

//--------------------------------------------------------------------
//! @brief シナジーデータへのポインタを設定する関数
//--------------------------------------------------------------------
void ComponentSynergyUI::SetSynergyData(const SynergyData* synergy_data)
{
    synergy_data_ = synergy_data;
}

//--------------------------------------------------------------------
//! @brief  画像座標の補正値を取得する関数
//--------------------------------------------------------------------
float3 ComponentSynergyUI::GetAdjustment() const
{
    auto   owner      = GetOwner();                  //オーナーを取得
    float3 adjustment = float3(0.0f, 0.0f, 0.0f);    // 調整値(Alignmentに合わせて)
    if(auto comp_transform = owner->GetComponent<ComponentTransformUI>()) {
        ComponentTransformUI::Alignment alignment = comp_transform->GetAlignment();
        float                           width     = 0;    // 幅
        float                           hight     = 0;    //高さ
        GetGraphSizeF(synergy_image_, &width, &hight);
        //配置位置(縦)
        switch(static_cast<int>(alignment) / 3) {
        case 0:
            adjustment.y = (-hight * 0.5f);
            break;    // 上寄せ
        case 1:
            adjustment.y = 0.0f;
            break;    // 中央寄せ
        case 2:
            adjustment.y = (hight * 0.5f);
            break;    // 下寄せ
        }
        //配置位置(横)
        switch(static_cast<int>(alignment) % 3) {
        case 0:
            adjustment.x = (-width * 0.5f);
            break;    // 左寄せ
        case 1:
            adjustment.x = 0.0f;
            break;    // 中央寄せ
        case 2:
            adjustment.x = (width * 0.5f);
            break;    // 右寄せ
        }
    }
    return adjustment;
}

CEREAL_REGISTER_TYPE(ComponentSynergyUI)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentSynergyUI)
