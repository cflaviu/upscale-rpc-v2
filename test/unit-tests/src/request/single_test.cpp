// Upscale RPC library
// Copyright (C) 2023 Flaviu C.
#include "types.hpp"
#include <upscale_rpc/request/single.hpp>

namespace upscale_rpc::request::single
{
    TEST_CASE("request/single - small payloads")
    {
        raw_buffer buffer(0xFFFFu);
        auto buffer_span = to_span(buffer);

        random::device dev {};
        random::generator rng(dev());
        random::distribution no_of_params_generator(5, 70);
        const auto no_of_params = no_of_params_generator(rng);
        const auto max_params_size = (buffer.size() - no_of_params * 10u) / no_of_params;
        random::distribution param_size_generator(1, max_params_size);
        random::distribution char_generator(32, 126);

        call_data expected_call {};

        {
            auto& item = expected_call.params;
            item.resize(param_size_generator(rng));
            std::for_each(item.begin(), item.end(), [&](std::uint8_t& c) { c = static_cast<std::uint8_t>(char_generator(rng)); });
        }

        const context_id_t expected_context_id {1234u};
        serialize(expected_context_id, expected_call, buffer_span);
        const auto [resulted_call, resulted_context_id] = deserialize(buffer_span);

        REQUIRE(expected_call == *resulted_call);
    }

    TEST_CASE("request/single - min size")
    {
        call_data expected_call {};
        expected_call.params.resize(1);

        raw_buffer buffer(128u);
        auto buffer_span = to_span(buffer);

        serialize({}, expected_call, buffer_span);
        constexpr auto expected_buffer_size = 41u;
        REQUIRE(buffer_span.size() == expected_buffer_size);
    }
}
