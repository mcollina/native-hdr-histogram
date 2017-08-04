'use strict'

var binding = require('bindings')('native-hdr-histogram.node')
module.exports = binding.HdrHistogram
