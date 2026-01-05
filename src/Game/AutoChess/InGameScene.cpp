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
#include <Game/AutoChess/Piece/PieceData/PieceData.h>
#include <Game/AutoChess/Piece/PieceData/LevelData.h>
#include <Game/AutoChess/system/Logic.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>
#include <Game/AutoChess/Funiture/Glass.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Events/SkillClickEvent.h>
#include <Game/AutoChess/system/UIHitManager.h>
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
    auto player = Scene::Object::Create<Player>();          //プレイヤー
    player->SetSynergySystemRepository(&game_context_.GetSynergyRepository(), &game_context_.GetPieceRepository());
    player->SetIsPurchaseOpenFlag(&is_purchase_open_);    //ピース購入画面が開いているかのフラグを設定
    auto event_bus = di_container_.resolve<TsukinoEventBus::EventBus>();
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
        //auto table = Scene::Object::Create<Object>();
        //auto model = table->AddComponent<ComponentModel>("data/AutoChess/Model/Stage/table.mv1");
        //table->SetScaleAxisXYZ(4.0f);
        //table->SetTranslate(float3(-0.5f, -20.0f, 0.0f));
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
            {
                auto curr_exp_ui = Scene::Object::Create<UIText>();
                curr_exp_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleLeft);     //左寄せに設定
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
            }
            //割線
            {
                auto line_ui = Scene::Object::Create<UIText>();
                line_ui->SetTranslate(float3(130.0f, 500.0f, 0.0f));                    //位置を設定
                line_ui->SetFontSize(30);                                               //フォントサイズ設定
                line_ui->SetColor(GetColor(128, 128, 128), GetColor(255, 255, 255));    //文字色設定
                line_ui->SetText("/");                                                  //割線を表示
            }
            //次のレベルまでに必要な経験値
            {
                auto next_exp_ui = Scene::Object::Create<UIText>();
                next_exp_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleLeft);     //左寄せに設定
                next_exp_ui->SetTranslate(float3(170.0f, 500.0f, 0.0f));                    //位置を設定
                next_exp_ui->SetFontSize(30);                                               //フォントサイズ設定
                next_exp_ui->SetColor(GetColor(128, 128, 128), GetColor(255, 255, 255));    //文字色設定
                //更新処理
                auto set_text_proc = [next_exp_ui]() {
                    auto player         = Scene::Object::Get<Player>();
                    int  next_level_exp = player->GetNextLevelExp();    //次のレベルまでに必要な経験値を取得
                    //必要な経験値を表示
                    next_exp_ui->SetText(std::to_string(next_level_exp));
                };
                next_exp_ui->SetProc("set_text", set_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
            }
        }
    }
    //---------------------------------------------------------------------------------
    //  売却ボタン
    //---------------------------------------------------------------------------------
    {
        auto sell_button = Scene::Object::Create<UIButton>();    //売却ボタン
        sell_button->SetImage(ImageBuffer::GetImageHandle("sell_button"));
        sell_button->SetScaleAxisXYZ(0.3f);                          //大きさを少し小さく設定
        sell_button->SetTranslate(float3(1050.0f, 500.0f, 0.0f));    //位置を画面右下あたりに設定
        //左クリックを促す
        sell_button->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
        //クリック時の処理
        auto click_func = [player]() {
            if(auto select_piece = player->GetSelectedPiece()) {
                int sell_piece_price = select_piece->GetPrice();    //売却価格を取得
                Scene::Object::Release(select_piece);               //選択されているオブジェクトを解放する
                player->ReleaseSelectedPiece();                     //選択中のピースをクリアする
                player->AddGold(sell_piece_price);                  //ゴールドを加算する(売却値の分)
            }
        };
        sell_button->SetClickFunc(click_func);
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
        //更新処理
        auto set_text_proc = [this, phase_timer_ui]() {
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
            // エージェントの所持ゴールド表示UI
            //---------------------------------------------------------------------------------
            auto gold_ui = Scene::Object::Create<UIText>();
            gold_ui->SetFontSize(30);                                                       //フォントサイズ設定
            gold_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));                  //文字色設定
            gold_ui->SetTranslate(float3(150.0f, 100.0f + (agent_count * 40.0f), 0.0f));    //位置を左上あたりに設定
            gold_ui->SetAlignment(ComponentTransformUI::Alignment::UpperLeft);              //左上寄せに設定
            //更新処理
            auto set_text_proc = [agent, gold_ui]() {
                gold_ui->SetText("Gold: " + std::to_string(agent->GetGold()));    //所持ゴールドを表示
            };
            gold_ui->SetProc("set_gold", set_text_proc, ProcTiming::Update, ProcPriority::NORMAL);
            agent_ui_objects.push_back(gold_ui);
            //---------------------------------------------------------------------------------
            // エージェントのHP表示UI
            //---------------------------------------------------------------------------------
            auto hp_gauge = Scene::Object::Create<UIGauge>();
            hp_gauge->SetTranslate(float3(300.0f, 100.0f + (agent_count * 40.0f), 0.0f));    //位置を左上あたりに設定
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
            hp_ui->SetTranslate(float3(300.0f, 100.0f + (agent_count * 40.0f), 0.0f));    //位置を左上あたりに設定
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
        auto purchase_window_filter = Scene::Object::Create<UIImage>();    //フィルターの宣言
        purchase_window_filter->SetScaleAxisXYZ(15.0f);                    //大きさを画面全体に設定
        purchase_window_filter->SetAlpha(168);                             //透明度を設定
        float x = WINDOW_W * 0.5f;
        float y = WINDOW_H * 0.5f;
        purchase_window_filter->SetTranslate(float3(x, y, 0.0f));
        purchase_window_objects.push_back(purchase_window_filter);    //購入画面のウィンドウ群に追加
    }
    //---------------------------------------------------------------------------------
    //  ピース購入ボタン
    //---------------------------------------------------------------------------------
    {
        auto shop_pieces = player->GetShopPieces();    //ショップに並んでいるピースを取得
        for(int i = 0; i < shop_pieces.size(); ++i) {
            auto piece_purchase_button = Scene::Object::Create<UIButton>();
            piece_purchase_button->SetName("PiecePurchaseButton");
            float x_pos = 400.0f + (i * 150.0f);    //X位置を設定
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
            piece_name_text->SetName("PieceNameText");
            float x_pos = 400.0f + (i * 150.0f);    //X位置を設定
            piece_name_text->SetTranslate(float3(x_pos, 370.0f, 0.0f));
            piece_name_text->SetFontSize(16);                                         //フォントサイズを設定
            piece_name_text->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));    //文字色を白に設定
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
    //  リロールボタン
    //---------------------------------------------------------------------------------
    {
        auto reroll_button = Scene::Object::Create<UIButton>();
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
        auto piece_purchase_open_button = Scene::Object::Create<PiecePurchaseOpenButton>();
        auto click_func                 = [this, purchase_window_objects]() {
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
        };
        piece_purchase_open_button->SetClickFunc(click_func);    //クリック時の処理を設定
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

    // 状態経過時間に加算
    state_timer_ += delta_time;

    switch(game_state_) {
    case GameState::Setup:
        if(state_timer_ >= SETUP_PHASE_DURATION) {
            TransitionTo(GameState::Battle);
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
            DestroyPiecesAfterBattlePhase();    //バトルフェーズ用に生成した駒を破棄する
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
                npc->OnTurnStart();
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
    //バトルフェーズはバトル用の描画処理を行う
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
    ImageBuffer::Exit();    //画像バッファの終了
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
                    // コリジョンを入れる
                    //---------------------------------------------------------------------------------
                    {
                        auto col_comp = piece->AddComponent<ComponentCollisionCapsule>();
                        col_comp->SetName("PieceCollision");
                        col_comp->SetRadius(0.3f);
                        col_comp->SetHeight(1.0f);
                        col_comp->SetCollisionGroup(ComponentCollision::CollisionGroup::ETC);
                        col_comp->SetOverlapCollisionGroup(static_cast<u32>(ComponentCollision::CollisionGroup::ETC));
                    }
                }
            }
        }
    }
    //----------------------------------------------------------------------
    //次にNPCの駒を生成
    //----------------------------------------------------------------------
    //生きているNPCの中からランダムに1体取得
    //auto npcs = Scene::Object::GetArray<Npc>();
    if(auto npc = Scene::Object::Get<Npc>()) {
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
                        col_comp->SetOverlapCollisionGroup(static_cast<u32>(ComponentCollision::CollisionGroup::ETC));
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
// バトルフェーズの処理
//----------------------------------------------------------------------
void InGameScene::UpdateBattlePhase()
{
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
                }
                else if(player_alive_piece_count == 0) {
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
            }
        }
    }
}

//------------------------------------------------------
// エージェントの保有している生きたバトルフェーズ中の駒の数を取得する関数
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
