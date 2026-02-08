//---------------------------------------------------------------------------
//!	@file	HelpManager.cpp
//! @brief	ピースの攻撃機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/Manager/HelpManager.h>
#include <Game/AutoChess/UIObject/UIImage.h>
#include <Game/AutoChess/UIObject/UIButton.h>
#include <System/UIComponent/ComponentImage.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/Events/HelpClickEvent.h>
#include <Game/AutoChess/Events/HelpCloseEvent.h>
#include <Game/AutoChess/Events/FlipHelpPageEvent.h>
#include <Game/AutoChess/system/SoundManager.h>
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
    // ヘルプ画面のフィルタを作成する
    //---------------------------------------------------------------------------------
    {
        auto help_window_filter = Scene::Object::Create<UIImage>();        //フィルターの宣言
        help_window_filter->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        help_window_filter->SetScaleAxisXYZ(15.0f);                        //大きさを画面全体に設定
        help_window_filter->SetAlpha(168);                                 //透明度を設定
        float x = WINDOW_W * 0.5f;
        float y = WINDOW_H * 0.5f;
        help_window_filter->SetTranslate(float3(x, y, 0.0f));
        auto image_comp = help_window_filter->GetComponent<ComponentImage>();
        //---------------------------------------------------------------------------------
        // 描画順序を遅くする(上に描画したいから)
        //---------------------------------------------------------------------------------
        if(auto image_comp = help_window_filter->GetComponent<ComponentImage>()) {
            image_comp->SetPriority("UIDraw", ProcTiming::UI, ProcPriority::LOW);
        }
        //---------------------------------------------------------------------------------
        // ヘルプ画面を表示する処理登録
        //---------------------------------------------------------------------------------
        std::weak_ptr<UIImage> weak_help_window_filter = help_window_filter;
        auto                   help_filter_show_proc   = [weak_help_window_filter](const HelpClickEvent& e) {
            std::shared_ptr<UIImage> help_window_filter = weak_help_window_filter.lock();
            if(!help_window_filter)
                return;

            help_window_filter->SetStatus(Object::StatusBit::NoDraw, false);    //表示する
        };
        if(auto event_bus = event_bus_.lock()) {
            auto help_event_handle = event_bus->subscribe<HelpClickEvent>(help_filter_show_proc, 0);
            event_handles_.push_back(std::move(help_event_handle));
        }
        //--------------------------------------------------------------------------------
        // ヘルプを閉じた際に非表示にする処理登録
        //--------------------------------------------------------------------------------
        {
            auto help_filter_close_proc = [weak_help_window_filter](const HelpCloseEvent& e) {
                std::shared_ptr<UIImage> help_window_filter = weak_help_window_filter.lock();
                if(!help_window_filter)
                    return;

                help_window_filter->SetStatus(Object::StatusBit::NoDraw, true);    //非表示にする
            };
            if(auto event_bus = event_bus_.lock()) {
                auto help_event_handle = event_bus->subscribe<HelpCloseEvent>(help_filter_close_proc, 0);
                event_handles_.push_back(std::move(help_event_handle));
            }
        }
    }
    //---------------------------------------------------------------------------------
    // ヘルプ画面の作成
    //---------------------------------------------------------------------------------
    auto help_screen = Scene::Object::Create<UIImage>();
    help_screen->SetName("HelpScreen");
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
    std::weak_ptr<UIImage> weak_help_screen      = help_screen;
    auto                   help_screen_show_proc = [weak_help_screen, this](const HelpClickEvent& e) {
        std::shared_ptr<UIImage> help_screen = weak_help_screen.lock();
        if(!help_screen)
            return;

        help_screen->SetStatus(Object::StatusBit::NoDraw, false);       //表示する
        help_num_ = 1;                                                  //ヘルプ番号を初期化
        help_screen->SetImage(ImageBuffer::GetImageHandle("help1"));    //ヘルプ1画像を設定
    };
    if(auto event_bus = event_bus_.lock()) {
        auto help_event_handle = event_bus->subscribe<HelpClickEvent>(help_screen_show_proc, 0);
        event_handles_.push_back(std::move(help_event_handle));
    }
    //--------------------------------------------------------------------------------
    // ヘルプを閉じた際に非表示にする処理登録
    //--------------------------------------------------------------------------------
    {
        auto help_screen_close_proc = [weak_help_screen](const HelpCloseEvent& e) {
            std::shared_ptr<UIImage> help_screen = weak_help_screen.lock();
            if(!help_screen)
                return;

            help_screen->SetStatus(Object::StatusBit::NoDraw, true);    //非表示にする
        };
        if(auto event_bus = event_bus_.lock()) {
            auto help_event_handle = event_bus->subscribe<HelpCloseEvent>(help_screen_close_proc, 0);
            event_handles_.push_back(std::move(help_event_handle));
        }
    }
    //---------------------------------------------------------------------------------
    // 右ボタンの作成
    //--------------------------------------------------------------------------------
    {
        auto right_button = Scene::Object::Create<UIButton>();
        right_button->SetName("HelpRightButton");
        right_button->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        float x = WINDOW_W - 100.0f;
        float y = WINDOW_H / 2;
        right_button->SetTranslate(float3(x, y, 0.0f));                              //位置を右あたりに設定
        right_button->SetImage(ImageBuffer::GetImageHandle("help_right_button"));    //右ボタン画像を設定
        right_button->SetScaleAxisXYZ(0.3f);                                         //左右反転
        //---------------------------------------------------------------------------------
        // 描画順序を遅くする(上に描画したいから)
        //---------------------------------------------------------------------------------
        if(auto image_comp = right_button->GetComponent<ComponentImage>()) {
            image_comp->SetPriority("UIDraw", ProcTiming::UI, ProcPriority::LOW);
        }
        std::weak_ptr<UIButton> weak_right_button = right_button;    //弱参照を作成
        //--------------------------------------------------------------------------------
        // 右ボタンを左右へ動かすアニメーション登録
        //--------------------------------------------------------------------------------
        {
            auto update_proc = [weak_right_button, x]() {
                auto right_button = weak_right_button.lock();
                if(!right_button)
                    return;

                //線形補間でx座標を動かす
                static float angle  = 0.0f;
                angle              += 0.05f;
                float x_offset      = std::sin(angle) * 10.0f;    //振幅10でsin波を作成
                if(auto transform = right_button->GetComponent<ComponentTransform>()) {
                    auto pos = transform->GetTranslate();
                    pos.x    = x + x_offset;
                    transform->SetTranslate(pos);
                }
            };
            right_button->SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        }
        //--------------------------------------------------------------------------------
        // 右ボタンがクリックされた時の処理登録
        //--------------------------------------------------------------------------------
        std::weak_ptr<UIImage> weak_help_screen        = help_screen;
        auto                   right_button_click_proc = [this, weak_help_screen]() {
            auto help_screen = weak_help_screen.lock();
            if(!help_screen)
                return;

            help_num_++;
            if(help_num_ > max_help_num_) {
                help_num_ = 1;
            }
            //ヘルプ番号を進める
            help_screen->SetImage(ImageBuffer::GetImageHandle("help" + std::to_string(help_num_)));
            //--------------------------------------------------------------------------------
            // イベント発行
            //--------------------------------------------------------------------------------
            if(auto event_bus = event_bus_.lock()) {
                event_bus->publish(FlipHelpPageEvent());    //ヘルプページめくりイベントを発行
            }
        };
        right_button->SetClickFunc(right_button_click_proc);    //クリック関数を設定
        //--------------------------------------------------------------------------------
        // ヘルプがクリックされたら表示にする処理登録
        //--------------------------------------------------------------------------------
        auto help_screen_click_proc = [weak_right_button](const HelpClickEvent& e) {
            auto right_button = weak_right_button.lock();
            if(!right_button)
                return;

            right_button->SetStatus(Object::StatusBit::NoDraw, false);    //表示にする
        };
        if(auto event_bus = event_bus_.lock()) {
            auto help_event_handle = event_bus->subscribe<HelpClickEvent>(help_screen_click_proc, 0);
            event_handles_.push_back(std::move(help_event_handle));
        }
        //--------------------------------------------------------------------------------
        // ヘルプを閉じた際に非表示にする処理登録
        //--------------------------------------------------------------------------------
        {
            auto help_screen_close_proc = [weak_right_button](const HelpCloseEvent& e) {
                auto right_button = weak_right_button.lock();
                if(!right_button)
                    return;

                right_button->SetStatus(Object::StatusBit::NoDraw, true);    //非表示にする
            };
            if(auto event_bus = event_bus_.lock()) {
                auto help_event_handle = event_bus->subscribe<HelpCloseEvent>(help_screen_close_proc, 0);
                event_handles_.push_back(std::move(help_event_handle));
            }
        }
    }
    //--------------------------------------------------------------------------------
    // 左ボタンの作成
    //--------------------------------------------------------------------------------
    {
        auto left_button = Scene::Object::Create<UIButton>();
        left_button->SetName("HelpLeftButton");
        left_button->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        float x = 100.0f;
        float y = WINDOW_H / 2;
        left_button->SetTranslate(float3(x, y, 0.0f));                             //位置を左あたりに設定
        left_button->SetImage(ImageBuffer::GetImageHandle("help_left_button"));    //左ボタン画像を設定
        left_button->SetScaleAxisXYZ(0.3f);                                        //左右反転
        //---------------------------------------------------------------------------------
        // 描画順序を遅くする(上に描画したいから)
        //---------------------------------------------------------------------------------
        if(auto image_comp = left_button->GetComponent<ComponentImage>()) {
            image_comp->SetPriority("UIDraw", ProcTiming::UI, ProcPriority::LOW);
        }
        std::weak_ptr<UIButton> weak_left_button = left_button;    //弱参照を作成
        //--------------------------------------------------------------------------------
        // 左ボタンを左右へ動かすアニメーション登録
        //--------------------------------------------------------------------------------
        {
            auto update_proc = [weak_left_button, x]() {
                auto left_button = weak_left_button.lock();
                if(!left_button)
                    return;

                //線形補間でx座標を動かす
                static float angle  = 0.0f;
                angle              -= 0.05f;
                float x_offset      = std::sin(angle) * 10.0f;    //振幅10でsin波を作成
                if(auto transform = left_button->GetComponent<ComponentTransform>()) {
                    auto pos = transform->GetTranslate();
                    pos.x    = x + x_offset;
                    transform->SetTranslate(pos);
                }
            };
            left_button->SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        }
        //--------------------------------------------------------------------------------
        // 左ボタンがクリックされた時の処理登録
        //--------------------------------------------------------------------------------
        auto left_button_click_proc = [this, weak_help_screen]() {
            auto help_screen = weak_help_screen.lock();
            if(!help_screen)
                return;

            help_num_--;
            if(help_num_ < 1) {
                help_num_ = max_help_num_;
            }
            //ヘルプ番号を戻す
            help_screen->SetImage(ImageBuffer::GetImageHandle("help" + std::to_string(help_num_)));
            //--------------------------------------------------------------------------------
            // イベント発行
            //--------------------------------------------------------------------------------
            if(auto event_bus = event_bus_.lock()) {
                event_bus->publish(FlipHelpPageEvent());    //ヘルプページめくりイベントを発行
            }
        };
        left_button->SetClickFunc(left_button_click_proc);    //クリック関数を設定
        //--------------------------------------------------------------------------------
        // ヘルプがクリックされたら表示にする処理登録
        //--------------------------------------------------------------------------------
        auto help_screen_click_proc = [weak_left_button](const HelpClickEvent& e) {
            auto left_button = weak_left_button.lock();
            if(!left_button)
                return;

            left_button->SetStatus(Object::StatusBit::NoDraw, false);    //表示にする
        };
        if(auto event_bus = event_bus_.lock()) {
            auto help_event_handle = event_bus->subscribe<HelpClickEvent>(help_screen_click_proc, 0);
            event_handles_.push_back(std::move(help_event_handle));
        }
        //--------------------------------------------------------------------------------
        // ヘルプを閉じた際に非表示にする処理登録
        //--------------------------------------------------------------------------------
        {
            auto help_screen_close_proc = [weak_left_button](const HelpCloseEvent& e) {
                auto left_button = weak_left_button.lock();
                if(!left_button)
                    return;

                left_button->SetStatus(Object::StatusBit::NoDraw, true);    //非表示にする
            };
            if(auto event_bus = event_bus_.lock()) {
                auto help_event_handle = event_bus->subscribe<HelpCloseEvent>(help_screen_close_proc, 0);
                event_handles_.push_back(std::move(help_event_handle));
            }
        }
    }
    //--------------------------------------------------------------------------------
    // 閉じるボタンの作成
    //--------------------------------------------------------------------------------
    {
        auto close_button = Scene::Object::Create<UIButton>();
        close_button->SetName("HelpCloseButton");
        close_button->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        float x = WINDOW_W - 100.0f;
        float y = 100.0f;
        close_button->SetTranslate(float3(x, y, 0.0f));    //位置を右上あたりに設定
        //close_button->SetImage(ImageBuffer::GetImageHandle("help_close_button"));    //閉じるボタン画像を設定
        close_button->SetImage(ImageBuffer::GetImageHandle("help_close_button"));    //閉じるボタン画像を設定
        close_button->SetScaleAxisXYZ(0.3f);                                         //左右反転
        //---------------------------------------------------------------------------------
        // 描画順序を遅くする(上に描画したいから)
        //---------------------------------------------------------------------------------
        if(auto image_comp = close_button->GetComponent<ComponentImage>()) {
            image_comp->SetPriority("UIDraw", ProcTiming::UI, ProcPriority::LOW);
        }
        std::weak_ptr<UIButton> weak_close_button = close_button;    //弱参照を作成
        //---------------------------------------------------------------------------------
        // ヘルプが空いたら表示にする処理登録
        //---------------------------------------------------------------------------------
        {
            auto help_screen_click_proc = [weak_close_button](const HelpClickEvent& e) {
                std::shared_ptr<UIButton> close_button = weak_close_button.lock();
                if(!close_button)
                    return;

                close_button->SetStatus(Object::StatusBit::NoDraw, false);    //表示にする
            };
            if(auto event_bus = event_bus_.lock()) {
                auto help_event_handle = event_bus->subscribe<HelpClickEvent>(help_screen_click_proc, 0);
                event_handles_.push_back(std::move(help_event_handle));
            }
        }
        //--------------------------------------------------------------------------------
        // 閉じるボタンがクリックされた時の処理登録
        //--------------------------------------------------------------------------------
        auto close_button_click_proc = [this, weak_close_button](void) {
            auto close_button = weak_close_button.lock();
            if(!close_button)
                return;

            close_button->SetStatus(Object::StatusBit::NoDraw, true);    //閉じるボタンを非表示にする
            if(auto event_bus = event_bus_.lock()) {
                event_bus->publish(HelpCloseEvent());    //ヘルプ画面を閉じるイベントを発行
            }
        };
        close_button->SetClickFunc(close_button_click_proc);    //クリック関数を設定
    }
    //--------------------------------------------------------------------------------
    // 効果音の初期化処理
    //--------------------------------------------------------------------------------
    SoundInit();
}

