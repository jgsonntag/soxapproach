/****************************************************************************
** Pathdialog meta object code from reading C++ file 'pathdialog.h'
**
** Created: Mon Oct 7 11:46:08 2013
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "pathdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Pathdialog::className() const
{
    return "Pathdialog";
}

QMetaObject *Pathdialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Pathdialog( "Pathdialog", &Pathdialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Pathdialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Pathdialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Pathdialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Pathdialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Pathdialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"slotAppend", 0, 0 };
    static const QUMethod slot_1 = {"slotOverwrite", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "slotAppend()", &slot_0, QMetaData::Private },
	{ "slotOverwrite()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Pathdialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Pathdialog.setMetaObject( metaObj );
    return metaObj;
}

void* Pathdialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Pathdialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool Pathdialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotAppend(); break;
    case 1: slotOverwrite(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Pathdialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Pathdialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool Pathdialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
