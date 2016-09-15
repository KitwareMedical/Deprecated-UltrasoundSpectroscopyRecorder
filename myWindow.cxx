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

#include "myWindow.hxx"
#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"

#include "ui_interson_test.h"

#include <iostream>
#include <QtGui>

#include "PlusTrackedFrame.h"
#include "vtkPlusTrackedFrameList.h"
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkPlusBuffer.h>
#include "vtkPlusSequenceIO.h"
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <QVTKWidget.h>
#include <QFileDialog>
#include <vtkInteractorStyleImage.h>

myWindow::myWindow(QWidget *parent)
	:QMainWindow(parent), ui(new Ui::MainWindow), recordedFrames(NULL), restart(TRUE)
{
	LOG_INFO("Generate the new Window");

	//Setup the graphical layout on this current Widget
	ui->setupUi(this);

	this->setWindowTitle("Ultrasound Spectroscopy Recorder");

	//Links buttons and actions
	connect(ui->pushButton_quit, SIGNAL(clicked()), this, SLOT(ActionQuit()));
	connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(ActionStart()));
	connect(ui->pushButton_display, SIGNAL(clicked()), this, SLOT(ActionDisplay()));

	connect(ui->pulseMin, SIGNAL(valueChanged(int)), this, SLOT(SetPulseMin()));
	connect(ui->pulseMax, SIGNAL(valueChanged(int)), this, SLOT(SetPulseMax()));
	connect(ui->pulseStep, SIGNAL(valueChanged(int)), this, SLOT(SetPulseStep()));

	ui->pushButton_start->setIcon(QPixmap("../UltrasoundSpectroscopyRecorder/Resources/icon_Record.png"));
	//ui->pushButton_start->setFocus();

	// Create tracked frame list
	recordedFrames = vtkPlusTrackedFrameList::New();
	recordedFrames->SetValidationRequirements(REQUIRE_UNIQUE_TIMESTAMP);

	// Timer
	this->timer = new QTimer(this);
	this->timer->setSingleShot(true);
}

myWindow::~myWindow()
{
	LOG_INFO("Destructor of myWindow is called");
	delete ui;
}

void myWindow::ActionDisplay()
{
	if (IntersonDeviceWindow->Connect() != PLUS_SUCCESS)
	{
		LOG_ERROR("Unable to connect to Interson Probe");
		exit(EXIT_FAILURE);
	}
	this->record = false;

	LOG_INFO("The frequency is set to 2.5MHz.");
	SetFrequency2_5MHz();

	//Set the values of the probe for the display
	if (GetPulseMin() == 0)
	{
		LOG_INFO("The pulse is set by default to 20V.");
		this->IntersonDeviceWindow->SetPulseMin(20);
	}
	this->IntersonDeviceWindow->SetProbeFrequencyMhz(GetFrequency());
	this->IntersonDeviceWindow->SetPulseVoltage(GetPulseMin());
	this->IntersonDeviceWindow->StartRecording(); // start recording frames for the video

	// Show the live ultrasound image in a VTK renderer window
	vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
	viewer->SetInputConnection(this->IntersonDeviceWindow->GetOutputPort());   //set image to the render and window
	viewer->SetColorWindow(255);
	viewer->SetColorLevel(127.5);
	viewer->SetRenderWindow(ui->vtkRenderer->GetRenderWindow()); //the main thing!
	viewer->Render();

	this->timer->setInterval(50);
	this->connect(timer, SIGNAL(timeout()), SLOT(UpdateImage()));
	this->timer->start();
}

