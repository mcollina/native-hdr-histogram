#include <napi.h>
#include "hdr_histogram_wrap.h"

extern "C" {
#include "hdr_histogram.h"
#include "hdr_histogram_log.h"
}

Napi::FunctionReference HdrHistogramWrap::constructor;

void HdrHistogramWrap::Init(Napi::Env env, Napi::Object target) {
  Napi::HandleScope scope(env);

  Napi::Function ctor = DefineClass(env, "HdrHistogram", {
    InstanceMethod("record", &HdrHistogramWrap::Record),
    InstanceMethod("min", &HdrHistogramWrap::Min),
    InstanceMethod("max", &HdrHistogramWrap::Max),
    InstanceMethod("mean", &HdrHistogramWrap::Mean),
    InstanceMethod("stddev", &HdrHistogramWrap::Stddev),
    InstanceMethod("percentile", &HdrHistogramWrap::Percentile),
    InstanceMethod("encode", &HdrHistogramWrap::Encode),
    InstanceMethod("percentiles", &HdrHistogramWrap::Percentiles),
    InstanceMethod("reset", &HdrHistogramWrap::Reset),
    StaticMethod("decode", &HdrHistogramWrap::Decode)
  });

  constructor = Napi::Persistent(ctor);
  constructor.SuppressDestruct();

  target.Set("HdrHistogram", ctor);
}

HdrHistogramWrap::HdrHistogramWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<HdrHistogramWrap>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int64_t lowest = info[0].IsUndefined() ? 1 : info[0].As<Napi::Number>().Int64Value();
  int64_t highest = info[1].IsUndefined() ? 100 : info[1].As<Napi::Number>().Int64Value();
  int significant_figures = info[2].IsUndefined() ? 3 : info[2].As<Napi::Number>().Int32Value();

  if (lowest <= 0) {
    throw Napi::Error::New(env, "The lowest trackable number must be greater than 0");
  }

  if (significant_figures < 1 || significant_figures > 5) {
    throw Napi::Error::New(env, "The significant figures must be between 1 and 5 (inclusive)");
  }

  int init_result = hdr_init(
      lowest,
      highest,
      significant_figures,
      &this->histogram);

  if (init_result != 0) {
    throw Napi::Error::New(env, "Unable to initialize the Histogram");
  }
}

HdrHistogramWrap::~HdrHistogramWrap() {
  if (this->histogram) {
    delete this->histogram;
  }
}

Napi::Value HdrHistogramWrap::Record(const Napi::CallbackInfo& info) {
  int64_t value;
  HdrHistogramWrap* obj = this;

  if (info[0].IsUndefined()) {
    return Napi::Boolean::New(info.Env(), false);
  }

  value = info[0].As<Napi::Number>().Int64Value();
  bool result = hdr_record_value(obj->histogram, value);
  return Napi::Boolean::New(info.Env(), result);
}

Napi::Value HdrHistogramWrap::Min(const Napi::CallbackInfo& info) {
  HdrHistogramWrap* obj = this;
  int64_t value = hdr_min(obj->histogram);
  return Napi::Number::New(info.Env(), (double) value);
}

Napi::Value HdrHistogramWrap::Max(const Napi::CallbackInfo& info) {
  HdrHistogramWrap* obj = this;
  int64_t value = hdr_max(obj->histogram);
  return Napi::Number::New(info.Env(), (double) value);
}

Napi::Value HdrHistogramWrap::Mean(const Napi::CallbackInfo& info) {
  HdrHistogramWrap* obj = this;
  double value = hdr_mean(obj->histogram);
  return Napi::Number::New(info.Env(), value);
}

Napi::Value HdrHistogramWrap::Stddev(const Napi::CallbackInfo& info) {
  HdrHistogramWrap* obj = this;
  double value = hdr_stddev(obj->histogram);
  return Napi::Number::New(info.Env(), value);
}

Napi::Value HdrHistogramWrap::Percentile(const Napi::CallbackInfo& info) {
  if (info[0].IsUndefined()) {
    Napi::Error::New(info.Env(), "No percentile specified").ThrowAsJavaScriptException();
  }

  double percentile = info[0].As<Napi::Number>().DoubleValue();

  if (percentile <= 0.0 || percentile > 100.0) {
    Napi::Error::New(info.Env(), "percentile must be > 0 and <= 100").ThrowAsJavaScriptException();
  }

  HdrHistogramWrap* obj = this;
  double value = hdr_value_at_percentile(obj->histogram, percentile);
  return Napi::Number::New(info.Env(), value);
}

Napi::Value HdrHistogramWrap::Encode(const Napi::CallbackInfo& info) {
  HdrHistogramWrap* obj = this;
  char *encoded;
  int result = hdr_log_encode(obj->histogram, &encoded);
  if (result != 0) {
    Napi::Error::New(info.Env(), "failed to encode").ThrowAsJavaScriptException();
  }
  int len = strlen(encoded);
  return Napi::Buffer<char>::New(info.Env(), encoded, len);
}

Napi::Value HdrHistogramWrap::Decode(const Napi::CallbackInfo& info) {
  Napi::Value buf;
  if (info.Length() > 0 && info[0].IsObject(), info[0].IsBuffer()) {
    buf = info[0];
  } else {
    throw Napi::Error::New(info.Env(), "Missing Buffer");
  }

  char *encoded = buf.As<Napi::Buffer<char>>().Data();
  size_t len  = buf.As<Napi::Buffer<char>>().Length();

  Napi::Object wrap = constructor.New(std::initializer_list<napi_value>{});
  HdrHistogramWrap* obj = HdrHistogramWrap::Unwrap(wrap);

  hdr_log_decode(&obj->histogram, encoded, len);

  return wrap;
}

Napi::Value HdrHistogramWrap::Percentiles(const Napi::CallbackInfo& info) {
  HdrHistogramWrap* obj = this;
  Napi::Env env = info.Env();
  Napi::Array result = Napi::Array::New(env);

  hdr_iter iter;
  hdr_iter_percentile_init(&iter, obj->histogram, 1);

  int count = 0;

  while(hdr_iter_next(&iter)) {
    Napi::Object percentile = Napi::Object::New(env);

    percentile.Set(Napi::String::New(env, "percentile"),
        Napi::Number::New(env, iter.specifics.percentiles.percentile));

    percentile.Set(Napi::String::New(env, "value"),
        Napi::Number::New(env, (double) iter.value));

    result.Set(count++, percentile);
  }

  return result;
}

Napi::Value HdrHistogramWrap::Reset(const Napi::CallbackInfo& info) {
  HdrHistogramWrap* obj = this;
  hdr_reset(obj->histogram);
  return info.This();
}
