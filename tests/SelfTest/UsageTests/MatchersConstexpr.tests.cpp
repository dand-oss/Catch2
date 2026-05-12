
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0

#include <catch2/internal/catch_compiler_capabilities.hpp>

#if defined( CATCH_INTERNAL_CONSTEXPR_MATCHERS_ENABLED )

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <catch2/matchers/catch_matchers_container_properties.hpp>
#include <catch2/matchers/catch_matchers_quantifiers.hpp>

#include <array>

namespace {
    struct MatchAllMatcher final : public Catch::Matchers::MatcherGenericBase {
    public:
        template <typename Any>
        constexpr bool match( Any&& ) const {
            return true;
        }

        std::string describe() const override {
            using namespace std::string_literals;
            return "Matches anything"s;
        }
    };

    constexpr MatchAllMatcher MatchAll() { return MatchAllMatcher(); }

    struct MatchTrueMatcher final : public Catch::Matchers::MatcherGenericBase {
    public:
        constexpr bool match( bool b ) const {
            return b;
        }

        std::string describe() const override {
            using namespace std::string_literals;
            return "Matches true"s;
        }
    };

    constexpr MatchTrueMatcher MatchTrue() { return MatchTrueMatcher(); }


} // namespace

TEST_CASE( "Constexpr support for matchers", "[constexpr][matchers][approvals]" ) {
    STATIC_REQUIRE( MatchAll().match( 1 ) );
    STATIC_REQUIRE_THAT( 1, MatchAll() );
}

TEST_CASE( "IsEmpty and HasSize matchers can be used in constexpr contexts",
           "[constexpr][matchers][approvals]" ){
    using namespace Catch::Matchers;
    static constexpr std::array<int, 0> empty{};
    STATIC_REQUIRE_THAT( empty, IsEmpty() );
    static constexpr int arr[1] = { 2 };
    STATIC_REQUIRE_THAT( arr, SizeIs( 1 ) );
    STATIC_REQUIRE_THAT( arr, SizeIs( MatchAll() ) );
}

constexpr std::array<bool, 3> compute_bools( int type ) {
    switch ( type ) {
    case 0:
        return { true, true, true };
    case 1:
        return { false, true, false };
    case 2:
        return { false, false, false };
    default:
        return { false, false, false };
    }
}

TEST_CASE( "Quantifier matchers can be used in constexpr contexts",
           "[constexpr][matchers][approvals]" ) {
    using namespace Catch::Matchers;
    STATIC_REQUIRE_THAT( compute_bools( 0 ), AllTrue() );
    STATIC_REQUIRE_THAT( compute_bools( 1 ), AnyTrue() );
    STATIC_REQUIRE_THAT( compute_bools( 2 ), NoneTrue() );

    STATIC_REQUIRE_THAT( compute_bools( 0 ), AllMatch( MatchTrue() ) );
    STATIC_REQUIRE_THAT( compute_bools( 1 ), AnyMatch( MatchTrue() ) );
    STATIC_REQUIRE_THAT( compute_bools( 2 ), NoneMatch( MatchTrue() ) );
}

// Combining matchers needs C++26 and P2738, so they are in separate preprocessor block
#    if __cpp_constexpr >= 202306L

TEST_CASE("Constexpr support for combining matchers",
    "[constexpr][matchers][approvals]") {
    STATIC_REQUIRE( ( MatchAll() && MatchAll() ).match( 1 ) );
    STATIC_REQUIRE( ( MatchAll() || MatchAll() ).match( 1 ) );
    STATIC_REQUIRE( ( !!MatchAll() ).match( 1 ) );
    STATIC_REQUIRE_THAT( 1, MatchAll() && MatchAll() );
    STATIC_REQUIRE_THAT( 1, MatchAll() || MatchAll() );
    STATIC_REQUIRE_THAT( 1, !!MatchAll() );
}

#endif // __cpp_constexpr >= 202306L

#endif
