module Realm
  class Avatar
    ###########################################################################
    after_death -> {
      respawn
    }

    ###########################################################################
    def respawn
      puts 'You have respawned.'

      health.current  = 1
      mana.current    = 1
      stamina.current = 1

      transport $world.find_room 1

      room.puts '$being slowly appears from nothingness, wincing and squnting in the light.', self, exclude: self
    end
  end
end

