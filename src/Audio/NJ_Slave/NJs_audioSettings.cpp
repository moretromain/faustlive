//
//  NJs_audioSettings.cpp
//  
//
//  Created by Sarah Denoux on 15/07/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

// This class describes the Settings of netjack clients : 
//  - Its compression Value
//  - Its IPadress
//  - Its Port
//  - Its Latencty
// All of them are described visually by a QLineEdit

#include "NJs_audioSettings.h"
#include "utilities.h"
#include "FLSettings.h"

#define DEFAULT_PORT        19000
#define DEFAULT_MTU         1500
#define DEFAULT_LATENCY     2

NJs_audioSettings::NJs_audioSettings(QGroupBox* parent) : AudioSettings(parent) 
{
    QFormLayout* layout = new QFormLayout;
    
    fCVLine = new QLineEdit;
    layout->addRow(new QLabel(tr("Compression Value")), fCVLine);
    
    fIPLine = new QLineEdit;
    fIPLine->setInputMask("000.000.000.000");
    layout->addRow(new QLabel(tr("Master IP adress")), fIPLine);
    
    fPortLine = new QLineEdit;
    fPortLine->setInputMask("000000");
    layout->addRow(new QLabel(tr("Master Port")), fPortLine);
    
    fLatLine = new QLineEdit;
    layout->addRow(new QLabel(tr("Latency")), fLatLine);
    
    fMTULine = new QLineEdit;
    layout->addRow(new QLabel(tr("MTU")), fMTULine);
    
    parent->setLayout(layout);
    
    setVisualSettings();
}

NJs_audioSettings::~NJs_audioSettings(){}

//Real Params TO Visual params
void NJs_audioSettings::setVisualSettings()
{
    FLSettings* settings = FLSettings::_Instance();
    
    fCVLine->setText(QString::number(settings->value("General/Audio/NetJackSlave/CV", -1).toInt()));
    fIPLine->setText(settings->value("General/Audio/NetJackSlave/IP", "225.3.19.154").toString());
    fPortLine->setText(QString::number(settings->value("General/Audio/NetJackSlave/Port", DEFAULT_PORT).toInt()));
    fMTULine->setText(QString::number(settings->value("General/Audio/NetJackSlave/MTU", DEFAULT_MTU).toInt()));
    fLatLine->setText(QString::number(settings->value("General/Audio/NetJackSlave/Latency", DEFAULT_LATENCY).toInt()));
}

//Visual Params TO Real params
void NJs_audioSettings::storeVisualSettings()
{
    FLSettings* settings = FLSettings::_Instance();
    
    if (isStringInt(fCVLine->text().toStdString().c_str())) {
        settings->setValue("General/Audio/NetJackSlave/CV", atoi(fCVLine->text().toStdString().c_str()));
    } else {
        fCVLine->setText(QString::number(-1));
        settings->setValue("General/Audio/NetJackSlave/CV", -1);
    }
    
    settings->setValue("General/Audio/NetJackSlave/IP", fIPLine->text());
    
    if (isStringInt(fPortLine->text().toStdString().c_str())) {
        settings->setValue("General/Audio/NetJackSlave/Port", atoi(fPortLine->text().toStdString().c_str()));
    } else {
        fPortLine->setText(QString::number(DEFAULT_PORT));
        settings->setValue("General/Audio/NetJackSlave/Port", DEFAULT_PORT);
    }
    
    if (isStringInt(fMTULine->text().toStdString().c_str())) {
        settings->setValue("General/Audio/NetJackSlave/MTU", atoi(fMTULine->text().toStdString().c_str()));
    } else{
        fMTULine->setText(QString::number(DEFAULT_MTU));
        settings->setValue("General/Audio/NetJackSlave/MTU", DEFAULT_MTU);
    }
    
    if(isStringInt(fLatLine->text().toStdString().c_str())) {
        settings->setValue("General/Audio/NetJackSlave/Latency", atoi(fLatLine->text().toStdString().c_str()));
    } else {
        fLatLine->setText(QString::number(DEFAULT_LATENCY));
        settings->setValue("General/Audio/NetJackSlave/Latency", DEFAULT_LATENCY);
    }
}

//Operator== for NetJack Settings
bool NJs_audioSettings::isEqual(AudioSettings* as)
{
    NJs_audioSettings* settings = dynamic_cast<NJs_audioSettings*>(as);
    return (settings != NULL 
        && settings->get_compressionValue() == get_compressionValue() 
        && settings->get_IP() == get_IP() 
        && settings->get_Port() == get_Port() 
        && settings->get_mtu() == get_mtu() 
        && settings->get_latency() == get_latency());
}

//Accessors to the parameters
int NJs_audioSettings::get_compressionValue()
{
    return (isStringInt(fCVLine->text().toStdString().c_str())) ? atoi(fCVLine->text().toStdString().c_str()) : -1;
}

QString NJs_audioSettings::get_IP()
{
    return fIPLine->text();
}

int NJs_audioSettings::get_Port()
{
    if (isStringInt(fPortLine->text().toStdString().c_str())) {
        return atoi(fPortLine->text().toStdString().c_str());
    } else {
        return DEFAULT_PORT;
    }
}

int NJs_audioSettings::get_mtu()
{
    if (isStringInt(fMTULine->text().toStdString().c_str())) {
        return atoi(fMTULine->text().toStdString().c_str());
    } else {
        return DEFAULT_MTU;
    }
}

int NJs_audioSettings::get_latency()
{
    if (isStringInt(fLatLine->text().toStdString().c_str())) {
        return atoi(fLatLine->text().toStdString().c_str());
    } else {
        return DEFAULT_LATENCY;
    }
}

QString NJs_audioSettings::get_ArchiName()
{
    return "NetJackSlave";
}



