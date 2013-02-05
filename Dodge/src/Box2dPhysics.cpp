/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <ctype.h>
#include <Box2dPhysics.hpp>
#include <Timer.hpp>
#include <KvpParser.hpp>
#include <StringId.hpp>
#include <utils/Functor.hpp>
#include <math/fAreEqual.hpp>
#include <math/shapes/shapes.hpp>


using namespace std;


namespace Dodge {


float32_t Box2dPhysics::m_timeStep = 1.f / 60.f;
float32_t Box2dPhysics::m_worldUnitsPerMetre = 0.01f;
int Box2dPhysics::m_v_iterations = 6;
int Box2dPhysics::m_p_iterations = 4;
EventManager Box2dPhysics::m_eventManager = EventManager();
set<EEvent*> Box2dPhysics::m_ignore = set<EEvent*>();
map<Entity*, Box2dPhysics*> Box2dPhysics::m_physEnts = map<Entity*, Box2dPhysics*>();
b2Vec2 Box2dPhysics::m_gravity = b2Vec2(0.f, -9.8f);
b2World Box2dPhysics::m_world = b2World(m_gravity);


//===========================================
// Box2dPhysics::Box2dPhysics
//===========================================
Box2dPhysics::Box2dPhysics(Entity* entity, const XmlNode data)
   : EntityPhysics(entity, data),
     m_entity(entity),
     m_body(NULL),
     m_opts(false, false, 1.f, 0.3f) {

   try {
      XML_NODE_CHECK(data, Box2dPhysics);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, dynamic);
      m_opts.dynamic = attr.getBool();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, fixedAngle);
      m_opts.fixedAngle = attr.getBool();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, density);
      m_opts.density = attr.getFloat();

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, friction);
      m_opts.friction = attr.getFloat();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Box2dPhysics; ");
      throw;
   }
}

