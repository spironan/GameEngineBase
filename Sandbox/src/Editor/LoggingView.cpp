#include "LoggingView.h"
#include <imgui.h>
std::deque<std::string> LoggingView::s_messages;

void LoggingView::Show()
{
	ImGui::Begin("Logger");
	for (std::string& s : s_messages)
		ImGui::Text(s.c_str());
	ImGui::End();
}

void LoggingView::AddLoggingMsg(const char* fmt, ...)
{
	char buf[1024];
	va_list vargs;
	va_start(vargs, fmt);
	vsnprintf(buf, sizeof(buf), fmt, vargs);
	buf[sizeof(buf) - 1] = '\0';
	s_messages.emplace_back(buf);
}
