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

  define :effect do
    from :status
    with :admin
    with :vork
    test do
      admin 'effect vork poison'
      expect 'You apply the'
      expect_last vork, 'admin applies the'
    end
  end

  define :poison do
    from :effect
    with :admin
    with :vork
    test do
      admin 'effect vork poison'
      expect_last vork, 'You feel ill'

      vork 'status'
      expect 'poison'
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

    from :effect
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
end
