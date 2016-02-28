'use strict'

const Histogram = require('./')
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
