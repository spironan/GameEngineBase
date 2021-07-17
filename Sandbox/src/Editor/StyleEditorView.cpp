#include "StyleEditorView.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <cmath>

#include <fstream>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <istream>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>

StyleEditorView::StyleEditorView()
{
	name = "Default";
	LoadStyle();

	(name+'\0').copy(namebuffer, 100);

}
StyleEditorView::~StyleEditorView()
{
}
void StyleEditorView::Show()
{
	// You can pass in a reference ImGuiStyle structure to compare to, revert to and save to
	// (without a reference style pointer, we will use one compared locally as a reference)
	ImGuiStyle& style = ImGui::GetStyle();
	static ImGuiStyle ref_saved_style;
	// Default to using internal storage as reference
	static bool init = true;
	//if (init && ref == NULL)
		ref_saved_style = style;
	init = false;
	//if (ref == NULL)
	//	ref = &ref_saved_style;

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

	if (ImGui::ShowStyleSelector("Colors##Selector"))
		ref_saved_style = style;
	ImGui::ShowFontSelector("Fonts##Selector");

	// Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
	if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
		style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
	{ bool border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &border)) { style.WindowBorderSize = border ? 1.0f : 0.0f; } }
	ImGui::SameLine();
	{ bool border = (style.FrameBorderSize > 0.0f);  if (ImGui::Checkbox("FrameBorder", &border)) { style.FrameBorderSize = border ? 1.0f : 0.0f; } }
	ImGui::SameLine();
	{ bool border = (style.PopupBorderSize > 0.0f);  if (ImGui::Checkbox("PopupBorder", &border)) { style.PopupBorderSize = border ? 1.0f : 0.0f; } }

	// Save/Revert button

	if (ImGui::InputText("FileName", namebuffer, sizeof(namebuffer), ImGuiInputTextFlags_AutoSelectAll))
		name = namebuffer;
	if (ImGui::Button("Save Ref") && !name.empty())
	{
		ref = ref_saved_style = style;
		SaveStyle();
	}
	ImGui::SameLine();
	if (ImGui::Button("Revert Ref"))
		style = ref;
	ImGui::SameLine();


	ImGui::Separator();

	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Sizes"))
		{
			ImGui::Text("Main");
			ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
			ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
			ImGui::Text("Borders");
			ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::Text("Rounding");
			ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
			ImGui::Text("Alignment");
			ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
			int window_menu_button_position = style.WindowMenuButtonPosition + 1;
			if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
				style.WindowMenuButtonPosition = window_menu_button_position - 1;
			ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
			ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
			ImGui::SameLine(); //HelpMarker("Alignment applies when a button is larger than its text content.");
			ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
			ImGui::SameLine(); //HelpMarker("Alignment applies when a selectable is larger than its text content.");
			ImGui::Text("Safe Area Padding");
			ImGui::SameLine(); //HelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
			ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Colors"))
		{
			static int output_dest = 0;
			static bool output_only_modified = true;
			if (ImGui::Button("Export"))
			{
				
			}
			ImGui::SameLine(); ImGui::SetNextItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0");
			ImGui::SameLine(); ImGui::Checkbox("Only Modified Colors", &output_only_modified);

			static ImGuiTextFilter filter;
			filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

			static ImGuiColorEditFlags alpha_flags = 0;
			if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None)) { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
			if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
			if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; } ImGui::SameLine();
			//HelpMarker(
			//	"In the color list:\n"
			//	"Left-click on color square to open color picker,\n"
			//	"Right-click to open edit options menu.");

			ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
			ImGui::PushItemWidth(-160);
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColorName(i);
				if (!filter.PassFilter(name))
					continue;
				ImGui::PushID(i);
				ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
				if (memcmp(&style.Colors[i], &ref.Colors[i], sizeof(ImVec4)) != 0)
				{
					// Tips: in a real user application, you may want to merge and use an icon font into the main font,
					// so instead of "Save"/"Revert" you'd use icons!
					// Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) { ref.Colors[i] = style.Colors[i]; }
					ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) { style.Colors[i] = ref.Colors[i]; }
				}
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				ImGui::TextUnformatted(name);
				ImGui::PopID();
			}
			ImGui::PopItemWidth();
			ImGui::EndChild();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Fonts"))
		{
			ImGuiIO& io = ImGui::GetIO();
			ImFontAtlas* atlas = io.Fonts;
			//HelpMarker("Read FAQ and docs/FONTS.md for details on font loading.");
			ImGui::PushItemWidth(120);
			for (int i = 0; i < atlas->Fonts.Size; i++)
			{
				ImFont* font = atlas->Fonts[i];
				ImGui::PushID(font);
				//NodeFont(font);
				ImGui::PopID();
			}
			if (ImGui::TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexWidth, atlas->TexHeight))
			{
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
				ImGui::Image(atlas->TexID, ImVec2((float)atlas->TexWidth, (float)atlas->TexHeight), ImVec2(0, 0), ImVec2(1, 1), tint_col, border_col);
				ImGui::TreePop();
			}

			// Post-baking font scaling. Note that this is NOT the nice way of scaling fonts, read below.
			// (we enforce hard clamping manually as by default DragFloat/SliderFloat allows CTRL+Click text to get out of bounds).
			const float MIN_SCALE = 0.3f;
			const float MAX_SCALE = 2.0f;
			//HelpMarker(
			//	"Those are old settings provided for convenience.\n"
			//	"However, the _correct_ way of scaling your UI is currently to reload your font at the designed size, "
			//	"rebuild the font atlas, and call style.ScaleAllSizes() on a reference ImGuiStyle structure.\n"
			//	"Using those settings here will give you poor quality results.");
			static float window_scale = 1.0f;
			if (ImGui::DragFloat("window scale", &window_scale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp)) // Scale only this window
				ImGui::SetWindowFontScale(window_scale);
			ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp); // Scale everything
			ImGui::PopItemWidth();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Rendering"))
		{
			ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
			ImGui::SameLine();
			//HelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");

			ImGui::Checkbox("Anti-aliased lines use texture", &style.AntiAliasedLinesUseTex);
			ImGui::SameLine();
			//HelpMarker("Faster lines using texture data. Require backend to render with bilinear filtering (not point/nearest filtering).");

			ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
			ImGui::PushItemWidth(100);
			ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, 10.0f, "%.2f");
			if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;

			// When editing the "Circle Segment Max Error" value, draw a preview of its effect on auto-tessellated circles.
			ImGui::DragFloat("Circle Tessellation Max Error", &style.CircleTessellationMaxError, 0.005f, 0.10f, 5.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			if (ImGui::IsItemActive())
			{
				ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
				ImGui::BeginTooltip();
				ImGui::TextUnformatted("(R = radius, N = number of segments)");
				ImGui::Spacing();
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				const float min_widget_width = ImGui::CalcTextSize("N: MMM\nR: MMM").x;
				for (int n = 0; n < 8; n++)
				{
					const float RAD_MIN = 5.0f;
					const float RAD_MAX = 70.0f;
					const float rad = RAD_MIN + (RAD_MAX - RAD_MIN) * (float)n / (8.0f - 1.0f);

					ImGui::BeginGroup();

					ImGui::Text("R: %.f\nN: %d", rad, draw_list->_CalcCircleAutoSegmentCount(rad));

					const float canvas_width = std::fmax(min_widget_width, rad * 2.0f);
					const float offset_x = std::floorf(canvas_width * 0.5f);
					const float offset_y = std::floorf(RAD_MAX);

					const ImVec2 p1 = ImGui::GetCursorScreenPos();
					draw_list->AddCircle(ImVec2(p1.x + offset_x, p1.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
					ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));

					/*
					const ImVec2 p2 = ImGui::GetCursorScreenPos();
					draw_list->AddCircleFilled(ImVec2(p2.x + offset_x, p2.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
					ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));
					*/

					ImGui::EndGroup();
					ImGui::SameLine();
				}
				ImGui::EndTooltip();
			}
			ImGui::SameLine();
			//HelpMarker("When drawing circle primitives with \"num_segments == 0\" tesselation will be calculated automatically.");

			ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
			ImGui::PopItemWidth();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::PopItemWidth();
}

