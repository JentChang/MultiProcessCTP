

#include "Timer.h"
#include <chrono>

#include "SystemDir.h"


std::shared_ptr<NanoTimer> NanoTimer::m_ptr = std::shared_ptr<NanoTimer>(nullptr);

NanoTimer* NanoTimer::getInstance()
{
    if (m_ptr.get() == nullptr)
    {
        m_ptr = std::shared_ptr<NanoTimer>(new NanoTimer());
    }
    return m_ptr.get();
}

inline std::chrono::steady_clock::time_point get_time_now()
{
	return std::chrono::steady_clock::now();
}
/*
inline int64_t get_socket_diff()
{
	using namespace std::asio;
	std::array<char, SOCKET_MESSAGE_MAX_LENGTH> input, output;
	io_service io_service;

#ifdef _MSC_VER
	ip::tcp::socket socket(io_service);
	socket.connect(ip::tcp::endpoint(ip::tcp::v4(), SystemDir::getPageEngineListenPort()));
#else
	local::stream_protocol::socket socket(io_service);
	socket.connect(local::stream_protocol::endpoint(PAGED_SOCKET_FILE));
#endif

	std::system::error_code error;
	input[0] = TIMER_SEC_DIFF_REQUEST;
	write(socket, buffer(input), error);
	socket.read_some(buffer(output), error);
	json socket_info = json::parse(string(&output[0]));
	return socket_info["secDiff"].get<int64_t>();
}
*/
inline int64_t get_local_diff()
{
    int unix_second_num = std::chrono::seconds(std::time(NULL)).count();
    int tick_second_num = std::chrono::duration_cast<std::chrono::seconds>(
            get_time_now().time_since_epoch()
    ).count();
    return (unix_second_num - tick_second_num) * NANOSECONDS_PER_SECOND;
}

NanoTimer::NanoTimer()
{
	/*
	try
	{
		secDiff = get_socket_diff();
	}
	catch (...)
	{
		secDiff = get_local_diff();
	}
	*/

	secDiff = get_local_diff();
}

int64_t NanoTimer::getNano() const
{
    int64_t _nano = std::chrono::duration_cast<std::chrono::nanoseconds>(
            get_time_now().time_since_epoch()
    ).count();
    return _nano + secDiff;
}
