#pragma once
// #include "LogPanel.h"

namespace tt {
namespace pattern {

template<class T>
T* GetPoint() {
	static T t;
	return &t;
}

template<class T>
T& GetRefrence() {
	static T t;
	return t;
}

}
}


