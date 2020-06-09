###############################################################################
# Load all Klasses and Abilities
###############################################################################
Dir[Symphony.anatomies.join '*.rb'].sort.each { |file| require file }
Dir[Symphony.klasses.join '*.rb'].sort.each { |file| require file }
Dir[Symphony.skills.join '*.rb'].sort.each { |file| require file }
Dir[Symphony.spells.join '*.rb'].sort.each { |file| require file }
