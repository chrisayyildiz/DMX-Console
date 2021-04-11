#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <list>
#include <map>
#include <vector>
#include "lightbinding.h"
#include "xmlparser.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <libola_ola.h>

#define FADER_INTENSITY_0 "0"
#define UNIVERSE 1
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum Mode {
    DEFAULT,
    PATCH_SETUP,
    PROGRAMMING,
    PLAYBACK,
    COLOR_SELECT,
    INTENSITY_SELECT
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_9_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_11_clicked();

    void ColorSelectButtonClicked(QString str);
    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_44_clicked();

    void on_verticalSlider_valueChanged(int value);

    void on_verticalSlider_2_valueChanged(int value);

    void on_verticalSlider_3_valueChanged(int value);

    void on_verticalSlider_4_valueChanged(int value);

    void on_verticalSlider_5_valueChanged(int value);

    void on_verticalSlider_6_valueChanged(int value);

    void on_verticalSlider_7_valueChanged(int value);

    void on_verticalSlider_8_valueChanged(int value);

    void on_pushButton_45_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_56_clicked();

    void on_pushButton_57_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_59_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_60_clicked();

    void on_pushButton_61_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_62_clicked();

    void on_pushButton_22_clicked();

private:
    Ui::MainWindow *ui;

    std::list<LightBinding> m_BoundLights;
    std::map<std::string, QPushButton*> m_patchedButtons;
    std::map<std::string, QPushButton*> m_patchedButtonsIntensitySelect;
    std::map<std::string, QPushButton*> m_PatchedButtonsColorSelectView;
    QHBoxLayout *hBoxLayout;
    QHBoxLayout *hBoxLayoutIntensitySelectView;
    QHBoxLayout *hBoxLayoutColorSelectView;
    void CreateButtonsPatchView();
    void CreateButtonsIntensitySelectView();
    void CreateButtonsColorSelectView();
    XMLParser* parser;

    std::string m_selectedLightColorSelectButton;

    QSignalMapper *signalMapper;

    int m_Fader1Value;
    int m_Fader2Value;
    int m_Fader3Value;
    int m_Fader4Value;
    int m_Fader5Value;
    int m_Fader6Value;
    int m_Fader7Value;
    int m_Fader8Value;
    //void ColorSelectButtonClicked(QString str);

    Mode m_Mode;

    int m_CurrentCursorPosition;

    std::vector<QTextCursor> m_TextBoxCursors;

    QString m_SearchString;

    unsigned char m_DmxByteArray[512];
    void InitializeByteArrayToZero();
    void ConstructDmxByteArray();
    int GetFaderIntensity(std::string fader);

    Libola_Ola m_LibOla;
};
#endif // MAINWINDOW_H
