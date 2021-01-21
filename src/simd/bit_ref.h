#ifndef BIT_PTR_H
#define BIT_PTR_H

#include <cstddef>
#include <cstdint>

/// A reference to a bit within a byte.
///
/// Conceptually behaves the same as a `bool &`, as opposed to a `bool *`. For example, the `=` operator overwrites the
/// contents of the bit being referenced instead of changing which bit is pointed to.
///
/// This should behave essentially identically to the weird bit references that come out of a `std::vector<bool>`.
struct bit_ref {
    uint8_t *byte;
    uint8_t bit_index;

    /// Construct a bit_ref from a pointer and a bit offset.
    /// The offset can be larger than a word.
    /// Automatically canonicalized so that the offset is less than 8.
    bit_ref(void *base, size_t offset);

    /// Copy assignment.
    inline bit_ref &operator=(bool value) {
        *byte &= ~((uint8_t)1 << bit_index);
        *byte |= (uint8_t)value << bit_index;
        return *this;
    }
    /// Copy assignment.
    inline bit_ref &operator=(const bit_ref &value) {
        *this = (bool)value;
        return *this;
    }
    /// Xor assignment.
    inline bit_ref &operator^=(bool value) {
        *byte ^= (uint8_t)value << bit_index;
        return *this;
    }
    /// Bitwise-and assignment.
    inline bit_ref &operator&=(bool value) {
        *byte &= (uint8_t)value << bit_index;
        return *this;
    }
    /// Bitwise-or assignment.
    inline bit_ref &operator|=(bool value) {
        *byte |= (uint8_t)value << bit_index;
        return *this;
    }
    /// Swap assignment.
    inline void swap_with(bit_ref other) {
        bool b = (bool)other;
        other = (bool)*this;
        *this = b;
    }

    /// Implicit conversion to bool.
    inline operator bool() const {  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        return (*byte >> bit_index) & 1;
    }
};

#endif
