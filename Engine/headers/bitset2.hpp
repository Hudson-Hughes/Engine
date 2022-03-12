// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_CB_HPP
#define BITSET2_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_HEX_PARAMS_CB_HPP
#define BITSET2_HEX_PARAMS_CB_HPP

#include <string>

namespace Bitset2
{

template< class CharT = char,
          class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT> >
struct hex_params
{
  using str_t= std::basic_string<CharT,Traits,Allocator>;

  hex_params( CharT         zero_ch=        CharT('0'),
              CharT         a_ch=           CharT('a'),
              bool          leading_zeroes= true,
              bool          non_empty=      true,
              str_t const & prfx=           str_t{} )
  : zeroCh{ zero_ch }
  , aCh{ a_ch }
  , leadingZeroes{ leading_zeroes }
  , nonEmpty{ non_empty }
  , prefix{ prfx }
  {}

  CharT        zeroCh=         CharT( '0' );
  CharT        aCh=            CharT( 'a' );
  bool         leadingZeroes=  true;
  bool         nonEmpty=       true;
  str_t        prefix;
}; // struct hex_params

} // namespace Bitset2

#endif // BITSET2_HEX_PARAMS_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_SELECT_BASE_T_CB_HPP
#define BITSET2_SELECT_BASE_T_CB_HPP

#include <cstdint>
#include <cstddef>

namespace Bitset2
{
namespace detail
{

template<bool b,class T1, class T2>
struct if_else                 {  using type= T1; };

template<class T1, class T2>
struct if_else<false,T1,T2>    {  using type= T2; };

template<bool b,class T1, class T2>
using if_else_t= typename if_else<b,T1,T2>::type ;

/// \brief Select any of uint8_t, uint16_t, uint32_t or
/// unsigned long long. Result depends on N and on provision
/// of these types by compiler.
template<size_t N>
struct select_base
{
#ifdef INT8_MIN
  enum : bool { has_int8= true };
  using  UI8= uint8_t;
#else
  enum : bool { has_int8= false };
  using  UI8= void;
#endif
#ifdef INT16_MIN
  enum : bool { has_int16= true };
  using  UI16= uint16_t;
#else
  enum : bool { has_int16= false };
  using  UI16= void;
#endif
#ifdef INT32_MIN
  enum : bool { has_int32= true };
  using  UI32= uint32_t;
#else
  enum : bool { has_int32= false };
  using  UI32= void;
#endif

  using type=
        if_else_t< has_int8  && (N<=8),  UI8,
        if_else_t< has_int16 && (N<=16), UI16,
        if_else_t< has_int32 && (N<=32), UI32,
                   unsigned long long > > >;
}; // struct select_base

template<size_t N>
using select_base_t= typename select_base<N>::type;

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_SELECT_BASE_T_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_HASH_CB_HPP
#define BITSET2_HASH_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_H_TYPES_CB_HPP
#define BITSET2_H_TYPES_CB_HPP

#include <array>
#include <utility>
#include <climits>
#include <type_traits>

namespace Bitset2
{
namespace detail
{

    template<class T,class Enabled=void>      struct h_types;

    template<class T>
    struct h_types<T,
                   typename std::enable_if<   std::is_integral<T>::value
                                           && std::is_unsigned<T>::value>::type>
    {
      using ULONG_t=  unsigned long;
      using ULLONG_t= unsigned long long;
      using base_t=   T;

      template<size_t n_array>
      using array_t= std::array<base_t,n_array>;

      enum : size_t
      { ulong_n_bits=   sizeof(ULONG_t)  * CHAR_BIT  ///< #bits in ULONG_t
      , ullong_n_bits=  sizeof(ULLONG_t) * CHAR_BIT  ///< #bits in ULLONG_t
      , base_t_n_bits=  sizeof(base_t)   * CHAR_BIT  ///< #bits in base_t
      , npos=          ~size_t(0)
      };
    }; // struct h_types

    template<class T>
    constexpr
    T
    ce_min( T v1, T v2 ) noexcept
    { return ( v1 < v2 ) ? v1 : v2; }

    /// http://stackoverflow.com/q/29136207/3876684
    template<class T>
    constexpr
    T
    ce_left_shift( T v1, size_t n_shift ) noexcept
    {
      return   ( n_shift == 0 ) ? v1
             : ( ( n_shift >= h_types<T>::base_t_n_bits ) ? T(0)
               : T( v1 << n_shift ) );
    }

    template<class T>
    constexpr
    T
    ce_right_shift( T v1, size_t n_shift ) noexcept
    {
      return   ( n_shift == 0 ) ? v1
             : ( ( n_shift >= h_types<T>::base_t_n_bits ) ? T(0)
               : T( v1 >> n_shift ) );
    }

    template<size_t n_array,class T,size_t ... S>
    inline constexpr
    typename h_types<T>::template array_t<n_array>
    gen_empty_array_impl( std::index_sequence<S...> ) noexcept
    {
      return
        typename h_types<T>::template array_t<n_array>{{ ( T(S) & T(0) ) ... }};
    }

    template<size_t n_array,class T>
    inline constexpr
    typename h_types<T>::template array_t<n_array>
    gen_empty_array() noexcept
    {
      return
        gen_empty_array_impl<n_array,T>( std::make_index_sequence<n_array>() );
    } // gen_empty_array

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_H_TYPES_CB_HPP

#include <functional>
#include <array>
#include <climits>
#include <algorithm>

namespace Bitset2
{
namespace detail
{

template<size_t n_words,class T>
struct hash_impl
{
  using    base_t=        T;
  using    result_type=   std::size_t;
  using    array_t=       typename h_types<T>::template array_t<n_words>;

  enum : size_t
  { size_t_bits=    sizeof(result_type) * CHAR_BIT   ///< #bits in result_type
  , base_t_n_bits=  h_types<T>::base_t_n_bits        ///< #bits in T
  , bits_mod=       base_t_n_bits % size_t_bits
  , bits_div=       base_t_n_bits / size_t_bits + ( bits_mod > 0 )
  , size_t_mod=     size_t_bits % base_t_n_bits
  , size_t_div=     size_t_bits / base_t_n_bits
  };

  enum : bool
  { easy_bits=  ( size_t_bits >= base_t_n_bits )
  , easy_ratio= ( size_t_mod == 0 )
  };

  result_type
  operator()( array_t const & arr ) const noexcept
  {
    if( n_words == 0 ) return 0;
    if( n_words == 1 )
    {
      if( easy_bits ) return arr[0];
      return to_result_t( arr[0] );
    } // if n_words == 1

    return cmpsd_hash( arr );
  }

  result_type
  to_result_t( base_t a ) const noexcept
  {
    result_type ret_val= 0;
    size_t      shft=    0;
    for( size_t c= 0; c < bits_div; ++c, shft += size_t_bits )
    {
      auto const crrnt= result_type( a >> shft );
      do_combine( ret_val, crrnt, c );
    }
    return ret_val;
  } // to_result_t

  result_type
  cmpsd_hash( array_t const & arr ) const noexcept
  {
    result_type ret_val= 0;

    if( easy_ratio )
    {
      for( size_t c= 0; c < n_words; c += size_t_div )
      {
        result_type  r= 0;
        auto const uppr= std::min( n_words, c + size_t_div );
        for( size_t w= c; w < uppr; ++w )
             r |= ( result_type(arr[w]) << ((w-c)*base_t_n_bits) );
        do_combine( ret_val, r, c / size_t_div );
      }
    }
    else
    {
      for( size_t c= 0; c < n_words; ++c )
      {
        auto const crrnt= easy_bits
                          ? result_type(arr[c]) : to_result_t(arr[c]);
        do_combine( ret_val, crrnt, c );
      }
    }

    return ret_val;
  } // cmpsd_hash

  void
  do_combine( result_type &r, result_type crrnt, size_t cnt ) const noexcept
  {
    crrnt += cnt;
    auto const n_rot= cnt % size_t_bits;
    if( n_rot > 0 )
              crrnt= ( crrnt << n_rot ) | ( crrnt >> (size_t_bits-n_rot) );

    r ^= crrnt;
  } // do_combine
}; // struct hash_impl

} } // namespace Bitset2::detail

#endif // BITSET2_HASH_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_ARRAY_ACCESS_CB_HPP
#define BITSET2_ARRAY_ACCESS_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_BIT_CHARS_CB_HPP
#define BITSET2_BIT_CHARS_CB_HPP

namespace Bitset2
{
namespace detail
{

  template<size_t N,class T>
  struct bit_chars
  {
      using h_t=      h_types<T>;
      using ULONG_t=  typename h_t::ULONG_t;
      using ULLONG_t= typename h_t::ULLONG_t;
      using base_t=   T;
      //
      enum : size_t
      { n_bits=         N
      , ulong_n_bits=   h_t::ulong_n_bits                ///< #bits in ULONG_t
      , ullong_n_bits=  h_t::ullong_n_bits               ///< #bits in ULLONG_t
      , base_t_n_bits=  h_t::base_t_n_bits               ///< #bits in T
      , div_val=        n_bits / base_t_n_bits
      , mod_val=        n_bits % base_t_n_bits
      , n_words=        mod_val ? div_val + 1 : div_val  ///< #words required
      , n_array=        ( n_words == 0 ) ? 1 : n_words   ///< #words used
      };
      enum : ULONG_t
      { ulong_max=      ULONG_MAX };
      enum : base_t
      { all_one= base_t(~base_t(0))
      , low_bit_pattern=                         ///< Mask for idx==0
              n_bits == 0 ? base_t(0)
              : ( div_val > 0 || mod_val == 0 )
                ? all_one
                : ce_right_shift( all_one, base_t_n_bits - mod_val )
      , hgh_bit_pattern=                        ///< Mask for idx+1==n_words
              n_bits == 0 ? base_t(0)
              : mod_val == 0
                ? all_one
                : ce_right_shift( all_one, base_t_n_bits - mod_val )
      };
  }; // struct bit_chars

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_BIT_CHARS_CB_HPP

namespace Bitset2
{
namespace detail
{

