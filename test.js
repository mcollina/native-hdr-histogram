'use strict'

const test = require('tap').test
const Histogram = require('./')

test('create an histogram', (t) => {
  t.doesNotThrow(() => new Histogram(1, 100))
  t.end()
})

test('create an histogram with a constructor', (t) => {
  t.doesNotThrow(() => new Histogram(1, 100))
  t.end()
})

test('create an histogram arguments checks', (t) => {
  t.throws(() => new Histogram(-1, 100))
  t.throws(() => new Histogram(0, 100))
  t.throws(() => new Histogram(1, 100, 20))
  t.throws(() => new Histogram(1, 100, 0))
  t.throws(() => new Histogram(1, 100, 6))
  for (let i = 1; i < 5; i++) {
    t.doesNotThrow(() => new Histogram(1, 100, i))
  }
  t.end()
})

test('record values in an histogram', (t) => {
  const instance = new Histogram(1, 100)
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  t.equal(instance.totalCount, 2, 'recordValue increments totalCount')
  t.end()
})

test('recording a non-value returns false', (t) => {
  const instance = new Histogram(1, 100)
  t.notOk(instance.record())
  t.notOk(instance.record(-42))
  t.end()
})

test('record values with counts', (t) => {
  const testValueLevel = 4

  const multiplier = 10
  const instance = new Histogram(1, 360000, 3)
  for (let i = 1; i < 5; i++) {
    instance.recordValues(i, multiplier)
    t.equal(i * multiplier, instance.totalCount, 'record(value, count) increments totalCount')
  }
  t.ok(instance.reset())

  for (let j = 1; j < 5; j++) {
    instance.recordValues(testValueLevel, multiplier)
    t.equal(j * multiplier, instance.countAtValue(testValueLevel), 'record(value, count) increments countAtValue')
  }

  t.end()
})

test('recordCorrectedValue', (t) => {
  const INTERVAL = 10000
  const HIGHEST = 3600 * 1000 * 1000
  const SIGNIFICANT = 3
  const instance = new Histogram(1, HIGHEST, SIGNIFICANT)

  // record this value with a count of 10,000
  instance.recordCorrectedValues(1000, INTERVAL, 10000)
  instance.recordCorrectedValue(100000000, INTERVAL)

  function checkPercentile (percentile, value, tolerance) {
    const valueAt = instance.percentile(percentile)
    const diff = Math.abs(valueAt - value)
    const val = value * tolerance
    t.ok(diff < val)
  }

  const percentiles = [
    [30.0, 1000.0],
    [50.0, 1000.0],
    [75.0, 50000000.0],
    [90.0, 80000000.0],
    [99.0, 98000000.0],
    [99.999, 100000000.0],
    [100.0, 100000000.0]
  ]

  percentiles.forEach(pair => {
    checkPercentile(pair[0], pair[1], 0.001)
  })
  t.equal(instance.totalCount, 20000, 'counts match')
  t.ok(instance.valuesAreEquivalent(instance.min(), 1000.0))
  t.ok(instance.valuesAreEquivalent(instance.max(), 100000000.0))
  t.end()
})

test('stdev, mean, min, max', (t) => {
  const instance = new Histogram(1, 100)
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  t.equal(instance.min(), 42, 'min is available')
  t.equal(instance.max(), 45, 'max is available')
  t.equal(instance.mean(), 43.5, 'mean is available')
  t.equal(instance.stddev(), 1.5, 'stdev is available')
  t.end()
})

test('properties', (t) => {
  const instance = new Histogram(5, 150, 4)

  t.equal(instance.lowestTrackableValue, 5, 'properly returns lowestTrackableValue')
  t.equal(instance.highestTrackableValue, 150, 'properly returns highestTrackableValue')
  t.equal(instance.significantFigures, 4, 'properly returns significantFigures')
  t.equal(instance.unitMagnitude, 2, 'properly returns unitMagnitude')
  t.equal(instance.totalCount, 0, 'properly returns totalCount')
  t.equal(instance.getMemorySize() > 0, true, 'memory allocated for instance')
  t.ok(instance.record(42))
  t.ok(instance.record(44))
  t.equal(instance.totalCount, 2, 'properly returns totalCount after add')
  t.end()
})

