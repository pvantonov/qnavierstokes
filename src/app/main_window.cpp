#include <climits>
#include <cfloat>
#include <limits>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>
#include <settings.hpp>
#include <settings_manager.hpp>
#include "main_window.hpp"

//============================================================================================================
//============================================================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settingsWindow(new SettingsWindow(this)),
    helpWindow(new HelpWindow(this))
{
    ui->setupUi(this);

    //********************************************************************************************************
    //Создаем окно настроек и окно с информацией о программе.
    //********************************************************************************************************
    aboutWindow = new AboutWindow(this);

    //********************************************************************************************************
    //Убеждаемся в существовании директории для временных файлов и дериктории для записи результата
    //********************************************************************************************************
    if(!QDir(qApp->applicationDirPath()).exists("result"))
        QDir(qApp->applicationDirPath()).mkdir("result");

    if(!QDir(qApp->applicationDirPath()).exists("tmp"))
        QDir(qApp->applicationDirPath()).mkdir("tmp");

    //********************************************************************************************************
    //Загружаем настройки
    //********************************************************************************************************

    auto settings = SettingsManager::instance().settings();

    on_heightComboBox_currentIndexChanged(0);
    on_tBCComboBox_currentIndexChanged(0);

    //********************************************************************************************************
    //Создаем виджет отрисовки результата
    //********************************************************************************************************
    glPainter = new OpenGLPainter();
    QVBoxLayout *solutionPainterLayout = new QVBoxLayout();
    solutionPainterLayout->addWidget(glPainter);
    solutionPainterLayout->setContentsMargins(0,0,0,0);
    ui->solutionPainterFrame->setLayout(solutionPainterLayout);

    //********************************************************************************************************
    //Соединяем сигналы/слоты
    //********************************************************************************************************
    connect(ui->aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui->helpAction,SIGNAL(triggered()),this,SLOT(showHelpWindow()));
    connect(ui->aboutAction,SIGNAL(triggered()),aboutWindow,SLOT(show()));
    connect(ui->settingsAction,SIGNAL(triggered()),this,SLOT(showSettingsWindow()));

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
    auto settings = SettingsManager::instance().settings();
    auto newSettings(settings);
    newSettings.limitPrGr = checked;
    SettingsManager::instance().setSettings(newSettings);
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

    if(ui->tBCComboBox->currentIndex() == 0)
    {
        solverSideHeating.setProblemParameters(ui->heightComboBox->currentText().replace(',', '.').toDouble(),
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
        solverBottomHeating.setProblemParameters(1.0/ui->heightComboBox->currentText().replace(',', '.').toDouble(),
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
    auto settings = SettingsManager::instance().settings();

    glPainter->processData();
    glPainter->setPaintAim(T);

    //********************************************************************************************************
    //Копируем результаты в уникальную папку.
    //********************************************************************************************************
    if(settings.useUniqueFolders == true)
    {
        QString uniqFoldername;

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
    glPainter->setPaintAim(T);
}

void MainWindow::on_paintPsiButton_clicked()
{
    glPainter->setPaintAim(Psi);
}

void MainWindow::on_paintOmegaButton_clicked()
{
    glPainter->setPaintAim(Omega);
}

void MainWindow::on_paintVxButton_clicked()
{
    glPainter->setPaintAim(Vx);
}

void MainWindow::on_paintVyButton_clicked()
{
    glPainter->setPaintAim(Vy);
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
    delete aboutWindow;
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

void MainWindow::showSettingsWindow()
{
    this->settingsWindow->show();
}

void MainWindow::showHelpWindow()
{
    this->helpWindow->show();
}

//============================================================================================================
//Сохранение изображения функции в файл.
//============================================================================================================
void MainWindow::on_saveImageButton_clicked()
{
    auto settings = SettingsManager::instance().settings();

    if(settings.paintEngine == PaintEngine::OpenGL)
    {
        QPixmap glPixmap = glPainter->grab();
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
