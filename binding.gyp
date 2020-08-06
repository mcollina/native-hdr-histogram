{
  "targets": [
    {
      "target_name": "<(module_name)",
      "sources": [
        "binding-src/binding.cc",
        "<!@(node -p \"require('fs').readdirSync('./binding-src/binding-util').map(f=>'binding-src/binding-util/'+f).join(' ')\")",
        "<!@(node -p \"require('fs').readdirSync('./binding-src/binding').map(f=>'binding-src/binding/'+f).join(' ')\")",
        "<!@(node -p \"require('fs').readdirSync('./HdrHistogram_c/src').map(f=>'HdrHistogram_c/src/'+f).join(' ')\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")",
        "<(module_root_dir)/zlib/zlib.gyp:zlib"
      ],
      "include_dirs": [
        "binding-src/binding-util",
        "HdrHistogram_c/src",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "defines": [
        "NAPI_VERSION=<(napi_build_version)"
      ],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1
        }
      }
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
