"use strict";

/*
 * jQuery Easing v1.3 - http://gsgd.co.uk/sandbox/jquery/easing/
 *
 * Uses the built in easing capabilities added In jQuery 1.1
 * to offer multiple easing options
 *
 * TERMS OF USE - EASING EQUATIONS
 * 
 * Open source under the BSD License. 
 * 
 * Copyright Ã‚Â© 2001 Robert Penner
 * All rights reserved.
 *
 * TERMS OF USE - jQuery Easing
 * 
 * Open source under the BSD License. 
 * 
 * Copyright Ã‚Â© 2008 George McGinley Smith
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of 
 * conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list 
 * of conditions and the following disclaimer in the documentation and/or other materials 
 * provided with the distribution.
 * 
 * Neither the name of the author nor the names of contributors may be used to endorse 
 * or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
*/
jQuery.easing.jswing = jQuery.easing.swing;jQuery.extend(jQuery.easing, { def: "easeOutQuad", swing: function swing(e, f, a, h, g) {
    return jQuery.easing[jQuery.easing.def](e, f, a, h, g);
  }, easeInQuad: function easeInQuad(e, f, a, h, g) {
    return h * (f /= g) * f + a;
  }, easeOutQuad: function easeOutQuad(e, f, a, h, g) {
    return -h * (f /= g) * (f - 2) + a;
  }, easeInOutQuad: function easeInOutQuad(e, f, a, h, g) {
    if ((f /= g / 2) < 1) {
      return h / 2 * f * f + a;
    }return -h / 2 * (--f * (f - 2) - 1) + a;
  }, easeInCubic: function easeInCubic(e, f, a, h, g) {
    return h * (f /= g) * f * f + a;
  }, easeOutCubic: function easeOutCubic(e, f, a, h, g) {
    return h * ((f = f / g - 1) * f * f + 1) + a;
  }, easeInOutCubic: function easeInOutCubic(e, f, a, h, g) {
    if ((f /= g / 2) < 1) {
      return h / 2 * f * f * f + a;
    }return h / 2 * ((f -= 2) * f * f + 2) + a;
  }, easeInQuart: function easeInQuart(e, f, a, h, g) {
    return h * (f /= g) * f * f * f + a;
  }, easeOutQuart: function easeOutQuart(e, f, a, h, g) {
    return -h * ((f = f / g - 1) * f * f * f - 1) + a;
  }, easeInOutQuart: function easeInOutQuart(e, f, a, h, g) {
    if ((f /= g / 2) < 1) {
      return h / 2 * f * f * f * f + a;
    }return -h / 2 * ((f -= 2) * f * f * f - 2) + a;
  }, easeInQuint: function easeInQuint(e, f, a, h, g) {
    return h * (f /= g) * f * f * f * f + a;
  }, easeOutQuint: function easeOutQuint(e, f, a, h, g) {
    return h * ((f = f / g - 1) * f * f * f * f + 1) + a;
  }, easeInOutQuint: function easeInOutQuint(e, f, a, h, g) {
    if ((f /= g / 2) < 1) {
      return h / 2 * f * f * f * f * f + a;
    }return h / 2 * ((f -= 2) * f * f * f * f + 2) + a;
  }, easeInSine: function easeInSine(e, f, a, h, g) {
    return -h * Math.cos(f / g * (Math.PI / 2)) + h + a;
  }, easeOutSine: function easeOutSine(e, f, a, h, g) {
    return h * Math.sin(f / g * (Math.PI / 2)) + a;
  }, easeInOutSine: function easeInOutSine(e, f, a, h, g) {
    return -h / 2 * (Math.cos(Math.PI * f / g) - 1) + a;
  }, easeInExpo: function easeInExpo(e, f, a, h, g) {
    return f == 0 ? a : h * Math.pow(2, 10 * (f / g - 1)) + a;
  }, easeOutExpo: function easeOutExpo(e, f, a, h, g) {
    return f == g ? a + h : h * (-Math.pow(2, -10 * f / g) + 1) + a;
  }, easeInOutExpo: function easeInOutExpo(e, f, a, h, g) {
    if (f == 0) {
      return a;
    }if (f == g) {
      return a + h;
    }if ((f /= g / 2) < 1) {
      return h / 2 * Math.pow(2, 10 * (f - 1)) + a;
    }return h / 2 * (-Math.pow(2, -10 * --f) + 2) + a;
  }, easeInCirc: function easeInCirc(e, f, a, h, g) {
    return -h * (Math.sqrt(1 - (f /= g) * f) - 1) + a;
  }, easeOutCirc: function easeOutCirc(e, f, a, h, g) {
    return h * Math.sqrt(1 - (f = f / g - 1) * f) + a;
  }, easeInOutCirc: function easeInOutCirc(e, f, a, h, g) {
    if ((f /= g / 2) < 1) {
      return -h / 2 * (Math.sqrt(1 - f * f) - 1) + a;
    }return h / 2 * (Math.sqrt(1 - (f -= 2) * f) + 1) + a;
  }, easeInElastic: function easeInElastic(f, h, e, l, k) {
    var i = 1.70158;var j = 0;var g = l;if (h == 0) {
      return e;
    }if ((h /= k) == 1) {
      return e + l;
    }if (!j) {
      j = k * 0.3;
    }if (g < Math.abs(l)) {
      g = l;var i = j / 4;
    } else {
      var i = j / (2 * Math.PI) * Math.asin(l / g);
    }return -(g * Math.pow(2, 10 * (h -= 1)) * Math.sin((h * k - i) * (2 * Math.PI) / j)) + e;
  }, easeOutElastic: function easeOutElastic(f, h, e, l, k) {
    var i = 1.70158;var j = 0;var g = l;if (h == 0) {
      return e;
    }if ((h /= k) == 1) {
      return e + l;
    }if (!j) {
      j = k * 0.3;
    }if (g < Math.abs(l)) {
      g = l;var i = j / 4;
    } else {
      var i = j / (2 * Math.PI) * Math.asin(l / g);
    }return g * Math.pow(2, -10 * h) * Math.sin((h * k - i) * (2 * Math.PI) / j) + l + e;
  }, easeInOutElastic: function easeInOutElastic(f, h, e, l, k) {
    var i = 1.70158;var j = 0;var g = l;if (h == 0) {
      return e;
    }if ((h /= k / 2) == 2) {
      return e + l;
    }if (!j) {
      j = k * (0.3 * 1.5);
    }if (g < Math.abs(l)) {
      g = l;var i = j / 4;
    } else {
      var i = j / (2 * Math.PI) * Math.asin(l / g);
    }if (h < 1) {
      return -0.5 * (g * Math.pow(2, 10 * (h -= 1)) * Math.sin((h * k - i) * (2 * Math.PI) / j)) + e;
    }return g * Math.pow(2, -10 * (h -= 1)) * Math.sin((h * k - i) * (2 * Math.PI) / j) * 0.5 + l + e;
  }, easeInBack: function easeInBack(e, f, a, i, h, g) {
    if (g == undefined) {
      g = 1.70158;
    }return i * (f /= h) * f * ((g + 1) * f - g) + a;
  }, easeOutBack: function easeOutBack(e, f, a, i, h, g) {
    if (g == undefined) {
      g = 1.70158;
    }return i * ((f = f / h - 1) * f * ((g + 1) * f + g) + 1) + a;
  }, easeInOutBack: function easeInOutBack(e, f, a, i, h, g) {
    if (g == undefined) {
      g = 1.70158;
    }if ((f /= h / 2) < 1) {
      return i / 2 * (f * f * (((g *= 1.525) + 1) * f - g)) + a;
    }return i / 2 * ((f -= 2) * f * (((g *= 1.525) + 1) * f + g) + 2) + a;
  }, easeInBounce: function easeInBounce(e, f, a, h, g) {
    return h - jQuery.easing.easeOutBounce(e, g - f, 0, h, g) + a;
  }, easeOutBounce: function easeOutBounce(e, f, a, h, g) {
    if ((f /= g) < 1 / 2.75) {
      return h * (7.5625 * f * f) + a;
    } else {
      if (f < 2 / 2.75) {
        return h * (7.5625 * (f -= 1.5 / 2.75) * f + 0.75) + a;
      } else {
        if (f < 2.5 / 2.75) {
          return h * (7.5625 * (f -= 2.25 / 2.75) * f + 0.9375) + a;
        } else {
          return h * (7.5625 * (f -= 2.625 / 2.75) * f + 0.984375) + a;
        }
      }
    }
  }, easeInOutBounce: function easeInOutBounce(e, f, a, h, g) {
    if (f < g / 2) {
      return jQuery.easing.easeInBounce(e, f * 2, 0, h, g) * 0.5 + a;
    }return jQuery.easing.easeOutBounce(e, f * 2 - g, 0, h, g) * 0.5 + h * 0.5 + a;
  } });

//# sourceMappingURL=jquery.easing.min-compiled.js.map