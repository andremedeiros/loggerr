require 'spec_helper'

describe Loggerr do
  describe '.parse' do
    it 'throws an exception when an invalid file is passed' do
      expect { Loggerr.parse('/something/i/just/made/up') }.to raise_error
    end

    it 'returns a hash when the file is found' do
      expect( Loggerr.parse('spec/fixtures/log.txt') ).to be_a Hash
    end

    it 'gets the information right for a change' do
      a = Loggerr.parse('spec/fixtures/log.txt')

      expect( a["errors"] ).to eq 1
      expect( a["max"]    ).to eq 0.8214
      expect( a["min"]    ).to eq 0.155
    end
  end
end