void myWindow::ActionStart()
{
	LOG_INFO("Checking of the frequency and pulse values.");
	SetFrequency2_5MHz();

	if (GetPulseMin() == 0 || GetPulseMax() == 0)
	{
		LOG_ERROR("The values must be strictly positive");
		return;
	}
	if (GetPulseMax() < GetPulseMin())
	{
		LOG_ERROR("Error with the input arguments of the pulse : the minimum is bigger than the maximum.");
		return;
	}
	else if (GetPulseMax() != GetPulseMin())
	{
		if (GetPulseStep() > (GetPulseMax() - GetPulseMin()) || GetPulseStep() == 0)
		{
			LOG_ERROR("Error with the input arguments of the pulse : the step is too big or equal to 0.");
			return;
		}
	}

	LOG_INFO("Start Interson program.");

	// Change the function to be invoked on clicking on the now Stop button
	ui->pushButton_start->setText("Stop");
	ui->pushButton_start->setIcon(QPixmap("../UltrasoundSpectroscopyRecorder/Resources/icon_Stop.png"));
	ui->pushButton_start->setFocus();
	disconnect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(ActionStart()));
	connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(Stop()));

	if (IntersonDeviceWindow->Connect() != PLUS_SUCCESS)
	{
		LOG_ERROR("Unable to connect to Interson Probe");
		exit(EXIT_FAILURE);
	}
	this->record = true;
	this-> restart = true;

	// Set the initial default values
	this->SetPulseMin();
	this->SetPulseMax();
	this->SetPulseStep();
	this->IntersonDeviceWindow->SetProbeFrequencyMhz(IntersonDeviceWindow->GetFrequency());
	this->IntersonDeviceWindow->SetPulseVoltage(IntersonDeviceWindow->GetPulseMin());

	this->IntersonDeviceWindow->StartRecording(); // start recording frames for the video

	// Show the live ultrasound image in a VTK renderer window
	vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
	viewer->SetInputConnection(this->IntersonDeviceWindow->GetOutputPort());   //set image to the render and window
	viewer->SetColorWindow(255);
	viewer->SetColorLevel(127.5);
	viewer->SetRenderWindow(ui->vtkRenderer->GetRenderWindow()); //the main thing!
	viewer->Render();

	this->timer->setInterval(50);
	this->connect(timer, SIGNAL(timeout()), SLOT(UpdateImage()));
	this->timer->start();
}

void myWindow::UpdateImage()
{
	unsigned long frameNumber = IntersonDeviceWindow->GetFrameNumber();
	ui->vtkRenderer->GetRenderWindow()->Render();
	static int pulseValue = GetPulseMin();
	static unsigned int previousFrame = 0;

	if (record == true)
	{
		if (restart == true) // start the recording (initialize the pulse value)
		{
			pulseValue = GetPulseMin();
			previousFrame += 1;
		}
		restart = false;

		if (frameNumber != 0 && frameNumber > previousFrame)
		{
			previousFrame = frameNumber;

			std::string frequency = std::to_string(GetFrequency());
			std::replace(frequency.begin(), frequency.end(), '.', '_');
			frequency.erase(3);
			std::string outputFolder = "C:\\Results";
			std::string path;
			if (CreateDirectory(outputFolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) // create the folder C:\Results if it doesn't exist
			{
				path = outputFolder + "\\VideoBufferMetafile_Rfmode-" + frequency + "MHz-" + std::to_string(pulseValue) + "V-";
			}
			else
			{
				LOG_ERROR("Failed to create the directory C:\Results");
				return;
			}
			IntersonDeviceWindow->SetOutputVideoBufferSequenceFileNameRfmode(path);

			AddTrackedFramesToList();
			SaveTrackedFrames();
			IntersonDeviceWindow->StopRecording();

			// Update the pulse and the frequency of the probe
			if (pulseValue >= this->GetPulseMax() && GetFrequency() >= 5.0)
			{
				this->timer->stop();
				IntersonDeviceWindow->Disconnect();
				return;
			}
			else
			{
				if (pulseValue >= GetPulseMax())
				{
					pulseValue = GetPulseMin();
					if (GetFrequency() == 2.5)
					{
						SetFrequency3_5MHz();
					}
					else if (GetFrequency() == 3.5)
					{
						SetFrequency5MHz();
					}
					IntersonDeviceWindow->SetProbeFrequencyMhz(GetFrequency());
				}
				else
				{
					pulseValue += GetPulseStep();
					if (pulseValue > GetPulseMax())
					{
						pulseValue = GetPulseMax();
					}
				}
				IntersonDeviceWindow->SetPulseVoltage(pulseValue);
			}

			this->IntersonDeviceWindow->StartRecording();
			this->timer->setInterval(10);
		}
	}
	this->timer->start();
	ui->vtkRenderer->GetRenderWindow()->Render();
}

void myWindow::Stop()
{
	LOG_INFO("Recording stopped");
	ui->pushButton_start->setText("Start");
	ui->pushButton_start->setIcon(QPixmap("../UltrasoundSpectroscopyRecorder/Resources/icon_Record.png"));
	//ui->pushButton_start->setFocus();
	disconnect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(Stop()));
	connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(ActionStart()));
	this->timer->stop();
}

void myWindow::ActionQuit()
{
	close();

	this->IntersonDeviceWindow->StopRecording();
	this->IntersonDeviceWindow->Disconnect();
}

