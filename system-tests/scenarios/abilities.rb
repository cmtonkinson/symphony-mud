module SystemTest
  #############################################################################
  define :promotion do
    with :admin
    with :lowbie

    before do
      expect_any admin, 'you have been made the system administrator'
    end

    test do
      admin 'promote lowbie 100'
      lowbie 'summary'
      expect 'you are a hero'
      expect(/level:\s+100\D/im)
    end
  end

  #############################################################################
  define :learn_independent do
    from :promotion
    with :lowbie

    before do
      lowbie 'skills'
      expect 'skills available'
      expect(/block\s+^/i)
    end

    test do
      lowbie 'learn parry'
      expect "Sorry, you don't meet the requirements"

      lowbie 'learn block'
      expect(/Congratulations.+block/i)

      lowbie 'skills'
      expect(/block.+\(bad/i)

      lowbie 'learn block'
      expect 'But you already know'
    end
  end

  #############################################################################
  define :learn_dependent do
    from :promotion
    with :lowbie

    before do
      lowbie 'skills'
      expect(/block\s+^/i)
      expect(/parry\s+^/i)
    end

    test do
      lowbie 'learn parry'
      expect "Sorry, you don't meet the requirements"

      lowbie 'learn block'
      lowbie 'learn parry'
      expect(/Congratulations.+parry/im)
    end
  end

  #############################################################################
  define :practice do
    def get_trains
      lowbie 'summary'
      matches = /trains:\s+(\d+)\D/im.match lowbie_client.buffer[-1]
      trains = matches[1].to_i
      lowbie "say I have #{@trains} training points."
      trains
    end

    from :learn_independent
    with :lowbie

    before do
      lowbie 'skills'
      expect(/block.+bad/im)
      @initial_trains = get_trains
    end

    test do
      lowbie 'practice block'
      lowbie 'practice block'
      lowbie 'practice block'
      expect(/you spend.+to increase your/im)

      lowbie 'skills'
      n_expect(/block.+bad/im)

      @updated_trains = get_trains

      raise "Didn't consume any training points" unless @updated_trains < @initial_trains
    end
  end
end
