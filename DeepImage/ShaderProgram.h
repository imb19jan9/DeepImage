#pragma once

#include <string>
#include <unordered_map>
#include <QMatrix4x4>

#include "Model3D.h"

class ShaderProgram {
private:
	unsigned int id;
	std::unordered_map<std::string, int> uniformLocationCache;

public:
	ShaderProgram(const std::string& vsFilePath_, const std::string& fsFilePath_);
	~ShaderProgram();

	virtual void Predraw(QMatrix4x4 view_, QMatrix4x4 proj_, Model3D& model_) {}

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name_, int value_);
	void SetUniform4f(const std::string& name_, float v0_, float v1_, float v2_, float v3_);
	void SetUniformMat4f(const std::string& name_, float* mat_);

private:
	std::string LoadShader(const std::string& filepath_);
	unsigned int CompileShader(unsigned int type_, const std::string& source_);
	unsigned int CreateShaderProgram(std::string vs_, std::string fs_);

	int GetUniformLocation(const std::string& name_);
};

class PhongShader : public ShaderProgram
{
public:
	PhongShader();
	~PhongShader() {}

	virtual void Predraw(QMatrix4x4 view_, QMatrix4x4 proj_, Model3D& model_);
};

class SolidColorShader : public ShaderProgram
{
public:
	SolidColorShader();
	~SolidColorShader() {}

	virtual void Predraw(QMatrix4x4 view_, QMatrix4x4 proj_, Model3D& model_);
};

class VertexColorShader : public ShaderProgram
{
public:
	VertexColorShader();
	~VertexColorShader() {}

	virtual void Predraw(QMatrix4x4 view_, QMatrix4x4 proj_, Model3D& model_);
};