void myWindow::SetIntersonDevice(vtkIntersonSDKCxxVideoSourceWindow* source)
{
	this->IntersonDeviceWindow = source;
    //get the values from the .ui and set them to internal IntersonDeviceWindow
    SetPulseMin();
    SetPulseMax();
    SetPulseStep();
}

void myWindow::SetPulseMin()
{
	this->IntersonDeviceWindow->SetPulseMin(this->ui->pulseMin->value());
}

void myWindow::SetPulseMax()
{
	this->IntersonDeviceWindow->SetPulseMax(this->ui->pulseMax->value());
}

void myWindow::SetPulseStep()
{
	this->IntersonDeviceWindow->SetPulseStep(this->ui->pulseStep->value());
}

int myWindow::GetPulseMin() const
{
	//return this->ui->pulseMin->value();
	return this->IntersonDeviceWindow->GetPulseMin();
}

int myWindow::GetPulseMax() const
{
	return this->ui->pulseMax->value();
}

int myWindow::GetPulseStep() const
{
	return this->ui->pulseStep->value();
}

double myWindow::GetFrequency() const
{
	return this->IntersonDeviceWindow->GetFrequency();
}

void myWindow::SetFrequency2_5MHz()
{
	this->IntersonDeviceWindow->SetFrequency(2.5);
}

void myWindow::SetFrequency3_5MHz()
{
	this->IntersonDeviceWindow->SetFrequency(3.5);
}

void myWindow::SetFrequency5MHz()
{
	this->IntersonDeviceWindow->SetFrequency(5);
}

void myWindow::AddTrackedFramesToList()
{
	PlusTrackedFrame trackedFrame;
	vtkPlusChannel* aChannel_Rf(NULL);

	if (IntersonDeviceWindow->GetOutputChannelByName(aChannel_Rf, "RfVideoStream") != PLUS_SUCCESS || aChannel_Rf == NULL || aChannel_Rf->GetTrackedFrame(trackedFrame) != PLUS_SUCCESS)
	{
		LOG_ERROR("Failed to get tracked frame for the record of multi frequencies and pulses!");
		return;
	}
	// Check if there are any valid transforms
	std::vector<PlusTransformName> transformNames;
	trackedFrame.GetCustomFrameTransformNameList(transformNames);
	bool validFrame = false;
	if (transformNames.size() == 0)
	{
		validFrame = true;
	}
	else
	{
		for (std::vector<PlusTransformName>::iterator it = transformNames.begin(); it != transformNames.end(); ++it)
		{
			TrackedFrameFieldStatus status = FIELD_INVALID;
			trackedFrame.GetCustomFrameTransformStatus(*it, status);
			if (status == FIELD_OK)
			{
				validFrame = true;
				break;
			}
		}
	}
	if (!validFrame)
	{
		LOG_WARNING("Unable to record tracked frame: All the tool transforms are invalid!");
		return;
	}

	// Add tracked frame to the list
	if (recordedFrames->AddTrackedFrame(&trackedFrame, vtkPlusTrackedFrameList::SKIP_INVALID_FRAME) != PLUS_SUCCESS)
	{
		LOG_WARNING("Frame could not be added because validation failed!");
		return;
	}
}

void myWindow::SaveTrackedFrames()
{
	//Save
	std::string defaultFileName = this->IntersonDeviceWindow->GetOutputVideoBufferSequenceFileNameRfmode() + vtksys::SystemTools::GetCurrentDateTime("%Y%m%d_%H%M%S") + ".nrrd";
	if (recordedFrames->GetNumberOfTrackedFrames() > 0)
	{
		WriteToFile(QString(defaultFileName.c_str()));
		LOG_INFO("Captured tracked frame list saved into '" << defaultFileName << "'");
		recordedFrames->Clear();
	}
	else
	{
		LOG_ERROR("Impossible to capture the tracked frame list : Number of tracked frames <= 0");
	}
}


void myWindow::WriteToFile(const QString& aFilename)
{
	if (aFilename.isEmpty())
	{
		LOG_ERROR("Writing sequence to metafile failed: output file name is empty");
		return;
	}

	QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

	// Actual saving
	if (vtkPlusSequenceIO::Write(aFilename.toLatin1().constData(), recordedFrames, US_IMG_ORIENT_FM) != PLUS_SUCCESS)
	{
		LOG_ERROR("Failed to save tracked frames to sequence metafile!");
		return;
	}

	QString result = "File saved to\n" + aFilename;

	recordedFrames->Clear();

	QApplication::restoreOverrideCursor();
}