#pragma once

#include <QOpenGLFunctions_4_5_Core>

struct VBElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		assert(false);
		return 0;
	}
};

class VBOLayout {
private:
	std::vector<VBElement> elements;
	unsigned int stride;

public:
	VBOLayout() : stride(0) {}

	template<typename T>
	void Push(unsigned int count_) {
		assert(false);
	}

	template<>
	void Push<float>(unsigned int count_) {
		elements.push_back({ GL_FLOAT, count_, GL_FALSE });
		stride += count_ * VBElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count_) {
		elements.push_back({ GL_UNSIGNED_INT, count_, GL_FALSE });
		stride += count_ * VBElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count_) {
		elements.push_back({ GL_UNSIGNED_BYTE, count_, GL_TRUE });
		stride += count_ * VBElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VBElement> GetElements() const { return elements; }
	inline unsigned int GetStride() const { return stride; }
};