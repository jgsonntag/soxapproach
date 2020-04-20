/****************************************************************************
** Showstate meta object code from reading C++ file 'showstate.h'
**
** Created: Mon Oct 7 11:46:10 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "showstate.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Showstate::className() const
{
    return "Showstate";
}

QMetaObject *Showstate::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Showstate( "Showstate", &Showstate::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Showstate::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Showstate", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Showstate::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Showstate", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Showstate::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    static const QUMethod slot_0 = {"update", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "update()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Showstate", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Showstate.setMetaObject( metaObj );
    return metaObj;
}

void* Showstate::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Showstate" ) )
	return this;
    return QVBox::qt_cast( clname );
}

bool Showstate::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: update(); break;
    default:
	return QVBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Showstate::qt_emit( int _id, QUObject* _o )
{
    return QVBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Showstate::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool Showstate::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
