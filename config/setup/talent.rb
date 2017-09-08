###############################################################################
# Load all Klasses and Abilities
###############################################################################
Dir[Symphony.klasses.join '*.rb'].each { |file| require file }
Dir[Symphony.skills.join '*.rb'].each { |file| require file }
#Dir[Symphony.spells.join '*.rb'].each { |file| require file }

