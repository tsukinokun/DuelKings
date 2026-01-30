//---------------------------------------------------------------------------
//!	@file	InGameScene.cpp
//! @brief	オートチェスのインゲームシーン
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "InGameScene.h"
#include "Camera.h"
#include "ChessBoard.h"
#include "PieceStand.h"
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Agent.h>
#include <Game/AutoChess/Player.h>
#include <Game/AutoChess/Npc.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/system/MouseRay.h>
#include <Game/AutoChess/ShopStand.h>
#include <Game/AutoChess/UIObject/PiecePurchaseOpenButton.h>
#include <Game/AutoChess/UIObject/UIText.h>
#include <System/UIComponent/ComponentTransformUI.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/PiecePool.h>
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/PieceFactory.h>
#include <Game/AutoChess/Square.h>
#include <Game/AutoChess/Component/PieceMover.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/Component/PieceAttacker.h>
#include <Game/AutoChess/Component/PieceUIDisplayer.h>
#include <Game/AutoChess/Component/PieceSkillUser.h>
#include <Game/AutoChess/UIObject/UIGauge.h>
#include <Game/AutoChess/UIObject/UIImage.h>
#include <Game/AutoChess/UIObject/UISynergy.h>
#include <Game/AutoChess/Piece/PieceData/PieceData.h>
#include <Game/AutoChess/Piece/PieceData/LevelData.h>
#include <Game/AutoChess/system/Logic.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>
#include <Game/AutoChess/Funiture/Glass.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Events/SkillClickEvent.h>
#include <Game/AutoChess/Events/SynergyClickEvent.h>
#include <Game/AutoChess/Events/PiecePurchaseOpenClickEvent.h>
#include <Game/AutoChess/system/UIHitManager.h>
#include <Game/AutoChess/Info/SynergyModifierData.h>
#include <Game/AutoChess/Component/StatusEffect/ModifierStatus.h>
#include <System/UIComponent/ComponentImage.h>
#include <Game/AutoChess/Events/GoldClickEvent.h>
#include <Game/AutoChess/Events/LoseEvent.h>
#include <Game/AutoChess/Events/WinEvent.h>
#include <System/Component/ComponentFilterFade.h>
#include <Game/AutoChess/system/SoundManager.h>
#include <Game/AutoChess/ResultScene.h>
#include <Game/AutoChess/system/GameRepository.h>
#include <Game/AutoChess/Component/MoveStrategy/IMoveStrategy.h>
#include <Game/AutoChess/Component/MoveStrategy/MoveWinStrategy.h>
#include <Game/AutoChess/system/DXLibUtils.h>
#include <Game/AutoChess/UIObject/UIObject.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool InGameScene::Init()
{
    __super::Init();
    //コンテナにイベントバスを登録(シングルトン)
    di_container_.registerType<TsukinoEventBus::EventBus, TsukinoEventBus::EventBus>(TsukinoDIContainer::Lifecycle::Singleton);
    // リポジトリを読み込み、初期化する
    game_context_.LoadRepositories("data/AutoChess/MasterData/PieceDatas.json",
                                   "data/AutoChess/MasterData/SynergyDatas.json",
                                   "data/AutoChess/MasterData/SkillDatas.json");    //マスターデータの読み込み
    PieceFactory::SetPieceRepository(&game_context_.GetPieceRepository());
    PieceFactory::SetSkillRepository(&game_context_.GetSkillRepository());
    PiecePool::Init(game_context_.GetPieceRepository());    //駒プールの初期化
    Scene::Object::Create<Camera>();                        //カメラ
    auto event_bus = di_container_.resolve<TsukinoEventBus::EventBus>();
    auto player    = Scene::Object::Create<Player>();    //プレイヤー
    player->SetSynergySystemRepository(&game_context_.GetSynergyRepository(), &game_context_.GetPieceRepository());
    player->SetEventBus(event_bus.get());                 //イベントバスのポインタを設定
    player->SetIsPurchaseOpenFlag(&is_purchase_open_);    //ピース購入画面が開いているかのフラグを設定
    //---------------------------------------------------------------------------------
    //  ピーススタンドの生成
    //---------------------------------------------------------------------------------
    {
        auto piece_stand = Scene::Object::Create<PieceStand>();
        piece_stand->SetOwner(player);
    }
    //---------------------------------------------------------------------------------
    //  ショップスタンドの生成
    //---------------------------------------------------------------------------------
    {
        auto shop = Scene::Object::Create<ShopStand>();
        shop->SetOwner(player);
    }
    //---------------------------------------------------------------------------------
    //  ボードの生成
    //---------------------------------------------------------------------------------
    {
        auto board = Scene::Object::Create<ChessBoard>();
    }
    //---------------------------------------------------------------------------------
    // 机の生成
    //---------------------------------------------------------------------------------
    {
        auto table = Scene::Object::Create<Glass>();
    }
    //---------------------------------------------------------------------------------
    // 部屋の生成
    //---------------------------------------------------------------------------------
    {
        auto room  = Scene::Object::Create<Object>();
        auto model = room->AddComponent<ComponentModel>("data/AutoChess/Model/Stage/Room.mv1");
        room->SetScaleAxisXYZ(2.0f);
        room->SetTranslate(float3(-0.5f, -20.0f, 0.0f));
    }
    //---------------------------------------------------------------------------------
    //  NPCの生成
    //---------------------------------------------------------------------------------
    for(int i = 0; i < AGENT_NUM - 1; i++) {
        auto npc = Scene::Object::Create<Npc>();
        npc->SetPieceRepository(&game_context_.GetPieceRepository());    //ピースリポジトリを設定
        npc->OnTurnStart();                                              //ターン開始時処理を実行
    }
    Scene::Object::Create<MouseRay>();    //マウス光線
    //---------------------------------------------------------------------------------
    //  経験値ボタン
    //---------------------------------------------------------------------------------
    {
        auto exp_button = Scene::Object::Create<UIButton>();    //経験値ボタン
        //exp_button->SetStatus(Object::StatusBit::NoDraw, true);
        exp_button->SetImage(ImageBuffer::GetImageHandle("exp_button"));
        exp_button->SetScaleAxisXYZ(0.6f);                         //大きさを少し小さく設定
        exp_button->SetTranslate(float3(150.0f, 600.0f, 0.0f));    //位置を画面左下あたりに設定
        //左クリックを促す
        exp_button->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
        //クリック時の処理
        auto click_func = []() {
            if(auto player = Scene::Object::Get<Player>()) {
                //4ゴールド以上所持しているなら経験値を増やす
                if(player->GetGold() >= 4) {
                    player->AddExp(4);    //経験値を4増やす
                    player->SpendGold(4);
                }
            }
        };
        exp_button->SetClickFunc(click_func);
        //---------------------------------------------------------------------------------
        //  次までに必要な経験値UIを重ねる
        //---------------------------------------------------------------------------------
        {
            //現在の経験値
            auto curr_exp_ui = Scene::Object::Create<UIText>();
            //curr_exp_ui->SetStatus(Object::StatusBit::NoDraw, true);
            curr_exp_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleRight);    //左寄せに設定
            curr_exp_ui->SetTranslate(float3(100.0f, 500.0f, 0.0f));                    //位置を設定
            curr_exp_ui->SetFontSize(30);                                               //フォントサイズ設定
            curr_exp_ui->SetColor(GetColor(128, 128, 128), GetColor(255, 255, 255));    //文字色設定
            //更新処理
            auto set_text_proc = [curr_exp_ui]() {
                auto player      = Scene::Object::Get<Player>();
                int  current_exp = player->GetCurrentExp();    //現在の経験値を取得
                //必要な経験値を表示
                curr_exp_ui->SetText(std::to_string(current_exp));
            };
            curr_exp_ui->SetProc("set_text", set_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
            //割線
            auto line_ui = Scene::Object::Create<UIText>();
            curr_exp_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            //line_ui->SetStatus(Object::StatusBit::NoDraw, true);
            line_ui->SetTranslate(float3(150.0f, 500.0f, 0.0f));                    //位置を設定
            line_ui->SetFontSize(30);                                               //フォントサイズ設定
            line_ui->SetColor(GetColor(128, 128, 128), GetColor(255, 255, 255));    //文字色設定
            line_ui->SetText("/");                                                  //割線を表示
            //次のレベルまでに必要な経験値
            auto next_exp_ui = Scene::Object::Create<UIText>();
            //next_exp_ui->SetStatus(Object::StatusBit::NoDraw, true);
            next_exp_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleLeft);     //左寄せに設定
            next_exp_ui->SetTranslate(float3(170.0f, 500.0f, 0.0f));                    //位置を設定
            next_exp_ui->SetFontSize(30);                                               //フォントサイズ設定
            next_exp_ui->SetColor(GetColor(128, 128, 128), GetColor(255, 255, 255));    //文字色設定
            //更新処理
            auto set_next_text_proc = [next_exp_ui]() {
                auto player         = Scene::Object::Get<Player>();
                int  next_level_exp = player->GetNextLevelExp();    //次のレベルまでに必要な経験値を取得
                //必要な経験値を表示
                next_exp_ui->SetText(std::to_string(next_level_exp));
            };
            next_exp_ui->SetProc("set_text", set_next_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
            //---------------------------------------------------------------------------------
            // ピース購入画面を開くボタンを押したときに、経験値UIを非表示にする処理を登録
            //---------------------------------------------------------------------------------
            auto exp_button_hide_proc = [exp_button, curr_exp_ui, line_ui, next_exp_ui](const PiecePurchaseOpenClickEvent& e) {
                if(e.is_open_) {
                    exp_button->SetStatus(Object::StatusBit::NoDraw, true);     //ピース購入画面が開いているなら非表示にする
                    curr_exp_ui->SetStatus(Object::StatusBit::NoDraw, true);    //ピース購入画面が開いているなら非表示にする
                    line_ui->SetStatus(Object::StatusBit::NoDraw, true);        //ピース購入画面が開いているなら非表示にする
                    next_exp_ui->SetStatus(Object::StatusBit::NoDraw, true);    //ピース購入画面が開いているなら非表示にする
                }
                else {
                    exp_button->SetStatus(Object::StatusBit::NoDraw, false);     //それ以外なら表示する
                    curr_exp_ui->SetStatus(Object::StatusBit::NoDraw, false);    //それ以外なら表示する
                    line_ui->SetStatus(Object::StatusBit::NoDraw, false);        //それ以外なら表示する
                    next_exp_ui->SetStatus(Object::StatusBit::NoDraw, false);    //それ以外なら表示する
                }
            };
            auto event_handle = event_bus->subscribe<PiecePurchaseOpenClickEvent>(exp_button_hide_proc, 0);
            event_handles_.push_back(std::move(event_handle));
        }
    }
    //---------------------------------------------------------------------------------
    //  売却ボタン
    //---------------------------------------------------------------------------------
    {
        auto sell_button = Scene::Object::Create<UIButton>();    //売却ボタン
        sell_button->SetIsFilter(true);                          // クリック判定をUIヒットマネージャーでフィルタリングするように設定
        //sell_button->SetStatus(Object::StatusBit::NoDraw, true);    //表示しない状態から開始
        sell_button->SetImage(ImageBuffer::GetImageHandle("sell_button"));
        sell_button->SetScaleAxisXYZ(0.3f);                          //大きさを少し小さく設定
        sell_button->SetTranslate(float3(1050.0f, 500.0f, 0.0f));    //位置を画面右下あたりに設定
        //左クリックを促す
        sell_button->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
        //クリック時の処理
        auto click_func = [player]() {
            if(auto select_piece = player->GetSelectedPiece()) {
                int sell_piece_price = select_piece->GetPrice();    //売却価格を取得
                PiecePool::ReturnPieceToPool(select_piece->GetNameDefault().data(), select_piece->GetLevel());
                Scene::Object::Release(select_piece);    //選択されているオブジェクトを解放する
                player->ReleaseSelectedPiece();          //選択中のピースをクリアする
                player->AddGold(sell_piece_price);       //ゴールドを加算する(売却値の分)
            }
        };
        sell_button->SetClickFunc(click_func);
        //---------------------------------------------------------------------------------
        // ピース購入画面を開くボタンを押したときに、売却ボタンを非表示にする処理を登録
        //---------------------------------------------------------------------------------
        auto sell_button_hide_proc = [sell_button](const PiecePurchaseOpenClickEvent& e) {
            if(e.is_open_) {
                sell_button->SetStatus(Object::StatusBit::NoDraw, true);    //ピース購入画面が開いているなら非表示にする
            }
            else {
                sell_button->SetStatus(Object::StatusBit::NoDraw, false);    //それ以外なら表示する
            }
        };
        auto event_handle = event_bus->subscribe<PiecePurchaseOpenClickEvent>(sell_button_hide_proc, 0);
        event_handles_.push_back(std::move(event_handle));
    }
    //ボード制限に関するUIをベクターへ保管
    std::vector<std::shared_ptr<UIObject>> board_limit_vector;
    //---------------------------------------------------------------------------------
    //  駒数UI
    //---------------------------------------------------------------------------------
    {
        auto piece_num_ui = Scene::Object::Create<UIText>();
        piece_num_ui->SetFontSize(80);                                         //フォントサイズ設定
        piece_num_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));    //文字色設定
        //更新処理
        auto set_text_proc = [piece_num_ui]() {
            auto player    = Scene::Object::Get<Player>();
            int  piece_num = player->GetPlacedPieceNum();    //置かれているピースの数を取得
            //駒数を表示
            piece_num_ui->SetText(std::to_string(piece_num));
            int piece_max_num = player->GetAgentLevel();
            if(piece_num > piece_max_num) {
                //駒数が制限を超えていたら赤く表示
                piece_num_ui->SetColor(GetColor(255, 0, 0), GetColor(255, 255, 255));
            }
            else if(piece_num == piece_max_num) {
                //駒数が制限と同じなら緑に表示
                piece_num_ui->SetColor(GetColor(0, 255, 0), GetColor(255, 255, 255));
            }
            else {
                //それ以外なら黒
                piece_num_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));
            }
        };
        piece_num_ui->SetProc("set_text", set_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
        piece_num_ui->SetTranslate(float3(WINDOW_W * 0.5f - 50.0f, WINDOW_H * 0.5f - 100.0f, 0.0f));
        piece_num_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        board_limit_vector.push_back(piece_num_ui);
    }
    //---------------------------------------------------------------------------------
    //  割線UI
    //---------------------------------------------------------------------------------
    {
        auto line_ui = Scene::Object::Create<UIText>();
        line_ui->SetFontSize(80);                                         //フォントサイズ設定
        line_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));    //文字色設定
        line_ui->SetText("/");
        line_ui->SetTranslate(float3(WINDOW_W * 0.5f, WINDOW_H * 0.5f - 100.0f, 0.0f));
        line_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        board_limit_vector.push_back(line_ui);
    }
    //---------------------------------------------------------------------------------
    //  駒数制限UI
    //---------------------------------------------------------------------------------
    {
        auto piece_max_ui = Scene::Object::Create<UIText>();
        piece_max_ui->SetFontSize(80);                                         //フォントサイズ設定
        piece_max_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));    //文字色設定
        //更新処理
        auto set_text_proc = [piece_max_ui]() {
            auto player = Scene::Object::Get<Player>();
            int  level  = player->GetAgentLevel();
            piece_max_ui->SetText(std::to_string(level));
        };
        piece_max_ui->SetProc("set_level", set_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
        piece_max_ui->SetTranslate(float3(WINDOW_W * 0.5f + 50.0f, WINDOW_H * 0.5f - 100.0f, 0.0f));
        piece_max_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        board_limit_vector.push_back(piece_max_ui);
    }
    //---------------------------------------------------------------------------------
    // 駒関係UIはバトルフェーズに非表示にする処理を登録
    //---------------------------------------------------------------------------------
    {
        for(auto& ui_object : board_limit_vector) {
            auto board_limit_update_proc = [this, ui_object]() {
                if(game_state_ == GameState::Battle) {
                    ui_object->SetStatus(Object::StatusBit::NoDraw, true);    //バトルフェーズなら非表示にする
                }
                else {
                    ui_object->SetStatus(Object::StatusBit::NoDraw, false);    //それ以外なら表示する
                }
            };
            ui_object->SetProc("board_limit_update", board_limit_update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        };
    }
    //---------------------------------------------------------------------------------
    //  ターン数表示UI
    //---------------------------------------------------------------------------------
    {
        auto turn_ui = Scene::Object::Create<UIText>();
        turn_ui->SetFontSize(50);                                                //フォントサイズ設定
        turn_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));           //文字色設定
        turn_ui->SetTranslate(float3(100.0f, 100.0f, 0.0f));                     //位置を左上部に設定
        turn_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //左上寄せに設定
        //更新処理
        auto set_text_proc = [this, turn_ui]() { turn_ui->SetText("Turn: " + std::to_string(turn_count_)); };
        turn_ui->SetProc("set_turn", set_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
    }
    //---------------------------------------------------------------------------------
    //  フェーズタイマーUI
    //---------------------------------------------------------------------------------
    {
        auto phase_timer_ui = Scene::Object::Create<UIText>();
        phase_timer_ui->SetFontSize(50);                                                //フォントサイズ設定
        phase_timer_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));           //文字色設定
        phase_timer_ui->SetTranslate(float3(WINDOW_W * 0.5f, 100.0f, 0.0f));            //位置を上部中央あたりに設定
        phase_timer_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央寄せに設定
        phase_timer_ui->SetText("Time : ∞");
        //更新処理
        auto set_text_proc = [this, phase_timer_ui]() {
            if(tutorial_active_) {
                return;    // チュートリアル中はタイマーを更新しない
            }
            int time_left = 0;
            switch(game_state_) {
            case GameState::Setup:
                time_left = static_cast<int>(SETUP_PHASE_DURATION - state_timer_);
                break;
            case GameState::Battle:
                time_left = static_cast<int>(BATTLE_PHASE_DURATION - state_timer_);
                break;
            default:
                break;
            }
            phase_timer_ui->SetText("Time : " + std::to_string(time_left) + "s");
        };
        phase_timer_ui->SetProc("set_phase_timer", set_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
    }
    //---------------------------------------------------------------------------------
    //  フェーズ表示UI
    //---------------------------------------------------------------------------------
    {
        auto phase_ui = Scene::Object::Create<UIText>();
        phase_ui->SetFontSize(50);                                                //フォントサイズ設定
        phase_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));           //文字色設定
        phase_ui->SetTranslate(float3(1000.0f, 100.0f, 0.0f));                    //位置を上部に設定
        phase_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //右上寄せに設定
        //更新処理
        auto set_text_proc = [this, phase_ui]() {
            switch(game_state_) {
            case GameState::Setup:
                phase_ui->SetText("Setup Phase");
                break;
            case GameState::Battle:
                phase_ui->SetText("Battle Phase");
                break;
            default:
                break;
            }
        };
        phase_ui->SetProc("set_phase", set_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
    }
    //---------------------------------------------------------------------------------
    //  Agent表示UI
    //---------------------------------------------------------------------------------
    {
        int                                  agent_count = 0;
        auto                                 agents      = Scene::Object::GetArray<Agent>();
        std::vector<std::shared_ptr<Object>> agent_ui_objects;    //Agent情報のUI群
        for(auto& agent : agents) {
            agent_count++;
            //---------------------------------------------------------------------------------
            // 名前表示UI
            //---------------------------------------------------------------------------------
            auto agent_ui = Scene::Object::Create<UIText>();
            agent_ui->SetFontSize(30);                                                      //フォントサイズ設定
            agent_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));                 //文字色設定
            agent_ui->SetTranslate(float3(20.0f, 100.0f + (agent_count * 40.0f), 0.0f));    //位置を左上あたりに設定
            agent_ui->SetAlignment(ComponentTransformUI::Alignment::UpperLeft);             //左上寄せに設定
            agent_ui->SetText(agent->GetName());                                            //エージェント名を表示
            agent_ui_objects.push_back(agent_ui);
            //---------------------------------------------------------------------------------
            // 所持ゴールドのアイコンUI
            //---------------------------------------------------------------------------------
            auto gold_icon_ui = Scene::Object::Create<UIImage>();
            gold_icon_ui->SetImage(ImageBuffer::GetImageHandle("gold_icon"));                    //ゴールドアイコン画像を設定
            gold_icon_ui->SetTranslate(float3(150.0f, 115.0f + (agent_count * 40.0f), 0.0f));    //位置を左上あたりに設定
            gold_icon_ui->SetScaleAxisXYZ(0.15f);                                                //大きさを少し小さく設定
            agent_ui_objects.push_back(gold_icon_ui);
            //---------------------------------------------------------------------------------
            // エージェントの所持ゴールド表示UI
            //---------------------------------------------------------------------------------
            auto gold_ui = Scene::Object::Create<UIText>();
            gold_ui->SetFontSize(30);                                                       //フォントサイズ設定
            gold_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));                  //文字色設定
            gold_ui->SetTranslate(float3(170.0f, 100.0f + (agent_count * 40.0f), 0.0f));    //位置を左上あたりに設定
            gold_ui->SetAlignment(ComponentTransformUI::Alignment::UpperLeft);              //左上寄せに設定
            //更新処理
            auto set_text_proc = [agent, gold_ui]() {
                gold_ui->SetText(std::to_string(agent->GetGold()));    //所持ゴールドを表示
            };
            gold_ui->SetProc("set_gold", set_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
            agent_ui_objects.push_back(gold_ui);
            //---------------------------------------------------------------------------------
            // エージェントのHP表示UI
            //---------------------------------------------------------------------------------
            auto hp_gauge = Scene::Object::Create<UIGauge>();
            hp_gauge->SetTranslate(float3(280.0f, 115.0f + (agent_count * 40.0f), 0.0f));    //位置を左上あたりに設定
            hp_gauge->SetGaugeSize(int2(100, 20));                                           //ゲージサイズ設定
            //更新処理
            auto set_gauge_proc = [agent, hp_gauge]() {
                float hp_ratio = static_cast<float>(agent->GetHP()) / static_cast<float>(MAX_AGENT_HP);
                hp_gauge->SetGaugeRate(hp_ratio);
            };
            hp_gauge->SetProc("set_hp_gauge", set_gauge_proc, ProcTiming::Update, ProcPriority::NORMAL);
            agent_ui_objects.push_back(hp_gauge);
            //---------------------------------------------------------------------------------
            // エージェントのHP数値表示UI
            //---------------------------------------------------------------------------------
            auto hp_ui = Scene::Object::Create<UIText>();
            hp_ui->SetTranslate(float3(280.0f, 115.0f + (agent_count * 40.0f), 0.0f));    //位置を左上あたりに設定
            hp_ui->SetName(std::string(agent->GetName()) + "HPText");
            hp_ui->SetFontSize(25);                                         //フォントサイズ設定
            hp_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));    //文字色設定
            agent_ui_objects.push_back(hp_ui);
        }
        //---------------------------------------------------------------------------------
        // プレイヤーがピースを選択中でないならエージェント情報を表示する
        //---------------------------------------------------------------------------------
        for(auto& obj : agent_ui_objects) {
            auto agent_ui_update_proc = [obj, player]() {
                bool is_visible = player->IsSelectingPiece();
                //エージェント情報UI群の表示・非表示を切り替え
                obj->SetStatus(Object::StatusBit::NoDraw, is_visible);
            };
            // 処理を登録
            obj->SetProc("agent_ui_update_proc", agent_ui_update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        }
    }
    //---------------------------------------------------------------------------------
    // ピース情報UI
    //---------------------------------------------------------------------------------
    {
        auto                                 piece_repository = game_context_.GetPieceRepository();    // ピースリポジトリを事前に取得しておく
        std::vector<std::shared_ptr<Object>> piece_ditail_ui_objects;                                  //Agent情報のUI群
        //---------------------------------------------------------------------------------
        // ピース詳細背景画像
        //---------------------------------------------------------------------------------
        {
            auto piece_detail_back = Scene::Object::Create<UIImage>();
            piece_detail_back->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
            piece_detail_back->SetImage(ImageBuffer::GetImageHandle("piece_detail_back"));
            piece_detail_back->SetIsFilter(true);                             //フィルターに設定
            piece_detail_back->SetTranslate(float3(170.0f, 300.0f, 0.0f));    //位置を左中央あたりに設定
            piece_detail_back->SetAlpha(128);                                 //透明度を設定
            piece_detail_back->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            piece_detail_back->SetScaleAxisXYZ(1.5f);    //大きさを少し大さく設定
            piece_ditail_ui_objects.push_back(piece_detail_back);
        }
        //---------------------------------------------------------------------------------
        // ピース名表示UI
        //---------------------------------------------------------------------------------
        {
            auto piece_name_ui = Scene::Object::Create<UIText>();
            piece_name_ui->SetTranslate(float3(170.0f, 150.0f, 0.0f));              //位置を左中央あたりに設定
            piece_name_ui->SetFontName("游明朝");                                   //フォントを設定
            piece_name_ui->SetFontSize(20);                                         //フォントサイズ設定
            piece_name_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));    //文字色設定
            piece_name_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            auto update_proc = [piece_name_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    //ピース情報UIに情報を設定
                    auto piece_data = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                    piece_name_ui->SetText(piece_data->display_name_);
                }
            };
            piece_name_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(piece_name_ui);
        }
        //---------------------------------------------------------------------------------
        // 駒のレベルUI
        //---------------------------------------------------------------------------------
        {
            auto piece_level_ui = Scene::Object::Create<UIImage>();
            piece_level_ui->SetTranslate(float3(170.0f, 180.0f, 0.0f));
            piece_level_ui->SetScaleAxisXYZ(0.1f);    //大きさを少し小さく設定
            piece_level_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            auto update_proc = [piece_level_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    //ピースのレベルを取得
                    int piece_level = piece->GetLevel();
                    //レベルに応じた画像を設定
                    piece_level_ui->SetImage(ImageBuffer::GetImageHandle(std::string("level" + std::to_string(piece_level) + "_star")));
                }
            };
            piece_level_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(piece_level_ui);
        }
        //---------------------------------------------------------------------------------
        // ピースアイコンUI
        //---------------------------------------------------------------------------------
        {
            auto piece_icon_ui = Scene::Object::Create<UIImage>();
            piece_icon_ui->SetTranslate(float3(90.0f, 200.0f, 0.0f));
            piece_icon_ui->SetScaleAxisXYZ(0.3f);    //大きさを少し小さく設定
            piece_icon_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            auto update_proc = [piece_icon_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    //ピース情報UIに情報を設定
                    auto piece_data = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                    piece_icon_ui->SetImage(ImageBuffer::GetImageHandle(piece_data->icon_path_));
                }
            };
            piece_icon_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(piece_icon_ui);
        }

        //---------------------------------------------------------------------------------
        // ピースのHP表示UI
        //---------------------------------------------------------------------------------
        {
            auto piece_hp_ui = Scene::Object::Create<UIGauge>();
            piece_hp_ui->SetTranslate(float3(210.0f, 220.0f, 0.0f));    //位置を左中央あたりに設定
            piece_hp_ui->SetGaugeSize(int2(150, 20));                   //ゲージサイズ設定
            auto update_proc = [piece_hp_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    //ピース情報UIに情報を設定
                    auto  piece_data        = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                    int   piece_level       = piece->GetLevel();
                    int   index             = piece_level - 1;
                    auto  piece_level_datas = piece_data->levels_;
                    float hp_ratio          = static_cast<float>(piece->GetHP()) / static_cast<float>(piece_level_datas[index].hp_);
                    piece_hp_ui->SetGaugeRate(hp_ratio);
                }
            };
            piece_hp_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(piece_hp_ui);
        }
        //---------------------------------------------------------------------------------
        // 攻撃力アイコンUI
        //---------------------------------------------------------------------------------
        {
            auto attack_icon_ui = Scene::Object::Create<UIImage>();
            attack_icon_ui->SetTranslate(float3(80.0f, 260.0f, 0.0f));
            attack_icon_ui->SetScaleAxisXYZ(0.2f);    //大きさを少し小さく設定
            attack_icon_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            attack_icon_ui->SetImage(ImageBuffer::GetImageHandle("attack_power_icon"));
            piece_ditail_ui_objects.push_back(attack_icon_ui);
        }
        //---------------------------------------------------------------------------------
        // 攻撃力表示UI
        //---------------------------------------------------------------------------------
        {
            auto attack_power_ui = Scene::Object::Create<UIText>();
            attack_power_ui->SetTranslate(float3(130.0f, 260.0f, 0.0f));    //位置を左中央あたりに設定
            attack_power_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            attack_power_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));    //文字色設定
            attack_power_ui->SetFontSize(26);                                         //少しだけ大きく
            auto update_proc = [attack_power_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    //ピース情報UIに情報を設定
                    auto piece_data        = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                    int  piece_level       = piece->GetLevel();
                    int  index             = piece_level - 1;
                    auto piece_level_datas = piece_data->levels_;
                    attack_power_ui->SetText(std::to_string(piece_level_datas[index].attack_));
                }
            };
            attack_power_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(attack_power_ui);
        }
        //---------------------------------------------------------------------------------
        // 攻撃間隔アイコンUI
        //---------------------------------------------------------------------------------
        {
            auto attack_interval_icon_ui = Scene::Object::Create<UIImage>();
            attack_interval_icon_ui->SetTranslate(float3(200.0f, 260.0f, 0.0f));
            attack_interval_icon_ui->SetScaleAxisXYZ(0.2f);    //大きさを少し小さく設定
            attack_interval_icon_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            attack_interval_icon_ui->SetImage(ImageBuffer::GetImageHandle("attack_interval_icon"));
            piece_ditail_ui_objects.push_back(attack_interval_icon_ui);
        }
        //---------------------------------------------------------------------------------
        // 攻撃間隔表示UI
        //---------------------------------------------------------------------------------
        {
            auto attack_interval_ui = Scene::Object::Create<UIText>();
            attack_interval_ui->SetTranslate(float3(230.0f, 260.0f, 0.0f));    //位置を左中央あたりに設定
            attack_interval_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            attack_interval_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));    //文字色設定
            attack_interval_ui->SetFontSize(26);                                         //少しだけ大きく
            auto update_proc = [attack_interval_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    //ピース情報UIに情報を設定
                    auto piece_data = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                    attack_interval_ui->SetText(std::format("{:.1f}", piece_data->attack_interval_));
                }
            };
            attack_interval_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(attack_interval_ui);
        }
        //---------------------------------------------------------------------------------
        // 攻撃範囲アイコンUI
        //---------------------------------------------------------------------------------
        {
            auto attack_range_icon_ui = Scene::Object::Create<UIImage>();
            attack_range_icon_ui->SetTranslate(float3(80.0f, 300.0f, 0.0f));
            attack_range_icon_ui->SetScaleAxisXYZ(0.2f);    //大きさを少し小さく設定
            attack_range_icon_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            attack_range_icon_ui->SetImage(ImageBuffer::GetImageHandle("attack_range_icon"));
            piece_ditail_ui_objects.push_back(attack_range_icon_ui);
        }
        //---------------------------------------------------------------------------------
        // 攻撃範囲表示UI
        //---------------------------------------------------------------------------------
        {
            auto attack_range_ui = Scene::Object::Create<UIText>();
            attack_range_ui->SetTranslate(float3(130.0f, 300.0f, 0.0f));    //位置を左中央あたりに設定
            attack_range_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            attack_range_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));    //文字色設定
            attack_range_ui->SetFontSize(26);                                         //少しだけ大きく
            auto update_proc = [attack_range_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    //ピース情報UIに情報を設定
                    auto piece_data = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                    attack_range_ui->SetText(std::format("{:.1f}", piece_data->attack_range_));
                }
            };
            attack_range_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(attack_range_ui);
        }
        //---------------------------------------------------------------------------------
        // 物理防御アイコンUI
        //---------------------------------------------------------------------------------
        {
            auto physical_defense_icon_ui = Scene::Object::Create<UIImage>();
            physical_defense_icon_ui->SetTranslate(float3(200.0f, 300.0f, 0.0f));
            physical_defense_icon_ui->SetScaleAxisXYZ(0.2f);    //大きさを少し小さく設定
            physical_defense_icon_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            physical_defense_icon_ui->SetImage(ImageBuffer::GetImageHandle("physical_defense_icon"));
            piece_ditail_ui_objects.push_back(physical_defense_icon_ui);
        }
        //---------------------------------------------------------------------------------
        // 物理防御表示UI
        //---------------------------------------------------------------------------------
        {
            auto physical_defense_ui = Scene::Object::Create<UIText>();
            physical_defense_ui->SetTranslate(float3(230.0f, 300.0f, 0.0f));    //位置を左中央あたりに設定
            physical_defense_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            physical_defense_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));    //文字色設定
            physical_defense_ui->SetFontSize(26);                                         //少しだけ大きく
            auto update_proc = [physical_defense_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    //ピース情報UIに情報を設定
                    auto piece_data = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                    physical_defense_ui->SetText(std::to_string(piece_data->physical_defense_));
                }
            };
            physical_defense_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(physical_defense_ui);
        }
        //---------------------------------------------------------------------------------
        // 魔法防御アイコンUI
        //---------------------------------------------------------------------------------
        {
            auto magic_defense_icon_ui = Scene::Object::Create<UIImage>();
            magic_defense_icon_ui->SetTranslate(float3(80.0f, 340.0f, 0.0f));
            magic_defense_icon_ui->SetScaleAxisXYZ(0.2f);    //大きさを少し小さく設定
            magic_defense_icon_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            magic_defense_icon_ui->SetImage(ImageBuffer::GetImageHandle("magical_defense_icon"));
            piece_ditail_ui_objects.push_back(magic_defense_icon_ui);
        }
        //---------------------------------------------------------------------------------
        // 魔法防御表示UI
        //---------------------------------------------------------------------------------
        {
            auto magic_defense_ui = Scene::Object::Create<UIText>();
            magic_defense_ui->SetTranslate(float3(130.0f, 340.0f, 0.0f));    //位置を左中央あたりに設定
            magic_defense_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            magic_defense_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));    //文字色設定
            magic_defense_ui->SetFontSize(26);                                         //少しだけ大きく
            auto update_proc = [magic_defense_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    //ピース情報UIに情報を設定
                    auto piece_data = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                    magic_defense_ui->SetText(std::format("{:.1f}", piece_data->attack_interval_));
                }
            };
            magic_defense_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(magic_defense_ui);
        }
        //---------------------------------------------------------------------------------
        // スキルアイコンのUI
        //---------------------------------------------------------------------------------
        {
            auto skill_icon_ui = Scene::Object::Create<UIButton>();
            skill_icon_ui->SetTranslate(float3(170.0f, 410.0f, 0.0f));
            skill_icon_ui->SetScaleAxisXYZ(0.4f);    //大きさを少し小さく設定
            skill_icon_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            auto update_proc = [skill_icon_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    if(auto skill_comp = piece->GetComponent<ComponentActiveSkill>()) {
                        //ピース情報UIに情報を設定
                        auto skill_icon_key = skill_comp->GetSkillIconKey();
                        skill_icon_ui->SetImage(ImageBuffer::GetImageHandle(skill_icon_key));
                    }
                }
            };
            skill_icon_ui->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
            skill_icon_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(skill_icon_ui);
            //クリック時の処理を設定
            auto click_func = [event_bus, player]() {
                auto select_piece = player->GetSelectedPiece();
                if(auto skill_comp = select_piece->GetComponent<ComponentActiveSkill>()) {
                    auto skill_data = skill_comp->GetMasterData();
                    event_bus->publish(SkillClickEvent(skill_data));
                }
            };
            skill_icon_ui->SetClickFunc(click_func);
        }
        //---------------------------------------------------------------------------------
        // スキルの名前を表示するUI
        //---------------------------------------------------------------------------------
        {
            auto skill_name_ui = Scene::Object::Create<UIText>();
            skill_name_ui->SetTranslate(float3(170.0f, 450.0f, 0.0f));              //位置を左中央あたりに設定
            skill_name_ui->SetFontName("游明朝");                                   //フォントを設定
            skill_name_ui->SetFontSize(25);                                         //フォントサイズ設定
            skill_name_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));    //文字色設定
            skill_name_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
            auto update_proc = [skill_name_ui, piece_repository, player]() {
                //選択されているピースを取得
                if(auto piece = player->GetSelectedPiece()) {
                    if(auto skill_comp = piece->GetComponent<ComponentActiveSkill>()) {
                        //ピース情報UIに情報を設定
                        skill_name_ui->SetText(skill_comp->GetSkillName());
                    }
                }
            };
            skill_name_ui->SetProc("update_piece_detail", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
            piece_ditail_ui_objects.push_back(skill_name_ui);
        }
        //---------------------------------------------------------------------------------
        // プレイヤーがピースを選択中ならエージェント情報を表示する
        //---------------------------------------------------------------------------------
        for(auto& obj : piece_ditail_ui_objects) {
            auto agent_ui_update_proc = [obj, player]() {
                bool is_visible = !player->IsSelectingPiece();
                //エージェント情報UI群の表示・非表示を切り替え
                obj->SetStatus(Object::StatusBit::NoDraw, is_visible);
            };
            // 処理を登録
            obj->SetProc("agent_ui_update_proc", agent_ui_update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        }
    }
    std::vector<std::shared_ptr<Object>> purchase_window_objects;    //購入画面のウィンドウ群
    //---------------------------------------------------------------------------------
    //  購入画面のフィルター
    //---------------------------------------------------------------------------------
    {
        auto purchase_window_filter = Scene::Object::Create<UIImage>();        //フィルターの宣言
        purchase_window_filter->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        purchase_window_filter->SetScaleAxisXYZ(15.0f);                        //大きさを画面全体に設定
        purchase_window_filter->SetAlpha(168);                                 //透明度を設定
        float x = WINDOW_W * 0.5f;
        float y = WINDOW_H * 0.5f;
        purchase_window_filter->SetTranslate(float3(x, y, 0.0f));
        auto image_comp = purchase_window_filter->GetComponent<ComponentImage>();
        purchase_window_objects.push_back(purchase_window_filter);    //購入画面のウィンドウ群に追加
    }
    //---------------------------------------------------------------------------------
    //  ピース購入ボタン
    //---------------------------------------------------------------------------------
    {
        auto shop_pieces = player->GetShopPieces();    //ショップに並んでいるピースを取得
        for(int i = 0; i < shop_pieces.size(); ++i) {
            auto piece_purchase_button = Scene::Object::Create<UIButton>();
            piece_purchase_button->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
            piece_purchase_button->SetName("PiecePurchaseButton");
            float x_pos = 300.0f + (i * 165.0f);    //X位置を設定
            piece_purchase_button->SetTranslate(float3(x_pos, 300.0f, 0.0f));
            auto texture = std::make_shared<Texture>(100, 100, DXGI_FORMAT_R8G8B8A8_UNORM);
            //int screen_buff = MakeScreen(100, 200, false);                           //スクリーンバッファを作成
            piece_purchase_button->SetImage(ImageBuffer::GetImageHandle("deff"));    //仮で空の画像を設定
            //---------------------------------------------------------------------------------
            //  クリック時処理の設定
            //---------------------------------------------------------------------------------
            auto click_func = [i, player]() {
                //ピーズスタンドが満タンなら購入できないようにする
                if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
                    if(piece_stand->IsFull()) {
                        return;
                    }
                    if(auto shop_stand = Scene::Object::Get<ShopStand>()) {
                        if(auto purchase_piece = shop_stand->GetShopPieces()[i].lock()) {
                            int piece_price = purchase_piece->GetPrice();
                            if(player->GetGold() >= piece_price) {
                                PieceInfo piece_info;
                                piece_info.SetTypeName(purchase_piece->GetNameDefault().data());
                                piece_info.SetOwner(player);       //ピースのオーナーをプレイヤーに設定
                                player->SpendGold(piece_price);    //ゴールド使用する
                                player->AddPieceToStand(piece_info);
                                piece_stand->AddPiece(std::move(purchase_piece));    //ピースを購入する
                                shop_stand->InvalidateShopPiece(i);                  //購入したピースをショップから無効化する
                                player->InvalidateShopPiece(i);                      //プレイヤー側のショップ情報も無効化する
                            }
                        }
                    }
                }
            };
            piece_purchase_button->SetClickFunc(click_func);    //クリック時の処理を設定
            //---------------------------------------------------------------------------------
            //  ターゲットをうつす処理を入れ込む。
            //---------------------------------------------------------------------------------
            auto draw_target = [piece_purchase_button, texture, i]() {
                if(auto shop_stand = Scene::Object::Get<ShopStand>()) {
                    auto shop_pieces = shop_stand->GetShopPieces();    //ショップのピースを取得
                    SetRenderTarget(texture.get(), nullptr);           //レンダーターゲットを変更
                    ClearColor(texture.get(), float4(0.0f, 0.0f, 0.0f, 0.0f));
                    MATRIX prev_mat      = GetCameraViewMatrix();
                    MATRIX prev_proj_mat = GetCameraProjectionMatrix();
                    //一旦ピースの一番目をうつす
                    if(auto draw_piece = shop_pieces[i].lock()) {
                        SetCameraPositionAndTarget_UpVecY(cast(draw_piece->GetTranslate() + float3(0.0f, 0.0f, -2.0f)),
                                                          cast(draw_piece->GetTranslate()));    //カメラをモデルの方に向ける
                        matrix proj_mat =
                            matrix::perspectiveFovLH(D2R(90.0f),
                                                     1.0f,
                                                     0.1f,
                                                     1000.0f);    //透視投影行列を作成(画角45度、アスペクト比1.0、前方クリップ距離0.1、後方クリップ距離1000.0)
                        SetupCamera_ProjectionMatrix(cast(proj_mat));
                        //モデルを描画
                        if(auto model = draw_piece->GetComponent<ComponentModel>()) {
                            MV1DrawModel(model->GetModel());
                        }
                    }
                    piece_purchase_button->SetImage(*texture);             //スクリーンを入れ込む。
                    SetRenderTarget(GetHdrBuffer(), GetDepthStencil());    //レンダーターゲットを戻す
                    //SetDrawScreen(DX_SCREEN_BACK);                       //描画先をバックバッファに戻す
                    SetCameraViewMatrix(prev_mat);                  //カメラ行列を戻す
                    SetupCamera_ProjectionMatrix(prev_proj_mat);    //遠近法投影行列を戻す
                }
            };
            piece_purchase_button->SetProc("draw_target", draw_target, ProcTiming::Draw, ProcPriority::NORMAL);
            purchase_window_objects.push_back(piece_purchase_button);    //購入画面のウィンドウ群に追加
        }
    }
    //---------------------------------------------------------------------------------
    // ショップピースの名前表示テキスト
    //---------------------------------------------------------------------------------
    {
        auto        shop_pieces      = player->GetShopPieces();               //ショップに並んでいるピースを取得
        const auto& piece_repository = game_context_.GetPieceRepository();    // ピースリポジトリを取得
        for(int i = 0; i < shop_pieces.size(); ++i) {
            auto piece_name_text = Scene::Object::Create<UIText>();
            piece_name_text->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
            piece_name_text->SetName("PieceNameText");
            float x_pos = 300.0f + (i * 165.0f);    //X位置を設定
            piece_name_text->SetTranslate(float3(x_pos, 370.0f, 0.0f));
            piece_name_text->SetFontSize(16);    //フォントサイズを設定
            piece_name_text->SetFontName("游明朝");
            piece_name_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央揃えに設定
            //---------------------------------------------------------------------------------
            //  更新処理の設定
            //---------------------------------------------------------------------------------
            auto update_func = [piece_name_text, piece_repository, i]() {
                if(auto shop_stand = Scene::Object::Get<ShopStand>()) {
                    auto shop_pieces = shop_stand->GetShopPieces();    //ショップのピースを取得
                    if(auto piece = shop_pieces[i].lock()) {
                        //ピース情報UIに情報を設定
                        auto piece_data = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                        piece_name_text->SetText(piece_data->display_name_);
                        piece_name_text->SetColor(PIECE_COLORS.at(piece_data->price_ - 1), GetColor(255, 255, 255));    //文字色をレベルの色に設定
                    }
                    else {
                        piece_name_text->SetText("");    //ピースがない場合は空文字にする
                    }
                }
            };
            piece_name_text->SetProc("update", update_func, ProcTiming::Update, ProcPriority::NORMAL);
            purchase_window_objects.push_back(piece_name_text);    //購入画面のウィンドウ群に追加
        }
    }
    //---------------------------------------------------------------------------------
    // ピースの価格の横のゴールドアイコン表示テキスト
    //---------------------------------------------------------------------------------
    {
        auto shop_pieces = player->GetShopPieces();    //ショップに並んでいるピースを取得
        for(int i = 0; i < shop_pieces.size(); ++i) {
            auto gold_icon_ui = Scene::Object::Create<UIImage>();
            gold_icon_ui->SetStatus(Object::StatusBit::NoDraw, true);      //初期状態では非表示にしておく
            gold_icon_ui->SetStatus(Object::StatusBit::NoUpdate, true);    //更新しない
            gold_icon_ui->SetName("GoldIconUI");
            float x_pos = 300.0f + (i * 165.0f) - 25.0f;    //X位置を設定
            gold_icon_ui->SetTranslate(float3(x_pos, 395.0f, 0.0f));
            gold_icon_ui->SetScaleAxisXYZ(0.1f);    //大きさを少し小さく設定
            gold_icon_ui->SetImage(ImageBuffer::GetImageHandle("gold_icon"));
            //---------------------------------------------------------------------------------
            //  更新処理の設定
            //---------------------------------------------------------------------------------
            auto update_func = [gold_icon_ui, i]() {
                if(auto shop_stand = Scene::Object::Get<ShopStand>()) {
                    auto shop_pieces = shop_stand->GetShopPieces();    //ショップのピースを取得
                    if(auto piece = shop_pieces[i].lock()) {
                        //ピース情報UIに情報を設定
                        gold_icon_ui->SetStatus(Object::StatusBit::NoDraw, false);    //ピースがある場合は表示にする
                    }
                    else {
                        gold_icon_ui->SetStatus(Object::StatusBit::NoDraw, true);    //ピースがない場合は非表示にする
                    }
                }
            };
            gold_icon_ui->SetProc("update", update_func, ProcTiming::Update, ProcPriority::NORMAL);
            purchase_window_objects.push_back(gold_icon_ui);    //購入画面のウィンドウ群に追加
        }
    }
    //---------------------------------------------------------------------------------
    // ピースの価格表示テキスト
    //---------------------------------------------------------------------------------
    {
        auto        shop_pieces      = player->GetShopPieces();               //ショップに並んでいるピースを取得
        const auto& piece_repository = game_context_.GetPieceRepository();    // ピースリポジトリを取得
        for(int i = 0; i < shop_pieces.size(); ++i) {
            auto piece_price_text = Scene::Object::Create<UIText>();
            piece_price_text->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
            piece_price_text->SetName("PiecePriceText");
            float x_pos = 300.0f + (i * 165.0f);    //X位置を設定
            piece_price_text->SetTranslate(float3(x_pos, 395.0f, 0.0f));
            piece_price_text->SetFontSize(16);    //フォントサイズを設定
            piece_price_text->SetFontName("游明朝");
            piece_price_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央揃えに設定
            //---------------------------------------------------------------------------------
            //  更新処理の設定
            //---------------------------------------------------------------------------------
            auto update_func = [piece_price_text, piece_repository, i]() {
                if(auto shop_stand = Scene::Object::Get<ShopStand>()) {
                    auto shop_pieces = shop_stand->GetShopPieces();    //ショップのピースを取得
                    if(auto piece = shop_pieces[i].lock()) {
                        //ピース情報UIに情報を設定
                        auto piece_data = piece_repository.FindByTypeName(piece->GetNameDefault().data());
                        piece_price_text->SetText(std::to_string(piece_data->price_));
                        piece_price_text->SetColor(PIECE_COLORS.at(piece_data->price_ - 1), GetColor(255, 255, 255));    //文字色をレベルの色に設定
                    }
                    else {
                        piece_price_text->SetText("");    //ピースがない場合は空文字にする
                    }
                }
            };
            piece_price_text->SetProc("update", update_func, ProcTiming::Update, ProcPriority::NORMAL);
            purchase_window_objects.push_back(piece_price_text);    //購入画面のウィンドウ群に追加
        }
    }

    //---------------------------------------------------------------------------------
    //  リロールボタン
    //---------------------------------------------------------------------------------
    {
        auto reroll_button = Scene::Object::Create<UIButton>();
        reroll_button->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        reroll_button->SetImage(ImageBuffer::GetImageHandle("reroll_button"));
        reroll_button->SetScaleAxisXYZ(0.5f);                          //大きさを少し小さく設定
        reroll_button->SetTranslate(float3(1150.0f, 300.0f, 0.0f));    //位置を画面右中央あたりに設定
        //左クリックを促す
        reroll_button->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
        //クリック時の処理
        auto click_func = [player]() {
            //ショップがロックされていなければリロール可能
            if(!player->IsShopLocked()) {
                //ピースリロールに2ゴールド消費する
                if(player->SpendGold(2)) {
                    player->RerollShopPieces();    //ショップのピースをリロールする
                }
            }
        };
        reroll_button->SetClickFunc(click_func);
        purchase_window_objects.push_back(reroll_button);    //購入画面のウィンドウ群に追加
    }
    //---------------------------------------------------------------------------------
    //  ロックボタンボタン
    //---------------------------------------------------------------------------------
    {
        auto lock_button = Scene::Object::Create<UIButton>();
        lock_button->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        lock_button->SetImage(ImageBuffer::GetImageHandle("unlocked_button"));
        lock_button->SetScaleAxisXYZ(0.5f);                         //大きさを少し小さく設定
        lock_button->SetTranslate(float3(150.0f, 300.0f, 0.0f));    //位置を画面左中央あたりに設定
        //左クリックを促す
        lock_button->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
        //クリック時の処理
        auto click_func = [player, lock_button]() {
            player->ToggleShopLockState();    //ショップのロックを切り替える
            //ロック状態に応じてボタンの見た目を変える
            if(player->IsShopLocked()) {
                lock_button->SetImage(ImageBuffer::GetImageHandle("locked_button"));
            }
            else {
                lock_button->SetImage(ImageBuffer::GetImageHandle("unlocked_button"));
            }
        };
        lock_button->SetClickFunc(click_func);
        purchase_window_objects.push_back(lock_button);    //購入画面のウィンドウ群に追加
    }
    //---------------------------------------------------------------------------------
    //  ピース購入画面を開けるボタン
    //---------------------------------------------------------------------------------
    {
        auto piece_purchase_open_button = Scene::Object::Create<PiecePurchaseOpenButton>("PiecePurchaseOpenButton");
        auto click_func                 = [this, purchase_window_objects, event_bus]() {
            is_purchase_open_ = !is_purchase_open_;    //ピース購入画面の開閉を切り替え
            //ウィンドウ群に対して開閉処理を行う
            if(is_purchase_open_) {
                for(auto& obj : purchase_window_objects) {
                    obj->SetStatus(Object::StatusBit::NoDraw, false);      //描画する
                    obj->SetStatus(Object::StatusBit::NoUpdate, false);    //更新する
                }
            }
            else {
                for(auto& obj : purchase_window_objects) {
                    obj->SetStatus(Object::StatusBit::NoDraw, true);      //描画しない
                    obj->SetStatus(Object::StatusBit::NoUpdate, true);    //更新しない
                }
            }
            //購入クリックイベントを発行
            event_bus->publish(PiecePurchaseOpenClickEvent(is_purchase_open_));
        };
        piece_purchase_open_button->SetClickFunc(click_func);    //クリック時の処理を設定
        //---------------------------------------------------------------------------------
        // ピース購入ボタンを押したときにシナジーアイコン群を非表示にする処理登録
        //---------------------------------------------------------------------------------
        auto synergy_hide_proc = [piece_purchase_open_button](const PiecePurchaseOpenClickEvent& e) {
            auto synergy_icons = Scene::Object::GetArray<UISynergy>();
            for(auto& icon : synergy_icons) {
                if(e.is_open_) {
                    //購入画面が開かれたらシナジーアイコンを非表示にする
                    icon->SetStatus(Object::StatusBit::NoDraw, true);
                }
                else {
                    //購入画面が閉じられたらシナジーアイコンを表示する
                    icon->SetStatus(Object::StatusBit::NoDraw, false);
                }
            };
        };
        auto event_handle = event_bus->subscribe<PiecePurchaseOpenClickEvent>(synergy_hide_proc, 0);
        event_handles_.push_back(std::move(event_handle));
    }
    //---------------------------------------------------------------------------------
    // スキル表示UIの作成
    //---------------------------------------------------------------------------------
    {
        std::vector<std::shared_ptr<UIObject>> skill_detail_ui_objects;    //スキル詳細UIオブジェクト群
        //---------------------------------------------------------------------------------
        // 背景画像
        //---------------------------------------------------------------------------------
        auto skill_detail_back = Scene::Object::Create<UIImage>();
        skill_detail_back->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        skill_detail_back->SetImage(ImageBuffer::GetImageHandle("piece_detail_back"));
        skill_detail_back->SetIsFilter(true);                             //フィルターに設定
        skill_detail_back->SetTranslate(float3(470.0f, 300.0f, 0.0f));    //位置を左中央あたりに設定
        skill_detail_back->SetAlpha(128);                                 //透明度を設定
        skill_detail_back->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        skill_detail_back->SetScaleAxisXYZ(1.5f);    //大きさを少し大さく設定
        skill_detail_ui_objects.push_back(skill_detail_back);
        //---------------------------------------------------------------------------------
        // スキルアイコンのUI
        //---------------------------------------------------------------------------------
        auto skill_icon_ui = Scene::Object::Create<UIImage>();
        skill_icon_ui->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        skill_icon_ui->SetTranslate(float3(380.0f, 165.0f, 0.0f));
        skill_icon_ui->SetScaleAxisXYZ(0.25f);    //大きさを少し小さく設定
        skill_icon_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        skill_detail_ui_objects.push_back(skill_icon_ui);

        //---------------------------------------------------------------------------------
        // スキルの名前を表示するUI
        //---------------------------------------------------------------------------------
        auto skill_name_ui = Scene::Object::Create<UIText>();
        skill_name_ui->SetStatus(Object::StatusBit::NoDraw, true);                   //初期状態では非表示にしておく
        skill_name_ui->SetTranslate(float3(430.0f, 170.0f, 0.0f));                   //位置を左中央あたりに設定
        skill_name_ui->SetFontName("游明朝");                                        //フォントを設定
        skill_name_ui->SetFontSize(20);                                              //フォントサイズ設定
        skill_name_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));         //文字色設定
        skill_name_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleLeft);    //左寄せに設定
        skill_detail_ui_objects.push_back(skill_name_ui);

        //---------------------------------------------------------------------------------
        // クールタイム表示UI
        //---------------------------------------------------------------------------------
        auto skill_cooltime_ui = Scene::Object::Create<UIText>();
        skill_cooltime_ui->SetStatus(Object::StatusBit::NoDraw, true);                     //初期状態では非表示にしておく
        skill_cooltime_ui->SetTranslate(float3(470.0f, 210.0f, 0.0f));                     //位置を左中央あたりに設定
        skill_cooltime_ui->SetFontName("游明朝");                                          //フォントを設定
        skill_cooltime_ui->SetFontSize(18);                                                //フォントサイズ設定
        skill_cooltime_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));           //文字色設定
        skill_cooltime_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //左寄せに設定
        skill_detail_ui_objects.push_back(skill_cooltime_ui);

        //---------------------------------------------------------------------------------
        // スキルの説明文UI
        //---------------------------------------------------------------------------------
        auto skill_description_ui = Scene::Object::Create<UIText>();
        skill_description_ui->SetStatus(Object::StatusBit::NoDraw, true);                   //初期状態では非表示にしておく
        skill_description_ui->SetTranslate(float3(365.0f, 270.0f, 0.0f));                   //位置を左中央あたりに設定
        skill_description_ui->SetFontName("游明朝");                                        //フォントを設定
        skill_description_ui->SetFontSize(22);                                              //フォントサイズ設定
        skill_description_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));         //文字色設定
        skill_description_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleLeft);    //左寄せに設定
        skill_description_ui->SetWrapWidth(230);
        skill_detail_ui_objects.push_back(skill_description_ui);
        //---------------------------------------------------------------------------------
        // スキル詳細UIの非表示
        //---------------------------------------------------------------------------------
        for(auto& obj : skill_detail_ui_objects) {
            auto skill_ui_update_proc = [obj, player]() {
                //左クリックであれば
                if(IsMouseOn(MOUSE_INPUT_LEFT)) {
                    if(!UIHitManager::IsMouseHitUIFilter()) {
                        //スキル詳細UI群の非表示
                        obj->SetStatus(Object::StatusBit::NoDraw, true);
                    }
                }
            };
            // 処理を登録
            obj->SetProc("skill_ui_update_proc", skill_ui_update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        }
        //---------------------------------------------------------------------------------
        // クリックイベントの登録
        //---------------------------------------------------------------------------------
        auto skill_click_event = [skill_detail_back, skill_icon_ui, skill_name_ui, skill_cooltime_ui, skill_description_ui](const SkillClickEvent& e) {
            //---------------------------------------------------------------------------------
            // 背景画像の表示
            //---------------------------------------------------------------------------------
            skill_detail_back->SetStatus(Object::StatusBit::NoDraw, false);    //背景画像を表示する
            //---------------------------------------------------------------------------------
            // スキルアイコンのUI
            //---------------------------------------------------------------------------------
            skill_icon_ui->SetStatus(Object::StatusBit::NoDraw, false);    //スキルアイコンUIを表示する
            skill_icon_ui->SetImage(ImageBuffer::GetImageHandle(e.skill_data_.skill_icon_key_));
            //---------------------------------------------------------------------------------
            // スキルの名前UI
            //---------------------------------------------------------------------------------
            skill_name_ui->SetStatus(Object::StatusBit::NoDraw, false);    //スキル名UIを表示する
            skill_name_ui->SetText(e.skill_data_.skill_name_);
            //---------------------------------------------------------------------------------
            // クールタイム表示UI
            //---------------------------------------------------------------------------------
            skill_cooltime_ui->SetStatus(Object::StatusBit::NoDraw, false);    //クールタイムUIを表示する
            skill_cooltime_ui->SetText(std::format("クールタイム: {:.1f}秒", e.skill_data_.max_cool_doen_time_));
            //---------------------------------------------------------------------------------
            // スキルの説明文UI
            //---------------------------------------------------------------------------------
            skill_description_ui->SetStatus(Object::StatusBit::NoDraw, false);
            skill_description_ui->SetText(e.skill_data_.skill_description_);
        };
        auto skill_click_event_handle = event_bus->subscribe<SkillClickEvent>(skill_click_event, 0);
        event_handles_.push_back(std::move(skill_click_event_handle));
    }
    //---------------------------------------------------------------------------------
    // シナジー情報のUI作成
    //---------------------------------------------------------------------------------
    {
        std::vector<std::shared_ptr<Object>> synergy_ui_objects;    //シナジー情報UIオブジェクト群
        //---------------------------------------------------------------------------------
        // シナジー情報背景UI
        //---------------------------------------------------------------------------------
        auto synergy_info_back = Scene::Object::Create<UIImage>();
        synergy_info_back->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        synergy_info_back->SetImage(ImageBuffer::GetImageHandle("piece_detail_back"));
        synergy_info_back->SetIsFilter(true);                             //フィルターに設定
        synergy_info_back->SetTranslate(float3(900.0f, 300.0f, 0.0f));    //位置を左中央あたりに設定
        synergy_info_back->SetAlpha(128);                                 //透明度を設定
        synergy_info_back->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        synergy_info_back->SetScaleAxisXYZ(1.5f);    //大きさを少し大さく設定
        synergy_ui_objects.push_back(synergy_info_back);
        //---------------------------------------------------------------------------------
        // シナジーの画像UI
        //---------------------------------------------------------------------------------
        auto synergy_info_image = Scene::Object::Create<UIImage>();
        synergy_info_image->SetStatus(Object::StatusBit::NoDraw, true);
        synergy_info_image->SetTranslate(float3(815.0f, 160.0f, 0.0f));
        synergy_info_image->SetScaleAxisXYZ(0.25f);    //大き
        synergy_info_image->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        synergy_ui_objects.push_back(synergy_info_image);

        //---------------------------------------------------------------------------------
        // シナジーの名前テキストUI
        //---------------------------------------------------------------------------------
        auto synergy_info_text = Scene::Object::Create<UIText>();
        synergy_info_text->SetStatus(Object::StatusBit::NoDraw, true);
        synergy_info_text->SetTranslate(float3(900.0f, 150.0f, 0.0f));    //位置を左中央あたりに設定
        synergy_info_text->SetFontName("游明朝");                         //フォントを設定
        synergy_info_text->SetFontSize(22);                               //フォントサイズ設定
        synergy_info_text->SetColor(GetColor(255, 255, 255));
        synergy_info_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央揃えに設定
        synergy_ui_objects.push_back(synergy_info_text);
        //---------------------------------------------------------------------------------
        // シナジー説明文UI
        //---------------------------------------------------------------------------------
        auto synergy_info_description = Scene::Object::Create<UIText>();
        synergy_info_description->SetStatus(Object::StatusBit::NoDraw, true);
        synergy_info_description->SetTranslate(float3(1030.0f, 210.0f, 0.0f));    //位置を左中央あたりに設定
        synergy_info_description->SetFontName("游明朝");                          //フォントを設定
        synergy_info_description->SetFontSize(20);                                //フォントサイズ設定
        synergy_info_description->SetColor(GetColor(255, 255, 255));
        synergy_info_description->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央
        synergy_info_description->SetWrapWidth(250);
        synergy_ui_objects.push_back(synergy_info_description);

        //---------------------------------------------------------------------------------
        // シナジー情報更新処理の登録
        //---------------------------------------------------------------------------------
        for(auto& obj : synergy_ui_objects) {
            auto skill_ui_update_proc = [obj, player, this]() {
                //左クリックであれば
                if(IsMouseOn(MOUSE_INPUT_LEFT)) {
                    if(!UIHitManager::IsMouseHitUIFilter()) {
                        //シナジー詳細UI群の非表示
                        obj->SetStatus(Object::StatusBit::NoDraw, true);
                        if(!is_purchase_open_) {
                            //シナジーアイコン群の表示
                            auto synergies = Scene::Object::GetArray<UISynergy>();
                            for(auto synergie : synergies) {
                                synergie->SetStatus(Object::StatusBit::NoDraw, false);
                            }
                        }
                    }
                }
            };
            // 処理を登録
            obj->SetProc("skill_ui_update_proc", skill_ui_update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        }
        //---------------------------------------------------------------------------------
        // シナジーUIクリック時の処理登録
        //---------------------------------------------------------------------------------
        auto synergy_ui_click_event = [synergy_info_back, synergy_info_text, synergy_info_image, synergy_info_description](const SynergyClickEvent& e) {
            //---------------------------------------------------------------------------------
            //シナジーアイコン群の非表示
            //---------------------------------------------------------------------------------
            auto synergies = Scene::Object::GetArray<UISynergy>();
            for(auto synergie : synergies) {
                synergie->SetStatus(Object::StatusBit::NoDraw, true);
            }
            //---------------------------------------------------------------------------------
            // シナジー情報背景UIの表示
            //---------------------------------------------------------------------------------
            synergy_info_back->SetStatus(Object::StatusBit::NoDraw, false);    //背景画像を表示する
            //---------------------------------------------------------------------------------
            // シナジーの名前テキストUIの表示
            //---------------------------------------------------------------------------------
            synergy_info_text->SetStatus(Object::StatusBit::NoDraw, false);
            synergy_info_text->SetText(e.synergy_data_->name_);
            //---------------------------------------------------------------------------------
            // シナジーの画像UIの表示
            //---------------------------------------------------------------------------------
            synergy_info_image->SetStatus(Object::StatusBit::NoDraw, false);
            synergy_info_image->SetImage(ImageBuffer::GetImageHandle(e.synergy_data_->icon_path_));
            //---------------------------------------------------------------------------------
            // 説明文の作成
            //---------------------------------------------------------------------------------
            auto level_thresholds = e.synergy_data_->level_thresholds_;
            synergy_info_description->SetStatus(Object::StatusBit::NoDraw, false);
            std::string description;
            for(int i = 0; i < level_thresholds.size(); i++) {
                std::string level_description  = std::format("{}:{}", level_thresholds[i], (e.synergy_data_->descriptions_.at(i) + "\n"));
                description                   += level_description;
            }
            synergy_info_description->SetText(description);
        };
        auto synergy_click_event_handle = event_bus->subscribe<SynergyClickEvent>(synergy_ui_click_event, 0);
        event_handles_.push_back(std::move(synergy_click_event_handle));
    }
    //---------------------------------------------------------------------------------
    // 自分のgoldを表示するUI
    //---------------------------------------------------------------------------------
    {
        //goldボタンUI
        auto gold_ui = Scene::Object::Create<UIButton>();
        //gold_ui->SetStatus(Object::StatusBit::NoDraw, true);
        gold_ui->SetTranslate(float3(1100.0f, 180.0f, 0.0f));    //位置を画面左上あたりに設定
        gold_ui->SetImage(ImageBuffer::GetImageHandle("gold_icon"));
        gold_ui->SetScaleAxisXYZ(0.2f);    //大きさ
        gold_ui->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
        //クリック時の処理
        auto gold_click_func = [event_bus]() { event_bus->publish(GoldClickEvent()); };
        gold_ui->SetClickFunc(gold_click_func);
        //goldテキストUI
        auto gold_text_ui = Scene::Object::Create<UIText>();
        //Fgold_text_ui->SetStatus(Object::StatusBit::NoDraw, true);
        gold_text_ui->SetTranslate(float3(1130.0f, 180.0f, 0.0f));             //位置を画面左上あたりに設定
        gold_text_ui->SetFontName("游明朝");                                   //フォントを設定
        gold_text_ui->SetFontSize(24);                                         //フォントサイズ設定
        gold_text_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));    //文字色設定
        gold_text_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleLeft);
        //gold更新処理F
        auto gold_update_proc = [gold_text_ui, player]() {
            //gold情報UIに情報を設定
            gold_text_ui->SetText(std::format("{}", player->GetGold()));
        };
        gold_text_ui->SetProc("update_gold_ui", gold_update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        //---------------------------------------------------------------------------------
        // 購入画面を押したときにGoldUIを非表示にする処理登録
        //---------------------------------------------------------------------------------
        auto gold_ui_hide_proc = [gold_ui, gold_text_ui](const PiecePurchaseOpenClickEvent& e) {
            if(e.is_open_) {
                //購入画面が開かれたらGoldUIを非表示にする
                gold_ui->SetStatus(Object::StatusBit::NoDraw, true);
                gold_text_ui->SetStatus(Object::StatusBit::NoDraw, true);
            }
            else {
                //購入画面が閉じられたらGoldUIを表示する
                gold_ui->SetStatus(Object::StatusBit::NoDraw, false);
                gold_text_ui->SetStatus(Object::StatusBit::NoDraw, false);
            }
        };
        auto event_handle = event_bus->subscribe<PiecePurchaseOpenClickEvent>(gold_ui_hide_proc, 0);
        event_handles_.push_back(std::move(event_handle));
    }
    //---------------------------------------------------------------------------------
    // Gold説明UI
    //---------------------------------------------------------------------------------
    {
        std::vector<std::shared_ptr<UIObject>> gold_info_ui_objects;    //gold説明UIオブジェクト群
        //説明の背景テキストUI
        auto gold_info_back = Scene::Object::Create<UIImage>();
        gold_info_back->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        gold_info_back->SetImage(ImageBuffer::GetImageHandle("gold_detail_back"));
        gold_info_back->SetAlpha(200);
        gold_info_back->SetScaleAxisXYZ(2.0f);
        gold_info_back->SetIsFilter(true);                                         //フィルターに設定
        gold_info_back->SetTranslate(float3(WINDOW_W / 2, WINDOW_H / 2, 0.0f));    //位置を中央あたりに設定
        gold_info_ui_objects.push_back(gold_info_back);
        //予想収益と表示するテキストUI
        auto gold_info_text = Scene::Object::Create<UIText>();
        gold_info_text->SetStatus(Object::StatusBit::NoDraw, true);    //初期状態では非表示にしておく
        gold_info_text->SetTranslate(float3(460.0f, 230.0f, 0.0f));    //位置を中央あたりに設定
        gold_info_text->SetFontName("游明朝");                         //フォントを設定
        gold_info_text->SetFontSize(22);                               //フォントサイズ設定
        gold_info_text->SetColor(GetColor(255, 255, 255));
        gold_info_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央
        gold_info_text->SetText("予想収益");
        gold_info_ui_objects.push_back(gold_info_text);
        //基礎収益テキストUI
        auto base_income_text = Scene::Object::Create<UIText>();
        base_income_text->SetStatus(Object::StatusBit::NoDraw, true);
        base_income_text->SetTranslate(float3(440.0f, 280.0f, 0.0f));
        base_income_text->SetFontName("游明朝");    //フォントを設定
        base_income_text->SetFontSize(20);          //フォントサイズ設定
        base_income_text->SetColor(GetColor(255, 255, 255));
        base_income_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央
        base_income_text->SetText("基礎");
        gold_info_ui_objects.push_back(base_income_text);
        //基礎収益予想テキストUI
        auto base_income_detail_text = Scene::Object::Create<UIText>();
        base_income_detail_text->SetStatus(Object::StatusBit::NoDraw, true);
        base_income_detail_text->SetTranslate(float3(440.0f, 435.0f, 0.0f));
        base_income_detail_text->SetFontName("游明朝");    //フォントを設定
        base_income_detail_text->SetFontSize(16);          //フォントサイズ設定
        base_income_detail_text->SetColor(GetColor(255, 255, 255));
        base_income_detail_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //
        base_income_detail_text->SetText("+2");
        gold_info_ui_objects.push_back(base_income_detail_text);
        //利子収益テキストUI
        auto interest_income_text = Scene::Object::Create<UIText>();
        interest_income_text->SetStatus(Object::StatusBit::NoDraw, true);
        interest_income_text->SetTranslate(float3(540.0f, 280.0f, 0.0f));
        interest_income_text->SetFontName("游明朝");    //フォントを設定
        interest_income_text->SetFontSize(20);          //フォントサイズ設定
        interest_income_text->SetColor(GetColor(255, 255, 255));
        interest_income_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央
        interest_income_text->SetText("利子");
        gold_info_ui_objects.push_back(interest_income_text);
        //利子収益予想テキストUI
        auto interest_income_detail_text = Scene::Object::Create<UIText>();
        interest_income_detail_text->SetStatus(Object::StatusBit::NoDraw, true);
        interest_income_detail_text->SetTranslate(float3(540.0f, 435.0f, 0.0f));
        interest_income_detail_text->SetFontName("游明朝");    //フォントを設定
        interest_income_detail_text->SetFontSize(16);          //フォントサイズ設定
        interest_income_detail_text->SetColor(GetColor(255, 255, 255));
        interest_income_detail_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        auto interest_income_update_proc = [interest_income_detail_text, player]() {
            int player_gold     = player->GetGold();
            int interest_income = std::min(player_gold / 10, 5);
            interest_income_detail_text->SetText(std::format("+{}", interest_income));
        };
        interest_income_detail_text->SetProc("update_interest_income_ui", interest_income_update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        gold_info_ui_objects.push_back(interest_income_detail_text);
        //勝利ボーナステキストUI
        auto win_bonus_text = Scene::Object::Create<UIText>();
        win_bonus_text->SetStatus(Object::StatusBit::NoDraw, true);
        win_bonus_text->SetTranslate(float3(640.0f, 280.0f, 0.0f));
        win_bonus_text->SetFontName("游明朝");    //フォントを設定
        win_bonus_text->SetFontSize(20);          //フォントサイズ設定
        win_bonus_text->SetColor(GetColor(255, 255, 255));
        win_bonus_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);    //中央
        win_bonus_text->SetText("勝利");
        gold_info_ui_objects.push_back(win_bonus_text);
        //勝利ボーナス予想テキストUI
        auto win_bonus_detail_text = Scene::Object::Create<UIText>();
        win_bonus_detail_text->SetStatus(Object::StatusBit::NoDraw, true);
        win_bonus_detail_text->SetTranslate(float3(640.0f, 435.0f, 0.0f));
        win_bonus_detail_text->SetFontName("游明朝");
        win_bonus_detail_text->SetFontSize(16);    //フォントサイズ設定
        win_bonus_detail_text->SetColor(GetColor(255, 255, 255));
        win_bonus_detail_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        win_bonus_detail_text->SetText("+1");
        gold_info_ui_objects.push_back(win_bonus_detail_text);
        //連勝ボーナステキストUI
        auto win_streak_bonus_text = Scene::Object::Create<UIText>();
        win_streak_bonus_text->SetStatus(Object::StatusBit::NoDraw, true);
        win_streak_bonus_text->SetTranslate(float3(740.0f, 280.0f, 0.0f));
        win_streak_bonus_text->SetFontName("游明朝");    //フォントを設定
        win_streak_bonus_text->SetFontSize(20);          //フォントサイズ設定
        win_streak_bonus_text->SetColor(GetColor(255, 255, 255));
        win_streak_bonus_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        win_streak_bonus_text->SetText("連勝");
        gold_info_ui_objects.push_back(win_streak_bonus_text);
        //連勝ボーナス予想テキストUI
        auto win_streak_bonus_detail_text = Scene::Object::Create<UIText>();
        win_streak_bonus_detail_text->SetStatus(Object::StatusBit::NoDraw, true);
        win_streak_bonus_detail_text->SetTranslate(float3(740.0f, 435.0f, 0.0f));
        win_streak_bonus_detail_text->SetFontName("游明朝");
        win_streak_bonus_detail_text->SetFontSize(16);    //フォントサイズ設定
        win_streak_bonus_detail_text->SetColor(GetColor(255, 255, 255));
        win_streak_bonus_detail_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        gold_info_ui_objects.push_back(win_streak_bonus_detail_text);
        //更新処理の登録
        auto win_streak_income_update_proc = [win_streak_bonus_detail_text, player]() {
            int win_streak = player->GetWinStreak();
            int bonus      = 0;
            if(win_streak >= 3) {
                bonus = win_streak;
            }
            win_streak = std::min(win_streak, 5);
            win_streak_bonus_detail_text->SetText(std::format("+{}", bonus));
        };
        win_streak_bonus_detail_text->SetProc("update_win_streak_income_ui", win_streak_income_update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        //連敗ボーナステキストUI
        auto loss_streak_bonus_text = Scene::Object::Create<UIText>();
        loss_streak_bonus_text->SetStatus(Object::StatusBit::NoDraw, true);
        loss_streak_bonus_text->SetTranslate(float3(840.0f, 280.0f, 0.0f));
        loss_streak_bonus_text->SetFontName("游明朝");    //フォント
        loss_streak_bonus_text->SetFontSize(20);          //フォントサイズ設定
        loss_streak_bonus_text->SetColor(GetColor(255, 255, 255));
        loss_streak_bonus_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        loss_streak_bonus_text->SetText("連敗");
        gold_info_ui_objects.push_back(loss_streak_bonus_text);
        //連敗ボーナス予想テキストUI
        auto loss_streak_bonus_detail_text = Scene::Object::Create<UIText>();
        loss_streak_bonus_detail_text->SetStatus(Object::StatusBit::NoDraw, true);
        loss_streak_bonus_detail_text->SetTranslate(float3(840.0f, 435.0f, 0.0f));
        loss_streak_bonus_detail_text->SetFontName("游明朝");
        loss_streak_bonus_detail_text->SetFontSize(16);    //フォントサイズ
        loss_streak_bonus_detail_text->SetColor(GetColor(255, 255, 255));
        loss_streak_bonus_detail_text->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
        gold_info_ui_objects.push_back(loss_streak_bonus_detail_text);
        //更新処理の登録
        auto loss_streak_income_update_proc = [loss_streak_bonus_detail_text, player]() {
            int loss_streak = player->GetLoseStreak();
            int bonus       = 0;
            if(loss_streak >= 3) {
                bonus = loss_streak;
            }
            loss_streak = std::min(loss_streak, 5);
            loss_streak_bonus_detail_text->SetText(std::format("+{}", bonus));
        };
        loss_streak_bonus_detail_text->SetProc("update_loss_streak_income_ui", loss_streak_income_update_proc, ProcTiming::Update, ProcPriority::NORMAL);
        //ゴールド画像UI
        for(int i = 0; i < 5; i++) {
            auto gold_info_icon = Scene::Object::Create<UIImage>();
            gold_info_icon->SetStatus(Object::StatusBit::NoDraw, true);
            gold_info_icon->SetImage(ImageBuffer::GetImageHandle("gold_icon"));
            gold_info_icon->SetScaleAxisXYZ(0.2f);
            gold_info_icon->SetTranslate(float3(440.0f + i * 100.0f, 380.0f, 0.0f));
            gold_info_ui_objects.push_back(gold_info_icon);
        }
        //---------------------------------------------------------------------------------
        // goldUI群の非表示処理登録
        //---------------------------------------------------------------------------------
        for(auto& obj : gold_info_ui_objects) {
            auto skill_ui_update_proc = [obj, player]() {
                //左クリックであれば
                if(IsMouseOn(MOUSE_INPUT_LEFT)) {
                    if(!UIHitManager::IsMouseHitUIFilter()) {
                        //スキル詳細UI群の非表示
                        obj->SetStatus(Object::StatusBit::NoDraw, true);
                    }
                }
            };
            // 処理を登録
            obj->SetProc("skill_ui_update_proc", skill_ui_update_proc, ProcTiming::PreUpdate, ProcPriority::NORMAL);
        }
        //---------------------------------------------------------------------------------
        // Gold説明UIの表示処理登録
        //---------------------------------------------------------------------------------
        auto gold_click_proc = [gold_info_ui_objects, this](const GoldClickEvent& e) {
            for(auto& obj : gold_info_ui_objects) {
                obj->SetStatus(Object::StatusBit::NoDraw, false);
            }
        };
        auto gold_info_event_handle = event_bus->subscribe<GoldClickEvent>(gold_click_proc, 0);
        event_handles_.push_back(std::move(gold_info_event_handle));
    }
    //---------------------------------------------------------------------------------
    // 中央に表示するUIの作成
    //---------------------------------------------------------------------------------
    {
        auto center_message_ui = Scene::Object::Create<UIText>();
        center_message_ui->SetStatus(Object::StatusBit::NoDraw, true);                //初期状態では非表示にしておく
        center_message_ui->SetTranslate(float3(WINDOW_W / 2, WINDOW_H / 2, 0.0f));    //位置を中央あたりに設定
        center_message_ui->SetFontSize(60);                                           //サイズを設定
        center_message_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));      //文字色設定
        center_message_ui->SetFontName("游明朝");                                     //フォントを設定
        float center_message_hide_timer = 0.0f;                                       //非表示タイマー
        //---------------------------------------------------------------------------------
        // 非表示処理の登録
        //---------------------------------------------------------------------------------
        auto center_message_hide_proc = [center_message_ui, &center_message_hide_timer]() {
            if(!center_message_ui->GetStatus(Object::StatusBit::NoDraw)) {
                center_message_hide_timer += GetDeltaTime();
                if(center_message_hide_timer >= 3.0f) {
                    center_message_ui->SetStatus(Object::StatusBit::NoDraw, true);
                    center_message_hide_timer = 0.0f;
                }
            }
        };
        center_message_ui->SetProc("hide_center_message_ui_proc", center_message_hide_proc, ProcTiming::Update, ProcPriority::NORMAL);
        //---------------------------------------------------------------------------------
        // 敗北時に表示するUIの作成
        //---------------------------------------------------------------------------------
        {
            auto lose_damage_ui = Scene::Object::Create<UIText>();
            lose_damage_ui->SetStatus(Object::StatusBit::NoDraw, true);                         //初期状態では非表示にしておく
            lose_damage_ui->SetTranslate(float3(WINDOW_W / 2, WINDOW_H / 2 + 100.0f, 0.0f));    //位置を中央あたりに設定
            lose_damage_ui->SetFontSize(20);                                                    //サイズを設定
            lose_damage_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));               //文字色設定
            lose_damage_ui->SetFontName("游明朝");                                              //フォントを設定
            float lose_damage_hide_timer = 0.0f;
            //---------------------------------------------------------------------------------
            // 非表示処理の登録
            //---------------------------------------------------------------------------------
            auto hide_proc = [lose_damage_ui, &lose_damage_hide_timer]() {
                if(!lose_damage_ui->GetStatus(Object::StatusBit::NoDraw)) {
                    lose_damage_hide_timer += GetDeltaTime();
                    if(lose_damage_hide_timer >= 3.0f) {
                        lose_damage_ui->SetStatus(Object::StatusBit::NoDraw, true);
                        lose_damage_hide_timer = 0.0f;
                    }
                }
            };
            lose_damage_ui->SetProc("hide_lose_damage_ui_proc", hide_proc, ProcTiming::Update, ProcPriority::NORMAL);
            //---------------------------------------------------------------------------------
            // 敗北時のイベントを登録
            //---------------------------------------------------------------------------------
            auto lose_event = [lose_damage_ui, center_message_ui, &center_message_hide_timer, &lose_damage_hide_timer](const LoseEvent& e) {
                lose_damage_ui->SetStatus(Object::StatusBit::NoDraw, false);
                lose_damage_ui->SetText(std::format("\n被ダメージ:{}", e.agent_damage_amount_));
                center_message_ui->SetStatus(Object::StatusBit::NoDraw, false);
                center_message_ui->SetText("敗北");
                center_message_hide_timer = 0.0f;
                lose_damage_hide_timer    = 0.0f;
            };
            auto lose_event_handle = event_bus->subscribe<LoseEvent>(lose_event, 0);
            event_handles_.push_back(std::move(lose_event_handle));
        }
        //---------------------------------------------------------------------------------
        // 勝利時のイベントを登録
        //---------------------------------------------------------------------------------
        auto win_event = [center_message_ui, &center_message_hide_timer](const WinEvent& e) {
            center_message_ui->SetStatus(Object::StatusBit::NoDraw, false);
            center_message_ui->SetText("勝利");
            center_message_hide_timer = 0.0f;
        };
        auto win_event_handle = event_bus->subscribe<WinEvent>(win_event, 0);
        event_handles_.push_back(std::move(win_event_handle));
    }
    //---------------------------------------------------------------------------------
    // ヘルプボタンの作成
    //---------------------------------------------------------------------------------
    {
        auto help_button = Scene::Object::Create<UIButton>();
        help_button->SetTranslate(float3(1200.0f, 180.0f, 0.0f));    //位置を画面左上あたりに設定
        help_button->SetImage(ImageBuffer::GetImageHandle("help_icon"));
        help_button->SetScaleAxisXYZ(0.3f);    //大きさ
        help_button->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
        //購入ボタンオープンでヘルプボタンを非表示にする処理登録
        auto help_button_hide_proc = [help_button](const PiecePurchaseOpenClickEvent& e) {
            if(e.is_open_) {
                help_button->SetStatus(Object::StatusBit::NoDraw, true);
            }
            else {
                help_button->SetStatus(Object::StatusBit::NoDraw, false);
            }
        };
        auto event_handle = event_bus->subscribe<PiecePurchaseOpenClickEvent>(help_button_hide_proc, 0);
        event_handles_.push_back(std::move(event_handle));
    }

    //---------------------------------------------------------------------------------
    // チュートリアル開始処理
    //---------------------------------------------------------------------------------
    tutorial_step_ = TutorialStep::PurchaseOpen;    //明示的に購入画面オープンから開始
    PurchaseOpenTutorialEnter();
    //---------------------------------------------------------------------------------
    //  フェードフィルター
    //---------------------------------------------------------------------------------
    Scene::Object::Create<Object>()              // フェード用オブジェクト
        ->SetName("FadeIn")                      // 名前設定
        ->AddComponent<ComponentFilterFade>()    // フェードコンポーネント
        ->StartFadeIn();                         // フェードインスタート
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void InGameScene::Update()
{
    __super::Update();
    // delta_time を更新
    float delta_time = phase_timer_.Tick();    // 前回からの経過時間（秒）
    //チュートリアル中
    if(tutorial_active_) {
        UpdateTutorial();
    }
    else {
        // 状態経過時間に加算
        state_timer_ += delta_time;
    }
    switch(game_state_) {
    case GameState::Setup:
        //----------------------------------------------------------------------
        // セットアップBGMを再生
        //----------------------------------------------------------------------
        UpdateSetupPhase();
        if(state_timer_ >= SETUP_PHASE_DURATION) {
            TransitionTo(GameState::Battle);
            auto sound_manager = SoundManager::instance();
            sound_manager->StopBGM("setup");    //BGMを停止する
            //駒数が上限を超えている場合、ピーススタンドに戻す、もしもピーススタンドが満タンなら強制的に破棄する。
            for(auto& agent : Scene::Object::GetArray<Agent>()) {
                agent->EnforcePieceLimit();    //駒数制限を強制適用
            }
            //---------------------------------------------------------------------------------
            // ボードと、ボードに配置されているピースの更新をoffにする
            //---------------------------------------------------------------------------------
            if(auto chess_board = Scene::Object::Get<ChessBoard>()) {
                chess_board->SetBoardProcessEnable(false);
            }
            CreatePiecesForBattlePhase();    //バトルフェーズ用に駒を生成する
        }
        break;

    case GameState::Battle:
        UpdateBattlePhase();    //バトルフェーズの更新処理
        if(state_timer_ >= BATTLE_PHASE_DURATION) {
            TransitionTo(GameState::Setup);
            auto sound_manager = SoundManager::instance();
            sound_manager->StopBGM("battle");    //BGMを停止する
            DestroyPiecesAfterBattlePhase();     //バトルフェーズ用に生成した駒を破棄する
            //---------------------------------------------------------------------------------
            //  各エージェントに処理
            //---------------------------------------------------------------------------------
            for(auto& agent : Scene::Object::GetArray<Agent>()) {
                //---------------------------------------------------------------------------------
                //  このタイミングで無料リロール
                //---------------------------------------------------------------------------------
                //ショップがロックしてなければ
                if(!agent->IsShopLocked()) {
                    agent->RerollShopPieces();    //ショップのピースをリロールする
                }
                //---------------------------------------------------------------------------------
                // 経験値を与える(1)
                //---------------------------------------------------------------------------------
                agent->AddExp(1);
            }
            //---------------------------------------------------------------------------------
            // ボードと、ボードに配置されているピースの更新をonにする
            //---------------------------------------------------------------------------------
            if(auto chess_board = Scene::Object::Get<ChessBoard>()) {
                chess_board->SetBoardProcessEnable(true);
            }
            ++turn_count_;
            //--------------------------------------------------------------------------------
            // NPCが盤面を強化する処理
            //--------------------------------------------------------------------------------
            for(auto& npc : Scene::Object::GetArray<Npc>()) {
                if(!npc->IsDead()) {
                    npc->OnTurnStart();
                }
            }
            has_battle_ended_ = false;    //バトル終了フラグをリセット
        }
        break;
    }
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void InGameScene::Draw()
{
    __super::Draw();
    //---------------------------------------------------------------------------------
    // チュートリアル描画
    //---------------------------------------------------------------------------------
    DrawTutorial();
    //---------------------------------------------------------------------------------
    //バトルフェーズはバトル用の描画処理を行う
    //---------------------------------------------------------------------------------
    if(game_state_ == GameState::Battle) {
        //ボードを描画
        for(int f = 0; f < 8; f++) {
            for(int r = 0; r < 8; r++) {
                int color = GetColor(0, 0, 0);
                //ファイルとランクの合計値が偶数なら白に
                if(((f + r) % 2) == 0) {
                    color = GetColor(255, 255, 255);
                }
                float  x  = (r * SQUARE_SIZE) - 4 * (SQUARE_SIZE);
                float  z  = (f * SQUARE_SIZE) - (4 * SQUARE_SIZE);
                float3 p1 = float3(x + -SQUARE_HALF, -0.1f, z + -SQUARE_HALF);
                float3 p2 = float3(x + SQUARE_HALF, 0.1f, z + SQUARE_HALF);
                DrawCube3D(cast(p1), cast(p2), color, color, TRUE);
            }
        }
    }
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void InGameScene::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void InGameScene::GUI()
{
    __super::GUI();
}

//----------------------------------------------------------------------
//! フェーズ遷移処理
//----------------------------------------------------------------------
void InGameScene::TransitionTo(GameState state)
{
    game_state_  = state;
    state_timer_ = 0.0f;    //状態経過時間をリセット
}
//----------------------------------------------------------------------
// バトルフェーズ開始時に、駒を生成する関数
//----------------------------------------------------------------------
void InGameScene::CreatePiecesForBattlePhase()
{
    //----------------------------------------------------------------------
    //まずはプレイヤーの駒を生成
    //----------------------------------------------------------------------
    if(auto player = Scene::Object::Get<Player>()) {
        //ボードの位置に駒を生成
        for(int f = 0; f < 4; f++) {
            for(int r = 0; r < 8; r++) {
                auto piece_info = player->GetBoardInfo(f, r);
                //駒情報が有効なら
                std::string type_name = piece_info.GetTypeName();
                if(type_name != "") {
                    //駒を生成
                    auto piece = PieceFactory::CreatePiece(type_name, piece_info.GetLevel());
                    //駒の位置を設定(左手前から)
                    float x_pos = (r * SQUARE_SIZE) - 4 * (SQUARE_SIZE);
                    float z_pos = (f * SQUARE_SIZE) - (4 * SQUARE_SIZE);
                    piece->SetTranslate(float3(x_pos, 0.5f, z_pos));
                    piece->SetOwner(player);    //オーナーを設定
                    //---------------------------------------------------------------------------------
                    // 敵を探索するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceSensor>();
                    //---------------------------------------------------------------------------------
                    // 移動を制御するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceMover>();
                    //---------------------------------------------------------------------------------
                    // 攻撃するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceAttacker>();
                    //---------------------------------------------------------------------------------
                    // HP表示するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceUIDisplayer>();
                    //---------------------------------------------------------------------------------
                    // スキルを使用するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceSkillUser>();
                    //---------------------------------------------------------------------------------
                    // シナジーを付与
                    //---------------------------------------------------------------------------------
                    for(auto& active_synergy : player->GetActiveSynergy()) {
                        int synergy_count = active_synergy.GetSynergyCount();    //シナジーのカウントを取得
                        int synergy_level = synergy_count / 2;                   //シナジーレベルを計算(2つでレベル1、4つでレベル2、6つでレベル3)
                        //レベルは3まで
                        if(synergy_level > 3) {
                            synergy_level = 3;
                        }
                        //レベルが1以上なら付与
                        if(synergy_level >= 1) {
                            auto instance = SynergyModifierData::instance();
                            auto mod_data = instance->GetModifierData(active_synergy.GetID());
                            piece->AddComponent<ModifierStatus>(mod_data.at(synergy_level), 100.0f);    //応急処置で第二引数に大きい数を入れておく
                        }
                    }
                    //---------------------------------------------------------------------------------
                    // コリジョンを入れる
                    //---------------------------------------------------------------------------------
                    {
                        auto col_comp = piece->AddComponent<ComponentCollisionCapsule>();
                        col_comp->SetName("PieceCollision");
                        col_comp->SetRadius(0.3f);
                        col_comp->SetHeight(1.0f);
                        col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::ETC);
                        //col_comp->SetOverlapCollisionGroup(static_cast<u32>(ComponentCollision::CollisionGroup::ETC));
                    }
                }
            }
        }
    }
    //----------------------------------------------------------------------
    //次にNPCの駒を生成
    //----------------------------------------------------------------------
    //生きているNPCの中からランダムに1体取得
    auto npcs = Scene::Object::GetArray<Npc>();

    //メルセンヌ・ツイスタ法の乱数生成器を初期化
    std::random_device rd;
    std::mt19937       mt(rd());
    std::shuffle(npcs.begin(), npcs.end(), mt);
    //----------------------------------------------------------------------
    // 先頭を保存
    //----------------------------------------------------------------------
    std::shared_ptr<Npc> first_npc = npcs[0];
    //ここでマッチング情報を作成
    match_infos_.clear();    //マッチ情報をクリア
    // 生きているNPCを格納するベクター、2体でマッチングする場合などに備えて
    std::vector<std::shared_ptr<Npc>> alive_npcs;
    //----------------------------------------------------------------------
    // NPCを走査して、死亡していなければ追加
    //----------------------------------------------------------------------
    for(auto& npc : npcs) {
        //死亡しているならスキップ
        if(npc->IsDead()) {
            continue;
        }
        //先頭と同じならスキップ
        if(npc == first_npc) {
            continue;
        }
        // 追加
        alive_npcs.push_back(npc);
        // 2体集まったらマッチ情報に登録
        if(alive_npcs.size() >= 2) {
            //----------------------------------------------------------------------
            //10.0f~BATTLE_PHASE_DURATIONの間でランダムにバトル時間を決定
            //----------------------------------------------------------------------
            std::uniform_real_distribution<float> dist(10.0f, BATTLE_PHASE_DURATION);
            float                                 duration = dist(mt);
            //----------------------------------------------------------------------
            // マッチ情報を作成して登録
            //----------------------------------------------------------------------
            MatchInfo match_info(alive_npcs[0], alive_npcs[1], duration, false);
            match_infos_.push_back(match_info);
            // ベクターをクリアして次のマッチングに備える
            alive_npcs.clear();
        }
    }
    //----------------------------------------------------------------------
    // 抜けた際に、1体だけ残っている場合、ghostを相手にマッチングを組む
    //----------------------------------------------------------------------
    if(alive_npcs.size() == 1) {
        //alive_npcsから、自分以外をランダムで選ぶ
        std::shared_ptr<Npc> ghost_npc = nullptr;
        //ランダムに選ぶ
        while(!ghost_npc) {
            std::uniform_int_distribution<int> dist(0, npcs.size() - 1);
            int                                index = dist(mt);
            //選んだNPCが自分自身でなければ採用
            if(alive_npcs[0] != npcs[index]) {
                ghost_npc = npcs[index];
            }
            //もしも選んだNPCが自分自身なら、再度ランダムに選び直す
        }
        //----------------------------------------------------------------------
        //10.0f~BATTLE_PHASE_DURATIONの間でランダムにバトル時間を決定
        //----------------------------------------------------------------------
        std::uniform_real_distribution<float> dist(10.0f, BATTLE_PHASE_DURATION);
        float                                 duration = dist(mt);
        //----------------------------------------------------------------------
        // マッチ情報を作成して登録
        //----------------------------------------------------------------------
        MatchInfo match_info(alive_npcs[0], ghost_npc, duration, true);
        match_infos_.push_back(match_info);
    }

    //最初の1体を取得
    if(auto npc = first_npc) {
        battle_agent_ = npc;    //このタイミングでバトルエージェントとして設定
        //ボードの位置に駒を生成
        for(int f = 0; f < 4; f++) {
            for(int r = 0; r < 8; r++) {
                auto piece_info = npc->GetBoardInfo(f, r);
                //駒情報が有効なら
                std::string type_name = piece_info.GetTypeName();
                if(type_name != "") {
                    //駒を生成
                    auto piece = PieceFactory::CreatePiece(type_name, piece_info.GetLevel());
                    //駒の位置を設定(右奥から)
                    float x_pos = (r * SQUARE_SIZE) - 4 * (SQUARE_SIZE);
                    float z_pos = ((7 - f) * SQUARE_SIZE) - (4 * SQUARE_SIZE);
                    piece->SetTranslate(float3(x_pos, 0.5f, z_pos));
                    piece->SetOwner(npc);    //オーナーを設定
                    //---------------------------------------------------------------------------------
                    // 敵を探索するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceSensor>();
                    //---------------------------------------------------------------------------------
                    // 移動を制御するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceMover>();
                    //---------------------------------------------------------------------------------
                    // 攻撃するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceAttacker>();
                    //---------------------------------------------------------------------------------
                    // HP表示するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceUIDisplayer>()->SetHPBarColor(GetColor(255, 0, 0));
                    //---------------------------------------------------------------------------------
                    // スキルを使用するコンポーネントを追加
                    //---------------------------------------------------------------------------------
                    piece->AddComponent<PieceSkillUser>();
                    //---------------------------------------------------------------------------------
                    // コリジョンを入れる
                    //---------------------------------------------------------------------------------
                    {
                        auto col_comp = piece->AddComponent<ComponentCollisionCapsule>();
                        col_comp->SetName("PieceCollision");
                        col_comp->SetRadius(0.3f);
                        col_comp->SetHeight(1.0f);
                        col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::ETC);
                        //col_comp->SetOverlapCollisionGroup(static_cast<u32>(ComponentCollision::CollisionGroup::ETC));
                    }
                }
            }
        }
    }
}
//----------------------------------------------------------------------
// バトルフェーズ終了時に、駒を破棄する関数
//----------------------------------------------------------------------
void InGameScene::DestroyPiecesAfterBattlePhase()
{
    //----------------------------------------------------------------------
    // プレイヤーが敗北していたら、リザルトシーンへ遷移
    //----------------------------------------------------------------------
    if(auto player = Scene::Object::Get<Player>()) {
        if(player->IsDead()) {
            //NPCの生き残り数をカウント
            int alive_npc_count = 0;
            for(auto& npc : Scene::Object::GetArray<Npc>()) {
                if(!npc->IsDead()) {
                    ++alive_npc_count;
                }
            }
            GameRepository::instance()->SetPlayerRank(alive_npc_count + 1);    //プレイヤーのランクは生き残りNPC数+1位
            Scene::Change(Scene::GetScene<ResultScene>());                     //シーンの変更を行う処理
        }
    }

    //----------------------------------------------------------------------
    // 生きているNPCが0体なら、リザルトシーンへ遷移
    //----------------------------------------------------------------------
    auto npcs = Scene::Object::GetArray<Npc>();
    if(npcs.empty()) {
        GameRepository::instance()->SetPlayerRank(1);     //プレイヤーのランクは1位
        Scene::Change(Scene::GetScene<ResultScene>());    //シーンの変更を行う処理
    }

    //----------------------------------------------------------------------
    //ショップの駒をマスク
    //----------------------------------------------------------------------
    if(auto shop_stand = Scene::Object::Get<ShopStand>()) {
        auto shop_pieces = shop_stand->GetShopPieces();
        //マスク
        for(auto& weak_piece : shop_pieces) {
            if(auto piece = weak_piece.lock()) {
                piece->SetStatus(Object::StatusBit::NoUpdate, true);    //更新不可に設定
                piece->SetStatus(Object::StatusBit::NoDraw, true);      //描画不可に設定
            }
        }
    }
    //----------------------------------------------------------------------
    // スタンドの駒をマスク
    //----------------------------------------------------------------------
    if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
        auto stand_square = piece_stand->GetSquarePtrArray();
        //マスク
        for(auto& weak_stand : stand_square) {
            if(auto square = weak_stand.lock()) {
                auto piece_weak = square->GetPutPiece();
                if(auto piece = piece_weak.lock()) {
                    piece->SetStatus(Object::StatusBit::NoUpdate, true);    //更新不可に設定
                    piece->SetStatus(Object::StatusBit::NoDraw, true);      //描画不可に設定
                }
            }
        }
    }
    //----------------------------------------------------------------------
    //シーン内の、update可能な駒オブジェクトをすべて破棄する
    //----------------------------------------------------------------------
    for(auto& piece : Scene::Object::GetArray<Piece>()) {
        if(!piece->GetStatus(Object::StatusBit::NoUpdate)) {
            Scene::Object::Release(piece);
        }
    }
    //----------------------------------------------------------------------
    //ショップの駒を戻す
    //----------------------------------------------------------------------
    if(auto shop_stand = Scene::Object::Get<ShopStand>()) {
        auto shop_pieces = shop_stand->GetShopPieces();
        //戻す
        for(auto& weak_piece : shop_pieces) {
            if(auto piece = weak_piece.lock()) {
                piece->SetStatus(Object::StatusBit::NoUpdate, false);
                piece->SetStatus(Object::StatusBit::NoDraw, false);
            }
        }
    }
    //----------------------------------------------------------------------
    // スタンドの駒を戻す
    //----------------------------------------------------------------------
    if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
        auto stand_square = piece_stand->GetSquarePtrArray();
        //戻す
        for(auto& weak_stand : stand_square) {
            if(auto square = weak_stand.lock()) {
                auto piece_weak = square->GetPutPiece();
                if(auto piece = piece_weak.lock()) {
                    piece->SetStatus(Object::StatusBit::NoUpdate, false);
                    piece->SetStatus(Object::StatusBit::NoDraw, false);
                }
            }
        }
    }

    //----------------------------------------------------------------------
    // 存在するスキルオブジェクトをすべて破棄する
    //----------------------------------------------------------------------
    for(auto& skill_obj : Scene::Object::GetArray<SkillObjectBase>()) {
        Scene::Object::Release(skill_obj);
    }
}

