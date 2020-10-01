'use strict'

const path = require('path')
const { HdrHistogram, HdrHistogramIterator } = require('node-gyp-build')(path.join(__dirname, '.'))
console.log(HdrHistogram, HdrHistogramIterator)
const isNumber = value => typeof value === 'number' && value !== Infinity && value !== -Infinity

class Histogram extends HdrHistogram {
  /**
   * Record a value in the histogram.
   *
   * @param {number} value the value to record.
   * @param {number} count the number of times the value is to be recorded (default 1).
   * @returns {boolean} true if the recording was successful, false otherwise.
   * @memberof Histogram
   */
  record (value, count) {
    if (isNumber(value)) {
      if (typeof count === 'undefined') {
        return super.recordValue(value)
      } else {
        return super.recordValues(value, count)
      }
    } else {
      return false
    }
  }

  /**
   * Serialize the histogram.
   *
   * @returns {Buffer} a Buffer containing a serialized version of the histogram.
   * @memberof Histogram
   */
  encode () {
    return super.getEncoded()
  }

  /**
   * Deserialize a histogram.
   *
   * @static
   * @param {Buffer} encoded a Buffer containing a previously serialized Histogram.
   * @returns {Histogram} a new populated Histogram.
   * @memberof Histogram
   */
  static decode (encoded) {
    const histogram = new Histogram(1, 10) // the values here are immaterial
    histogram.setEncoded(encoded)
    return histogram
  }

  /**
   * Get the value at a given percentile.
   *
   * @param {number} percentile must be > 0 and <= 100, otherwise it will throw.
   * @returns {number} the value at the given percentile.
   * @memberof Histogram
   */
  percentile (percentile) {
    if (!isNumber(percentile)) {
      throw (Error('No percentile specified'))
    } else if (percentile <= 0 || percentile > 100) {
      throw (Error('percentile must be > 0 and <= 100'))
    } else {
      return super.percentile(percentile)
    }
  }

  /**
   * Get all the percentiles.
   *
   * Used for iterating through histogram values according to percentile levels.
   * The iteration is performed in steps that start at 0% and reduce their distance
   * to 100% according to the `ticksPerHalfDistance` parameter,
   * ultimately reaching 100% when all recorded histogram values are exhausted.
   *
   * @param {number} ticksPerHalfDistance the number of iteration steps per half-distance to 100% (default 1).
   * @returns {<Array>Object} an array of objects with `percentile` and `value` numeric properties.
   * @memberof Histogram
   */
  percentiles (ticksPerHalfDistance) {
    const result = []
    const iter = new HdrHistogramIterator(this)
    iter.initPercentile(ticksPerHalfDistance || 1)
    while (iter.next()) {
      result.push({ percentile: iter.getPercentile(), value: iter.getValue() })
    }
    return result
  }

  /**
   * Get the linear counts.
   *
   * Used for iterating through histogram values in linear steps. The iteration is performed in steps of
   * `valueUnitsPerBucket` in size, terminating when all recorded histogram values are exhausted.
   *
   * Note that each iteration "bucket" includes values up to and including the next bucket boundary value.
   *
   * @param {number} valueUnitsPerBucket the bucket size as described above.
   * @returns {<Array>Object} an array of objects with `count` and `value` numeric properties.
   * @memberof Histogram
   */
  linearcounts (valueUnitsPerBucket) {
    const result = []
    const iter = new HdrHistogramIterator(this)
    iter.initLinear(valueUnitsPerBucket)
    while (iter.next()) {
      result.push({ count: iter.getCountLinear(), value: iter.getValue() })
    }
    return result
  }

  /**
   * Get the logarithmic counts.
   *
   * Used for iterating through histogram values in logarithmically increasing levels.
   * The iteration is performed in steps that start at `valueUnitsFirstBucket` and
   * increase exponentially according to `logBase`, terminating when all recorded histogram
   * values are exhausted.
   *
   * Note that each iteration "bucket" includes values up to and including the next bucket boundary value.
   *
   * @param {number} valueUnitsFirstBucket the value units for the first bucket as described above.
   * @param {number} logBase the logarithmic base as described above.
   * @returns {<Array>Object} an array of objects with `count` and `value` numeric properties.
   * @memberof Histogram
   */
  logcounts (valueUnitsFirstBucket, logBase) {
    const result = []
    const iter = new HdrHistogramIterator(this)
    iter.initLog(valueUnitsFirstBucket, logBase)
    while (iter.next()) {
      result.push({ count: iter.getCountLog(), value: iter.getValue() })
    }
    return result
  }

  /**
   * Get the recorded counts.
   *
   * Used for iterating through all recorded histogram values using the finest granularity
   * steps supported by the underlying representation. The iteration steps through all non-zero
   * recorded value counts, and terminates when all recorded histogram values are exhausted.
   *
   * @returns {<Array>Object} an array of objects with `count` and `value` numeric properties.
   * @memberof Histogram
   */
  recordedcounts () {
    const result = []
    const iter = new HdrHistogramIterator(this)
    iter.initRecorded()
    while (iter.next()) {
      result.push({ count: iter.getCountRecorded(), value: iter.getValue() })
    }
    return result
  }

  /**
   * Reset the Histogram so it can be reused.
   *
   * @returns {Histogram} the empty Histogram.
   * @memberof Histogram
   */
  reset () {
    super.reset()
    return this
  }

  /**
   * Add values from another Histogram.
   *
   * @param {Histogram} histogram the Histogram containing the values to be added.
   * @param {number} expectedInterval the delay between recording values (optional).
   * @returns {number} the number of dropped values.
   * @memberof Histogram
   */
  add (histogram, expectedInterval) {
    if (typeof expectedInterval === 'undefined') {
      return super.add(histogram)
    } else {
      return super.addWhileCorrectingForCoordinatedOmission(histogram, expectedInterval)
    }
  }

  /**
   * Get the highest value that is equivalent to the given value within the histogram's resolution.
   *
   * @param {number} value the value for which the highest equivalent value is to be determined.
   * @returns {number} the highest equivalent value.
   * @memberof Histogram
   */
  highestEquivalentValue (value) {
    return super.nextNonEquivalentValue(value) - 1
  }
}

module.exports = Histogram
