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
        "hdr_histogram_wrap.cc",
        "histogram.cc"
      ],
      "dependencies": [
        "<(module_root_dir)/zlib/zlib.gyp:zlib"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "src/"
      ]
    }
  ]
}