  /// \brief array_access is used for getting bunches of 4 bits, which is
  /// needed when creating hex-strings
  template<size_t N,class T>
  struct array_access
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : size_t {   base_t_n_bits=   b_chars::base_t_n_bits
                    , n_array=         b_chars::n_array
                  };
    enum : size_t {   div_four=        N / 4
                    , mod_four=        N % 4
                  };
    using array_t=       typename h_types<T>::template array_t<n_array>;

    constexpr
    base_t
    get_four_bits( array_t const &arr, size_t offset ) const noexcept
    {
      return
        get_four_bits_impl( arr,
                            offset / base_t_n_bits, offset % base_t_n_bits );
    }

    constexpr
    base_t
    get_four_bits_impl( array_t const &arr,
                        size_t idx, size_t bit_idx ) const noexcept
    {
      return ( bit_idx >= 3 )
             ? h1_get_four_bits( arr[idx], bit_idx )
             : h2_get_four_bits( arr, idx, bit_idx );
    }

    constexpr
    base_t
    h1_get_four_bits( base_t v, size_t bit_idx ) const noexcept
    { return ( v >> ( bit_idx - 3 ) ) & base_t(0xF); }

    constexpr
    base_t
    h2_get_four_bits( array_t const &arr,
                      size_t idx, size_t bit_idx ) const noexcept
    {
      return
        (( arr[idx] & ( ( base_t(1) << (bit_idx+1) ) - 1 ) ) << ( 3 - bit_idx ))
        + ( ( idx == 0 ) ? base_t(0)
                         : ( arr[idx-1] >> (base_t_n_bits - (3 - bit_idx)) ) );
    }
  }; // struct array_access

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_ARRAY_ACCESS_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_ARRAY_FUNCS_CB_HPP
#define BITSET2_ARRAY_FUNCS_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_COUNT_BITS_CB_HPP
#define BITSET2_COUNT_BITS_CB_HPP

namespace Bitset2
{
namespace detail
{

  /// Returns the number of bits set in val
  template<class T>
  constexpr
  inline
  size_t
  count_bits( T val, size_t count= 0 ) noexcept
  {
    return
      ( val == T(0) )
               ? count
               : count_bits( T(val & T( val - T(1) )), // clears lowest set bit
                             count + 1 );
  }

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_COUNT_BITS_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_INDEX_LSB_SET_CB_HPP
#define BITSET2_INDEX_LSB_SET_CB_HPP

#include <limits>
#include <climits>
#include <cstddef>

namespace Bitset2
{
namespace detail
{

/// https://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightBinSearch
template<class T>
struct index_lsb_set
{
  enum : size_t { npos=   std::numeric_limits<size_t>::max()
                , n_bits= sizeof(T) * CHAR_BIT };

  constexpr
  index_lsb_set() noexcept
  {
    static_assert( ( n_bits & ( n_bits - 1 ) ) == 0,
                   "Number of bits in data type is not a power of 2" );
  }

  /// \brief Returns index of first (least significant) bit set in val.
  /// Returns npos if all bits are zero.
  constexpr
  size_t
  operator()( T val ) const noexcept
  {
    return   ( T(0) == val ) ? npos
           : find_idx( val, T( T(~T(0)) >> (n_bits >> 1) ), n_bits >> 1, 1 );
  }

private:
  constexpr
  size_t
  find_idx( T val, T pttrn, size_t sh_rght, size_t ct ) const noexcept
  {
    return   ( sh_rght == 1 ) ? ( ct - size_t( T(val & T(0x1)) ) )
           : T( val & pttrn ) == T(0)
             ? find_idx( T(val >> sh_rght), T( pttrn >> ( sh_rght >> 1 )),
                         sh_rght >> 1, ct + sh_rght )
             : find_idx( val, T(pttrn >> ( sh_rght >> 1 )), sh_rght >> 1, ct );
  }
}; // struct index_lsb_set

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_INDEX_LSB_SET_CB_HPP

#include <utility>

namespace Bitset2
{
namespace detail
{

  template<size_t n_array,class T>
  struct array_funcs
  {
      using base_t=                 T;
      using array_t=       typename h_types<T>::template array_t<n_array>;
      using array_p1_t=    typename h_types<T>::template array_t<n_array+1>;

      enum : size_t { base_t_n_bits=  h_types<T>::base_t_n_bits
                    , npos=           h_types<T>::npos };

      /// Binary operator type
      enum class op_type { or_op, and_op, xor_op, sdiff_op };

      constexpr
      array_t
      bitwise_or( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
          return bitwise_op_impl( op_type::or_op, arr1, arr2,
                                  std::make_index_sequence<n_array>() );
      }

      /// Used for |= operator. Separate implementation for better performance.
      constexpr
      void
      bitwise_or_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
      { return bitwise_op_assgn_impl( op_type::or_op, arr1, arr2 ); }

      constexpr
      array_t
      bitwise_and( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return bitwise_op_impl( op_type::and_op, arr1, arr2,
                                std::make_index_sequence<n_array>() );
      }

      /// Used for &= operator. Separate implementation for better performance.
      constexpr
      void
      bitwise_and_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
      { return bitwise_op_assgn_impl( op_type::and_op, arr1, arr2 ); }

      constexpr
      array_t
      bitwise_xor( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return bitwise_op_impl( op_type::xor_op, arr1, arr2,
                                std::make_index_sequence<n_array>() );
      }

      /// Used for ^= operator. Separate implementation for better performance.
      constexpr
      void
      bitwise_xor_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
      { return bitwise_op_assgn_impl( op_type::xor_op, arr1, arr2 ); }

      /// Computes the set difference, i.e. arr1 & ~arr2
      constexpr
      array_t
      bitwise_setdiff( array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return bitwise_op_impl( op_type::sdiff_op, arr1, arr2,
                                std::make_index_sequence<n_array>() );
      }

      /// \brief Computes the set difference, i.e. arr1 & ~arr2.
      /// Separate implementation for better performance.
      constexpr
      void
      bitwise_setdiff_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
      { return bitwise_op_assgn_impl( op_type::sdiff_op, arr1, arr2 ); }

      constexpr
      bool
      none( array_t const &arr ) const noexcept
      { return none_impl( n_array - 1, arr ); }

      constexpr
      size_t
      count( array_t const &arr ) const noexcept
      {
        return
          sum_impl( count_impl( arr, std::make_index_sequence<n_array>() ) );
      }

      constexpr
      bool
      equal( array_t const &arr1, array_t const &arr2 ) const noexcept
      { return equal_impl( arr1, arr2, 0 ); }

      constexpr
      bool
      less_than( array_t const &arr1, array_t const &arr2 ) const noexcept
      { return less_than_impl( arr1, arr2, n_array - 1 ); }

      /// \brief Returns true if f returns true for each pair
      /// of elements in arr1 and arr2
      template<class F>
      constexpr
      bool
      zip_fold_and( array_t const &arr1, array_t const &arr2,
                    F &f ) const noexcept(noexcept( f( base_t(0), base_t(0) ) ))
      { return zip_fold_and_impl( arr1, arr2, f, 0 ); }

      /// \brief Returns true if f returns true for at least one pair
      /// of elements in arr1 and arr2
      template<class F>
      constexpr
      bool
      zip_fold_or( array_t const &arr1, array_t const &arr2,
                   F &f ) const noexcept(noexcept( f( base_t(0), base_t(0) ) ))
      { return zip_fold_or_impl( arr1, arr2, f, 0 ); }

      /// Prepend v1 in front of arr
      constexpr
      array_p1_t
      prepend( base_t const v1, array_t const &arr ) const noexcept
      { return prepend_impl( v1, arr, std::make_index_sequence<n_array>()); }

      /// Append v1 to arr
      constexpr
      array_p1_t
      append( array_t const &arr, base_t const v1 ) const noexcept
      { return append_impl( arr, v1, std::make_index_sequence<n_array>()); }

      /// Copy each element in arr but apply pttrn to most significant entry
      template<size_t n>
      constexpr
      array_t
      copy_and_map( base_t const                          pttrn,
                    typename
                    h_types<T>::template array_t<n> const &arr ) const noexcept
      {
        return
          n_array == 0 ? array_t{}
             : copy_and_map_impl(
                 pttrn,
                 arr,
                 gen_empty_array<n_array,T>(),
                 n >= n_array,
                 std::make_index_sequence<ce_min( n_array-1, n )>(),
                 std::make_index_sequence<n_array-1-ce_min(n_array-1, n)>() );
      } // copy_and_map

      //** _impl functions

      template<size_t n,size_t ... S1,size_t ... S2>
      constexpr
      array_t
      copy_and_map_impl( base_t const                          pttrn,
                         typename
                         h_types<T>::template array_t<n> const &arr,
                         array_t const                         &zeroes,
                         bool const                            take_all,
                         std::index_sequence<S1...>,
                         std::index_sequence<S2...> ) const noexcept
      {
        return {{ arr[S1]..., zeroes[S2]...,
                  base_t(( take_all ? arr[n_array-1] : base_t(0) ) & pttrn) }};
      }

      constexpr
      bool
      none_impl( size_t idx, array_t const &arr ) const noexcept
      {
        return    ( arr[idx] == base_t(0) )
               && ( ( idx == 0 ) ? true : none_impl( idx - 1, arr )  );
      }

      template<size_t ... S>
      constexpr
      array_p1_t
      append_impl( array_t const &arr, base_t const v1,
                   std::index_sequence<S...> ) const noexcept
      { return {{ arr[S]..., v1 }}; }

      template<size_t ... S>
      constexpr
      array_p1_t
      prepend_impl( base_t const v1, array_t const &arr,
                    std::index_sequence<S...> ) const noexcept
      { return {{ v1, arr[S]... }}; }

      constexpr
      bool
      equal_impl( array_t const &arr1, array_t const &arr2,
                  size_t ct ) const noexcept
      {
        return   ( ct == n_array ) ? true
               : ( arr1[ct] == arr2[ct] ) && equal_impl( arr1, arr2, ct + 1 );
      }