void StyleEditorView::SaveStyle()
{
	std::ofstream ofs;
	ofs.open(name + ".json");
	if (ofs.is_open() == false)
		return;
	rapidjson::OStreamWrapper osw(ofs);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	ImGuiStyle& item = (this->ref);
	writer.StartObject();
	writer.Key(name.c_str());
	writer.StartArray();
	writer.Double(item.Alpha);
	writer.Double(item.WindowPadding.x); writer.Double(item.WindowPadding.y);
	writer.Double(item.WindowRounding);
	writer.Double(item.WindowBorderSize);
	writer.Double(item.WindowMinSize.x); writer.Double(item.WindowMinSize.y);
	writer.Double(item.WindowTitleAlign.x); writer.Double(item.WindowTitleAlign.y);
	writer.Double(item.WindowMenuButtonPosition);
	writer.Double(item.ChildRounding);
	writer.Double(item.ChildBorderSize);
	writer.Double(item.PopupRounding);
	writer.Double(item.PopupBorderSize);
	writer.Double(item.FramePadding.x); writer.Double(item.FramePadding.y);
	writer.Double(item.FrameRounding);
	writer.Double(item.FrameBorderSize);
	writer.Double(item.ItemSpacing.x); writer.Double(item.ItemSpacing.y);
	writer.Double(item.ItemInnerSpacing.x); writer.Double(item.ItemInnerSpacing.y);
	writer.Double(item.CellPadding.x); writer.Double(item.CellPadding.y);
	writer.Double(item.TouchExtraPadding.x); writer.Double(item.TouchExtraPadding.y);
	writer.Double(item.IndentSpacing);
	writer.Double(item.ColumnsMinSpacing);
	writer.Double(item.ScrollbarSize);
	writer.Double(item.ScrollbarRounding);
	writer.Double(item.GrabMinSize);
	writer.Double(item.GrabRounding);
	writer.Double(item.LogSliderDeadzone);
	writer.Double(item.TabRounding);
	writer.Double(item.TabBorderSize);
	writer.Double(item.TabMinWidthForCloseButton);
	writer.Double(item.ColorButtonPosition);
	writer.Double(item.ButtonTextAlign.x); writer.Double(item.ButtonTextAlign.y);
	writer.Double(item.SelectableTextAlign.x); writer.Double(item.SelectableTextAlign.y);
	writer.Double(item.DisplayWindowPadding.x); writer.Double(item.DisplayWindowPadding.y);
	writer.Double(item.DisplaySafeAreaPadding.x);	writer.Double(item.DisplaySafeAreaPadding.y);
	writer.Double(item.MouseCursorScale);
	writer.Double(item.AntiAliasedLines);
	writer.Double(item.AntiAliasedLinesUseTex);
	writer.Double(item.AntiAliasedFill);
	writer.Double(item.CurveTessellationTol);
	writer.Double(item.CircleTessellationMaxError);
	for (int i = 0; i < ImGuiCol_COUNT; ++i)
	{
		writer.StartArray();
		writer.Double(item.Colors[i].x);
		writer.Double(item.Colors[i].y);
		writer.Double(item.Colors[i].z);
		writer.Double(item.Colors[i].w);
		writer.EndArray();
	}
	writer.EndArray();
	writer.EndObject();
	ofs.close();
}

