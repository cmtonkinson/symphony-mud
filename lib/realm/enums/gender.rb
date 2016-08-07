module Realm
  class Realm::Gender

    include Ruby::Enum

    define :MALE,   I18n.t("linguist.gender.male")
    define :FEMALE, I18n.t("linguist.gender.female")
    define :NEUTER, I18n.t("linguist.gender.neuter")

    @pronouns = {
      object: {
        MALE:   I18n.t("linguist.pronoun.object.male"),
        FEMALE: I18n.t("linguist.pronoun.object.female"),
        NEUTER: I18n.t("linguist.pronoun.object.neuter"),
      },
      subject: {
        MALE:   I18n.t("linguist.pronoun.subject.male"),
        FEMALE: I18n.t("linguist.pronoun.subject.female"),
        NEUTER: I18n.t("linguist.pronoun.subject.neuter"),
      },
      possessive: {
        MALE:   I18n.t("linguist.pronoun.possessive.male"),
        FEMALE: I18n.t("linguist.pronoun.possessive.female"),
        NEUTER: I18n.t("linguist.pronoun.possessive.neuter"),
      },
      reflexive: {
        MALE:   I18n.t("linguist.pronoun.reflexive.male"),
        FEMALE: I18n.t("linguist.pronoun.reflexive.female"),
        NEUTER: I18n.t("linguist.pronoun.reflexive.neuter"),
      },
    }

    @pronouns.each do |type, defns|
      self.class.send :define_method, "#{type}_pronoun" do |gender|
        defns[key gender]
      end
    end

  end
end
