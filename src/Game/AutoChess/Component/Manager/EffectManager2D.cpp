//---------------------------------------------------------------------------
//!	@file	EffectManager2D.cpp
//! @brief	ショップエフェクト管理クラスの定義の実装
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/Manager/EffectManager2D.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/UIObject/UIAnimation.h>
#include <Game/AutoChess/Events/LoseEvent.h>
#include <Game/AutoChess/Events/DrawEvent.h>
#include <Game/AutoChess/Events/WinEvent.h>
//---------------------------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------------------------
void EffectManager2D::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------
    // ゴールド取得時に行う関数
    //---------------------------------------------------------------------------
    auto gold_gain_func = [this](const TsukinoEventBus::BaseEvent& e) {
        //---------------------------------------------------------------------------
        // 2Dのアニメーションオブジェクトを生成
        //---------------------------------------------------------------------------
        auto  ui_animation = Scene::Object::Create<UIAnimation>();
        int   div_num      = 15;                                                                              // スプライトの分割数
        float ex_rate      = 0.5f;                                                                            // 拡大率
        int   speed        = 3;                                                                               // アニメーションの更新速度
        ui_animation->SetAnimStatus(ImageBuffer::GetImageHandle("sparkle_anim"), div_num, ex_rate, speed);    // スパークルエフェクト
        ui_animation->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        //透明度は半透明くらい
        int alpha = 128;
        ui_animation->SetAlpha(alpha);
        //---------------------------------------------------------------------------
        // アニメーションの位置設定処理を登録
        //---------------------------------------------------------------------------
        float3 start_pos = float3(WINDOW_W / 2, 435.0f, 0.0f);    //こちらも制約でUIの位置を取得できないため、固定値で指定、悔しい
        ui_animation->SetTranslate(start_pos);
        float3 end_pos = float3(1100.0f, 180.0f, 0.0f);    // ゴールドUIの位置、エンジン側の制約でUIの位置を取得できないため、固定値で指定、悔しい
        std::weak_ptr<UIAnimation> weak_ui_animation = ui_animation;
        auto                       set_position_proc = [end_pos, weak_ui_animation]() {
            //UIアニメーションオブジェクトが存在しているか確認
            std::shared_ptr<UIAnimation> ui_animation = weak_ui_animation.lock();
            if(!ui_animation)
                return;

            //現在の位置を取得
            float3 current_pos = ui_animation->GetTranslate();
            //徐々にゴールドUIの位置に移動させる
            float3 next_pos = hlslpp::lerp(current_pos, end_pos, 0.05f);
            //位置を更新
            ui_animation->SetTranslate(next_pos);
            //---------------------------------------------------------------------------
            //UIアニメーションオブジェクトがゴールドUIの位置に近づいたら、アニメーションを終了させる
            //---------------------------------------------------------------------------
            //終了位置と更新した位置との距離を計算
            float3 diff     = end_pos - next_pos;
            float  distance = hlslpp::length(diff);
            //距離が一定以下になったらアニメーションを終了させる
            float threshold = 1.0f;    // 距離の閾値
            if(distance < threshold) {
                Scene::Object::Release(ui_animation);
            }
        };
        ui_animation->SetProc("set_position_proc", set_position_proc, ProcTiming::Update, ProcPriority::NORMAL);
    };
    //---------------------------------------------------------------------------
    // ゴールド取得時イベント登録
    //---------------------------------------------------------------------------
    if(auto event_bus = event_bus_.lock()) {
        event_handles_.emplace_back(event_bus->subscribe<WinEvent>(gold_gain_func, 0));
        event_handles_.emplace_back(event_bus->subscribe<LoseEvent>(gold_gain_func, 0));
        event_handles_.emplace_back(event_bus->subscribe<DrawEvent>(gold_gain_func, 0));
    }
}

//---------------------------------------------------------------------------
//! @brief Create内で呼ばれる初期化関数
//---------------------------------------------------------------------------
void EffectManager2D::Construct(ObjectPtr owner, const std::shared_ptr<TsukinoEventBus::EventBus> event_bus)
{
    __super::Construct(owner);
    event_bus_ = event_bus;
}
