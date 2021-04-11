#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtXml>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hBoxLayout = new QHBoxLayout();
    hBoxLayoutIntensitySelectView = new QHBoxLayout();
    hBoxLayoutColorSelectView = new QHBoxLayout();

    m_CurrentCursorPosition = 0;

    m_SearchString = "";

    InitializeByteArrayToZero();

    m_Mode = DEFAULT;
    m_Fader1Value = 0;
    m_Fader2Value = 0;
    m_Fader3Value = 0;
    m_Fader4Value = 0;
    m_Fader5Value = 0;
    m_Fader6Value = 0;
    m_Fader7Value = 0;
    m_Fader8Value = 0;

    ui->verticalSlider->setTracking(false);
    ui->verticalSlider_2->setTracking(false);
    ui->verticalSlider_3->setTracking(false);
    ui->verticalSlider_4->setTracking(false);
    ui->verticalSlider_5->setTracking(false);
    ui->verticalSlider_6->setTracking(false);
    ui->verticalSlider_7->setTracking(false);
    ui->verticalSlider_8->setTracking(false);

    ui->verticalSlider->setRange(0, 255);
    ui->verticalSlider_2->setRange(0, 255);
    ui->verticalSlider_3->setRange(0, 255);
    ui->verticalSlider_4->setRange(0, 255);
    ui->verticalSlider_5->setRange(0, 255);
    ui->verticalSlider_6->setRange(0, 255);
    ui->verticalSlider_7->setRange(0, 255);
    ui->verticalSlider_8->setRange(0, 255);
    //ui->scrollArea->setLayout(hBoxLayout);
    //m_BoundLights = new std::list<LightBinding>();
    m_selectedLightColorSelectButton = "0";
    parser = new XMLParser("Lighting.xml");
    // Parse the XML and configure Patch setup page
    //XMLParser parser("Lighting.xml");
    parser->InitializeXMLFile();

    m_BoundLights = parser->ParseXML();

    signalMapper = new QSignalMapper(this);
    CreateButtonsPatchView();
}

void MainWindow::InitializeByteArrayToZero()
{
    for(int i = 0; i < 512; i++)
    {
        m_DmxByteArray[i] =  0;
    }
}

