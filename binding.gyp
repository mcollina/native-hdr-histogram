{
  "targets": [
    {
      "target_name": "native-hdr-histogram",
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
      'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "src/"
      ],
      "dependencies": [
        "<(module_root_dir)/zlib/zlib.gyp:zlib",
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7',
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      }
    }
  ]
}
