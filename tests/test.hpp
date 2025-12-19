#pragma once

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#define CHECK_EQ(a, b) CHECK((a) == (b))
#define REQUIRE_EQ(a, b) REQUIRE((a) == (b))