void MainWindow::ConstructDmxByteArray()
{
    // Reset the byte array
    InitializeByteArrayToZero();

    std::list<LightBinding>::iterator it = m_BoundLights.begin();
    for(; it != m_BoundLights.end(); ++it)
    {
        std::string strLightNumber = it->GetLightName();

        int lightNumber = std::stoi(strLightNumber);

        int arrayIndex = ((lightNumber - 1) * 4); // Configure 4 channels per fixture Channel 0 - Red, Channel 1 - Green, Channel 2 - Blue, Channel 3 - Intensity

        if(arrayIndex >= 512)
            continue;

        // Update DMX Byte array
        m_DmxByteArray[arrayIndex] = (unsigned char)it->GetRed();
        m_DmxByteArray[arrayIndex + 1] = (unsigned char)it->GetGreen();
        m_DmxByteArray[arrayIndex + 2] = (unsigned char)it->GetBlue();
        m_DmxByteArray[arrayIndex + 3] = (unsigned char)it->GetIntensity();

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_9_clicked()
{
    // Set the mode
    m_Mode = PATCH_SETUP;
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_pushButton_4_clicked()
{
    // Set the mode
    m_Mode = PATCH_SETUP;
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_pushButton_7_clicked()
{
    // Set the mode
    m_Mode = DEFAULT;

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_pushButton_13_clicked()
{
    this->close();
}

void MainWindow::CreateButtonsIntensitySelectView()
{
    while(!m_patchedButtonsIntensitySelect.empty())
    {
        m_patchedButtonsIntensitySelect.begin()->second->deleteLater();
        m_patchedButtonsIntensitySelect.erase(m_patchedButtonsIntensitySelect.begin());
    }

    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        QPushButton *button = new QPushButton();
        const QSize btnSize = QSize(91,61);
        button->setStyleSheet("background: white");
        button->setFixedSize(btnSize);
        button->setText("Light " + QString::fromUtf8(it->GetLightName().c_str()) + QString("\n") + "Fader " + QString::fromUtf8(it->GetFaderName().c_str()));
        m_patchedButtonsIntensitySelect.insert({it->GetLightName(), button});
        hBoxLayoutIntensitySelectView->addWidget(button);
        button->show();
    }

    ui->scrollAreaWidgetContents_3->setLayout(hBoxLayoutIntensitySelectView);
}
void MainWindow::CreateButtonsPatchView()
{
    //Erase existing buttons
    while(!m_patchedButtons.empty())
    {
        m_patchedButtons.begin()->second->deleteLater();
        m_patchedButtons.erase(m_patchedButtons.begin());
    }

    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        QPushButton *button = new QPushButton();
        const QSize btnSize = QSize(91, 61);
        button->setStyleSheet("background: white");
        button->setFixedSize(btnSize);
        button->setText("Light " + QString::fromUtf8(it->GetLightName().c_str()) + QString("\n") + "Fader " + QString::fromUtf8(it->GetFaderName().c_str()));
        m_patchedButtons.insert({it->GetLightName(), button});

        hBoxLayout->addWidget(button);
        button->show();
    }
    ui->scrollAreaWidgetContents->setLayout(hBoxLayout);

}

void MainWindow::on_pushButton_14_clicked()
{
    std::string light = "1";
    bool ok;
    QString text = QInputDialog::getText(0, "Input dialog", "Fader:", QLineEdit::Normal, "", &ok);

    if(text.isEmpty() || text.isNull())
    {
        // Empty string do not do anything
        return;
    }
    std::string fader = text.toUtf8().constData();
    int faderInt = 0;
    try {
        faderInt = std::stoi(fader);
    }  catch (...) {
        // not a number. Do nothing
        return;
    }
    if(faderInt <= 0 || faderInt >= 9)
    {
        // Incorrect value entered by user. Do nothing
        return;
    }
    // Check if light already exists in patched lights.
    // If exists update the fader and add to patched list
    bool bLightAlreadyPatched = false;
    std::list<LightBinding>::iterator it = m_BoundLights.begin();
    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == light)
        {
            bLightAlreadyPatched = true;
            it->SetFaderName(fader);
            break;
        }
    }

    if(!bLightAlreadyPatched)
    {
        LightBinding binding(light, fader, FADER_INTENSITY_0);
        m_BoundLights.push_back(binding);
    }

    // Recreate the patch view buttons
    CreateButtonsPatchView();

    // Update the programmed XML file
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_15_clicked()
{
    std::string light = "2";
    bool ok;
    QString text = QInputDialog::getText(0, "Input dialog", "Fader:", QLineEdit::Normal, "", &ok);

    if(text.isEmpty() || text.isNull())
    {
        // Empty string do not do anything
        return;
    }

    std::string fader = text.toUtf8().constData();
    int faderInt = 0;
    try {
        faderInt = std::stoi(fader);
    }  catch (...) {
        // not a number. Do nothing
        return;
    }
    if(faderInt <= 0 || faderInt >= 9)
    {
        // Incorrect value entered by user. Do nothing
        return;
    }
    // Check if light already exists in patched lights.
    // If exists update the fader and add to patched list
    bool bLightAlreadyPatched = false;
    std::list<LightBinding>::iterator it = m_BoundLights.begin();
    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == light)
        {
            bLightAlreadyPatched = true;
            it->SetFaderName(fader);
            break;
        }
    }

    if(!bLightAlreadyPatched)
    {
        LightBinding binding(light, fader, FADER_INTENSITY_0);
        m_BoundLights.push_back(binding);
    }

    // Recreate the patch view buttons
    CreateButtonsPatchView();

    // Update the programmed XML file
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_16_clicked()
{
    std::string light = "3";
    bool ok;
    QString text = QInputDialog::getText(0, "Input dialog", "Fader:", QLineEdit::Normal, "", &ok);

    if(text.isEmpty() || text.isNull())
    {
        // Empty string do not do anything
        return;
    }

    std::string fader = text.toUtf8().constData();
    int faderInt = 0;
    try {
        faderInt = std::stoi(fader);
    }  catch (...) {
        // not a number. Do nothing
        return;
    }
    if(faderInt <= 0 || faderInt >= 9)
    {
        // Incorrect value entered by user. Do nothing
        return;
    }
    // Check if light already exists in patched lights.
    // If exists update the fader and add to patched list
    bool bLightAlreadyPatched = false;
    std::list<LightBinding>::iterator it = m_BoundLights.begin();
    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == light)
        {
            bLightAlreadyPatched = true;
            it->SetFaderName(fader);
            break;
        }
    }

    if(!bLightAlreadyPatched)
    {
        LightBinding binding(light, fader, FADER_INTENSITY_0);
        m_BoundLights.push_back(binding);
    }

    // Recreate the patch view buttons
    CreateButtonsPatchView();

    // Update the programmed XML file
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_17_clicked()
{
    std::string light = "4";
    bool ok;
    QString text = QInputDialog::getText(0, "Input dialog", "Fader:", QLineEdit::Normal, "", &ok);

    if(text.isEmpty() || text.isNull())
    {
        // Empty string do not do anything
        return;
    }

    std::string fader = text.toUtf8().constData();
    int faderInt = 0;
    try {
        faderInt = std::stoi(fader);
    }  catch (...) {
        // not a number. Do nothing
        return;
    }
    if(faderInt <= 0 || faderInt >= 9)
    {
        // Incorrect value entered by user. Do nothing
        return;
    }
    // Check if light already exists in patched lights.
    // If exists update the fader and add to patched list
    bool bLightAlreadyPatched = false;
    std::list<LightBinding>::iterator it = m_BoundLights.begin();
    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == light)
        {
            bLightAlreadyPatched = true;
            it->SetFaderName(fader);
            break;
        }
    }

    if(!bLightAlreadyPatched)
    {
        LightBinding binding(light, fader, FADER_INTENSITY_0);
        m_BoundLights.push_back(binding);
    }

    // Recreate the patch view buttons
    CreateButtonsPatchView();

    // Update the programmed XML file
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_18_clicked()
{
    std::string light = "5";
    bool ok;
    QString text = QInputDialog::getText(0, "Input dialog", "Fader:", QLineEdit::Normal, "", &ok);

    if(text.isEmpty() || text.isNull())
    {
        // Empty string do not do anything
        return;
    }

    std::string fader =text.toUtf8().constData();
    int faderInt = 0;
    try {
        faderInt = std::stoi(fader);
    }  catch (...) {
        // not a number. Do nothing
        return;
    }
    if(faderInt <= 0 || faderInt >= 9)
    {
        // Incorrect value entered by user. Do nothing
        return;
    }
    // Check if light already exists in patched lights.
    // If exists update the fader and add to patched list
    bool bLightAlreadyPatched = false;
    std::list<LightBinding>::iterator it = m_BoundLights.begin();
    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == light)
        {
            bLightAlreadyPatched = true;
            it->SetFaderName(fader);
            break;
        }
    }

    if(!bLightAlreadyPatched)
    {
        LightBinding binding(light, fader, FADER_INTENSITY_0);
        m_BoundLights.push_back(binding);
    }

    // Recreate the patch view buttons
    CreateButtonsPatchView();

    // Update the programmed XML file
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_8_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{
    m_Mode = PROGRAMMING;
    //Create intensity select page buttons before opening page
    CreateButtonsIntensitySelectView();

    ui->stackedWidget_2->setCurrentIndex(2);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_10_clicked()
{
    // Set current mode to programming
    m_Mode = PROGRAMMING;
    //Create intensity select page buttons before opening page
    CreateButtonsIntensitySelectView();

    ui->stackedWidget_2->setCurrentIndex(2);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::CreateButtonsColorSelectView()
{
    while(!m_PatchedButtonsColorSelectView.empty())
    {
        m_PatchedButtonsColorSelectView.begin()->second->deleteLater();
        m_PatchedButtonsColorSelectView.erase(m_PatchedButtonsColorSelectView.begin());
    }

    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        QPushButton *button = new QPushButton();
        const QSize btnSize = QSize(91,61);
        button->setStyleSheet("background: white");
        button->setFixedSize(btnSize);
        button->setText("Light " + QString::fromUtf8(it->GetLightName().c_str()));
        signalMapper->setMapping(button, QString::fromUtf8(it->GetLightName().c_str())); //Map button with name
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));

        m_PatchedButtonsColorSelectView.insert({it->GetLightName(), button});
        hBoxLayoutColorSelectView->addWidget(button);
        button->show();
    }
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(ColorSelectButtonClicked(QString)));
    ui->scrollAreaWidgetContents_2->setLayout(hBoxLayoutColorSelectView);
}

