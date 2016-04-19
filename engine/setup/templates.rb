require "ostruct"

$templates = OpenStruct.new

Dir[Symphony.templates.join "*.txt"].each do |path|
  name = File.basename(path).sub ".txt", ""
  $templates.send "#{name}=", File.read(path)
end

