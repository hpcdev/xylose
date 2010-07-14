/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 *                                                                             *
 * This file is part of xylose                                                 *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/



#define BOOST_TEST_MODULE bits

#include <xylose/bits.hpp>

#include <boost/test/unit_test.hpp>

#include <string>
#include <iostream>
#include <iterator>
#include <sstream>

#include <stdint.h>

namespace {

  template < typename T >
  size_t test_size() {
    std::vector<T> bytes;
    bytes.resize(1);
    return xylose::bits::size(bytes);
  }

  template < typename T >
  size_t test_resize() {
    std::vector<T> bytes;
    xylose::bits::resize(bytes, 65);
    return xylose::bits::size(bytes);
  }

  template < typename T >
  size_t test_reserve() {
    std::vector<T> bytes;
    xylose::bits::reserve(bytes, 65);
    BOOST_CHECK_EQUAL( bytes.size(), 0u );
    return bytes.capacity() * 8u * sizeof(T);
  }


  template < typename T >
  T test_set() {
    std::vector<T> bytes;
    bytes.resize(2); // one set of bits

    xylose::bits::set(bytes, 1);
    xylose::bits::set(bytes, 7);
    xylose::bits::set(bytes,      sizeof(T) * 8u - 1u);
    xylose::bits::set(bytes,      sizeof(T) * 8u + 1u);
    xylose::bits::set(bytes,      sizeof(T) * 8u + 7u);
    xylose::bits::set(bytes, 2u * sizeof(T) * 8u - 1u);

    BOOST_CHECK_EQUAL( bytes[0], bytes[1] );
    return bytes[0];
  }

  template < typename T >
  T test_unset() {
    std::vector<T> bytes;
    bytes.push_back( ~T(0) );
    bytes.push_back( ~T(0) );

    xylose::bits::unset(bytes, 1);
    xylose::bits::unset(bytes, 7);
    xylose::bits::unset(bytes,      sizeof(T) * 8u - 1u);
    xylose::bits::unset(bytes,      sizeof(T) * 8u + 1u);
    xylose::bits::unset(bytes,      sizeof(T) * 8u + 7u);
    xylose::bits::unset(bytes, 2u * sizeof(T) * 8u - 1u);

    BOOST_CHECK_EQUAL( bytes[0], bytes[1] );
    return bytes[0];
  }

  template < typename T >
  void test_setAll() {
    std::vector<T> bytes;
    bytes.push_back( T(0x1) );
    bytes.push_back( T(0x2) );
    bytes.push_back( T(0x3) );
    bytes.push_back( T(0x4) );

    xylose::bits::setAll( bytes );

    BOOST_CHECK_EQUAL( bytes[0], T(~T(0)) );
    BOOST_CHECK_EQUAL( bytes[1], T(~T(0)) );
    BOOST_CHECK_EQUAL( bytes[2], T(~T(0)) );
    BOOST_CHECK_EQUAL( bytes[3], T(~T(0)) );
  }

  template < typename T >
  void test_unsetAll() {
    std::vector<T> bytes;
    bytes.push_back( T(0x1) );
    bytes.push_back( T(0x2) );
    bytes.push_back( T(0x3) );
    bytes.push_back( T(0x4) );

    xylose::bits::unsetAll( bytes );

    BOOST_CHECK_EQUAL( bytes[0], T(0) );
    BOOST_CHECK_EQUAL( bytes[1], T(0) );
    BOOST_CHECK_EQUAL( bytes[2], T(0) );
    BOOST_CHECK_EQUAL( bytes[3], T(0) );
  }

