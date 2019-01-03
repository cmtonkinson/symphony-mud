module Talent::PassiveAbility

  def type
    :passive
  end

  def type_s
    "passive"
  end

  def passive?
    true
  end

  def active?
    false
  end

end

