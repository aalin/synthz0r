files = Dir[File.join(File.dirname(__FILE__), "waveforms", "*.wav")]

File.open("waveform.cpp", "w") do |f|
  f.puts '#include "waveform.hpp"'
  f.puts
  f.print "const std::vector<Waveform> Waveform::WAVEFORMS = {"

  f.print files.map { |filename|
    File.open(filename, encoding: "binary") do |f|
      header = Hash[*
        [:riff_tag, :filesize, :wave_tag, :fmt_tag, :fmt_length, :audio_format, :num_channels, :sample_rate, :byte_rate, :block_align, :bits_per_sample, :data_tag, :data_length].zip(
          f.read(44).unpack("a4 l< a4 a4 l< s< s< l< l< s< s< a4 l<")
        ).flatten
      ]

      if header[:num_channels] > 1
        puts "Too many channels, skipping..."
        next
      end

      values = f.read(header[:data_length]).unpack("s<*")
      data = values.each_slice(8).map { |shorts|
        "\t\t" + shorts.map { |short|
          sign = short < 0 ? "-" : " "
          "#{sign}0x#{short.abs.to_s(16).rjust(4, "0")}"
        }.join(",")
      }.join(",\n")

    "{
\t.name = \"#{File.basename(filename, ".*")}\",
\t.sampleRate = #{header[:sample_rate]},
\t.data = {
#{data}
\t}
}"
    end
  }.join(", ")

  f.puts "};"
end
