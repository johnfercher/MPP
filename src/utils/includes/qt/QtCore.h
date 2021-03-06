/*The MIT License (MIT)

Copyright (c) 2016 Johnathan Fercher

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#ifndef _QTCORE_H_
#define _QTCORE_H_

#include "QtCore/qabstracteventdispatcher.h"
#include "QtCore/qabstractitemmodel.h"
#include "QtCore/qbasictimer.h"
#include "QtCore/qcoreapplication.h"
#include "QtCore/qcoreevent.h"
#include "QtCore/qeventloop.h"
#include "QtCore/qmath.h"
#include "QtCore/qmetaobject.h"
#include "QtCore/qmetatype.h"
#include "QtCore/qmimedata.h"
#include "QtCore/qobject.h"
#include "QtCore/qobjectcleanuphandler.h"
#include "QtCore/qobjectdefs.h"
#include "QtCore/qpointer.h"
#include "QtCore/qsharedmemory.h"
#include "QtCore/qsignalmapper.h"
#include "QtCore/qsocketnotifier.h"
#include "QtCore/qsystemsemaphore.h"
#include "QtCore/qtimer.h"
#include "QtCore/qtranslator.h"
#include "QtCore/qvariant.h"
#include "QtCore/qfactoryinterface.h"
#include "QtCore/qlibrary.h"
#include "QtCore/qplugin.h"
#include "QtCore/qpluginloader.h"
#include "QtCore/quuid.h"
#include "QtCore/qtextcodec.h"
#include "QtCore/qtextcodecplugin.h"
#include "QtCore/qabstractstate.h"
#include "QtCore/qabstracttransition.h"
#include "QtCore/qeventtransition.h"
#include "QtCore/qfinalstate.h"
#include "QtCore/qhistorystate.h"
#include "QtCore/qsignaltransition.h"
#include "QtCore/qstate.h"
#include "QtCore/qstatemachine.h"
#include "QtCore/qalgorithms.h"
#include "QtCore/qbitarray.h"
#include "QtCore/qbytearray.h"
#include "QtCore/qbytearraymatcher.h"
#include "QtCore/qcache.h"
#include "QtCore/qchar.h"
#include "QtCore/qcontainerfwd.h"
#include "QtCore/qcontiguouscache.h"
#include "QtCore/qcryptographichash.h"
#include "QtCore/qdatetime.h"
#include "QtCore/qeasingcurve.h"
#include "QtCore/qelapsedtimer.h"
#include "QtCore/qhash.h"
#include "QtCore/qiterator.h"
#include "QtCore/qline.h"
#include "QtCore/qlinkedlist.h"
#include "QtCore/qlist.h"
#include "QtCore/qlocale.h"
#include "QtCore/qmap.h"
#include "QtCore/qmargins.h"
#include "QtCore/qpair.h"
#include "QtCore/qpoint.h"
#include "QtCore/qqueue.h"
#include "QtCore/qrect.h"
#include "QtCore/qregexp.h"
#include "QtCore/qscopedpointer.h"
#include "QtCore/qscopedvaluerollback.h"
#include "QtCore/qset.h"
#include "QtCore/qshareddata.h"
#include "QtCore/qsharedpointer.h"
#include "QtCore/qsize.h"
#include "QtCore/qstack.h"
#include "QtCore/qstring.h"
#include "QtCore/qstringbuilder.h"
#include "QtCore/qstringlist.h"
#include "QtCore/qstringmatcher.h"
#include "QtCore/qtextboundaryfinder.h"
#include "QtCore/qtimeline.h"
#include "QtCore/qvarlengtharray.h"
#include "QtCore/qvector.h"
#include "QtCore/qendian.h"
#include "QtCore/qfeatures.h"
#include "QtCore/qglobal.h"
#include "QtCore/qlibraryinfo.h"
#include "QtCore/qnamespace.h"
#include "QtCore/qnumeric.h"
#include "QtCore/qfuture.h"
#include "QtCore/qfutureinterface.h"
#include "QtCore/qfuturesynchronizer.h"
#include "QtCore/qfuturewatcher.h"
#include "QtCore/qrunnable.h"
#include "QtCore/qtconcurrentcompilertest.h"
#include "QtCore/qtconcurrentexception.h"
#include "QtCore/qtconcurrentfilter.h"
#include "QtCore/qtconcurrentfilterkernel.h"
#include "QtCore/qtconcurrentfunctionwrappers.h"
#include "QtCore/qtconcurrentiteratekernel.h"
#include "QtCore/qtconcurrentmap.h"
#include "QtCore/qtconcurrentmapkernel.h"
#include "QtCore/qtconcurrentmedian.h"
#include "QtCore/qtconcurrentreducekernel.h"
#include "QtCore/qtconcurrentresultstore.h"
#include "QtCore/qtconcurrentrun.h"
#include "QtCore/qtconcurrentrunbase.h"
#include "QtCore/qtconcurrentstoredfunctioncall.h"
#include "QtCore/qtconcurrentthreadengine.h"
#include "QtCore/qthreadpool.h"
#include "QtCore/qabstractfileengine.h"
#include "QtCore/qbuffer.h"
#include "QtCore/qdatastream.h"
#include "QtCore/qdebug.h"
#include "QtCore/qdir.h"
#include "QtCore/qdiriterator.h"
#include "QtCore/qfile.h"
#include "QtCore/qfileinfo.h"
#include "QtCore/qfilesystemwatcher.h"
#include "QtCore/qfsfileengine.h"
#include "QtCore/qiodevice.h"
#include "QtCore/qprocess.h"
#include "QtCore/qresource.h"
#include "QtCore/qsettings.h"
#include "QtCore/qtemporaryfile.h"
#include "QtCore/qtextstream.h"
#include "QtCore/qurl.h"
#include "QtCore/qxmlstream.h"
#include "QtCore/qabstractanimation.h"
#include "QtCore/qanimationgroup.h"
#include "QtCore/qparallelanimationgroup.h"
#include "QtCore/qpauseanimation.h"
#include "QtCore/qpropertyanimation.h"
#include "QtCore/qsequentialanimationgroup.h"
#include "QtCore/qvariantanimation.h"
#include "QtCore/qatomic.h"
#include "QtCore/qbasicatomic.h"
#include "QtCore/qmutex.h"
#include "QtCore/qreadwritelock.h"
#include "QtCore/qsemaphore.h"
#include "QtCore/qthread.h"
#include "QtCore/qthreadstorage.h"
#include "QtCore/qwaitcondition.h"

#endif