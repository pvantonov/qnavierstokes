#include <climits>
#include <cfloat>
#include <limits>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtXml/QDomDocument>
#include "mainwindow.h"

//============================================================================================================
//============================================================================================================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //********************************************************************************************************
    //Создаем окно настроек и окно с информацией о программе.
    //********************************************************************************************************
    settingsWindow = new SettingsWindow(this);
    aboutWindow = new AboutWindow(this);
    helpWindow = new HelpWindow();

    //********************************************************************************************************
    //Убеждаемся в существовании директории для временных файлов и дериктории для записи результата
    //********************************************************************************************************
    if(!QDir(qApp->applicationDirPath()).exists("result"))
        QDir(qApp->applicationDirPath()).mkdir("result");

    if(!QDir(qApp->applicationDirPath()).exists("tmp"))
        QDir(qApp->applicationDirPath()).mkdir("tmp");

    //********************************************************************************************************
    //При необходимости создаем скрипт Surfer'а
    //********************************************************************************************************
    if(!QFile(qApp->applicationDirPath() + "/SurferScript.bas").exists())
        createSurferScript();

    //********************************************************************************************************
    //Загружаем настройки
    //********************************************************************************************************
    if(!QFile(qApp->applicationDirPath() + "/settings.xml").exists())
        saveDefaultSettings();
    loadSettings();

    curEngine = settings.paintEngin;
    if(curEngine == Surfer)
        ui->saveImageButton->setVisible(false);

    on_heightComboBox_currentIndexChanged(0);
    on_tBCComboBox_currentIndexChanged(0);

    //********************************************************************************************************
    //Создаем виджет отрисовки результата
    //********************************************************************************************************
    if(curEngine == Surfer)
    {
        emfPainter = new EmfPainter();
        QVBoxLayout *solutionPainterLayout = new QVBoxLayout();
        solutionPainterLayout->addWidget(emfPainter);
        solutionPainterLayout->setContentsMargins(0,0,0,0);
        ui->solutionPainterFrame->setLayout(solutionPainterLayout);
    }
    else
    {
        glPainter = new OpenGLPainter(settings);
        QVBoxLayout *solutionPainterLayout = new QVBoxLayout();
        solutionPainterLayout->addWidget(glPainter);
        solutionPainterLayout->setContentsMargins(0,0,0,0);
        ui->solutionPainterFrame->setLayout(solutionPainterLayout);
    }

    //********************************************************************************************************
    //Соединяем сигналы/слоты
    //********************************************************************************************************
    connect(ui->aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui->helpAction,SIGNAL(triggered()),helpWindow,SLOT(show()));
    connect(ui->aboutAction,SIGNAL(triggered()),aboutWindow,SLOT(show()));
    connect(ui->settingsAction,SIGNAL(triggered()),this,SLOT(showSettingsWindow()));
    connect(settingsWindow,SIGNAL(settingsUpdated()),this,SLOT(getSettings()));

    connect(&solverSideHeating,SIGNAL(solutionFinished()),this,SLOT(onSolverFinished()));
    connect(&solverBottomHeating,SIGNAL(solutionFinished()),this,SLOT(onSolverFinished()));

    connect(&solverSideHeating,SIGNAL(iterationFinished(int,int,double)),
            this,SLOT(onIterationFinished(int,int,double)));
    connect(&solverBottomHeating,SIGNAL(iterationFinished(int,int,double)),
            this,SLOT(onIterationFinished(int,int,double)));

    connect(&solverSideHeating,SIGNAL(maxIterNumberAttained(double)),
            this,SLOT(onMaxIterNumAttained(double)));
    connect(&solverBottomHeating,SIGNAL(maxIterNumberAttained(double)),
            this,SLOT(onMaxIterNumAttained(double)));
}

