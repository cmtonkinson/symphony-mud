$universe = Base::Universe.new
$world    = Realm::World.new

if Symphony.configuration.console
  $universe.do_console
elsif Symphony.configuration.script
  $universe.do_script
else
  $universe.do_server
end
