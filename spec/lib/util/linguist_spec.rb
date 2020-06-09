require 'spec_helper'

include Util

describe Linguist do

  #############################################################################
  describe '#interpret' do
    err = Realm.configuration.error_string

    room = Realm::Room.new

    chuck    = Realm::Avatar.new nil, gender: Gender::MALE, name: 'Chuck'
    charlene = Realm::Avatar.new nil, gender: Gender::FEMALE, name: 'Charlene'
    pat      = Realm::Avatar.new nil, name: 'Pat'

    rat1     = Realm::Being.new nil, name: 'fat rat'
    rat2     = Realm::Being.new nil, name: 'obese rat'
    rat3     = Realm::Being.new nil, name: 'King Rat'

    sword1 = Realm::Item.new nil, name: 'sword'
    sword2 = Realm::Item.new nil, name: 'auger'
    sword3 = Realm::Item.new nil, name: 'Litch Hammer'

    tests = [
      #######################################################################
      # No interpretation.
      [['a b c'], 'a b c'],
      #######################################################################
      # Not enough arguments
      [['$string $string', 'a'], "a #{err}"],
      # Too many arguments
      [['$string', 'a', 'b'], 'a'],
      #######################################################################
      # Generic strings
      [['$string $string', 'a', 'b'], 'a b'],
      #######################################################################
      # Avatars
      [['$being', chuck], 'Chuck'],
      #######################################################################
      # Beings
      [['$being', rat1],  'a fat rat'],
      [['$being', rat2],  'an obese rat'],
      [['$being', rat3],  'King Rat'],
      #######################################################################
      # Items
      [['$item', sword1], 'a sword'],
      [['$item', sword2], 'an auger'],
      [['$item', sword3], 'Litch Hammer'],
      #######################################################################
      # Exits
      [['$exit', 'north'], 'the north'],
      [['$exit', 'east'],  'the east'],
      [['$exit', 'south'], 'the south'],
      [['$exit', 'west'],  'the west'],
      [['$exit', 'up'],    'above'],
      [['$exit', 'down'],  'below'],
      [['$opposite', 'north'], 'the south'],
      [['$opposite', 'east'],  'the west'],
      [['$opposite', 'south'], 'the north'],
      [['$opposite', 'west'],  'the east'],
      [['$opposite', 'up'],    'below'],
      [['$opposite', 'down'],  'above'],
      #######################################################################
      # Pronouns.
      [['$subjective', chuck],    'he'],
      [['$subjective', charlene], 'she'],
      [['$subjective', pat],      'it'],

      [['$objective', chuck],    'him'],
      [['$objective', charlene], 'her'],
      [['$objective', pat],      'it'],

      [['$possessive', chuck],    'his'],
      [['$possessive', charlene], 'her'],
      [['$possessive', pat],      'its'],

      [['$reflexive', chuck],    'himself'],
      [['$reflexive', charlene], 'herself'],
      [['$reflexive', pat],      'itself'],
    ]

    tests.each do |params|
      it "formats '#{params[0][0]}' with #{params[0].length - 1} arguments correctly" do
        input, output = params
        expect(Linguist.interpret(*input)).to eq output
      end
    end

  end

end
