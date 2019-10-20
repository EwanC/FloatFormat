#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

namespace FF {

typedef uint16_t hex_half;
typedef uint32_t hex_single;
typedef uint64_t hex_double;

template <class T> struct NativeFloat;

template <> struct NativeFloat<hex_double> { using NativeType = double; };

template <> struct NativeFloat<hex_single> { using NativeType = float; };

template <> struct NativeFloat<hex_half> { using NativeType = float; };

template <class T> struct FloatInfo final {
public:
  FloatInfo() = delete;
  explicit FloatInfo(T x) { hex_val.hex = x; };
  explicit FloatInfo(typename NativeFloat<T>::NativeType x) { hex_val.f = x; };

  static const unsigned mantissa_bits;
  static const unsigned exponent_bits;
  static const unsigned sign_shift;

  static const T mantissa_mask;
  static const T exponent_mask;
  static const T sign_mask;

  static const int exp_bias;

private:
  template <class X>
  friend std::ostream &operator<<(std::ostream &, const FloatInfo<X> &);

  union Converter {
    typename NativeFloat<T>::NativeType f;
    T hex;
  };
  union Converter hex_val;

  bool isDenorm() const;
  bool isInf() const;
  bool isNan() const;

  T getSignBits() const {
    return (hex_val.hex & sign_mask) >> ((sizeof(T) * 8) - 1);
  }
  T getExponentBits() const {
    return (hex_val.hex & exponent_mask) >> mantissa_bits;
  }
  T getMantissaBits() const { return hex_val.hex & mantissa_mask; }
};

template <class T>
std::ostream &operator<<(std::ostream &os, const FloatInfo<T> &fi);

template <class T>
typename NativeFloat<T>::NativeType parseString(const std::string &&str);

void printAll(const double);

void parseHalf(const char *);
void parseSingle(const char *);
void parseDouble(const char *);
}