void StyleEditorView::LoadStyle()
{
	std::ifstream ifs;
	ifs.open(name + ".json");
	if (ifs.is_open() == false)
		return;
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	ImGuiStyle& item = (this->ref);

	auto it = doc.MemberBegin();
	name = it->name.GetString();
	auto arr  = it->value.GetArray();
	item.Alpha = arr[0].GetFloat();
	item.WindowPadding.x = arr[1].GetFloat();
	item.WindowPadding.y = arr[2].GetFloat();
	item.WindowRounding = arr[3].GetFloat();
	item.WindowBorderSize = arr[4].GetFloat();
	item.WindowMinSize.x = arr[5].GetFloat();
	item.WindowMinSize.y = arr[6].GetFloat();
	item.WindowTitleAlign.x = arr[7].GetFloat();
	item.WindowTitleAlign.y = arr[8].GetFloat();
	item.WindowMenuButtonPosition = arr[9].GetFloat();
	item.ChildRounding = arr[10].GetFloat();
	item.ChildBorderSize = arr[11].GetFloat();
	item.PopupRounding = arr[12].GetFloat();
	item.PopupBorderSize = arr[13].GetFloat();
	item.FramePadding.x = arr[14].GetFloat();
	item.FramePadding.y = arr[15].GetFloat();
	item.FrameRounding = arr[16].GetFloat();
	item.FrameBorderSize = arr[17].GetFloat();
	item.ItemSpacing.x = arr[18].GetFloat();
	item.ItemSpacing.y = arr[19].GetFloat();
	item.ItemInnerSpacing.x = arr[20].GetFloat();
	item.ItemInnerSpacing.y = arr[21].GetFloat();
	item.CellPadding.x = arr[22].GetFloat();
	item.CellPadding.y = arr[23].GetFloat();
	item.TouchExtraPadding.x = arr[24].GetFloat();
	item.TouchExtraPadding.y = arr[25].GetFloat();
	item.IndentSpacing = arr[26].GetFloat();
	item.ColumnsMinSpacing = arr[27].GetFloat();
	item.ScrollbarSize = arr[28].GetFloat();
	item.ScrollbarRounding = arr[29].GetFloat();
	item.GrabMinSize = arr[30].GetFloat();
	item.GrabRounding = arr[31].GetFloat();
	item.LogSliderDeadzone = arr[32].GetFloat();
	item.TabRounding = arr[33].GetFloat();
	item.TabBorderSize = arr[34].GetFloat();
	item.TabMinWidthForCloseButton = arr[35].GetFloat();
	item.ColorButtonPosition = arr[36].GetFloat();
	item.ButtonTextAlign.x = arr[37].GetFloat();
	item.ButtonTextAlign.y = arr[38].GetFloat();
	item.SelectableTextAlign.x = arr[39].GetFloat();
	item.SelectableTextAlign.y = arr[40].GetFloat();
	item.DisplayWindowPadding.x = arr[41].GetFloat();
	item.DisplayWindowPadding.y = arr[42].GetFloat();
	item.DisplaySafeAreaPadding.x = arr[43].GetFloat();
	item.DisplaySafeAreaPadding.y = arr[44].GetFloat();
	item.MouseCursorScale = arr[45].GetFloat();
	item.AntiAliasedLines = arr[46].GetFloat();
	item.AntiAliasedLinesUseTex = arr[47].GetFloat();
	item.AntiAliasedFill = arr[48].GetFloat();
	item.CurveTessellationTol = arr[49].GetFloat();
	item.CircleTessellationMaxError = arr[50].GetFloat();

	for (int i = 0; i < ImGuiCol_COUNT; ++i)
	{
		auto colarr = arr[51+i].GetArray();
		item.Colors[i].x= colarr[0].GetFloat();
		item.Colors[i].y= colarr[1].GetFloat();
		item.Colors[i].z= colarr[2].GetFloat();
		item.Colors[i].w= colarr[3].GetFloat();
	}
	ifs.close();
	ImGui::GetStyle() = ref;
}
