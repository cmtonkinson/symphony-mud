module SystemTest

  define :create_sysadmin do
    with "sysadmin"
    test do
      sysadmin.command "say Hello, world!"
    end
  end

  define :show_summary do
    from :create_sysadmin
    with "sysadmin"
    test do
      sysadmin.command "summary"
    end
  end

end

