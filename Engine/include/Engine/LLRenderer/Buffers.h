#pragma once
#include <Engine/LLRenderer/Vertex.h>
#include <Engine/LLRenderer/Handles.h>
#include <Engine/Core/Types/Vec.h>
#include <Engine/Core/Types/Color.h>
#include <Engine/Core/Types/HashedString.h>
#include <Engine/Resources/Texture2D.h>
#include <vector>

namespace Eng {

	enum class VERTEX_BUFFER_USAGE {
		DYNAMIC,
		STATIC
	};

	template<typename T>
	class Buffer {
	public:
		const T* GetData() const {
			return buffer.data();
		}

		size_t GetSize() const {
			return buffer.size();
		}

		size_t GetSizeBytes() const {
			return buffer.size() * sizeof(T);
		}
	protected:
		std::vector<T> buffer;
	};

	class VertexBuffer : public Buffer<Vertex> {
	public:

		void AddVertex(Vec3 position, Color4 diffuse_color, Color4 specular_color, Vec3 vertex_normal, Vec2 texture_coordinate) {
			buffer.emplace_back(position, diffuse_color, specular_color, vertex_normal, texture_coordinate);
		}

		void AddVertex(const Vertex& v) {
			buffer.push_back(v);
		}

		void SetUsage(VERTEX_BUFFER_USAGE usage) {
			buffer_usage = usage;
		}

		VERTEX_BUFFER_USAGE GetUsage() const {
			return buffer_usage;
		}
	private:
		VERTEX_BUFFER_USAGE buffer_usage = VERTEX_BUFFER_USAGE::STATIC;
	};

	class IndexBuffer : public Buffer<unsigned int> {
	public:
		void AddIndex(unsigned int index) {
			buffer.emplace_back(index);
		}
	};

	class TextureBuffer : public Buffer<Texture2DHandle> {
	public:
		void AddTexture(const Texture2DHandle& tex) {
			buffer.push_back(tex);
		}
	};

	class TextureHashes : public Buffer<HashedString> {
	public:
		void AddTexture(HashedString hs) {
			buffer.emplace_back(hs);
		}
	};
}