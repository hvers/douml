// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
// Copyright 2012-2012 Nikolai Marchenko.
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// e-mail : enmarantispam@gmail.com
//
// *************************************************************************





#include <qapplication.h>
#include <QTextCodec>
//Added by qt3to4:
#include <Q3CString>
#include <QTextCodec>

#include "UmlWindow.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "UmlDesktop.h"
#include "ToolCom.h"
#include "BrowserView.h"
#include "BrowserPackage.h"
#include "Shortcut.h"
#include "DialogUtil.h"
#include "mu.h"
#include "err.h"
#include "EnvDialog.h"
#include "Logging/QsLogDest.h"
#include "Logging/QsLog.h"
#include "Factories/EdgeMenuFactory.h"
#include "Factories/DialogConnections.h"
#include "Factories/EdgeToolBarCreation.h"
#include "dialog/ClassDialog.h"
#include "dialog/ArtifactDialog.h"
#include "dialog/OperationDialog.h"
#include "ui/constructorinitializerdialog.h"
#include "misc/TypeIdentifier.h"
#include <QSettings>
#include <QFileInfo>




#include "translate.h"

bool ExitOnError = FALSE;
int main(int argc, char ** argv)
{

    ExitOnError = FALSE;

    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QsLogging::Logger & logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    QDir dir;
    dir.setPath(qApp->applicationDirPath());
    dir.remove(QString("cpp_generator") + QString(".log"));
    const QString sLogPath(QDir(qApp->applicationDirPath()).filePath(QString("douml") + QString(".log")));
    QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(sLogPath));
    QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination.get());
    logger.addDestination(fileDestination.get());
    QLOG_INFO() << "Starting the log";


    An<EdgeMenuFactory> factory;
    factory->AddFactory(TypeIdentifier<ClassDialog>::id(), CreateClassDialogMenu);
    factory->AddConnectionFunctor(TypeIdentifier<ClassDialog>::id(), ConnectToClassDialog<EdgeMenuDialog>);
    factory->AddFactory(TypeIdentifier<OperationDialog>::id(), CreateClassDialogMenu);
    factory->AddConnectionFunctor(TypeIdentifier<OperationDialog>::id(), ConnectToClassDialog<EdgeMenuDialog>);
    factory->AddFactory(TypeIdentifier<ArtifactDialog>::id(), CreateLimitedDialogMenu);
    factory->AddConnectionFunctor(TypeIdentifier<ArtifactDialog>::id(), ConnectToLimitedDialog<EdgeMenuDialog>);
    factory->AddFactory(TypeIdentifier<ConstructorInitializerDialog>::id(), CreateLimitedDialogMenu);
    factory->AddConnectionFunctorQt4(TypeIdentifier<ConstructorInitializerDialog>::id(), ConnectToLimitedDialog<EdgeMenuDialogQt4>);


    UmlDesktop::init();
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    bool overridePresent = QFileInfo("override_transition.txt").exists();
    if(settings.value("Main/compatibility_save") .toInt() == 1 && !overridePresent)
    {
    QMessageBox::warning(0, QObject::tr("Warning"),
                         QObject::tr("Douml is working in transitional mode.\n All UI improvements are yours to use,  "
                                     "but saving is done in the format of Bouml 4.22 "
                                     "which loses all new c++11 and hierarchy specifiers\n\n"
                                     "To suppress this warning place empty file override_transition.txt into the application folder\n"
                                     "To disable the mode - change compatibility_save parameter to 0 in settings.ini\n"));
    }

    // note : bool conv_env = !QDir::home().exists(".doumlrc") doesn't work
    // if the path contains non latin1 characters, for instance cyrillic !
    QString s = QDir::home().absFilePath(".doumlrc");
    FILE * fp = fopen((const char *) s, "r");
    bool conv_env = (fp == 0);


    if (conv_env)
        EnvDialog::edit(TRUE);
    else
        fclose(fp);

    read_doumlrc();	// for virtual desktop
    init_pixmaps();
    init_font();
    Shortcut::init(conv_env);

    bool exec = FALSE;
    bool no_gui = FALSE;

    if (argc > 3) {
        if (!strcmp(argv[2], "-execnogui"))
            exec = no_gui = TRUE;
        else
            exec = !strcmp(argv[2], "-exec");
    }

    UmlWindow * uw = new UmlWindow(exec);

    if (no_gui)
        UmlDesktop::set_nogui();
    else
        uw->show();

    if (argc > 1) {
        try {
            if ((argc == 3) &&
                !strcmp(argv[2], "-root") &&
                (msg_critical(TR("DO NOT CONFIRM"),
                              TR("Root mode protection\n\n"
                                 "This mode allows me to develop BOUML\n\n"
                                 "do NOT confirm to avoid a disaster !!!\n\n"
                                 "confirm ?"),
                              QMessageBox::Yes, QMessageBox::No)
                 == QMessageBox::Yes)) {
                set_user_id(0);
                set_editor(getenv("BOUML_EDITOR")); // no environment file
                argc = 1;
            }

            uw->load_it(argv[1]);
        }
        catch (...) {
            // cannot read a file
            return -1;

        }
    }

    QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

    try {
        if (argc > 2) {
            if (exec) {
                bool with_exit = FALSE;

                if (!strcmp(argv[argc - 1], "-exit")) {
                    with_exit = TRUE;
                    argc -= 1;
                }

                Q3CString cmd = argv[3];
                Q3CString space = " ";
                int index;

                for (index = 4; index != argc; index += 1)
                    cmd += space + Q3CString(argv[index]);

                ToolCom::run((const char *) cmd, BrowserView::get_project(), with_exit);
            }
            else
                msg_warning(TR("Error"), TR("Bouml was called with wrong parameters, ignore them"));
        }


        ExitOnError = TRUE;
        a.exec();
    }
    catch (...) {
        ;
    }


    return exit_value();
}
