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
		Buffer() = default;
		Buffer(const std::vector<T>& buffer_data) : buffer{buffer_data}{}

		const T* GetData() const {
			return buffer.data();
		}

		size_t GetSize() const {
			return buffer.size();
		}

		size_t GetSizeBytes() const {
			return buffer.size() * sizeof(T);
		}

		typedef std::vector<T>::const_iterator buffer_const_iterator;
		typedef std::vector<T>::iterator buffer_iterator;

		buffer_iterator begin() {
			return buffer.begin();
		}

		buffer_iterator end() {
			return buffer.end();
		}

		buffer_const_iterator begin() const{
			return buffer.cbegin();
		}

		buffer_const_iterator end() const{
			return buffer.cend();
		}

	protected:
		std::vector<T> buffer;
		
	};

	class VertexBuffer : public Buffer<Vertex> {
	public:
		VertexBuffer() : Buffer{} {}
		VertexBuffer(const std::vector<Vertex>& buffer_data) : Buffer{buffer_data} {}

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
		IndexBuffer() : Buffer{} {}
		IndexBuffer(const std::vector<unsigned int>& buffer_data) : Buffer{ buffer_data } {}
		void AddIndex(unsigned int index) {
			buffer.emplace_back(index);
		}
	};

	class TextureBuffer : public Buffer<Texture2DHandle> {
	public:
		TextureBuffer() : Buffer{} {}
		TextureBuffer(const std::vector<Texture2DHandle>& buffer_data) : Buffer{ buffer_data } {}
		void AddTexture(const Texture2DHandle& tex) {
			buffer.push_back(tex);
		}
	};
}