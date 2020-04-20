/****************************************************************************
** Position meta object code from reading C++ file 'position.h'
**
** Created: Mon Oct 7 11:46:09 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "position.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Position::className() const
{
    return "Position";
}

QMetaObject *Position::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Position( "Position", &Position::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Position::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Position", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Position::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Position", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Position::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"slotsetautoleg", 0, 0 };
    static const QUMethod slot_1 = {"slotsetmanual", 0, 0 };
    static const QUMethod slot_2 = {"slotsetturnin", 0, 0 };
    static const QUMethod slot_3 = {"slotsetlocalizer", 0, 0 };
    static const QUMethod slot_4 = {"slotsetglideslope", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "slotsetautoleg()", &slot_0, QMetaData::Public },
	{ "slotsetmanual()", &slot_1, QMetaData::Public },
	{ "slotsetturnin()", &slot_2, QMetaData::Public },
	{ "slotsetlocalizer()", &slot_3, QMetaData::Public },
	{ "slotsetglideslope()", &slot_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Position", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Position.setMetaObject( metaObj );
    return metaObj;
}

void* Position::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Position" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool Position::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotsetautoleg(); break;
    case 1: slotsetmanual(); break;
    case 2: slotsetturnin(); break;
    case 3: slotsetlocalizer(); break;
    case 4: slotsetglideslope(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Position::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Position::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool Position::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
