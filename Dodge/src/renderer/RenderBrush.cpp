#include <renderer/RenderBrush.hpp>
#include <xml/xml.hpp>


namespace Dodge {


//===========================================
// RenderBrush::RenderBrush
//===========================================
RenderBrush::RenderBrush(const RenderBrush& copy) {
   m_fillColour = copy.m_fillColour;
   m_lineColour = copy.m_lineColour;
   m_lineWidth = copy.m_lineWidth;
}

//===========================================
// RenderBrush::RenderBrush
//===========================================
RenderBrush::RenderBrush(const Colour& fCol, const Colour& lCol, int lineW) {
   setFillColour(fCol);
   setLineColour(lCol);
   m_lineWidth = lineW;
}

//===========================================
// RenderBrush::RenderBrush
//===========================================
RenderBrush::RenderBrush(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, RenderRenderBrush);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, fillColour);
      m_fillColour = Colour(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, lineColour);
      m_lineColour = Colour(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, lineWidth);
      m_lineWidth = node.getInt();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of type RenderRenderBrush");
      throw;
   }
}

//===========================================
// RenderBrush::clone
//===========================================
RenderBrush* RenderBrush::clone() const {
   return new RenderBrush(*this);
}


}
