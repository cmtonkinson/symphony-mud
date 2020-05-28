module SystemTest

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
      groups = available.scan /^[A-Z]+\s+[a-z,_\s]+$/
      groups.each { |group| list += group.scan(/[a-z_]+/) }

      # There are a few we don't want to try.
      dont_try = %w(pry reboot shutdown test quit log_level)
      list.reject! { |c| dont_try.include? c }

      # Run each command in the list - with no parameters - just as a basic
      # smoke test.
      list.each do |c|
        admin c
        n_expect 'error'
      end
    end
  end

end

