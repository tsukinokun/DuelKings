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
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool InGameScene::Init()
{
    __super::Init();
    ImageBuffer::Init();                              //画像バッファの初期化
    PiecePool::Init();                                //駒プールの初期化
    Scene::Object::Create<Camera>();                  //カメラ
    auto player = Scene::Object::Create<Player>();    //プレイヤー
    //---------------------------------------------------------------------------------
    //  NPCの生成
    //---------------------------------------------------------------------------------
    for(int i = 0; i < AGENT_NUM; i++) {
        Scene::Object::Create<Npc>();
    }
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
    //  NPCの生成
    //---------------------------------------------------------------------------------
    for(int i = 0; i < AGENT_NUM - 1; i++) {
        Scene::Object::Create<Npc>();
    }
    Scene::Object::Create<MouseRay>();                               //マウス光線
    std::vector<std::shared_ptr<Object>> purchase_window_objects;    //購入画面のウィンドウ群
    //---------------------------------------------------------------------------------
    //  ピース購入ボタン
    //---------------------------------------------------------------------------------
    {
        auto shop_pieces = player->GetShopPieces();    //ショップに並んでいるピースを取得
        for(int i = 0; i < shop_pieces.size(); ++i) {
            auto piece_purchase_button = Scene::Object::Create<UIButton>();
            piece_purchase_button->SetName("PiecePurchaseButton");
            float x_pos = 400.0f + (i * 150.0f);    //X位置を設定
            piece_purchase_button->SetTranslate(float3(x_pos, 500.0f, 0.0f));
            auto texture = std::make_shared<Texture>(100, 200, DXGI_FORMAT_R8G8B8A8_UNORM);
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
                            PieceInfo piece_info;
                            piece_info.SetTypeName(purchase_piece->GetNameDefault().data());
                            piece_info.SetOwner(player);    //ピースのオーナーをプレイヤーに設定
                            player->AddPieceToStand(piece_info);
                            piece_stand->AddPiece(std::move(purchase_piece));    //ピースを購入する
                            shop_stand->InvalidateShopPiece(i);                  //購入したピースをショップから無効化する
                            player->InvalidateShopPiece(i);                      //プレイヤー側のショップ情報も無効化する
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
                    //一旦ピースの一番目をうつす
                    //if(auto draw_piece = shop_pieces[i].lock()) {
                    //    //モデルを描画
                    //    //if(auto model = draw_piece->GetComponent<ComponentModel>()) {
                    //    //    MV1DrawModel(model->GetModel());
                    //    //}
                    //}
                    //piece_purchase_button->SetImage(*texture);             //スクリーンを入れ込む。
                    SetRenderTarget(GetHdrBuffer(), GetDepthStencil());    //レンダーターゲットを戻す
                }
            };
            piece_purchase_button->SetProc("draw_target", draw_target, ProcTiming::Draw, ProcPriority::NONE);
            purchase_window_objects.push_back(piece_purchase_button);    //購入画面のウィンドウ群に追加
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
            player->RerollShopPieces();    //ショップのピースをリロールする
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
                player->AddExp(4);    //経験値を4増やす
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
                curr_exp_ui->SetProc("set_text", set_text_proc, ProcTiming::Update, ProcPriority::NONE);
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
                next_exp_ui->SetProc("set_text", set_text_proc, ProcTiming::Update, ProcPriority::NONE);
            }
        }
    }
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
        piece_num_ui->SetProc("set_text", set_text_proc, ProcTiming::Update, ProcPriority::NONE);
        piece_num_ui->SetTranslate(float3(500.0f, 250.0f, 0.0f));
        piece_num_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
    }
    //---------------------------------------------------------------------------------
    //  割線UI
    //---------------------------------------------------------------------------------
    {
        auto line_ui = Scene::Object::Create<UIText>();
        line_ui->SetFontSize(80);                                         //フォントサイズ設定
        line_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));    //文字色設定
        line_ui->SetText("/");
        line_ui->SetTranslate(float3(550.0f, 250.0f, 0.0f));
        line_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
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
        piece_max_ui->SetProc("set_level", set_text_proc, ProcTiming::Update, ProcPriority::NONE);
        piece_max_ui->SetTranslate(float3(600.0f, 250.0f, 0.0f));
        piece_max_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
    }
    //---------------------------------------------------------------------------------
    //  ターン数表示UI
    //---------------------------------------------------------------------------------
    {
        auto turn_ui = Scene::Object::Create<UIText>();
        turn_ui->SetFontSize(50);                                             //フォントサイズ設定
        turn_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));        //文字色設定
        turn_ui->SetTranslate(float3(50.0f, 50.0f, 0.0f));                    //位置を左上部に設定
        turn_ui->SetAlignment(ComponentTransformUI::Alignment::UpperLeft);    //左上寄せに設定
        //更新処理
        auto set_text_proc = [this, turn_ui]() { turn_ui->SetText("Turn: " + std::to_string(turn_count_)); };
        turn_ui->SetProc("set_turn", set_text_proc, ProcTiming::Update, ProcPriority::NONE);
    }
    //---------------------------------------------------------------------------------
    //  フェーズタイマーUI
    //---------------------------------------------------------------------------------
    {
        auto phase_timer_ui = Scene::Object::Create<UIText>();
        phase_timer_ui->SetFontSize(50);                                               //フォントサイズ設定
        phase_timer_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));          //文字色設定
        phase_timer_ui->SetTranslate(float3(300.0f, 50.0f, 0.0f));                     //位置を上部中央あたりに設定
        phase_timer_ui->SetAlignment(ComponentTransformUI::Alignment::UpperCenter);    //中央寄せに設定
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
        phase_timer_ui->SetProc("set_phase_timer", set_text_proc, ProcTiming::Update, ProcPriority::NONE);
    }
    //---------------------------------------------------------------------------------
    //  フェーズ表示UI
    //---------------------------------------------------------------------------------
    {
        auto phase_ui = Scene::Object::Create<UIText>();
        phase_ui->SetFontSize(50);                                              //フォントサイズ設定
        phase_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));         //文字色設定
        phase_ui->SetTranslate(float3(600.0f, 50.0f, 0.0f));                    //位置を上部に設定
        phase_ui->SetAlignment(ComponentTransformUI::Alignment::UpperRight);    //右上寄せに設定
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
        phase_ui->SetProc("set_phase", set_text_proc, ProcTiming::Update, ProcPriority::NONE);
    }
    //---------------------------------------------------------------------------------
    //  Agent表示UI
    //---------------------------------------------------------------------------------
    {
        int agent_count = 0;
        for(auto& agent : Scene::Object::GetArray<Agent>()) {
            agent_count++;
            auto agent_ui = Scene::Object::Create<UIText>();
            agent_ui->SetFontSize(30);                                                       //フォントサイズ設定
            agent_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));                  //文字色設定
            agent_ui->SetTranslate(float3(150.0f, 100.0f + (agent_count * 40.0f), 0.0f));    //位置を左上あたりに設定
            agent_ui->SetAlignment(ComponentTransformUI::Alignment::UpperLeft);              //左上寄せに設定
            agent_ui->SetText(agent->GetName());                                             //エージェント名を表示
        }
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
        if(state_timer_ >= BATTLE_PHASE_DURATION) {
            TransitionTo(GameState::Setup);
            DestroyPiecesAfterBattlePhase();    //バトルフェーズ用に生成した駒を破棄する
            //---------------------------------------------------------------------------------
            //  このタイミングで無料リロール
            //---------------------------------------------------------------------------------
            for(auto& agent : Scene::Object::GetArray<Agent>()) {
                //ショップがロックしてなければ
                if(!agent->IsShopLocked()) {
                    agent->RerollShopPieces();    //ショップのピースをリロールする
                }
            }
            //---------------------------------------------------------------------------------
            // ボードと、ボードに配置されているピースの更新をonにする
            //---------------------------------------------------------------------------------
            if(auto chess_board = Scene::Object::Get<ChessBoard>()) {
                chess_board->SetBoardProcessEnable(true);
            }
            ++turn_count_;
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
                    auto piece = PieceFactory::CreatePiece(type_name);
                    //駒の位置を設定(左手前から)
                    float x_pos = (r * SQUARE_SIZE) - 4 * (SQUARE_SIZE);
                    float z_pos = (f * SQUARE_SIZE) - (4 * SQUARE_SIZE);
                    piece->SetTranslate(float3(x_pos, 0.5f, z_pos));
                    piece->SetOwner(player);    //オーナーを設定
                }
            }
        }
    }
    //----------------------------------------------------------------------
    //次にNPCの駒を生成
    //----------------------------------------------------------------------
    //とりあえずテストで抽選ナシの一人分
    for(auto& npc : Scene::Object::GetArray<Npc>()) {
        //ボードの位置に駒を生成
        for(int f = 0; f < 4; f++) {
            for(int r = 0; r < 8; r++) {
                auto piece_info = npc->GetBoardInfo(f, r);
                //駒情報が有効なら
                std::string type_name = piece_info.GetTypeName();
                if(type_name != "") {
                    //駒を生成
                    auto piece = PieceFactory::CreatePiece(type_name);
                    //駒の位置を設定(右奥から)
                    float x_pos = (r * SQUARE_SIZE) - 4 * (SQUARE_SIZE);
                    float z_pos = ((7 - f) * SQUARE_SIZE) - (4 * SQUARE_SIZE);
                    piece->SetTranslate(float3(x_pos, 0.5f, z_pos));
                    piece->SetOwner(npc);    //オーナーを設定
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
}
