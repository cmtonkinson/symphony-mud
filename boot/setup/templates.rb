require 'ostruct'

$templates = OpenStruct.new

[Symphony.root.join('templates'), Symphony.root.join('game', 'templates')].each do |tpl_dir|
  Dir[tpl_dir.join '*.txt'].each do |path|
    name = File.basename(path).sub '.txt', ''
    $templates.send "#{name}=", File.read(path)
  end
end