      constexpr
      bool
      less_than_impl( array_t const &arr1, array_t const &arr2,
                      size_t ct ) const noexcept
      {
        return
          ( arr1[ct] < arr2[ct] )
          || ( arr1[ct] == arr2[ct]
               && ( ct == 0 ? false : less_than_impl( arr1, arr2, ct-1 ) ) );
      }

      template<class F>
      constexpr
      bool
      zip_fold_and_impl( array_t const &arr1, array_t const &arr2,
                         F      &f,
                         size_t ct )
                            const noexcept(noexcept( f(base_t(0), base_t(0))))
      {
        return   ( ct == n_array ) ? true
               : (    f( arr1[ct], arr2[ct] )
                   && zip_fold_and_impl( arr1, arr2, f, ct + 1 ) );
      }

      template<class F>
      constexpr
      bool
      zip_fold_or_impl( array_t const &arr1, array_t const &arr2,
                        F      &f,
                        size_t ct )
                            const noexcept(noexcept( f(base_t(0), base_t(0))))
      {
        return   ( ct == n_array ) ? false
               : (    f( arr1[ct], arr2[ct] )
                   || zip_fold_or_impl( arr1, arr2, f, ct + 1 ) );
      }

      constexpr
      void
      bitwise_op_assgn_impl( op_type opt,
                             array_t &arr1, array_t const &arr2 ) const noexcept
      {
        for( size_t c= 0; c < n_array; ++c )
        {
          if(      opt == op_type::or_op  ) arr1[c] |=  arr2[c];
          else if( opt == op_type::and_op ) arr1[c] &=  arr2[c];
          else if( opt == op_type::xor_op ) arr1[c] ^=  arr2[c];
          else                              arr1[c] &= ~arr2[c];
        }
      } // bitwise_op_assgn_impl

      template<size_t ... S>
      constexpr
      array_t
      bitwise_op_impl( op_type opt,
                       array_t const &arr1, array_t const &arr2,
                       std::index_sequence<S...> ) const noexcept
      { return {{ h_bitwise_op( S, opt, arr1, arr2 )... }};  }

      constexpr
      base_t
      h_bitwise_op( size_t idx, op_type opt,
                     array_t const &arr1, array_t const &arr2 ) const noexcept
      {
        return   (   opt == op_type::or_op )  ? ( arr1[idx] | arr2[idx] )
               : ( ( opt == op_type::and_op ) ? ( arr1[idx] & arr2[idx] )
                   : ( opt == op_type::xor_op ) ? ( arr1[idx] ^ arr2[idx] )
                                                : ( arr1[idx] & ~arr2[idx] ) );
      }

      /// Count bits in each element of arr
      template<size_t ... S>
      constexpr
      std::array<size_t, n_array>
      count_impl( array_t const &arr, std::index_sequence<S...> ) const noexcept
      { return {{ count_bits( arr[S] )... }};  }

      /// Sum over all elements in vals
      template<class T1>
      constexpr
      T1
      sum_impl( std::array<T1, n_array> const &vals,
                size_t ct= n_array - 1 ) const noexcept
      { return vals[ct] + ( ( ct == 0 ) ? T1(0) : sum_impl( vals, ct - 1 ) ); }

      constexpr
      size_t
      idx_lsb_set( array_t const &arr, base_t v, size_t idx ) const noexcept
      {
        return
          v == base_t(0)
            ? ( idx + 1 == n_array ? npos
                                   : idx_lsb_set( arr, arr[idx+1], idx + 1 ) )
            : idx * base_t_n_bits + index_lsb_set<base_t>()( v );
      }

  }; // struct array_funcs

} // namespace detail

} // namespace Bitset2

#endif // BITSET2_ARRAY_FUNCS_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_ARRAY_ADD_CB_HPP
#define BITSET2_ARRAY_ADD_CB_HPP

namespace Bitset2
{
namespace detail
{

  template<size_t N,size_t it_n,class T>
  struct array_add_base
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : size_t {   n_array=         b_chars::n_array };
    enum : base_t
    {   hgh_bit_pattern= b_chars::hgh_bit_pattern
    ,   all_one=         b_chars::all_one
    };
    using array_t=            typename h_types<T>::template array_t<n_array>;
    using zero_array_t=       typename h_types<T>::template array_t<0>;

    /// Used to submit the curent result of the addition and the carry over
    template<size_t n>
    using array_pair_t= std::pair<base_t,
                                  typename h_types<T>::template array_t<n> >;

    constexpr
    array_pair_t<it_n+1>
    add_h1( base_t a, base_t b, base_t sm1, base_t sum,
            typename
            h_types<T>::template array_t<it_n> const &so_far ) const noexcept
    {
      return
        std::make_pair( ( sum < a || sm1 < b ) ? base_t(1) : base_t(0)
                      , array_funcs<it_n,T>()
                             .append( so_far
                                    , ( it_n + 1 < n_array )
                                            ? sum
                                            : base_t(sum & hgh_bit_pattern) ) );
    }

    constexpr
    array_pair_t<it_n+1>
    add_h2( base_t a, base_t b, array_pair_t<it_n> const &a_p ) const noexcept
    { return add_h1( a, b, b + a_p.first, a + b + a_p.first, a_p.second ); }
  }; //struct array_add_base

  /// \brief This struct is introduced for beeing able to partially
  /// specialize function add_h3.
  template<size_t N,size_t it_n,class T>
  struct array_add_h : public array_add_base<N,it_n,T>
  {
    using array_t= typename array_add_base<N,it_n,T>::array_t;

    template<size_t n>
    using array_pair_t=
          typename array_add_base<N,it_n,T>::template array_pair_t<n>;

    constexpr
    array_pair_t<it_n+1>
    add_h3( array_t const &arr1, array_t const &arr2 ) const noexcept
    {
      return
        this->add_h2( arr1[it_n], arr2[it_n]
                    , array_add_h<N,it_n-1,T>().add_h3( arr1, arr2 ) );
    }
  }; // struct array_add_h

  template<size_t N,class T>
  struct array_add_h<N,0,T> : public array_add_base<N,0,T>
  {
    using array_t=      typename array_add_base<N,0,T>::array_t;
    using zero_array_t= typename array_add_base<N,0,T>::zero_array_t;

    template<size_t n>
    using array_pair_t=typename array_add_base<N,0,T>::template array_pair_t<n>;

    constexpr
    array_pair_t<1>
    add_h3( array_t const &arr1, array_t const &arr2 ) const noexcept
    {
      return
          this->add_h2( arr1[0], arr2[0]
                      , std::make_pair( T(0), zero_array_t{} ) );
    }
  }; // struct array_add_h

  /// Allows to add two std::array's in a constexpr
  template<size_t N,class T>
  struct array_add
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : base_t
    {   hgh_bit_pattern= b_chars::hgh_bit_pattern
    ,   all_one=         b_chars::all_one
    };
    enum : size_t {   n_array=         b_chars::n_array   };
    using array_t=            typename h_types<T>::template array_t<n_array>;

    constexpr
    array_t
    add( array_t const &arr1, array_t const &arr2 ) const noexcept
    {
      return
        ( N == 0 ) ? array_t{}
                   : array_add_h<N,n_array-1,T>().add_h3( arr1, arr2 ).second;
    }

    constexpr
    void
    add_assgn( array_t &arr1, array_t const &arr2 ) const noexcept
    {
      base_t  carry= base_t(0);
      for( size_t  c= 0; c < n_array; ++c )
      {
        auto const sm1= base_t( arr2[c] + carry );
        auto const sm=  base_t( arr1[c] + sm1 );
        carry=  sm < arr1[c] || sm1 < arr2[c];
        arr1[c]= sm;
      }
      arr1[n_array-1] &= hgh_bit_pattern;
    } // add_assgn
  }; // struct array_add

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_ARRAY_ADD_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_ARRAY_OPS_CB_HPP
#define BITSET2_ARRAY_OPS_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_REVERSE_BITS_CB_HPP
#define BITSET2_REVERSE_BITS_CB_HPP

#include <limits>
#include <climits>
#include <cstddef>

namespace Bitset2
{
namespace detail
{

/// https://graphics.stanford.edu/~seander/bithacks.html#ReverseParallel
template<class T>
struct reverse_bits
{
  enum : size_t { n_bits=   sizeof(T) * CHAR_BIT
                , n_bits_h= n_bits >> 1 };

  constexpr
  reverse_bits() noexcept
  {
    static_assert( ( n_bits & ( n_bits - 1 ) ) == 0,
                   "Number of bits in data type is not a power of 2" );
  }

  /// \brief Reverses bits in val.
  constexpr
  T
  operator()( T val ) const noexcept  { return rvrs( val ); }

private:
  constexpr
  T
  rvrs( T       val,
        T       mask= T( ~T(0) ) >> n_bits_h,
        size_t  s=    n_bits_h ) const noexcept
  {
    return   s == 0 ? val
           : rvrs( ( (val >> s) & mask ) | ( (val << s) & ~mask ),
                   mask ^ ( mask << ( s >> 1 ) ),
                   s >> 1 );
  }
}; // struct reverse_bits

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_REVERSE_BITS_CB_HPP

namespace Bitset2
{
namespace detail
{

  template<size_t N,class T>
  struct array_ops
  {
    using base_t=                      T;
    using this_t=                      array_ops<N,T>;
    using b_chars=                     bit_chars<N,T>;
    enum : size_t {   base_t_n_bits=   b_chars::base_t_n_bits
                    , n_words=         b_chars::n_words
                    , n_array=         b_chars::n_array
                    , mod_val=         b_chars::mod_val
                    , n_m_mod=         mod_val == 0 ? 0 : base_t_n_bits-mod_val
                  };
    enum : base_t
    {   hgh_bit_pattern= b_chars::hgh_bit_pattern
    ,   all_one=         b_chars::all_one };
    using array_t=            typename h_types<T>::template array_t<n_array>;
    using zero_array_t=       typename h_types<T>::template array_t<0>;

