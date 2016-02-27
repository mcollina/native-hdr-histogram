#ifndef HDRHISTOGRAMWRAP_H
#define HDRHISTOGRAMWRAP_H

#include <nan.h>

extern "C" {
#include "hdr_histogram.h"
}

class HdrHistogramWrap : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  HdrHistogramWrap(
    int64_t lowest_trackable_value,
    int64_t highest_trackable_value,
    int significant_figures);
  ~HdrHistogramWrap();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Record(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Min(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Max(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static Nan::Persistent<v8::Function> constructor;

  struct hdr_histogram *histogram;
};

#endif
