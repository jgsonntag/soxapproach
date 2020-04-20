/****************************************************************************
** Showfrom meta object code from reading C++ file 'showfrom.h'
**
** Created: Mon Oct 7 11:46:09 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "showfrom.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Showfrom::className() const
{
    return "Showfrom";
}

QMetaObject *Showfrom::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Showfrom( "Showfrom", &Showfrom::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Showfrom::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Showfrom", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Showfrom::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Showfrom", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Showfrom::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QHBox::staticMetaObject();
    static const QUMethod slot_0 = {"update", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "update()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Showfrom", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Showfrom.setMetaObject( metaObj );
    return metaObj;
}

void* Showfrom::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Showfrom" ) )
	return this;
    return QHBox::qt_cast( clname );
}

bool Showfrom::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: update(); break;
    default:
	return QHBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Showfrom::qt_emit( int _id, QUObject* _o )
{
    return QHBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Showfrom::qt_property( int id, int f, QVariant* v)
{
    return QHBox::qt_property( id, f, v);
}

bool Showfrom::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
