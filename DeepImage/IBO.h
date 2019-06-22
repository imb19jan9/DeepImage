#pragma once

class IBO {
private:
	unsigned int id;
	unsigned int count;

public:
	IBO(const unsigned int* data_, unsigned int count_);
	~IBO();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return count; }
};