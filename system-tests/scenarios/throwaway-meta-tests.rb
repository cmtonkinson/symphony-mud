module SystemTest

  #############################################################################
  define :create_sysadmin do
    with "sysadmin"
    test do
      str = "Hello, world!"
      sysadmin "say #{str}"
      expect str
    end
  end

  #############################################################################
  define :promotion do
    from :create_sysadmin
    with "sysadmin"
    with "lowbie"

    before do
      sysadmin 'promote lowbie 100'
    end

    test do
      lowbie 'summary'
      expect 'you are a hero'
      expect /level:\s+100\D/
    end
  end

  #############################################################################
  define :show_connections do
    from :create_sysadmin
    with "sysadmin"
    test do
      sysadmin "connections"

      expect_last sysadmin, 'sysadmin'
      expect_last sysadmin, /localhost/ix

      expect 'IP:Port'

      expect_any sysadmin, 'has returned to the Realm!'
      expect_any sysadmin, /\d+\.\d+\.\d+\.\d+:\d+/

      nexpect_any sysadmin, /\[error\]/ix
    end
  end

  #############################################################################
  define :smoke_test_commands do
    from :create_sysadmin
    with "sysadmin"
    test do
      # List all commands.
      sysadmin "commands"
      available = sysadmin.buffer[-1]

      # Parse the output into a list of Strings.
      list = []
      groups = available.scan /^[A-Z]+\s+[a-z,_\s]+$/
      groups.each { |group| list += group.scan(/[a-z_]+/) }

      # There are a few we don't want to try.
      dont_try = %w(pry reboot shutdown test quit log_level)
      list.reject! { |c| dont_try.include? c }

      # Run each command in the list - with no parameters - just as a basic
      # smoke test.
      list.each do |c|
        sysadmin c
        n_expect 'error'
      end
    end
  end

end