void MainWindow::ColorSelectButtonClicked(QString str)
{
    // Set the current button selected for color selection
    m_selectedLightColorSelectButton = str.toUtf8().constData();

    QMessageBox messageBox;
    messageBox.setText("Button clicked " + str);
    messageBox.exec();
}
void MainWindow::on_pushButton_21_clicked()
{
    // Set the mode
    m_Mode = COLOR_SELECT;

    // Set the buttons for Color select view
    CreateButtonsColorSelectView();

    ui->stackedWidget->setCurrentIndex(3);
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    // Set the mode
    m_Mode = PLAYBACK;

    ui->stackedWidget->setCurrentIndex(4);
    ui->stackedWidget_2->setCurrentIndex(3);
}

void MainWindow::on_pushButton_11_clicked()
{
    // Set the mode
    m_Mode = PLAYBACK;
    ui->stackedWidget->setCurrentIndex(4);
    ui->stackedWidget_2->setCurrentIndex(3);
}

int MainWindow::GetFaderIntensity(std::string fader)
{
    int faderInt = std::stoi(fader);
    switch (faderInt) {
    case 1:
    {
        return m_Fader1Value;
    }
    case 2:
    {
        return m_Fader2Value;
    }
    case 3:
    {
        return m_Fader3Value;
    }
    case 4:
    {
        return m_Fader4Value;
    }
    case 5:
    {
        return m_Fader5Value;
    }
    case 6:
    {
        return m_Fader6Value;
    }
    case 7:
    {
        return m_Fader7Value;
    }
    case 8:
    {
        return m_Fader8Value;
    }
    default: return 0;
    }
}

