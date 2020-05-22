module SystemTest

  define :a do
    test do
      puts "DOING THE a TEST"
    end
  end

  define :b do
    from :a
    test do
      puts "DOING THE b TEST"
    end
  end

  define :c do
    from :a
    test do
      puts "DOING THE c TEST"
    end
  end

  define :d do
    from :b
    test do
      puts "DOING THE d TEST"
    end
  end

  define :e do
    from :d
    test do
      puts "DOING THE e TEST"
    end
  end

end

