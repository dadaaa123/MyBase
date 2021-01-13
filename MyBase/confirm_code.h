#pragma once
#include "iostream"

namespace BASE_TEST1 {
	int a = 1;
	void value_change(int &value) { 
		value = 9; 
	}
	int memory_track(){ 
		
		value_change(a);
		std::cout << a << std::endl;

		return a;
	}
}