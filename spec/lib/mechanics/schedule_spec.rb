require 'spec_helper'

include Mechanics

describe Schedule do
  before :each do
    @schedule = Schedule.new
    @tester_a = JobTester.new
    @tester_b = JobTester.new
    @tester_c = JobTester.new

    @job_a    = Job.new 2, @tester_a, :call_me_maybe
    @job_b    = Job.new 1, @tester_b, :call_me_maybe
    @job_c    = Job.new 3, @tester_c, :call_me_maybe
    @jobs = [@job_a, @job_b, @job_c]

    @docket = @schedule.instance_variable_get '@docket'
  end

  #############################################################################
  describe '#add' do
    it 'should prioritize Jobs needing to occur first' do
      @jobs.each { |j| @schedule.add j }
      expect(@docket.top).to be @job_b
    end
  end

  #############################################################################
  describe '#fire!' do
    it 'should return nil if the queue is empty' do
      expect(@schedule.fire!).to be_nil
    end

    it 'should return nil if the next Job is not ready' do
      @job_a.instance_variable_set '@wen', Time.now.to_i + 100
      @schedule.add @job_a
      expect(@schedule.fire!).to be_nil
    end

    it 'should not pop the next Job if it is not ready' do
      @job_a.instance_variable_set '@wen', Time.now.to_i + 100
      @schedule.add @job_a
      @schedule.fire!
      expect(@docket.top).to be @job_a
    end

    it 'should return false if the next Job is dead' do
      @job_a.instance_variable_set '@wen', Time.now.to_i + 100
      @schedule.add @job_a
      @job_a.kill
      expect(@schedule.fire!).to eq false
    end

    it 'should pop the next Job if it is dead' do
      @job_a.instance_variable_set '@wen', Time.now.to_i + 100
      @schedule.add @job_a
      @job_a.kill
      @schedule.fire!
      expect(@docket.top).not_to be @job_a
    end

    it 'should return true if the next Job is ready' do
      @job_a.instance_variable_set '@wen', Time.now.to_i - 100
      @schedule.add @job_a
      expect(@schedule.fire!).to eq true
    end

    it 'should fire the next Job if it is ready' do
      @job_a.instance_variable_set '@wen', Time.now.to_i - 100
      @schedule.add @job_a
      expect(@tester_a.x).to eq 0
      @schedule.fire!
      expect(@tester_a.x).to eq 10
    end

    it 'should pop the next Job if it is ready' do
      @job_a.instance_variable_set '@wen', Time.now.to_i - 100
      @schedule.add @job_a
      @schedule.fire!
      expect(@docket.top).not_to be @job_a
    end

    context 'with a recurring Job' do
      before :each do
        @repeat_tester = JobTester.new
        @repeat_job    = Job.new 0, @repeat_tester, :call_me_maybe
      end

      it 'should re-add the Job after firing' do
        @repeat_job.recur
        expect(@docket).to be_empty
        @schedule.add @repeat_job

        expect(@docket.top).to be @repeat_job
        expect(@schedule.fire!).to eq true
        expect(@docket.top).to be @repeat_job
      end

      it 'should not re-add the Job if the Job is dead' do
        @repeat_job.recur
        @schedule.add @repeat_job
        expect(@docket.top).to be @repeat_job

        @repeat_job.kill
        @schedule.fire!
        expect(@docket).to be_empty
      end

      it 'should recur the Job a set number of times' do
        count = 0
        times = 5

        @repeat_job.recur 0, times
        @schedule.add @repeat_job

        count += 1 until @schedule.fire!.nil?

        expect(count).to eq times
      end

      it 'should recur the Job infinitely' do
        count = 1

        @repeat_job.recur 0, -1
        @schedule.add @repeat_job

        until count > 20
          expect(@schedule.fire!).to eq true
          expect(@docket.top).to be @repeat_job
          expect(@repeat_job.repeat_times).to eq(-1)
          count += 1
        end
      end

      it 'should recur the Job after the set delay' do
      end
    end
  end
end
