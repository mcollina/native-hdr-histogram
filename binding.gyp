{
  "targets": [
    {
      "target_name": "<(module_name)",
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
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
