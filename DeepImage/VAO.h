#pragma once

#include "VBO.h"
#include "VBOLayout.h"

class VAO {
private:
	unsigned int id;
public:
	VAO();
	~VAO();

	void AddBuffer(const VBO& vbo_, const VBOLayout& layout_);

	void Bind() const;
	void Unbind() const;
};