void MainWindow::on_actionPrGr_toggled(bool checked)
{
    if(checked)
    {
        ui->prandtlDoubleSpinBox->setMinimum(0.001);
        ui->prandtlDoubleSpinBox->setMaximum(1000.);
        ui->grashofDoubleSpinBox->setMinimum(0.001);
        ui->grashofDoubleSpinBox->setMaximum(100000.);

        ui->prandtlDoubleSpinBox->setDecimals(3);
        ui->grashofDoubleSpinBox->setDecimals(3);

        ui->prandtlDoubleSpinBox->setToolTip(tr("Введите число от 0,001 до 1000"));
        ui->grashofDoubleSpinBox->setToolTip(tr("Введите число от 0,001 до 100000"));
    }
    else
    {
        ui->prandtlDoubleSpinBox->setMinimum(std::numeric_limits<double>::epsilon());
        ui->prandtlDoubleSpinBox->setMaximum(1e22);
        ui->grashofDoubleSpinBox->setMinimum(std::numeric_limits<double>::epsilon());
        ui->grashofDoubleSpinBox->setMaximum(1e22);

        ui->prandtlDoubleSpinBox->setDecimals(18);
        ui->grashofDoubleSpinBox->setDecimals(18);

        ui->prandtlDoubleSpinBox->setToolTip(tr("Введите число от ") + QString::number(std::numeric_limits<double>::epsilon()) + tr(" до ") + QString::number(1e22));
        ui->grashofDoubleSpinBox->setToolTip(tr("Введите число от ") + QString::number(std::numeric_limits<double>::epsilon()) + tr(" до ") + QString::number(1e22));
    }
    saveSettings();
}

//============================================================================================================
//Запуск решателя. Временная деактивация кнопки запуска решателя. Функция вызывается автоматически при
//щелчке на кнопку.
//============================================================================================================
void MainWindow::on_startButton_clicked()
{
    //********************************************************************************************************
    //Удаляем старые результаты
    //********************************************************************************************************
    QFileInfoList fileInfoList = QDir(qApp->applicationDirPath() + "/result").entryInfoList(QStringList(),QDir::Files);
    foreach(QFileInfo fileInfo, fileInfoList)
        QDir(qApp->applicationDirPath() + "/result").remove(fileInfo.absoluteFilePath());

    //********************************************************************************************************
    //Во избежание повторного нажатия кнопки запуска решателя (а это может привести к непредсказуемым
    //последствиям) временно деактивируем эту кнопку. Также деактивируем вызов окна настроек.
    //********************************************************************************************************
    ui->startButton->setDisabled(true);
    ui->settingsAction->setDisabled(true);

    //********************************************************************************************************
    //На основе параметров задачи формируем уникальное имя папки для результата.
    //********************************************************************************************************
    if(ui->tBCComboBox->currentIndex() == 0)
        uniqFoldername = "s";
    else
        uniqFoldername = "b";

    uniqFoldername += ui->heightComboBox->currentText();

    uniqFoldername += "_[Pr";
    uniqFoldername += QString::number(ui->prandtlDoubleSpinBox->value());
    uniqFoldername += ";Gr";
    uniqFoldername += QString::number(ui->grashofDoubleSpinBox->value());
    uniqFoldername += "][";
    uniqFoldername += "t" + QString::number(ui->topWallCheckBox->isChecked());
    uniqFoldername += ";";
    uniqFoldername += "l" + QString::number(ui->leftWallCheckBox->isChecked());
    uniqFoldername += ";";
    uniqFoldername += "r" + QString::number(ui->rightWallCheckBox->isChecked());
    uniqFoldername += ";";
    uniqFoldername += "b" + QString::number(ui->bottomWallCheckBox->isChecked());
    uniqFoldername += "][";
    uniqFoldername += QString::number(ui->nxSpinBox->value());
    uniqFoldername += "x";
    uniqFoldername += QString::number(ui->nySpinBox->value());
    uniqFoldername += "][";
    uniqFoldername += QString::number(ui->wTDoubleSpinBox->value());
    uniqFoldername += ";";
    uniqFoldername += QString::number(ui->wPsiDoubleSpinBox->value());
    uniqFoldername += ";";
    uniqFoldername += QString::number(ui->wOmegaDoubleSpinBox->value());
    uniqFoldername += "]_";
    uniqFoldername += QString::number(ui->maxIterSpinBox->value());

    if(ui->tBCComboBox->currentIndex() == 0)
    {
        solverSideHeating.setProblemParameters(ui->heightComboBox->currentText().toDouble(),
                                               ui->prandtlDoubleSpinBox->value(),
                                               ui->grashofDoubleSpinBox->value(),
                                               ui->leftWallCheckBox->isChecked(),
                                               ui->rightWallCheckBox->isChecked(),
                                               ui->topWallCheckBox->isChecked(),
                                               ui->bottomWallCheckBox->isChecked());
        solverSideHeating.setSolverParameters(ui->nxSpinBox->value(), ui->nySpinBox->value(),
                                              ui->maxIterSpinBox->value(),ui->wTDoubleSpinBox->value(),
                                              ui->wPsiDoubleSpinBox->value(),ui->wOmegaDoubleSpinBox->value());
        solverSideHeating.start();
    }
    else
    {
        solverBottomHeating.setProblemParameters(1.0/ui->heightComboBox->currentText().toDouble(),
                                                 ui->prandtlDoubleSpinBox->value(),
                                                 ui->grashofDoubleSpinBox->value(),
                                                 ui->leftWallCheckBox->isChecked(),
                                                 ui->rightWallCheckBox->isChecked(),
                                                 ui->topWallCheckBox->isChecked(),
                                                 ui->bottomWallCheckBox->isChecked());
        solverBottomHeating.setSolverParameters(ui->nxSpinBox->value(), ui->nySpinBox->value(),
                                                ui->maxIterSpinBox->value(),ui->wTDoubleSpinBox->value(),
                                                ui->wPsiDoubleSpinBox->value(),ui->wOmegaDoubleSpinBox->value());
        solverBottomHeating.start();
    }
}