//----------------------------------------------------------------------
//! @brief セットアップの処理
//----------------------------------------------------------------------
void InGameScene::UpdateSetupPhase()
{
    auto sound_manager = SoundManager::instance();
    if(!sound_manager->IsPlayingBGM("setup")) {
        sound_manager->PlayBGM("setup");
    }
}

//----------------------------------------------------------------------
//! @brief バトルフェーズの処理
//----------------------------------------------------------------------
void InGameScene::UpdateBattlePhase()
{
    //----------------------------------------------------------------------
    // バトルBGMを再生
    //----------------------------------------------------------------------
    auto sound_manager = SoundManager::instance();
    if(!sound_manager->IsPlayingBGM("battle")) {
        sound_manager->PlayBGM("battle");
    }

    //----------------------------------------------------------------------
    // NPC同士のバトルをシミュレートする
    //----------------------------------------------------------------------
    SimulateNpcBattle();

    //バトルが終了していない場合、終了判定を行う
    if(!has_battle_ended_) {
        //エージェントの駒数を取得
        int player_alive_piece_count = GetAlivePieceCountForAgent(Scene::Object::Get<Player>());
        int npc_alive_piece_count    = 0;
        if(auto battle_npc = battle_agent_.lock()) {
            npc_alive_piece_count = GetAlivePieceCountForAgent(battle_npc);
        }
        //どちらかの駒数が0になったらバトル終了
        if(player_alive_piece_count == 0 || npc_alive_piece_count == 0) {
            //----------------------------------------------------------------------
            // 存在するスキルオブジェクトをすべて破棄する
            //----------------------------------------------------------------------
            for(auto& skill_obj : Scene::Object::GetArray<SkillObjectBase>()) {
                Scene::Object::Release(skill_obj);
            }

            has_battle_ended_      = true;                            //バトル終了フラグを立てる
            bool is_player_victory = (npc_alive_piece_count == 0);    //プレイヤーの勝利判定
            //ダメージ処理
            if(auto battle_npc = battle_agent_.lock()) {
                if(is_player_victory) {
                    //----------------------------------------------------------------------
                    // 勝利SEを再生
                    //----------------------------------------------------------------------
                    if(!sound_manager->IsPlayingSE("win")) {
                        sound_manager->PlaySE("win");
                    }

                    //----------------------------------------------------------------------
                    //NPCの駒が全滅したら、NPCがダメージを受ける
                    //----------------------------------------------------------------------
                    if(auto battle_npc = battle_agent_.lock()) {
                        int damage = player_alive_piece_count * 2;
                        battle_npc->ApplyDamage(damage);
                        //HPの数値UIを更新
                        if(auto hp_ui = Scene::Object::Get<UIText>(std::string(battle_npc->GetName()) + "HPText")) {
                            int hp = battle_npc->GetHP();
                            if(hp > 0) {
                                //HPが0以下でなければ表示
                                hp_ui->SetText(std::to_string(hp));
                            }
                            else {
                                //0以下なら、敗北と表示
                                hp_ui->SetFontName("游明朝");
                                hp_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));
                                hp_ui->SetText("敗北");
                                //NPCをシーンから削除
                                Scene::Object::Release(battle_npc);
                            }
                            // HPが減るほど白(255,255,255)から赤(255,0,0)へ変化
                            float hp_ratio         = static_cast<float>(battle_npc->GetHP()) / static_cast<float>(MAX_AGENT_HP);
                            int   green_blue_value = static_cast<int>(255.0f * hp_ratio);
                            hp_ui->SetColor(GetColor(255, green_blue_value, green_blue_value), GetColor(0, 0, 0));
                        }
                    }
                    auto event_bus = di_container_.resolve<TsukinoEventBus::EventBus>();
                    event_bus->publish(WinEvent());
                }
                else if(player_alive_piece_count == 0) {
                    //----------------------------------------------------------------------
                    // 敗北SEを再生
                    //----------------------------------------------------------------------
                    if(!sound_manager->IsPlayingSE("lose")) {
                        sound_manager->PlaySE("lose");
                    }

                    //----------------------------------------------------------------------
                    //プレイヤーの駒が全滅したら、NPCの残り駒数分ダメージを受ける
                    //----------------------------------------------------------------------
                    int damage = npc_alive_piece_count * 2;
                    if(auto player = Scene::Object::Get<Player>()) {
                        player->ApplyDamage(damage);
                        //HPの数値UIを更新
                        if(auto hp_ui = Scene::Object::Get<UIText>(std::string(player->GetName()) + "HPText")) {
                            int hp = player->GetHP();
                            if(hp > 0) {
                                //HPが0以下でなければ表示
                                hp_ui->SetText(std::to_string(hp));
                            }
                            else {
                                //0以下なら、敗北と表示
                                hp_ui->SetFontName("游明朝");
                                hp_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));
                                hp_ui->SetText("敗北");
                            }
                            // HPが減るほど白(255,255,255)から赤(255,0,0)へ変化
                            float hp_ratio         = static_cast<float>(player->GetHP()) / static_cast<float>(MAX_AGENT_HP);
                            int   green_blue_value = static_cast<int>(255.0f * hp_ratio);
                            hp_ui->SetColor(GetColor(255, green_blue_value, green_blue_value), GetColor(0, 0, 0));
                        }
                    }
                    auto event_bus = di_container_.resolve<TsukinoEventBus::EventBus>();
                    event_bus->publish(LoseEvent(damage));
                }
                //----------------------------------------------------------------------
                // 内部的な勝ち負けを更新してゴールドを付与
                //----------------------------------------------------------------------
                //プレイヤー
                if(auto player = Scene::Object::Get<Player>()) {
                    player->UpdateResult(is_player_victory);
                    int goldain = CalculateRoundGold(player, is_player_victory);
                    player->AddGold(goldain);    //ゴールドを5増やす
                }
                //NPC
                if(auto battle_npc = battle_agent_.lock()) {
                    battle_npc->UpdateResult(!is_player_victory);
                    int goldain = CalculateRoundGold(battle_npc, !is_player_victory);
                    battle_npc->AddGold(goldain);    //ゴールドを5増やす
                }
                //----------------------------------------------------------------------
                // 移動ストラテジを勝利へ
                //----------------------------------------------------------------------
                for(auto& piece : Scene::Object::GetArray<Piece>()) {
                    if(auto piece_mover = piece->GetComponent<PieceMover>()) {
                        piece_mover->SetMoveStrategy(std::make_unique<MoveWinStrategy>());
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------
//! @brief NPC同士のバトルをシミュレートする関数
//----------------------------------------------------------------------
void InGameScene::SimulateNpcBattle()
{
    //マッチ情報を走査
    for(auto& match_info : match_infos_) {
        //既に終了しているマッチングならスキップ
        if(match_info.is_judged_) {
            continue;
        }
        auto agent1 = match_info.agent1_.lock();
        auto agent2 = match_info.agent2_.lock();
        //ポインタチェック、なければスキップ
        if(!agent1 || !agent2) {
            continue;
        }
        //経過時間が設定されたバトル時間を超えたらマッチング終了
        if(state_timer_ >= match_info.battle_duration_) {
            match_info.is_judged_ = true;    //終了フラグを立てる
            // エージェントの駒数を取得
            int agent1_piece_count = agent1->GetPlacedPieceNum();
            int agent2_piece_count = agent2->GetPlacedPieceNum();
            //戦力から勝敗を確率的に決定
            int total_pieces = agent1_piece_count + agent2_piece_count;
            if(total_pieces == 0) {
                //両者とも駒がいない場合は引き分け
                {
                    agent1->UpdateResult(false);    //引き分けは敗北を与える
                    int goldain = CalculateRoundGold(agent1, false);
                    agent1->AddGold(goldain);    //ゴールドを増やす
                }
                {
                    agent2->UpdateResult(false);    //引き分けは敗北を与える
                    int goldain = CalculateRoundGold(agent2, false);
                    agent2->AddGold(goldain);    //ゴールドを増やす
                }
                continue;
            }
            //エージェント1が勝つ確率
            float agent1_win_probability = static_cast<float>(agent1_piece_count) / static_cast<float>(total_pieces);
            //メルセンヌ・ツイスタ法の乱数生成器を初期化
            std::random_device                    rd;
            std::mt19937                          mt(rd());
            std::uniform_real_distribution<float> dist(0.0f, 1.0f);
            float                                 random_value   = dist(mt);
            bool                                  agent1_victory = (random_value < agent1_win_probability);
            //------------------------------------------------------
            //エージェント1の勝敗更新
            //------------------------------------------------------
            agent1->UpdateResult(agent1_victory);
            int goldain1 = CalculateRoundGold(agent1, agent1_victory);
            agent1->AddGold(goldain1);    //ゴールドを増やす

            //------------------------------------------------------
            // ゴースト戦でなければエージェント2にダメージを与える
            //------------------------------------------------------
            if(!match_info.is_ghost_2_ && agent1_victory) {
                int damage = agent1_piece_count * 2;    //ダメージ概算
                agent2->ApplyDamage(damage);
                //HPの数値UIを更新
                if(auto hp_ui = Scene::Object::Get<UIText>(std::string(agent2->GetName()) + "HPText")) {
                    int hp = agent2->GetHP();
                    if(hp > 0) {
                        //HPが0以下でなければ表示
                        hp_ui->SetText(std::to_string(hp));
                    }
                    else {
                        //0以下なら、敗北と表示
                        hp_ui->SetFontName("游明朝");
                        hp_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));
                        hp_ui->SetText("敗北");
                        //NPCをシーンから削除
                        Scene::Object::Release(agent2);
                    }
                    // HPが減るほど白(255,255,255)から赤(255,0,0)へ変化
                    float hp_ratio         = static_cast<float>(agent2->GetHP()) / static_cast<float>(MAX_AGENT_HP);
                    int   green_blue_value = static_cast<int>(255.0f * hp_ratio);
                    hp_ui->SetColor(GetColor(255, green_blue_value, green_blue_value), GetColor(0, 0, 0));
                }
            }
            //------------------------------------------------------
            //ゴースト戦でなければエージェント2の勝敗更新
            //------------------------------------------------------
            if(!match_info.is_ghost_2_) {
                agent2->UpdateResult(!agent1_victory);
                int goldain2 = CalculateRoundGold(agent2, !agent1_victory);
                agent2->AddGold(goldain2);    //ゴールドを増やす
            }
            //------------------------------------------------------
            // エージェント1が負けたならダメージを与える
            //------------------------------------------------------
            if(!agent1_victory) {
                int damage = agent2_piece_count * 2;    //ダメージ概算
                agent1->ApplyDamage(damage);
                //HPの数値UIを更新
                if(auto hp_ui = Scene::Object::Get<UIText>(std::string(agent1->GetName()) + "HPText")) {
                    int hp = agent1->GetHP();
                    if(hp > 0) {
                        //HPが0以下でなければ表示
                        hp_ui->SetText(std::to_string(hp));
                    }
                    else {
                        //0以下なら、敗北と表示
                        hp_ui->SetFontName("游明朝");
                        hp_ui->SetColor(GetColor(255, 255, 255), GetColor(0, 0, 0));
                        hp_ui->SetText("敗北");
                        //NPCをシーンから削除
                        Scene::Object::Release(agent1);
                    }
                    // HPが減るほど白(255,255,255)から赤(255,0,0)へ変化
                    float hp_ratio         = static_cast<float>(agent1->GetHP()) / static_cast<float>(MAX_AGENT_HP);
                    int   green_blue_value = static_cast<int>(255.0f * hp_ratio);
                    hp_ui->SetColor(GetColor(255, green_blue_value, green_blue_value), GetColor(0, 0, 0));
                }
            }
        }
    }
}

