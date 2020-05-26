module SystemTest

  define :promotion do
    with 'admin'
    with 'lowbie'

    before do
      expect_last admin, 'you have been made the system administrator'
    end

    test do
      admin 'promote lowbie 100'
      lowbie 'summary'
      expect 'you are a hero'
      expect /level:\s+100\D/im
    end
  end

  define :learn_independent do
    from :promotion
    with 'lowbie'

    before do
      lowbie 'skills'
      expect /skills you might learn.+block/im
   end

    test do
      lowbie 'learn parry'
      expect "Sorry, you don't meet the requirements"

      lowbie 'learn block'
      expect /Congratulations.+block/im

      lowbie 'skills'
      expect /skills you know.+block.+you might learn/im
      expect /block.+\(bad\)/

      lowbie 'learn block'
      expect 'But you already know'
   end
  end

  define :learn_dependent do
    from :promotion
    with 'lowbie'

    before do
      lowbie 'skills'
      expect /skills you might learn.+block/im
      expect /skills you might learn.+parry/im
     end

    test do
      lowbie 'learn parry'
      expect "Sorry, you don't meet the requirements"

      lowbie 'learn block'
      lowbie 'learn parry'
      expect /Congratulations.+parry/im
    end
  end

end

