//
//  AudioCreator.cpp
//  
//
//  Created by Sarah Denoux on 15/07/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

//This class is a SINGLETON (as described in DESIGN PATTERNS)
//
//The goal of this class is to control the type of audio architecture used in the application. Therefore it creates the right type of audioFactory

#include "AudioCreator.h"
#include "AudioSettings.h"
#include "AudioManager.h"
#include "AudioFactory.h"
#include "FLSettings.h"

#ifdef COREAUDIO
	#include "CA_audioSettings.h"
    #include "CA_audioFactory.h"
#endif

#ifdef PORTAUDIO
    #include "PA_audioFactory.h"
#endif

#ifdef JACK
    #include "JA_audioFactory.h"
#endif

#ifdef NETJACK
    #include "NJs_audioFactory.h"
  //  #include "NJm_audioFactory.h"
#endif

#ifdef ALSA
    #include "AL_audioFactory.h"
#endif

#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

enum audioArchi {
   
#ifdef COREAUDIO
    kCoreaudio, 
#endif
#ifdef PORTAUDIO
    kPortaudio,
#endif
#ifdef JACK
    kJackaudio,
#endif
#ifdef NETJACK
    kNetjackaudio,
#endif
#ifdef ALSA
    kAlsaaudio
#endif
};

AudioCreator* AudioCreator::_instance = 0;

AudioCreator::AudioCreator(QGroupBox* parent) : QObject(NULL)
{
    fMenu = parent;
    fLayout = new QFormLayout;
    fAudioArchi = new QComboBox(fMenu);
    
//Conditionnal compilation | the options are disabled when not chosen as qmake options
#ifdef COREAUDIO
    fAudioArchi->addItem("CoreAudio");
#endif
#ifdef PORTAUDIO
    fAudioArchi->addItem("PortAudio");
#endif
#ifdef JACK
    fAudioArchi->addItem("Jack");
#endif
#ifdef NETJACK
    fAudioArchi->addItem("NetJack");
#endif
#ifdef ALSA
    fAudioArchi->addItem("Alsa");
#endif

    printf("fAudioArchitecture number of items = %i\n", fAudioArchi->count());
    connect(fAudioArchi, SIGNAL(activated(int)), this, SLOT(indexChanged(int)));
    fLayout->addRow(new QLabel("Audio Architecture"), fAudioArchi);
    
    // Initializing current settings
    fFactory = NULL;
    fSettingsBox = NULL;
    fCurrentSettings = NULL;
    savedSettingsToVisualSettings();
    
    // Initialize temporary settings
    fTempAudioIndex = driverNameToIndex(FLSettings::_Instance()->value("General/Audio/DriverName", "").toString());
    fTempBox = new QGroupBox();
    fTempSettings = fFactory->createAudioSettings(fTempBox);
}

//Returns the instance of the audioCreator
AudioCreator* AudioCreator::_Instance(QGroupBox* box)
{
    if (!_instance) {
        _instance = new AudioCreator(box);
    }
    return _instance;
}

AudioCreator::~AudioCreator()
{
    fCurrentSettings->storeVisualSettings();
    
    delete fSettingsBox;
    delete fTempBox;
    delete fCurrentSettings;
    delete fTempSettings;
    delete fFactory;
    delete fAudioArchi;
    delete fLayout;
}

//Parsing ComboBox to find the index corresponding to a Drover Name
int AudioCreator::driverNameToIndex(const QString& driverName)
{
    // In case compilation options have changed, it is checked if it still exists
    for (int i = 0; i < fAudioArchi->count(); i++) {
        if (driverName == fAudioArchi->itemText(i)) {
            return i;
        }
    }
    return 0;
}

//Dynamic change when the audio index (= audio architecture) changes
void AudioCreator::indexChanged(int index)
{
    printf("AudioCreator::indexChanged %d\n", index);
    
    delete fFactory;
    delete fCurrentSettings;
    delete fSettingsBox;
    
    fFactory = createFactory(index);
    fSettingsBox = new QGroupBox;
    fCurrentSettings = fFactory->createAudioSettings(fSettingsBox);
    
    fLayout->addRow(fSettingsBox);
    fMenu->setLayout(fLayout);
}

void AudioCreator::reset_AudioArchitecture()
{
    fAudioArchi->setCurrentIndex(0);
    indexChanged(0);
}

//Creation of the Factory/Settings/Manager depending on audio index
AudioFactory* AudioCreator::createFactory(int index)
{
    switch(index) {
#ifdef COREAUDIO
        case kCoreaudio:
            return new CA_audioFactory();
            break;
#endif
#ifdef PORTAUDIO
        case kPortaudio:
            return new PA_audioFactory();
            break;
#endif  
#ifdef JACK
        case kJackaudio:
            return new JA_audioFactory();
            break;
#endif
#ifdef NETJACK
        case kNetjackaudio:
            return new NJs_audioFactory();
            break;
#endif  
#ifdef ALSA
        case kAlsaaudio:
            return new AL_audioFactory();
            break;
#endif  
        default:
            return NULL;
    }
}

AudioSettings* AudioCreator::createAudioSettings(QGroupBox* parent)
{
    return fFactory->createAudioSettings(parent);
}

AudioManager* AudioCreator::createAudioManager(AudioShutdownCallback cb, void* arg)
{
    return fFactory->createAudioManager(cb, arg);
}

//Accessors to the Settings
QString AudioCreator::get_ArchiName()
{
    return fCurrentSettings->get_ArchiName();
}

//Does the visual parameters concord to the stored settings?
//Determines if the audio has to be reloaded
bool AudioCreator::didSettingChanged()
{
    
    if (driverNameToIndex(FLSettings::_Instance()->value("General/Audio/DriverName", "").toString()) != fAudioArchi->currentIndex()) {
        return true;
    } else {
        
        printf("Current Settings = %p || Temp = %p\n", fCurrentSettings, fTempSettings);
        
        if (!((*fCurrentSettings)==(*fTempSettings))) {
            return true;
        } else {
            
            // Not really the right thing to do with the actual system but JA settings don't influence audio updates 
            // so it's directly stored --> to avoid update for just going from connect to disconnect or the other way around...
            visualSettingsToTempSettings();
            tempSettingsToSavedSettings();
            return false;
        }
    }
}

// Restoring saved Settings
void AudioCreator::savedSettingsToVisualSettings()
{
    int index = driverNameToIndex(FLSettings::_Instance()->value("General/Audio/DriverName", "").toString());
    fAudioArchi->setCurrentIndex(index);     
    indexChanged(index);
}

//Storing temporarily the settings to test them
void AudioCreator::visualSettingsToTempSettings()
{
    fTempAudioIndex = driverNameToIndex(FLSettings::_Instance()->value("General/Audio/DriverName", "").toString());
    FLSettings::_Instance()->setValue("General/Audio/DriverName", fAudioArchi->currentText());
    fCurrentSettings->storeVisualSettings();
}

//Store temporary settings
void AudioCreator::tempSettingsToSavedSettings()
{
    delete fTempBox;
    fTempBox = new QGroupBox();
    fTempSettings = fFactory->createAudioSettings(fTempBox);
    // Synchronize saved settings & current settings in case of changes (like buffer size...)
    fCurrentSettings->setVisualSettings();
}

//Restoring settings
void AudioCreator::restoreSavedSettings()
{
    FLSettings::_Instance()->setValue("General/Audio/DriverName", fAudioArchi->itemText(fTempAudioIndex));
    fAudioArchi->setCurrentIndex(fTempAudioIndex);
    indexChanged(fTempAudioIndex);
    fTempSettings->storeVisualSettings();
}