test('record values', (t) => {
  const instance = new Histogram(1, 100)
  t.ok(instance.record(10, 3))
  t.equal(instance.min(), 10, 'min is correct')
  t.equal(instance.max(), 10, 'max is correct')
  t.equal(instance.mean(), 10, 'mean is correct')
  t.equal(instance.totalCount, 3, 'metotalCountan is correct')
  t.ok(instance.record(30, 3))
  t.equal(instance.min(), 10, 'min is correct')
  t.equal(instance.max(), 30, 'max is correct')
  t.equal(instance.mean(), 20, 'mean is correct')
  t.equal(instance.totalCount, 6, 'metotalCountan is correct')
  t.ok(instance.record(40, 6))
  t.equal(instance.min(), 10, 'min is correct')
  t.equal(instance.max(), 40, 'max is correct')
  t.equal(instance.mean(), 30, 'mean is correct')
  t.equal(instance.totalCount, 12, 'metotalCountan is correct')
  t.end()
})

test('percentile', (t) => {
  const instance = new Histogram(1, 100)
  t.ok(instance.record(42))
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  t.equal(instance.percentile(10), 42, 'percentile match')
  t.equal(instance.percentile(99), 45, 'percentile match')
  t.equal(instance.percentile(100), 45, 'percentile match')
  t.end()
})

test('wrong percentile', (t) => {
  const instance = new Histogram(1, 100)
  t.ok(instance.record(42))
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  t.throws(() => instance.percentile(), 'no percentile throws')
  t.throws(() => instance.percentile(101), 'percentile > 100 throws')
  t.throws(() => instance.percentile(0), 'percentile == 0 throws')
  t.throws(() => instance.percentile(-1), 'percentile < 0 throws')
  t.end()
})

test('encode/decode', (t) => {
  const instance = new Histogram(1, 100)
  t.ok(instance.record(42))
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  const instance2 = Histogram.decode(instance.encode())
  t.equal(instance2.percentile(10), 42, 'percentile match')
  t.equal(instance2.percentile(99), 45, 'percentile match')
  t.end()
})

test('fail decode', (t) => {
  t.throws(() => Histogram.decode())
  t.throws(() => Histogram.decode('hello'))
  t.throws(() => Histogram.decode({}))
  t.throws(() => Histogram.decode(42))
  t.end()
})

test('percentiles', (t) => {
  const instance = new Histogram(1, 100)
  t.deepEqual(instance.percentiles(), [{
    percentile: 100,
    value: 0
  }], 'empty percentiles has 0 till 100%')
  t.ok(instance.record(42))
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  t.deepEqual(instance.percentiles(), [{
    percentile: 0,
    value: 42
  }, {
    percentile: 50,
    value: 42
  }, {
    percentile: 75,
    value: 45
  }, {
    percentile: 100,
    value: 45
  }], 'percentiles matches')
  t.end()
})

test('linear, log, and recorded counts', (t) => {
  const countTotal = counts => {
    return counts.reduce((total, entry) =>
      total + entry.count, 0)
  }

  const highestTrackableValue = 3600 * 1000 * 1000
  const significantFigures = 3
  const interval = 10000

  const rawHistogram = new Histogram(1, highestTrackableValue, significantFigures)
  const corHistogram = new Histogram(1, highestTrackableValue, significantFigures)

  for (let i = 0; i < 10000; i++) {
    rawHistogram.recordValue(1000)
    corHistogram.recordCorrectedValue(1000, interval)
  }

  rawHistogram.recordValue(100000000)
  corHistogram.recordCorrectedValue(100000000, 10000)

  const linearCounts = rawHistogram.linearcounts(100000)
  const linearCountsCor = corHistogram.linearcounts(10000)
  const logCounts = rawHistogram.logcounts(10000, 2.0)
  const logCountsCor = corHistogram.logcounts(10000, 2.0)
  const recordedCounts = rawHistogram.recordedcounts()
  const recordedCountsCor = corHistogram.recordedcounts()

  t.equal(linearCounts[0].count, 10000, 'linear count at 0 is 10000')
  t.equal(linearCounts[99].count, 0, 'linear count at 99 is 0')
  t.equal(linearCounts[499].count, 0, 'linear count at 499 is 0')
  t.equal(linearCounts[999].count, 1, 'linear count at 999 is 1')

  t.equal(linearCountsCor[0].count, 10001, 'corrected linear count at 0 is 10001')
  t.equal(linearCountsCor.length, 10000, 'corrected linear count length is 10000')
  t.equal(countTotal(linearCountsCor), 20000, 'corrected total linear counts is 20000')

  t.equal(logCounts[0].count, 10000, 'log count at 0 is 10000')
  t.equal(logCounts[9].count, 0, 'log count at 9 is 0')
  t.equal(logCounts[14].count, 1, 'log count at 14 is 1')

  t.equal(logCountsCor[0].count, 10001, 'corrected log count at 0 is 10001')
  t.equal(logCountsCor.length, 15, 'corrected log count length is 15')
  t.equal(countTotal(logCountsCor), 20000, 'corrected total log counts is 20000')

  t.equal(recordedCounts[0].count, 10000, 'recorded count at 1 is 10000')
  t.equal(recordedCounts[1].count, 1, 'recorded count at 1 is 1')

  t.equal(recordedCountsCor[0].count, 10000, 'corrected recorded count 1 is 10000')
  t.equal(countTotal(recordedCountsCor), 20000, 'corrected total recorded counts is 20000')

  t.end()
})

