module Command

  ##################################################################################################
  define :reboot do
    usage
    exec do
      being.puts "Hold on to your butts... Copyover!!"
      $universe.should_reboot = true
    end
  end

  ##################################################################################################
  define :shutdown do
    usage
    exec do
      being.puts "Shutting down the server."
      $universe.should_stop = true
    end
  end

end
