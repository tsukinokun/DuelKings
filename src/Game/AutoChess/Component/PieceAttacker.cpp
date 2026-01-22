//---------------------------------------------------------------------------
//!	@file	PieceAttacker.cpp
//! @brief	ピースの攻撃機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceAttacker.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/system/Logic.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>
#include <Game/AutoChess/Component/StatusEffect/StunStatus.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void PieceAttacker::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // 攻撃処理
    //---------------------------------------------------------
    auto attack_proc = [this]() {
        auto this_piece = dynamic_pointer_cast<Piece>(GetOwnerPtr());    //ピースであることが前提
        //---------------------------------------------------------
        // スタンチェックをして、スタン中なら処理を抜ける
        //---------------------------------------------------------
        if(this_piece->GetComponent<StunStatus>()) {
            return;
        }
        //---------------------------------------------------------
        //  通常攻撃
        //---------------------------------------------------------
        auto delta_time  = GetDeltaTime();
        attack_timer_   -= delta_time;
        lock_timer_     -= delta_time;
        //攻撃クールタイムが0.0f以下なら
        if(attack_timer_ <= 0.0f) {
            if(this_piece->GetComponent<ComponentTransform>()) {
                //---------------------------------------------------------
                //  ロックしているターゲットに攻撃を仕掛ける
                //---------------------------------------------------------
                if(auto locked_target = locked_target_.lock()) {
                    //自分の位置を取得
                    float3 translate = this_piece->GetTranslate();
                    //敵の位置を取得
                    float3 locked_target_position = locked_target->GetTranslate();
                    //攻撃方向を計算
                    float3 direction = locked_target_position - translate;
                    //ベクトルの大きさが射程距離以下なら攻撃
                    float distance = length(direction);
                    if(distance <= this_piece->GetAttackRange()) {
                        //攻撃力を取得
                        int         attack_power  = this_piece->GetAttackPower();
                        PieceStatus target_status = locked_target->GetFinalStatus();
                        //最終的なダメージをこちらで計算(MP回復がしたいので)
                        int final_damage = CalculateFinalDamage(attack_power, DamageType::Physical, target_status);
                        //---------------------------------------------------------
                        // MP回復処理
                        //---------------------------------------------------------
                        if(auto skill_component = this_piece->GetComponent<ComponentActiveSkill>()) {
                            int mp_gain = CalculateMPGain(final_damage, DamageType::Physical, false);
                            skill_component->AddMP(mp_gain);
                        }
                        //敵のHPを減少させる
                        locked_target->TakeDamage(attack_power);
                        //攻撃クールタイムをリセット
                        attack_timer_ = this_piece->GetAttackInterval();
                    }
                }
                //---------------------------------------------------------
                //  なにもロックしていなければ最も近い敵の駒に攻撃を仕掛ける
                //---------------------------------------------------------
                else if(auto sensor = this_piece->GetComponent<PieceSensor>()) {
                    //敵が存在していたら
                    if(auto nearest_enemy = sensor->GetNearestEnemy()) {    //最も近い敵を取得
                        //自分の位置を取得
                        float3 translate = this_piece->GetTranslate();
                        //敵の位置を取得
                        float3 nearest_enemy_position = nearest_enemy->GetTranslate();
                        //攻撃方向を計算
                        float3 direction = nearest_enemy_position - translate;
                        //ベクトルの大きさが射程距離以下なら攻撃
                        float distance = length(direction);
                        if(distance <= this_piece->GetAttackRange()) {
                            //攻撃力を取得
                            int         attack_power  = this_piece->GetAttackPower();
                            PieceStatus target_status = nearest_enemy->GetFinalStatus();
                            //最終的なダメージをこちらで計算(MP回復がしたいので)
                            int final_damage = CalculateFinalDamage(attack_power, DamageType::Physical, target_status);
                            //---------------------------------------------------------
                            // MP回復処理
                            //---------------------------------------------------------
                            if(auto skill_component = this_piece->GetComponent<ComponentActiveSkill>()) {
                                int mp_gain = CalculateMPGain(final_damage, DamageType::Physical, false);
                                skill_component->AddMP(mp_gain);
                            }
                            //敵のHPを減少させる
                            nearest_enemy->TakeDamage(attack_power);
                            //攻撃クールタイムをリセット
                            attack_timer_ = this_piece->GetAttackInterval();
                        }
                    }
                }
            }
        }
    };
    SetProc("attack_proc", attack_proc, ProcTiming::Update, ProcPriority::NORMAL);
}

//---------------------------------------------------------
//! @brief  ターゲットをロックする関数
//---------------------------------------------------------
void PieceAttacker::LockTarget(const std::shared_ptr<Piece>& target, float duration)
{
    locked_target_ = target;
    lock_timer_    = duration;
}
