# native-hdr-histogram

node.js bindings for [hdr histogram][hdr] [C implementation][cimpl](version 0.9.3).

[![Build status](https://ci.appveyor.com/api/projects/status/tgu1of6olrg64so4?svg=true)](https://ci.appveyor.com/project/mcollina/native-hdr-histogram)
[![Build Status](https://travis-ci.org/mcollina/native-hdr-histogram.svg?branch=master)](https://travis-ci.org/mcollina/native-hdr-histogram)

> HDR Histogram is designed for recoding histograms of value measurements
in latency and performance sensitive applications. Measurements show
value recording times as low as 3-6 nanoseconds on modern (circa 2014)
Intel CPUs. A Histogram's memory footprint is constant, with no
allocation operations involved in recording data values or in iterating through them.
- from [hdr histogram][hdr] website

This library is blazingly fast, and you can use it to record
histograms with no overhead. Linux, Mac OS X and Windows are all
supported.

  * <a href="#install">Installation</a>
  * <a href="#example">Example</a>
  * <a href="#api">API</a>
  * <a href="#licence">Licence &amp; copyright</a>

## Install

```
npm i native-hdr-histogram --save
```

If you see any errors, you might need to configure your system to compile native addons:
follow the instructions at [node-gyp][node-gyp].

## Example

```js
'use strict'

const Histogram = require('native-hdr-histogram')
const max = 1000000
const key = 'record*' + max
const histogram = new Histogram(1, 100)

console.time(key)
for (let i = 0; i < max; i++) {
  histogram.record(Math.floor((Math.random() * 42 + 1)))
}
console.timeEnd(key)

console.log('80 percentile is', histogram.percentile(80))
console.log('99 percentile is', histogram.percentile(99))

console.log(histogram.percentiles())
```

## API

  * <a href="#histogram"><code>Histogram</code></a>
  * <a href="#record"><code>histogram#<b>record()</b></code></a>
  * <a href="#min"><code>histogram#<b>min()</b></code></a>
  * <a href="#max"><code>histogram#<b>max()</b></code></a>
  * <a href="#mean"><code>histogram#<b>mean()</b></code></a>
  * <a href="#stddev"><code>histogram#<b>stddev()</b></code></a>
  * <a href="#percentile"><code>histogram#<b>percentile()</b></code></a>
  * <a href="#percentiles"><code>histogram#<b>percentiles()</b></code></a>
  * <a href="#encode"><code>histogram#<b>encode()</b></code></a>
  * <a href="#decode"><code>histogram#<b>decode()</b></code></a>
  * <a href="#reset"><code>histogram#<b>reset()</b></code></a>

-------------------------------------------------------
<a name="histogram"></a>

### Histogram(lowest, max, figures)

Create a new histogram with:

* `lowest`: is the lowest possible number that can be recorded (default
  1).
* `max`: is the maximum number that can be recorded (default 100).
* `figures`: the number of figures in a decimal number that will be
  maintained, must be between 1 and 5 (inclusive) (default 3).

-------------------------------------------------------
<a name="record"></a>

### histogram.record(value)

Record `value` in the histogram. Returns `true` if the recording was
successful, `false` otherwise.

-------------------------------------------------------
<a name="min"></a>

### histogram.min()

Return the minimum value recorded in the histogram.

-------------------------------------------------------
<a name="max"></a>

### histogram.max()

Return the maximum value recorded in the histogram.

-------------------------------------------------------
<a name="mean"></a>

### histogram.mean()

Return the mean of the histogram.

-------------------------------------------------------
<a name="stddev"></a>

### histogram.stddev()

Return the standard deviation of the histogram.

-------------------------------------------------------
<a name="percentile"></a>

### histogram.percentile(percentile)

Returns the value at the given percentile. `percentile` must be >
0 and <= 100, otherwise it will throw.

-------------------------------------------------------
<a name="percentiles"></a>

### histogram.percentiles()

Returns all the percentiles.

Sample output:

```js
[ { percentile: 0, value: 1 },
  { percentile: 50, value: 22 },
  { percentile: 75, value: 32 },
  { percentile: 87.5, value: 37 },
  { percentile: 93.75, value: 40 },
  { percentile: 96.875, value: 41 },
  { percentile: 98.4375, value: 42 },
  { percentile: 100, value: 42 } ]
```

-------------------------------------------------------
<a name="encode"></a>

### histogram.encode()

Returns a `Buffer` containing a serialized version of the histogram

-------------------------------------------------------
<a name="decode"></a>

### histogram.decode(buf)

Reads a `Buffer` and deserialize an histogram.

-------------------------------------------------------
<a name="reset"></a>

### histogram.reset()

Resets the histogram so it can be reused.

## Acknowledgements

This project was kindly sponsored by [nearForm](http://nearform.com).

The pre-compilation work of this project is only possible because of [mapbox's][mapbox]
amazing work on [node-pre-gyp][node-pre-gyp]. A lot of the functionality enabled
is following the example set by their [node-sqlite3 library][sqlite3].

## License

This library is licensed as MIT

HdrHistogram_c is licensed as [BSD license][HdrHistogram_c-license]

zlib is licensed as [zlib License][zlib-license]

The scripts used in the scripts folder are modified [BSD licensed][sqlite3-scripts-license] scripts from the [node-sqlite3][sqlite3] libary.

[hdr]: http://hdrhistogram.org/
[cimpl]: https://github.com/HdrHistogram/HdrHistogram_c
[node-gyp]: https://github.com/nodejs/node-gyp#installation
[mapbox]: http://mapbox.com
[node-pre-gyp]: https://github.com/mapbox/node-pre-gyp
[sqlite3]: https://github.com/mapbox/node-sqlite3
[HdrHistogram_c-license]: https://github.com/HdrHistogram/HdrHistogram_c/blob/master/LICENSE.txt
[sqlite3-scripts-license]: https://github.com/mapbox/node-sqlite3/blob/master/LICENSE
[zlib-license]: http://www.zlib.net/zlib_license.html
