#pragma once

class VBO {
private:
	unsigned int id;

public:
	VBO(const void* data_, unsigned int size_);
	~VBO();

	void Bind() const;
	void Unbind() const;
};