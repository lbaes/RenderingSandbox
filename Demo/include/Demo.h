#pragma once
#include <iostream>
#include "Engine/Engine.h"

class Demo : public Eng::Application {
public:
	void OnStart() override {
		std::cout << "Demo Start\n";
	}

	void OnUpdate() override {

	}

	~Demo() override;
};


