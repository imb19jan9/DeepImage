#include "FBO.h"

#include <QOpenGLFunctions_4_5_Core>

FBO::FBO(int width_, int height_)
	: width(width_),
	height(height_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glGenFramebuffers(1, &id);
	Bind();

	f->glGenTextures(1, &colorID);
	f->glBindTexture(GL_TEXTURE_2D, colorID);
	f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	f->glGenTextures(1, &depthID);
	f->glBindTexture(GL_TEXTURE_2D, depthID);
	f->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL,
		width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	f->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorID, 0);
	f->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthID, 0);
	f->glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, depthID, 0);

	f->glDrawBuffer(GL_COLOR_ATTACHMENT0);
	Unbind();
}

FBO::~FBO()
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glDeleteFramebuffers(1, &id);
}

void FBO::Bind() const
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FBO::Unbind() const
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