//===========================================
// Box2dPhysics::assignData
//===========================================
void Box2dPhysics::assignData(const XmlNode data) {
   if (data.isNull() || data.name() != "Box2dPhysics") return;

   try {
      XmlAttribute attr = data.firstAttribute();
      if (!attr.isNull() && attr.name() == "dynamic") {
         m_opts.dynamic = attr.getBool();
         attr = attr.nextAttribute();
      }

      if (!attr.isNull() && attr.name() == "fixedAngle") {
         m_opts.fixedAngle = attr.getBool();
         attr = attr.nextAttribute();
      }

      if (!attr.isNull() && attr.name() == "density") {
         m_opts.density = attr.getFloat();
         attr = attr.nextAttribute();
      }

      if (!attr.isNull() && attr.name() == "friction") {
         m_opts.friction = attr.getFloat();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Box2dPhysics ;");
      throw;
   }
}

//===========================================
// Box2dPhysics::getSize
//===========================================
size_t Box2dPhysics::getSize() const {
   return sizeof(Box2dPhysics); // TODO Not accurate
}

#ifdef DEBUG
//===========================================
// Box2dPhysics::dbg_print
//===========================================
void Box2dPhysics::dbg_print(std::ostream& out, int tab) const {
   // TODO
}
#endif

//===========================================
// Box2dPhysics::setEntity
//===========================================
void Box2dPhysics::setEntity(Entity* entity) {
   m_entity = entity;
}

//===========================================
// Box2dPhysics::addToWorld
//===========================================
void Box2dPhysics::addToWorld() {
   constructBody();
   m_physEnts[m_entity] = this;
   m_init = true;
}

//===========================================
// Box2dPhysics::removeFromWorld
//===========================================
void Box2dPhysics::removeFromWorld() {
   if (m_init) {
      m_physEnts.erase(m_entity);
      if (m_body) m_world.DestroyBody(m_body);

      m_body = NULL;
      m_numFixtures = 0;

      m_init = false;
   }
}

//===========================================
// Box2dPhysics::shapeToBox2dBody
//===========================================
void Box2dPhysics::shapeToBox2dBody(const Shape& shape, const EntityPhysics::options_t& opts,
   b2Body* body, uint_t* nFixtures) const {

   static long ellipseStr = internString("Ellipse");
   static long lineSegmentStr = internString("LineSegment");
   static long polygonStr = internString("Polygon");
   static long quadStr = internString("Quad");

   string msg("Error constructing Box2D body from Shape");

   *nFixtures = 0;

   if (shape.typeId() == ellipseStr) {
      throw PhysicsException(msg + "; Ellipse not yet supported", __FILE__, __LINE__);
   }
   else if (shape.typeId() == lineSegmentStr) {
      throw PhysicsException(msg + "; LineSegment not yet supported", __FILE__, __LINE__);
   }
   else if (shape.typeId() == polygonStr) { // TODO: Currently assumes convex
      const Polygon& poly = static_cast<const Polygon&>(shape);

      b2Vec2* verts = new b2Vec2[poly.getNumVertices()];

      for (int i = 0; i < poly.getNumVertices(); ++i) {
         verts[i] = b2Vec2(poly.getVertex(i).x / m_worldUnitsPerMetre,
            poly.getVertex(i).y / m_worldUnitsPerMetre);
      }

      b2PolygonShape b2Poly;
      b2Poly.Set(verts, poly.getNumVertices());

      b2FixtureDef fdef;
      fdef.shape = &b2Poly;
      fdef.density = opts.density;
      fdef.friction = opts.friction;

      body->CreateFixture(&fdef);
      ++(*nFixtures);
   }
   else if (shape.typeId() == quadStr) {
      throw PhysicsException(msg + "; Quad not yet supported", __FILE__, __LINE__);
   }
   else
      throw PhysicsException(msg + "Unknown shape type", __FILE__, __LINE__);
}

//===========================================
// Box2dPhysics::constructBody
//===========================================
void Box2dPhysics::constructBody() {
   if (m_body) {
      m_world.DestroyBody(m_body);
      m_numFixtures = 0;
   }

   b2BodyDef bdef;

   if (m_opts.dynamic) bdef.type = b2_dynamicBody;
   bdef.fixedRotation = m_opts.fixedAngle;

   Vec2f pos = m_entity->getTranslation_abs() / m_worldUnitsPerMetre;
   bdef.position.Set(pos.x, pos.y);

   m_body = m_world.CreateBody(&bdef);

   const Shape& entShape = m_entity->getShape();
   shapeToBox2dBody(entShape, m_opts, m_body, &m_numFixtures);
}

//===========================================
// Box2dPhysics::makeDynamic
//===========================================
void Box2dPhysics::makeDynamic() {
   if (!m_init)
      throw PhysicsException("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   if (m_body->GetType() == b2_dynamicBody)
      return;

   m_opts.dynamic = true;
   constructBody();
}

//===========================================
// Box2dPhysics::makeStatic
//===========================================
void Box2dPhysics::makeStatic() {
   if (!m_init)
      throw PhysicsException("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   if (m_body->GetType() != b2_dynamicBody)
      return;

   m_opts.dynamic = false;
   constructBody();
}

//===========================================
// Box2dPhysics::loadSettings
//===========================================
void Box2dPhysics::loadSettings(const string& file) {
   KvpParser parser;
   parser.parseFile(file);

   if (parser.getMetaData(0).compare("Box2D") != 0)
      throw PhysicsException("Error loading settings; File is not for this implementation (Box2D)", __FILE__, __LINE__);

   m_timeStep = 1.f / static_cast<float32_t>(atof(parser.getValue("fps").data()));
   m_worldUnitsPerMetre = static_cast<float32_t>(atof(parser.getValue("worldUnitsPerMetre").data()));
   m_v_iterations = atoi(parser.getValue("vIterations").data());
   m_p_iterations = atoi(parser.getValue("pIterations").data());

   Functor<void, TYPELIST_1(EEvent*)> fEntMovedHandler(&Box2dPhysics::entityMovedHandler);
   m_eventManager.registerCallback(internString("entityTranslation"), fEntMovedHandler);
   m_eventManager.registerCallback(internString("entityRotation"), fEntMovedHandler);
   m_eventManager.registerCallback(internString("entityShape"), fEntMovedHandler);
}

//===========================================
// Box2dPhysics::entityMovedHandler
//===========================================
void Box2dPhysics::entityMovedHandler(EEvent* event) {
//   static long entityRotationStr = internString("entityRotation");
   static long entityShapeStr = internString("entityShape");
   static long entityTranslationStr = internString("entityTranslation");

   Entity* entity;

//   if (event->getType() == entityRotationStr) entity = static_cast<EEntityRotation*>(event)->entity.get();
/*   else*/ if (event->getType() == entityShapeStr) entity = static_cast<EEntityShape*>(event)->entity.get();
   else if (event->getType() == entityTranslationStr) entity = static_cast<EEntityTranslation*>(event)->entity.get();

   if (m_ignore.find(event) == m_ignore.end()) {
      map<Entity*, Box2dPhysics*>::iterator it = m_physEnts.find(entity);
      if (it != m_physEnts.end()) it->second->updatePos(event);
   }
   else
      m_ignore.erase(event);
}

//===========================================
// Box2dPhysics::updatePos
//===========================================
void Box2dPhysics::updatePos(EEvent* ev) {
//   static long entityRotationStr = internString("entityRotation");
   static long entityShapeStr = internString("entityShape");
   static long entityTranslationStr = internString("entityTranslation");

   if (!m_init)
      throw PhysicsException("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   if (ev->getType() == entityTranslationStr) {
      EEntityTranslation* event = static_cast<EEntityTranslation*>(ev);

      // Update position
      float32_t x = event->newTransl_abs.x;
      float32_t y = event->newTransl_abs.y;

      m_body->SetTransform(b2Vec2(x / m_worldUnitsPerMetre, y / m_worldUnitsPerMetre), m_body->GetAngle());
   }/*
   else if (ev->getType() == entityRotationStr) {
      EEntityRotation* event = static_cast<EEntityRotation*>(ev);
      m_body->SetTransform(m_body->GetPosition(), DEG_TO_RAD(event->newRotation_abs));
   }*/
   else if (ev->getType() == entityShapeStr) {
      EEntityShape* event = static_cast<EEntityShape*>(ev);

      unique_ptr<Shape> oldShape(dynamic_cast<Shape*>(event->oldShape.get()->clone()));
      unique_ptr<Shape> newShape(dynamic_cast<Shape*>(event->newShape.get()->clone()));
      oldShape->rotate(-event->oldRotation_abs);
      newShape->rotate(-event->newRotation_abs);

      // Update shape
      if (oldShape == NULL || *oldShape != *newShape) {
         for (unsigned int i = 0; i < m_numFixtures; ++i)
            m_body->DestroyFixture(m_body->GetFixtureList());

         shapeToBox2dBody(*newShape, m_opts, m_body, &m_numFixtures);
      }
   }

   // Wake bodies
   for (map<Entity*, Box2dPhysics*>::iterator it = m_physEnts.begin(); it != m_physEnts.end(); ++it)
      it->second->m_body->SetAwake(true);
}

//===========================================
// Box2dPhysics::update
//===========================================
void Box2dPhysics::update() {
   if (m_physEnts.empty()) return;


   // Update b2Body positions
   m_world.Step(m_timeStep, m_v_iterations, m_p_iterations);
   m_world.ClearForces();


   // Match Entity to b2Body position
   for (map<Entity*, Box2dPhysics*>::iterator it = m_physEnts.begin(); it != m_physEnts.end(); ++it) {
      if (!it->second->m_opts.dynamic) continue;

      Entity* ent = it->second->m_entity;

      Vec2f pos(it->second->m_body->GetPosition()(0) * m_worldUnitsPerMetre,
         it->second->m_body->GetPosition()(1) * m_worldUnitsPerMetre);

      float32_t a = RAD_TO_DEG(it->second->m_body->GetAngle());

      if (pos != ent->getTranslation_abs() || a != ent->getRotation_abs()) {
         Range oldBounds = ent->getBoundary();
         Vec2f oldTransl = ent->getTranslation();
         Vec2f oldTransl_abs = ent->getTranslation_abs();
         float32_t oldRot = ent->getRotation();
         float32_t oldRot_abs = ent->getRotation_abs();

         Vec2f relTrans = ent->getTranslation();
         Vec2f absTrans = ent->getTranslation_abs();

         ent->setSilent(true);
         ent->setTranslation(relTrans + (pos - absTrans));
         ent->setRotation(a);
         ent->setSilent(false);

         EEvent* event1 = new EEntityBoundingBox(ent->getSharedPtr(), oldBounds, ent->getBoundary());
         EEvent* event2 = new EEntityTranslation(ent->getSharedPtr(), oldTransl, oldTransl_abs, ent->getTranslation(), ent->getTranslation_abs());
         EEvent* event3 = new EEntityRotation(ent->getSharedPtr(), oldRot, oldRot_abs, ent->getRotation(), ent->getRotation_abs());

         ent->onEvent(event1);
         ent->onEvent(event2);
         ent->onEvent(event3);

         m_eventManager.queueEvent(event1);
         m_eventManager.queueEvent(event2);
         m_eventManager.queueEvent(event3);

         m_ignore.insert(event1);
         m_ignore.insert(event2);
         m_ignore.insert(event3);
      }
   }
}

//===========================================
// Box2dPhysics::getLinearVelocity
//===========================================
Vec2f Box2dPhysics::getLinearVelocity() const {
   if (!m_init)
      throw PhysicsException("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   return Vec2f(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
}

//===========================================
// Box2dPhysics::applyLinearImpulse
//===========================================
void Box2dPhysics::applyLinearImpulse(const Vec2f& impulse, const Vec2f& p) {
   if (!m_init)
      throw PhysicsException("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   m_body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(static_cast<float>(p.x) / m_worldUnitsPerMetre,
      static_cast<float>(p.y) / m_worldUnitsPerMetre));
}

//===========================================
// Box2dPhysics::applyLinearImpulse
//===========================================
void Box2dPhysics::applyLinearImpulse(const Vec2f& impulse) {
   if (!m_init)
      throw PhysicsException("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   applyLinearImpulse(impulse, Vec2f(m_body->GetWorldCenter().x * m_worldUnitsPerMetre,
      m_body->GetWorldCenter().y * m_worldUnitsPerMetre));
}

//===========================================
// Box2dPhysics::applyForce
//===========================================
void Box2dPhysics::applyForce(const Vec2f& force, const Vec2f& p) {
   if (!m_init)
      throw PhysicsException("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   m_body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(static_cast<float>(p.x) / m_worldUnitsPerMetre,
      static_cast<float>(p.y) / m_worldUnitsPerMetre));
}

//===========================================
// Box2dPhysics::applyForce
//===========================================
void Box2dPhysics::applyForce(const Vec2f& force) {
   if (!m_init)
      throw PhysicsException("Instance of Box2dPhysics is not initialised", __FILE__, __LINE__);

   applyForce(force, Vec2f(m_body->GetWorldCenter().x * m_worldUnitsPerMetre,
      m_body->GetWorldCenter().y * m_worldUnitsPerMetre));
}


}
