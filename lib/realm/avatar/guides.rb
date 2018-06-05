module Realm
  class Avatar
    attr_accessor :num_logins

    define_hooks :on_first_login
    define_hooks :on_login

    ###########################################################################
    on_hashify -> {
      hash = {
        num_logins: @num_logins
      }

      { guides: hash }
    }

    ###########################################################################
    on_dehashify -> hash {
      @num_logins = hash[:num_logins]
    }

    ###########################################################################
    # Promote the first avatar in the system to CREATOR.
    on_first_login -> {
      # Only run for the first account.
      return if Base::Account.user_map.length > 1

      # Advance to hero by awarding experience, so as not to cause issues for
      # higher level Avatars if/when new features depend on experience.
      award_experience tnl while level < HERO
      gain_level while level < CREATOR
      
      # Notify.
      puts "\n\n{WAs the first Avatar in the system, you have been made system administrator (level {Y%d{W aka 'CREATOR')" % level
    }
  end
end
