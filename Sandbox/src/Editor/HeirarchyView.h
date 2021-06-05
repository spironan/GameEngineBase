#pragma once
class testclass;
class HeirarchyView
{
public: 
	HeirarchyView() :m_dragging{ false }, m_editing{ false }, m_Buffer{""}{}
	void Show();
private:
	void HeirarchyPopUp();
	void ListHeirarchy(testclass* );
private:
	char m_Buffer[100];
	bool m_dragging;
	bool m_editing;
};

