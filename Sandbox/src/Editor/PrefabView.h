#pragma once
class PrefabView
{
public:
	PrefabView();
	~PrefabView();
	void Show();
	void SetActive(bool);
private:
	bool m_active;
};