/****************************************************************************
** Mapview meta object code from reading C++ file 'mapview.h'
**
** Created: Mon Oct 7 11:46:08 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mapview.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Mapview::className() const
{
    return "Mapview";
}

QMetaObject *Mapview::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Mapview( "Mapview", &Mapview::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Mapview::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Mapview", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Mapview::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Mapview", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Mapview::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUMethod slot_0 = {"updatemap", 0, 0 };
    static const QUMethod slot_1 = {"slotzoomin", 0, 0 };
    static const QUMethod slot_2 = {"slotzoomout", 0, 0 };
    static const QUMethod slot_3 = {"slotsettrackup", 0, 0 };
    static const QUMethod slot_4 = {"slotsetnorthup", 0, 0 };
    static const QUMethod slot_5 = {"slotselectbgcolor", 0, 0 };
    static const QUMethod slot_6 = {"slotselectaccolor", 0, 0 };
    static const QUMethod slot_7 = {"slotselectllcolor", 0, 0 };
    static const QUMethod slot_8 = {"slotselectrrcolor", 0, 0 };
    static const QUMethod slot_9 = {"slotselectpathcolor", 0, 0 };
    static const QUMethod slot_10 = {"slotselectdicolor", 0, 0 };
    static const QUMethod slot_11 = {"slotselectwaypointcolor", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"slotsetpathskip", 1, param_slot_12 };
    static const QUMethod slot_13 = {"slotPrevWP", 0, 0 };
    static const QUMethod slot_14 = {"slotNextWP", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "updatemap()", &slot_0, QMetaData::Public },
	{ "slotzoomin()", &slot_1, QMetaData::Public },
	{ "slotzoomout()", &slot_2, QMetaData::Public },
	{ "slotsettrackup()", &slot_3, QMetaData::Public },
	{ "slotsetnorthup()", &slot_4, QMetaData::Public },
	{ "slotselectbgcolor()", &slot_5, QMetaData::Public },
	{ "slotselectaccolor()", &slot_6, QMetaData::Public },
	{ "slotselectllcolor()", &slot_7, QMetaData::Public },
	{ "slotselectrrcolor()", &slot_8, QMetaData::Public },
	{ "slotselectpathcolor()", &slot_9, QMetaData::Public },
	{ "slotselectdicolor()", &slot_10, QMetaData::Public },
	{ "slotselectwaypointcolor()", &slot_11, QMetaData::Public },
	{ "slotsetpathskip(int)", &slot_12, QMetaData::Public },
	{ "slotPrevWP()", &slot_13, QMetaData::Public },
	{ "slotNextWP()", &slot_14, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Mapview", parentObject,
	slot_tbl, 15,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Mapview.setMetaObject( metaObj );
    return metaObj;
}

void* Mapview::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Mapview" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool Mapview::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: updatemap(); break;
    case 1: slotzoomin(); break;
    case 2: slotzoomout(); break;
    case 3: slotsettrackup(); break;
    case 4: slotsetnorthup(); break;
    case 5: slotselectbgcolor(); break;
    case 6: slotselectaccolor(); break;
    case 7: slotselectllcolor(); break;
    case 8: slotselectrrcolor(); break;
    case 9: slotselectpathcolor(); break;
    case 10: slotselectdicolor(); break;
    case 11: slotselectwaypointcolor(); break;
    case 12: slotsetpathskip((int)static_QUType_int.get(_o+1)); break;
    case 13: slotPrevWP(); break;
    case 14: slotNextWP(); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Mapview::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Mapview::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool Mapview::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
