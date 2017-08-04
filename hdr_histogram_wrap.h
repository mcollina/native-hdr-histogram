#ifndef HDRHISTOGRAMWRAP_H
#define HDRHISTOGRAMWRAP_H

#include <napi.h>
#include <uv.h>

extern "C" {
#include "hdr_histogram.h"
}

class HdrHistogramWrap : public Napi::ObjectWrap<HdrHistogramWrap> {
 public:
  static void Init(Napi::Env env, Napi::Object target);

  HdrHistogramWrap(const Napi::CallbackInfo& info);
  ~HdrHistogramWrap();

  Napi::Value Record(const Napi::CallbackInfo& info);
  Napi::Value Min(const Napi::CallbackInfo& info);
  Napi::Value Max(const Napi::CallbackInfo& info);
  Napi::Value Mean(const Napi::CallbackInfo& info);
  Napi::Value Stddev(const Napi::CallbackInfo& info);
  Napi::Value Percentile(const Napi::CallbackInfo& info);
  Napi::Value Encode(const Napi::CallbackInfo& info);
  Napi::Value Percentiles(const Napi::CallbackInfo& info);
  Napi::Value Reset(const Napi::CallbackInfo& info);

  static Napi::Value Decode(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  struct hdr_histogram *histogram;
};

#endif
