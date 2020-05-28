module SystemTest

  define :default_gains do
    from :create_admin
    with :vork, { klass: 'warrior' }
    with :zaboo, { klass: 'mage' }

    test do
    end
  end

end
