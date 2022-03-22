#pragma once

#include <memory>
#include "Core.h"
#include "Window.h"
#include "Engine/Events/EventDispatcher.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Logger.h"
#include "Engine/LLRenderer/RenderDevice.h"
#include "Engine/LLRenderer/Renderer.h"

namespace Eng
{

	Window* GetWindow();
	Input* GetInput();
	class Application
	{
	public:
		Application();
		virtual ~Application();
		int Start();

	protected:
		virtual void OnStart(){};
		virtual void OnUpdate(){};
		virtual void Draw(){};

//		GPUShaderHandle CreateShaderProgram(const Shader& shader);
//		GPUModelHandle CreateModel(const Model& model);
//		void RenderModel(const GPUModelHandle& gpuModelHandle);
//
//		int GetWindowHeight() const;
//		int GetWindowWidth() const;
		Window* window;
		Input* input;
		std::unique_ptr<RenderDevice> renderDevice;
		std::unique_ptr<Renderer> renderer;

	private:
		friend Window* GetWindow();
		friend Input* GetInput();

		Logger* logger;

		bool isRunning;
	};
}