test('support >2e9', (t) => {
  const recordValue = 4 * 1e9
  const instance = new Histogram(1, recordValue)
  var compare = (a, b) => {
    var diff = Math.abs(a - b)
    // hdr_min and hdr_max do not return precise data, even before
    // conversion to double.
    if (diff < 1e-3 * Math.min(Math.abs(a), Math.abs(b))) {
      return true
    } else {
      console.error(`Mismatch! Got ${a}, expected ${b}!`)
      return false
    }
  }
  t.ok(instance.record(recordValue))
  t.ok(compare(instance.min(), recordValue), 'min match')
  t.ok(compare(instance.max(), recordValue), 'max match')
  t.end()
})

test('simple add histogram', (t) => {
  const instance1 = new Histogram(1, 100)
  t.ok(instance1.record(1))
  t.ok(instance1.record(2))
  t.ok(instance1.record(3))
  t.equal(instance1.mean(), 2, 'mean is correct')
  const instance2 = new Histogram(1, 200)
  t.ok(instance2.record(5))
  t.ok(instance2.record(6))
  t.ok(instance2.record(7))
  t.equal(instance2.mean(), 6, 'mean is correct')
  const dropped = instance1.add(instance2)
  t.equal(dropped, 0, 'nothing dropped')
  t.equal(instance1.mean(), 4, 'mean is correct')
  t.end()
})

test('comprehensive add histogram', (t) => {
  const highestTrackableValue = 3600 * 1000 * 1000 // 1 hour in usec units
  const numberOfSignificantValueDigits = 3 // Maintain at least 3 decimal points of accuracy
  const testValueLevel = 4

  const histogram = new Histogram(1, highestTrackableValue, numberOfSignificantValueDigits)
  const other = new Histogram(1, highestTrackableValue, numberOfSignificantValueDigits)

  histogram.record(testValueLevel)
  histogram.record(testValueLevel * 1000)
  other.record(testValueLevel)
  other.record(testValueLevel * 1000)

  histogram.add(other)
  t.equal(histogram.countAtValue(testValueLevel), 2)
  t.equal(histogram.countAtValue(testValueLevel * 1000), 2)
  t.equal(histogram.totalCount, 4, 'add should increase the totalCount of the destination by the count added')

  const biggerOther = new Histogram(1, highestTrackableValue * 2, numberOfSignificantValueDigits)
  biggerOther.record(testValueLevel)
  biggerOther.record(testValueLevel * 1000)
  biggerOther.record(highestTrackableValue * 2)

  // Adding the smaller histogram to the bigger one should work:
  const dropped = biggerOther.add(histogram)
  t.equal(dropped, 0, 'no values should be dropped if a histogram with a smaller range is added')
  t.equal(biggerOther.countAtValue(testValueLevel), 3)
  t.equal(biggerOther.countAtValue(testValueLevel * 1000), 3)
  t.equal(biggerOther.countAtValue(highestTrackableValue * 2), 1) // overflow smaller hist...
  t.equal(biggerOther.totalCount, 7)

  // But trying to add a larger histogram into a smaller one should cause values to be dropped
  t.equal(histogram.add(biggerOther), 1, 'add a larger histogram to a smaller one causes out of range values to be dropped')

  t.equal(histogram.add(other, 42), 0, 'add with expected_interval succeeds')
  t.end()
})

test('fail add', (t) => {
  const instance = new Histogram(1, 100)
  t.throws(() => instance.add())
  t.throws(() => instance.add('hello'))
  t.throws(() => instance.add({}))
  t.end()
})

test('add with subclasses', (t) => {
  class Subclass extends Histogram {
    constructor () {
      super(1, 100)
    }
  }

  const instance = new Histogram(1, 100)
  const sub = new Subclass()
  for (let i = 0; i < 5; i++) {
    sub.record((i + 1) * 10)
  }
  t.equal(instance.add(sub), 0, 'can add from subclass')
  t.equal(instance.totalCount, sub.totalCount, 'totalCount is correct after add')
  t.end()
})

