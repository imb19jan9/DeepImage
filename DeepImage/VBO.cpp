#include "VBO.h"

#include <QOpenGLFunctions_4_5_Core>

VBO::VBO(const void * data_, unsigned int size_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glGenBuffers(1, &id);
	Bind();
	f->glBufferData(GL_ARRAY_BUFFER, size_, data_, GL_STATIC_DRAW);
	Unbind();
}

VBO::~VBO()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glDeleteBuffers(1, &id);
}

void VBO::Bind() const
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::Unbind() const
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glBindBuffer(GL_ARRAY_BUFFER, 0);
}
