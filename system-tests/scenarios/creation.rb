#module SystemTest
#
#  define :avatar_creation do
#    with :admin
#
#    def creation_permutations
#      %w(human elf dwarf).each do |race|
#        %w(male female neuter).each do |gender|
#          %w(right left).each do |hand|
#            %w(warrior mage cleric rogue).each do |klass|
#              %w(tiny short average tall huge).each do |height|
#                %w(bony thin average heavy obese).each do |weight|
#                  yield race, gender, hand, klass, height, weight
#                end
#              end
#            end
#          end
#        end
#      end 
#    end
#
#    creation_permutations do |race, gender, hand, klass, height, weight|
#      with ("%s_%s_%s_%s_%s_%s" % [race, gender, hand, klass, height, weight]).to_sym, {
#        race: race,
#        gender: gender,
#        hand: hand,
#        klass: klass,
#        height: height,
#        weight: weight,
#      }
#    end
#
#    test do
#      creation_permutations do |race, gender, hand, klass, height, weight|
#        str = "%s_%s_%s_%s_%s_%s" % [race, gender, hand, klass, height, weight]
#        output_polling = false
#        send str.to_sym, 'summary'
#        output_polling = true
#        client = (str + '_client').to_sym
#        expect /race:\s+#{race}/i, str
#        expect /gender:\s+#{gender}/i, str
#        expect /hand:\s+#{hand}/i, str
#        expect /class:\s+#{klass}/i, str
#        expect /height:\s+#{height}/i, str
#        expect /weight:\s+#{weight}/i, str
#      end
#    end
#  end
#
#end
#