void MainWindow::on_pushButton_23_clicked()
{
    // Button 1 - Color fe0000
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("fe0000");
            it->SetRed(254);
            it->SetGreen(0);
            it->SetBlue(0);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_24_clicked()
{
    // Button 2 - Color 538234
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("538234");
            it->SetRed(83);
            it->SetGreen(130);
            it->SetBlue(52);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_25_clicked()
{
    // Color Button 3 - Color 01b0f1
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("01b0f1");
            it->SetRed(1);
            it->SetGreen(176);
            it->SetBlue(241);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_26_clicked()
{
    // Color Button 4 - Color ffc000
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("ffc000");
            it->SetRed(255);
            it->SetGreen(192);
            it->SetBlue(0);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_27_clicked()
{
    // Color Button 5 - Color ffff01
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("ffff01");
            it->SetRed(255);
            it->SetGreen(255);
            it->SetBlue(1);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_28_clicked()
{
    // Color Button 6 - Color ed7d31
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("ed7d31");
            it->SetRed(237);
            it->SetGreen(125);
            it->SetBlue(49);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_29_clicked()
{
    // Color Button 7 - Color 92d14f
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("92d14f");
            it->SetRed(146);
            it->SetGreen(209);
            it->SetBlue(79);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_31_clicked()
{
    // Color Button 8 - Color ffffff
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("ffffff");
            it->SetRed(255);
            it->SetGreen(255);
            it->SetBlue(255);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_32_clicked()
{
    // Color Button 9 - Color ff8ad8
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("ff8ad8");
            it->SetRed(255);
            it->SetGreen(138);
            it->SetBlue(216);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_33_clicked()
{
    // Color Button 10 - Color 7030a0
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("7030a0");
            it->SetRed(112);
            it->SetGreen(48);
            it->SetBlue(160);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_34_clicked()
{
    // Color Button 11 - Color 012060
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("012060");
            it->SetRed(1);
            it->SetGreen(32);
            it->SetBlue(96);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_35_clicked()
{
    // Color Button 12 - Color 941100
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("941100");
            it->SetRed(148);
            it->SetGreen(17);
            it->SetBlue(0);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_30_clicked()
{
    // Color Button 13 - Color fbe5d7
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("fbe5d7");
            it->SetRed(251);
            it->SetGreen(229);
            it->SetBlue(215);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_36_clicked()
{
    // Color Button 14 - Color fff2cd
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("fff2cd");
            it->SetRed(255);
            it->SetGreen(242);
            it->SetBlue(205);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_37_clicked()
{
    // Color Button 15 - Color 44546b
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("44546b");
            it->SetRed(68);
            it->SetGreen(84);
            it->SetBlue(107);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_38_clicked()
{
    // Color Button 16 - Color 00feff
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("00feff");
            it->SetRed(0);
            it->SetGreen(254);
            it->SetBlue(255);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_39_clicked()
{
    // Color Button 17 - Color 0071c1
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("0071c1");
            it->SetRed(0);
            it->SetGreen(113);
            it->SetBlue(193);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_40_clicked()
{
    // Color Button 18 - Color 0433ff
    if(m_selectedLightColorSelectButton == "0")
    {
        // No Light selected
        return;
    }

    // Update the database with color for selected light if light is already patched
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetLightName() == m_selectedLightColorSelectButton)
        {
            it->SetColor("0433ff");
            it->SetRed(4);
            it->SetGreen(51);
            it->SetBlue(255);
            it->SetIntensity(GetFaderIntensity(it->GetFaderName()));
            break;
        }
    }

    // Reconstruct the DMX array
    ConstructDmxByteArray();

    // Send DMX signal for the color
    m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
}

void MainWindow::on_pushButton_19_clicked()
{
    ui->verticalSlider->setValue(255);
    ui->verticalSlider_2->setValue(255);
    ui->verticalSlider_3->setValue(255);
    ui->verticalSlider_4->setValue(255);
    ui->verticalSlider_5->setValue(255);
    ui->verticalSlider_6->setValue(255);
    ui->verticalSlider_7->setValue(255);
    ui->verticalSlider_8->setValue(255);
}

void MainWindow::on_pushButton_20_clicked()
{
    ui->verticalSlider->setValue(0);
    ui->verticalSlider_2->setValue(0);
    ui->verticalSlider_3->setValue(0);
    ui->verticalSlider_4->setValue(0);
    ui->verticalSlider_5->setValue(0);
    ui->verticalSlider_6->setValue(0);
    ui->verticalSlider_7->setValue(0);
    ui->verticalSlider_8->setValue(0);
}

void MainWindow::on_pushButton_44_clicked()
{
    // Get value of fader 1
    std::string fader1Value = std::to_string(m_Fader1Value);

    // Save the value of fader 1 into m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "1")
        {
            it->SetFaderIntensity(fader1Value);
        }
    }

    // Update the XML document with new value of fader 1 intensity.
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    m_Fader1Value = value;

    if(m_Mode == COLOR_SELECT || m_Mode == INTENSITY_SELECT || m_Mode == PROGRAMMING)
    {
        // Save the fader intensity to m_BoundLights m_Intensity.
        std::list<LightBinding>::iterator it = m_BoundLights.begin();
        for(; it != m_BoundLights.end(); ++it)
        {
            if(it->GetFaderName() == "1")
            {
                // Set intensity
                it->SetIntensity(value);
            }
        }

        // Send the DMX signal
    }

    if(m_Mode == COLOR_SELECT)
    {
        // Construct DMX array
        ConstructDmxByteArray();

        // Send DMX signal
        m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
    }
    //QMessageBox msgBox;
    //msgBox.setText(QString::fromUtf8(std::to_string(value).c_str()));
    //msgBox.exec();
}

void MainWindow::on_verticalSlider_2_valueChanged(int value)
{
    m_Fader2Value = value;

    if(m_Mode == COLOR_SELECT || m_Mode == INTENSITY_SELECT || m_Mode == PROGRAMMING)
    {
        // Save the fader intensity to m_BoundLights m_Intensity.
        std::list<LightBinding>::iterator it = m_BoundLights.begin();
        for(; it != m_BoundLights.end(); ++it)
        {
            if(it->GetFaderName() == "2")
            {
                // Set intensity
                it->SetIntensity(value);
            }
        }

        // Send the DMX signal
    }

    if(m_Mode == COLOR_SELECT)
    {
        // Construct DMX array
        ConstructDmxByteArray();

        // Send DMX signal
        m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
    }
}

void MainWindow::on_verticalSlider_3_valueChanged(int value)
{
    m_Fader3Value = value;

    if(m_Mode == PROGRAMMING || m_Mode == COLOR_SELECT || m_Mode == INTENSITY_SELECT)
    {
        // Save the fader intensity to m_BoundLights m_Intensity.
        std::list<LightBinding>::iterator it = m_BoundLights.begin();
        for(; it != m_BoundLights.end(); ++it)
        {
            if(it->GetFaderName() == "3")
            {
                // Set intensity
                it->SetIntensity(value);
            }
        }

        // Send the DMX signal
    }

    if(m_Mode == COLOR_SELECT)
    {
        // Construct DMX array
        ConstructDmxByteArray();

        // Send DMX signal
        m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
    }
}

void MainWindow::on_verticalSlider_4_valueChanged(int value)
{
    m_Fader4Value = value;

    if(m_Mode == PROGRAMMING || m_Mode == COLOR_SELECT || m_Mode == INTENSITY_SELECT)
    {
        // Save the fader intensity to m_BoundLights m_Intensity.
        std::list<LightBinding>::iterator it = m_BoundLights.begin();
        for(; it != m_BoundLights.end(); ++it)
        {
            if(it->GetFaderName() == "4")
            {
                // Set intensity
                it->SetIntensity(value);
            }
        }

        // Send the DMX signal
    }

    if(m_Mode == COLOR_SELECT)
    {
        // Construct DMX array
        ConstructDmxByteArray();

        // Send DMX signal
        m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
    }
}

void MainWindow::on_verticalSlider_5_valueChanged(int value)
{
    m_Fader5Value = value;

    if(m_Mode == PROGRAMMING || m_Mode == COLOR_SELECT || m_Mode == INTENSITY_SELECT)
    {
        // Save the fader intensity to m_BoundLights m_Intensity.
        std::list<LightBinding>::iterator it = m_BoundLights.begin();
        for(; it != m_BoundLights.end(); ++it)
        {
            if(it->GetFaderName() == "5")
            {
                // Set intensity
                it->SetIntensity(value);
            }
        }

        // Send the DMX signal
    }

    if(m_Mode == COLOR_SELECT)
    {
        // Construct DMX array
        ConstructDmxByteArray();

        // Send DMX signal
        m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
    }
}

void MainWindow::on_verticalSlider_6_valueChanged(int value)
{
    m_Fader6Value = value;

    if(m_Mode == PROGRAMMING || m_Mode == COLOR_SELECT || m_Mode == INTENSITY_SELECT)
    {
        // Save the fader intensity to m_BoundLights m_Intensity.
        std::list<LightBinding>::iterator it = m_BoundLights.begin();
        for(; it != m_BoundLights.end(); ++it)
        {
            if(it->GetFaderName() == "6")
            {
                // Set intensity
                it->SetIntensity(value);
            }
        }

        // Send the DMX signal
    }

    if(m_Mode == COLOR_SELECT)
    {
        // Construct DMX array
        ConstructDmxByteArray();

        // Send DMX signal
        m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
    }
}

void MainWindow::on_verticalSlider_7_valueChanged(int value)
{
    m_Fader7Value = value;

    if(m_Mode == PROGRAMMING || m_Mode == COLOR_SELECT || m_Mode == INTENSITY_SELECT)
    {
        // Save the fader intensity to m_BoundLights m_Intensity.
        std::list<LightBinding>::iterator it = m_BoundLights.begin();
        for(; it != m_BoundLights.end(); ++it)
        {
            if(it->GetFaderName() == "7")
            {
                // Set intensity
                it->SetIntensity(value);
            }
        }

        // Send the DMX signal
    }

    if(m_Mode == COLOR_SELECT)
    {
        // Construct DMX array
        ConstructDmxByteArray();

        // Send DMX signal
        m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
    }
}

void MainWindow::on_verticalSlider_8_valueChanged(int value)
{
    m_Fader8Value = value;

    if(m_Mode == PROGRAMMING || m_Mode == COLOR_SELECT || m_Mode == INTENSITY_SELECT)
    {
        // Save the fader intensity to m_BoundLights m_Intensity.
        std::list<LightBinding>::iterator it = m_BoundLights.begin();
        for(; it != m_BoundLights.end(); ++it)
        {
            if(it->GetFaderName() == "8")
            {
                // Set intensity
                it->SetIntensity(value);
            }
        }

        // Send the DMX signal
    }

    if(m_Mode == COLOR_SELECT)
    {
        // Construct DMX array
        ConstructDmxByteArray();

        // Send DMX signal
        m_LibOla.write_dmx(UNIVERSE, m_DmxByteArray);
    }
}

void MainWindow::on_pushButton_45_clicked()
{
    // Get value of fader 1
    std::string faderValue = std::to_string(m_Fader2Value);

    // Save the value of fader 1 into m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "2")
        {
            it->SetFaderIntensity(faderValue);
        }
    }

    // Update the XML document with new value of fader 1 intensity.
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_46_clicked()
{
    // Get value of fader 1
    std::string faderValue = std::to_string(m_Fader3Value);

    // Save the value of fader 1 into m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "3")
        {
            it->SetFaderIntensity(faderValue);
        }
    }

    // Update the XML document with new value of fader 1 intensity.
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_47_clicked()
{
    // Get value of fader 1
    std::string faderValue = std::to_string(m_Fader4Value);

    // Save the value of fader 1 into m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "4")
        {
            it->SetFaderIntensity(faderValue);
        }
    }

    // Update the XML document with new value of fader 1 intensity.
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_48_clicked()
{
    // Get value of fader 1
    std::string faderValue = std::to_string(m_Fader5Value);

    // Save the value of fader 1 into m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "5")
        {
            it->SetFaderIntensity(faderValue);
        }
    }

    // Update the XML document with new value of fader 1 intensity.
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_49_clicked()
{
    // Get value of fader 1
    std::string faderValue = std::to_string(m_Fader6Value);

    // Save the value of fader 1 into m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "6")
        {
            it->SetFaderIntensity(faderValue);
        }
    }

    // Update the XML document with new value of fader 1 intensity.
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_50_clicked()
{
    // Get value of fader 1
    std::string faderValue = std::to_string(m_Fader7Value);

    // Save the value of fader 1 into m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "7")
        {
            it->SetFaderIntensity(faderValue);
        }
    }

    // Update the XML document with new value of fader 1 intensity.
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_51_clicked()
{
    // Get value of fader 1
    std::string faderValue = std::to_string(m_Fader8Value);

    // Save the value of fader 1 into m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "8")
        {
            it->SetFaderIntensity(faderValue);
        }
    }

    // Update the XML document with new value of fader 1 intensity.
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_52_clicked()
{
    // Clear fader value => Set to 0 in m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "1")
        {
            it->SetFaderIntensity("0");
        }
    }

    // Set fader value to 0 using SetValue
    ui->verticalSlider->setValue(0);

    // Update XML file with new m_BoundLights
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_53_clicked()
{
    // Clear fader value => Set to 0 in m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "2")
        {
            it->SetFaderIntensity("0");
        }
    }

    // Set fader value to 0 using SetValue
    ui->verticalSlider->setValue(0);

    // Update XML file with new m_BoundLights
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_54_clicked()
{
    // Clear fader value => Set to 0 in m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "3")
        {
            it->SetFaderIntensity("0");
        }
    }

    // Set fader value to 0 using SetValue
    ui->verticalSlider->setValue(0);

    // Update XML file with new m_BoundLights
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_55_clicked()
{
    // Clear fader value => Set to 0 in m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "4")
        {
            it->SetFaderIntensity("0");
        }
    }

    // Set fader value to 0 using SetValue
    ui->verticalSlider->setValue(0);

    // Update XML file with new m_BoundLights
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_56_clicked()
{
    // Clear fader value => Set to 0 in m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "5")
        {
            it->SetFaderIntensity("0");
        }
    }

    // Set fader value to 0 using SetValue
    ui->verticalSlider->setValue(0);

    // Update XML file with new m_BoundLights
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_57_clicked()
{
    // Clear fader value => Set to 0 in m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "6")
        {
            it->SetFaderIntensity("0");
        }
    }

    // Set fader value to 0 using SetValue
    ui->verticalSlider->setValue(0);

    // Update XML file with new m_BoundLights
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_58_clicked()
{
    // Clear fader value => Set to 0 in m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "7")
        {
            it->SetFaderIntensity("0");
        }
    }

    // Set fader value to 0 using SetValue
    ui->verticalSlider->setValue(0);

    // Update XML file with new m_BoundLights
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_59_clicked()
{
    // Clear fader value => Set to 0 in m_BoundLights
    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        if(it->GetFaderName() == "8")
        {
            it->SetFaderIntensity("0");
        }
    }

    // Set fader value to 0 using SetValue
    ui->verticalSlider->setValue(0);

    // Update XML file with new m_BoundLights
    parser->UpdateXMLFile(m_BoundLights, 8);
}

void MainWindow::on_pushButton_41_clicked()
{
    // Clear all saved lighting states

    // Clear sliders in GUI
    ui->verticalSlider->setValue(0);
    ui->verticalSlider_2->setValue(0);
    ui->verticalSlider_3->setValue(0);
    ui->verticalSlider_4->setValue(0);
    ui->verticalSlider_5->setValue(0);
    ui->verticalSlider_6->setValue(0);
    ui->verticalSlider_7->setValue(0);
    ui->verticalSlider_8->setValue(0);

    // Clear (set to 0) all lighting states in m_BoundLights

    std::list<LightBinding>::iterator it = m_BoundLights.begin();

    for(; it != m_BoundLights.end(); ++it)
    {
        it->SetFaderIntensity("0");
    }

    // Update XML with new m_BoundLights
}

void MainWindow::on_pushButton_42_clicked()
{
    // Set all lights to WHITE and INT 100%

    // Set the RGB value in m_BoundLights
    // Set the faderIntensity in m_BoundLights to 100
    // Update XML with m_BoundLights
}

void MainWindow::on_pushButton_43_clicked()
{
    // Set all lights to INT 0%
}

void MainWindow::on_pushButton_5_clicked()
{
    // Resets all lighting fixtures to default values (no colour, no intensity)

    // Set RGB to 255, 255, 255
    // Set faderIntensity to 0
}

void MainWindow::on_pushButton_60_clicked()
{
    // Show user manual
    ui->stackedWidget->setCurrentIndex(5);

    // Read text file and display in text browser
    QFile file("UserManual.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        ui->textBrowser->setText(in.readAll());
    }
}

void MainWindow::on_pushButton_61_clicked()
{
    // Display Show Support;
    ui->stackedWidget->setCurrentIndex(6);

    // Set the text in textBox
    QFile file("Manual.txt");
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    in.setCodec("UTF-8");


    ui->textEdit->setText(in.readAll());    // Set text editor with browsed text
}

void MainWindow::on_pushButton_3_clicked()
{
    // Set the mode
    m_Mode = DEFAULT;

    ui->stackedWidget_2->setCurrentIndex(4);
}

void MainWindow::on_pushButton_12_clicked()
{
    // Set the mode
    m_Mode = DEFAULT;

    ui->stackedWidget_2->setCurrentIndex(4);
}

void MainWindow::on_pushButton_62_clicked()
{
    // Find button is clicked
    QString searchString = ui->lineEdit->text();
    QTextDocument *document = ui->textEdit->document();

    if(searchString == m_SearchString && searchString != "")
    {
        //m_TextBoxCursors
        ui->textEdit->setTextCursor(m_TextBoxCursors[m_CurrentCursorPosition]);

        int sizeVector = m_TextBoxCursors.size();
        if(m_CurrentCursorPosition == (sizeVector - 1))
        {
            m_CurrentCursorPosition = 0;
        }
        else
        {
            ++m_CurrentCursorPosition;
        }

        return;
    }
    else
    {
        m_TextBoxCursors.clear();
        m_SearchString = searchString;
    }

    bool bFound = false;

    document->undo();

    if(searchString.isEmpty())
    {
        return;
    }

    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    QTextCharFormat plainFormat(highlightCursor.charFormat());
    QTextCharFormat colorFormat = plainFormat;

    //colorFormat.setForeground(Qt::red);

    while(!highlightCursor.isNull() && !highlightCursor.atEnd())
    {
        highlightCursor = document->find(searchString, highlightCursor, QTextDocument::FindWholeWords);
        if(!highlightCursor.isNull())
        {
            bFound = true;
            highlightCursor.movePosition(QTextCursor::WordRight, QTextCursor::KeepAnchor);
            highlightCursor.mergeCharFormat(colorFormat);

            m_TextBoxCursors.push_back(highlightCursor);
            //ui->textEdit->setTextCursor(highlightCursor);
        }
    }

    cursor.endEditBlock();
}

void MainWindow::on_pushButton_22_clicked()
{
    // Set the mode
    m_Mode = INTENSITY_SELECT;
}
