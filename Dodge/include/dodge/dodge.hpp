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
#include "Animation.hpp"
#include "AnimFrame.hpp"
#include "Colour.hpp"
#include "CompoundPoly.hpp"
#include "definitions.hpp"
#include "EAnimFinished.hpp"
#include "EEvent.hpp"
#include "EGL_CHECK.hpp"
#include "Entity.hpp"
#include "EntityAnimations.hpp"
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
#include "Quadtree.hpp"
#include "Rectangle.hpp"
#include "Renderer.hpp"
#include "Sprite.hpp"
#include "StackAllocator.hpp"
#include "StringId.hpp"
#include "Texture.hpp"
#include "Vec2f.hpp"
#include "Vec2i.hpp"
#include "Vec3f.hpp"
#include "Vec3i.hpp"


#endif
