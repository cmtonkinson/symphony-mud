require 'spec_helper'

include Mechanics

describe Job do

  #############################################################################
  describe "#initialize" do

    it "should require either a method name or a Proc" do
      expect { Job.new(1, OpenStruct.new) }.to raise_error(ArgumentError, /method or proc/)
    end

    it "should reject both a method name and a Proc" do
      expect { Job.new(1, OpenStruct.new, :meth) { |_| } }.to raise_error(ArgumentError, /method and proc/)
    end

    it "should reject a method name that can't be symbolized" do
      expect { Job.new(1, OpenStruct.new, {}) }.to raise_error(ArgumentError, /not respond/)
    end

    it "should assume low wen values are 'in the future'" do
      expect(Job.new(1, OpenStruct.new, :meth).wen).to eq Time.now.to_i + 1
      expect(Job.new(1_470_638_651, OpenStruct.new, :meth).wen).to eq 1_470_638_651
    end

    it "should add itself to an Objekt's job list" do
      o = Objekt.new
      expect(o.jobs.empty?).to be true
      j = Job.new 1, o, :meth
      expect(o.jobs).to contain_exactly j
    end

  end

  #############################################################################
  describe "#ready?" do

    before :each do
      @job = Job.new 1, OpenStruct.new, :method
    end

    it "should return false for future values of wen" do
      @job.instance_variable_set "@wen", Time.now.to_i + 10
      expect(@job.ready?).to eq false
    end

    it "should return true for present values of wen" do
      @job.instance_variable_set "@wen", Time.now.to_i
      expect(@job.ready?).to eq true
    end

    it "should return true for past values of wen" do
      @job.instance_variable_set "@wen", Time.now.to_i - 10
      expect(@job.ready?).to eq true
    end

  end

  #############################################################################
  describe "#fire" do

    before :each do
      @job_tester = JobTester.new
      expect(@job_tester.x).to eq 0
    end

    it "should invoke the callable if passed as a method name" do
      job = Job.new 0, @job_tester, :call_me_maybe
      job.fire
      expect(@job_tester.x).to eq 10
    end

    it "should invoke the callable if passed as a Proc" do
      job = Job.new 0, @job_tester do |tester|
        tester.call_me_maybe
      end
      job.fire
      expect(@job_tester.x).to eq 10
    end

    it "should invoke the callable if passed as a Lambda" do
      job = Job.new 0, @job_tester, -> tester { tester.call_me_maybe }
      job.fire
      expect(@job_tester.x).to eq 10
    end

    it "should no-op if the Objekt has been disposed" do
      ob  = Objekt.new
      job = Job.new 0, ob, :meth
      ob.dispose
      expect(job.dead?).to eq true
    end

  end

  #############################################################################
  describe "#kill" do

    it "should mark the Job as dead" do
      job = Job.new 0, OpenStruct.new, :meth

      expect(job.who).not_to be_nil
      expect(job.what).not_to be_nil
      expect(job.wen).not_to eq 0
      expect(job.dead?).to eq false

      job.kill
      expect(job.who).to be_nil
      expect(job.what).to be_nil
      expect(job.wen).to eq 0
      expect(job.dead?).to eq true
    end
  end

end
