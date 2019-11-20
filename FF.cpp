#include "FF.h"
#include <bitset>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <type_traits>

namespace FF {
template <class T>
using FI = FloatInfo<T>;

// Half precision
template <>
const unsigned FI<hex_half>::mantissa_bits = 10;
template <>
const unsigned FI<hex_half>::exponent_bits = 5;
template <>
const unsigned FI<hex_half>::sign_shift = 15;
template <>
const uint16_t FI<hex_half>::mantissa_mask = 0x3FF;
template <>
const uint16_t FI<hex_half>::exponent_mask = 0x7C00;
template <>
const uint16_t FI<hex_half>::sign_mask = 0x8000;
template <>
const int FI<hex_half>::exp_bias = -15;

// Single precision
template <>
const unsigned FI<hex_single>::mantissa_bits = 23;
template <>
const unsigned FI<hex_single>::exponent_bits = 8;
template <>
const unsigned FI<hex_single>::sign_shift = 31;
template <>
const uint32_t FI<hex_single>::mantissa_mask = 0x007fffff;
template <>
const uint32_t FI<hex_single>::exponent_mask = 0x7f800000;
template <>
const uint32_t FI<hex_single>::sign_mask = 0x80000000;
template <>
const int FI<hex_single>::exp_bias = -127;

// Double precision
template <>
const unsigned FI<hex_double>::mantissa_bits = 52;
template <>
const unsigned FI<hex_double>::exponent_bits = 11;
template <>
const unsigned FI<hex_double>::sign_shift = 63;
template <>
const uint64_t FI<hex_double>::mantissa_mask = 0xfffffffffffff;
template <>
const uint64_t FI<hex_double>::exponent_mask = 0x7ff0000000000000;
template <>
const uint64_t FI<hex_double>::sign_mask = 0x8000000000000000;
template <>
const int FI<hex_double>::exp_bias = -1023;
}

namespace {
template <typename U, typename T>
inline U bitcast(const T in) {
  static_assert(sizeof(T) == sizeof(U), "Sizes must match");
  static_assert(std::is_trivially_copyable<T>::value,
                "Source type not trivially copyable");
  static_assert(std::is_trivially_copyable<U>::value,
                "Destination type not trivially copyable");

  U out;
  std::memcpy(&out, &in, sizeof(T));
  return out;
}

template <typename T>
inline std::ostream& dumpFloat(std::ostream& os, T f) {
  os << "Float: " << std::defaultfloat << f;
  os << ", " << std::hexfloat << f;
  os << ", " << std::scientific << f;
  return os;
}

float convertHalfToFloat(FF::hex_half half) {
  using FI_H = FF::FloatInfo<FF::hex_half>;
  using FI_S = FF::FloatInfo<FF::hex_single>;

  if (FI_H(half).isInf()) {
    uint32_t bits = (FI_H::sign_mask & half) << 16;
    bits |= FI_S::exponent_mask;
    return bitcast<float>(bits);
  } else if (FI_H(half).isNan()) {
    return NAN;
  } else if (FI_H(half).isZero()) {
    const uint32_t bits = (FI_H::sign_mask & half) << 16;
    return bitcast<float>(bits);
  }

  const uint16_t abs_bits = ~FI_H::sign_mask & half;
  uint16_t mantissa_bits = abs_bits & FI_H::mantissa_mask;
  uint16_t exponent_bits = abs_bits >> FI_H::mantissa_bits;

  constexpr int bias_diff = FI_H::exp_bias - FI_S::exp_bias;
  const uint16_t exp_lsb = 1 << FI_H::mantissa_bits;
  if (exponent_bits == 0) {
    int16_t e = -1;
    do {
      e++;
      mantissa_bits <<= 1;
    } while ((mantissa_bits & exp_lsb) == 0);
    mantissa_bits &= FI_H::mantissa_mask;
    exponent_bits = bias_diff - e;
  } else {
    exponent_bits += bias_diff;
  }

  uint32_t single_bits = exponent_bits << FI_S::mantissa_bits;

  single_bits |= (FI_H::sign_mask & half) << 16;

  constexpr uint32_t mantissa_shift = FI_S::mantissa_bits - FI_H::mantissa_bits;
  single_bits |= uint32_t(mantissa_bits) << mantissa_shift;

  return bitcast<float>(single_bits);
}

template <typename T>
struct Converter final {
  using F = typename FF::NativeFloat<T>::NativeType;
  static F HexToFloat(T x) {
    return bitcast<F>(x);
  }
};

template <>
struct Converter<FF::hex_half> final {
  static float HexToFloat(FF::hex_half x) {
    return convertHalfToFloat(x);
  }
};
}  // namespace anonymous

