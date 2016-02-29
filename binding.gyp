{
  "targets": [
    {
      "target_name": "histogram",
      "sources": [
        "src/hdr_encoding.h",
        "src/hdr_encoding.c",
        "src/hdr_histogram.h",
        "src/hdr_histogram.c",
        "src/hdr_histogram_log.h",
        "src/hdr_histogram_log.c",
        "src/hdr_interval_recorder.h",
        "src/hdr_interval_recorder.c",
        "src/hdr_writer_reader_phaser.h",
        "src/hdr_writer_reader_phaser.c",
        "hdr_histogram_wrap.cc",
        "histogram.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "src/"
      ],
      "conditions": [
        ["OS == \"linux\"", {
          "ccflags": [ "-std=c99" ]
        }]
      ]
    }
  ]
}
