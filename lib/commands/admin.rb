module Command

  ##################################################################################################
  # reboot
  ##################################################################################################
  define :reboot do
    exec do
      user.puts "Hold on to your butts... Copyover!!"
      $universe.should_reboot = true
    end
  end

  ##################################################################################################
  # shutdown
  ##################################################################################################
  define :shutdown do
    exec do
      user.puts "Shutting down the server."
      $universe.should_stop = true
    end
  end

end
