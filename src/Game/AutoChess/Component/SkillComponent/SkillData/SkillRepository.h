//---------------------------------------------------------------------------
//!	@file	SkillRepository.h
//! @brief	スキルのマスターデータ情報のリポジトリクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include "SkillData.h"
class SkillRepository
{
public:
    //--------------------------------------------------
    // JSONファイルからスキル定義を読み込む
    //! @param path JSONファイルのパス
    //! @return 成功時 true、失敗時 false
    //--------------------------------------------------
    bool LoadFromJson(const std::string& path);

    //--------------------------------------------------
    // スキル名で定義を検索
    //! @param skill_name [in] スキル名
    //! @return 該当する SkillData（存在しない場合は nullptr）
    //--------------------------------------------------
    const SkillData* FindBySkillName(const std::string_view& skill_name) const;

    //--------------------------------------------------
    // 全スキル定義を取得（UI表示などに使用）
    //! @return スキル定義の一覧
    //--------------------------------------------------
    const std::vector<SkillData>& GetAllSkills() const;

private:
    std::vector<SkillData> skills_;    //!< スキル定義の一覧
};
