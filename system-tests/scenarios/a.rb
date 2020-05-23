module SystemTest

  define :create_sysadmin do
    with "sysadmin"
    test do
      sysadmin.command "say Hello, world!"
    end
  end

end

