module Command
  define :imap, :imap do
    usage
    exec do
      being.interactive = InteractiveMap.new being
      being.user.operators.push InteractiveOperator.new being.user, :imap
    end
  end
end