    constexpr
    array_ops( size_t n_shift ) noexcept
    : m_n_shift_mod( n_shift % N )
    , m_shft_div( n_shift / base_t_n_bits )
    , m_shft_mod( n_shift % base_t_n_bits )
    , m_shft_leftright_shift( base_t_n_bits-(n_shift % base_t_n_bits) )
    , m_shft_left_pattern( ce_left_shift( T(~T(0)),
                                          base_t_n_bits
                                             - (n_shift % base_t_n_bits) ))
    , m_shft_right_pattern( ce_right_shift( T(~T(0)),
                                            base_t_n_bits
                                             - (n_shift % base_t_n_bits) ))
    {}

    constexpr
    array_t
    shift_left( array_t const &arr ) const noexcept
    { return shift_left_impl( arr, std::make_index_sequence<n_array>() ); }

    constexpr
    array_t
    shift_right( array_t const &arr ) const noexcept
    { return shift_right_impl( arr, std::make_index_sequence<n_array>() ); }

    /// Used for <<= operator. Separate implementation for better performance.
    constexpr
    void
    shift_left_assgn( array_t &arr ) const noexcept
    {
      for( size_t c= n_words; c > 0; )
      {
        --c;
        if( c >= m_shft_div )
        {
          auto const c2= c - m_shft_div;
          base_t const v1= arr[c2] << m_shft_mod;
          base_t const v2=
            c2 == 0 ? base_t(0)
                    : ce_right_shift( base_t(arr[c2-1] & m_shft_left_pattern),
                                      m_shft_leftright_shift );
          arr[c]= base_t( v1 | v2 );
        }
        else arr[c]= base_t(0);
      } // for c
      arr[n_array-1] &= hgh_bit_pattern;
    } // shift_left_assgn

    /// Used for >>= operator. Separate implementation for better performance.
    constexpr
    void
    shift_right_assgn( array_t &arr ) const noexcept
    {
      for( size_t c= 0; c < n_words; ++c )
      {
        auto const c2= c + m_shft_div;
        if( c2 < n_words )
        {
          base_t const v1= arr[c2] >> m_shft_mod;
          base_t const v2=
            ( c2 + 1 >= n_words ) ? base_t(0)
                  : ce_left_shift( base_t( arr[c2+1] & m_shft_right_pattern ),
                                           m_shft_leftright_shift );
          arr[c]= v1 | v2;
        }
        else arr[c]= base_t(0);
      } // for c
      arr[n_array-1] &= hgh_bit_pattern;
    } // shift_right_assgn

    constexpr
    array_t
    rotate_left( array_t const &arr ) const noexcept
    {
      return
        ( n_array == 1 )
          ? array_t{{ base_t((   base_t( arr[0] << m_n_shift_mod )
                               | ( ce_right_shift( arr[0], N - m_n_shift_mod ) )
                             ) & hgh_bit_pattern ) }}
          : rotate_left_impl( arr,
                              array_ops<N,T>(     m_n_shift_mod ),
                              array_ops<N,T>( N - m_n_shift_mod ),
                              std::make_index_sequence<n_array>() );
    } // rotate_left

    constexpr
    array_t
    flip( array_t const &arr ) const noexcept
    { return flip_impl( arr, std::make_index_sequence<n_array>() ); }

    constexpr
    bool
    all( array_t const &arr ) const noexcept
    { return ( N > 0 ) && all_impl( n_words - 1, arr ); }

    /// Used for ++ operator.
    constexpr
    void
    increment( array_t &arr ) const noexcept
    {
      if( N == 0 ) return;

      size_t  c= 0;
      for( ; c + 1 < n_words; ++c )
      {
        if( ( ++arr[c] ) != base_t(0) ) return;
      }
      ++arr[c];
      arr[c] &= hgh_bit_pattern;
    } // increment

    /// Used for -- operator.
    constexpr
    void
    decrement( array_t &arr ) const noexcept
    {
      if( N == 0 ) return;

      size_t  c= 0;
      for( ; c + 1 < n_words; ++c )
      {
        if( ( arr[c]-- ) != base_t(0) ) return;
      }
      --arr[c];
      arr[c] &= hgh_bit_pattern;
    } // decrement

    /// Reverse bits
    constexpr
    array_t
    reverse( array_t const &arr ) const noexcept
    {
      return   n_array == 1
             ? array_t{{ base_t( reverse_bits<base_t>()(arr[0]) >> n_m_mod ) }}
             : reverse_impl( arr, std::make_index_sequence<n_array>() );
    } // reverse

    //****************************************************

    constexpr
    bool
    all_impl( size_t idx, array_t const &arr ) const noexcept
    {
      return    h_all( idx, arr )
             && ( ( idx == 0 ) ? true : all_impl( idx - 1, arr ) );
    }

    constexpr
    bool
    h_all( size_t idx, array_t const &arr ) const noexcept
    {
      return ( idx + 1 ==  n_words ) ? ( arr[idx] == hgh_bit_pattern )
                                     : ( arr[idx] == all_one );
    }

    template<size_t ... S>
    constexpr
    array_t
    shift_left_impl( array_t const &arr,
                     std::index_sequence<S...> ) const noexcept
    { return {{ h_shift_left( S, arr )... }};  }

    constexpr
    base_t
    h_shift_left( size_t idx, array_t const &arr ) const noexcept
    {
      return   h_shift_left1( idx, arr )
             & ( ( idx+1 == n_words ) ?  hgh_bit_pattern : all_one );
    }

    constexpr
    base_t
    h_shift_left1( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx >= n_words || idx < m_shft_div ) ? base_t(0)
             : base_t( ( arr[idx-m_shft_div] << m_shft_mod )
                       | h_shift_left_rem( idx - m_shft_div, arr ) );
    }

    constexpr
    base_t
    h_shift_left_rem( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx == 0 ) ? base_t(0)
             : ce_right_shift( base_t( arr[idx-1] & m_shft_left_pattern ),
                                m_shft_leftright_shift );
    }

    template<size_t ... S>
    constexpr
    array_t
    shift_right_impl( array_t const &arr,
                      std::index_sequence<S...> ) const noexcept
    { return {{ h_shift_right( S, arr )... }};  }

    constexpr
    base_t
    h_shift_right( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx + m_shft_div >= n_words ) ? base_t(0)
             : base_t(   ( arr[idx+m_shft_div] >> m_shft_mod )
                       | h_shift_right_rem( idx + m_shft_div, arr ) );
    }

    constexpr
    base_t
    h_shift_right_rem( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx + 1 >= n_words ) ? base_t(0)
             : ce_left_shift( base_t( arr[idx+1] & m_shft_right_pattern ),
                              m_shft_leftright_shift );
    }

    template<size_t ... S>
    constexpr
    array_t
    rotate_left_impl( array_t const &arr,
                      this_t  lft,
                      this_t  rgt,
                      std::index_sequence<S...> ) const noexcept
    {
      return
        {{ ( S > lft.m_shft_div ? lft.h_shift_left( S, arr )
             : S ==  lft.m_shft_div
               ? base_t( lft.h_shift_left(  lft.m_shft_div, arr )
                         | rgt.h_shift_right( lft.m_shft_div, arr ) )
               : rgt.h_shift_right( S, arr ) )... }};
    } // rotate_left_impl

    template<size_t ... S>
    constexpr
    array_t
    flip_impl( array_t const &arr, std::index_sequence<S...> ) const noexcept
    { return {{ h_flip( S, arr )... }}; }

    constexpr
    base_t
    h_flip( size_t idx, array_t const &arr ) const noexcept
    {
      return   ( idx >= n_words ) ? base_t(0)
             :   ( ~arr[idx] )
               & ( (idx+1 == n_words) ? hgh_bit_pattern : all_one );
    }

    template<size_t ... S>
    constexpr
    array_t
    reverse_impl( array_t const &arr, std::index_sequence<S...> ) const noexcept
    { return {{ h_reverse( S, arr )... }}; }

    constexpr
    base_t
    h_reverse( size_t idx, array_t const &arr ) const noexcept
    {
      return   idx + 1 == n_words
             ? base_t( reverse_bits<base_t>()( arr[0] ) >> n_m_mod )
             : reverse_bits<base_t>()( h2_reverse( idx, arr ) );
    }

    constexpr
    base_t
    h2_reverse( size_t idx, array_t const &arr ) const noexcept
    {
      return   mod_val == 0 ? arr[n_words-idx-1]
             : base_t(   ( arr[n_words-idx-1] << n_m_mod )
                       | ( arr[n_words-idx-2] >> mod_val ) );
    }

    size_t const     m_n_shift_mod;
    size_t const     m_shft_div;
    size_t const     m_shft_mod;
    size_t const     m_shft_leftright_shift;
    base_t const     m_shft_left_pattern;
    base_t const     m_shft_right_pattern;

  }; // struct array_ops

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_ARRAY_OPS_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_ARRAY_COMPLEMENT2_CB_HPP
#define BITSET2_ARRAY_COMPLEMENT2_CB_HPP

namespace Bitset2
{
namespace detail
{

  template<size_t N,size_t it_n,class T>
  struct array_complement2_base
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : base_t {   hgh_bit_pattern= b_chars::hgh_bit_pattern };
    enum : size_t {   n_array=         b_chars::n_array   };
    using array_t=            typename h_types<T>::template array_t<n_array>;

    template<size_t n>
    using arr_t= typename h_types<T>::template array_t<n>;

    constexpr
    arr_t<it_n+1>
    comp2_h1( arr_t<it_n> const &so_far, base_t sum ) const noexcept
    {
      return
        array_funcs<it_n,T>().append( so_far
                                    , ( it_n + 1 < n_array )
                                      ? sum
                                      : base_t(sum & hgh_bit_pattern) );
    }

