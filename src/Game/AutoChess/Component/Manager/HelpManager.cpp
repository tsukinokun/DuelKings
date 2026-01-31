//---------------------------------------------------------------------------
//!	@file	HelpManager.cpp
//! @brief	ピースの攻撃機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/Manager/HelpManager.h>
#include <Game/AutoChess/UIObject/UIImage.h>
#include <System/UIComponent/ComponentImage.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/Events/HelpClickEvent.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void HelpManager::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    // イベントとUIを作成していく
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    // ヘルプ画面の作成
    //---------------------------------------------------------------------------------
    {
        auto help_screen = Scene::Object::Create<UIImage>();
        help_screen->SetStatus(Object::StatusBit::NoDraw, true);                //初期状態では非表示にしておく
        help_screen->SetIsFilter(true);                                         //フィルターに設定
        help_screen->SetTranslate(float3(WINDOW_W / 2, WINDOW_H / 2, 0.0f));    //位置を中央あたりに設定
        //---------------------------------------------------------------------------------
        // 描画順序を遅くする(上に描画したいから)
        //---------------------------------------------------------------------------------
        if(auto image_comp = help_screen->GetComponent<ComponentImage>()) {
            image_comp->SetPriority("UIDraw", ProcTiming::UI, ProcPriority::LOW);
        }

        //---------------------------------------------------------------------------------
        // ヘルプ画面を表示する処理登録
        //---------------------------------------------------------------------------------
        auto help_screen_show_proc = [help_screen](const HelpClickEvent& e) {
            help_screen->SetStatus(Object::StatusBit::NoDraw, false);       //表示する
            help_screen->SetImage(ImageBuffer::GetImageHandle("help1"));    //ヘルプ1画像を設定
        };
        if(auto event_bus = event_bus_.lock()) {
            auto help_event_handle = event_bus->subscribe<HelpClickEvent>(help_screen_show_proc, 0);
            event_handles_.push_back(std::move(help_event_handle));
        }
    }
    //---------------------------------------------------------------------------------
    // 右ボタンの作成
    //---------------------------------------------------------------------------------
}

//---------------------------------------------------------------------------
//! @brief Create内で呼ばれる初期化関数
//---------------------------------------------------------------------------
void HelpManager::Construct(ObjectPtr owner, const std::shared_ptr<TsukinoEventBus::EventBus> event_bus)
{
    __super::Construct(owner);
    event_bus_ = event_bus;
}
