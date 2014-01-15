#include <string>
#include <sstream>
#include <iostream>
#include <dodge/dodge.hpp>
#include "Test.hpp"


#define SUBCASE_CHECK(x, pass, b) \
{ \
   if (!x && m_verbose && !b) { \
      std::cout << "\t\tFAILED AT LINE " << __LINE__ << "\n"; \
      pass = false; \
      b = true; \
   } \
}

#define SUBCASE_CHECK_CRITICAL(x) \
{ \
   if (!x && m_verbose) { \
      std::cout << "\t\tFAILED AT LINE " << __LINE__ << "\n"; \
      return false; \
   } \
}


using namespace std;
using namespace Dodge;


bool Test::testCase0() const {
   bool pass = true;

   if (m_verbose) cout << "\tCASE 0: Parsing bad document\n";
   try {
      XmlDocument doc;
      doc.parse("./test0.xml");

      pass = false;
   }
   catch (XmlException& e) {}

   if (m_verbose) cout << (pass ? "\tPASS\n" : "\tFAIL\n");
   return pass;
}

bool Test::testCase1() const {
   bool pass = true;
   bool b = false;

   if (m_verbose) cout << "\tCASE 1: Parsing good document and checking tree\n";
   try {
      XmlDocument doc;
      doc.parse("./test1.xml");

      /*
      <?xml version="1.0" encoding="utf-8"?>
      <element1 a="1" b="2">
         <element2 c="3" d="true"/>
         <element3>Hello World!</element3>
         <element4 a="123">
            <element5 yes="no">
               <element6 str="This is a string" num="4.5"/>
               <element7>99.8</element7>
            </element5>
         </element4>
      </element1>
      */

      XmlNode node = doc.firstNode();
      node = node.nextSibling();

      SUBCASE_CHECK_CRITICAL(!node.isNull());
      SUBCASE_CHECK(node.name().compare("element1") == 0, pass, b);

      XmlAttribute attr = node.firstAttribute();
      SUBCASE_CHECK_CRITICAL(!attr.isNull());
      SUBCASE_CHECK(attr.name().compare("a") == 0, pass, b);
      SUBCASE_CHECK(attr.getInt() == 1, pass, b);

      attr = attr.nextAttribute();
      SUBCASE_CHECK_CRITICAL(!attr.isNull());
      SUBCASE_CHECK(attr.name().compare("b") == 0, pass, b);
      SUBCASE_CHECK(attr.getInt() == 2, pass, b);

      {
         XmlNode node_ = node.firstChild();
         SUBCASE_CHECK_CRITICAL(!node_.isNull());
         SUBCASE_CHECK(node_.name().compare("element2") == 0, pass, b);

         XmlAttribute attr = node_.firstAttribute();
         SUBCASE_CHECK_CRITICAL(!attr.isNull());
         SUBCASE_CHECK(attr.name().compare("c") == 0, pass, b);
         SUBCASE_CHECK(attr.getInt() == 3, pass, b);

         attr = attr.nextAttribute();
         SUBCASE_CHECK_CRITICAL(!attr.isNull());
         SUBCASE_CHECK(attr.name().compare("d") == 0, pass, b);
         SUBCASE_CHECK(attr.getString().compare("true") == 0, pass, b);

         node_ = node_.nextSibling();
         SUBCASE_CHECK_CRITICAL(!node_.isNull());
         SUBCASE_CHECK(node_.name().compare("element3") == 0, pass, b);
         SUBCASE_CHECK(node_.getString().compare("Hello World!") == 0, pass, b);

         node_ = node_.nextSibling();
         SUBCASE_CHECK_CRITICAL(!node_.isNull());
         SUBCASE_CHECK(node_.name().compare("element4") == 0, pass, b);

         attr = node_.firstAttribute();
         SUBCASE_CHECK_CRITICAL(!attr.isNull());
         SUBCASE_CHECK(attr.name().compare("a") == 0, pass, b);
         SUBCASE_CHECK(attr.getInt() == 123, pass, b);

         {
            XmlNode node__ = node_.firstChild();
            SUBCASE_CHECK_CRITICAL(!node__.isNull());
            SUBCASE_CHECK(node__.name().compare("element5") == 0, pass, b);

            XmlAttribute attr = node__.firstAttribute();
            SUBCASE_CHECK_CRITICAL(!attr.isNull());
            SUBCASE_CHECK(attr.name().compare("yes") == 0, pass, b);
            SUBCASE_CHECK(attr.getString().compare("no") == 0, pass, b);

            {
               XmlNode node___ = node__.firstChild();
               SUBCASE_CHECK_CRITICAL(!node___.isNull());
               SUBCASE_CHECK(node___.name().compare("element6") == 0, pass, b);

               XmlAttribute attr = node___.firstAttribute();
               SUBCASE_CHECK_CRITICAL(!attr.isNull());
               SUBCASE_CHECK(attr.name().compare("str") == 0, pass, b);
               SUBCASE_CHECK(attr.getString().compare("This is a string") == 0, pass, b);

               attr = attr.nextAttribute();
               SUBCASE_CHECK_CRITICAL(!attr.isNull());
               SUBCASE_CHECK(attr.name().compare("num") == 0, pass, b);
               SUBCASE_CHECK(attr.getFloat() == 4.5, pass, b);
            }
         }
      }
   }
   catch (XmlException& e) {
      pass = false;
      if (m_verbose) {
         cout << e.what() << "\n";
      }
   }

   if (m_verbose) cout << (pass ? "\tPASS\n" : "\tFAIL\n");
   return pass;
}

