# NOTE: To use this module, your class must implement a 'values' method which
# returns a list of the Objekts within.

module Util
  module Locatable
    ###########################################################################
    # Find Objekts by name in the values Array.
    # Returns an Array of Objekts on success; or an empty Array on any failure.
    def locate(input)
      if input.is_a? String
        id_arg = Command::ARGUMENT_TYPES[:id]
        return false unless id_arg[:pattern].match input

        id = id_arg[:cast_func].call input
      elsif input.is_a? OpenStruct
        id = input
      end

      # Look for exact matches first.
      candidates = values.select { |obj| obj.name.downcase == id.name }
      # If no exact matches were found, look for prefix matches among the words in the name.
      if candidates.empty?
        candidates = values.select do |obj|
          terms = obj.name.downcase.split
          terms.any? { |term| term.index(id.name).zero? }
        end
      end

      # If no prefix matches were found, give up.
      return [] if candidates.empty?

      case id.symbol
      when '#' # Return a single candidate (if exists) by 1-indexed list
        return [] if candidates.length < id.number

        [candidates[id.number - 1]]
      when '*' # Return some quantity of candidates
        how_many = case id.number
                   when Integer then id.number
                   when 'all' then candidates.length
                   when 'half' then candidates.length / 2
                   else 0
                   end
        candidates.take how_many
      else # Return the first candidate
        [candidates.first]
      end
    end

    ###########################################################################
    def locate_first(input)
      matches = locate input
      return nil if matches.empty?

      matches.first
    end
  end
end
