//
// Created by Sergey Sedov on 14.10.2018.
//

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "Tensor.h"


TEST_CASE("Empty tensor is empty", "[tensor]") {
    Tensor<float> t1;
    REQUIRE(t1.dim().empty());
}

TEST_CASE("Tensor initialized", "[tensor]") {
    Tensor<float> t1({5, 5, 5});
    REQUIRE(t1[{4, 4, 4}] == 0);
}

TEST_CASE("Copied tensor is independent", "[tensor]") {
    Tensor<float> t1({5, 5, 5});
    Tensor<float> t2 = t1;
    t1[{0, 0, 0}] = 1.f;
    t2[{1, 1, 1}] = 2.f;
    REQUIRE(t1[{0, 0, 0}] == 1.f);
    REQUIRE(t2[{0, 0, 0}] == 0.f);
    REQUIRE(t1[{1, 1, 1}] == 0.f);
    REQUIRE(t2[{1, 1, 1}] == 2.f);
}

TEST_CASE("Subtensor is dependent", "[tensor]") {
    Tensor<float> t1({5, 5, 5});
    Tensor<float> t2 = t1(2);
    t1[{0, 0, 0}] = 1.f;
    t1[{2, 0, 0}] = 3.f;
    t2[{1, 1}] = 2.f;
    REQUIRE(t1[{0, 0, 0}] == 1.f);
    REQUIRE(t2[{0, 0}] == 3.f);
    REQUIRE(t1[{2, 1, 1}] == 2.f);
    REQUIRE(t2[{1, 1}] == 2.f);
}

TEST_CASE("Sliced tensor is dependent", "[tensor]") {
    Tensor<float> t1({5, 5, 5});
    Tensor<float> t2 = t1(2, 3);
    t1[{0, 0, 0}] = 1.f;
    t1[{2, 0, 0}] = 3.f;
    t2[{0, 1, 1}] = 2.f;
    REQUIRE(t1[{0, 0, 0}] == 1.f);
    REQUIRE(t2[{0, 0, 0}] == 3.f);
    REQUIRE(t1[{2, 1, 1}] == 2.f);
    REQUIRE(t2[{0, 1, 1}] == 2.f);
}

TEST_CASE("Exception on out of bounds", "[tensor]") {
    Tensor<float> t1({5, 5, 5});
    REQUIRE_THROWS(t1[{5, 5, 5}] = 0);
    REQUIRE_THROWS(t1[{1, -1, 1}] = 0);
}

TEST_CASE("Exception on wrong dimensions length", "[tensor]") {
    Tensor<float> t1({5, 5, 5});
    REQUIRE_THROWS(t1[{5, 5}] = 0);
    REQUIRE_THROWS(t1[{5, 5, 5, 5}] = 0);
}

TEST_CASE("Exceptions on slice out of bounds", "[tensor]") {
    Tensor<float> t1;
    Tensor<float> t2({5, 5, 5});
    REQUIRE_THROWS(t1(0));
    REQUIRE_THROWS(t2(5));
    REQUIRE_THROWS(t2(-1));
    REQUIRE_THROWS(t2(0)(0)(0)(0));
    REQUIRE_THROWS(t1(0, 0));
    REQUIRE_THROWS(t2(0, 5));
    REQUIRE_THROWS(t2(0, 0));
    REQUIRE_THROWS(t2(1, 1));
    REQUIRE_THROWS(t2(-1, 0));
    REQUIRE_THROWS(t2(0, 1)(0, 2));
    REQUIRE_THROWS(t2(0, 1)(1, 2));
}

TEST_CASE("Reshaped tensor is dependent", "[tensor]") {
    Tensor<float> t1({5, 5, 5});
    Tensor<float> t2 = t1.reshape({5, 25});
    t1[{0, 0, 0}] = 1.f;
    t2[{0, 1}] = 2.f;
    REQUIRE(t1[{0, 0, 0}] == 1.f);
    REQUIRE(t2[{0, 0}] == 1.f);
    REQUIRE(t1[{0, 0, 1}] == 2.f);
    REQUIRE(t2[{0, 1}] == 2.f);
}

TEST_CASE("Reshaped exception", "[tensor]") {
    Tensor<float> t1({5, 5, 5});
    Tensor<float> t2;
    REQUIRE_THROWS(t1.reshape({5, 20}));
    REQUIRE_THROWS(t1.reshape({100}));
    REQUIRE_THROWS(t1.reshape({}));
    REQUIRE_NOTHROW(t2.reshape({}));
    REQUIRE_NOTHROW(t1.reshape({125}));
    REQUIRE_NOTHROW(t1.reshape({1, 1, 1, 1, 5, 5, 5}));

}
