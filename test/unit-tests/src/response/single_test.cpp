// Upscale RPC library
// Copyright (C) 2023 Flaviu C.
#include "types.hpp"
#include <upscale_rpc/response/single.hpp>

namespace upscale_rpc::response::single
{
    TEST_CASE("response/single - small payloads")
    {
        raw_buffer buffer(0xFFFFu);
        auto buffer_span = to_span(buffer);

        random::device dev {};
        random::generator rng(dev());
        random::distribution param_size_generator(1, 1000);
        random::distribution char_generator(32, 126);

        call_data expected_response {};
        {
            auto& response = expected_response.params;
            response.resize(param_size_generator(rng));
            std::for_each(response.begin(), response.end(), [&](std::uint8_t& c) { c = static_cast<std::uint8_t>(char_generator(rng)); });
        }

        const context_id_t expected_context_id {1234u};
        serialize(expected_context_id, expected_response, buffer_span);
        const auto result = deserialize(buffer_span);

        REQUIRE(expected_response == *result.first);
        REQUIRE(expected_context_id == result.second);
    }

    TEST_CASE("response/single - min size")
    {
        call_data expected_response {};
        expected_response.params.resize(1);

        raw_buffer buffer(128u);
        auto buffer_span = to_span(buffer);

        serialize({}, expected_response, buffer_span);
        constexpr auto expected_buffer_size = 41u;
        REQUIRE(buffer_span.size() == expected_buffer_size);
    }
}