//============================================================================================================
//Активация кнопки запуска решателя. Обработка результата. Функция вызывается после завершения работы решателя.
//============================================================================================================
void MainWindow::onSolverFinished()
{
    if(curEngine == Surfer)
    {
        if(QFile(settings.pathToScripter).exists())
        {
            createSurferScript();
            QStringList arguments;
            arguments << "-x" << qApp->applicationDirPath() + "/SurferScript.bas";
            QProcess::execute(settings.pathToScripter,arguments);
        }
        emfPainter->setPaintAim(T);
    }
    else
    {
        glPainter->processData();
        glPainter->setPaintAim(T);
    }

    //********************************************************************************************************
    //Копируем результаты в уникальную папку.
    //********************************************************************************************************
    if(settings.useUniqFolders == true)
    {
        if(!QDir(qApp->applicationDirPath() + "/result/" + uniqFoldername).exists())
            QDir(qApp->applicationDirPath() + "/result").mkdir(uniqFoldername);
        else
        {
            QFileInfoList fileInfoList = QDir(qApp->applicationDirPath() + "/result/" + uniqFoldername).entryInfoList(QStringList(),QDir::Files);
            foreach(QFileInfo fileInfo, fileInfoList)
                QDir(qApp->applicationDirPath() + "/result").remove(fileInfo.absoluteFilePath());
        }

        QFileInfoList fileInfoList = QDir(qApp->applicationDirPath() + "/result").entryInfoList(QStringList(),QDir::Files);
        foreach(QFileInfo fileInfo, fileInfoList)
            QFile::copy(fileInfo.absoluteFilePath(),qApp->applicationDirPath() + "/result/" + uniqFoldername + "/" + fileInfo.fileName());
    }

    QMessageBox::information(this,tr("Решение завершено"),tr("Решение завершено"));

    ui->settingsAction->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->paintTButton->setChecked(true);
}

