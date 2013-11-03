//
//  audioSettings.h
//  
//
//  Created by Sarah Denoux on 15/07/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

// This class is an abstract description of the Settings specific to an audio architecture.

#ifndef _AudioSettings_h
#define _AudioSettings_h

#include <QtGui>

class AudioSettings : public QObject{

    Q_OBJECT
    
    protected:

        std::string              fSavingFile;
    
    public :
    
        AudioSettings(std::string home, QGroupBox* parent){ Q_UNUSED(home); Q_UNUSED(parent); }
        virtual ~AudioSettings(){}
    
        virtual void readSettings() = 0;
        virtual void writeSettings() = 0;
        virtual void setCurrentSettings() = 0;
        virtual void getCurrentSettings() = 0;
    
        virtual bool isEqual(AudioSettings* as) = 0;
        virtual bool operator==(AudioSettings& as){return isEqual(&as);}
    
        virtual std::string get_ArchiName() = 0;

    protected slots:
        virtual void linkClicked(const QUrl&){}
};

#endif
