// The MIT License (MIT)

// Copyright (c) 2014 Rapptz

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <jsonpp/parser.hpp>
#include <catch.hpp>
#include <fstream>

// tests for real JSON files found in the wild

TEST_CASE("twitter response", "[real-twitter]") {
    json::value v;
    std::ifstream in("tests/real/twitter.json");
    REQUIRE(in.is_open());
    REQUIRE(in.good());
    REQUIRE_NOTHROW(json::parse(in, v));
    REQUIRE(v.is<json::array>());
    REQUIRE(!v.is<double>());
    REQUIRE(!v.is<json::null>());
    REQUIRE(!v.is<std::string>());
    REQUIRE(!v.is<json::object>());

    auto&& arr = v.as<json::array>();
    REQUIRE(arr.size() == 3);
    REQUIRE(arr.back().is<json::object>());
    REQUIRE(arr.front().is<json::object>());
    REQUIRE(arr[1].is<json::object>());

    auto&& first = arr.front().as<json::object>();
    REQUIRE(first.count("coordinates"));
    REQUIRE(first["coordinates"].is<json::null>());
    REQUIRE(first["text"].is<std::string>());
    REQUIRE(first["user"].is<json::object>());
    REQUIRE(first["entities"].is<json::object>());
    REQUIRE(first["id_str"].as<std::string>("hello") == "240558470661799936");
    REQUIRE(first["id"].as<unsigned long long>(10) == 240558470661799936);

    auto&& user = first["user"].as<json::object>();
    REQUIRE(user["name"].is<std::string>());
    REQUIRE(user["name"].as<std::string>() == "OAuth Dancer");
    REQUIRE(user["profile_background_tile"].as<bool>(false));
    REQUIRE(!user["is_translator"].as<bool>(true));
    REQUIRE(user["url"].as<std::string>("hello") == "http://bit.ly/oauth-dancer");
    REQUIRE(user["verified"].is<bool>());
    REQUIRE(user["verified"].as<bool>() == false);

    auto&& second = arr[1].as<json::object>();
    REQUIRE(second.count("entities"));
    REQUIRE(second["entities"].is<json::object>());
    REQUIRE(second["dne"].as<std::string>("hello") == "hello");
    REQUIRE(second["geo"].is<json::object>());
    REQUIRE(second["retweeted"].as<bool>(true) == false);
    REQUIRE(second.count("in_reply_to_user_id"));
    REQUIRE(second["in_reply_to_user_id"].is<json::null>());

    auto&& entities = second["entities"].as<json::object>();
    REQUIRE(entities["urls"].is<json::array>());
    REQUIRE(entities["user_mentions"].is<json::array>());
}
