class Parser
  def initialize(filename)
    @fn = filename
    @in = IO.readlines(filename).map do |line|                    # read each line and format to array
      line = line.match( /(\S+),\S+\t(\d+)\t(\S+)?/ ).captures     # match the end of the line "(host),3.9.2	(1)	(00)" => ["host", "0", "00"]
    end
    @out = []
  end # initialize

  def parse
    print "Converted #{@in.length} lines "
    @in.delete_if do |line|                                       # remove line if null packet from device
      line[1] == "0"
    end # delete_if
    @in = @in.chunk{ |line| line[0] }.map{ |sender, bytes| [sender,bytes.map(&:last)] }
    @in.each_with_index do |data,idx|
      sender, bytes = data
      while(sender == "host" && bytes.length % 7 != 0)
        bytes << @in[idx+2][1].shift
      end
      bytes.each_slice(7) do |word|
        word = word.join(":")
        @out << [sender,word]
      end
    end  # each
    puts "into #{@out.length} packets"
    return self
  end # parse

  def fout
    File.open(@fn+".parse","w") do |file|
      @out.each do |sender,word|
        file.puts "#{sender}\t#{word}"
      end # each
    end # File.open
  end # fout
end # class Parser

if __FILE__ == $0
  Parser.new(ARGV.shift).parse.fout
end
