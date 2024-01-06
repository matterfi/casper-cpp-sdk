#pragma once

#include <memory>
#include <string>

namespace spdlog
{
class logger;
} // namespace spdlog

namespace Casper
{
struct LogConfig;

class LogConfigurator final
{
    std::string const log_name_;

public:
    static std::shared_ptr<spdlog::logger> initDefault(LogConfig const& log_config);

private:
    explicit LogConfigurator(std::string const& log_name) noexcept;
    std::shared_ptr<spdlog::logger> configure(LogConfig const& log_config);
    std::shared_ptr<spdlog::logger> configureRotatingLogger();
    std::shared_ptr<spdlog::logger> configureConsoleLogger();
};
} // namespace Casper