//---------------------------------------------------------------------------
//! @brief Create内で呼ばれる初期化関数
//---------------------------------------------------------------------------
void HelpManager::Construct(ObjectPtr owner, const std::shared_ptr<TsukinoEventBus::EventBus> event_bus)
{
    __super::Construct(owner);
    event_bus_ = event_bus;
}

//---------------------------------------------------------------------------
//! @brief 効果音の初期化処理
//---------------------------------------------------------------------------
void HelpManager::SoundInit()
{
    //---------------------------------------------------------------------------
    // ヘルプを開いた時の効果音を登録
    //---------------------------------------------------------------------------
    {
        auto help_open_sound_proc = [](const HelpClickEvent& e) { SoundManager::instance()->PlaySE("paper"); };
        if(auto event_bus = event_bus_.lock()) {
            auto help_event_handle = event_bus->subscribe<HelpClickEvent>(help_open_sound_proc, 0);
            event_handles_.push_back(std::move(help_event_handle));
        }
    }
    //---------------------------------------------------------------------------
    // ヘルプを閉じた時の効果音を登録
    //---------------------------------------------------------------------------
    {
        auto help_close_sound_proc = [](const HelpCloseEvent& e) { SoundManager::instance()->PlaySE("push_button"); };
        if(auto event_bus = event_bus_.lock()) {
            auto help_event_handle = event_bus->subscribe<HelpCloseEvent>(help_close_sound_proc, 0);
            event_handles_.push_back(std::move(help_event_handle));
        }
    }
    //---------------------------------------------------------------------------
    // ヘルプをめくった時の効果音を登録
    //---------------------------------------------------------------------------
    {
        auto help_flip_sound_proc = [](const FlipHelpPageEvent& e) { SoundManager::instance()->PlaySE("paper"); };
        if(auto event_bus = event_bus_.lock()) {
            auto help_event_handle = event_bus->subscribe<FlipHelpPageEvent>(help_flip_sound_proc, 0);
            event_handles_.push_back(std::move(help_event_handle));
        }
    }
}