//============================================================================================================
//Установка параметров решателя по умолчанию. Функция вызывается автоматически при смене типа задачи
//(подогрев сбоку/подогрев снизу)
//============================================================================================================
void MainWindow::on_tBCComboBox_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        ui->wTDoubleSpinBox->setValue(solverSideHeating.getDefaultWT());
        ui->wPsiDoubleSpinBox->setValue(solverSideHeating.getDefaultWPsi());
        ui->wOmegaDoubleSpinBox->setValue(solverSideHeating.getDefaultWOmega());
        ui->maxIterSpinBox->setValue(solverSideHeating.getDefaultMaxNumOfIter());
        break;
    case 1:
        ui->wTDoubleSpinBox->setValue(solverBottomHeating.getDefaultWT());
        ui->wPsiDoubleSpinBox->setValue(solverBottomHeating.getDefaultWPsi());
        ui->wOmegaDoubleSpinBox->setValue(solverBottomHeating.getDefaultWOmega());
        ui->maxIterSpinBox->setValue(solverBottomHeating.getDefaultMaxNumOfIter());
        break;
    default:
        ui->wTDoubleSpinBox->setValue(0.3);
        ui->wPsiDoubleSpinBox->setValue(0.3);
        ui->wOmegaDoubleSpinBox->setValue(0.3);
        ui->maxIterSpinBox->setValue(1000);
        break;
    }
}

//============================================================================================================
//Установка числа узлов для сетки по умолчанию. Функция вызывается автоматически при смене высоты расчетной
//области.
//============================================================================================================
void MainWindow::on_heightComboBox_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        ui->nxSpinBox->setValue(100);
        ui->nySpinBox->setValue(25);
        break;
    case 1:
        ui->nxSpinBox->setValue(100);
        ui->nySpinBox->setValue(50);
        break;
    case 2:
        ui->nxSpinBox->setValue(100);
        ui->nySpinBox->setValue(100);
        break;
    case 3:
        ui->nxSpinBox->setValue(50);
        ui->nySpinBox->setValue(100);
        break;
    case 4:
        ui->nxSpinBox->setValue(25);
        ui->nySpinBox->setValue(100);
        break;
    default:
        ui->nxSpinBox->setValue(100);
        ui->nySpinBox->setValue(100);
    }
}

//============================================================================================================
//Отрисовка результата. Функции вызываются автоматически при активации соттветствующих RadioButton.
//============================================================================================================
void MainWindow::on_paintTButton_clicked()
{
    if(curEngine == Surfer)
        emfPainter->setPaintAim(T);
    else
        glPainter->setPaintAim(T);
}

void MainWindow::on_paintPsiButton_clicked()
{
    if(curEngine == Surfer)
        emfPainter->setPaintAim(Psi);
    else
        glPainter->setPaintAim(Psi);
}

void MainWindow::on_paintOmegaButton_clicked()
{
    if(curEngine == Surfer)
        emfPainter->setPaintAim(Omega);
    else
        glPainter->setPaintAim(Omega);
}

void MainWindow::on_paintVxButton_clicked()
{
    if(curEngine == Surfer)
        emfPainter->setPaintAim(Vx);
    else
        glPainter->setPaintAim(Vx);
}

void MainWindow::on_paintVyButton_clicked()
{
    if(curEngine == Surfer)
        emfPainter->setPaintAim(Vy);
    else
        glPainter->setPaintAim(Vy);
}

//============================================================================================================
//Загрузка настроек.
//============================================================================================================
int MainWindow::loadSettings()
{
    //********************************************************************************************************
    //Считываем настройки из файла.
    //********************************************************************************************************
    QDomDocument doc;
    QFile file(qApp->applicationDirPath() + "/settings.xml");
    if(!file.open(QIODevice::ReadOnly))
        return 1;
    if(!doc.setContent(&file))
    {
        file.close();
        return 1;
    }
    file.close();

    //********************************************************************************************************
    //Присваиваем значения переменным настройки.
    //********************************************************************************************************
    ui->actionPrGr->setChecked(doc.documentElement().firstChildElement("restrict_Pr_Gr_value").text() == "true" ? true : false);
    settings.useUniqFolders = doc.documentElement().firstChildElement("use_uniq_folders").text() == "true" ? true : false;
    settings.pathToScripter = doc.documentElement().firstChildElement("path_to_scripter").text();
    settings.paintEngin = doc.documentElement().firstChildElement("paint_engine").text() == "OpenGL" ? OpenGL : Surfer;
    settings.surferVersion = doc.documentElement().firstChildElement("surfer_version").text() == "8" ? Surfer8 : Surfer9;

    QString scheme = doc.documentElement().firstChildElement("color_scheme").text();
    if(scheme == "Rainbow")
        settings.colorScheme = Rainbow;
    else if(scheme == "BlueRed")
        settings.colorScheme = BlueRed;

    return 0;
}

