require "spec_helper"

include World

describe World::Scribe do

  #############################################################################
  describe "#save" do

    around :each do |example|
      @id = SecureRandom.uuid
      @ob = TestObj.new @id
      FakeFS do
        example.run
        FakeFS::FileSystem.clear # https://github.com/fakefs/fakefs/issues/343
      end
    end

    it "creates the parent directory" do
      parent_dir = File.dirname Scribe.pathname TestObj, @id
      expect(File.directory? parent_dir).to be false
      Scribe.save @ob
      expect(File.directory? parent_dir).to be true
    end

    it "creates the data file" do
      data_file = Scribe.pathname TestObj, @id
      expect(File.readable? data_file).to be false
      Scribe.save @ob
      expect(File.readable? data_file).to be true
    end

  end

end
