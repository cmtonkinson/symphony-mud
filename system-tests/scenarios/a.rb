module SystemTest

  define :a do
    test do
      puts ">>>> A test"
    end
  end

  define :b do
    from :a
    test do
      puts ">>>> B test"
    end
  end

  define :c do
    from :b
    test do
      puts ">>>> C test"
    end
  end


  define :d do
    from :c
    test do
      puts ">>>> D test"
    end
  end


  define :e do
    from :b
    test do
      puts ">>>> E test"
    end
  end


  define :f do
    from :a
    test do
      puts ">>>> F test"
    end
  end

  define :g do
    from :a
    test do
      puts ">>>> G test"
    end
  end

end

