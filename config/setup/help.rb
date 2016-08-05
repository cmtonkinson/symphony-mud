# Each subdirectory of game/help corresponds to a logical grouping (a category)
# of help files (topics). So the file "game/help/foo/bar.txt" contains the help
# text for topic "bar" in category "foo". Categorization is arbitrary and
# only exists to prevent an unmanageably large single directory of files.

$help = Hash.new

Dir[Symphony.help.join "*"].each do |help_dir|
  Dir[Pathname.new(help_dir).join "*.txt"].each do |help_file|
    category = File.basename(help_dir).to_sym
    topic    = File.basename help_file, ".txt"
    $help[category] ||= {}
    $help[category][topic] = File.read help_file
  end
end
