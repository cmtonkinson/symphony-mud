module Realm
  class Avatar < Being
    attr_accessor :user
    attr_accessor :iedit, :nedit, :zedit, :redit, :qedit

    define_hooks :before_disconnect

    simple_flag :pref_numeric, false, :pref_show_numeric?, :pref_show_numeric!, nil, :pref_hide_numeric!
    simple_flag :pref_color, true, :pref_show_color?, :pref_show_color!, nil, :pref_hide_color!

    def initialize(id = nil, props = {})
      super
      @force_output = false
    end

    before_disconnect lambda {
      @room.puts I18n.t('universe.room_announce_exit'), self
      Symphony.logger.info '%s logged out' % fqid_str
      @room.beings.delete self
    }

    vivify lambda {
      $world.avatars.insert self
    }

    enervate lambda {
      @world.avatars.remove self
    }

    def force_output!
      @force_output = true
    end

    def test_and_clear_forced_output
      o = @force_output
      @force_output = false
      o
    end

    def _prompt_prefix
      if redit
        '{RREDIT{x {Y%d{x ' % room.rid
      elsif qedit
        '{RQEDIT{x {Y%d{x ' % qedit.rid
      else
        ''
      end
    end

    # Verify room edit permissions prior to room change.
    before_room_change lambda { |from, to|
      if redit && from.zone != to.zone
        self.redit = false
        user.operators.pop
      end
    }

    def summary
      $templates.avatar_summary % [
        name,
        id,
        level
      ] + summary_plate
    end

    def summary_plate
      $templates.summary_plate % [
        anatomy&.name, strength.current, hit_bonus.current, level,
        gender, dexterity.current, damage_bonus.current, health.current, health.max,
        klass&.name, constitution.current, mana.current, mana.max,
        hand, intelligence.current, armor.current, stamina.current,
        focus.current, bash.current, exp,
        age, creativity.current, slash.current, tnl,
        Display::Format.height(height), charisma.current, pierce.current, training_points,
        Display::Format.weight(weight), luck.current, exotic.current,
        items.length, 0, 0, 0, '',
        0, 0, 0, ''
      ]
    end
  end
end
