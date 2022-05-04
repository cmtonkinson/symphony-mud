module Test
  define :say_hello do
    with :vork
    with :tink
    test do
      vork 'say hello'
      expect 'You say, "hello"'
      expect_last tink, 'vork says, "hello"'
    end
  end
end
