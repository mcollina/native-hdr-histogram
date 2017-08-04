#include <napi.h>
#include <uv.h>
#include "hdr_histogram_wrap.h"

void InitAll(Napi::Env env, Napi::Object exports, Napi::Object module) {
  HdrHistogramWrap::Init(env, exports);
}

NODE_API_MODULE(Histogram, InitAll)
