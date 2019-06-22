#include "IBO.h"

#include <QOpenGLFunctions_4_5_Core>

IBO::IBO(const unsigned int * data_, unsigned int count_)
	: count(count_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glGenBuffers(1, &id);
	Bind();
	f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		count * sizeof(unsigned int), data_, GL_STATIC_DRAW);
	Unbind();
}

IBO::~IBO()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glDeleteBuffers(1, &id);
}

void IBO::Bind() const
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IBO::Unbind() const
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();
	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
