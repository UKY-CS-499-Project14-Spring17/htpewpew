#!/usr/bin/env ruby
require 'serialport'

class DummyUSB
  def initialize( filename )
    baud_rate = 115200
    data_bits = 8
    stop_bits = 1
    parity    = SerialPort::NONE
    @filename = filename
    @file     = SerialPort.new(filename, baud_rate, data_bits, stop_bits, parity)
  end

  def convert(string)
    new_string = []
    string.each_byte { |c| new_string << c.to_s(16) }
    return new_string.join(":")
  end

  def respond
    @file.write("....")
  end

  def listen
    line = ""
    size = 7
    while( newline = @file.readline(size) )
      line << newline
      if line.bytesize == 7
        newline = line
        line    = ""
        size    = 7
      elsif line.bytesize < 7
        size    = (7 - line.bytesize)
        next
      else
        newline = line.byteslice(0,7)
        line    = line.byteslice(7..-1)
        size    = 1 # grab as little as possible
      end
      puts "Recieved: #{convert(newline)}"
      char = newline.chr.ord
      if char == 0x15 || char == 0x3d || char == 0x5b
        respond
        puts "Responding!"
      elsif char == 0x1a
        response = [0x3e,0x07,0xff,0xff,
        0x3e,0x28,0xf4,0x69,0x00,0xd0,0x04,0xde,0xcb,0xff,0xff,
        0x3e,0x29,0x17,0x30,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
        0x3e,0x2a,0x5e,0x62,0x01,0x00,0x00,0x00,0x00,0xff,0xff,
        0x3e,0x2b,0x43,0x02,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
        0x3e,0x2c,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
        0x3e,0x2d,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff]
        @file.write( response.map(&:chr).join )
        puts "Handshaking"
      elsif newline.chars[1..5].all?{ |b| b.chr.ord == 0x09 }
        puts "Done reading!"
        return
      end
    end
  end

  def terminate
    @file.close
  end
end

if __FILE__ == $0 # if this is run directly
  usb = DummyUSB.new( ARGV.shift )
  usb.listen
  usb.terminate
end