//----------------------------------------------------------------------
//! @brief ピース購入オープンボタン催促用UIを生成する関数
//----------------------------------------------------------------------
void InGameScene::CreatePurchaseOpenTutorialUI()
{
    //---------------------------------------------------------------------------------
    // ピース購入オープンボタン催促用UI
    //---------------------------------------------------------------------------------
    {
        auto purchase_open_button_prompt_ui = Scene::Object::Create<UIImage>("purchase_open_button_prompt_ui");
        purchase_open_button_prompt_ui->SetImage(ImageBuffer::GetImageHandle("cursor"));
        float x_base_pos = 1100.0f;
        float y_base_pos = 650.0f;
        purchase_open_button_prompt_ui->SetTranslate(float3(x_base_pos, y_base_pos, 0.0f));    //右下に配置
        purchase_open_button_prompt_ui->SetScaleAxisXYZ(0.4f);                                 //小さくする
        auto update_proc = [purchase_open_button_prompt_ui, x_base_pos, y_base_pos]() {
            static float sin_rot = 90.0f;
            //左下方向にサインカーブで前後させる
            sin_rot        += 4.0f;
            float x_offset  = std::sin(D2R(sin_rot)) * 10.0f;
            float y_offset  = std::sin(D2R(sin_rot)) * 10.0f;
            purchase_open_button_prompt_ui->SetTranslate(float3(x_base_pos - x_offset, y_base_pos + y_offset, 0.0f));
        };
        purchase_open_button_prompt_ui->SetProc("piece_button_prompt_update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
    }
}

//------------------------------------------------------
//! @brief エージェントの保有している生きたバトルフェーズ中の駒の数を取得する関数
//------------------------------------------------------
int InGameScene::GetAlivePieceCountForAgent(const std::shared_ptr<Agent>& agent)
{
    int count = 0;
    for(auto piece : Scene::Object::GetArray<Piece>()) {
        if(!piece->GetComponent<PieceSensor>()) {
            continue;    //センサーコンポーネントが無い駒はスキップ
        }
        if(piece->GetOwner() == agent) {
            //オーナーが一致したらカウントアップ
            count++;
        }
    }
    return count;
}

//----------------------------------------------------------------------
//! @brief チュートリアルの更新処理関数
//----------------------------------------------------------------------
void InGameScene::UpdateTutorial()
{
    switch(tutorial_step_) {
    case TutorialStep::None:
        //何もしない
        break;
    case TutorialStep::PurchaseOpen:
        PurchaseOpenTutorialUpdate();
        break;
    case TutorialStep::PurchasePiece:
        PurchasePieceTutorialUpdate();
        break;
    case TutorialStep::PurchaseClose:
        PurchaseCloseTutorialUpdate();
        break;
    case TutorialStep::PutPiece:
        PutPieceTutorialUpdate();
        break;
    }
}

//----------------------------------------------------------------------
//! @brief チュートリアルの描画処理関数
//----------------------------------------------------------------------
void InGameScene::DrawTutorial()
{
    switch(tutorial_step_) {
    case TutorialStep::PutPiece:
        PutPieceTutorialDraw();
        break;
    }
}

//----------------------------------------------------------------------
//! @brief 購入オープンチュートリアルの開始処理関数
//----------------------------------------------------------------------
void InGameScene::PurchaseOpenTutorialEnter()
{
    //---------------------------------------------------------------------------------
    // ピース購入オープンボタン催促用UI
    //---------------------------------------------------------------------------------
    CreatePurchaseOpenTutorialUI();
}

//----------------------------------------------------------------------
//! @brief 購入オープンチュートリアルの更新処理関数
//----------------------------------------------------------------------
void InGameScene::PurchaseOpenTutorialUpdate()
{
    //----------------------------------------------------------------------
    // 切り替え処理
    //----------------------------------------------------------------------
    if(auto piece_purchase_open_button = Scene::Object::Get<PiecePurchaseOpenButton>("PiecePurchaseOpenButton")) {
        //購入ボタンをクリックしたら切り替え
        if(piece_purchase_open_button->IsClick()) {
            PurchaseOpenTutorialExit();
            tutorial_step_ = TutorialStep::PurchasePiece;    //次のステップへ
            PurchasePieceTutorialEnter();                    // ピース購入チュートリアル開始
        }
    }
}

//----------------------------------------------------------------------
//! @brief 購入オープンチュートリアルの終了処理関数
//----------------------------------------------------------------------
void InGameScene::PurchaseOpenTutorialExit()
{
    //----------------------------------------------------------------------
    // 購入促しUIを削除
    //----------------------------------------------------------------------
    if(auto ui = Scene::Object::Get<UIImage>("purchase_open_button_prompt_ui")) {
        Scene::Object::Release(ui);
    }
}

//----------------------------------------------------------------------
//! @brief ピース購入チュートリアルの開始処理関数
//----------------------------------------------------------------------
void InGameScene::PurchasePieceTutorialEnter()
{
    //----------------------------------------------------------------------
    // ピース購入を促すUIを作成
    //----------------------------------------------------------------------
    for(int i = 0; i < 5; i++) {
        auto purchase_piece_prompt_ui = Scene::Object::Create<UIImage>("purchase_piece_prompt_ui_" + std::to_string(i));
        purchase_piece_prompt_ui->SetImage(ImageBuffer::GetImageHandle("cursor"));
        float x_base_pos = 270.0f + (i * 165.0f);    //X位置を設定
        float y_base_pos = 330.0f;
        purchase_piece_prompt_ui->SetTranslate(float3(x_base_pos, y_base_pos, 0.0f));    //右下に配置
        purchase_piece_prompt_ui->SetScaleAxisXYZ(0.2f);                                 //小さくする
        auto update_proc = [purchase_piece_prompt_ui, x_base_pos, y_base_pos]() {
            static float sin_rot = 90.0f;
            //左下方向にサインカーブで前後させる
            sin_rot        += 0.5f;
            float x_offset  = std::sin(D2R(sin_rot)) * 5.0f;
            float y_offset  = std::sin(D2R(sin_rot)) * 5.0f;
            purchase_piece_prompt_ui->SetTranslate(float3(x_base_pos - x_offset, y_base_pos + y_offset, 0.0f));
        };
        purchase_piece_prompt_ui->SetProc("piece_button_prompt_update_proc_" + std::to_string(i), update_proc, ProcTiming::Update, ProcPriority::NORMAL);
    }
}

//----------------------------------------------------------------------
//! @brief ピース購入チュートリアルの更新処理関数
//----------------------------------------------------------------------
void InGameScene::PurchasePieceTutorialUpdate()
{
    //----------------------------------------------------------------------
    // 切り替え処理
    //----------------------------------------------------------------------
    if(auto piece_purchase_open_button = Scene::Object::Get<PiecePurchaseOpenButton>("PiecePurchaseOpenButton")) {
        //購入ボタンをクリックしたら切り替え
        if(piece_purchase_open_button->IsClick()) {
            PurchasePieceTutorialExit();
            tutorial_step_ = TutorialStep::PurchaseOpen;    // 購入チュートリアルに戻る
            PurchaseOpenTutorialEnter();
        }
    }
    if(auto player = Scene::Object::Get<Player>()) {
        // プレイヤーがピースを1体以上所有したら
        if(player->GetOwnedPieceNum() >= 1) {
            PurchasePieceTutorialExit();
            tutorial_step_ = TutorialStep::PurchaseClose;    // 次のステップへ
            PurchaseCloseTutorialEnter();                    // ピース購入画面を閉じるチュートリアル開始
        }
    }
}

//----------------------------------------------------------------------
//! @brief ピース購入チュートリアルの終了処理関数
//----------------------------------------------------------------------
void InGameScene::PurchasePieceTutorialExit()
{
    //購入促しUIを削除
    for(int i = 0; i < 5; i++) {
        if(auto ui = Scene::Object::Get<UIImage>("purchase_piece_prompt_ui_" + std::to_string(i))) {
            Scene::Object::Release(ui);
        }
    }
}

//----------------------------------------------------------------------
//! @brief ピース購入画面を閉じるチュートリアルの開始処理関数
//----------------------------------------------------------------------
void InGameScene::PurchaseCloseTutorialEnter()
{
    //---------------------------------------------------------------------------------
    // ピース購入オープンボタン催促用UI
    //---------------------------------------------------------------------------------
    CreatePurchaseOpenTutorialUI();
}

//----------------------------------------------------------------------
//! @brief ピース購入画面を閉じるチュートリアルの更新処理関数
//----------------------------------------------------------------------
void InGameScene::PurchaseCloseTutorialUpdate()
{
    //----------------------------------------------------------------------
    // 切り替え処理
    //----------------------------------------------------------------------
    if(auto piece_purchase_open_button = Scene::Object::Get<PiecePurchaseOpenButton>("PiecePurchaseOpenButton")) {
        //購入ボタンをクリックしたら切り替え
        if(piece_purchase_open_button->IsClick()) {
            PurchaseCloseTutorialExit();
            tutorial_step_ = TutorialStep::PutPiece;    // ピース配置チュートリアル
            PutPieceTutorialEnter();
        }
    }
}

//----------------------------------------------------------------------
//! @brief ピース購入画面を閉じるチュートリアルの終了処理関数
//----------------------------------------------------------------------
void InGameScene::PurchaseCloseTutorialExit()
{
    //----------------------------------------------------------------------
    // 購入促しUIを削除
    //----------------------------------------------------------------------
    if(auto ui = Scene::Object::Get<UIImage>("purchase_open_button_prompt_ui")) {
        Scene::Object::Release(ui);
    }
}

//----------------------------------------------------------------------
//! @brief ピースを置くチュートリアルの開始処理関数
//----------------------------------------------------------------------
void InGameScene::PutPieceTutorialEnter()
{
}

//----------------------------------------------------------------------
//! @brief ピースを置くチュートリアルの更新処理関数
//----------------------------------------------------------------------
void InGameScene::PutPieceTutorialUpdate()
{
    //----------------------------------------------------------------------
    // 切り替え処理
    //----------------------------------------------------------------------
    if(auto player = Scene::Object::Get<Player>()) {
        // プレイヤーがピースを1体以上配置したら
        if(player->GetPlacedPieceNum() >= 1) {
            PutPieceTutorialExit();
            tutorial_step_   = TutorialStep::None;    // チュートリアル終了
            tutorial_active_ = false;                 // チュートリアル無効化
        }
    }
}

//----------------------------------------------------------------------
// ピースを置くチュートリアルの更新処理関数
//----------------------------------------------------------------------
void InGameScene::PutPieceTutorialDraw()
{
    //----------------------------------------------------------------------
    // ピーススタンドのピースを取得して、盤面のほうへのベジェ曲線を描画
    //----------------------------------------------------------------------
    if(auto piece_stand = Scene::Object::Get<PieceStand>()) {
        auto stand_square = piece_stand->GetSquarePtrArray();
        //ベジェ曲線を描画
        for(auto& weak_stand : stand_square) {
            if(auto square = weak_stand.lock()) {
                auto piece_weak = square->GetPutPiece();
                if(auto piece = piece_weak.lock()) {
                    float3 start_pos = piece->GetTranslate();
                    float3 end_pos   = float3(0.0f, 0.5f, -3.0f);
                    //----------------------------------------------------------------------
                    //方向ベクトルを計算
                    //----------------------------------------------------------------------
                    float3 dir     = end_pos - start_pos;
                    float3 div_vec = (dir / 3);
                    //----------------------------------------------------------------------
                    // 制御点の高さ
                    //----------------------------------------------------------------------
                    float control_y = 4.0f;
                    //----------------------------------------------------------------------
                    //方向を分割して、その真上座標を制御点にする。
                    //----------------------------------------------------------------------
                    float3 control_point2 = start_pos + div_vec;
                    control_point2.y      = control_y;
                    float3 control_point3 = start_pos + (div_vec * 2);
                    control_point3.y      = control_y;
                    //----------------------------------------------------------------------
                    // 制御点の追加
                    //----------------------------------------------------------------------
                    std::vector<float3> control_points;
                    control_points.push_back(start_pos);
                    control_points.push_back(control_point2);
                    control_points.push_back(control_point3);
                    control_points.push_back(end_pos);
                    //----------------------------------------------------------------------
                    //ベジェ曲線を描画
                    //----------------------------------------------------------------------
                    DrawBezierArrow3D(control_points, 16, DxLib::GetColor(255, 255, 0));
                }
            }
        }
    }
}

//----------------------------------------------------------------------
//! @brief ピースを置くチュートリアルの終了処理関数
//----------------------------------------------------------------------
void InGameScene::PutPieceTutorialExit()
{
}