  template < typename T >
  size_t test_test() {
    namespace bits = xylose::bits;
    std::vector<T> bytes;
    bytes.resize(2); // one set of bits

    bits::set(bytes, 1u);
    bits::set(bytes, 7u);
    bits::set(bytes,      sizeof(T) * 8u - 1u);
    bits::set(bytes,      sizeof(T) * 8u + 1u);
    bits::set(bytes,      sizeof(T) * 8u + 7u);
    bits::set(bytes, 2u * sizeof(T) * 8u - 1u);

    BOOST_CHECK_EQUAL( bytes[0], bytes[1] );

    BOOST_CHECK_EQUAL( bits::test( bytes, 0u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, 1u ), true );
    BOOST_CHECK_EQUAL( bits::test( bytes, 2u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, 3u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, 4u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, 5u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, 6u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, 7u ), true );

    size_t n_unset = 6u;

    for ( size_t i =                  8u,
                 f = sizeof(T) * 8u - 1u;
          i < f; ++i, ++n_unset ) {
      BOOST_CHECK_EQUAL( bits::test( bytes, i ), false );
    }

    BOOST_CHECK_EQUAL( bits::test( bytes, sizeof(T) * 8u + 0u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, sizeof(T) * 8u + 1u ), true );
    BOOST_CHECK_EQUAL( bits::test( bytes, sizeof(T) * 8u + 2u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, sizeof(T) * 8u + 3u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, sizeof(T) * 8u + 4u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, sizeof(T) * 8u + 5u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, sizeof(T) * 8u + 6u ), false );
    BOOST_CHECK_EQUAL( bits::test( bytes, sizeof(T) * 8u + 7u ), true );

    n_unset += 6u;

    for ( size_t i =      sizeof(T) * 8u + 8u,
                 f = 2u * sizeof(T) * 8u - 1u;
          i < f; ++i, ++n_unset ) {
      BOOST_CHECK_EQUAL( bits::test( bytes, i ), false );
    }

    return n_unset;
  }

  template < typename T >
  void check_ostream( const std::string & ans ) {
    //using xylose::bits::operator<<;
    //temporary change in using to support older gcc 3.2
    using namespace xylose::bits;
    std::vector<T> bytes;
    bytes.push_back(0xff);

    std::ostringstream ostr;
    ostr << bytes;
    BOOST_CHECK_EQUAL( ostr.str(), ans );
  }

  template < typename T >
  void check_istream( const std::string & ans ) {
    //using xylose::bits::operator<<;
    //using xylose::bits::operator>>;
    //temporary change in using to support older gcc 3.2
    using namespace xylose::bits;
    std::vector<T> bytes;
    std::istringstream istr("10110011" "10001111" "00001111" "10000011" "1111");
    istr >> bytes;

    std::ostringstream ostr;
    ostr << bytes;
    BOOST_CHECK_EQUAL( ostr.str(), ans );
  }

  template < typename T >
  void check_reverse( const std::string & ans ) {
    //using xylose::bits::operator<<;
    //using xylose::bits::operator>>;
    //temporary change in using to support older gcc 3.2
    using namespace xylose::bits;
    std::vector<T> bytes;
    std::istringstream istr("10110011" "10001111" "00001111" "10000011" "1111");
    istr >> bytes;

    bytes = xylose::bits::reverse(bytes);

    std::ostringstream ostr;
    ostr << bytes;
    BOOST_CHECK_EQUAL( ostr.str(), ans );
  }



  BOOST_AUTO_TEST_CASE( numT_function ) {
    using xylose::bits::numT;
    BOOST_CHECK_EQUAL( numT<uint8_t>(0u), 0u );

    BOOST_CHECK_EQUAL( numT<uint8_t>(1u), 1u );
    BOOST_CHECK_EQUAL( numT<uint8_t>(7u), 1u );
    BOOST_CHECK_EQUAL( numT<uint8_t>(8u), 1u );
    BOOST_CHECK_EQUAL( numT<uint8_t>(9u), 2u );
    BOOST_CHECK_EQUAL( numT<uint8_t>(16u), 2u );
    BOOST_CHECK_EQUAL( numT<uint8_t>(17u), 3u );

    BOOST_CHECK_EQUAL( numT<uint16_t>(0u), 0u );
    BOOST_CHECK_EQUAL( numT<uint16_t>(1u), 1u );
    BOOST_CHECK_EQUAL( numT<uint16_t>(16u), 1u );
    BOOST_CHECK_EQUAL( numT<uint16_t>(17u), 2u );
    BOOST_CHECK_EQUAL( numT<uint16_t>(32u), 2u );
    BOOST_CHECK_EQUAL( numT<uint16_t>(33u), 3u );

    BOOST_CHECK_EQUAL( numT<uint32_t>(0u),  0u );
    BOOST_CHECK_EQUAL( numT<uint32_t>(1u),  1u );
    BOOST_CHECK_EQUAL( numT<uint32_t>(32u), 1u );
    BOOST_CHECK_EQUAL( numT<uint32_t>(33u), 2u );
    BOOST_CHECK_EQUAL( numT<uint32_t>(64u), 2u );
    BOOST_CHECK_EQUAL( numT<uint32_t>(65u), 3u );

    BOOST_CHECK_EQUAL( numT<uint64_t>(0u),  0u );
    BOOST_CHECK_EQUAL( numT<uint64_t>(1u),  1u );
    BOOST_CHECK_EQUAL( numT<uint64_t>(64u), 1u );
    BOOST_CHECK_EQUAL( numT<uint64_t>(65u), 2u );
    BOOST_CHECK_EQUAL( numT<uint64_t>(128u), 2u );
    BOOST_CHECK_EQUAL( numT<uint64_t>(129u), 3u );
  }



