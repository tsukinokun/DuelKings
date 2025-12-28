//---------------------------------------------------------------------------
//!	@file	PieceSkillUser.cpp
//! @brief	ピースのスキル使用機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceSkillUser.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>
#include <Game/AutoChess/Component/StatusEffect/StunStatus.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void PieceSkillUser::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // スキル使用処理の登録
    //---------------------------------------------------------
    auto update_proc = [this]() {
        auto this_piece = dynamic_pointer_cast<Piece>(GetOwnerPtr());    //オーナーはピースであることが前提
        //---------------------------------------------------------
        // スタンチェックをして、スタン中なら処理を抜ける
        //---------------------------------------------------------
        if(this_piece->GetComponent<StunStatus>()) {
            return;
        }
        //---------------------------------------------------------
        //アクティブスキルコンポーネントを取得
        //---------------------------------------------------------
        if(auto active_skill = this_piece->GetComponent<ComponentActiveSkill>()) {
            //スキルが使用可能かチェック
            if(active_skill->CanActivate()) {
                //スキルを使用
                active_skill->Activate();
            }
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NONE);
}
