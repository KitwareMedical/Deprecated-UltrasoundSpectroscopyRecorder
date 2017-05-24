/*=========================================================================

Library:   UltrasoundSpectroscopyRecorder

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#pragma once

#ifndef MYWINDOW_HXX
#define MYWINDOW_HXX

#include <QMainWindow>
#include <QTimer>
#include <QCheckBox>
#include <qgroupbox.h>

#include "vtkIntersonSDKCxxVideoSourceWindow.h"
#include "vtksys/CommandLineArguments.hxx"

//Forward declaration of Ui::MainWindow;
namespace Ui{
	class MainWindow;
}

//Declaration of myWindow
class myWindow : public QMainWindow
{
	Q_OBJECT

public:
  myWindow(QWidget *parent = nullptr);
  ~myWindow();

  void SetIntersonDevice(vtkIntersonSDKCxxVideoSourceWindow* source);

protected slots:
  /** Quit the application */
  void ActionQuit();
  /** Start the application */
  void ActionStart();
  /** View the B-mode images without recording */
  void ActionDisplay();
  /** Update the images displayed from the probe */
  void UpdateImage();
  /** Slot handling stop button click (Record button becomes Stop after clicking) */
  void Stop();

  void SetPulseMin();
  void SetPulseMax();
  void SetPulseStep();
  void SetFrequencyMHz( double frequency );

  void SetOutputFolder( QString outputFolder );
  QString GetOutputFolder() const;


protected:
  void DeconnectConnect();
  int GetPulseMin() const;
  int GetPulseMax() const;
  int GetPulseStep() const;
  double GetFrequency() const;
  void AddTrackedFramesToList();
  void SaveTrackedFrames();
  void WriteToFile(const QString& aFilename, vtkPlusTrackedFrameList* currentFrame );

private:
  /** Layout for the Window */
  Ui::MainWindow *ui;
  vtkSmartPointer<vtkIntersonSDKCxxVideoSourceWindow> IntersonDeviceWindow;
  QTimer *timer;
  bool start; // = true if we start a new record
  bool record; // = true if we record the images, false if we only display them
  vtkPlusTrackedFrameList* recordedFrames;
  QString outputFolder;
  int pulseValue;
  bool failRestart; // = true if the program is waiting for more than 5 second whithout receiving another image; in that case we stop and restart the probe.
  unsigned int timerHits;
};

#endif