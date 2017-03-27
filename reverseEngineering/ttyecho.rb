#!/usr/local/bin/ruby

raw_bytes = ARGV.join(' ')

if ARGV[0] == "-h" || ARGV.empty?
  puts "USAGE: #{__FILE__} '<hex data>'"
  puts "example: #{__FILE__} '00 0a 12 bc'"
  exit
end

bytes = raw_bytes.split(" ")

system("echo","-ne", "\\x#{bytes.join('\x')}")
