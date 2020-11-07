module SystemTest
  define :status do
    from :create_admin
    with :admin
    with :vork, { klass: 'warrior' }
    test do
      vork 'status'
      expect "You don't currently have any"
    end
  end

  define :poison do
    from :status
    with :admin
    with :vork
    test do
      admin 'effect vork poison'
      expect_last vork, 'You feel ill'

      vork 'status'
      expect 'poison'
    end
  end

  define :chill do
    from :status
    with :admin
    with :vork
    test do
      admin 'effect vork chill'
      expect_last vork, 'You feel cold'

      vork 'status'
      expect 'chill'
    end
  end

  define :rage do
    def lookup_attr(attr)
      vork 'summary'
      matches = /#{attr}:\s+(\d+)\D/im.match vork_client.buffer[-1]
      num = matches[1].to_i
      vork "say My #{attr} is #{num}."
      num
    end

    from :status
    with :admin
    with :vork

    before do
      @str0 = lookup_attr 'stren'
      @dex0 = lookup_attr 'dexte'
      @con0 = lookup_attr 'const'
    end

    test do
      admin 'effect vork rage'
      expect_last vork, 'You grow angry'

      vork 'status'
      expect 'rage'

      @str1 = lookup_attr 'stren'
      @dex1 = lookup_attr 'dexte'
      @con1 = lookup_attr 'const'

      raise "Strength isn't higher" unless @str1 > @str0
      raise "Dexterity isn't higher" unless @dex1 > @dex0
      raise "Constitution isn't higher" unless @con1 < @con0
    end
  end

  define :create_weapon do
    from :status
    with :admin
    before do
      admin 'iedit create'
      admin 'name sword'
      admin 'wearable hold'
      admin 'type weapon'
      admin 'weapon_type sword'
      admin 'damage_type slash'
      admin 'hitroll 10'
      admin 'damroll 10'
      admin 'exit'
    end

    test do
      admin 'load item 1'
      expect 'create a sword'
      admin 'id sword'
      n_expect %w[Effects sharp hit_bonus damage_bonus]
    end
  end

  define :hone do
    from :create_weapon
    with :admin
    test do
      admin 'hone sword'
      expect 'You hone a sword'
      admin 'id sword'
      expect %w[Effects sharp hit_bonus damage_bonus]
    end
  end
end
