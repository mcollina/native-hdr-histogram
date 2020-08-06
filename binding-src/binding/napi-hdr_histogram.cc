// 2020-07-08T13:10:42.357-07:00 binding hdr_histogram.c (GenerateDefinitions)
// © Copyright 2020 Simply Inspired Software, Inc., dba inspiredware
// Released under the MIT License located in the root directory of this repository
// Created by the inspiredware automated binding generator — https://inspiredware.com

#include "napi-hdr_histogram.h"
#include "napi-ingress.h"
#include "hdr_histogram_log.h"

extern "C" {
}
// pseudoClass HdrHistogram definition begin //

HdrHistogram::HdrHistogram (const Napi::CallbackInfo& info)
: Napi::ObjectWrap<HdrHistogram> (info)
, histogram (NULL)
{
  Napi::Env env = info.Env();
  long long min = getInt64<long long> (info[0]);
  long long max = getInt64<long long> (info[1]);
  int sigFigures = info[2].IsUndefined() ? 3 : getInt32<int> (info[2]);
  int status = hdr_init (min, max, sigFigures, &histogram);
  if (status) {
    Napi::Error::New(env, "hdr_init failed.").ThrowAsJavaScriptException();
  }
}

HdrHistogram::~HdrHistogram () {
  hdr_close (histogram); // frees memory
}