//============================================================================================================
//Сохранение настроек.
//============================================================================================================
int MainWindow::saveSettings()
{
    //********************************************************************************************************
    //Открываем файл настроек на запись.
    //********************************************************************************************************
    QFile file(qApp->applicationDirPath() + "/settings.xml");
    if(!file.open(QIODevice::WriteOnly))
        return 1;

    //********************************************************************************************************
    //********************************************************************************************************
    QDomDocument doc;
    QTextStream out(&file);

    //********************************************************************************************************
    //Создаем корневой тег.
    //********************************************************************************************************
    QDomElement root = doc.createElement("settings");
    doc.appendChild(root);

    //********************************************************************************************************
    //Указываем значение настроек
    //********************************************************************************************************
    QDomText settingValue;
    QDomElement settingValueElement;

    settingValue = doc.createTextNode(ui->actionPrGr->isChecked() ? "true" : "false");
    settingValueElement = doc.createElement("restrict_Pr_Gr_value");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    settingValue = doc.createTextNode(settings.useUniqFolders ? "true" : "false");
    settingValueElement = doc.createElement("use_uniq_folders");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    settingValue = doc.createTextNode(settings.pathToScripter);
    settingValueElement = doc.createElement("path_to_scripter");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    settingValue = doc.createTextNode(settings.paintEngin == OpenGL ? "OpenGL" : "Surfer");
    settingValueElement = doc.createElement("paint_engine");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    if(settings.colorScheme == BlueRed)
        settingValue = doc.createTextNode("BlueRed");
    else if(settings.colorScheme == Rainbow)
        settingValue = doc.createTextNode("Rainbow");
    settingValueElement = doc.createElement("color_scheme");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    settingValue = doc.createTextNode(settings.surferVersion == Surfer8 ? "8" : "9");
    settingValueElement = doc.createElement("surfer_version");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    //********************************************************************************************************
    //Указываем техническую информацию о файле настроек.
    //********************************************************************************************************
    QDomNode xml_node = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"windows-1251\"");
    doc.insertBefore(xml_node,doc.firstChild());

    //********************************************************************************************************
    //Записываем настройки в файл.
    //********************************************************************************************************
    doc.save(out,4);
    file.close();

    return 0;
}

//============================================================================================================
//Сохранение настроек по умолчанию. Вызывается при запуске программы если не обнаружен файл настроек.
//============================================================================================================
int MainWindow::saveDefaultSettings()
{
    //********************************************************************************************************
    //Открываем файл настроек на запись.
    //********************************************************************************************************
    QFile file(qApp->applicationDirPath() + "/settings.xml");
    if(!file.open(QIODevice::WriteOnly))
        return 1;

    //********************************************************************************************************
    //********************************************************************************************************
    QDomDocument doc;
    QTextStream out(&file);

    //********************************************************************************************************
    //Создаем корневой тег.
    //********************************************************************************************************
    QDomElement root = doc.createElement("settings");
    doc.appendChild(root);

    //********************************************************************************************************
    //Указываем значение настроек по умолчанию.
    //********************************************************************************************************
    QDomText settingValue;
    QDomElement settingValueElement;

    settingValue = doc.createTextNode("true");
    settingValueElement = doc.createElement("restrict_Pr_Gr_value");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    settingValue = doc.createTextNode("false");
    settingValueElement = doc.createElement("use_uniq_folders");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    settingValue = doc.createTextNode("C:/Program Files/Golden Software/Surfer8/Scripter/Scripter.exe");
    settingValueElement = doc.createElement("path_to_scripter");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    settingValue = doc.createTextNode("OpenGL");
    settingValueElement = doc.createElement("paint_engine");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    settingValue = doc.createTextNode("Rainbow");
    settingValueElement = doc.createElement("color_scheme");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    settingValue = doc.createTextNode("8");
    settingValueElement = doc.createElement("surfer_version");

    settingValueElement.appendChild(settingValue);
    root.appendChild(settingValueElement);

    //********************************************************************************************************
    //Указываем техническую информацию о файле настроек.
    //********************************************************************************************************
    QDomNode xml_node = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"windows-1251\"");
    doc.insertBefore(xml_node,doc.firstChild());

    //********************************************************************************************************
    //Записываем настройки в файл.
    //********************************************************************************************************
    doc.save(out,4);
    file.close();

    return 0;
}