    constexpr
    arr_t<it_n+1>
    comp2_h2( array_t     const &arr,
              arr_t<it_n> const &so_far,
              base_t            carry ) const noexcept
    { return comp2_h1( so_far, base_t( base_t(~arr[it_n]) + carry ) ); }

    constexpr
    arr_t<it_n+1>
    comp2_h3( array_t     const &arr,
              arr_t<it_n> const &so_far ) const noexcept
    {
      return comp2_h2( arr, so_far,
                       it_n == 0
                       ? base_t(1)
                       : base_t(so_far[it_n-1] == 0 && arr[it_n-1] == 0) );
    }

  }; // struct array_complement2_base

  template<size_t N,size_t it_n,class T>
  struct array_complement2_h : array_complement2_base<N,it_n,T>
  {
    using array_t= typename array_complement2_base<N,it_n,T>::array_t;

    template<size_t n>
    using arr_t= typename array_complement2_base<N,it_n,T>::template arr_t<n>;

    constexpr
    arr_t<it_n+1>
    comp2_h4( array_t const &arr ) const noexcept
    {
      return
        this->comp2_h3( arr,
                        array_complement2_h<N,it_n-1,T>().comp2_h4( arr ) );
    }

  }; // struct array_complement2_h

  template<size_t N,class T>
  struct array_complement2_h<N,0,T> : array_complement2_base<N,0,T>
  {
    using array_t= typename array_complement2_base<N,0,T>::array_t;

    template<size_t n>
    using arr_t= typename array_complement2_base<N,0,T>::template arr_t<n>;

    constexpr
    arr_t<1>
    comp2_h4( array_t const &arr ) const noexcept
    { return this->comp2_h3( arr, arr_t<0>{} ); }

  }; // struct array_complement2_h<N,0,T>

  /// Computes the two's complement of the number encoded in the array
  template<size_t N,class T>
  struct array_complement2
  {
    using base_t=                      T;
    using b_chars=                     bit_chars<N,T>;
    enum : base_t {   hgh_bit_pattern= b_chars::hgh_bit_pattern };
    enum : size_t {   n_array=         b_chars::n_array   };
    using array_t=            typename h_types<T>::template array_t<n_array>;

    constexpr
    array_t
    comp2( array_t const &arr ) const noexcept
    {
      return
        ( N == 0 ) ? array_t{}
                   : array_complement2_h<N,n_array-1,T>().comp2_h4( arr );
    }

    constexpr
    void
    comp2_assgn( array_t &arr ) const noexcept
    {
      base_t  carry= base_t(1);
      for( size_t  c= 0; c < n_array; ++c )
      {
        auto const sm= base_t( base_t(~arr[c]) + carry );
        carry=  base_t( (carry == base_t(1)) && ( sm == 0 ) );
        arr[c]= sm;
      } // for c
      arr[n_array-1] &= hgh_bit_pattern;
    } // comp2_assgn

  }; // struct array_complement2

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_ARRAY_COMPLEMENT2_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_ARRAY2ARRAY_CB_HPP
#define BITSET2_ARRAY2ARRAY_CB_HPP

namespace Bitset2
{
namespace detail
{

/// \brief Convert std::array<Tsrc,src_n> to std::array<Ttrgt,trgt_n>
template<size_t trgt_n, size_t src_n, class Ttrgt, class Tsrc>
struct array2array
{
  using h_t_trgt=           h_types<Ttrgt>;
  using h_t_src=            h_types<Tsrc>;
  using trgt_array_t=       typename h_t_trgt::template array_t<trgt_n>;
  using src_array_t=        typename h_t_src:: template array_t<src_n>;

  enum : size_t
  { trgt_base_n_bits=   h_t_trgt::base_t_n_bits
  , src_base_n_bits=    h_t_src:: base_t_n_bits
  };

  enum : bool
  { small_to_large= ( src_base_n_bits < trgt_base_n_bits ) };

  enum : size_t
  { ts_div= small_to_large ? trgt_base_n_bits / src_base_n_bits
                           : src_base_n_bits  / trgt_base_n_bits };

  enum : Tsrc
  { h_all_set= Tsrc( Ttrgt(~Ttrgt(0)) ) };

  /// Applies pttrn to most significant entry in result
  constexpr
  trgt_array_t
  operator()( Ttrgt pttrn, src_array_t const &src ) const noexcept
  {
    static_assert( ( small_to_large && trgt_base_n_bits % src_base_n_bits == 0)
                || (!small_to_large && src_base_n_bits % trgt_base_n_bits == 0),
                   "Conversion between arrays of these types not supported" );
    return small_to_large
      ? conv_small_to_large( pttrn, src, std::make_index_sequence<trgt_n-1>() )
      : conv_large_to_small( pttrn, src, std::make_index_sequence<trgt_n-1>() );
  }

  template<size_t ... S1>
  constexpr
  trgt_array_t
  conv_small_to_large( Ttrgt                      pttrn,
                       src_array_t const          &src,
                       std::index_sequence<S1...> ) const noexcept
  {
    return {{ get_from_smaller( S1, src, S1 * ts_div )...,
              Ttrgt(   get_from_smaller( trgt_n-1, src, (trgt_n-1) * ts_div )
                     & pttrn ) }};
  }

  template<size_t ... S1>
  constexpr
  trgt_array_t
  conv_large_to_small( Ttrgt                      pttrn,
                       src_array_t const          &src,
                       std::index_sequence<S1...> ) const noexcept
  {
    return
      {{ get_from_larger( S1 / ts_div, S1 % ts_div, src )...,
         Ttrgt( get_from_larger((trgt_n-1) / ts_div, (trgt_n-1) % ts_div, src)
                & pttrn ) }};
  }

  constexpr
  Ttrgt
  get_from_smaller( size_t             trgt_idx,
                    src_array_t const  &src,
                    size_t             src_idx,
                    size_t             src_ct= 0,
                    Ttrgt              so_far= Ttrgt(0) ) const noexcept
  {
    return ( src_ct >= ts_div || src_idx >= src_n )
      ? so_far
      : get_from_smaller( trgt_idx, src, src_idx + 1, src_ct + 1,
                          so_far | Ttrgt(    Ttrgt(src[src_idx])
                                          << (src_base_n_bits * src_ct) ) );
  }

  constexpr
  Ttrgt
  get_from_larger( size_t             div_val,
                   size_t             mod_val,
                   src_array_t const  &src )  const noexcept
  {
    return ( div_val >= src_n ) ? Ttrgt(0)
      : Ttrgt(Tsrc( src[div_val] >> (mod_val*trgt_base_n_bits) ) & h_all_set );
  }
}; // struct array2array

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_ARRAY2ARRAY_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_IMPL_CB_HPP
#define BITSET2_IMPL_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_ULLONG2ARRAY_CB_HPP
#define BITSET2_ULLONG2ARRAY_CB_HPP

namespace Bitset2
{
namespace detail
{

  /// \brief Takes a variable 'v' of type unsigned long long
  /// and returns a std::array 'a' equivalent to v. 'a' represents
  /// an N bit bitset2 with base_t == T.
  template<size_t N,class T>
  struct ullong2array
  {
    using  base_t=    T;
    using  b_c=       bit_chars<N,T>;
    using  ULLONG_t=  typename b_c::ULLONG_t;

    enum : size_t
    { n_bits= N
    , base_t_n_bits=  b_c::base_t_n_bits
    , ullong_n_bits=  b_c::ullong_n_bits
    , n_array=        b_c::n_array
    , centrl_i=       ce_min( (ullong_n_bits-1) / base_t_n_bits, n_array - 1 )
    , n_empty_vals=   n_array - centrl_i - 1
    };

    enum : base_t
    { hgh_bit_pattern= b_c::hgh_bit_pattern
    , use_pattern=     (n_empty_vals==0) ? hgh_bit_pattern : base_t(~base_t(0))
    };

    using array_t= typename h_types<T>::template array_t<n_array>;

    constexpr
    array_t
    operator()( ULLONG_t v ) const noexcept
    {
      return fill( gen_empty_array<n_array,T>(), v,
                   std::make_index_sequence<n_empty_vals>(),
                   std::make_index_sequence<centrl_i>() );
    }

    template<size_t ... S1,size_t ... S2>
    constexpr
    array_t
    fill( array_t const & empty, ULLONG_t v,
          std::index_sequence<S1...>,
          std::index_sequence<S2...> ) const noexcept
    {
      return {{ base_t(ce_right_shift(v,       S2 * base_t_n_bits))...,
                base_t(ce_right_shift(v, centrl_i * base_t_n_bits)&use_pattern),
                empty[S1]... }};
    }
  }; // struct ullong2array

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_ULLONG2ARRAY_CB_HPP

// BITSET2
//
//  Copyright Claas Bontus
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ClaasBontus/bitset2
//

#ifndef BITSET2_ARRAY2U_LONG_T_CB_HPP
#define BITSET2_ARRAY2U_LONG_T_CB_HPP

namespace Bitset2
{
namespace detail
{

  /// \brief Takes a std::array 'arr' and returns a variable 'x' of type Tlong.
  /// Bits in 'x' are set if corresponding bits in 'arr' are set.
  template<size_t N, class T, class Tlong>
  struct array2u_long_t
  {
    using  base_t=    T;
    using  b_c=       bit_chars<N,T>;

    enum : size_t
    { n_bits= N
    , base_t_n_bits=  b_c::base_t_n_bits
    , long_t_n_bits=  sizeof( Tlong ) * CHAR_BIT
    , n_array=        b_c::n_array
    , div_val=        long_t_n_bits / base_t_n_bits
    , mod_val=        long_t_n_bits % base_t_n_bits
    , use_vals=       ce_min( div_val + (mod_val!=0), n_array )
    , bit_diff=       mod_val == 0 ? 0 : ( base_t_n_bits - mod_val )
    };

    enum : base_t
    { allset=  base_t( ~base_t(0) )
    , h_pttrn= use_vals < n_array ? allset : ce_right_shift( allset, bit_diff )
    , i_pttrn= base_t( ~h_pttrn )
    };

