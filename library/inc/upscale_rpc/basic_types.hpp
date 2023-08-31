// Upscale RPC library
// Copyright (C) 2023 Flaviu C.
#pragma once
#ifndef PCH
    #include <cista/containers/vector.h>
    #include <span>
#endif

namespace upscale_rpc
{
    using context_id_t = std::uint16_t;
    using object_id_t = std::uint8_t;
    using method_id_t = std::uint8_t;
    using object_method_id_t = std::pair<object_id_t, method_id_t>;
    using raw_data_t = std::span<std::uint8_t>;
    using c_raw_data_t = std::span<const std::uint8_t>;
    using payload_size_t = std::uint32_t;

    template <typename T>
    constexpr auto operator+(const T e) noexcept //-> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>>
    {
        return static_cast<std::underlying_type_t<T>>(e);
    }

    template <typename _Array>
    constexpr raw_data_t to_span(_Array& array) noexcept
    {
        return {reinterpret_cast<std::uint8_t*>(array.data()), array.size() * sizeof(typename _Array::value_type)};
    }

    template <typename _Array>
    constexpr c_raw_data_t to_cspan(const _Array& array) noexcept
    {
        return {reinterpret_cast<const std::uint8_t*>(array.data()), array.size() * sizeof(typename _Array::value_type)};
    }

    enum class action_t : std::uint8_t
    {
        request,
        request_cancel,
        response,
    };

    enum class encoding_scheme_t : std::uint8_t
    {
        single,
        aggregate,
    };

    union primary_header
    {
        using value_t = std::uint8_t;

        value_t data {};
        struct
        {
            value_t action : 3, encoding_scheme : 5;
        } fields;

        constexpr primary_header() noexcept = default;
        constexpr primary_header(const action_t action, const encoding_scheme_t encoding_scheme) noexcept
        {
            fields.action = +action;
            fields.encoding_scheme = +encoding_scheme;
        }

        action_t action() const noexcept { return static_cast<action_t>(fields.action); }
        encoding_scheme_t encoding_scheme() const noexcept { return static_cast<encoding_scheme_t>(fields.encoding_scheme); }
    };

    struct common_header
    {
        primary_header basic_header {};
        std::uint8_t padding {};
        context_id_t context_id {};
        payload_size_t payload_size {};
    };

    struct call_data
    {
        template <typename T, typename SizeType = std::uint32_t>
        using internal_vector = cista::basic_vector<T, cista::offset::ptr, false, SizeType>;

        using vector = internal_vector<call_data>;
        using param_data = internal_vector<std::uint8_t>;

        bool operator==(const call_data& rhs) const noexcept = default;
        bool operator!=(const call_data& rhs) const noexcept = default;

        param_data params {};
        object_method_id_t method {};
    };
}
