require 'time'
require 'serialport'

class Parser
  def initialize(filename, outfilename, options)
    # incoming filename
    @opts = options
    @outfilename = outfilename || filename + ".parse"
    @outfilename = STDOUT if outfilename == '-'
    # grab the file, read the lines, convert that into a hash of data
    @in = IO.readlines(filename).map do |line|                    # read each line and format to array
      # match groups:   1: first digit 2: date and time 3: time           4: sender   5: size  6:value
      m = line.match( /(\d+)\t([^,]+, \d\d\d\d (\d\d:\d\d:\d\d\.\d+) \w+)\t(\S+),\S+\t(\d+)\t(\S+)?/ ).captures
      line = {  bitnum:   [m[0].to_i]*2,      # bitnum => [0,0] or [307,307]
                date:     Time.parse(m[1]),
                time:     Time.parse(m[2]),
                sender:   m[3],
                size:     m[4].to_i,
                contents: m[5]
              }
    end
    @out = []
  end # initialize

  def parse
    print "Converted #{@in.length} lines " unless @outfilename == STDOUT
    host = { sender: 'host', size: 0, contents: [], bitnum: [] }
    @in.each_with_index do |packet,idx|
      next if packet[:size] == 0              # first, remove all null packets
      if packet[:sender] == 'host'            # group host packets into groups of 7
        if host[:size] == 0                   # new packet, use this timestamp
          host[:date] = packet[:date]
          host[:time] = packet[:time]
        end
        host[:size] += packet[:size]
        host[:contents] << packet[:contents]
        host[:bitnum] << packet[:bitnum][0]   # add the bitnum to the group
        if host[:size] >= 7                   # if host is complete (7 bytes)
          host[:contents] = host[:contents].join(":") # join the contents to one string
          host[:bitnum] = host[:bitnum].minmax        # set bitnum to the largest and smallest bitnums
          @out << host
          host = { sender: 'host', size: 0, contents: [], bitnum: [] }
        end
      else                                    # print all other packets as they appear
        @out << packet
      end # packet[:sender]
    end # each
    puts "into #{@out.length} packets" unless @outfilename == STDOUT
    return self
  end # parse


  def hexout
    if @outfilename == STDOUT
      file = STDOUT
    elsif @outfilename =~ /^\/dev\// # serial mode
      baud_rate = 115200
      data_bits = 8
      stop_bits = 1
      parity = SerialPort::NONE
      
      file = SerialPort.new(@outfilename, baud_rate, data_bits, stop_bits, parity)
    else
      puts "Saving to #{@outfilename}"
      file = File.open(@outfilename,'w')
    end
    @out.each do |packet|
      if packet[:sender] == 'host'
        packet[:contents].split(":").map do |byte|
          file.write byte.to_i(16).chr
          sleep(0.007) # how to improve?
        end
        if packet[:contents] =~ /^15:/ # start run
          sleep(2) # this could be shorter
        end
      end
    end # @out.each
    file.close
  end # fout

  def fout
    if @outfilename == STDOUT
      file = STDOUT
    else
      puts "Saving to #{@outfilename}"
      file = File.open(@outfilename,'w')
    end
    last_time = @out.first[:time]
    @out.each do |packet|
      if @opts['bitnum']
        file.print "#{packet[:bitnum][0].to_s.rjust(4,'0')}-#{packet[:bitnum][1].to_s.rjust(4,'0')}\t"
      end # bitnum

      if @opts['date']
        file.print "#{packet[:date].strftime("%b %_m, %Y %T.%9N")}\t"
      else
        if @opts['time']
          file.print "#{packet[:time].strftime("%T.%9N")}\t"
        else
          if @opts['timediff']
            file.print "#{(packet[:time] - last_time).to_s.ljust(8,'0') }\t"
            if packet[:sender] == 'host'
              last_time = packet[:time]
            end
          end
        end
      end # date
      file.puts "#{packet[:sender]}\t#{packet[:contents]}"
    end # @out.each
    file.close
  end # fout

  def self.help
    puts "ruby parser.rb [options] filename\n\
	-c       	add the first digit from the raw input\n\
	-d       	add the date and time to the output (implies -t)\n\
	-o output	change the output file (default: filename.parse) (- for STDOUT)\n\
	-t       	add the timestamp to the output\n\
	-ti     	add the difference between timestamps\n"
  end # help
end # class Parser


# this rountine sets
# bitnum (bool) to print bitnum
# date (bool) to print date and time
# time (bool) to print just time
# outfilename to set the output location
# filename to set the input location
if __FILE__ == $0
  filename = nil
  outfilename = nil
  opts = Hash.new
  hex = false
  while( arg = ARGV.shift )
    case(arg)
    when('-c') # print bitnum
      opts['bitnum'] = true

    when('-d') # print date and time
      opts['date'] = true

    when('-ti')
      opts['timediff'] = true

    when('-t') # print only time
      opts['time'] = true

    when('-o') # change file output location
      outfilename = ARGV.shift
      if !outfilename
        STDERR.puts "No output filename given with -o argument."
        Parser.help
        exit(1)
      end

    when('-x') # hex mode!
      hex = true
    when('--hex')
      hex = true

    when('-h') # ask for help menu
      Parser.help
      exit(0)
    when('--help')
      Parser.help
      exit(0)

    when(/-./) # not understood, error
      STDERR.puts "#{arg} not understood"
      Parser.help
      exit(1)

    else # a filename
      if filename
        STDERR.puts "Ignoring file: #{filename}"
      end
      filename = arg
    end # case(arg)
  end # while args
  if filename # was a file given?
    if hex
      Parser.new(filename, outfilename, opts).parse.hexout
    else
      Parser.new(filename, outfilename, opts).parse.fout
    end
  else
    STDERR.puts "No file given."
    Parser.help
    exit(1)
  end
end