//============================================================================================================
//Создать скрипт Surfer'а для обработки результатов.
//============================================================================================================
int MainWindow::createSurferScript()
{
    //********************************************************************************************************
    //Открываем файл скрипта на запись.
    //********************************************************************************************************
    QFile file(qApp->applicationDirPath() + "/SurferScript.bas");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return 1;

    QTextStream output(&file);
    output << "Sub Main" << endl;
    output << "Dim Doc As Object" << endl;
    output << "Dim ImageMap As Object" << endl;
    output << "Dim SurferApp As Object" << endl;
    output << "Dim ContourMap As Object" << endl;
    output << "Set SurferApp = CreateObject(\"Surfer.Application\")" << endl;
    output << "Set Doc = SurferApp.Documents.Add" << endl;
    output << "SurferApp.Visible = False" << endl;

    output << "SurferApp.GridData(\"" + qApp->applicationDirPath() + "/result/T.dat\", Algorithm:=srfTriangulation, ShowReport:=False, OutGrid:=\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ImageMap = Doc.Shapes.AddImageMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ContourMap = Doc.Shapes.AddContourMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    if(settings.surferVersion == Surfer8)
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/Samples/Rainbow.clr\")" << endl;
    else
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/ColorScales/Rainbow.clr\")" << endl;
    output << "Doc.Export(\"" + qApp->applicationDirPath() + "/result/T.emf\")" << endl;
    output << "ImageMap.Visible = False" << endl;
    output << "ContourMap.Visible = False" << endl;
    output << "SurferApp.GridData(\"" + qApp->applicationDirPath() + "/result/Psi.dat\", Algorithm:=srfTriangulation, ShowReport:=False, OutGrid:=\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ImageMap = Doc.Shapes.AddImageMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ContourMap = Doc.Shapes.AddContourMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    if(settings.surferVersion == Surfer8)
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/Samples/Rainbow.clr\")" << endl;
    else
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/ColorScales/Rainbow.clr\")" << endl;
    output << "Doc.Export(\"" + qApp->applicationDirPath() + "/result/Psi.emf\")" << endl;
    output << "ImageMap.Visible = False" << endl;
    output << "ContourMap.Visible = False" << endl;

    output << "SurferApp.GridData(\"" + qApp->applicationDirPath() + "/result/Omega.dat\", Algorithm:=srfTriangulation, ShowReport:=False, OutGrid:=\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ImageMap = Doc.Shapes.AddImageMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ContourMap = Doc.Shapes.AddContourMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    if(settings.surferVersion == Surfer8)
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/Samples/Rainbow.clr\")" << endl;
    else
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/ColorScales/Rainbow.clr\")" << endl;
    output << "Doc.Export(\"" + qApp->applicationDirPath() + "/result/Omega.emf\")" << endl;
    output << "ImageMap.Visible = False" << endl;
    output << "ContourMap.Visible = False" << endl;

    output << "SurferApp.GridData(\"" + qApp->applicationDirPath() + "/result/Vx.dat\", Algorithm:=srfTriangulation, ShowReport:=False, OutGrid:=\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ImageMap = Doc.Shapes.AddImageMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ContourMap = Doc.Shapes.AddContourMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    if(settings.surferVersion == Surfer8)
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/Samples/Rainbow.clr\")" << endl;
    else
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/ColorScales/Rainbow.clr\")" << endl;
    output << "Doc.Export(\"" + qApp->applicationDirPath() + "/result/Vx.emf\")" << endl;
    output << "ImageMap.Visible = False" << endl;
    output << "ContourMap.Visible = False" << endl;

    output << "SurferApp.GridData(\"" + qApp->applicationDirPath() + "/result/Vy.dat\", Algorithm:=srfTriangulation, ShowReport:=False, OutGrid:=\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ImageMap = Doc.Shapes.AddImageMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    output << "Set ContourMap = Doc.Shapes.AddContourMap(\"" + qApp->applicationDirPath() + "/tmp/grid\")" << endl;
    if(settings.surferVersion == Surfer8)
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/Samples/Rainbow.clr\")" << endl;
    else
        output << "ImageMap.Overlays(1).ColorMap.LoadFile(FileName:=SurferApp.Path+\"/ColorScales/Rainbow.clr\")" << endl;
    output << "Doc.Export(\"" + qApp->applicationDirPath() + "/result/Vy.emf\")" << endl;
    output << "ImageMap.Visible = False" << endl;
    output << "ContourMap.Visible = False" << endl;

    output << "End Sub" << endl;
    output.flush();
    file.close();

    return 0;
}

//============================================================================================================
//Вывод информации о завершении очередной итерации решателя.
//============================================================================================================
void MainWindow::onIterationFinished(int currentIteration, int maxNumOfIterations ,double residual)
{
    ui->solutionProgressLabel->setText(tr("Прогресс решения: Итерация ") + QString::number(currentIteration) +
                                       tr(" из ") + QString::number(maxNumOfIterations) +
                                       tr(" ; Текущая невязка ") + QString::number(residual));
    ui->solutionProgressBar->setValue((int)((double)(currentIteration)*100/(double)(maxNumOfIterations)));
}

//============================================================================================================
//Вывод информации о том, что решатель не смог найти решение.
//============================================================================================================
void MainWindow::onMaxIterNumAttained(double residual)
{
    QMessageBox::warning(this,tr("Решение не найдено"),tr("Решатель не смог найти решение за отведенное число итераций.\n") +
                         tr("Текущая невязка: ") + QString::number(residual) + tr(".\nРешение может быть неточным или вообще неправильным"));
}

//============================================================================================================
//============================================================================================================
MainWindow::~MainWindow()
{
    delete ui;
    delete settingsWindow;
    delete aboutWindow;
    delete helpWindow;
}

//============================================================================================================
//============================================================================================================
void MainWindow::on_stopButton_clicked()
{
    if(solverBottomHeating.isRunning())
        solverBottomHeating.cutOffSolution();
    if(solverSideHeating.isRunning())
        solverSideHeating.cutOffSolution();

    ui->settingsAction->setEnabled(true);
    ui->startButton->setEnabled(true);
}

//============================================================================================================
//============================================================================================================
void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//============================================================================================================
//Вызов диалогового окна с настройками программы.
//============================================================================================================
void MainWindow::showSettingsWindow()
{
    settingsWindow->init(settings);
    settingsWindow->show();
}

//============================================================================================================
//Получение и сохранение новых настроек программы.
//============================================================================================================
void MainWindow::getSettings()
{
    settings = settingsWindow->getSettings();
    if(curEngine == OpenGL)
        glPainter->setColorScheme(settings.colorScheme);
    saveSettings();
}

//============================================================================================================
//Сохранение изображения функции в файл.
//============================================================================================================
void MainWindow::on_saveImageButton_clicked()
{
    if(curEngine == OpenGL)
    {
        QPixmap glPixmap = QPixmap::grabWidget(glPainter);
        QString filename = qApp->applicationDirPath() + "/result/";
        if(ui->paintTButton->isChecked())
            filename += "T.png";
        else if(ui->paintPsiButton->isChecked())
            filename += "Psi.png";
        else if(ui->paintOmegaButton->isChecked())
            filename += "Omega.png";
        else if(ui->paintVxButton->isChecked())
            filename += "Vx.png";
        else if(ui->paintVyButton->isChecked())
            filename += "Vy.png";
        else
            filename += "unknown.png";
        glPixmap.save(filename,"PNG");
    }
}
