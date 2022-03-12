#pragma once
#include <LLRenderer/Vertex.h>
#include <vector>

namespace Eng {

	enum class VERTEX_BUFFER_USAGE {
		DYNAMIC,
		STATIC
	};

	class VertexBuffer{
	public:

		void AddVertex(Vec3 position, Color4 diffuse_color, Color4 specular_color, Vec3 vertex_normal, Vec2 texture_coordinate) {
			vertex_buffer.emplace_back(position, diffuse_color, specular_color, vertex_normal, texture_coordinate);
		}

		void AddVertex(const Vertex& v) {
			vertex_buffer.push_back(v);
		}

		void SetUsage(VERTEX_BUFFER_USAGE usage) {
			buffer_usage = usage;
		}

		VERTEX_BUFFER_USAGE GetUsage() const{
			return buffer_usage;
		}

		Vertex* GetData() const{
			return vertex_buffer.data();
		}
	private:
		std::vector<Vertex> vertex_buffer;
		VERTEX_BUFFER_USAGE buffer_usage = VERTEX_BUFFER_USAGE::USAGE_STATIC;
	};

	class IndexBuffer {
	public:

		void AddIndex(unsigned int index) {
			indices.emplace_back(index);
		}

		unsigned int* GetData() const{
			return indices.GetData();
		}

	private:
		std::vector<unsigned int> indices;
	};
}