module SystemTest
  define :new_account do
    with :admin
    with :newbie
    test do
      newbie 'detail'
      expect(/level:\s+1\s+/i)
    end
  end

  define :default_preferences do
    from :new_account
    with :newbie
    test do
      newbie 'preferences'
      expect 'terminal colors: YES'
      expect 'damage numbers: NO'
    end
  end

  define :change_color_preference do
    from :default_preferences
    with :newbie
    test do
      newbie 'preferences color no'
      expect(/color output.+no/i)
      newbie 'preferences'
      expect 'terminal colors: NO'

      newbie 'preferences color yes'
      expect(/color output.+yes/i)
      newbie 'preferences'
      expect 'terminal colors: YES'
    end
  end

  define :change_numeric_preference do
    from :default_preferences
    with :newbie
    test do
      newbie 'preferences numeric no'
      expect(/damage numbers.+no/i)
      newbie 'preferences'
      expect 'damage numbers: NO'

      newbie 'preferences numeric yes'
      expect(/damage numbers.+yes/i)
      newbie 'preferences'
      expect 'damage numbers: YES'
    end
  end
end