    using array_t= typename h_types<T>::template array_t<n_array>;

    constexpr
    Tlong
    operator()( array_t const & arr ) const noexcept
    {
      return
        base_t_n_bits >= long_t_n_bits ? Tlong( arr[0] )
                                       : combine( Tlong(0), arr, 0 );
    }

    /// \brief Returns true if arr cannot be converted to Tlong.
    constexpr
    bool
    check_overflow( array_t const & arr ) const noexcept
    { return N <= long_t_n_bits ? false : check_impl( arr, use_vals - 1 ); }

    constexpr
    Tlong
    combine( Tlong v, array_t const & arr, size_t idx ) const noexcept
    {
      return ( idx >= use_vals ) ? v
               : idx + 1 == use_vals
                 ? Tlong( v + take_v( arr, idx, h_pttrn ) )
                 : combine( Tlong( v + take_v( arr, idx ) ), arr, idx + 1 );
    }

    constexpr
    Tlong
    take_v( array_t const & arr, size_t idx, base_t pttrn= allset ) const noexcept
    { return ce_left_shift( Tlong( arr[idx] & pttrn ), idx * base_t_n_bits ); }

    constexpr
    bool
    check_impl( array_t const & arr, size_t idx ) const noexcept
    {
      return idx >= n_array ? false
             : ( ( take_check( arr, idx ) != base_t(0) )
                 || check_impl( arr, idx + 1 ) );
    }

    constexpr
    base_t
    take_check( array_t const & arr, size_t idx ) const noexcept
    { return arr[idx] & ( idx + 1 == use_vals ? i_pttrn : allset ); }

  }; // struct array2u_long_t

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_ARRAY2U_LONG_T_CB_HPP

#include <bitset>
#include <stdexcept>

namespace Bitset2
{
namespace detail
{

template<size_t N,class T>
class bitset2_impl
{
  using b_chars=                     bit_chars<N,T>;
  using h_t=                         h_types<T>;

public:
  enum : size_t { n_array= b_chars::n_array
                , npos=    h_t::npos };
  using base_t=      T;
  using ULONG_t=     typename b_chars::ULONG_t;
  using ULLONG_t=    typename b_chars::ULLONG_t;
  using array_t=     typename h_types<T>::template array_t<n_array>;

protected:
  enum : size_t
  { n_words=         b_chars::n_words
  , ulong_n_bits=    b_chars::ulong_n_bits
  , ullong_n_bits=   b_chars::ullong_n_bits
  , base_t_n_bits=   b_chars::base_t_n_bits
  };
  enum : ULONG_t
  { ulong_max=       b_chars::ulong_max };
  enum : base_t
  { low_bit_pattern= b_chars::low_bit_pattern
  , hgh_bit_pattern= b_chars::hgh_bit_pattern
  , all_one=         b_chars::all_one
  };

  template<size_t n_arr_src, class Tsrc>
  using a2a= detail::array2array<n_array, n_arr_src,T,Tsrc>;

  /* ----------------------------------------------------------------------- */
  constexpr
  bitset2_impl() noexcept
  {}

  constexpr
  bitset2_impl( bitset2_impl const & ) noexcept= default;

  constexpr
  bitset2_impl( bitset2_impl && ) noexcept= default;

  constexpr
  bitset2_impl &
  operator=( bitset2_impl const & ) noexcept= default;

  constexpr
  bitset2_impl &
  operator=( bitset2_impl && ) noexcept= default;

  explicit
  constexpr
  bitset2_impl( ULLONG_t v ) noexcept
  : m_value( ullong2array<N,T>()( v ) )
  {}

  template<size_t n,class Tsrc>
  explicit
  constexpr
  bitset2_impl( std::array<Tsrc,n> const & value ) noexcept
  : m_value( a2a<n,Tsrc>()( hgh_bit_pattern, value ) )
  {}

  explicit
  bitset2_impl( const std::bitset<N> &bs ) noexcept
  {
    if( N == 0 ) return;
    if( ullong_n_bits <= base_t_n_bits && n_words == 1 )
    {
      m_value[0]= bs.to_ullong();
      return;
    }

    size_t offset= 0;
    for( size_t ct= 0; ct < n_words; ++ct )
    {
      base_t  val= base_t(0);
      auto const bit_limit=
                 ( ct < n_words - 1 ) ? base_t_n_bits : N - offset;
      for( size_t bit_ct= 0; bit_ct < bit_limit; ++bit_ct )
      {
        auto const test_bit= offset + bit_limit - bit_ct - 1;
        val <<= 1;
        if( bs.test( test_bit ) ) val |= base_t(1);
      } // for bit_ct
      m_value[ct]= val;
      offset += base_t_n_bits;
    } // for ct
  } // bitset2_impl( const std::bitset<N> &bs )

  template< class CharT, class Traits, class Alloc >
  explicit
  bitset2_impl( std::basic_string<CharT,Traits,Alloc> const      &str,
                typename
                std::basic_string<CharT,Traits,Alloc>::size_type pos,
                typename
                std::basic_string<CharT,Traits,Alloc>::size_type n,
                CharT                                            zero,
                CharT                                            one )
  {
    auto const str_sz= str.size();
    if( pos > str_sz )
            throw std::out_of_range( "bitset2: String submitted to "
                                     "constructor smaller than pos" );
    auto const n_bits= std::min( N, std::min( n, str_sz - pos ) );

    for( size_t bit_ct= 0; bit_ct < n_bits; ++bit_ct )
    {
      auto const chr= str[bit_ct+pos];
      if(       Traits::eq( one,  chr ) ) set( n_bits - bit_ct - 1 );
      else if( !Traits::eq( zero, chr ) )
              throw std::invalid_argument( "bitset2: Invalid argument in "
                                           "string submitted to constructor" );
    } // for bit_ct
  }
  /* ----------------------------------------------------------------------- */

  //**********************************************************

  constexpr
  array_t &
  get_data() noexcept
  { return m_value; }

  constexpr
  bool
  operator[]( size_t bit ) const noexcept
  { return test_noexcept( bit ); }

  constexpr
  bool
  test_noexcept( size_t bit ) const noexcept
  { return m_value[bit / base_t_n_bits] & ( T(1) << ( bit % base_t_n_bits ) ); }

  constexpr
  bitset2_impl &
  set( size_t bit, bool value= true )
  {
    if( bit >= N  )
            throw std::out_of_range( "bitset2: Setting of bit out of range" );
    set_noexcept( bit, value );
    return *this;
  } // set

  constexpr
  bitset2_impl &
  set() noexcept
  {
    if( N > 0 )
    {
      size_t c= 0;
      for( ; c < n_words - 1; ++c ) m_value[c]= ~base_t(0);
      m_value[c]= hgh_bit_pattern;
    }
    return *this;
  } // set

  constexpr
  bitset2_impl &
  reset() noexcept
  {
    for( size_t c= 0; c < n_array; ++c ) m_value[c]= base_t(0);
    return *this;
  }

  constexpr
  bool
  test_set( size_t bit, bool value= true )
  {
    if( bit >= N  )
        throw std::out_of_range( "bitset2: test_set out of range" );
    return test_set_noexcept( bit, value );
  } // test_set

  constexpr
  bitset2_impl &
  flip_noexcept( size_t bit ) noexcept
  {
    m_value[bit / base_t_n_bits] ^= ( base_t(1) << ( bit % base_t_n_bits ) );
    return *this;
  }

  constexpr
  bitset2_impl &
  flip( size_t bit )
  {
    if( bit >= N  )
            throw std::out_of_range( "bitset2: Flipping of bit out of range" );
    return flip_noexcept( bit );
  } // flip

  constexpr
  bitset2_impl &
  flip() noexcept
  {
    if( N > 0 )
    {
      size_t c= 0;
      for( ; c < n_words - 1; ++c ) m_value[c] ^= ~base_t(0);
      m_value[c] ^= hgh_bit_pattern;
    }
    return *this;
  } // flip

public:
  constexpr
  array_t const &
  data() const noexcept
  { return m_value; }

  constexpr
  ULONG_t
  to_ulong() const
  {
    using a2l= array2u_long_t<N,T,ULONG_t>;
    return  ( N == 0 ) ? 0ul
            : a2l().check_overflow( m_value )
              ? throw std::overflow_error( "Cannot convert bitset2 "
                                           "to unsigned long" )
              : a2l()( m_value );
  } // to_ulong

  constexpr
  ULLONG_t
  to_ullong() const
  {
    using a2l= array2u_long_t<N,T,ULLONG_t>;
    return  ( N == 0 ) ? 0ull
            : a2l().check_overflow( m_value )
              ? throw std::overflow_error( "Cannot convert bitset2 "
                                           "to unsigned long long" )
              : a2l()( m_value );
  } // to_ullong

  constexpr
  bool
  test( size_t bit ) const
  {
    return  ( bit >= N )
            ? throw std::out_of_range( "bitset2: Testing of bit out of range" )
            : operator[]( bit );
  }

  constexpr
  void
  set_noexcept( size_t bit, bool value= true ) noexcept
  {
    if( value ) m_value[bit / base_t_n_bits]
                   |=  base_t(   base_t(1) << ( bit % base_t_n_bits )  );
    else        m_value[bit / base_t_n_bits]
                   &=  base_t(~( base_t(1) << ( bit % base_t_n_bits ) ));
  }

  constexpr
  bool
  test_set_noexcept( size_t bit, bool value= true ) noexcept
  {
    auto const dv= bit / base_t_n_bits;
    auto const md= bit % base_t_n_bits;
    auto const pttrn= ( base_t(1) << md );
    auto const ret_val= bool( m_value[dv] & pttrn );

    if( value ) m_value[dv] |=  pttrn;
    else        m_value[dv] &= ~pttrn;

    return ret_val;
  } // test_set_noexcept

  constexpr
  bool
  none() const noexcept
  { return detail::array_funcs<n_array,T>().none( m_value ); }

  constexpr
  bool
  any() const noexcept
  { return ( N > 0 ) && !none(); }

