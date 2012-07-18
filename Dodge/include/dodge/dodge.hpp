/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __DODGE_HPP__
#define __DODGE_HPP__


#ifdef WIN32
#include "windows/WinIO.hpp"
#include "windows/Timer.hpp"
#else
#include "linux/WinIO.hpp"
#include "linux/Timer.hpp"
#endif
#include "Colour.hpp"
#include "CompoundPoly.hpp"
#include "definitions.hpp"
#include "EEvent.hpp"
#include "EGL_CHECK.hpp"
#include "EventManager.hpp"
#include "Exception.hpp"
#include "Font.hpp"
#include "GL_CHECK.hpp"
#include "Graphics2d.hpp"
#include "IRenderer.hpp"
#include "ITexture.hpp"
#include "IWinIO.hpp"
#include "PNG_CHECK.hpp"
#include "Poly.hpp"
#include "Renderer.hpp"
#include "rendererTypes.hpp"
#include "StackAllocator.hpp"
#include "StringId.hpp"
#include "Texture.hpp"
#include "Vec2f.hpp"
#include "Vec2i.hpp"


#endif
