/*
  This file is part of the Grantlee template system.

  Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GRANTLEE_TYPEACCESSOR_H
#define GRANTLEE_TYPEACCESSOR_H

#include <QtCore/QVariant>

namespace Grantlee
{

template <typename T>
struct TypeAccessor
{
  static QVariant lookUp( const T object, const QString &property );
};

template <typename T>
struct TypeAccessor<T*>
{
  static QVariant lookUp( const T * const object, const QString &property );
};

namespace
{

template<typename Container>
QVariant doAssociativeContainerLookup( const Container &object, const QString &property )
{
  {
    typename Container::const_iterator it = object.find( property );
    if ( it != object.end() )
      return QVariant::fromValue( *it );
  }
  if ( property == QLatin1String( "items" ) ) {
    typename Container::const_iterator it = object.begin();
    const typename Container::const_iterator end = object.end();
    QVariantList list;
    for( ; it != end; ++it ) {
      QVariantList nested;
      nested.push_back( QVariant::fromValue( it.key() ) );
      nested.push_back( QVariant::fromValue( *it ) );
      list.push_back( nested );
    }
    return list;
  }

  if ( property == QLatin1String( "keys" ) ) {
    typename Container::const_iterator it = object.begin();
    const typename Container::const_iterator end = object.end();
    QVariantList list;
    for( ; it != end; ++it ) {
      list.push_back( QVariant::fromValue( it.key() ) );
    }
    return list;
  }

  if ( property == QLatin1String( "values" ) ) {
    typename Container::const_iterator it = object.begin();
    const typename Container::const_iterator end = object.end();
    QVariantList list;
    for( ; it != end; ++it ) {
      list.push_back( QVariant::fromValue( *it ) );
    }
    return list;
  }

  return QVariant();
}

}

}

#define GRANTLEE_ASSOCIATIVE_TYPE_CONTAINER_ACCESSOR(Container)                    \
namespace Grantlee {                                                               \
template<typename T>                                                               \
struct TypeAccessor<Container<QString, T> >                                        \
{                                                                                  \
  static QVariant lookUp( const Container<QString, T> c, const QString &property ) \
  {                                                                                \
    return doAssociativeContainerLookup( c, property );                            \
  }                                                                                \
};                                                                                 \
}                                                                                  \

GRANTLEE_ASSOCIATIVE_TYPE_CONTAINER_ACCESSOR(QHash)

#endif
