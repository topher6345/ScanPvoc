#!/usr/bin/env ruby

require 'fileutils'

timestamp = Time.now.to_i
pvx_filename = "toss-#{timestamp}.pvx"

if File.exist?(input_filename = ARGV.first.to_s)
  system <<-SHELL
  pvanal -s 44100 #{input_filename} #{pvx_filename} 2>/dev/null
SHELL
else
  puts 'usage: $ scanpvoc inputfilename'
  exit(1)
end

permutation = ([', 1'] * rand(0..3)).join

csd = <<~CSOUND
  <CsoundSynthesizer>
  <CsInstruments>
  sr=44100
  ksmps=10
  nchnls=2
  0dbfs=1
  #define FILE #"#{pvx_filename}"#
  git1 ftgen 1, 0, 128, 10, 1 #{permutation}
  git11 ftgen 11, 0, 128, 10, 1
  git2 ftgen 2, 0, 128, -7,  1, 128,  .5
  git3 ftgen 3, 0, 128, -7, 0, 128, 1
  git4 ftgen 4, 0, 128, -7, 1, 128, 1
  git5 ftgen 5, 0, 128, -7, 0, 128, 0
  instr cntrl
  Sfile = $FILE.
  gilen filelen Sfile
  print gilen
  event_i "i", "scanpvoc", 0, gilen
  endin
  instr scanpvoc
  ipos = 1
  imass = 2
  istiff = 3
  istiff2 = 33
  idamp = 4
  ivel = 5
  a0 scantable 1, 440, ipos, imass, istiff, idamp, ivel
  kphase phasor 1/gilen
  klfo tablei kphase, 1
  chnset kphase, "lfo"
  ktime = ((klfo*.5+.5) )*gilen+.1
  if ktime >0 then
  ares pvoc  ktime , 1, $FILE., 0
  outs ares,ares
  endif
  endin
  </CsInstruments>
  <CsScore>
  i 1 0 10
  </CsScore>
  </CsoundSynthesizer>
CSOUND

csd_filename = "toss-#{timestamp}.csd"
File.write(csd_filename, csd)

outpu_audio_filename = "_scanpvoc-#{ARGV.first}-#{timestamp}.aiff"

system <<-SHELL
  csound -f -d -A -o #{outpu_audio_filename} #{csd_filename}  2>/dev/null 
SHELL

FileUtils.rm([pvx_filename, csd_filename])