  constexpr
  bool
  all() const noexcept
  { return ( N > 0 ) && detail::array_ops<N,T>( 0 ).all( m_value ); }

  constexpr
  size_t
  count() const noexcept
  { return detail::array_funcs<n_array,T>().count( m_value ); }

  /// \brief Returns index of first (least significant) bit set.
  /// Returns npos if all bits are zero.
  constexpr
  size_t
  find_first() const noexcept
  {
    return detail::array_funcs<n_array,T>().idx_lsb_set(m_value, m_value[0], 0);
  }

  /// \brief Returns index of next (> idx) bit set.
  /// Returns npos if no more bits set.
  /// Throws out_of_range if idx >= N.
  constexpr
  size_t
  find_next( size_t idx ) const
  {
    return idx >= N
      ? throw std::out_of_range( "bitset2: find_next index out of range" )
      : idx + 1 == N
        ? npos
        : detail::array_funcs<n_array,T>()
            .idx_lsb_set( m_value,
                          base_t( m_value[(idx+1) / base_t_n_bits]
                            & ce_left_shift(T(~T(0)),(idx+1) % base_t_n_bits) ),
                          (idx+1) / base_t_n_bits );
  }

  constexpr
  bool
  operator==( bitset2_impl const &v2 ) const noexcept
  { return detail::array_funcs<n_array,T>().equal( m_value, v2.m_value ); }

  constexpr
  bool
  operator!=( bitset2_impl const &v2 ) const noexcept
  { return !( *this == v2 ); }

  constexpr
  bool
  operator<( bitset2_impl const &v2 ) const noexcept
  { return detail::array_funcs<n_array,T>().less_than( m_value, v2.m_value ); }

  constexpr
  bool
  operator<=( bitset2_impl const &v2 ) const noexcept
  { return ! ( *this > v2 ); }

  constexpr
  bool
  operator>( bitset2_impl const &v2 ) const noexcept
  { return detail::array_funcs<n_array,T>().less_than(v2.m_value, m_value); }

  constexpr
  bool
  operator>=( bitset2_impl const &v2 ) const noexcept
  { return ! ( *this < v2 ); }

