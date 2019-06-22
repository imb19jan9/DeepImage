#pragma once

class FBO {
private:
	unsigned int id;
	unsigned int colorID, depthID;

	int width, height;

public:
	FBO(int width_, int height_);
	~FBO();

	void Bind() const;
	void Unbind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};