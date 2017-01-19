#include "frame.hpp"


#include <logger.hpp>
#include "shaders/ttsshader.hpp"
#include "meshes/plane.hpp"


logger_t moduleLogger = loggerModule(loggerLDebug, loggerDFull);


// -------------------------- Abstract Frame -------------------------- //

Frame::Frame(const std::string& title,
			 const size_t& posX,
			 const size_t& posY,
			 const size_t& width,
			 const size_t& height,
			 const unsigned int& displayMode)
	: Component("FRAME", title),
	  _title(title),
	  _posX(posX),
	  _posY(posY),
	  _width(width),
	  _height(height),
	  _displayMode(displayMode)
{
	init();
	logDebug << "Frame " << getName()
			 << " [" << _width << "x" << _height << "] created."
			 << logEndl;
}


Frame::~Frame()
{
	logDebug << "Frame " << _title << " removed." << logEndl;
}


const size_t& Frame::getWidth() const
{
	return _width;
}


const size_t& Frame::getHeight() const
{
	return _height;
}


void Frame::setPos(const size_t& posX, const size_t& posY)
{
	_posX = posX;
	_posY = posY;
}


void Frame::setViewMatrix(const glm::mat4x4& mv)
{
	_MV = mv;
}


void Frame::setProjectionMatrix(const glm::mat4x4& mp)
{
	_MP = mp;
}


void Frame::init()
{
	static GLFrame glFrame(_title, _posX, _posY, _width, _height, _displayMode);
	_glWindow = glFrame.getGLWindowNum();
}


bool Frame::validate()
{
	return (_glWindow != 0);
}


void Frame::resize(const size_t& width,
				   const size_t& height)
{
	_width = width;
	_height = height;

	glViewport(0, 0, (GLsizei)_width, (GLsizei)_height);
}


void Frame::clear(const Color& color)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(color.getRedF(),
				 color.getGreenF(),
				 color.getBlueF(),
				 color.getAlphaF());
	glClearDepth(1.0f);
}


void Frame::draw(const Primitive& primitive,
				 const LightPtr& light,
				 const CameraPtr& camera)
{
	MaterialPtr mat = primitive.getMaterial();
	mat->use();

	const ShaderPtr& sh = primitive.getShader();
	sh->passViewMatrix(_MV);
	sh->passProjectionMatrix(_MP);
	sh->passComponent(light);
	sh->passComponent(camera);

	primitive.draw();
}


void Frame::flush()
{
	glFlush();
}


void Frame::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


bool Frame::_checkFBO(GLuint& fbo) const
{
	bool result;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE:
			logInfo <<  "Framebuffer complete." << logEndl;
			result = true;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			logError <<  "Framebuffer incomplete: Attachment is NOT complete." << logEndl;
			result = false;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			logError <<  "Framebuffer incomplete: No image is attached to FBO." << logEndl;
			result = false;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			logError <<  "Framebuffer incomplete: Draw buffer." << logEndl;
			result = false;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			logError <<  "Framebuffer incomplete: Read buffer." << logEndl;
			result = false;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			logError <<  "Framebuffer incomplete: Multisample." << logEndl;
			result = false;
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			logError <<  "Framebuffer incomplete: Unsupported by FBO implementation." << logEndl;
			result = false;
			break;

		default:
			logError <<  "Framebuffer incomplete: Unknown error." << logEndl;
			result = false;
			break;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return result;
}


// -------------------------- OpenGL frame -------------------------- //

Frame::GLFrame::GLFrame(const std::string& title,
						const size_t& posX,
						const size_t& posY,
						const size_t& width,
						const size_t& height,
						const unsigned int& displayMode)
{
	logDebug << "GL frame initialization started." << logEndl;

	int argc = 1;
	char* argv[1] = {"engine"};

	glutInit(&argc, argv);
	glutInitDisplayMode(displayMode);
	glutInitWindowPosition(posX, posY);
	glutInitWindowSize(width, height);

	_glWindow = glutCreateWindow(title.c_str());

	GLenum err = glewInit();
	if (err != GLEW_OK)
		logFatal << "Error in glewInit, code: " << err
				 << ", message: " << glewGetErrorString(err) << logEndl;
	else if (_glWindow == 0)
		logFatal << "Invalid GL window: glutCreateWindow returned 0." << logEndl;

	logDebug << "GL frame initialization finished." << logEndl;
}


Frame::GLFrame::~GLFrame()
{
	// FIXME: glutDestroyWindow call raises an error: Function <glutDestroyWindow> called without first calling 'glutInit'.
//	glutDestroyWindow(_glWindow);
	logDebug << "GL frame deinitialized." << logEndl;
}


int Frame::GLFrame::getGLWindowNum() const
{
	return _glWindow;
}


// -------------------------- DoubleBufferFrame -------------------------- //

DoubleBufferFrame::DoubleBufferFrame(const std::string& title,
									 const size_t& posX,
									 const size_t& posY,
									 const size_t& width,
									 const size_t& height)
	: Frame(title, posX, posY, width, height, (GLUT_RGBA | GLUT_DOUBLE))
{
}


DoubleBufferFrame::~DoubleBufferFrame()
{
}


void DoubleBufferFrame::flush()
{
	glutSwapBuffers();
}


// -------------------------- RTRFrame -------------------------- //

RTTFrame::RTTFrame(const std::string& title,
				   const size_t& posX,
				   const size_t& posY,
				   const size_t& width,
				   const size_t& height)
	: Frame(title, posX, posY, width, height, (GLUT_RGBA | GLUT_DEPTH)),
	  _fboId(0),
	  _colorTexture(width, height),
	  _depthRboId(0)
{
	init();
}


