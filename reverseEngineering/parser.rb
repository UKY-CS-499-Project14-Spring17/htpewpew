class Parser
  def initialize(filename)
    @fn = filename
    @in = IO.readlines(filename).map do |line|                    # read each line and format to array
      line = line.match( /(\d+:\d+:\d+\.\d+)\s\S+\s(\S+),\S+\t(\d+)\t(\S+)?/ ).captures     # match the end of the line "(host),3.9.2	(1)	(00)" => ["host", "0", "00"]
    end
    @out = []
  end # initialize

  def parse
    print "Converted #{@in.length} lines "
    @in.delete_if do |line|                                       # remove line if null packet from device
      line[2] == "0"
    end # delete_if
    @in = @in.chunk{ |line| line[1] }.map{ |sender, bytes| [bytes.first.first,sender,bytes.map(&:last)] } # time sender bytes
    @in.each_with_index do |data,idx|
      time, sender, bytes = data
      while(sender == "host" && bytes.length % 7 != 0)
        bytes << @in[idx+2][2].shift
      end
      bytes.each_slice(7) do |word|
        word = word.join(":")
        @out << [time,sender,word]
      end
    end  # each
    puts "into #{@out.length} packets"
    return self
  end # parse

  def fout
    File.open(@fn+".parse","w") do |file|
      @out.each do |time,sender,word|
        file.puts "#{time}\t#{sender}\t#{word}"
      end # each
    end # File.open
  end # fout
end # class Parser

if __FILE__ == $0
  Parser.new(ARGV.shift).parse.fout
end
