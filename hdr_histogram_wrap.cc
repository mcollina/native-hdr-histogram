#include <nan.h>
#include "hdr_histogram_wrap.h"

extern "C" {
#include "hdr_histogram.h"
}

Nan::Persistent<v8::Function> HdrHistogramWrap::constructor;

NAN_MODULE_INIT(HdrHistogramWrap::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("HdrHistogram").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "record", Record);
  Nan::SetPrototypeMethod(tpl, "min", Min);
  Nan::SetPrototypeMethod(tpl, "max", Max);
  Nan::SetPrototypeMethod(tpl, "mean", Mean);
  Nan::SetPrototypeMethod(tpl, "stddev", Stddev);
  Nan::SetPrototypeMethod(tpl, "percentile", Percentile);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("HdrHistogram").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

HdrHistogramWrap::HdrHistogramWrap(
    int64_t lowest_trackable_value,
    int64_t highest_trackable_value,
    int significant_figures) {

  hdr_init(
      lowest_trackable_value,
      highest_trackable_value,
      significant_figures,
      &this->histogram);
}

HdrHistogramWrap::~HdrHistogramWrap() {
  delete this->histogram;
}

NAN_METHOD(HdrHistogramWrap::New) {
  if (info.IsConstructCall()) {
    int64_t lowest = info[0]->IsUndefined() ? 1 : Nan::To<int64_t>(info[0]).FromJust();
    int64_t highest = info[1]->IsUndefined() ? 100 : Nan::To<int64_t>(info[1]).FromJust();
    int significant_figures = info[2]->IsUndefined() ? 3 : Nan::To<int>(info[2]).FromJust();

    HdrHistogramWrap *obj = new HdrHistogramWrap(lowest, highest, significant_figures);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 3;
    v8::Local<v8::Value> argv[argc] = {
      info[0],
      info[1],
      info[2]
    };
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

NAN_METHOD(HdrHistogramWrap::Record) {
  int64_t value;
  HdrHistogramWrap* obj = Nan::ObjectWrap::Unwrap<HdrHistogramWrap>(info.This());

  if (info[0]->IsUndefined()) {
    info.GetReturnValue().Set(false);
    return;
  }

  value = Nan::To<int64_t>(info[0]).FromJust();
  bool result = hdr_record_value(obj->histogram, value);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(HdrHistogramWrap::Min) {
  HdrHistogramWrap* obj = Nan::ObjectWrap::Unwrap<HdrHistogramWrap>(info.This());
  int64_t value = hdr_min(obj->histogram);
  info.GetReturnValue().Set((int32_t) value);
}

NAN_METHOD(HdrHistogramWrap::Max) {
  HdrHistogramWrap* obj = Nan::ObjectWrap::Unwrap<HdrHistogramWrap>(info.This());
  int64_t value = hdr_max(obj->histogram);
  info.GetReturnValue().Set((int32_t) value);
}

NAN_METHOD(HdrHistogramWrap::Mean) {
  HdrHistogramWrap* obj = Nan::ObjectWrap::Unwrap<HdrHistogramWrap>(info.This());
  double value = hdr_mean(obj->histogram);
  info.GetReturnValue().Set(value);
}

NAN_METHOD(HdrHistogramWrap::Stddev) {
  HdrHistogramWrap* obj = Nan::ObjectWrap::Unwrap<HdrHistogramWrap>(info.This());
  double value = hdr_stddev(obj->histogram);
  info.GetReturnValue().Set(value);
}

NAN_METHOD(HdrHistogramWrap::Percentile) {
  double percentile;

  if (info[0]->IsUndefined()) {
    // TODO maybe throw?
    return;
  }

  percentile = Nan::To<double>(info[0]).FromJust();
  HdrHistogramWrap* obj = Nan::ObjectWrap::Unwrap<HdrHistogramWrap>(info.This());
  double value = hdr_value_at_percentile(obj->histogram, percentile);
  info.GetReturnValue().Set(value);
}