RTTFrame::~RTTFrame()
{
	delete _screenPlane;
}


void RTTFrame::init()
{
	Frame::init();

	glGenFramebuffers(1, &_fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);

	_colorTexture.generate();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _colorTexture.getBuffId(), NULL);

	glGenRenderbuffers(1, &_depthRboId);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRboId);

	MeshPtr plane = new Plane();
	plane->setPosition(0.5, 0.5, 0.0);
	plane->setRotation(1.0, 0.0, 0.0);
	plane->setScale(5.0, 1.0, 5.0);
	plane->applyScale();
	plane->applyRotation();
	plane->applyPosition();
	_screenPlane = new Primitive(plane);

	_ttsShader = new TTSShader();
	_ttsShader->compile();
}


bool RTTFrame::validate()
{
	bool r = Frame::validate();
	_checkFBO(_fboId); // TODO: use result.
	return r;
}


void RTTFrame::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
}


void RTTFrame::clear(const Color& color)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(color.getRedF(),
				 color.getGreenF(),
				 color.getBlueF(),
				 color.getAlphaF());
	glClearDepth(1.0f);
}


void RTTFrame::draw(const Primitive& primitive,
					const LightPtr& light,
					const CameraPtr& camera)
{
//	ShaderPtr lsh = light->getShader();
//	if (!lsh.is_null())
//	{
//		lsh->bind();

//		lsh->passViewMatrix(light->getOrthoMatrix());
//		lsh->passComponent(light);
//		primitive.draw();

//		lsh->unbind();
//	}

	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	MaterialPtr mat = primitive.getMaterial();
	mat->use();

	const ShaderPtr& sh = primitive.getShader();
	sh->passViewMatrix(_MV);
	sh->passProjectionMatrix(_MP);
	sh->passComponent(light);
	sh->passComponent(camera);
	primitive.draw();

//	if (!lsh.is_null())
//	{
//		_colorTexture.bind();
//		sh->passUniform("shadows", 1.0f);
//		sh->passUniform("shadowMap", _colorTexture.getSamplerId());
//		sh->passUniform("screenW", float(_width));
//		sh->passUniform("screenH", float(_height));
//	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_ttsShader->bind();
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, _colorTexture.getBuffId());
	_ttsShader->passUniform("text", 1);
	_ttsShader->passViewMatrix(_MV);
	_ttsShader->passProjectionMatrix(_MP);
	_screenPlane->draw();
}


void RTTFrame::flush()
{
	glFlush();
}


// -------------------------- TextureBufferFrame -------------------------- //

TextureBufferFrame::TextureBufferFrame(const std::string& title,
						 const size_t& posX,
						 const size_t& posY,
						 const size_t& width,
						 const size_t& height)
	: Frame(title, posX, posY, width, height, GLUT_RGBA),
	  _frontBuffer(0),
	  _frontTexture(0),
	  _backBuffer(0),
	  _backTexture(0)
{
	logDebug << "Buffer frame created." << logEndl;
}


TextureBufferFrame::~TextureBufferFrame()
{
	if (!isBuffersValid())
		return;

	_deinitGLBufferAndTexture(_frontBuffer, _frontTexture);
	_deinitGLBufferAndTexture(_backBuffer, _backTexture);

	logDebug << "Buffer frame removed." << logEndl;
}


bool TextureBufferFrame::isBuffersValid() const
{
	return ((_frontBuffer != 0)
			&& (_backBuffer != 0)
			&& (_frontTexture != 0)
			&& (_backTexture != 0)
			&& (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE));
}


void TextureBufferFrame::init()
{
	logDebug << "Initialization started." << logEndl;

	Frame::init();

	_initGLBufferAndTexture(_frontBuffer, _frontTexture);
	_initGLBufferAndTexture(_backBuffer, _backTexture);

	// Set the list of draw buffers.
	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(sizeof(drawBuffers), drawBuffers);

	logDebug << "Initialization finished." << logEndl;
}


bool TextureBufferFrame::validate()
{
	init();
	return isBuffersValid();
}


void TextureBufferFrame::clear(const Color& color)
{
	// Clear _back frame ONLY!
	glViewport(0, 0, _width, _height);
	glBindFramebuffer(GL_FRAMEBUFFER, _backBuffer);
	Frame::clear(color);

	logDebug << "Buffer " << _backBuffer << " is clear." << logEndl;
}


void TextureBufferFrame::flush()
{
	/*
	 * A new frame (where a new picture was stored to) is _back,
	 * _front is an old picture at this moment.
	 * So here we swap them, new drawing will be displayed.
	 */
	std::swap(_frontBuffer, _backBuffer);
	std::swap(_frontTexture, _backTexture);

	// Show _front frame ONLY!
//	glViewport(0, 0, _width, _height);
	glBindFramebuffer(GL_FRAMEBUFFER, _frontBuffer);
	glFlush();

	logDebug << "Buffer " << _frontBuffer << " was displayed." << logEndl;
}


void TextureBufferFrame::_initGLBufferAndTexture(GLuint& buffer,
										  GLuint& texture)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	logDebug << "Texture: " << texture << ", FBO: " << buffer << logEndl;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		logError << "FBO is invalid" << logEndl;
}


void TextureBufferFrame::_deinitGLBufferAndTexture(GLuint& buffer, GLuint& texture)
{
	glDeleteTextures(1, &texture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &buffer);
}
