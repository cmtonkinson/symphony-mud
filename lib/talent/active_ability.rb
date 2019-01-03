module Talent::ActiveAbility

  def type
    :active
  end

  def type_s
    "active"
  end

  def passive?
    false
  end

  def active?
    true
  end

end

