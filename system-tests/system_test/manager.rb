require 'rubytree'

module SystemTest
  class Manager

    ###########################################################################
    def initialize
      @test_tree = Tree::TreeNode.new :root, nil
      @reg = Configuration.configuration.test_registry

      setup
    end

    ###########################################################################
    def setup
      until @reg.empty? do
        analyze @reg[0]
      end
    end

    ###########################################################################
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
      return new_node
    end

    ###########################################################################
    def start
      @test_tree.breadth_each { |n| Micromanager.new n }
    end

  end
end