test('totalCount', (t) => {
  const instance = new Histogram(1, 100)
  const count = Math.floor(Math.random() * 20) + 10
  for (let i = 0; i < count; i++) {
    instance.record(i + 1)
  }
  t.equal(instance.totalCount, count, 'totalCount is correct after add')
  t.ok(instance.reset())
  t.equal(instance.totalCount, 0, 'totalCount is correct after reset')
  t.end()
})

test('countAtValue', (t) => {
  const instance = new Histogram(1, 100)
  const testValueLevel = 4
  const count = Math.floor(Math.random() * 20) + 10
  for (let i = 0; i < count; i++) {
    instance.record(testValueLevel)
  }
  t.equal(instance.countAtValue(testValueLevel), count, 'record increments countAtValue')
  t.ok(instance.reset())
  t.equal(instance.countAtValue(testValueLevel), 0, 'reset() sets count to zero')
  t.end()
})

test('valuesAreEquivalent', (t) => {
  const instance = new Histogram(20000000, 100000000, 5)
  instance.record(100000000)
  instance.record(20000000)
  instance.record(30000000)
  t.equal(true, instance.valuesAreEquivalent(20000000, instance.percentile(50.0)))
  t.equal(true, instance.valuesAreEquivalent(30000000, instance.percentile(50.0)))
  t.equal(true, instance.valuesAreEquivalent(100000000, instance.percentile(83.34)))
  t.equal(true, instance.valuesAreEquivalent(100000000, instance.percentile(99.0)))
  t.end()
})

test('lowestEquivalentValue', (t) => {
  const histogram = new Histogram(1, 3600 * 1000 * 1000, 3) // e.g. for 1 hr in usec units;
  t.equal(histogram.lowestEquivalentValue(10007), 10000, 'The lowest equivalent value to 10007 is 10000')
  t.equal(histogram.lowestEquivalentValue(10009), 10008, 'The lowest equivalent value to 10009 is 10008')
  t.end()
})

test('highestEquivalentValue', (t) => {
  const histogram = new Histogram(1024, 3600000000, 3)
  t.equal(histogram.highestEquivalentValue(8180 * 1024), 8183 * 1024 + 1023, 'The highest equivalent value to 8180 * 1024 is 8183 * 1024 + 1023')
  t.equal(histogram.highestEquivalentValue(8191 * 1024), 8191 * 1024 + 1023, 'The highest equivalent value to 8187 * 1024 is 8191 * 1024 + 1023')
  t.equal(histogram.highestEquivalentValue(8193 * 1024), 8199 * 1024 + 1023, 'The highest equivalent value to 8193 * 1024 is 8199 * 1024 + 1023')
  t.equal(histogram.highestEquivalentValue(9995 * 1024), 9999 * 1024 + 1023, 'The highest equivalent value to 9995 * 1024 is 9999 * 1024 + 1023')
  t.equal(histogram.highestEquivalentValue(10007 * 1024), 10007 * 1024 + 1023, 'The highest equivalent value to 10007 * 1024 is 10007 * 1024 + 1023')
  t.equal(histogram.highestEquivalentValue(10008 * 1024), 10015 * 1024 + 1023, 'The highest equivalent value to 10008 * 1024 is 10015 * 1024 + 1023')
  t.end()
})

test('reset histogram', (t) => {
  const instance = new Histogram(1, 100)
  t.equal(instance.min(), 9223372036854776000, 'min is setup')
  t.equal(instance.max(), 0, 'max is setup')
  t.equal(instance.totalCount, 0, 'totalCount is setup')
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  t.equal(instance.totalCount, 2, 'totalCount is correct before reset')
  t.equal(instance.min(), 42, 'min is correct before reset')
  t.equal(instance.max(), 45, 'max is correct before reset')
  t.equal(instance.mean(), 43.5, 'mean is correct before reset')
  t.equal(instance.stddev(), 1.5, 'stdev is correct before reset')
  t.ok(instance.reset())
  t.equal(instance.min(), 9223372036854776000, 'min is reset')
  t.equal(instance.max(), 0, 'max is reset')
  t.equal(instance.totalCount, 0, 'totalCount is reset')
  t.ok(instance.record(52))
  t.ok(instance.record(55))
  t.equal(instance.totalCount, 2, 'totalCount is correct after reset')
  t.equal(instance.min(), 52, 'min is correct after reset')
  t.equal(instance.max(), 55, 'max is correct after reset')
  t.equal(instance.mean(), 53.5, 'mean is correct after reset')
  t.equal(instance.stddev(), 1.5, 'stdev is correct after reset')
  // test to ensure maintains ref to self when returning after reset
  var resetInstance = instance.reset()
  t.equal(instance, resetInstance)
  t.end()
})