namespace FF {
template <class T>
using FI = FloatInfo<T>;

template <class T>
bool FloatInfo<T>::isDenorm() const {
  return (0 == (hex_val.hex & FI<T>::exponent_mask)) &&
         (hex_val.hex & FI<T>::mantissa_mask);
}

template <class T>
bool FloatInfo<T>::isInf() const {
  return (hex_val.hex & ~FI<T>::sign_mask) == FI<T>::exponent_mask;
}

template <class T>
bool FloatInfo<T>::isNan() const {
  return (hex_val.hex & FI<T>::mantissa_mask) &&
         ((hex_val.hex & FI<T>::exponent_mask) == FI<T>::exponent_mask);
}

template <class T>
bool FloatInfo<T>::isZero() const {
  return (hex_val.hex == FI<T>::sign_mask) || (hex_val.hex == 0);
}

template <>
FloatInfo<hex_half>::FloatInfo(float single_precision) {
  float single_abs = std::fabs(single_precision);
  uint32_t single_bits = *reinterpret_cast<uint32_t*>(&single_abs);
  if (std::isinf(single_precision)) {
    hex_val.hex = FI<hex_half>::exponent_mask;
  } else if (std::isnan(single_precision)) {
    hex_val.hex = FI<hex_half>::exponent_mask | FI<hex_half>::mantissa_mask;
  } else {
    int exp_unbiased = std::ilogb(single_abs);
    if (exp_unbiased < -14) {
      uint32_t single_bits = *reinterpret_cast<uint32_t*>(&single_abs);

      constexpr const auto mant_shift =
          FI<hex_single>::mantissa_bits - FI<hex_half>::mantissa_bits;
      uint32_t mant_bits =
          (single_bits & FI<hex_single>::mantissa_mask) >> mant_shift;

      // Implicit dropped bit
      mant_bits |= uint32_t(1) << FI<hex_half>::mantissa_bits;

      const unsigned exp_shift =
          std::abs(exp_unbiased - FI<hex_half>::exp_bias) + 1;
      if (exp_shift > FI<hex_half>::mantissa_bits) {
        // 2^-24
        hex_val.hex = 0;  // return
      } else {
        hex_val.hex = mant_bits >> exp_shift;
        const uint32_t MSB_dropped_bit = uint32_t(1) << (exp_shift - 1);
        const bool dropped_bit_set = 0 != (mant_bits & MSB_dropped_bit);
        if (dropped_bit_set) {
          const bool RTE = 0 == (mant_bits & (MSB_dropped_bit - 1));
          if (!RTE) {
            hex_val.hex++;  // Round away from zero
          } else if (hex_val.hex & 1) {
            // lsb we're keeping is set, round up
            hex_val.hex++;
          }
        }
      }
    } else if (single_abs > 65519.0f) {
      // 65504 is max value half can hold, but RTE limit 65519
      hex_val.hex = FI<hex_half>::exponent_mask;
    } else {
      uint32_t exponent_bits = (single_bits & FI<hex_single>::exponent_mask) >>
                               FI<hex_single>::mantissa_bits;
      exponent_bits += FI<hex_single>::exp_bias;
      hex_val.hex = (exponent_bits - FI<hex_half>::exp_bias)
                    << FI<hex_half>::mantissa_bits;

      constexpr const auto shift =
          FI<hex_single>::mantissa_bits - FI<hex_half>::mantissa_bits;
      const uint32_t mant_bits =
          (single_bits & FI<hex_single>::mantissa_mask) >> shift;
      hex_val.hex |= mant_bits;  // RTZ value

      /*
         RTE Rounding - Round To Nearest Even
         * If first lost mantissa bit from isn't set, round down towards zero.
         * If first lost mantissa bit is set:
            * If at least one other bit set, Round up
            * If no other bits set -> then round to even:
                * Even if first non-dropped bit is set, then we round up
      */
      constexpr const uint32_t MSB_dropped_bit = uint32_t(1) << (shift - 1);
      const bool dropped_bit_set = 0 != (single_bits & MSB_dropped_bit);
      if (dropped_bit_set) {
        const bool RTE = 0 == (single_bits & (MSB_dropped_bit - 1));
        if (!RTE) {
          hex_val.hex++;  // Round away from zero
        } else if (mant_bits & 1) {
          // lsb we're keeping is set, round up
          hex_val.hex++;
        }
      }
    }
  }

  if (std::signbit(single_precision)) {
    hex_val.hex |= FI<hex_half>::sign_mask;
  }
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const FI<T>& fi) {
  static_assert(std::is_same<T, hex_double>::value ||
                    std::is_same<T, hex_single>::value ||
                    std::is_same<T, hex_half>::value,
                "bad template instantiation");

  if (std::is_same<T, hex_double>::value) {
    os << "64-bit double precision";
  } else if (std::is_same<T, hex_single>::value) {
    os << "32-bit single precision";
  } else {
    os << "16-bit half precision";
  }

  if (fi.isDenorm()) {
    os << " denormal\n";
  } else if (fi.isInf()) {
    os << " infinity\n";
  } else if (fi.isNan()) {
    os << " NaN\n";
  } else {
    os << "\n";
  }

  os << "Hex: 0x" << std::hex << fi.hex_val.hex << "\n";

  os << "Bits: ";
  os << std::bitset<1>(fi.getSignBits()) << " ";
  os << std::bitset<FI<T>::exponent_bits>(fi.getExponentBits()) << " ";
  os << std::bitset<FI<T>::mantissa_bits>(fi.getMantissaBits()) << "\n";

  if (std::is_same<T, hex_half>::value) {
    using FloatType = typename NativeFloat<T>::NativeType;
    const FloatType printer = Converter<T>::HexToFloat(fi.hex_val.hex);
    return dumpFloat(os, printer);
  } else {
    return dumpFloat(os, fi.hex_val.f);
  }
}

template <typename T>
typename NativeFloat<T>::NativeType parseString(const std::string&& str) {
  // Check for 0x hex digits
  const bool isHex =
      (str.size() > 2) && (str.compare(0, 2, "0x") == 0) &&
      (str.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos);

  using FloatType = typename NativeFloat<T>::NativeType;
  FloatType native;
  if (isHex) {
    const T hex_value = static_cast<T>(std::strtoul(str.c_str(), 0, 16));
    native = Converter<T>::HexToFloat(hex_value);

  } else {
    try {
      native = static_cast<FloatType>(std::stod(str));
    } catch (std::invalid_argument) {
      std::cerr << "Invalid argument: " << str << "\n";
      return -1;
    }
  }
  return native;
}

// instantiates all template types
void printAll(const double x) {
  FF::FloatInfo<FF::hex_double> double_info(x);
  std::cout << double_info << "\n\n";

  const float downcast = static_cast<float>(x);
  FF::FloatInfo<FF::hex_single> single_info(downcast);
  std::cout << single_info << "\n\n";

  FF::FloatInfo<FF::hex_half> half_info(downcast);
  std::cout << half_info << "\n";
}

void parseHalf(const char* str) {
  FF::NativeFloat<uint16_t>::NativeType native = FF::parseString<uint16_t>(str);
  FF::printAll(native);
}

void parseSingle(const char* str) {
  FF::NativeFloat<uint32_t>::NativeType native = FF::parseString<uint32_t>(str);
  FF::printAll(native);
}

void parseDouble(const char* str) {
  FF::NativeFloat<uint64_t>::NativeType native = FF::parseString<uint64_t>(str);
  FF::printAll(native);
}
}  // namespace FF