  BOOST_AUTO_TEST_SUITE( native_aggregate ); // {
    BOOST_AUTO_TEST_SUITE( reversal ); // {
      BOOST_AUTO_TEST_CASE( unsigned_32_bits ) {
        using xylose::bits::reverse;
        BOOST_CHECK_EQUAL(
          reverse( static_cast<uint32_t>(0x80000000) ),
                   static_cast<uint32_t>(0x00000001)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<uint32_t>(0x80000001) ),
                   static_cast<uint32_t>(0x80000001)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<uint32_t>(0xffffffff) ),
                   static_cast<uint32_t>(0xffffffff)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<int32_t>(0x80000000) ),
                   static_cast<int32_t>(0x00000001)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<int32_t>(0x80000001) ),
                   static_cast<int32_t>(0x80000001)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<int32_t>(0xffffffff) ),
                   static_cast<int32_t>(0xffffffff)
        );
      }

      BOOST_AUTO_TEST_CASE( unsigned_16_bits ) {
        using xylose::bits::reverse;
        BOOST_CHECK_EQUAL(
          reverse( static_cast<uint16_t>(0x8000) ),
                   static_cast<uint16_t>(0x0001)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<uint16_t>(0x8001) ),
                   static_cast<uint16_t>(0x8001)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<uint16_t>(0xffff) ),
                   static_cast<uint16_t>(0xffff)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<int16_t>(0x8000) ),
                   static_cast<int16_t>(0x0001)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<int16_t>(0x8001) ),
                   static_cast<int16_t>(0x8001)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<int16_t>(0xffff) ),
                   static_cast<int16_t>(0xffff)
        );
      }

      BOOST_AUTO_TEST_CASE( unsigned_8_bits ) {
        using xylose::bits::reverse;
        BOOST_CHECK_EQUAL(
          reverse( static_cast<uint8_t>(0x80) ),
                   static_cast<uint8_t>(0x01)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<uint8_t>(0x81) ),
                   static_cast<uint8_t>(0x81)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<uint8_t>(0xff) ),
                   static_cast<uint8_t>(0xff)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<int8_t>(0x80) ),
                   static_cast<int8_t>(0x01)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<int8_t>(0x81) ),
                   static_cast<int8_t>(0x81)
        );
        BOOST_CHECK_EQUAL(
          reverse( static_cast<int8_t>(0xff) ),
                   static_cast<int8_t>(0xff)
        );
      }

    BOOST_AUTO_TEST_SUITE_END(); // } reversal
  BOOST_AUTO_TEST_SUITE_END(); // } native_aggregate




  BOOST_AUTO_TEST_SUITE( vector_aggregate ); // {

  BOOST_AUTO_TEST_CASE( size ) {
    BOOST_CHECK_EQUAL( test_size<  int8_t>(), 8u );
    BOOST_CHECK_EQUAL( test_size< uint8_t>(), 8u );
    BOOST_CHECK_EQUAL( test_size< int16_t>(), 16u );
    BOOST_CHECK_EQUAL( test_size<uint16_t>(), 16u );
    BOOST_CHECK_EQUAL( test_size< int32_t>(), 32u );
    BOOST_CHECK_EQUAL( test_size<uint32_t>(), 32u );
    BOOST_CHECK_EQUAL( test_size< int64_t>(), 64u );
    BOOST_CHECK_EQUAL( test_size<uint64_t>(), 64u );
  }

  BOOST_AUTO_TEST_CASE( resize ) {
    BOOST_CHECK_EQUAL( test_resize<  int8_t>(), 72u );
    BOOST_CHECK_EQUAL( test_resize< uint8_t>(), 72u );
    BOOST_CHECK_EQUAL( test_resize< int16_t>(), 80u );
    BOOST_CHECK_EQUAL( test_resize<uint16_t>(), 80u );
    BOOST_CHECK_EQUAL( test_resize< int32_t>(), 96u );
    BOOST_CHECK_EQUAL( test_resize<uint32_t>(), 96u );
    BOOST_CHECK_EQUAL( test_resize< int64_t>(), 128u );
    BOOST_CHECK_EQUAL( test_resize<uint64_t>(), 128u );
  }

  BOOST_AUTO_TEST_CASE( reserve ) {
    BOOST_CHECK_EQUAL( test_reserve<  int8_t>(), 72u );
    BOOST_CHECK_EQUAL( test_reserve< uint8_t>(), 72u );
    BOOST_CHECK_EQUAL( test_reserve< int16_t>(), 80u );
    BOOST_CHECK_EQUAL( test_reserve<uint16_t>(), 80u );
    BOOST_CHECK_EQUAL( test_reserve< int32_t>(), 96u );
    BOOST_CHECK_EQUAL( test_reserve<uint32_t>(), 96u );
    BOOST_CHECK_EQUAL( test_reserve< int64_t>(), 128u );
    BOOST_CHECK_EQUAL( test_reserve<uint64_t>(), 128u );
  }

  BOOST_AUTO_TEST_CASE( set ) {
    BOOST_CHECK_EQUAL( test_set<  int8_t>(),  -0x7e );
    BOOST_CHECK_EQUAL( test_set< uint8_t>(),   0x82u );
    BOOST_CHECK_EQUAL( test_set< int16_t>(), -0x7f7e );
    BOOST_CHECK_EQUAL( test_set<uint16_t>(),  0x8082u );
    BOOST_CHECK_EQUAL( test_set< int32_t>(), -0x7fffff7e );
    BOOST_CHECK_EQUAL( test_set<uint32_t>(),  0x80000082u );
    BOOST_CHECK_EQUAL( test_set< int64_t>(), -0x7fffffffffffff7eLL );
    BOOST_CHECK_EQUAL( test_set<uint64_t>(),  0x8000000000000082uLL );
  }

  BOOST_AUTO_TEST_CASE( unset ) {
    BOOST_CHECK_EQUAL( test_unset< int8_t>(),   0x7d );
    BOOST_CHECK_EQUAL( test_unset<uint8_t>(),   0x7du );
    BOOST_CHECK_EQUAL( test_unset< int16_t>(),  0x7f7d );
    BOOST_CHECK_EQUAL( test_unset<uint16_t>(),  0x7f7du );
    BOOST_CHECK_EQUAL( test_unset< int32_t>(),  0x7fffff7d );
    BOOST_CHECK_EQUAL( test_unset<uint32_t>(),  0x7fffff7du );
    BOOST_CHECK_EQUAL( test_unset< int64_t>(),  0x7fffffffffffff7dLL );
    BOOST_CHECK_EQUAL( test_unset<uint64_t>(),  0x7fffffffffffff7duLL );
  }

  BOOST_AUTO_TEST_CASE( setAll ) {
    test_setAll< int8_t>();
    test_setAll<uint8_t>();
    test_setAll< int16_t>();
    test_setAll<uint16_t>();
    test_setAll< int32_t>();
    test_setAll<uint32_t>();
    test_setAll< int64_t>();
    test_setAll<uint64_t>();
  }

  BOOST_AUTO_TEST_CASE( unsetAll ) {
    test_unsetAll< int8_t>();
    test_unsetAll<uint8_t>();
    test_unsetAll< int16_t>();
    test_unsetAll<uint16_t>();
    test_unsetAll< int32_t>();
    test_unsetAll<uint32_t>();
    test_unsetAll< int64_t>();
    test_unsetAll<uint64_t>();
  }

  BOOST_AUTO_TEST_CASE( test ) {
    BOOST_CHECK_EQUAL( test_test<  int8_t>(), 12u );
    BOOST_CHECK_EQUAL( test_test< uint8_t>(), 12u );
    BOOST_CHECK_EQUAL( test_test< int16_t>(), 26u );
    BOOST_CHECK_EQUAL( test_test<uint16_t>(), 26u );
    BOOST_CHECK_EQUAL( test_test< int32_t>(), 58u );
    BOOST_CHECK_EQUAL( test_test<uint32_t>(), 58u );
    BOOST_CHECK_EQUAL( test_test< int64_t>(), 122u );
    BOOST_CHECK_EQUAL( test_test<uint64_t>(), 122u );
  }

  BOOST_AUTO_TEST_CASE( ostream ) {
    check_ostream<        int8_t>("11111111");
    check_ostream<       uint8_t>("11111111");
    check_ostream<          char>("11111111");
    check_ostream<unsigned  char>("11111111");
    check_ostream<         short>("1111111100000000");
    check_ostream<unsigned short>("1111111100000000");
    check_ostream<       int16_t>("1111111100000000");
    check_ostream<      uint16_t>("1111111100000000");
    check_ostream<           int>("11111111000000000000000000000000");
    check_ostream<unsigned   int>("11111111000000000000000000000000");
    check_ostream<       int32_t>("11111111000000000000000000000000");
    check_ostream<      uint32_t>("11111111000000000000000000000000");
    check_ostream<       int64_t>("11111111000000000000000000000000"
                                  "00000000000000000000000000000000");
    check_ostream<      uint64_t>("11111111000000000000000000000000"
                                  "00000000000000000000000000000000");
  }

  BOOST_AUTO_TEST_CASE( istream ) {
    check_istream<        int8_t>("10110011" "10001111" "00001111" "10000011" "11110000");
    check_istream<       uint8_t>("10110011" "10001111" "00001111" "10000011" "11110000");
    check_istream<          char>("10110011" "10001111" "00001111" "10000011" "11110000");
    check_istream<unsigned  char>("10110011" "10001111" "00001111" "10000011" "11110000");
    check_istream<         short>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000");
    check_istream<unsigned short>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000");
    check_istream<       int16_t>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000");
    check_istream<      uint16_t>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000");
    check_istream<           int>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000" "00000000" "00000000");
    check_istream<unsigned   int>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000" "00000000" "00000000");
    check_istream<       int32_t>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000" "00000000" "00000000");
    check_istream<      uint32_t>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000" "00000000" "00000000");
    check_istream<       int64_t>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000" "00000000" "00000000");
    check_istream<      uint64_t>("10110011" "10001111" "00001111" "10000011" "11110000" "00000000" "00000000" "00000000");
  }

  BOOST_AUTO_TEST_CASE( reversal ) {
    check_reverse<        int8_t>("00001111" "11000001" "111100001" "1110001" "11001101");
    check_reverse<       uint8_t>("00001111" "11000001" "111100001" "1110001" "11001101");
    check_reverse<          char>("00001111" "11000001" "111100001" "1110001" "11001101");
    check_reverse<unsigned  char>("00001111" "11000001" "111100001" "1110001" "11001101");
    check_reverse<         short>("00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
    check_reverse<unsigned short>("00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
    check_reverse<       int16_t>("00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
    check_reverse<      uint16_t>("00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
    check_reverse<           int>("00000000" "00000000" "00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
    check_reverse<unsigned   int>("00000000" "00000000" "00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
    check_reverse<       int32_t>("00000000" "00000000" "00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
    check_reverse<      uint32_t>("00000000" "00000000" "00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
    check_reverse<       int64_t>("00000000" "00000000" "00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
    check_reverse<      uint64_t>("00000000" "00000000" "00000000" "00001111" "11000001" "11110000" "11110001" "11001101");
  }

  BOOST_AUTO_TEST_SUITE_END(); // } vector_aggregate

} // namespace

