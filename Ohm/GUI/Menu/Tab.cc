#include "./Tab.h"

Tab::Tab(std::wstring tab_title) {
	this->title = tab_title;
}

void Tab::Draw() {
	if (!this->draw)
		return;


}