/*****************************************************************//**
 * \file   LogCallbackSink.h
 * \brief  a spdlog sink which allows for callback when using the 
 *		   logging functions
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   12 July 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "spdlog/sinks/base_sink.h"
#include <functional>
#include <string>
#include <vector>
namespace engine
{
	template <typename Mutex>
	class CallbackSink : public spdlog::sinks::base_sink <Mutex>
	{
	public:
		CallbackSink() 
		{
			this->set_pattern("%n: %v%$");
		}
		/**
		 * \brief 
		 * subscribe to the current sink when the string gets formatted
		 */
		static void SubscribeToSink(std::function<void(const std::string&)> subscriber)
		{
			m_subscriberList.emplace_back(subscriber);
		}
	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{

			// log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
			// msg.raw contains pre formatted log
			
			// If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
			std::string temp = fmt::to_string(formatted);
			for (auto& subscriber : m_subscriberList)
			{
				subscriber(temp);
			}
		}

		void flush_() override
		{
			
		}
	private:
		static std::vector < std::function<void(const std::string&)> > m_subscriberList;
	};
	template<typename mutex>
	std::vector <std::function<void(const std::string&)> > CallbackSink<mutex>::m_subscriberList;
}
#include "spdlog/details/null_mutex.h"
#include <mutex>
using CallbackSink_mt = engine::CallbackSink<std::mutex>;
using CallbackSink_st = engine::CallbackSink<spdlog::details::null_mutex>;