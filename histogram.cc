#include <nan.h>
#include "hdr_histogram_wrap.h"

NAN_MODULE_INIT(InitAll) {
  HdrHistogramWrap::Init(target);
}

NODE_MODULE(Histogram, InitAll)