bool Test::testCase2() const {
   if (m_verbose) cout << "\tCase 2: Parse and print\n";

   string text =
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
      "<element1 a=\"1\" b=\"2\">\n"
      "\t<element2 c=\"3\" d=\"true\"/>\n"
      "\t<element3>Hello World!</element3>\n"
      "\t<element4 a=\"123\">\n"
      "\t\t<element5 yes=\"no\">\n"
      "\t\t\t<element6 str=\"This is a string\" num=\"4.5\"/>\n"
      "\t\t\t<element7>99.8</element7>\n"
      "\t\t</element5>\n"
      "\t</element4>\n"
      "</element1>\n\n";

   XmlDocument doc;
   doc.parse(text.data(), text.length());

   string output;
   doc.print(output);

   bool pass = output.compare(text) == 0;

   if (m_verbose) cout << (pass ? "\tPASS\n" : "\tFAIL\n");
   return pass;
}

bool Test::testCase3() const {
   if (m_verbose) cout << "\tCase 3: Move node between documents (1)\n";

   /*
   <nodeA>
      <nodeB a="1" b="two" c="3.4"/>
      <nodeC d="true">
         <nodeD>
            Hello World!
         </nodeD>
         <nodeE>
            Goodbye World!
         </nodeE>
      </nodeC>
   </nodeA>
   */

   XmlDocument doc2;
   string tree1, tree2;

   {
      XmlDocument doc1;

      XmlNode nodeA = doc1.addNode("nodeA");

      XmlNode nodeB(doc1, "nodeB");
      nodeB.addAttribute("a", "1");
      nodeB.addAttribute("b", "two");
      nodeB.addAttribute("c", "3.4");

      nodeA.addNode(nodeB);

      XmlNode nodeC(doc1, "nodeC");
      nodeC.addAttribute("d", "true");

      nodeA.addNode(nodeC);

      XmlNode nodeD(doc1, "nodeD");
      nodeD.setValue("Hello World!");

      nodeC.addNode(nodeD);

      XmlNode nodeE(doc1, "nodeE");
      nodeE.setValue("Goodbye World!");

      nodeC.addNode(nodeE);

      {
         XmlNode nodeA = doc2.addNode("nodeA");

         XmlNode nodeB(doc2, "nodeB");
         nodeB.addAttribute("a", "1");
         nodeB.addAttribute("b", "two");
         nodeB.addAttribute("c", "3.4");

         nodeA.addNode(nodeB);
         nodeA.addNode(nodeC);
      }

      doc1.print(tree1);
   } // doc1 destroyed here

   doc2.print(tree2);

   bool pass = tree1 == tree2;

   if (m_verbose) cout << (pass ? "\tPASS\n" : "\tFAIL\n");
   return pass;
}

bool Test::testCase4() const {
   if (m_verbose) cout << "\tCase 4: Move node between documents (2)\n";

   /*
   <nodeA>
      <nodeB a="1" b="two" c="3.4"/>
      <nodeC d="true">
         <nodeD>
            Hello World!
         </nodeD>
         <nodeE>
            Goodbye World!
         </nodeE>
      </nodeC>
   </nodeA>
   */

   XmlDocument doc2;

   try {
      XmlDocument doc1;
      doc1.parse("./test4.xml");

      XmlNode node = doc1.firstNode();
      XML_NODE_CHECK(node, nodeA);

      node = node.firstChild();
      XML_NODE_CHECK(node, nodeB);

      node = node.nextSibling();
      XML_NODE_CHECK(node, nodeC);

      XmlNode node_ = doc2.addNode("MyNode");
      node_.addNode(node);
   } // doc1 destroyed here
   catch (XmlException& e) {
      if (m_verbose) {
         cout << e.what() << "\n";
      }
      return false;
   }

   bool pass = true;
   bool b = false;

   XmlNode node = doc2.firstNode();
   SUBCASE_CHECK_CRITICAL(!node.isNull());
   SUBCASE_CHECK(node.name().compare("MyNode") == 0, pass, b);

   XmlNode node_ = node.firstChild();
   SUBCASE_CHECK_CRITICAL(!node_.isNull());
   SUBCASE_CHECK(node_.name().compare("nodeC") == 0, pass, b);

   XmlAttribute attr = node_.firstAttribute();
   SUBCASE_CHECK_CRITICAL(!attr.isNull());
   SUBCASE_CHECK(attr.name().compare("d") == 0, pass, b);
   SUBCASE_CHECK(attr.getString().compare("true") == 0, pass, b);

   XmlNode node__ = node_.firstChild();
   SUBCASE_CHECK_CRITICAL(!node__.isNull());
   SUBCASE_CHECK(node__.name().compare("nodeD") == 0, pass, b);

   node__ = node__.nextSibling();
   SUBCASE_CHECK_CRITICAL(!node__.isNull());
   SUBCASE_CHECK(node__.name().compare("nodeE") == 0, pass, b);

   if (m_verbose) cout << (pass ? "\tPASS\n" : "\tFAIL\n");
   return pass;
}

void Test::run(bool verbose) {
   m_verbose = verbose;
   bool pass = true;

   cout << "TEST: XML\n";
   if (!testCase0()) pass = false;
   if (!testCase1()) pass = false;
   if (!testCase2()) pass = false;
   if (!testCase3()) pass = false;
   if (!testCase4()) pass = false;

   cout << "RESULT: " << (pass ? "PASS" : "FAIL") << "\n";
}
