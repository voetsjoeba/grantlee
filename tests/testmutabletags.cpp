/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef MUTABLETAGSTEST_H
#define MUTABLETAGSTEST_H

#include <QtTest>
#include <QtCore>
#include <QObject>

#include "mutabletemplate.h"
#include "engine.h"
#include "context.h"
#include "grantlee.h"

typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

class TestMutableTagsSyntax : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();

  void testRawTag0();
  void testRawTag();
  void testRepeaterTag();

  void cleanupTestCase();

private:

  void doTest();

  Engine *m_engine;

};

void TestMutableTagsSyntax::initTestCase()
{
  m_engine = Engine::instance();

  QString appDirPath = QFileInfo( QCoreApplication::applicationDirPath() ).absoluteDir().path();
  m_engine->setPluginDirs( QStringList() << appDirPath + "/grantlee_defaulttags/"
                           << appDirPath + "/grantlee_mutabletags/" );

  m_engine->setDefaultLibraries( QStringList() << m_engine->defaultLibraries()
                                 << "grantlee_mutabletags_library" );
}

void TestMutableTagsSyntax::cleanupTestCase()
{
  delete m_engine;
}

void TestMutableTagsSyntax::testRawTag0()
{

  QVariantHash dict;

  dict.insert( "var", "Far" );
  dict.insert( "nameList", QVariantList() << "Tom" << "Dick" << "Harry" );

  Template *t = new MutableTemplate( this );
  t->setContent( "Begin"
                 "{% raw %} Stuff {% endraw %}"
                 "Afters."
               );

  Context c1( dict );
  QString result = t->render( &c1 );
  QString expected = "Begin Stuff Afters.";
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( "var", "SPAM" );
  dict.insert( "nameList", QVariantList() << "Larry" << "Moe" << "Curly" );

  Context c2( dict );
  result = t->render( &c2 );
  QCOMPARE( result, expected );

}

void TestMutableTagsSyntax::testRawTag()
{

  QVariantHash dict;

  dict.insert( "var", "Far" );
  dict.insert( "nameList", QVariantList() << "Tom" << "Dick" << "Harry" );

  Template *t = new MutableTemplate( this );
  t->setContent( "Begin {% for name in nameList %}{{ name }},{% endfor %}"
                 "{% raw %}{% for name in nameList %}{{ name }},{% endfor %} var: {{ var }}. {% endraw %}"
                 "Afters."
               );

  Context c1( dict );
  QString result = t->render( &c1 );
  QString expected = "Begin Tom,Dick,Harry,Tom,Dick,Harry, var: Far. Afters.";
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( "var", "SPAM" );
  dict.insert( "nameList", QVariantList() << "Larry" << "Moe" << "Curly" );

  Context c2( dict );
  result = t->render( &c2 );
  expected = "Begin Tom,Dick,Harry,Larry,Moe,Curly, var: SPAM. Afters.";
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( "var", "EGGS" );
  dict.insert( "nameList", QVariantList() << "John" << "Michael" << "Terry" );

  Context c3( dict );
  result = t->render( &c3 );
  expected = "Begin Tom,Dick,Harry,John,Michael,Terry, var: EGGS. Afters.";
  QCOMPARE( result, expected );

}

void TestMutableTagsSyntax::testRepeaterTag()
{
  QVariantHash dict;

  dict.insert( "var", "Far" );
  dict.insert( "nameList", QVariantList() << "Tom" << "Dick" << "Harry" );

  Template *t = new MutableTemplate( this );
  t->setContent( "{% for name in nameList %}{{ name }},{% endfor %}"
                 "{% repeater %}{% for name in nameList %}{{ name }},{% endfor %} var: {{ var }}. {% endrepeater %}"
                 "Afters"
               );

  Context c1( dict );
  QString result = t->render( &c1 );
  QString expected = "Tom,Dick,Harry,"
                     "Tom,Dick,Harry, var: Far. "
                     "Afters";
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( "var", "SPAM" );
  dict.insert( "nameList", QVariantList() << "Larry" << "Moe" << "Curly" );

  Context c2( dict );
  result = t->render( &c2 );
  expected = "Tom,Dick,Harry,"
             "Tom,Dick,Harry, var: Far. "
             "Larry,Moe,Curly, var: SPAM. "
             "Afters";
  QCOMPARE( result, expected );

  dict.clear();
  dict.insert( "var", "EGGS" );
  dict.insert( "nameList", QVariantList() << "John" << "Michael" << "Terry" );

  Context c3( dict );
  result = t->render( &c3 );
  expected = "Tom,Dick,Harry,"
             "Tom,Dick,Harry, var: Far. "
             "Larry,Moe,Curly, var: SPAM. "
             "John,Michael,Terry, var: EGGS. "
             "Afters";
  QCOMPARE( result, expected );
}

QTEST_MAIN( TestMutableTagsSyntax )
#include "testmutabletags.moc"

#endif
