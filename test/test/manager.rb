require 'rubytree'
require 'colorize'

module Test
  class Manager
    attr_accessor :num_tests, :num_pass, :num_fail

    def initialize
      @test_tree = Tree::TreeNode.new :root, nil
      @reg = Configuration.configuration.test_registry
      @num_tests = 0
      @num_pass = 0
      @num_fail = 0

      setup
    end

    def setup
      # Construct the full test tree.
      analyze @reg[0] until @reg.empty?

      if $LEAF_TEST
        # During a leaf test, we only need the target node and it's ancestors.
        target = @test_tree.find { |n| n.name == $LEAF_TEST }
        @test_tree = target.parentage.reverse << target
      elsif $TREE_TEST
        # During a tree test, we need the target node and it's ancestors, but
        # then must also recreate the
        target = @test_tree.find { |n| n.name == $TREE_TEST }
        parents = target.parentage.reverse
        new_tree = Tree::TreeNode.new :root, nil
        last_node = new_tree.root
        parents.each do |p|
          p_copy = p.detached_copy
          last_node.add p_copy
          last_node = p_copy
        end
        last_node.add target
        @test_tree = new_tree
      end
    end

    def analyze(test)
      # If the test has a parent_id, we need to resolve the dependency.
      if test.get_parent_id

        # Maybe the parent is already analized and in the tree?
        parent_node = @test_tree.find { |n| n.name == test.get_parent_id }

        # If not, maybe the parent is still un-analyzed sitting in @reg?
        if parent_node.nil?
          parent_test = @reg.find { |t| t.get_id == test.get_parent_id }

          # If not, id doesn't exist. That's a Bostonian "errah!"
          raise "Undefined parent test #{test.get_parent_id}" if parent_test.nil?

          parent_node = analyze parent_test
        end

      end

      # Create the new Node and remove the test from the registry.
      new_node = Tree::TreeNode.new test.get_id, test
      @reg.delete test

      # Add the new Node to either it's parent, or directly to the root.
      if parent_node.nil?
        @test_tree << new_node
      else
        parent_node << new_node
      end

      # Explicitly return the new node, because recursion.
      new_node
    end

    def start
      FileUtils.rm_rf Dir.glob $ROOT_DIR.join 'test', 'state', '*'

      @test_tree.each do |n|
        next if n.name == :root

        Micromanager.new n

        @num_tests += 1
        color = :green

        output = '%s: %s (%d assertions in %s) ' % [
          n.name.to_s.gsub('-', ' '),
          n.content.status.to_s,
          n.content.assertions,
          n.content.run_time
        ]

        if n.content.status == :pass
          @num_pass += 1
        else
          @num_fail += 1
          color = :red
          output += n.content.result
        end

        puts output.gsub(/^/, i(n)).colorize(color)
      end
    end

    # Returns two spaces for every level of depth of the node in the tree.
    # Used for indenting output.
    def i(node)
      # I'm not spending the time to figure out why node_depth is off-by-one
      # (seemingly) when running a tree test.
      num = $TREE_TEST ? node.node_depth - 2 : node.node_depth - 1
      '  ' * num
    end
  end
end
