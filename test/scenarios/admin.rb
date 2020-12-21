module Test
  #############################################################################
  define :create_admin do
    with :admin
    test do
      str = 'Hello, world!'
      admin "say #{str}"
      expect str
    end
  end

  #############################################################################
  define :smoke_test_commands do
    from :create_admin
    with :admin
    test do
      # List all commands.
      admin 'commands'
      available = admin.buffer[-1]

      # Parse the output into a list of Strings.
      list = []
      groups = available.scan(/^[A-Z]+\s+[a-z,_\s]+$/)
      groups.each { |group| list += group.scan(/[a-z_]+/) }

      # There are a few we either can't try, don't want to, or want to handle
      # specially (down below).
      dont_try = %w[pry reboot shutdown test quit log_level]
      list.reject! { |c| dont_try.include? c }

      # Run each command in the list - with no parameters - just as a basic
      # smoke test.
      list.each do |c|
        admin c
        n_expect 'error'
      end
    end
  end

  #############################################################################
  # This test is a little janky (including 2 hacks) but the alternative is a
  # possibly significant refactor of the system testing infrastructure to
  # accommodate just this one stupid test.
  define :reboot do
    from :create_admin
    with :admin
    test do
      admin 'reboot'

      # Let the server reboot. If we don't do this, the Micromanager will try
      # to read the pidfile before it's updated with the new Process Id.
      sleep 1
      expect_any admin, 'beginning reboot'

      # Force some command or else the reboot messages won't be in the output
      # buffer by default.
      admin 'look'
      expect_any admin, 'reboot successful'
    end
  end

  #############################################################################
  define :shutdown do
    from :create_admin
    with :admin
    test do
      admin 'shutdown'
      expect 'Shutting down'
    end
  end

  #############################################################################
  define :quit do
    from :create_admin
    with :admin
    test do
      admin 'quit'
      expect_any admin, 'Logging out'
    end
  end

  #############################################################################
  define :log_level do
    from :create_admin
    with :admin
    test do
      admin 'log_level qux'
      expect "doesn't make sense"

      admin 'log_level trace'
      expect(/adjusted.+verbosity/i)
    end
  end

  #############################################################################
  define :repeat do
    from :create_admin
    with :admin
    test do
      admin 'repeat 5 say test-a'
      expect 'You say, "test-a"'

      admin 'repeat 5 say test-b {#'
      expect_any admin, 'You say, "test-b 0"'
      expect_any admin, 'You say, "test-b 1"'
      expect_any admin, 'You say, "test-b 2"'
      expect_any admin, 'You say, "test-b 3"'
      expect_any admin, 'You say, "test-b 4"'
    end
  end

  #############################################################################
  define :identify do
    from :create_admin
    with :admin
    with :other
    test do
      admin 'id other'
      expect 'name other'
      expect 'level 1'
    end
  end
end
