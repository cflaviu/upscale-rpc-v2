#include <cstdint>
#include <cista/containers/variant.h>
#include <cista/containers/vector.h>
#include <cista/containers/bitset.h>
#include <iostream>

namespace home
{
    template <typename T, typename Error>
    using result_t = cista::offset::variant<T, Error>;

    template <typename T>
    using vector_t = cista::offset::vector<T>;

    template <std::size_t Size>
    using bitset_t = cista::bitset<Size>;

    using temperature_t = std::int8_t;
    using temperature_vector_t = vector_t<temperature_t>;
    using room_count_t = std::uint8_t;
    using room_index_t = room_count_t;
    using room_index_vector_t = vector_t<room_index_t>;

    enum class Error: std::uint8_t
    {
        none,
        out_of_range_index,
    };

    enum Status: std::uint8_t
    {
        inactive,
        active,
        failure,
    };

    using status_vector_t = vector_t<Status>;
    using statuses_result_t = result_t<status_vector_t, Error>;
    using status_result_t = result_t<Status, Error>;
    using temperatures_result_t = result_t<temperature_vector_t, Error>;

    namespace generic
    {
        class manager
        {
        public:
            room_count_t room_count() const noexcept;

            statuses_result_t statuses() const noexcept;
            statuses_result_t statuses(const room_index_vector_t& room_indexes) const noexcept;
            status_result_t status(const room_index_t room_index) const noexcept;
        };
    }


    namespace heating
    {
        using room_index_temperature_pair_t = std::pair<room_index_t, temperature_t>;
        using room_index_temperature_pair_vector_t = vector_t<room_index_temperature_pair_t>;

        class manager: public generic::manager
        {
        public:
            Status heating_status() const noexcept;
            Error set_heating_status(const Status value) noexcept;

            temperatures_result_t temperatures() const noexcept;
            temperatures_result_t temperatures(const room_index_vector_t& room_indexes) const noexcept;

            temperatures_result_t target_temperatures() const noexcept;
            temperatures_result_t target_temperatures(const room_index_vector_t& room_indexes) const noexcept;
            Error set_target_temperature(const room_index_temperature_pair_t& item) noexcept;
            Error set_target_temperatures(const room_index_temperature_pair_vector_t& items) noexcept;
        };
    }

    namespace lighting
    {
        using room_index_status_pair_t = std::pair<room_index_t, bool>;
        using room_index_status_vector_t = vector_t<room_index_status_pair_t>;

        class manager: public generic::manager
        {
        public:
            Error set_target_status(const room_index_status_pair_t& item) noexcept;
            Error set_target_statuses(const status_set_t& items) noexcept;
            Error set_target_statuses(const room_index_status_vector_t& items) noexcept;
        };
    }

    namespace locking
    {
        enum class state: std::uint8_t
        {
            unlocked_open,
            unlocked_closed,
            locked,
        };

        using room_index_status_pair_t = std::pair<room_index_t, state>;
        using room_index_status_vector_t = vector_t<room_index_status_pair_t>;

        class manager: public generic::manager
        {
        public:
            Error set_status(const room_index_status_pair_t& item) noexcept;
            Error set_statuses(const status_set_t& items) noexcept;
            Error set_statuses(const room_index_status_vector_t& items) noexcept;
        };
    }

    class manager
    {
    public:
        room_count_t room_count() const noexcept;

        const heating::manager& heating_manager() const noexcept;
        heating::manager& heating_manager() noexcept;

        const lighting::manager& lighting_manager() const noexcept;
        lighting::manager& lighting_manager() noexcept;
    };
}

#define size_of(x) #x << ": " << sizeof(x)

int main()
{
    using namespace std;
    cout << size_of(home::heating::room_index_temperature_pair_vector_t) << " bytes\n";
    cout << size_of(home::temperatures_result_t) << " bytes\n";
    cout << size_of(home::lighting::room_index_status_vector_t) << " bytes\n";
    cout << size_of(home::statuses_result_t) << " bytes\n";
    return 0;
}
