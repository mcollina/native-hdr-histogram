'use strict'

const test = require('tap').test
const Histogram = require('./')

test('create an histogram', (t) => {
  t.doesNotThrow(() => Histogram(1, 100))
  t.end()
})

test('create an histogram with a constructor', (t) => {
  t.doesNotThrow(() => new Histogram(1, 100))
  t.end()
})

test('record values in an histogram', (t) => {
  const instance = new Histogram(1, 100)
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  t.end()
})

test('recording a non-value returns false', (t) => {
  const instance = Histogram(1, 100)
  t.notOk(instance.record())
  t.notOk(instance.record(-42))
  t.end()
})

test('stdev, mean, min, max', (t) => {
  const instance = Histogram(1, 100)
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  t.equal(instance.min(), 42, 'min is available')
  t.equal(instance.max(), 45, 'max is available')
  t.equal(instance.mean(), 43.5, 'mean is available')
  t.equal(instance.stddev(), 1.5, 'stdev is available')
  t.end()
})

test('percentile', (t) => {
  const instance = Histogram(1, 100)
  t.ok(instance.record(42))
  t.ok(instance.record(42))
  t.ok(instance.record(45))
  t.equal(instance.percentile(10), 42, 'percentile match')
  t.equal(instance.percentile(99), 45, 'percentile match')
  t.end()
})