Napi::Value HdrHistogram::recordCorrectedValue (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value = getInt64<long long> (info[0]);
  long long expected_interval = getInt64<long long> (info[1]);
  _Bool retVal = hdr_record_corrected_value (histogram, value, expected_interval);
  Napi::Value jsRetVal = Boolean::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::recordCorrectedValues (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value = getInt64<long long> (info[0]);
  long long count = getInt64<long long> (info[1]);
  long long expected_interval = getInt64<long long> (info[2]);
  _Bool retVal = hdr_record_corrected_values (histogram, value, count, expected_interval);
  Napi::Value jsRetVal = Boolean::New (env, retVal);
  return jsRetVal;
}

/**
 * Record a value in the histogram.
 * 
 * @param {number} value the value to record.
 * @returns {boolean} true if the recording was successful, false otherwise.
 * @memberof HdrHistogram
 */
Napi::Value HdrHistogram::recordValue (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value = getInt64<long long> (info[0]);
  _Bool retVal = hdr_record_value (histogram, value);
  Napi::Value jsRetVal = Boolean::New (env, retVal);
  return jsRetVal;
}

/**
 * Record a value multiple times in the histogram.
 * 
 * @param {number} value the value to record.
 * @param {number} count the number of times the value is to be recorded.
 * @returns {boolean} true if the recording was successful, false otherwise.
 * @memberof HdrHistogram
 */
Napi::Value HdrHistogram::recordValues (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value = getInt64<long long> (info[0]);
  long long count = getInt64<long long> (info[1]);
  _Bool retVal = hdr_record_values (histogram, value, count);
  Napi::Value jsRetVal = Boolean::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::valuesAreEquivalent (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long a = getInt64<long long> (info[0]);
  long long b = getInt64<long long> (info[1]);
  _Bool retVal = hdr_values_are_equivalent (histogram, a, b);
  Napi::Value jsRetVal = Boolean::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::mean (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  double retVal = hdr_mean (histogram);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::stddev (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  double retVal = hdr_stddev (histogram);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::add (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  const struct hdr_histogram * from = Napi::ObjectWrap<HdrHistogram>::Unwrap (info[0].As<Napi::Object>())->histogram;
  long long retVal = hdr_add (histogram, from);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::addWhileCorrectingForCoordinatedOmission (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  struct hdr_histogram * from = Napi::ObjectWrap<HdrHistogram>::Unwrap (info[0].As<Napi::Object>())->histogram;
  long long expected_interval = getInt64<long long> (info[1]);
  long long retVal = hdr_add_while_correcting_for_coordinated_omission (histogram, from, expected_interval);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::countAtValue (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value = getInt64<long long> (info[0]);
  long long retVal = hdr_count_at_value (histogram, value);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::lowestEquivalentValue (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value = getInt64<long long> (info[0]);
  long long retVal = hdr_lowest_equivalent_value (histogram, value);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::max (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long retVal = hdr_max (histogram);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::medianEquivalentValue (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value = getInt64<long long> (info[0]);
  long long retVal = hdr_median_equivalent_value (histogram, value);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::min (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long retVal = hdr_min (histogram);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::nextNonEquivalentValue (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value = getInt64<long long> (info[0]);
  long long retVal = hdr_next_non_equivalent_value (histogram, value);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::percentile (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  double percentile = getDouble<double> (info[0]);
  long long retVal = hdr_value_at_percentile (histogram, percentile);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::getMemorySize (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  unsigned long retVal = hdr_get_memory_size (histogram);
  Napi::Value jsRetVal = Number::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogram::reset (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  hdr_reset (histogram);
  Napi::Value jsRetVal = env.Undefined();
  return jsRetVal;
}

Napi::Value HdrHistogram::getEncoded (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  char * encoded = NULL;
  int result = hdr_log_encode(histogram, &encoded);
  if (!encoded || result != 0) {
    Napi::Error::New(env, "failed to encode").ThrowAsJavaScriptException();
  }
  Napi::Value jsRetVal = Napi::Buffer<char>::Copy(env, encoded, strlen(encoded));
  delete encoded; // TODO: verify this call is required
  return jsRetVal;
}

Napi::Value HdrHistogram::setEncoded (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Buffer<char> buffer = info[0].As< Napi::Buffer<char> >();
  char * encoded = buffer.Data();
  size_t len = buffer.Length();
  hdr_log_decode(&histogram, encoded, len); // TODO: verify any existing histogram is freed
  return env.Undefined();
}

Napi::Value HdrHistogram::getHighestTrackableValue (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, histogram->highest_trackable_value);
  return getVal;
}

Napi::Value HdrHistogram::getLowestTrackableValue (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, histogram->lowest_trackable_value);
  return getVal;
}

Napi::Value HdrHistogram::getSignificantFigures (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, histogram->significant_figures);
  return getVal;
}

Napi::Value HdrHistogram::getTotalCount (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, histogram->total_count);
  return getVal;
}

Napi::Value HdrHistogram::getUnitMagnitude (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, histogram->unit_magnitude);
  return getVal;
}

Napi::Function HdrHistogram::GetClassDef (Napi::Env env) { // static
  return DefineClass (env, "HdrHistogram", {
    InstanceMethod ("add", &HdrHistogram::add),
    InstanceMethod ("addWhileCorrectingForCoordinatedOmission", &HdrHistogram::addWhileCorrectingForCoordinatedOmission),
    InstanceMethod ("countAtValue", &HdrHistogram::countAtValue),
    InstanceMethod ("getMemorySize", &HdrHistogram::getMemorySize),
    InstanceMethod ("lowestEquivalentValue", &HdrHistogram::lowestEquivalentValue),
    InstanceMethod ("max", &HdrHistogram::max),
    InstanceMethod ("mean", &HdrHistogram::mean),
    InstanceMethod ("min", &HdrHistogram::min),
    InstanceMethod ("recordCorrectedValue", &HdrHistogram::recordCorrectedValue),
    InstanceMethod ("recordCorrectedValues", &HdrHistogram::recordCorrectedValues),
    InstanceMethod ("recordValue", &HdrHistogram::recordValue),
    InstanceMethod ("recordValues", &HdrHistogram::recordValues),
    InstanceMethod ("reset", &HdrHistogram::reset),
    InstanceMethod ("stddev", &HdrHistogram::stddev),
    InstanceMethod ("percentile", &HdrHistogram::percentile),
    InstanceMethod ("valuesAreEquivalent", &HdrHistogram::valuesAreEquivalent),
    InstanceMethod ("nextNonEquivalentValue", &HdrHistogram::nextNonEquivalentValue),
    InstanceMethod ("medianEquivalentValue", &HdrHistogram::medianEquivalentValue),
    InstanceMethod ("getEncoded", &HdrHistogram::getEncoded),
    InstanceMethod ("setEncoded", &HdrHistogram::setEncoded),
    InstanceAccessor ("highestTrackableValue", &HdrHistogram::getHighestTrackableValue, nullptr, napi_default),
    InstanceAccessor ("lowestTrackableValue", &HdrHistogram::getLowestTrackableValue, nullptr, napi_default),
    InstanceAccessor ("significantFigures", &HdrHistogram::getSignificantFigures, nullptr, napi_default),
    InstanceAccessor ("totalCount", &HdrHistogram::getTotalCount, nullptr, napi_default),
    InstanceAccessor ("unitMagnitude", &HdrHistogram::getUnitMagnitude, nullptr, napi_default),
  });
}

// pseudoClass HdrHistogram definition end //
// pseudoClass HdrHistogramIterator definition begin //

HdrHistogramIterator::HdrHistogramIterator (const Napi::CallbackInfo& info)
: Napi::ObjectWrap<HdrHistogramIterator> (info)
, iterStruct ()
, iter (&iterStruct)
, histogram (Napi::ObjectWrap<HdrHistogram>::Unwrap (info[0].As<Napi::Object>())->histogram)
{
  Napi::Env env = info.Env();
  hdr_iter_init (iter, histogram);
}

HdrHistogramIterator::~HdrHistogramIterator () {
}

Napi::Value HdrHistogramIterator::next (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  _Bool retVal = hdr_iter_next (iter);
  Napi::Value jsRetVal = Boolean::New (env, retVal);
  return jsRetVal;
}

Napi::Value HdrHistogramIterator::initLinear (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value_units_per_bucket = getInt64<long long> (info[0]);
  hdr_iter_linear_init (iter, histogram, value_units_per_bucket);
  Napi::Value jsRetVal = env.Undefined();
  return jsRetVal;
}

Napi::Value HdrHistogramIterator::initLog (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  long long value_units_first_bucket = getInt64<long long> (info[0]);
  double log_base = getDouble<double> (info[1]);
  hdr_iter_log_init (iter, histogram, value_units_first_bucket, log_base);
  Napi::Value jsRetVal = env.Undefined();
  return jsRetVal;
}

Napi::Value HdrHistogramIterator::initPercentile (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  int ticks_per_half_distance = getInt32<int> (info[0]);
  hdr_iter_percentile_init (iter, histogram, ticks_per_half_distance);
  Napi::Value jsRetVal = env.Undefined();
  return jsRetVal;
}

Napi::Value HdrHistogramIterator::initRecorded (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  hdr_iter_recorded_init (iter, histogram);
  Napi::Value jsRetVal = env.Undefined();
  return jsRetVal;
}

Napi::Value HdrHistogramIterator::getValue (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, iter->value);
  return getVal;
}

Napi::Value HdrHistogramIterator::getPercentile (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, iter->specifics.percentiles.percentile);
  return getVal;
}

Napi::Value HdrHistogramIterator::getCountLinear (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, iter->specifics.linear.count_added_in_this_iteration_step);
  return getVal;
}

Napi::Value HdrHistogramIterator::getCountLog (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, iter->specifics.log.count_added_in_this_iteration_step);
  return getVal;
}

Napi::Value HdrHistogramIterator::getCountRecorded (const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Value getVal = Number::New (env, iter->specifics.recorded.count_added_in_this_iteration_step);
  return getVal;
}

Napi::Function HdrHistogramIterator::GetClassDef (Napi::Env env) { // static
  return DefineClass (env, "HdrHistogramIterator", {
    InstanceMethod ("initLinear", &HdrHistogramIterator::initLinear),
    InstanceMethod ("initLog", &HdrHistogramIterator::initLog),
    InstanceMethod ("initPercentile", &HdrHistogramIterator::initPercentile),
    InstanceMethod ("initRecorded", &HdrHistogramIterator::initRecorded),
    InstanceMethod ("next", &HdrHistogramIterator::next),
    InstanceMethod ("getValue", &HdrHistogramIterator::getValue),
    InstanceMethod ("getPercentile", &HdrHistogramIterator::getPercentile),
    InstanceMethod ("getCountLinear", &HdrHistogramIterator::getCountLinear),
    InstanceMethod ("getCountLog", &HdrHistogramIterator::getCountLog),
    InstanceMethod ("getCountRecorded", &HdrHistogramIterator::getCountRecorded),
  });
}

// pseudoClass HdrHistogramIterator definition end //