  explicit
  operator std::bitset<N>() const
  {
    using b_t= std::bitset<N>;
    if( N == 0 ) return b_t{};
    if( n_words == 1 ) return b_t( to_ullong() );

    b_t  ret_val;
    for( size_t ct= 0; ct < N; ++ct ) ret_val[ct]= operator[](ct);

    return ret_val;
  }
private:
  array_t   m_value= (detail::gen_empty_array<n_array,T>)();
}; // class bitset2_impl

} // namespace detail
} // namespace Bitset2

#endif // BITSET2_IMPL_CB_HPP

#include <stdexcept>
#include <bitset>
#include <climits>
#include <cstdint>
#include <array>
#include <stdexcept>
#include <utility>
#include <string>
#include <functional>
#include <type_traits>

namespace Bitset2
{

template<size_t N,
         class T= Bitset2::detail::select_base_t<N>,
         class Enabled=void> class bitset2;

template<size_t N,class T>
class bitset2<N,T,
              typename std::enable_if<   std::is_integral<T>::value
                                      && std::is_unsigned<T>::value>::type>
: public detail::bitset2_impl<N,T>
{
  enum : size_t { base_t_n_bits= detail::bitset2_impl<N,T>::base_t_n_bits };
public:
  using array_t=  typename detail::bitset2_impl<N,T>::array_t;
  using ULLONG_t= typename detail::bitset2_impl<N,T>::ULLONG_t;
  using base_t=   T;
  using detail::bitset2_impl<N,T>::n_array;

  enum : size_t { npos= detail::h_types<T>::npos };

  class reference
  {
    friend class bitset2;
    reference() noexcept {}
    reference( bitset2<N,T> *ptr, size_t bit ) noexcept
    : m_ptr( ptr )
    , m_bit( bit )
    {}
    bitset2<N,T> *m_ptr= nullptr;
    size_t       m_bit;
  public:
    ~reference() noexcept {}
    reference& operator=( bool x ) noexcept
    {
      m_ptr->set_noexcept( m_bit, x );
      return *this;
    }
    reference& operator=( reference const & r ) noexcept
    {
      m_ptr->set_noexcept( m_bit, bool( r ) );
      return *this;
    }
    reference& flip() noexcept
    {
      m_ptr->flip_noexcept( m_bit );
      return *this;
    }
    operator bool() const noexcept
    { return m_ptr->test_noexcept(m_bit); }
    bool operator~() const noexcept
    { return !bool(*this); }
  }; // class reference

  /* ------------------------------------------------------------- */
  constexpr
  bitset2() noexcept
  : detail::bitset2_impl<N,T>()
  {}

  constexpr
  bitset2( bitset2 const & ) noexcept= default;

  constexpr
  bitset2( bitset2 && ) noexcept= default;

  constexpr
  bitset2 &
  operator=( bitset2 const & ) noexcept= default;

  constexpr
  bitset2 &
  operator=( bitset2 && ) noexcept= default;

  explicit
  bitset2( const std::bitset<N> &bs ) noexcept
  : detail::bitset2_impl<N,T>( bs )
  {}

  explicit
  constexpr
  bitset2( ULLONG_t v ) noexcept
  : detail::bitset2_impl<N,T>( v )
  {}

  template<size_t n,class Tsrc>
  explicit
  constexpr
  bitset2( std::array<Tsrc,n> const & value ) noexcept
  : detail::bitset2_impl<N,T>( value )
  {}

  template< class CharT, class Traits, class Alloc >
  explicit
  bitset2( std::basic_string<CharT,Traits,Alloc> const
                    & str,
           typename std::basic_string<CharT,Traits,Alloc>::size_type
                    pos = 0,
           typename std::basic_string<CharT,Traits,Alloc>::size_type
                    n = std::basic_string<CharT,Traits,Alloc>::npos,
           CharT    zero= CharT('0'),
           CharT    one=  CharT('1') )
  : detail::bitset2_impl<N,T>( str, pos, n, zero, one )
  {}

  template< class CharT >
  explicit
  bitset2( const CharT        *str,
           typename std::basic_string<CharT>::size_type
                              n=     std::basic_string<CharT>::npos,
           CharT              zero=  CharT('0'),
           CharT              one=   CharT('1') )
  : detail::bitset2_impl<N,T>( n == std::basic_string<CharT>::npos
                               ? std::basic_string<CharT>( str )
                               : std::basic_string<CharT>( str, n ),
                               0, n, zero, one )
  {}
  /* ------------------------------------------------------------- */

  //****************************************************

  /// Bitwise NOT
  constexpr
  bitset2
  operator~() const noexcept
  { return bitset2(detail::array_ops<N,T>( 0 ).flip(this->data())); }

  constexpr
  bool
  operator[]( size_t bit ) const noexcept
  { return detail::bitset2_impl<N,T>::operator[]( bit ); }

  reference
  operator[]( size_t bit ) noexcept
  { return reference( this, bit ); }

  constexpr
  bitset2 &
  operator<<=( size_t n_shift ) noexcept
  {
    detail::array_ops<N,T>( n_shift ).shift_left_assgn( this->get_data() );
    return *this;
  }

  constexpr
  bitset2 &
  operator>>=( size_t n_shift ) noexcept
  {
    detail::array_ops<N,T>( n_shift ).shift_right_assgn( this->get_data() );
    return *this;
  }

  constexpr
  bitset2 &
  rotate_left( size_t n_rot ) noexcept
  {
    this->get_data()= detail::array_ops<N,T>(n_rot).rotate_left( this->data() );
    return *this;
  }

  constexpr
  bitset2 &
  rotate_right( size_t n_rot ) noexcept
  {
    this->get_data()= detail::array_ops<N,T>( N - ( n_rot % N ) )
                                        .rotate_left( this->data() );
    return *this;
  }

  constexpr
  bitset2 &
  reverse() noexcept
  {
    this->get_data()= detail::array_ops<N,T>( 0 ).reverse( this->data() );
    return *this;
  }

  /// Computes two's complement
  constexpr
  bitset2 &
  complement2() noexcept
  {
    detail::array_complement2<N,T>().comp2_assgn( this->get_data() );
    return *this;
  }

  constexpr
  bitset2 &
  operator+=( bitset2 const &bs2 ) noexcept
  {
    detail::array_add<N,T>().add_assgn(this->get_data(), bs2.data());
    return *this;
  }

  constexpr
  bitset2 &
  operator++() noexcept
  {
    detail::array_ops<N,T>(0).increment( this->get_data() );
    return *this;
  }

  constexpr
  bitset2
  operator++(int) noexcept
  {
    bitset2 tmp( *this );
    operator++();
    return tmp;
  }

  constexpr
  bitset2 &
  operator--() noexcept
  {
    detail::array_ops<N,T>(0).decrement( this->get_data() );
    return *this;
  }

  constexpr
  bitset2
  operator--(int) noexcept
  {
    bitset2 tmp( *this );
    operator--();
    return tmp;
  }

  constexpr
  bitset2 &
  operator|=( bitset2 const & v2 ) noexcept
  {
    detail::array_funcs<bitset2::n_array,T>()
             .bitwise_or_assgn(this->get_data(), v2.data() );
    return *this;
  }

  constexpr
  bitset2 &
  operator&=( bitset2 const & v2 ) noexcept
  {
    detail::array_funcs<bitset2::n_array,T>()
              .bitwise_and_assgn( this->get_data(), v2.data() );
    return *this;
  }

  constexpr
  bitset2 &
  operator^=( bitset2 const & v2 ) noexcept
  {
    detail::array_funcs<bitset2::n_array,T>()
              .bitwise_xor_assgn( this->get_data(), v2.data() );
    return *this;
  }

  /// Computes the set difference, i.e. *this &= ~v2
  constexpr
  bitset2 &
  difference( bitset2 const & v2 ) noexcept
  {
    detail::array_funcs<bitset2::n_array,T>()
              .bitwise_setdiff_assgn( this->get_data(), v2.data() );
    return *this;
  }

  constexpr
  bitset2 &
  set() noexcept
  { detail::bitset2_impl<N,T>::set(); return *this; }

  constexpr
  bitset2 &
  set( size_t bit, bool value= true )
  { detail::bitset2_impl<N,T>::set( bit, value ); return *this; }

  constexpr
  bitset2 &
  reset() noexcept
  { detail::bitset2_impl<N,T>::reset(); return *this; }

  constexpr
  bitset2 &
  reset( size_t bit )
  {
    if( bit >= N  ) throw std::out_of_range( "bitset2: reset out of range" );
   return set( bit, false );
 }

 /// \brief Sets the specified bit if value==true,
 /// clears it otherwise. Returns the previous state of the bit.
 constexpr
 bool
 test_set( size_t bit, bool value= true )
 { return detail::bitset2_impl<N,T>::test_set( bit, value ); }

 constexpr
 bitset2 &
 flip() noexcept
 { detail::bitset2_impl<N,T>::flip(); return *this; }

 constexpr
 bitset2 &
 flip( size_t bit )
 { detail::bitset2_impl<N,T>::flip( bit ); return *this; }

 constexpr std::size_t size() const noexcept { return N; }

 template<class CharT = char,
          class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT> >
 std::basic_string<CharT,Traits,Allocator>
 to_string( CharT zero = CharT('0'), CharT one = CharT('1') ) const
 {
   std::basic_string<CharT,Traits,Allocator> ret_val;
   ret_val.reserve( N );
   for( size_t ct= N; ct > 0; )
   {
     --ct;
     ret_val += this->operator[]( ct ) ? one : zero;
   }
   return ret_val;
 } // to_string

 template<class CharT = char,
          class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>,
          typename std::enable_if<base_t_n_bits % 4 == 0>::type* = nullptr >
 std::basic_string<CharT,Traits,Allocator>
 to_hex_string( hex_params<CharT,Traits,Allocator> const &params=
                hex_params<CharT,Traits,Allocator>{} ) const
 {
   using arr_acc=  detail::array_access<N,T>;
   arr_acc         a_a;
   constexpr auto  div_four= arr_acc::div_four;
   constexpr auto  mod_four= arr_acc::mod_four;
   constexpr auto  n_char= div_four + ( mod_four > 0 );

   auto const      zeroCh= params.zeroCh;
   auto const      aCh=    params.aCh;

   std::basic_string<CharT,Traits,Allocator> ret_val;
   ret_val.reserve( n_char + params.prefix.size() );
   ret_val= params.prefix;
   size_t ct= n_char;
   if( !params.leadingZeroes )
   {
     for( ; ct > 0; --ct )
     {
       auto  const val= a_a.get_four_bits( this->data(), 4 * ct - 1 );
       if( val != 0 ) break;
     }
   }
   if( ct == 0 && params.nonEmpty ) ret_val += zeroCh;
   for( ; ct > 0; --ct )
   {
     auto  const val= a_a.get_four_bits( this->data(), 4 * ct - 1 );
     CharT const c=
     ( val < 10 ) ? ( zeroCh + val ) : ( aCh + ( val - 10 ) );
     ret_val += c;
   }
   return ret_val;
 } // to_hex_string

}; // class bitset2

template<size_t N, class T>
constexpr
bitset2<N,T>
rotate_left( bitset2<N,T> const & bs, size_t n_rot ) noexcept
{
  return
    bitset2<N,T>( detail::array_ops<N,T>( n_rot ).rotate_left( bs.data() ) );
}

template<size_t N, class T>
constexpr
bitset2<N,T>
rotate_right( bitset2<N,T> const & bs, size_t n_rot ) noexcept
{
  return
    bitset2<N,T>( detail::array_ops<N,T>( N - ( n_rot % N ) ).
                                               rotate_left( bs.data() ) );
}

/// Computes the set difference, i.e. bs1 & ~bs2
template<size_t N, class T>
constexpr
bitset2<N,T>
difference( bitset2<N,T> const & bs1, bitset2<N,T> const & bs2 ) noexcept
{
  return
    bitset2<N,T>( detail::array_funcs<bitset2<N,T>::n_array,T>()
                            .bitwise_setdiff( bs1.data(), bs2.data() ) );
}

/// Returns bs with bits reversed
template<size_t N, class T>
constexpr
bitset2<N,T>
reverse( bitset2<N,T> const & bs ) noexcept
{ return bitset2<N,T>( detail::array_ops<N,T>( 0 ).reverse( bs.data() ) ); }

/// Computes the two's complement
template<size_t N, class T>
constexpr
bitset2<N,T>
complement2( bitset2<N,T> const & bs ) noexcept
{ return bitset2<N,T>( detail::array_complement2<N,T>().comp2(bs.data()) ); }

/// Converts an M-bit bitset2 to an N-bit bitset2.
template<size_t N,class T1,size_t M, class T2>
constexpr
bitset2<N,T1>
convert_to( bitset2<M,T2> const & bs ) noexcept
{
  using a2a=
    detail::array2array<bitset2<N,T1>::n_array,bitset2<M,T2>::n_array,T1,T2>;
  return
    bitset2<N,T1>(a2a()(detail::bit_chars<N,T1>::hgh_bit_pattern, bs.data()));
}

/// Converts an M-bit bitset2 to an N-bit bitset2.
template<size_t N,size_t M, class T>
constexpr
bitset2<N,T>
convert_to( bitset2<M,T> const & bs ) noexcept
{ return bitset2<N,T>( bs.data() ); }

/// \brief Returns true if f returns true for each pair
/// of base_t=T values in bs1 and bs2. f should be a binary function
/// taking two base_t values and returning bool.
/// zip_fold_and does short circuit if possible.
template<size_t N, class F, class T>
constexpr
bool
zip_fold_and( bitset2<N,T> const & bs1, bitset2<N,T> const & bs2,
              F f ) noexcept(noexcept( f( T(0), T(0) ) ))
{
  return
    detail::array_funcs<bitset2<N,T>::n_array,T>().zip_fold_and(bs1.data(),
                                                                bs2.data(), f);
}

/// \brief Returns true if f returns true for at least one pair
/// of base_t=T values in bs1 and bs2. f should be a binary function
/// taking two base_t values and returning bool.
/// zip_fold_or does short circuit if possible.
template<size_t N, class F,class T>
constexpr
bool
zip_fold_or( bitset2<N,T> const & bs1, bitset2<N,T> const & bs2,
             F f ) noexcept(noexcept( f( T(0), T(0) ) ))
{
  return
    detail::array_funcs<bitset2<N,T>::n_array,T>().zip_fold_or( bs1.data(),
                                                                bs2.data(), f );
}

} // namespace Bitset2

/// Stream output
template <class CharT, class Traits, size_t N,class T>
std::basic_ostream<CharT, Traits>&
operator<<( std::basic_ostream<CharT, Traits> & os,
            Bitset2::bitset2<N,T> const & x )
{
  for( size_t ct= N; ct > 0; )
  {
     --ct;
     os << ( x[ct] ? "1" : "0" );
  }
  return os;
}

/// Stream input
template <class CharT, class Traits, size_t N,class T>
std::basic_istream<CharT, Traits>&
operator>>( std::basic_istream<CharT, Traits> & is,
            Bitset2::bitset2<N,T> & x )
{
  std::bitset<N>  bs;
  is >> bs;
  x= Bitset2::bitset2<N,T>( bs );
  return is;
}

/// Shift left
template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator<<( Bitset2::bitset2<N,T> const & bs, size_t n_shift ) noexcept
{
  return
    Bitset2::bitset2<N,T>( Bitset2::detail::array_ops<N,T>( n_shift )
                                                 .shift_left( bs.data() ) );
}

/// Shift right
template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator>>( Bitset2::bitset2<N,T> const & bs, size_t n_shift ) noexcept
{
  return
    Bitset2::bitset2<N,T>( Bitset2::detail::array_ops<N,T>( n_shift )
                                                 .shift_right( bs.data() ) );
}

template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator|( Bitset2::bitset2<N,T> const & bs1,
           Bitset2::bitset2<N,T> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N,T>(
      Bitset2::detail::array_funcs<Bitset2::bitset2<N,T>::n_array,T>()
                                    .bitwise_or( bs1.data(), bs2.data() ) );
}

template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator&( Bitset2::bitset2<N,T> const & bs1,
           Bitset2::bitset2<N,T> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N,T>(
      Bitset2::detail::array_funcs<Bitset2::bitset2<N,T>::n_array,T>()
                                  .bitwise_and( bs1.data(), bs2.data() ) );
}

template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator^( Bitset2::bitset2<N,T> const & bs1,
           Bitset2::bitset2<N,T> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N,T>(
      Bitset2::detail::array_funcs<Bitset2::bitset2<N,T>::n_array,T>()
                                 .bitwise_xor( bs1.data(), bs2.data() ) );
}

template<size_t N,class T>
constexpr
Bitset2::bitset2<N,T>
operator+( Bitset2::bitset2<N,T> const & bs1,
           Bitset2::bitset2<N,T> const & bs2 ) noexcept
{
  return
    Bitset2::bitset2<N,T>(
      Bitset2::detail::array_add<N,T>().add( bs1.data(), bs2.data() ) );
}

namespace std
{
  template<size_t N,class T>
  struct hash<Bitset2::bitset2<N,T> >
  {
  private:
    enum : size_t
    { n_array= Bitset2::detail::bitset2_impl<N,T>::n_array };

    Bitset2::detail::hash_impl<n_array,T>   m_func;

  public:
    using argument_type= Bitset2::bitset2<N,T>;
    using result_type=
          typename Bitset2::detail::hash_impl<n_array,T>::result_type;

    result_type operator()( argument_type const& bs ) const
    { return m_func( bs.data() ); }
  }; // struct hash

} // namespace std

#endif // BITSET2_CB_HPP
