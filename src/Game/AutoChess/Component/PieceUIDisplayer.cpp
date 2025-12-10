//---------------------------------------------------------------------------
//!	@file	PieceUIDisplayer.cpp
//! @brief	ピースのHP表示機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceUIDisplayer.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/UIObject/UIGauge.h>
#include <Game/AutoChess/system/HlslppUseful.h>
//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void PieceUIDisplayer::Init()
{
    __super::Init();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------------------------------
    // ピースにHPゲージの描画を追加
    //---------------------------------------------------------------------------------
    auto hp_proc = [owner, this]() {
        std::string hp_ui_name = std::string(owner->GetName()) + "hp_ui_name";
        //最大HP未満であれば描画をする。
        if(owner->GetHP() < (owner->GetMaxHP())) {
            //HP表示
            if(auto hp_ui = Scene::Object::Get<UIGauge>(hp_ui_name)) {
                //ピースのスクリーン座標を取得
                float2 screen_pos = WorldPositionToScreenPosition(owner->GetTranslate());
                //少し上にずらす
                float2 offset  = float2(0.0f, -30.0f);
                screen_pos    += offset;
                hp_ui->SetTranslate(float3(screen_pos.x, screen_pos.y, 0.0f));                              //位置設定
                float rate = static_cast<float>(owner->GetHP()) / static_cast<float>(owner->GetMaxHP());    //割合を計算
                hp_ui->SetGaugeRate(rate);
            }
            else {
                //ゲージの表示
                hp_ui = Scene::Object::Create<UIGauge>();    //HPゲージオブジェクトを生成
                hp_ui->SetGaugeColor(hp_bar_color);
                hp_ui->SetGaugeSize(int2(50, 10));
                std::weak_ptr<Piece> owner_wp = owner;           //オーナーの弱参照を取得
                hp_ui->SetAutoReleaseTarget(owner_wp.lock());    //オーナーが消えたら自分も消えるように設定
                //ピースのスクリーン座標を取得
                float2 screen_pos = WorldPositionToScreenPosition(owner->GetTranslate());
                //少し上にずらす
                float2 offset  = float2(0.0f, -30.0f);
                screen_pos    += offset;
                hp_ui->SetTranslate(float3(screen_pos.x, screen_pos.y, 0.0f));    //位置を設定
                hp_ui->SetName(hp_ui_name);                                       //名前を設定
            }
        }
        else {
            if(auto button_ui = Scene::Object::Get<UIGauge>(hp_ui_name)) {
                //非表示
                Scene::Object::Release(button_ui);
            }
        }
    };
    SetProc("hp_proc", hp_proc, ProcTiming::Update, ProcPriority::NONE);
}

//---------------------------------------------------------
//! @brief HPバーの色を設定する関数
//---------------------------------------------------------
std::shared_ptr<PieceUIDisplayer> PieceUIDisplayer::SetHPBarColor(int color)
{
    hp_bar_color = color;
    return std::dynamic_pointer_cast<PieceUIDisplayer>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief スクリーン座標を取得し、指定オフセットを加算する
//---------------------------------------------------------------------------
float2 PieceUIDisplayer::GetScreenPositionWithOffset(const Piece* owner, const float2& offset)
{
    float2 screen_pos = WorldPositionToScreenPosition(owner->GetTranslate());
    return screen_pos + offset;
}

//---------------------------------------------------------------------------
//! @brief ゲージオブジェクトを生成する
//---------------------------------------------------------------------------
std::shared_ptr<UIGauge> PieceUIDisplayer::CreateGauge(const std::string& name, const std::shared_ptr<Piece>& owner, int color, const int2& size)
{
    auto gauge = Scene::Object::Create<UIGauge>();
    gauge->SetGaugeColor(color);
    gauge->SetGaugeSize(size);
    gauge->SetName(name);

    std::weak_ptr<Piece> owner_wp = owner;
    gauge->SetAutoReleaseTarget(owner_wp.lock());

    return gauge;
}

//---------------------------------------------------------------------------
//! @brief 任意のゲージ（HP/MP/スタミナなど）を更新する
//---------------------------------------------------------------------------
void PieceUIDisplayer::UpdateGauge(const std::shared_ptr<Piece>& owner,
                                   const std::string&            gauge_name_suffix,
                                   int                           current_value,
                                   int                           max_value,
                                   int                           gauge_color,
                                   const int2&                   gauge_size,
                                   const float2&                 offset)
{
    std::string ui_name = std::string(owner->GetName()) + gauge_name_suffix;

    if(current_value < max_value) {
        auto ui_gauge = Scene::Object::Get<UIGauge>(ui_name);
        if(!ui_gauge) {
            ui_gauge = CreateGauge(ui_name, owner, gauge_color, gauge_size);
        }

        float2 screen_pos = GetScreenPositionWithOffset(owner.get(), offset);
        ui_gauge->SetTranslate(float3(screen_pos.x, screen_pos.y, 0.0f));

        float rate = static_cast<float>(current_value) / static_cast<float>(max_value);
        ui_gauge->SetGaugeRate(rate);
    }
    else {
        if(auto ui_gauge = Scene::Object::Get<UIGauge>(ui_name)) {
            Scene::Object::Release(ui_gauge);
        }
    }
}
